#ifndef IMAGE_H
#define IMAGE_H

#include "bake/bake_config.h"
#include "core/io/packed_data.h"

typedef enum {
    IMAGE_FORMAT_GRAYSCALE = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE,
    IMAGE_FORMAT_GRAY_ALPHA = PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA,
    IMAGE_FORMAT_R5G6B5 = PIXELFORMAT_UNCOMPRESSED_R5G6B5,
    IMAGE_FORMAT_R8G8B8 = PIXELFORMAT_UNCOMPRESSED_R8G8B8,
    IMAGE_FORMAT_R5G5B5A1 = PIXELFORMAT_UNCOMPRESSED_R5G5B5A1,
    IMAGE_FORMAT_R4G4B4A4 = PIXELFORMAT_UNCOMPRESSED_R4G4B4A4,
    IMAGE_FORMAT_R8G8B8A8 = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
    IMAGE_FORMAT_R32 = PIXELFORMAT_UNCOMPRESSED_R32,
    IMAGE_FORMAT_R32G32B32 = PIXELFORMAT_UNCOMPRESSED_R32G32B32,
    IMAGE_FORMAT_R32G32B32A32 = PIXELFORMAT_UNCOMPRESSED_R32G32B32A32
} ImageFormat;

typedef struct Image {
    Texture2D texture;
    int width;
    int height;
    ImageFormat format;
} Image;

Image* image_load(const char* fileName);
Image* image_load_from_memory(const void* fileData, int dataSize);
void image_unload(Image* image);
bool image_resize(Image* image, int newWidth, int newHeight);
bool image_crop(Image* image, Rectangle crop);
bool image_save(Image* image, const char* fileName);
bool image_change_format(Image* image, ImageFormat newFormat);
void image_draw(const Image* image);
unsigned char* save_image_to_memory(const Image* image, int* output_size);

#endif // IMAGE_H
