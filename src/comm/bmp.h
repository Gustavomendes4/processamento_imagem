#ifndef BMP_H_INCLUDED
#define BMP_H_INCLUDED

#include <stdint.h>

#define INVALID_BMP ((BMPImage){0})

#pragma pack(push, 1)
typedef struct _BITMAPFILEHEADER{
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct _BITMAPINFOHEADER{
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;

typedef struct _RGBQUAD{
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
    uint8_t rgbReserved;
} RGBQUAD;

#pragma pack(pop)

typedef struct _BMPImage{
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    uint8_t** pixelData;
    
    int width, height;

    int isValid;

}BMPImage;

BMPImage bmp_create(int width, int height);

BMPImage bmp_load(const char* filename);

int bmp_save(const char* filename, BMPImage* image);

void bmp_free(BMPImage* image);

#endif // BMP_H_INCLUDED