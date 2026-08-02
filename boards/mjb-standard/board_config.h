#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#define CRYSTALLESS 1

#define VENDOR_NAME "lc"
#define PRODUCT_NAME "Mr Jiggly Bones bootloader"
#define VOLUME_LABEL "MJBBOOT" /* unused: no MSC */
#define INDEX_URL "https://github.com/leocov-dev/mr-jiggly-bones"
#define BOARD_ID "SAMD21E16B-mjb-standard-v1"

/* pid.codes test VID; app enumerates as 0x1209:0x0001, bootloader as :0x0002
 * so the Chrome configurator can filter for boot mode. */
#define USB_VID 0x1209
#define USB_PID 0x0002

/* RGB status LED (common-cathode: high = lit). LED_PIN (green) is the
 * upstream uf2-samdx1 activity LED; LED_R_PIN/LED_B_PIN are solid on/off
 * (no PWM) so the bootloader can show a distinct color — see main.c, which
 * turns it solid red once check_start_application() commits to staying
 * resident, as the "safe to release the button" cue. Pin map matches the
 * app-side PWM driver in firmware/platform/target/hal_pwm.c. */
#define LED_PIN PIN_PA05
#define LED_R_PIN PIN_PA04
#define LED_B_PIN PIN_PA10
/* The chosen LED is very powerful and is blinding at full duty cycle — match
 * the app's cap (firmware/core/app/config.h CFG_LED_BRIGHTNESS_PCT). */
#define LED_BRIGHTNESS_PCT 10

/* E16: 64 KB flash / 256 B rows (upstream default assumes a 256 KB part). */
#define FLASH_NUM_ROWS 256

/* RWW EEPROM section, 2 KB on the E16 (samd21e16bu.h). Non-zero enables the
 * bootloader's RWWEE write path, which is where the app's runtime motion
 * config lives — see firmware/DESIGN.md §7. Sized per-part because the
 * shared CMSIS instance header gets it wrong; see inc/uf2.h. */
#define NVMCTRL_RWW_EEPROM_SIZE 0x800

/* Transport policy: HF2 over HID + WebUSB only.
 * - No MSC/UF2 drive ever (product requirement), no FAT.
 * - No CDC: HF2 covers flashing; saves ~1.3 KB.
 * - No double-tap-reset arming: dongle has no reset button; the app requests
 *   the bootloader by writing DBL_TAP_MAGIC (5 s hold of button A).
 * - No handovers: the app's USB stack (TinyUSB HID mouse) has no HF2/MSC. */
#define USE_MSC 0
#define USE_FAT 0
#define USE_CDC 0
#define USE_HID 1
#define USE_WEBUSB 1
#define USE_DBL_TAP 0
#define USE_SINGLE_RESET 0
#define USE_HID_HANDOVER 0
#define USE_MSC_HANDOVER 0

#endif
