#ifndef UF2_H
#define UF2_H 1

#include <stdint.h>
#include "board_config.h"

#include "sam.h"
#define UF2_DEFINE_HANDOVER 1 // for testing
#include "uf2format.h"
#include "uf2hid.h"
#include "main.h"
#include "cdc_enumerate.h"
#include "sam_ba_monitor.h"
#include "usart_sam_ba.h"
#include <stdio.h>
#include <string.h>


#include "configkeys.h"


#undef DISABLE
#undef ENABLE

// always go for crystalless - smaller and more compatible
#ifndef CRYSTALLESS
#define CRYSTALLESS 1
#endif

#ifndef USB_PID
#define USB_VID 0x03EB // Atmel
#define USB_PID 0x2402 // Generic HID device
#endif

#ifndef INDEX_URL
#define INDEX_URL "https://www.pxt.io/"
#endif

#include "uf2_version.h"

// needs to be more than ~4200 (to force FAT16)
#define NUM_FAT_BLOCKS 16000

// 8kB with NeoPixel or DotStar is a tight fit.
#define SAMD21_MINIMIZE (defined(SAMD21) && (defined(BOARD_NEOPIXEL_PIN) || defined(BOARD_RGBLED_CLOCK_PIN)))

// Feature flags below are all #ifndef-guarded so a board_config.h can
// override them (mr-jiggly-bones fork; upstream hard-codes them).
// Logging to help debugging
#ifndef USE_LOGS
#define USE_LOGS 0
#endif
// Check various conditions; best leave on
#ifndef USE_ASSERT
#define USE_ASSERT 0 // 188 bytes
#endif
// Enable reading flash via FAT files; otherwise drive will appear empty
#ifndef USE_FAT
#define USE_FAT 1 // 272 bytes
#endif

// Enable URL in INFO_UF2.TXT. Used for boards that are a very tight fit.
#ifndef USE_URL_IN_INFO
#define USE_URL_IN_INFO SAMD21_MINIMIZE
#endif

// Enable index.htm file on the drive.
// 132 bytes
#ifndef USE_INDEX_HTM
#define USE_INDEX_HTM (!USE_URL_IN_INFO)
#endif

// Enable USB CDC (Communication Device Class; i.e., USB serial) monitor for Arduino style flashing
#ifndef USE_CDC
#define USE_CDC 1 // 1264 bytes (plus terminal, see below)
#endif
// Support the UART (real serial port, not USB)
#ifndef USE_UART
#define USE_UART 0
#endif
// Support Human Interface Device (HID) - serial, flashing and debug
#ifndef USE_HID
#define USE_HID 1 // 788 bytes
#endif
// Expose HID via WebUSB
#ifndef USE_WEBUSB
#define USE_WEBUSB 1
#endif
// Doesn't yet disable code, just enumeration
#ifndef USE_MSC
#define USE_MSC 1
#endif

#ifdef BOARD_SCREEN
#define USE_SCREEN 1
#else
#define USE_SCREEN 0
#endif

// If enabled, bootloader will start on power-on and every reset. A second reset
// will start the app. This only happens if the app says it wants that (see SINGLE_RESET() below).
// If disabled here or by the app, the bootloader will only start with double-click of the reset
// button.
#ifndef USE_SINGLE_RESET
#define USE_SINGLE_RESET 1
#endif

// Arm the double-tap-of-reset entry (write magic, wait 500 ms for a second
// tap). Boards without a reset button (e.g. mr-jiggly-bones dongles) disable this
// so power glitches can't strand the device in the bootloader; the app can
// still request the bootloader by writing DBL_TAP_MAGIC itself.
#ifndef USE_DBL_TAP
#define USE_DBL_TAP 1
#endif

// Scale the on-time of every LED soft-PWM cycle (led_tick()) to this percent
// of full brightness. Boards that don't set it get full brightness
// (unchanged upstream behavior).
#ifndef LED_BRIGHTNESS_PCT
#define LED_BRIGHTNESS_PCT 100
#endif

