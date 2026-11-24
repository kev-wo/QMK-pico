# QMK firmware for Curve83 (curved one-handed keyboard + trackpad, Raspberry Pi Pico)

```
keyboards/curve83/
├── keyboard.json          # MCU, pins, diode direction, and the key -> (row, col) map
├── config.h               # trackpad (Cirque, I2C) + bootloader settings
├── mcuconf.h              # turns on the RP2040's I2C1 for the trackpad
├── rules.mk               # trackpad driver choice
└── keymaps/
    ├── default/keymap.c   # the real layout (base layer + Fn layer)
    └── matrix_test/       # wiring check: prints row/col of each key press
docs/matrix_map.png        # every key with its row wire and column wire
docs/plate_holes.png       # plate holes numbered (used by tools/gen_layout.py)
tools/gen_layout.py        # regenerates keyboard.json key positions from the plate SVG
```

## How the "not a normal matrix" is handled

Electrically it **is** still a matrix: 6 row wires (the yellow lines) and
16 column wires (the pink lines). Each switch + diode sits where one row wire
and one column wire meet. What's unusual is that the column wires snake
across the curved board, so a "column" is not a straight vertical line of
keys, and some row/column crossings have no switch at all (6 x 16 = 96
spots, 83 switches).

QMK handles this in `keyboard.json`: every key lists its own
`"matrix": [row, col]`. The `LAYOUT(...)` macro in `keymap.c` then takes keys
in plain reading order (left to right, top to bottom), and QMK puts each one
into the right matrix slot. Empty slots are `KC_NO` automatically.

| Column wire | Keys on it (top -> bottom) |
|---|---|
| C0  | Esc, PrtSc, Caps, Tab, Shift, Ctrl |
| C1  | `, Q |
| C2  | F1, 1, W, A |
| C3  | F2, 2, E, S, Z, GUI |
| C4  | F3, 3, R, D, X, Alt |
| C5  | F4, 4, T, F, C, Fn |
| C6  | F5, 5, Y, G, V, Del |
| C7  | F6, 6, U, H, B |
| C8  | F7, 7, I, J, N, Space |
| C9  | F8, 8, O, K, M |
| C10 | F9, 9, P, L, `,`, RAlt |
| C11 | F10, 0, [, ;, `.`, RCtrl |
| C12 | F11, -, ], ', /, Left |
| C13 | =, \\, Home, Up, Down |
| C14 | F12, PgUp |
| C15 | Ins, Bksp, Enter, PgDn, End, Right |

Rows: R0 = F-key row, R1 = number row, R2 = QWERTY row, R3 = ASDF row,
R4 = ZXCV row, R5 = bottom row.

## Pins: **edit these to match your wiring**

The pins in `keyboard.json` are placeholders until the real wiring is known:

| Signal | Pico pin (placeholder) |
|---|---|
| Columns C0 ... C15 | GP0 ... GP15 |
| Rows R0 ... R5 | GP16 ... GP21 |
| Trackpad SDA / SCL | GP26 / GP27 (I2C1) |

`"diode_direction": "COL2ROW"` means the diode's black stripe (cathode) points
toward the **row** wire. If your stripes point toward the column wires, change
it to `"ROW2COL"`.

## Build & flash

```bash
# one-time
python3 -m pip install qmk
qmk setup                      # clones qmk_firmware to ~/qmk_firmware
cp -r keyboards/curve83 ~/qmk_firmware/keyboards/

# build
qmk compile -kb curve83 -km default        # -> curve83_default.uf2
```

Flash: hold **BOOTSEL** on the Pico while plugging it in. It shows up as a USB
drive called `RPI-RP2`. Drag the `.uf2` file onto it. After the first flash
you can use Fn + Esc instead of BOOTSEL.

## Check the wiring first (recommended)

```bash
qmk compile -kb curve83 -km matrix_test
```

Flash it, run `qmk console`, and press every key. Each press prints
`row R, col C -> label`. If a key prints nothing, check that switch's diode
and wires. If it prints the wrong label, fix that key's `"matrix"` entry in
`keyboard.json` (or swap the pin order). Moving the trackpad prints x/y.
Then flash `default`.
