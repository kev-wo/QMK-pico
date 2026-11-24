// Copyright 2026 kev-wo
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

/* ---------------------------------------------------------------------------
 * Raspberry Pi Pico
 * Wiring: columns C0..C15 = GP0..GP15, rows R5..R0 = GP16..GP21
 *         (so R0 / F-key row = GP21, R5 / spacebar row = GP16).
 *         Diodes: cathode stripe toward the row wire -> COL2ROW.
 *
 * Enter the UF2 bootloader: hold BOOTSEL while plugging in, hold Esc while
 * plugging in (Bootmagic), press Fn + Esc (QK_BOOT), or double-tap RUN/reset.
 * ------------------------------------------------------------------------- */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP25 // on-board LED blinks while waiting

// Trackpad: not used yet. When it is added, its driver and pins go here.
