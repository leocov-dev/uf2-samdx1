CHIP_FAMILY = samd21
# 'A'-suffix header: the E16B die is register- and memory-map-identical to the
# E15A the vendored CMSIS pack ships (same trick as the main firmware build).
CHIP_VARIANT = SAMD21E15A
# Upstream links every SAMD21 against a J18A (256K/32K) script; the E15 has
# 32K flash / 4K RAM, so use a right-sized board-local script.
LINKER_SCRIPT = boards/lcj-lite/bootloader.ld