// Fine-tuning of features
#ifndef USE_HID_SERIAL
#define USE_HID_SERIAL 0   // just an example, not really needed; 36 bytes
#endif
#ifndef USE_HID_EXT
#define USE_HID_EXT 1      // extended HID commands (read/write mem); 60 bytes
#endif
#ifndef USE_HID_HANDOVER
#define USE_HID_HANDOVER 1 // allow HID application->bootloader seamless transition; 56 bytes
#endif
#ifndef USE_MSC_HANDOVER
#define USE_MSC_HANDOVER 1 // ditto for MSC; 348 bytes
#endif
#ifndef USE_MSC_CHECKS
#define USE_MSC_CHECKS 0   // check validity of MSC commands; 460 bytes
#endif
#ifndef USE_CDC_TERMINAL
#define USE_CDC_TERMINAL 0 // enable ASCII mode on CDC loop (not used by BOSSA); 228 bytes
#endif
#ifndef USE_DBG_MSC
#define USE_DBG_MSC 0      // output debug info about MSC
#endif

#if USE_CDC
#define CDC_VERSION "S"
#else
#define CDC_VERSION ""
#endif

#if USE_LOGS
#define LOGS_VERSION "L"
#else
#define LOGS_VERSION ""
#endif

#if USE_FAT
#define FAT_VERSION "F"
#else
#define FAT_VERSION ""
#endif

#if USE_ASSERT
#define ASSERT_VERSION "A"
#else
#define ASSERT_VERSION ""
#endif

#if USE_HID
#define HID_VERSION "H"
#else
#define HID_VERSION ""
#endif

#if USE_SINGLE_RESET
#define RESET_VERSION "R"
#else
#define RESET_VERSION ""
#endif

#if USE_WEBUSB
#define WEB_VERSION "W"
#else
#define WEB_VERSION ""
#endif

#if USE_MSC_HANDOVER
#define MSC_HANDOVER_VERSION "O"
#else
#define MSC_HANDOVER_VERSION ""
#endif

#define UF2_VERSION                                                                                \
    UF2_VERSION_BASE " " CDC_VERSION LOGS_VERSION FAT_VERSION ASSERT_VERSION HID_VERSION           \
        WEB_VERSION RESET_VERSION MSC_HANDOVER_VERSION

// End of config

#define USE_MONITOR (USE_CDC || USE_UART)

#ifdef SAMD51
// 51 also runs at 48MHz in bootloader mode, but it's still faster
#define TIMER_STEP 2000
#else
#define TIMER_STEP 1500
#endif

#ifdef BOARD_NEOPIXEL_PIN
#define COLOR_START 0x040000
#define COLOR_USB 0x000400
#define COLOR_UART 0x040400
#define COLOR_LEAVE 0x000000
#else
#define COLOR_START 0x000040
#define COLOR_USB 0x004000
#define COLOR_UART 0x404000
#define COLOR_LEAVE 0x000000
#endif

/*
From CPU config:
#define FLASH_SIZE            0x8000UL
#define FLASH_PAGE_SIZE       64
#define FLASH_NB_OF_PAGES     512
*/

// These two need to be defined as plain decimal numbers, as we're using # on them
#define FLASH_ROW_SIZE 256
#ifndef FLASH_NUM_ROWS
#define FLASH_NUM_ROWS 1024
#endif

