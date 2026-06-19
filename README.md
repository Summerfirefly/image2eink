# image2eink

This is a small & simple tool to resize and convert PNG/JPEG/BMP image to E-Ink usable BMP image with Floyd-Steinberg algorithm.

### Usage
```
image2eink [-w <width> -h <height>] [-p <colours>] [-b] [-r <0|1|2|3>] [FILE...]
```

`-w <width> -h <height>` sets the width and height of output. Default: width 800, height 480

`-r <0|1|2|3>` sets the image rotate: 0 for no rotate, 1 for 90-degree clockwise, 2 for 180 degree, 3 for 90-degree anti-clockwise.<br />
If `-r` is not specified. An auto rotation will apply to image, which means input image will rotate until long side matches canvas's long side

`-b` can ask program to output a binary file, in which each byte present a colour.

`-p <colours>` supports pass a string to set the colours in pallette. Each character in colours string represent a colour.<br />
**Only 6 colours are support**
- d -> Black (Dark)
- w -> White
- y -> Yellow
- r -> Red
- b -> Blue
- g -> Green

Any other character can be place holder to adjust the binary mode output data of valid colours.

For example:
- "dw" means only use black and white to generate output image, and in binary output mode, 0x0 for black and 0x1 for white
- "daw" also means onlu black and white are used, but in binary output mode, 0x0 for black and 0x2 for white

Default: dwyrabg

### Credits
- [nothings/stb](https://github.com/nothings/stb)
  - License: Public Domain
