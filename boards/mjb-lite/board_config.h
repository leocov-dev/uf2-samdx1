#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#define CRYSTALLESS 1

#define VENDOR_NAME "lc"
#define PRODUCT_NAME "Mr Jiggly Bones bootloader"
#define VOLUME_LABEL "MJBBOOT" /* unused: no MSC */
#define INDEX_URL "https://github.com/leocov-dev/mr-jiggly-bones"
#define BOARD_ID "SAMD21E15B-mjb-lite-v1"

/* pid.codes test VID; app enumerates as 0x1209:0x0001, bootloader as :0x0002
 * so the Chrome configurator can filter for boot mode. */
#define USB_VID 0x1209
#define USB_PID 0x0002

/* Green channel of the RGB status LED (common-cathode: high = lit). */
#define LED_PIN PIN_PA05

/* E15: 32 KB flash / 256 B rows (upstream default assumes a 256 KB part). */
#define FLASH_NUM_ROWS 128

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
