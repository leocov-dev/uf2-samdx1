CHIP_FAMILY = samd21
# 'A'-suffix header: the E16B die is register- and memory-map-identical to the
# E16A the vendored CMSIS pack ships (same trick as the main firmware build).
CHIP_VARIANT = SAMD21E16A
# Upstream links every SAMD21 against a J18A (256K/32K) script; the E16 has
# 64K flash / 8K RAM, so use a right-sized board-local script.
LINKER_SCRIPT = boards/lcj-standard/bootloader.ld
