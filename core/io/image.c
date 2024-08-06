#include "image.h"


#include <stdlib.h>


Image* image_load(const char* fileName) {
    Image* image = (Image*)malloc(sizeof(Image));
    if (image == NULL) {
        return NULL;
    }

    int width, height, color_channels, bit_depth;
    unsigned char* imageData = rpng_load_image(fileName, &width, &height, &color_channels, &bit_depth);

    if (imageData == NULL) {
        free(image);
        return NULL;
    }

    Image img = {
        .data = imageData,
        .width = width,
        .height = height,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };

    image->texture = LoadTextureFromImage(img);
    image->width = image->texture.width;
    image->height = image->texture.height;
    image->format = IMAGE_FORMAT_R8G8B8A8;

    // free the image data (not needed after texture is created)
    free(imageData);

    return image;
}


Image* image_load_from_memory(const void* fileData, int dataSize) {
    Image* image = (Image*)malloc(sizeof(Image));
    if (image == NULL) {
        return NULL;
    }

    int width, height, color_channels, bit_depth;
    unsigned char* imageData = rpng_load_image_from_memory(fileData, dataSize, &width, &height, &color_channels, &bit_depth);

    if (imageData == NULL) {
        free(image);
        return NULL;
    }

    Image img = {
        .data = imageData,
        .width = width,
        .height = height,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 
    };

    image->texture = LoadTextureFromImage(img);
    image->width = image->texture.width;
    image->height = image->texture.height;
    image->format = IMAGE_FORMAT_R8G8B8A8;

    free(imageData);

    return image;
}


void image_unload(Image* image) {
    if (image != NULL) {
        UnloadTexture(image->texture);
        free(image);
    }
}


bool image_resize(Image* image, int newWidth, int newHeight) {
    if (image == NULL) return false;

    Image temp = LoadImageFromTexture(image->texture);
    image_resize(&temp, newWidth, newHeight);
    UnloadTexture(image->texture);
    image->texture = LoadTextureFromImage(temp);
    UnloadImage(temp);

    image->width = newWidth;
    image->height = newHeight;

    return true;
}


bool image_crop(Image* image, Rectangle crop) {
    if (image == NULL) return false;

    Image temp = LoadImageFromTexture(image->texture);
    image_crop(&temp, crop);
    UnloadTexture(image->texture);
    image->texture = LoadTextureFromImage(temp);
    UnloadImage(temp);

    image->width = crop.width;
    image->height = crop.height;

    return true;
}


bool image_save(Image* image, const char* fileName) {
    if (image == NULL) return false;

    Image temp = LoadImageFromTexture(image->texture);

    // save image using rpng
    bool success = rpng_save_image(fileName, temp.data, temp.width, temp.height, 4, 8); // Assuming 4 color channels (RGBA) and 8 bit depth

    UnloadImage(temp);

    return success;
}

bool image_change_format(Image* image, ImageFormat newFormat) {
    if (image == NULL) return false;

    Image temp = LoadImageFromTexture(image->texture);
    image_format(&temp, newFormat);
    UnloadTexture(image->texture);
    image->texture = LoadTextureFromImage(temp);
    UnloadImage(temp);

    image->format = newFormat;

    return true;
}

void image_draw(const Image* image) {
    if (image == NULL) return;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    DrawTexture(image->texture, (screenWidth - image->width) / 2, (screenHeight - image->height) / 2, WHITE);
}


unsigned char* save_image_to_memory(const Image* image, int* output_size) {
    if (image == NULL || output_size == NULL) return NULL;

    Image temp = LoadImageFromTexture(image->texture);

    unsigned char* imageData = rpng_save_image_to_memory(temp.data, temp.width, temp.height, 4, 8, output_size);

    UnloadImage(temp);

    return imageData;
}


static unsigned char* rpng_save_image_to_memory(const char* data, int width, int height, int color_channels, int bit_depth, int* output_size) {
    const char* tempFileName = "temp_image.png";

    rpng_save_image(tempFileName, data, width, height, color_channels, bit_depth);

    FILE* file = fopen(tempFileName, "rb");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    *output_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char* buffer = (unsigned char*)malloc(*output_size);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, *output_size, file);
    fclose(file);

    remove(tempFileName);

    return buffer;
}