// --- RWW EEPROM ------------------------------------------------------------
// The SAMD21 "B" die carries a read-while-write EEPROM section in its own
// address aperture, separate from the main array. We build against the
// samd21a CMSIS pack (lib/samd21/samd21a), which predates it and defines none
// of these; the samd21b pack's Nvmctrl register struct is byte-identical, so
// the constants are hand-ported here. Values from
// samd21b/include/{component,instance}/nvmctrl.h.
//
// The section mirrors the main array's geometry: 64 B page, 256 B row. Erase
// by row (RWWEEER), write by page (RWWEEWP); PBC (page buffer clear) is
// shared with the main array and needs no RWWEE-specific opcode.
//
// Deliberately NOT ported: NVMCTRL_RWWEE_PAGES. The vendor header comments it
// "// Page size" but it is a page *count*, and it lives in the shared
// instance header, so its 32 is wrong for the 1 KB (16-page) E15.
#ifndef NVMCTRL_CTRLA_CMD_RWWEEER
#define NVMCTRL_CTRLA_CMD_RWWEEER_Val 0x1A
#define NVMCTRL_CTRLA_CMD_RWWEEER (NVMCTRL_CTRLA_CMD_RWWEEER_Val << NVMCTRL_CTRLA_CMD_Pos)
#endif
#ifndef NVMCTRL_CTRLA_CMD_RWWEEWP
#define NVMCTRL_CTRLA_CMD_RWWEEWP_Val 0x1C
#define NVMCTRL_CTRLA_CMD_RWWEEWP (NVMCTRL_CTRLA_CMD_RWWEEWP_Val << NVMCTRL_CTRLA_CMD_Pos)
#endif
#ifndef NVMCTRL_RWW_EEPROM_ADDR
#define NVMCTRL_RWW_EEPROM_ADDR 0x00400000
#endif
// Per-part, so each board_config.h sets it: 0x800 on the E16, 0x400 on the
// E15 (samd21e16bu.h / samd21e15bu.h, NVMCTRL_RWW_EEPROM_SIZE). Zero means
// "this board doesn't use RWWEE" and compiles the write path out — that's
// every upstream board, whose behavior is then unchanged.
#ifndef NVMCTRL_RWW_EEPROM_SIZE
#define NVMCTRL_RWW_EEPROM_SIZE 0
#endif
#define USE_RWWEE (NVMCTRL_RWW_EEPROM_SIZE > 0)

#define NOOP                                                                                       \
    do {                                                                                           \
    } while (0)

#if USE_LOGS
struct LogStore {
    int ptr;
    char buffer[4096];
};
extern struct LogStore logStoreUF2;
void logmsg(const char *msg);
void logval(const char *lbl, uint32_t v);
void logwritenum(uint32_t n);
void logwrite(const char *msg);
void logreset(void);
#else
#define logmsg(...) NOOP
#define logval(...) NOOP
#define logwritenum(...) NOOP
#define logwrite(...) NOOP
#define logreset() NOOP
#endif

#if USE_DBG_MSC
#define DBG_MSC(x) x
#else
#define DBG_MSC(x) NOOP
#endif

void panic(int code);

#if USE_ASSERT
#define assert(cond)                                                                               \
    if (!(cond)) {                                                                                 \
        panic(__LINE__);                                                                           \
    }
#else
#define assert(cond) NOOP
#endif

extern volatile bool b_sam_ba_interface_usart;
void flash_write_row(uint32_t *dst, uint32_t *src);
void flash_erase_to_end(uint32_t *start_address);
void flash_write_words(uint32_t *dst, uint32_t *src, uint32_t n_words);
void copy_words(uint32_t *dst, uint32_t *src, uint32_t n_words);
#if USE_RWWEE
// dst must be a row-aligned address inside the RWWEE aperture. Reading the
// section back needs no special path — it's plain memory to the CPU, so
// copy_words() (and thus HF2_CMD_READ_WORDS) already works on it.
void flash_write_rwwee_row(uint32_t *dst, uint32_t *src);
#endif

int writeNum(char *buf, uint32_t n, bool full);

void process_hid(void);

// index of highest LUN
#define MAX_LUN 0
void process_msc(void);
void msc_reset(void);
//! Static block size for all memories
#define UDI_MSC_BLOCK_SIZE 512L

void read_block(uint32_t block_no, uint8_t *data);
#define MAX_BLOCKS (FLASH_SIZE / 256 + 100)
typedef struct {
    uint32_t numBlocks;
    uint32_t numWritten;
    uint8_t writtenMask[MAX_BLOCKS / 8 + 1];
} WriteState;
void write_block(uint32_t block_no, uint8_t *data, bool quiet, WriteState *state);
void padded_memcpy(char *dst, const char *src, int len);

// Last word in RAM
// Unlike for ordinary applications, our link script doesn't place the stack at the bottom
// of the RAM, but instead after all allocated BSS.
// In other words, this word should survive reset.
#ifdef SAMD21
#define DBL_TAP_PTR ((volatile uint32_t *)(HMCRAMC0_ADDR + HMCRAMC0_SIZE - 4))
#endif
#ifdef SAMD51
#define DBL_TAP_PTR ((volatile uint32_t *)(HSRAM_ADDR + HSRAM_SIZE - 4))
#endif
#define DBL_TAP_MAGIC 0xf01669ef // Randomly selected, adjusted to have first and last bit set
#define DBL_TAP_MAGIC_QUICK_BOOT 0xf02669ef

