// Copyright 2026 kev-wo
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

/* ---------------------------------------------------------------------------
 * Raspberry Pi Pico
 * Double-tap the RUN/reset button (if wired) to enter the UF2 bootloader.
 * Otherwise: hold BOOTSEL while plugging in, or hold Esc while plugging in
 * (Bootmagic), or press Fn + Esc (QK_BOOT) once the firmware is running.
 * ------------------------------------------------------------------------- */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP25 // on-board LED blinks while waiting

/* ---------------------------------------------------------------------------
 * Trackpad: Cirque Pinnacle (GlidePoint TM0xx0xx) over I2C
 *   Pico GP26 = SDA (I2C1),  Pico GP27 = SCL (I2C1),  3V3 + GND
 *   Change the pins here if yours is wired differently. SDA must be an
 *   I2C1-SDA pin (GP2/6/10/14/18/26) and SCL an I2C1-SCL pin (GP3/7/11/15/19/27).
 * ------------------------------------------------------------------------- */
#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP26
#define I2C1_SCL_PIN GP27

#define CIRQUE_PINNACLE_DIAMETER_MM 40         // 23, 35 or 40 mm pads
#define CIRQUE_PINNACLE_POSITION_MODE CIRQUE_PINNACLE_ABSOLUTE_MODE
#define CIRQUE_PINNACLE_TAP_ENABLE             // tap pad = left click
#define CIRQUE_PINNACLE_SECONDARY_TAP_ENABLE   // tap top-right corner = right click
#define POINTING_DEVICE_GESTURES_SCROLL_ENABLE // circle the outer ring to scroll
#define POINTING_DEVICE_GESTURES_CURSOR_GLIDE_ENABLE // flick = cursor keeps gliding
#define POINTING_DEVICE_ROTATION_90            // remove/change if cursor axes are off
