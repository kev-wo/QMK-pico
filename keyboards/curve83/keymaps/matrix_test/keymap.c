// Copyright 2026 kev-wo
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Wiring check keymap. Flash this, run `qmk console`, then press every key:
// the console prints which row wire / column wire the switch closed and what
// the default keymap thinks that key is. Any key that prints nothing, or
// prints the wrong label, tells you exactly which wire or table entry to fix.
#include QMK_KEYBOARD_H

static const char *const names[MATRIX_ROWS][MATRIX_COLS] = {
    {"Esc",   "-",  "F1", "F2", "F3",  "F4", "F5",  "F6", "F7",    "F8", "F9",   "F10",   "F11",  "-",    "F12",  "Ins"},
    {"PrtSc", "`",  "1",  "2",  "3",   "4",  "5",   "6",  "7",     "8",  "9",    "0",     "-",    "=",    "-",    "Bksp"},
    {"Caps",  "Q",  "W",  "E",  "R",   "T",  "Y",   "U",  "I",     "O",  "P",    "[",     "]",    "\\",   "-",    "Enter"},
    {"Tab",   "-",  "A",  "S",  "D",   "F",  "G",   "H",  "J",     "K",  "L",    ";",     "'",    "Home", "PgUp", "PgDn"},
    {"Shift", "-",  "-",  "Z",  "X",   "C",  "V",   "B",  "N",     "M",  ",",    ".",     "/",    "Up",   "-",    "End"},
    {"Ctrl",  "-",  "-",  "GUI","Alt", "Fn", "Del", "-",  "Space", "-",  "RAlt", "RCtrl", "Left", "Down", "-",    "Right"},
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // every key is a no-op except Esc, so nothing gets typed while testing
    [0] = LAYOUT(
        QK_BOOT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        uint8_t r = record->event.key.row, c = record->event.key.col;
        uprintf("pressed: row %u, col %2u  ->  %s\n", r, c, names[r][c]);
    }
    return true;
}

void pointing_device_init_user(void) {
    uprintf("trackpad init done\n");
}

report_mouse_t pointing_device_task_user(report_mouse_t m) {
    if (m.x || m.y) uprintf("trackpad x=%d y=%d\n", m.x, m.y);
    return m;
}
