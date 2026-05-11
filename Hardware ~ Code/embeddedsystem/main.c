/*
 * ============================================================================
 *  Smart Temperature & Humidity Monitoring System  —  System 319
 *  ATmega328P @ 16 MHz  |  AHT20  |  SSD1306 128×64 OLED  |  AVR-GCC
 * ============================================================================
 *
 *  Lecturer   : Miss Aimen Najeeb
 *  Group Lead : Muhammad Taha   EE-23319
 *  Members    : Ali      EE-23328
 *               Ahmed    EE-23325
 *               Farzam   EE-23326
 *
 * ============================================================================
 *  HARDWARE CONNECTIONS
 * ============================================================================
 *  SDA       → A4 (PC4)   I2C data   [AHT20 + SSD1306]
 *  SCL       → A5 (PC5)   I2C clock  [AHT20 + SSD1306]
 *  Fan PWM   → D9 (PB1)   OC1A — Timer1 Fast-PWM 8-bit
 *  Buzzer    → D8 (PB0)   Active buzzer   LOW = ON, HIGH = OFF
 *  LED_GRN   → D4 (PD4)   Green  LED  — Normal state
 *  LED_YLW   → D5 (PD5)   Yellow LED  — Medium state
 *  LED_RED   → D6 (PD6)   Red    LED  — High Alert state
 *  BTN_RIGHT → D2 (PD2)   Navigate →  (next screen)
 *  BTN_LEFT  → D3 (PD3)   Navigate ←  (prev screen / info)
 *  BTN_MAIN  → D7 (PD7)   Return to Main screen (System 319)
 *
 *  LED Wiring: Pin → 220Ω → LED(+) → LED(−) → GND
 *  Buttons   : Pin → GND  (internal pull-ups active)
 *
 * ============================================================================
 *  SCREEN FLOW
 * ============================================================================
 *
 *  Power ON →  Slide-in animations:
 *    Anim 1: Project name slides in from right
 *    Anim 2: Lecturer + Group Leader slide in
 *    Anim 3: Group Members slide in
 *    Anim 4: "System 319" + live status
 *
 *  Runtime screens (RIGHT / LEFT cycles, MAIN jumps to screen 0):
 *    Screen 0 — MAIN      : System 319 live readings + status
 *    Screen 1 — INFO LEFT : State info (LEDs, fan, buzzer table)
 *    Screen 2 — TEMP GRAPH: Rolling bar chart — temperature
 *    Screen 3 — HUM  GRAPH: Rolling bar chart — humidity
 *
 * ============================================================================
 *  FSM ALERT STATES  (humidity-driven, with hysteresis)
 * ============================================================================
 *  NORMAL (0) : hum ≤ 30 %   → Fan OFF  | Buzzer OFF  | LED_GRN steady ON
 *  MEDIUM (1) : 30 % < hum ≤ 70 %  → Fan 40 %  | Buzzer intermittent (3 s cycle)
 *                                     | LED_GRN + LED_YLW ON
 *  HIGH   (2) : hum > 70 %   → Fan 90 %  | Buzzer continuous ON
 *                                     | LED_GRN + LED_YLW + LED_RED ON
 *
 *  Hysteresis: ±1.5 % around each threshold prevents rapid toggling.
 *
 * ============================================================================
 *  TIMING
 * ============================================================================
 *  1 tick = 100 ms  (poll loop granularity)
 *  Sensor read every 20 ticks ≈ 2 s
 *  Buzzer Medium: ON 300 ms (3 ticks) / OFF 2700 ms (27 ticks) per 30-tick cycle
 *  LED blink (Medium) : 500 ms toggle
 *  LED blink (High)   : 200 ms toggle  ← steady-on per spec, but kept as option
 * ============================================================================
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdint.h>
#include <string.h>

/* ============================================================================
 *  §1  I2C (TWI) DRIVER — 400 kHz, blocking, no interrupts
 * ============================================================================ */

static void i2c_init(void)
{
    /* TWBR = (F_CPU/SCL − 16) / (2 × prescaler);  prescaler = 1 */
    TWBR = (uint8_t)(((F_CPU / 400000UL) - 16UL) / 2UL);
    TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
    TWCR  = (1 << TWEN);
}

static uint8_t i2c_start(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    uint8_t s = TWSR & 0xF8;
    return (s == 0x08 || s == 0x10);   /* START or REP_START */
}

