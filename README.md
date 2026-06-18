# image2eink

This is a small & simple tool to resize and convert PNG/JPEG/BMP image to E-Ink usable BMP image with Floyd-Steinberg algorithm.

### Usage
```
image2eink [-w <width> -h <height>] [-p <colours>] [FILE...]
```

`-p <colours>` supports pass a string to set the colours in pallette. Each character in colours string represent a colour.<br />
**Only 6 colours are support**
- d -> Black (Dark)
- w -> White
- y -> Yellow
- r -> Red
- b -> Blue
- g -> Green

For example, "dw" means only use black and white to generate output image.

The default pallette colours setting is "dwyrbg", and default resolution is width 800 and height 480

### Credits
- [nothings/stb](https://github.com/nothings/stb)
  - License: Public Domain
