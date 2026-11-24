# Curve83 keyboard firmware (Raspberry Pi Pico)

Firmware for a curved one-handed 83-key keyboard, hand-wired to a Raspberry Pi Pico.
**You don't need to install anything to flash it.**

## Flash it in 3 steps

1. **Download** [`firmware/curve83_default.uf2`](firmware/curve83_default.uf2)
   (open the link, then click the download button on the right, "Download raw file").
2. **Hold the white BOOTSEL button** on the Pico, plug the USB cable into your
   computer, then let go. A USB drive called **`RPI-RP2`** appears.
3. **Drag the `.uf2` file onto `RPI-RP2`.** The drive disappears by itself in a
   second or two. That means it worked, and the keyboard is ready to type.

Works on Windows, macOS, Linux and ChromeOS. No drivers needed.

To flash again later you don't need BOOTSEL: hold **Fn** and press **Esc**, and
`RPI-RP2` appears again.

### If something doesn't work

| Problem | Fix |
|---|---|
| `RPI-RP2` never shows up | Use a different USB cable. Many cables only charge and can't carry data. Make sure BOOTSEL is held *before* the cable goes in. |
| Windows/macOS says the drive was "not ejected properly" | Normal. The Pico restarts as soon as the file lands. |
| Some keys don't type, or type the wrong thing | Flash [`firmware/curve83_matrix_test.uf2`](firmware/curve83_matrix_test.uf2) the same way, then see **Wiring test** below. |
| Whole rows or columns are dead | That wire isn't reaching its Pico pin. Check against the pin table below. |

### Wiring test (optional)

`curve83_matrix_test.uf2` makes every key type nothing, and instead reports
which wires each press closes:

1. Flash `curve83_matrix_test.uf2`.
2. Open [QMK Toolbox](https://github.com/qmk/qmk_toolbox/releases) (Windows/macOS)
   or run `qmk console` (Linux). Leave it open.
3. Press each key. You should see lines like `pressed: row 3, col  5  ->  F`.
4. Compare with [`docs/matrix_map.png`](docs/matrix_map.png). A key that prints
   nothing has a bad diode or solder joint. A key that prints the wrong name is
   wired to a different row or column than expected.
5. Press **Esc** to get back to `RPI-RP2`, then flash `curve83_default.uf2`.

## Layout

```
Esc  F1 F2 F3 F4 F5 F6 F7 F8 F9 F10 F11 F12 Ins
PrSc `  1  2  3  4  5  6  7  8  9   0   -   =   Bksp
Caps Q  W  E  R  T  Y  U  I  O  P   [   ]   \   Enter
Tab  A  S  D  F  G  H  J  K  L  ;   '   Home PgUp PgDn
Shft Z  X  C  V  B  N  M  ,  .  /   Up  End
Ctrl GUI Alt Fn Del  Space  RAlt RCtl Left Down Right
```

Hold **Fn** for:

| Fn + | Does |
|---|---|
| Esc | Flashing mode (`RPI-RP2` drive) |
| F1 / F2 | Screen brightness down / up |
| F5 / F6 / F7 | Previous / play-pause / next track |
| F9 / F10 / F11 | Mute / volume down / volume up |
| Space / RAlt / Del | Left / right / middle mouse click |
| Up / Down | Scroll up / down |

## Pins

| Signal | Pico pin |
|---|---|
| Columns C0, C1, ... C15 | GP0, GP1, ... GP15 |
| Row R5 (spacebar row) | GP16 |
| Row R4 (ZXCV row) | GP17 |
| Row R3 (ASDF row) | GP18 |
| Row R2 (QWERTY row) | GP19 |
| Row R1 (number row) | GP20 |
| Row R0 (F-key row) | GP21 |

Diodes: black stripe (cathode) faces the row wire (toward the spacebar), so
`"diode_direction": "COL2ROW"`. No trackpad yet.

---

# For people changing the firmware

```
keyboards/curve83/
├── keyboard.json          # MCU, pins, diode direction, and the key -> (row, col) map
├── config.h               # bootloader settings (trackpad goes here later)
└── keymaps/
    ├── default/keymap.c   # the layout (base layer + Fn layer)  <- edit this to remap keys
    └── matrix_test/       # wiring check: prints row/col of each key press
firmware/*.uf2             # ready-to-flash builds of the two keymaps
build.sh                   # rebuilds firmware/*.uf2 after you edit something
docs/matrix_map.png        # every key with its row wire and column wire
docs/plate_holes.png       # plate holes numbered (used by tools/gen_layout.py)
tools/gen_layout.py        # regenerates keyboard.json key positions from the plate SVG
```

### Rebuilding after an edit

**Easiest: let GitHub build it.** Push your change, open the repo's **Actions**
tab, click the newest "Build firmware" run, and download `curve83-firmware`
from the **Artifacts** box (a zip with both `.uf2` files). Push a tag
(`git tag v1.1 && git push --tags`) to also attach them to a Release.

**On your own computer:**

```bash
./build.sh
```

This puts fresh `.uf2` files in `firmware/`. The first run downloads QMK
(about 1 GB). It needs `git`, `python3` and the ARM compiler:

| System | Install the compiler with |
|---|---|
| Ubuntu / WSL | `sudo apt install gcc-arm-none-eabi libnewlib-arm-none-eabi` |
| macOS | `brew install arm-none-eabi-gcc` |
| Windows | Install [QMK MSYS](https://msys.qmk.fm) and run `./build.sh` inside it |

Commit the updated `firmware/*.uf2` so the download links above stay current.

### How the "not a normal matrix" is handled

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
