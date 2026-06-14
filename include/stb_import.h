#ifndef STB_IMPORT_H
#define STB_IMPORT_H

#define STB_IMAGE_IMPLEMENTATION
#define STB_ONLY_JPEG
#define STB_ONLY_PNG
#define STB_ONLY_BMP
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize2.h>

#endif // STB_IMPORT_H
