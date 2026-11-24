#!/usr/bin/env python3
"""Generate the "layouts" block of keyboard.json from the laser-cut plate SVG.

Usage:  python3 tools/gen_layout.py plate.png > layout.json
plate.png = the plate SVG rendered at 2330x1796 px (white background, red outlines).

Every switch hole is found by flood-filling the inside of each cut-out; its centre
and rotation become the key's x/y/r in keyboard units (1u = 19.05 mm), and the
HOLE -> (label, row, col) table below says which matrix position it is wired to.
"""
import sys, json, math
from collections import deque
import numpy as np
from PIL import Image

PX_PER_MM = 2330 / 308.2688          # render width / plate width in the SVG
U = 19.05 * PX_PER_MM                 # one key unit in px

# hole index (sorted as found by scan order, see docs/plate_holes.png) -> label, row, col
HOLE = {
  68:("Esc",0,0), 60:("F1",0,2), 53:("F2",0,3), 41:("F3",0,4), 31:("F4",0,5), 23:("F5",0,6),
  16:("F6",0,7), 11:("F7",0,8), 8:("F8",0,9), 5:("F9",0,10), 3:("F10",0,11), 2:("F11",0,12),
  1:("F12",0,14), 0:("Ins",0,15),
  73:("PrtSc",1,0), 67:("`",1,1), 61:("1",1,2), 55:("2",1,3), 48:("3",1,4), 38:("4",1,5),
  30:("5",1,6), 26:("6",1,7), 18:("7",1,8), 15:("8",1,9), 12:("9",1,10), 9:("0",1,11),
  7:("-",1,12), 6:("=",1,13), 4:("Bksp",1,15),
  77:("Caps",2,0), 72:("Q",2,1), 66:("W",2,2), 63:("E",2,3), 56:("R",2,4), 50:("T",2,5),
  42:("Y",2,6), 36:("U",2,7), 28:("I",2,8), 25:("O",2,9), 20:("P",2,10), 17:("[",2,11),
  14:("]",2,12), 13:("\\",2,13), 10:("Enter",2,15),
  80:("Tab",3,0), 75:("A",3,2), 70:("S",3,3), 64:("D",3,4), 58:("F",3,5), 54:("G",3,6),
  46:("H",3,7), 39:("J",3,8), 34:("K",3,9), 29:("L",3,10), 27:(";",3,11), 24:("'",3,12),
  22:("Home",3,13), 21:("PgUp",3,14), 19:("PgDn",3,15),
  81:("Shift",4,0), 76:("Z",4,3), 71:("X",4,4), 65:("C",4,5), 62:("V",4,6), 57:("B",4,7),
  51:("N",4,8), 45:("M",4,9), 40:(",",4,10), 37:(".",4,11), 35:("/",4,12), 33:("Up",4,13),
  32:("End",4,15),
  82:("Ctrl",5,0), 79:("GUI",5,3), 78:("Alt",5,4), 74:("Fn",5,5), 69:("Del",5,6),
  59:("Space",5,8), 52:("RAlt",5,10), 49:("RCtrl",5,11), 47:("Left",5,12), 44:("Down",5,13),
  43:("Right",5,15),
}

img = np.array(Image.open(sys.argv[1]).convert("L")) < 200
H, W = img.shape
seen = np.zeros_like(img)
holes = []
for y in range(0, H, 3):
    for x in range(0, W, 3):
        if img[y, x] or seen[y, x]:
            continue
        q, pts = deque([(y, x)]), []
        seen[y, x] = True
        while q:
            a, b = q.popleft(); pts.append((a, b))
            for c, d in ((a+1, b), (a-1, b), (a, b+1), (a, b-1)):
                if 0 <= c < H and 0 <= d < W and not img[c, d] and not seen[c, d]:
                    seen[c, d] = True; q.append((c, d))
        if 5000 < len(pts) < 20000:
            p = np.array(pts, float)
            cy, cx = p.mean(0)
            far = p[np.argmax((p[:, 0]-cy)**2 + (p[:, 1]-cx)**2)]   # a corner
            ang = math.degrees(math.atan2(far[0]-cy, far[1]-cx)) - 45
            ang = ang % 90 - 90          # square is symmetric mod 90; keys tilt CCW (r <= 0)
            holes.append((cx, cy, ang))
assert len(holes) == len(HOLE) == 83, len(holes)

minx = min(h[0] for h in holes); miny = min(h[1] for h in holes)
keys = []
for i, (cx, cy, ang) in enumerate(holes):
    label, r, c = HOLE[i]
    ux, uy = (cx - minx) / U, (cy - miny) / U
    keys.append({"label": label, "matrix": [r, c],
                 "x": round(ux, 3), "y": round(uy, 3),
                 "r": round(ang, 1), "rx": round(ux + 0.5, 3), "ry": round(uy + 0.5, 3)})
keys.sort(key=lambda k: (k["matrix"][0], k["matrix"][1]))
json.dump(keys, sys.stdout, indent=None)
