# image2eink

This is a small & simple tool to resize and convert PNG/JPEG/BMP image to E-Ink usable BMP image with Floyd-Steinberg algorithm.

### Usage
```
image2eink [-w <width> -h <height>] [-p <colours>] [-b] [FILE...]
```

`-b` can ask program to output a binary file, in which each byte present a colour.<br />
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

The default pallette colours setting is "dwyrabg", and default resolution is width 800 and height 480

### Credits
- [nothings/stb](https://github.com/nothings/stb)
  - License: Public Domain
