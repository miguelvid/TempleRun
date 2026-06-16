# Temple Run 3D

First-person endless runner: you run down a corridor with **3 lanes** and must dodge
obstacles by changing lanes, jumping or ducking.

## Obstacles
- **Red (low):** JUMP over it.
- **Blue (high):** DUCK to pass under.
- **Orange (full block):** CHANGE lane.

## Controls
| Action      | Keys                  |
|-------------|-----------------------|
| Move left   | `←` or `A`            |
| Move right  | `→` or `D`            |
| Jump        | `Space`, `↑` or `W`   |
| Duck        | `↓` or `S` (hold)     |
| Restart     | `R` (on the end screen) |

Speed increases over time. Each obstacle you overtake is worth 1 point.

## Architecture (src/)
The code is split into small modules, each with a declaration header (`.h`) and a
definition file (`.c`):

- `config.h` — tunable constants.
- `player` — player state and movement.
- `obstacle` — obstacle pool, spawning and update.
- `collision` — collision test.
- `texture` — PNG/JPG texture loading (via `stb_image.h`).
- `render2d` — 2D overlay setup and the pixel-art font.
- `sprites` — first-person view-model sprites and full-screen overlays.
- `world` — corridor and obstacle drawing.
- `hud` — on-screen text (score, game over).
- `resources` — loads and holds all assets.
- `game` — game state, loop and input (orchestrates everything).
- `main` — GLUT setup and callbacks.

## Build and run (Linux)
```bash
make        # build
make run    # build and run
make clean  # remove the binary
```

Or directly:
```bash
gcc src/*.c -o templerun -lglut -lGL -lGLU -lm
./templerun
```

Dependencies (once):
```bash
sudo apt install build-essential freeglut3-dev
```