static void i2c_stop(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

static uint8_t i2c_write(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    uint8_t s = TWSR & 0xF8;
    return (s == 0x18 || s == 0x28);   /* SLA+W ACK or DATA ACK */
}

static uint8_t i2c_read(uint8_t send_ack)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (send_ack ? (1 << TWEA) : 0);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

/* ============================================================================
 *  §2  AHT20 SENSOR DRIVER
 *  Output: temp_c10 (°C×10), hum_p10 (% ×10)
 *  Example: temp_c10=253 → 25.3 °C;  hum_p10=456 → 45.6 %
 * ============================================================================ */

#define AHT20_W  0x70   /* (0x38 << 1) | 0 */
#define AHT20_R  0x71   /* (0x38 << 1) | 1 */

static void aht20_init(void)
{
    _delay_ms(40);
    i2c_start();
    i2c_write(AHT20_W);
    i2c_write(0xBE);    /* soft-reset / init */
    i2c_write(0x08);
    i2c_write(0x00);
    i2c_stop();
    _delay_ms(10);
}

static void aht20_read(int16_t *tc10, uint16_t *hp10)
{
    uint8_t b[6];

    /* Trigger measurement */
    i2c_start(); i2c_write(AHT20_W);
    i2c_write(0xAC); i2c_write(0x33); i2c_write(0x00);
    i2c_stop();
    _delay_ms(80);

    /* Read 6 bytes */
    i2c_start(); i2c_write(AHT20_R);
    b[0]=i2c_read(1); b[1]=i2c_read(1); b[2]=i2c_read(1);
    b[3]=i2c_read(1); b[4]=i2c_read(1); b[5]=i2c_read(0);
    i2c_stop();

    uint32_t H = ((uint32_t)b[1]<<12)|((uint32_t)b[2]<<4)|(b[3]>>4);
    uint32_t T = (((uint32_t)b[3]&0x0F)<<16)|((uint32_t)b[4]<<8)|b[5];

    *hp10 = (uint16_t)((H * 1000UL) >> 20);
    *tc10 = (int16_t)((int32_t)((T * 2000UL) >> 20) - 500);
}

/* ============================================================================
 *  §3  SSD1306 OLED DRIVER  (128×64, I2C address 0x3C)
 * ============================================================================ */

#define OLED_W  0x78    /* (0x3C << 1) | 0 */
#define SCR_W   128
#define SCR_PG  8

static void oled_cmd(uint8_t c)
{
    i2c_start(); i2c_write(OLED_W); i2c_write(0x00); i2c_write(c); i2c_stop();
}

static void oled_dat(uint8_t d)
{
    i2c_start(); i2c_write(OLED_W); i2c_write(0x40); i2c_write(d); i2c_stop();
}

static void oled_init(void)
{
    _delay_ms(100);
    static const uint8_t seq[] PROGMEM = {
        0xAE,             /* display OFF */
        0xD5,0x80,        /* clock divide / osc */
        0xA8,0x3F,        /* mux ratio 64 */
        0xD3,0x00,        /* display offset 0 */
        0x40,             /* start line 0 */
        0x8D,0x14,        /* charge pump ON */
        0x20,0x00,        /* horizontal addr mode */
        0xA1,             /* seg remap */
        0xC8,             /* COM scan direction */
        0xDA,0x12,        /* COM pins */
        0x81,0xCF,        /* contrast */
        0xD9,0xF1,        /* pre-charge */
        0xDB,0x40,        /* VCOMH deselect */
        0xA4,             /* RAM display */
        0xA6,             /* normal (non-inverted) */
        0xAF              /* display ON */
    };
    uint8_t i;
    for (i = 0; i < sizeof(seq); i++) oled_cmd(pgm_read_byte(&seq[i]));
}

static void oled_pos(uint8_t page, uint8_t col)
{
    oled_cmd((uint8_t)(0xB0 | (page & 0x07)));
    oled_cmd((uint8_t)(0x00 | (col  & 0x0F)));
    oled_cmd((uint8_t)(0x10 | (col  >> 4)));
}

static void oled_clear(void)
{
    uint8_t p, c;
    for (p = 0; p < SCR_PG; p++) {
        oled_pos(p, 0);
        for (c = 0; c < SCR_W; c++) oled_dat(0x00);
    }
}

static void oled_row(uint8_t page, uint8_t pat)
{
    uint8_t c;
    oled_pos(page, 0);
    for (c = 0; c < SCR_W; c++) oled_dat(pat);
}

static void oled_clr_row(uint8_t page) { oled_row(page, 0x00); }

/* Partial row: fill from col_start to col_end (inclusive) with pat */
static void oled_fill_range(uint8_t page, uint8_t cs, uint8_t ce, uint8_t pat)
{
    uint8_t c;
    oled_pos(page, cs);
    for (c = cs; c <= ce; c++) oled_dat(pat);
}

/* ============================================================================
 *  §4  5×7 PIXEL FONT  (stored in flash)
 * ============================================================================ */

static const uint8_t FONT[][5] PROGMEM = {
    {0x00,0x00,0x00,0x00,0x00}, /* 0x20 ' ' */
    {0x00,0x00,0x5F,0x00,0x00}, /* '!' */
    {0x00,0x07,0x00,0x07,0x00}, /* '"' */
    {0x14,0x7F,0x14,0x7F,0x14}, /* '#' */
    {0x24,0x2A,0x7F,0x2A,0x12}, /* '$' */
    {0x23,0x13,0x08,0x64,0x62}, /* '%' */
    {0x36,0x49,0x55,0x22,0x50}, /* '&' */
    {0x00,0x05,0x03,0x00,0x00}, /* '\'' */
    {0x00,0x1C,0x22,0x41,0x00}, /* '(' */
    {0x00,0x41,0x22,0x1C,0x00}, /* ')' */
    {0x08,0x2A,0x1C,0x2A,0x08}, /* '*' */
    {0x08,0x08,0x3E,0x08,0x08}, /* '+' */
    {0x00,0x50,0x30,0x00,0x00}, /* ',' */
    {0x08,0x08,0x08,0x08,0x08}, /* '-' */
    {0x00,0x60,0x60,0x00,0x00}, /* '.' */
    {0x20,0x10,0x08,0x04,0x02}, /* '/' */
    {0x3E,0x51,0x49,0x45,0x3E}, /* '0' */
    {0x00,0x42,0x7F,0x40,0x00}, /* '1' */
    {0x42,0x61,0x51,0x49,0x46}, /* '2' */
    {0x21,0x41,0x45,0x4B,0x31}, /* '3' */
    {0x18,0x14,0x12,0x7F,0x10}, /* '4' */
    {0x27,0x45,0x45,0x45,0x39}, /* '5' */
    {0x3C,0x4A,0x49,0x49,0x30}, /* '6' */
    {0x01,0x71,0x09,0x05,0x03}, /* '7' */
    {0x36,0x49,0x49,0x49,0x36}, /* '8' */
    {0x06,0x49,0x49,0x29,0x1E}, /* '9' */
    {0x00,0x36,0x36,0x00,0x00}, /* ':' */
    {0x00,0x56,0x36,0x00,0x00}, /* ';' */
    {0x08,0x14,0x22,0x41,0x00}, /* '<' */
    {0x14,0x14,0x14,0x14,0x14}, /* '=' */
    {0x00,0x41,0x22,0x14,0x08}, /* '>' */
    {0x02,0x01,0x51,0x09,0x06}, /* '?' */
    {0x32,0x49,0x79,0x41,0x3E}, /* '@' */
    {0x7E,0x11,0x11,0x11,0x7E}, /* 'A' */
    {0x7F,0x49,0x49,0x49,0x36}, /* 'B' */
    {0x3E,0x41,0x41,0x41,0x22}, /* 'C' */
    {0x7F,0x41,0x41,0x22,0x1C}, /* 'D' */
    {0x7F,0x49,0x49,0x49,0x41}, /* 'E' */
    {0x7F,0x09,0x09,0x09,0x01}, /* 'F' */
    {0x3E,0x41,0x49,0x49,0x7A}, /* 'G' */
    {0x7F,0x08,0x08,0x08,0x7F}, /* 'H' */
    {0x00,0x41,0x7F,0x41,0x00}, /* 'I' */
    {0x20,0x40,0x41,0x3F,0x01}, /* 'J' */
    {0x7F,0x08,0x14,0x22,0x41}, /* 'K' */
    {0x7F,0x40,0x40,0x40,0x40}, /* 'L' */
    {0x7F,0x02,0x04,0x02,0x7F}, /* 'M' */
    {0x7F,0x04,0x08,0x10,0x7F}, /* 'N' */
    {0x3E,0x41,0x41,0x41,0x3E}, /* 'O' */
    {0x7F,0x09,0x09,0x09,0x06}, /* 'P' */
    {0x3E,0x41,0x51,0x21,0x5E}, /* 'Q' */
    {0x7F,0x09,0x19,0x29,0x46}, /* 'R' */
    {0x46,0x49,0x49,0x49,0x31}, /* 'S' */
    {0x01,0x01,0x7F,0x01,0x01}, /* 'T' */
    {0x3F,0x40,0x40,0x40,0x3F}, /* 'U' */
    {0x1F,0x20,0x40,0x20,0x1F}, /* 'V' */
    {0x3F,0x40,0x38,0x40,0x3F}, /* 'W' */
    {0x63,0x14,0x08,0x14,0x63}, /* 'X' */
    {0x07,0x08,0x70,0x08,0x07}, /* 'Y' */
    {0x61,0x51,0x49,0x45,0x43}, /* 'Z' */
    {0x00,0x7F,0x41,0x41,0x00}, /* '[' */
    {0x02,0x04,0x08,0x10,0x20}, /* '\\' */
    {0x00,0x41,0x41,0x7F,0x00}, /* ']' */
    {0x04,0x02,0x01,0x02,0x04}, /* '^' */
    {0x40,0x40,0x40,0x40,0x40}, /* '_' */
    {0x00,0x01,0x02,0x04,0x00}, /* '`' */
    {0x20,0x54,0x54,0x54,0x78}, /* 'a' */
    {0x7F,0x48,0x44,0x44,0x38}, /* 'b' */
    {0x38,0x44,0x44,0x44,0x20}, /* 'c' */
    {0x38,0x44,0x44,0x48,0x7F}, /* 'd' */
    {0x38,0x54,0x54,0x54,0x18}, /* 'e' */
    {0x08,0x7E,0x09,0x01,0x02}, /* 'f' */
    {0x0C,0x52,0x52,0x52,0x3E}, /* 'g' */
    {0x7F,0x08,0x04,0x04,0x78}, /* 'h' */
    {0x00,0x44,0x7D,0x40,0x00}, /* 'i' */
    {0x20,0x40,0x44,0x3D,0x00}, /* 'j' */
    {0x7F,0x10,0x28,0x44,0x00}, /* 'k' */
    {0x00,0x41,0x7F,0x40,0x00}, /* 'l' */
    {0x7C,0x04,0x18,0x04,0x78}, /* 'm' */
    {0x7C,0x08,0x04,0x04,0x78}, /* 'n' */
    {0x38,0x44,0x44,0x44,0x38}, /* 'o' */
    {0x7C,0x14,0x14,0x14,0x08}, /* 'p' */
    {0x08,0x14,0x14,0x18,0x7C}, /* 'q' */
    {0x7C,0x08,0x04,0x04,0x08}, /* 'r' */
    {0x48,0x54,0x54,0x54,0x20}, /* 's' */
    {0x04,0x3F,0x44,0x40,0x20}, /* 't' */
    {0x3C,0x40,0x40,0x20,0x7C}, /* 'u' */
    {0x1C,0x20,0x40,0x20,0x1C}, /* 'v' */
    {0x3C,0x40,0x30,0x40,0x3C}, /* 'w' */
    {0x44,0x28,0x10,0x28,0x44}, /* 'x' */
    {0x0C,0x50,0x50,0x50,0x3C}, /* 'y' */
    {0x44,0x64,0x54,0x4C,0x44}, /* 'z' */
    {0x00,0x08,0x36,0x41,0x00}, /* '{' */
    {0x00,0x00,0x7F,0x00,0x00}, /* '|' */
    {0x00,0x41,0x36,0x08,0x00}, /* '}' */
    {0x08,0x08,0x2A,0x1C,0x08}, /* '~' */
};

/*
 * oled_char_at: write one 5×7 character (6 px wide with 1 px gap).
 * Returns next column position.
 */
static uint8_t oled_ch(uint8_t page, uint8_t col, char c)
{
    uint8_t i;
    if (c < 0x20 || c > 0x7E) c = ' ';
    oled_pos(page, col);
    for (i = 0; i < 5; i++)
        oled_dat(pgm_read_byte(&FONT[c - 0x20][i]));
    oled_dat(0x00);
    return (uint8_t)(col + 6);
}

static void oled_str(uint8_t page, uint8_t col, const char *s)
{
    while (*s && col < SCR_W)
        col = oled_ch(page, col, *s++);
}

/*
 * oled_str_slide: draw string starting from x_offset (can be > 128 for off-screen).
 * Used for slide-in animation — caller passes decreasing x_offset each frame.
 * Characters outside 0..127 are simply skipped.
 */
static void oled_str_slide(uint8_t page, int16_t x_off, const char *s)
{
    int16_t col = x_off;
    while (*s) {
        if (col >= SCR_W) break;
        if (col >= 0) oled_ch(page, (uint8_t)col, *s);
        col += 6;
        s++;
    }
}

/* ============================================================================
 *  §5  NUMBER FORMATTING
 * ============================================================================ */

/* Convert ×10 fixed-point to "XX.X" null-terminated string. */
static void fmt1(char *buf, int16_t v)
{
    uint8_t i = 0;
    if (v < 0) { buf[i++] = '-'; v = (int16_t)(-v); }
    uint16_t w = (uint16_t)(v / 10);
    uint8_t  f = (uint8_t)(v % 10);
    if (w >= 100) buf[i++] = (char)('0' + w / 100);
    if (w >=  10) buf[i++] = (char)('0' + (w / 10) % 10);
    buf[i++] = (char)('0' + w % 10);
    buf[i++] = '.';
    buf[i++] = (char)('0' + f);
    buf[i]   = '\0';
}

/* ============================================================================
 *  §6  FAN  —  Timer1 Fast-PWM 8-bit, OC1A = PB1 = D9
 *  f_PWM = 16 MHz / (8 × 256) ≈ 7.8 kHz
 * ============================================================================ */

#define FAN_OFF   0     /*  0 % */
#define FAN_MED   102   /* 40 % */
#define FAN_HIGH  230   /* 90 % */

static void fan_init(void)
{
    DDRB   |=  (1 << PB1);
    TCCR1A  = (1 << COM1A1) | (1 << WGM10);
    TCCR1B  = (1 << WGM12)  | (1 << CS11);
    OCR1A   = FAN_OFF;
}

/* ============================================================================
 *  §7  BUZZER  —  D8 / PB0, Active-LOW
 * ============================================================================ */

static void buz_init(void)
{
    DDRB  |=  (1 << PB0);
    PORTB |=  (1 << PB0);   /* HIGH = OFF */
}

static inline void buz_on(void)  { PORTB &= ~(1 << PB0); }
static inline void buz_off(void) { PORTB |=  (1 << PB0); }

/* Boot beep — 3 short + 1 long; called once before main loop. */
static void buz_boot(void)
{
    uint8_t i;
    for (i = 0; i < 3; i++) {
        buz_on();  _delay_ms(100);
        buz_off(); _delay_ms(70);
    }
    _delay_ms(120);
    buz_on();  _delay_ms(250);
    buz_off();
}

/* ============================================================================
 *  §8  STATUS LEDs
 *  LED_GRN = D4/PD4   LED_YLW = D5/PD5   LED_RED = D6/PD6
 * ============================================================================ */

#define LED_G PD4
#define LED_Y PD5
#define LED_R PD6

static void leds_init(void)
{
    DDRD  |=  (1 << LED_G) | (1 << LED_Y) | (1 << LED_R);
    PORTD &= ~((1 << LED_G) | (1 << LED_Y) | (1 << LED_R));
}

static inline void led_g_on(void)  { PORTD |=  (1 << LED_G); }
static inline void led_g_off(void) { PORTD &= ~(1 << LED_G); }
static inline void led_y_on(void)  { PORTD |=  (1 << LED_Y); }
static inline void led_y_off(void) { PORTD &= ~(1 << LED_Y); }
static inline void led_r_on(void)  { PORTD |=  (1 << LED_R); }
static inline void led_r_off(void) { PORTD &= ~(1 << LED_R); }
static void leds_all_off(void)     { led_g_off(); led_y_off(); led_r_off(); }

/* ============================================================================
 *  §9  BUTTONS  —  D2=RIGHT, D3=LEFT, D7=MAIN  (active LOW, pull-ups)
 * ============================================================================ */

#define BTN_R PD2
#define BTN_L PD3
#define BTN_M PD7

static void btn_init(void)
{
    DDRD  &= ~((1 << BTN_R) | (1 << BTN_L) | (1 << BTN_M));
    PORTD |=  ((1 << BTN_R) | (1 << BTN_L) | (1 << BTN_M));
}

/* 30 ms debounce — returns 1 if confirmed pressed */
static uint8_t btn_right(void)
{ if(!(PIND&(1<<BTN_R))){ _delay_ms(30); return !(PIND&(1<<BTN_R)); } return 0; }

static uint8_t btn_left(void)
{ if(!(PIND&(1<<BTN_L))){ _delay_ms(30); return !(PIND&(1<<BTN_L)); } return 0; }

static uint8_t btn_main(void)
{ if(!(PIND&(1<<BTN_M))){ _delay_ms(30); return !(PIND&(1<<BTN_M)); } return 0; }

/* Block until release + 40 ms settle — prevents ghost presses */
static void rel_right(void){ while(!(PIND&(1<<BTN_R))); _delay_ms(40); }
static void rel_left(void) { while(!(PIND&(1<<BTN_L))); _delay_ms(40); }
static void rel_main(void) { while(!(PIND&(1<<BTN_M))); _delay_ms(40); }

/* ============================================================================
 *  §10  ALERT FSM  (hysteresis thresholds in hum_pct10 units)
 * ============================================================================ */

typedef enum { ST_NORMAL=0, ST_MEDIUM=1, ST_HIGH=2 } State;

/* Entry thresholds (going UP) */
#define HUM_MED_HI   310    /* 31.0 % — enter MEDIUM  */
#define HUM_HIGH_HI  710    /* 71.0 % — enter HIGH    */
/* Exit  thresholds (going DOWN — hysteresis gap ~1.5 %) */
#define HUM_MED_LO   285    /* 28.5 % — drop to NORMAL */
#define HUM_HIGH_LO  685    /* 68.5 % — drop to MEDIUM */

static State fsm_next(State cur, uint16_t h10)
{
    switch (cur) {
        case ST_NORMAL:
            if (h10 >= HUM_HIGH_HI) return ST_HIGH;
            if (h10 >= HUM_MED_HI)  return ST_MEDIUM;
            break;
        case ST_MEDIUM:
            if (h10 >= HUM_HIGH_HI) return ST_HIGH;
            if (h10 <= HUM_MED_LO)  return ST_NORMAL;
            break;
        case ST_HIGH:
            if (h10 <= HUM_MED_LO)  return ST_NORMAL;
            if (h10 <= HUM_HIGH_LO) return ST_MEDIUM;
            break;
    }
    return cur;
}

/* ============================================================================
 *  §11  OUTPUT CONTROLLER  —  LEDs + Fan + Buzzer, tick-synchronised
 *
 *  Called every 100 ms.  Contains ZERO blocking delays.
 *
 *  NORMAL : GRN steady ON  | Fan OFF   | Buzzer OFF
 *  MEDIUM : GRN+YLW ON     | Fan 40 %  | Buzzer 300 ms ON / 2700 ms OFF
 *  HIGH   : GRN+YLW+RED ON | Fan 90 %  | Buzzer continuous ON
 * ============================================================================ */

static void outputs_update(State st, uint8_t tick)
{
    switch (st) {

        case ST_NORMAL:
            OCR1A = FAN_OFF;
            buz_off();
            led_g_on();
            led_y_off();
            led_r_off();
            break;

        case ST_MEDIUM:
            OCR1A = FAN_MED;
            /* 3-second cycle: ON 300 ms (3 ticks), OFF 2700 ms (27 ticks) */
            if ((tick % 30) < 3) buz_on(); else buz_off();
            led_g_on();
            led_y_on();
            led_r_off();
            break;

        case ST_HIGH:
        default:
            OCR1A = FAN_HIGH;
            buz_on();          /* continuous */
            led_g_on();
            led_y_on();
            led_r_on();
            break;
    }
}

/* ============================================================================
 *  §12  GRAPH RING BUFFER
 * ============================================================================ */

#define GLEN 21

static int16_t  gt[GLEN];   /* temperature history */
static uint16_t gh[GLEN];   /* humidity    history */
static uint8_t  gi = 0;     /* next write index    */
static uint8_t  gc = 0;     /* count of valid entries */

static void gpush(int16_t t, uint16_t h)
{
    gt[gi] = t; gh[gi] = h;
    gi = (uint8_t)((gi + 1) % GLEN);
    if (gc < GLEN) gc++;
}

/* Map value to pixel row 0–39 (0=top, 39=bottom) */
static uint8_t gmap(int32_t v, int32_t lo, int32_t hi)
{
    if (v <= lo) return 39;
    if (v >= hi) return  0;
    return (uint8_t)(39L - ((v - lo) * 39L / (hi - lo)));
}

/*
 * gbar: draw a filled bar from top_row to row 39 at column col.
 * Uses page-level bitmask accumulation to avoid the pixel-overwrite bug
 * (multiple rows in the same OLED page-band must be ORed into one byte).
 */
static void gbar(uint8_t col, uint8_t top_row)
{
    uint8_t pg;
    for (pg = 0; pg < 5; pg++) {
        uint8_t pfirst = (uint8_t)(pg * 8);
        if (top_row > pfirst + 7) continue;
        uint8_t mask = 0x00, r;
        for (r = 0; r < 8; r++) {
            uint8_t pr = pfirst + r;
            if (pr >= top_row && pr <= 39)
                mask |= (uint8_t)(0x80 >> r);
        }
        if (mask) { oled_pos((uint8_t)(2 + pg), col); oled_dat(mask); }
    }
}

/* Single dot at (col, row) within the 40-px graph area */
static void gdot(uint8_t col, uint8_t row)
{
    if (row > 39) return;
    oled_pos((uint8_t)(2 + row / 8), col);
    oled_dat((uint8_t)(0x80 >> (row % 8)));
}

/* ============================================================================
 *  §13  SLIDE-IN SPLASH ANIMATIONS
 *
 *  Each frame the text column offset decreases from SCR_W → target_col.
 *  Step size = 8 px per frame, 30 ms per frame → ≈ 16 frames × 30 ms ≈ 480 ms.
 *  Any button press skips the animation immediately.
 * ============================================================================ */

#define SLIDE_STEP  8
#define SLIDE_MS    30

/* Returns 1 if a button was pressed (skip request) */
static uint8_t any_btn(void)
{
    return (!(PIND & (1 << BTN_R)) ||
            !(PIND & (1 << BTN_L)) ||
            !(PIND & (1 << BTN_M)));
}

/* Wait ms milliseconds, returns early if button pressed */
static uint8_t wait_ms_check(uint16_t ms)
{
    uint16_t i;
    for (i = 0; i < ms; i++) {
        _delay_ms(1);
        if (any_btn()) return 1;
    }
    return 0;
}

/*
 * slide_in_line: animate one text line sliding in from the right.
 * page       — OLED page (0–7)
 * target_col — final resting column
 * text       — string to display
 * Returns 1 if button pressed.
 */
static uint8_t slide_in_line(uint8_t page, uint8_t target_col, const char *text)
{
    int16_t x = (int16_t)SCR_W;
    while (x > (int16_t)target_col) {
        oled_clr_row(page);
        oled_str_slide(page, x, text);
        if (any_btn()) {
            /* snap to final position and return */
            oled_clr_row(page);
            oled_str(page, target_col, text);
            return 1;
        }
        _delay_ms(SLIDE_MS);
        x -= SLIDE_STEP;
    }
    oled_clr_row(page);
    oled_str(page, target_col, text);
    return 0;
}

/* Hold a splash frame for `ms` ms, skippable by any button. */
static void splash_hold(uint16_t ms)
{
    wait_ms_check(ms);
    /* flush release */
    _delay_ms(50);
}

/* --------------------------------------------------------------------------
 *  Splash frame 1: Project name
 * -------------------------------------------------------------------------- */
static void splash_project(void)
{
    oled_clear();
    oled_row(0, 0xFF);
    oled_str(0, 14, "  SYSTEM  319  ");
    oled_row(1, 0xFF);

    if (slide_in_line(3, 10, "Smart Temp &")) return;
    if (slide_in_line(4,  8, "Humidity Monitor")) return;
    if (slide_in_line(5,  8, "ATmega328P | I2C")) return;

    oled_row(7, 0xFF);
    oled_str(7, 14, "Any key to skip");
    splash_hold(2500);
}

/* --------------------------------------------------------------------------
 *  Splash frame 2: Lecturer + Group Leader
 * -------------------------------------------------------------------------- */
static void splash_lecturer(void)
{
    oled_clear();
    oled_row(0, 0xFF);
    oled_str(0, 14, "   EDUCATORS   ");
    oled_row(1, 0x01);

    if (slide_in_line(2, 4, "Lecturer:")) return;
    if (slide_in_line(3, 4, "Miss Aimen Najeeb")) return;
    oled_row(4, 0x08);
    if (slide_in_line(5, 4, "Group Leader:")) return;
    if (slide_in_line(6, 4, "M. Taha  EE-23319")) return;

    oled_row(7, 0xFF);
    oled_str(7, 14, "Any key to skip");
    splash_hold(2500);
}

/* --------------------------------------------------------------------------
 *  Splash frame 3: Group members
 * -------------------------------------------------------------------------- */
static void splash_members(void)
{
    oled_clear();
    oled_row(0, 0xFF);
    oled_str(0, 14, " GROUP MEMBERS ");
    oled_row(1, 0xFF);

    if (slide_in_line(2, 4, "Members:")) return;
    if (slide_in_line(3, 4, "Ali    EE-23328")) return;
    if (slide_in_line(4, 4, "Ahmed  EE-23325")) return;
    if (slide_in_line(5, 4, "Farzam EE-23326")) return;

    oled_row(7, 0xFF);
    oled_str(7, 14, "Any key to skip");
    splash_hold(2500);
}

/* --------------------------------------------------------------------------
 *  Splash frame 4: System 319 + "System Ready" status
 * -------------------------------------------------------------------------- */
static void splash_ready(void)
{
    oled_clear();
    oled_row(0, 0xFF);
    oled_str(0, 14, "  SYSTEM  319  ");
    oled_row(1, 0xFF);

    if (slide_in_line(3, 20, "System 319")) return;
    if (slide_in_line(4,  8, "Initializing...")) return;
    if (slide_in_line(5, 14, "Please wait")) return;

    oled_row(7, 0xFF);
    oled_str(7, 20, "Starting up...");
    splash_hold(1500);

    /* "Ready" confirmation frame */
    oled_clr_row(4); oled_clr_row(5);
    oled_str(4,  8, "System Ready!");
    oled_str(5, 14, "Monitoring ON");
    splash_hold(1200);
}

/* ============================================================================
 *  §14  MAIN DISPLAY  (Screen 0)
 *
 *  Page 0 — ████ SYSTEM 319 ████  solid header
 *  Page 1 — separator
 *  Page 2 — Temp: XX.X C
 *  Page 3 — Hum:  XX.X %
 *  Page 4 — separator
 *  Page 5 — [state message]
 *  Page 6 — humidity bar meter
 *  Page 7 — [<] LEFT Info   [>] Graphs
 * ============================================================================ */

static void draw_main(int16_t t10, uint16_t h10, State st)
{
    char buf[22]; uint8_t len, c;

    /* Header */
    oled_row(0, 0xFF);
    oled_str(0, 10, "  SYSTEM  319  ");

    /* Separator */
    oled_row(1, 0x01);

    /* Temperature */
    oled_clr_row(2);
    strcpy(buf, "Temp: ");
    fmt1(buf + 6, t10);
    len = (uint8_t)strlen(buf);
    buf[len++] = ' '; buf[len++] = 'C'; buf[len] = '\0';
    oled_str(2, 2, buf);

    /* Humidity */
    oled_clr_row(3);
    strcpy(buf, "Hum:  ");
    fmt1(buf + 6, (int16_t)h10);
    len = (uint8_t)strlen(buf);
    buf[len++] = ' '; buf[len++] = '%'; buf[len] = '\0';
    oled_str(3, 2, buf);

    /* Separator */
    oled_row(4, 0x40);

    /* Status */
    oled_clr_row(5);
    switch (st) {
        case ST_NORMAL: oled_str(5, 2, "[OK]  System Normal "); break;
        case ST_MEDIUM: oled_str(5, 2, "[! ]  Medium Alert  "); break;
        default:        oled_str(5, 2, "[!!]  HIGH ALERT!!  "); break;
    }

    /* Humidity bar meter */
    oled_clr_row(6);
    {
        uint8_t pct = (h10 > 1000u) ? 100u : (uint8_t)(h10 / 10u);
        uint8_t bw  = (uint8_t)((uint16_t)pct * 116u / 100u);
        oled_pos(6, 6);
        for (c = 0; c < 116u; c++) oled_dat(c < bw ? 0x3C : 0x04);
    }

    /* Navigation */
    oled_row(7, 0x80);
    oled_str(7, 2, "[<]Info [M]Main[>]Graph");
}

/* ============================================================================
 *  §15  INFO / STATUS SCREEN  (Screen 1 — Left side)
 *
 *  Shows: current state, which LEDs are ON, fan %, buzzer status.
 *  Page 0 — header
 *  Page 1 — Current State: NORMAL / MEDIUM / HIGH
 *  Page 2 — separator
 *  Page 3 — LEDs: GRN [on/off] YLW [on/off] RED [on/off]
 *  Page 4 — Fan:  OFF / 40% / 90%
 *  Page 5 — Buzzer: OFF / Intermittent / ON
 *  Page 6 — Threshold info
 *  Page 7 — nav
 * ============================================================================ */

static void draw_info(State st, uint16_t h10)
{
    char buf[22]; uint8_t len;

    oled_row(0, 0xFF);
    oled_str(0, 10, "  SYSTEM INFO  ");
    oled_row(1, 0xFF);

    /* State */
    oled_clr_row(2);
    switch (st) {
        case ST_NORMAL: oled_str(2, 2, "State: NORMAL       "); break;
        case ST_MEDIUM: oled_str(2, 2, "State: MEDIUM ALERT "); break;
        default:        oled_str(2, 2, "State: HIGH ALERT!! "); break;
    }

    oled_row(3, 0x08);

    /* LEDs */
    oled_clr_row(4);
    switch (st) {
        case ST_NORMAL:
            oled_str(4, 2, "LED: GRN  ylw  red"); break;
        case ST_MEDIUM:
            oled_str(4, 2, "LED: GRN  YLW  red"); break;
        default:
            oled_str(4, 2, "LED: GRN  YLW  RED"); break;
    }

    /* Fan + Buzzer */
    oled_clr_row(5);
    switch (st) {
        case ST_NORMAL:
            oled_str(5, 2, "Fan:OFF  Buz:OFF  "); break;
        case ST_MEDIUM:
            oled_str(5, 2, "Fan:40%  Buz:INT  "); break;
        default:
            oled_str(5, 2, "Fan:90%  Buz:ON   "); break;
    }

    /* Humidity reading + threshold note */
    oled_clr_row(6);
    strcpy(buf, "Hum:");
    fmt1(buf + 4, (int16_t)h10);
    len = (uint8_t)strlen(buf);
    buf[len++] = '%'; buf[len] = '\0';
    /* append threshold reference */
    if (len < 18) {
        switch (st) {
            case ST_NORMAL: strcat(buf, " (<30%)"); break;
            case ST_MEDIUM: strcat(buf, " (<70%)"); break;
            default:        strcat(buf, " (>70%)"); break;
        }
    }
    oled_str(6, 2, buf);

    oled_row(7, 0x80);
    oled_str(7, 2, "[<]Graph[M]Main[>]Main");
}

/* ============================================================================
 *  §16  GRAPH PAGES
 * ============================================================================ */

static void draw_graph_temp(void)
{
    uint8_t i, col, c, r;
    oled_clear();

    oled_row(0, 0xFF);
    oled_str(0, 8, " TEMP HISTORY(C)");

    oled_str(2, 0, "50");
    oled_str(4, 0, "25");
    oled_str(6, 0, " 0");

    /* Y-axis */
    for (r = 0; r < 40; r++) gdot(14, r);
    /* 25 °C dotted guide */
    for (c = 16; c < SCR_W; c += 5) gdot(c, 20);

    {
        uint8_t s = (gc < GLEN) ? 0 : gi;
        for (i = 0; i < gc; i++) {
            uint8_t di   = (uint8_t)((s + i) % GLEN);
            uint8_t prow = gmap(gt[di], 0, 500);
            col = (uint8_t)(16 + i * 5);
            if (col + 2 >= SCR_W) break;
            gbar(col, prow); gbar((uint8_t)(col+1), prow); gbar((uint8_t)(col+2), prow);
        }
    }

    oled_row(7, 0x80);
    {
        char b[18]; strcpy(b, "Now:");
        uint8_t last = gi ? (uint8_t)(gi - 1) : (GLEN - 1);
        fmt1(b + 4, gt[last]);
        uint8_t l = (uint8_t)strlen(b); b[l++] = 'C'; b[l] = '\0';
        oled_str(7, 2, b);
    }
    oled_str(7, 68, "[<]Info[>]Hum");
}

static void draw_graph_hum(void)
{
    uint8_t i, col, c, r;
    oled_clear();

    oled_row(0, 0xFF);
    oled_str(0, 8, " HUM  HISTORY(%)");

    oled_str(2, 0, "99");
    oled_str(4, 0, "50");
    oled_str(6, 0, " 0");

    for (r = 0; r < 40; r++) gdot(14, r);

    /* 30 % boundary (sparse dots) */
    { uint8_t r30 = gmap(300, 0, 1000); for(c=16;c<SCR_W;c+=8) gdot(c,r30); }
    /* 50 % midline */
    { uint8_t r50 = gmap(500, 0, 1000); for(c=16;c<SCR_W;c+=5) gdot(c,r50); }
    /* 70 % HIGH threshold (dense) */
    { uint8_t r70 = gmap(700, 0, 1000); for(c=16;c<SCR_W;c+=3) gdot(c,r70); }

    {
        uint8_t s = (gc < GLEN) ? 0 : gi;
        for (i = 0; i < gc; i++) {
            uint8_t di   = (uint8_t)((s + i) % GLEN);
            uint8_t prow = gmap(gh[di], 0, 1000);
            col = (uint8_t)(16 + i * 5);
            if (col + 2 >= SCR_W) break;
            gbar(col, prow); gbar((uint8_t)(col+1), prow); gbar((uint8_t)(col+2), prow);
        }
    }

    oled_row(7, 0x80);
    {
        char b[18]; strcpy(b, "Now:");
        uint8_t last = gi ? (uint8_t)(gi - 1) : (GLEN - 1);
        fmt1(b + 4, (int16_t)gh[last]);
        uint8_t l = (uint8_t)strlen(b); b[l++] = '%'; b[l] = '\0';
        oled_str(7, 2, b);
    }
    oled_str(7, 62, "[<]Temp[>]Main");
}

/* ============================================================================
 *  §17  APPLICATION MAIN
 *
 *  Screens:
 *    0 = MAIN  (System 319 live readings)
 *    1 = INFO  (state details, LEDs, fan, buzzer)
 *    2 = TEMP GRAPH
 *    3 = HUM  GRAPH
 *
 *  Button mapping (runtime):
 *    RIGHT (D2) → screen = (screen + 1) % 4
 *    LEFT  (D3) → screen = (screen == 0) ? 3 : screen - 1
 *    MAIN  (D7) → screen = 0  (always returns to System 319 main)
 * ============================================================================ */

int main(void)
{
    int16_t T    = 250;          /* 25.0 °C default */
    uint16_t H   = 400;          /* 40.0 % default  */
    State   st   = ST_NORMAL;
    uint8_t sc   = 0;            /* current screen  */
    uint8_t redraw = 1;
    uint8_t tick   = 0;          /* 100 ms counter, wraps at 200 */

    /* ---- Init peripherals ---- */
    fan_init();
    buz_init();
    leds_init();
    btn_init();
    i2c_init();
    oled_init();
    aht20_init();

    /* ---- Slide-in splash sequence ---- */
    splash_project();
    splash_lecturer();
    splash_members();
    splash_ready();

    /* Flush any button presses from splash skipping */
    _delay_ms(100);

    /* Boot confirmation beep */
    buz_boot();

    /* ================================================================
     *  MAIN LOOP
     * ================================================================ */
    for (;;)
    {
        /* Step 1: Read sensor */
        aht20_read(&T, &H);

        /* Step 2: Update FSM */
        st = fsm_next(st, H);

        /* Step 3: Push to graph history */
        gpush(T, H);

        /* Step 4: Render screen */
        if (redraw) {
            oled_clear();
            redraw = 0;
        }
        switch (sc) {
            case 1:  draw_info(st, H);  break;
            case 2:  draw_graph_temp(); break;
            case 3:  draw_graph_hum();  break;
            default: draw_main(T, H, st); break;
        }

        /* Step 5: Poll sub-loop (20 × 100 ms ≈ 2 s between reads) */
        {
            uint8_t lp;
            for (lp = 0; lp < 20; lp++)
            {
                /* Drive outputs — fully tick-based, zero blocking */
                outputs_update(st, tick);
                tick++;
                if (tick >= 200) tick = 0;

                /* RIGHT → next screen */
                if (btn_right()) {
                    rel_right();
                    sc = (uint8_t)((sc + 1) % 4);
                    redraw = 1; break;
                }

                /* LEFT → previous screen */
                if (btn_left()) {
                    rel_left();
                    sc = (sc == 0) ? 3u : (uint8_t)(sc - 1);
                    redraw = 1; break;
                }

                /* MAIN → System 319 main screen */
                if (btn_main()) {
                    rel_main();
                    sc = 0;
                    redraw = 1; break;
                }

                _delay_ms(100);
            }
        }
    }

    return 0;
}