#if USE_SINGLE_RESET
#ifdef SAMD21
#define SINGLE_RESET() (*((uint32_t *)0x20B4) == 0x87eeb07c)
#endif
#ifdef SAMD51
#define SINGLE_RESET() (*((uint32_t *)0x4268) == 0x87eeb07c)
#endif
#endif

void resetIntoApp(void);
void resetIntoBootloader(void);
extern uint32_t current_cpu_frequency_MHz;
extern volatile bool led_tick_on;
extern volatile bool led_boot_locked;
void system_init(void);

#define LED_TICK led_tick

#define PINOP(pin, OP) (PORT->Group[(pin) / 32].OP.reg = (1 << ((pin) % 32)))

void led_tick(void);
void led_signal(void);
void led_init(void);
void RGBLED_set_color(uint32_t color);

// Not all targets have a LED
#if defined(LED_PIN)
#if !defined(LED_PIN_PULLUP)
#define LED_MSC_OFF() PINOP(LED_PIN, OUTCLR)
#define LED_MSC_ON() PINOP(LED_PIN, OUTSET)
#else
#define LED_MSC_OFF() PINOP(LED_PIN, OUTSET)
#define LED_MSC_ON() PINOP(LED_PIN, OUTCLR)
#endif
#define LED_MSC_TGL() PINOP(LED_PIN, OUTTGL)
#else
#define LED_MSC_OFF()
#define LED_MSC_ON()
#define LED_MSC_TGL()
#endif

// Not all targets have a TX LED
#if defined(LED_TX_PIN)
#if defined(LED_TX_PIN_PULLUP)
#define LED_TX_OFF() PINOP(LED_TX_PIN, OUTSET)
#define LED_TX_ON() PINOP(LED_TX_PIN, OUTCLR)
#else
#define LED_TX_OFF() PINOP(LED_TX_PIN, OUTCLR)
#define LED_TX_ON() PINOP(LED_TX_PIN, OUTSET)
#endif
#define LED_TX_TGL() PINOP(LED_TX_PIN, OUTTGL)
#else
#define LED_TX_OFF()
#define LED_TX_ON()
#define LED_TX_TGL()
#endif

// Not all targets have a RX LED
#if defined(LED_RX_PIN)
#if defined(LED_RX_PIN_PULLUP)
#define LED_RX_OFF() PINOP(LED_RX_PIN, OUTSET)
#define LED_RX_ON() PINOP(LED_RX_PIN, OUTCLR)
#else
#define LED_RX_OFF() PINOP(LED_RX_PIN, OUTCLR)
#define LED_RX_ON() PINOP(LED_RX_PIN, OUTSET)
#endif
#define LED_RX_TGL() PINOP(LED_RX_PIN, OUTTGL)
#else
#define LED_RX_OFF()
#define LED_RX_ON()
#define LED_RX_TGL()
#endif

extern uint32_t timerHigh, resetHorizon;
void timerTick(void);
void delay(uint32_t ms);
void hidHandoverLoop(int ep);
void handoverPrep(void);

// Useful for debugging.
#ifdef BLINK_DEBUG
void blink_n(uint32_t pin, uint32_t n, uint32_t interval);
void blink_n_forever(uint32_t pin, uint32_t n, uint32_t interval);
#endif

#define CONCAT_1(a, b) a##b
#define CONCAT_0(a, b) CONCAT_1(a, b)
#define STATIC_ASSERT(e) enum { CONCAT_0(_static_assert_, __LINE__) = 1 / ((e) ? 1 : 0) }

#ifdef SAMD21
STATIC_ASSERT(FLASH_ROW_SIZE == FLASH_PAGE_SIZE * 4);
STATIC_ASSERT(FLASH_ROW_SIZE == NVMCTRL_ROW_SIZE);
STATIC_ASSERT(FLASH_NUM_ROWS * 4 == FLASH_NB_OF_PAGES);
#endif

extern const char infoUf2File[];

#if USE_SCREEN
void draw_screen(void);
void draw_hf2(void);
void draw_drag(void);
void screen_init(void);
void screen_early_init(void);
#endif

#endif
