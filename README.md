*This project has been created as part of the 42 curriculum by mnasimi.*

# fract-ol

## Description
`fract-ol` is a real-time fractal exploration program using MiniLibX.

Project goals:
- render fractals in a graphical window;
- support smooth interaction (zoom, movement, controls);
- manage events and exits cleanly;
- implement mandatory and bonus features from the subject.

Implemented fractals:
- Mandelbrot
- Julia (with optional custom parameters)
- Burning Ship (bonus fractal)

## Instructions
### Prerequisites
- Linux + X11 development libraries
- `cc`
- MiniLibX Linux source in `minilibx-linux/`

### Build
```bash
make
```

### Run
```bash
./fractol mandelbrot
./fractol julia
./fractol julia -0.8 0.156
./fractol burningship
```

### Clean
```bash
make clean
make fclean
make re
```

### Bonus target
```bash
make bonus
```

## Controls
- `Mouse Wheel Up`: zoom in
- `Mouse Wheel Down`: zoom out
- `Left / Right / Up / Down`: move view
- `C`: shift color palette forward
- `V`: shift color palette backward
- `R`: reset view
- `ESC`: close program
- Window close button (`X`): close program

## Technical Summary
- Escape-time rendering with complex plane mapping.
- Mandelbrot and Burning Ship start from `z = 0`.
- Julia starts from pixel-mapped `z` and uses command-line constant `c`.
- Colors are generated from iteration depth with a smooth RGB gradient.
- Color range shifting is applied by palette index offset.
- Zoom is centered on mouse position for precise deep exploration.

Source files:
- `main.c`: initialization, graphics setup, hooks, main loop
- `events.c`: keyboard/mouse controls and default-view reset
- `render.c`: fractal iteration, palette generation, and pixel rendering
- `utils.c`: strict argument parsing and numeric parser for Julia
- `fractol.h`: shared declarations
- `Makefile`: build rules (`all`, `bonus`, `clean`, `fclean`, `re`)

## Mandatory + Bonus Status
Mandatory:
- MiniLibX window + image rendering
- Mandelbrot + Julia
- mouse-wheel zoom
- Julia parameter support
- invalid/no argument handling with usage output
- ESC + window close clean exit
- multi-color depth rendering

Bonus:
- extra fractal (Burning Ship)
- zoom follows mouse position
- arrow-key movement
- color range shifting

## Resources
Classic references:
- Benoit Mandelbrot, *The Fractal Geometry of Nature*
- MiniLibX manual pages and 42 documentation
- Complex numbers and escape-time fractal references

AI usage:
- AI was used to check subject constraints and help draft this README.
- Final code behavior and project logic were verified against the source files.
