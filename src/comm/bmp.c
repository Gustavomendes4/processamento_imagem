
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

static uint8_t** alloc_matrix(int height, int width) {

    uint8_t** matrix = malloc(height * sizeof(uint8_t*));
    
    if (matrix == NULL) return NULL;

    for (int i = 0; i < height; i++) {

        // matrix[i] = malloc(width * sizeof(uint8_t));
        matrix[i] = calloc(width, sizeof(uint8_t));
        
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

BMPImage bmp_create(int width, int height){

    if (width <= 0 || height <= 0)
        return INVALID_BMP;

    BMPImage image;
    image.width = width;
    image.height = height;
    image.isValid = 1;

    image.pixelData = alloc_matrix(height, width);
    
    if (image.pixelData == NULL) {
        image.isValid = 0;
        return INVALID_BMP;
    }

    return image;
}

BMPImage bmp_load(const char* filename){

    FILE* file = fopen(filename, "rb");
    if (!file) { printf("Erro ao abrir %s\n", filename); exit(1); }

    BITMAPFILEHEADER fh; BITMAPINFOHEADER ih;
    int width, height;

    /* Read headers */
    if(
        fread(&fh, sizeof(BITMAPFILEHEADER), 1, file) != 1 ||
        fread(&ih, sizeof(BITMAPINFOHEADER), 1, file) != 1
    ){
        fclose(file);
        return INVALID_BMP;
    }

    width = ih.biWidth;
    height = abs(ih.biHeight);
    
    /* Read pallet */
    RGBQUAD paleta[256];
    if (ih.biBitCount == 8) fread(paleta, sizeof(RGBQUAD), 256, file);

    /* Truncate padding to multiple of 4 */
    int padding = (4 - (ih.biWidth % 4)) % 4;
    if (ih.biBitCount == 24) padding = (4 - (ih.biWidth * 3 % 4)) % 4;

    /* alloc matrix */
    uint8_t** matrix = alloc_matrix(height, width);

    if( matrix == NULL ){
        fclose(file);
        return INVALID_BMP;
    }
    /* Read pixel data */
    fseek(file, fh.bfOffBits, SEEK_SET);

    /* Read pixels */
    for (int y = 0; y < height; y++) {

        int row = ih.biHeight > 0 ? height - 1 - y : y;

        for(int x = 0; x < width; x++) {

            if (ih.biBitCount == 8) {
                uint8_t indice;
                fread(&indice, sizeof(indice), 1, file);

                matrix[row][x] =
                    (paleta[indice].rgbRed +
                    paleta[indice].rgbGreen +
                    paleta[indice].rgbBlue) / 3;
            }

            else{
                uint8_t color[3];
                fread(color, sizeof(uint8_t), 3, file);

                matrix[row][x] = (color[0] + color[1] + color[2]) / 3;
            }
        }

        fseek(file, padding, SEEK_CUR);
    }

    fclose(file);
    
    /* Save values to struct and return */
    BMPImage image;
    image.fileHeader = fh;
    image.infoHeader = ih;
    image.pixelData = matrix;

    image.height = height;
    image.width = width;
    image.isValid = 1;

    return image;
}

int bmp_save(const char* filename, BMPImage* image){

    if (filename == NULL || image == NULL ||
        !image->isValid || image->pixelData == NULL ||
        image->width <= 0 || image->height <= 0
    )
        return 0;

    FILE* file = fopen(filename, "wb");
    if (file == NULL)
        return 0;

    BITMAPFILEHEADER fh = {0};
    BITMAPINFOHEADER ih = {0};
    RGBQUAD palette[256] = {0};

    int width = image->width;
    int height = image->height;

    /* Calculate row size and padding */
    int rowSize = (width + 3) & ~3;
    int padding = rowSize - width;

    /* Configure file header */
    fh.bfType = 0x4D42;
    fh.bfOffBits = sizeof(BITMAPFILEHEADER)
                 + sizeof(BITMAPINFOHEADER)
                 + sizeof(palette);
    fh.bfSize = fh.bfOffBits + rowSize * height;

    /* Configure information header */
    ih.biSize = sizeof(BITMAPINFOHEADER);
    ih.biWidth = width;
    ih.biHeight = height;
    ih.biPlanes = 1;
    ih.biBitCount = 8;
    ih.biCompression = 0; /* BI_RGB */
    ih.biSizeImage = rowSize * height;
    ih.biClrUsed = 256;
    ih.biClrImportant = 256;

    /* Create grayscale palette */
    for (int i = 0; i < 256; i++) {
        palette[i].rgbBlue = i;
        palette[i].rgbGreen = i;
        palette[i].rgbRed = i;
        palette[i].rgbReserved = 0;
    }

    /* Write headers and palette */
    if (fwrite(&fh, sizeof(fh), 1, file) != 1 ||
        fwrite(&ih, sizeof(ih), 1, file) != 1 ||
        fwrite(palette, sizeof(palette), 1, file) != 1) {
        fclose(file);
        return 0;
    }

    /* Write pixel data */
    uint8_t pad[3] = {0};

    for (int y = height - 1; y >= 0; y--) {
        if (fwrite(image->pixelData[y], 1, width, file) !=
            (size_t)width) {
            fclose(file);
            return 0;
        }

        if (padding > 0 &&
            fwrite(pad, 1, padding, file) != (size_t)padding) {
            fclose(file);
            return 0;
        }
    }

    /* Check file closing */
    if (fclose(file) != 0)
        return 0;

    return 1;
}

void bmp_free(BMPImage* image){
    if (image->pixelData) {
        for (int i = 0; i < image->height; i++) {
            free(image->pixelData[i]);
        }
        free(image->pixelData);
        image->pixelData = NULL;
    }

    image->isValid = 0;
}
