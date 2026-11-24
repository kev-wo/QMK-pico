#!/usr/bin/env bash
# Rebuild the Curve83 firmware after editing the keymap.
#
#   ./build.sh                 # builds default + matrix_test into ./firmware/
#   ./build.sh default         # builds only the keymap you name
#
# Works on Linux, macOS, WSL, and Windows "QMK MSYS".
# First run downloads QMK (~1 GB) into ./.qmk_firmware unless QMK_HOME is set.
set -euo pipefail

REPO="$(cd "$(dirname "$0")" && pwd)"
KB=curve83
KEYMAPS=("${@:-default matrix_test}")
read -r -a KEYMAPS <<< "${KEYMAPS[*]}"
export QMK_HOME="${QMK_HOME:-$REPO/.qmk_firmware}"

if ! command -v qmk >/dev/null 2>&1; then
    echo ">> Installing the QMK command-line tool (python3 -m pip install --user qmk)"
    python3 -m pip install --user qmk
    export PATH="$HOME/.local/bin:$PATH"
fi

if [ ! -f "$QMK_HOME/Makefile" ]; then
    echo ">> Downloading QMK into $QMK_HOME (one time only)"
    git clone --depth 1 https://github.com/qmk/qmk_firmware.git "$QMK_HOME"
fi
if [ ! -f "$QMK_HOME/lib/chibios/os/license.txt" ] && [ ! -d "$QMK_HOME/lib/chibios/os" ]; then
    echo ">> Downloading QMK libraries (one time only)"
    (cd "$QMK_HOME" && git submodule update --init --depth 1 --recursive \
        lib/chibios lib/chibios-contrib lib/pico-sdk lib/printf lib/lufa lib/vusb)
fi

if ! command -v arm-none-eabi-gcc >/dev/null 2>&1; then
    echo "!! arm-none-eabi-gcc not found. Install it first:"
    echo "   Ubuntu/WSL: sudo apt install gcc-arm-none-eabi libnewlib-arm-none-eabi"
    echo "   macOS:      brew install arm-none-eabi-gcc   (or run: qmk setup)"
    echo "   Windows:    use QMK MSYS (https://msys.qmk.fm), it includes the compiler"
    exit 1
fi

rm -rf "$QMK_HOME/keyboards/$KB"
cp -r "$REPO/keyboards/$KB" "$QMK_HOME/keyboards/$KB"
mkdir -p "$REPO/firmware"

for km in "${KEYMAPS[@]}"; do
    echo ">> Building $KB:$km"
    (cd "$QMK_HOME" && make "$KB:$km" -j"$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)")
    cp "$QMK_HOME/${KB}_${km}.uf2" "$REPO/firmware/"
    echo ">> firmware/${KB}_${km}.uf2 is ready to drag onto the Pico"
done
