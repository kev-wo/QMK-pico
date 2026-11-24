// Copyright 2026 kev-wo
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

enum layers { _BASE, _FN };

/*
 * The LAYOUT() macro takes the keys row by row in the order you read them on
 * the keyboard (left to right). keyboard.json translates every key to the
 * (row wire, column wire) it is really soldered to, so the curved / snaking
 * column wires never show up here.
 *
 * Base layer
 * ,------------------------------------------------------------------------------------.
 * |Esc | F1 | F2 | F3 | F4 | F5 | F6 | F7 | F8 | F9 |F10 |F11 |F12 |Ins |
 * |PrSc|  ` |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 |  0 |  - |  = |Bksp|
 * |Caps|  Q |  W |  E |  R |  T |  Y |  U |  I |  O |  P |  [ |  ] |  \ |Entr|
 * |Tab |  A |  S |  D |  F |  G |  H |  J |  K |  L |  ; |  ' |Home|PgUp|PgDn|
 * |Shft|  Z |  X |  C |  V |  B |  N |  M |  , |  . |  / | Up |End |
 * |Ctrl|GUI |Alt | Fn |Del |   Space   |RAlt|RCtl|Left|Down|Rght|
 * `------------------------------------------------------------------------------------'
 */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_INS,
        KC_PSCR, KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_CAPS, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_ENT,
        KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_HOME, KC_PGUP, KC_PGDN,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_UP,   KC_END,
        KC_LCTL, KC_LGUI, KC_LALT, MO(_FN), KC_DEL,  KC_SPC,  KC_RALT, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),
    /*
     * Fn layer (hold Fn): media, mouse buttons (mouse keys), bootloader.
     *   Fn+Esc     = jump to bootloader (flash new firmware)
     *   Fn+Space   = left click,  Fn+RAlt = right click,  Fn+Del = middle click
     *   Fn+Up/Down = scroll wheel
     */
    [_FN] = LAYOUT(
        QK_BOOT, KC_BRID, KC_BRIU, _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, _______, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, MS_WHLU, _______,
        _______, _______, _______, _______, MS_BTN3, MS_BTN1, MS_BTN2, _______, _______, MS_WHLD, _______
    ),
};
