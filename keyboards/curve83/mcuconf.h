// Copyright 2026 kev-wo
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include_next <mcuconf.h>

// Trackpad is on the RP2040's I2C1 peripheral.
#undef RP_I2C_USE_I2C1
#define RP_I2C_USE_I2C1 TRUE
