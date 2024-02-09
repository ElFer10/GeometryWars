# Program Specification

Este juego debe tener las siguientes características:

## Player

- The player is represented by a shape which is defined in the
  config file.
- The player must spawn in the center of the screen at the 
  beginning of the game and after it dies.
- The player moves by a speed read from the config file in these 
  directions: up (W), down (S), left (A), right (D).
- The player is confined to move only within the bounds of the 
  screen.
- The player will shoot

### Player specs

| Shape radius      | SR         | int           |
| Collision radius  | CR         | int           |
| Speed             | S          | float         |
| Fill color        | FR, FG, FB | int, int, int |
| Outiline color    | OR, OG, OB | int, int, int |
| Outline thickness | OT         | int           |
| Shape vertices    | V          | int           |

### Enemy specs

| Shape radius      | SR         | int           |
| Collision radius  | CR         | int           |
| Min/Max speed     | SMIN, SMAX | float, float  |
| Outline color     | OR, OG, OB | int, int, int |
| Outline thickness | OT         | int           |
| Min/Max vertices  | VMIN, VMAX | int, int      |
| Small Lifespan    | L          | int           |
| Spawn Interval    | SR         | int           |

### Bullet specs

| Shape radius      | SR         | int           |
| Collision radius  | CR         | int           |
| Speed             | S          | float         |
| Fill color        | FR, FG, FB | int, int, int |
| Outline color     | OR, OG, OB | int, int, int |
| Outline thickness | OT         | int           |
| Shape vertices    | V          | int           |
| Lifespan          | L          | int           |
