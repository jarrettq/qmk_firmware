VIA_ENABLE = yes
COMBO_ENABLE = yes

ifeq ($(strip $(MCU)), atmega32u4)
    QMK_SETTINGS = no
    COMBO_ENABLE = no
    MOUSEKEY_ENABLE = no
    TAP_DANCE_ENABLE = yes
    SPACE_CADET_ENABLE = no
    GRAVE_ESC_ENABLE = no
    KEY_OVERRIDE_ENABLE = no
    MAGIC_ENABLE = no
endif