#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../bmp.h"

#define BLOCOS_Y 6
#define BLOCOS_X 5

const char* IMAGE_PATH = "C:\\Users\\Gustavo\\Desktop\\img\\images\\icon.bmp";

const char* OUTPUT_PATH = "C:\\Users\\Gustavo\\Desktop\\img\\images\\icon_out.bmp";

int main(){

    int w, h;
    uint8_t** imagem_lbp;

    BMPImage imagem = bmp_load(IMAGE_PATH);

    BMPImage imagem2 = bmp_create(imagem.width, imagem.height);

    for( int x = 0; x < imagem2.width; x++ ){
        for( int y = 0; y < imagem2.height; y++ ){
            imagem2.pixelData[y][x] = imagem.pixelData[y][x];
        }
    }
    
    if( bmp_save(OUTPUT_PATH, &imagem2) ){
        printf("Imagem salva com sucesso em %s\n", OUTPUT_PATH);
    } else {
        printf("Falha ao salvar a imagem em %s\n", OUTPUT_PATH);
    }

    bmp_free(&imagem);
    bmp_free(&imagem2);
    
    return 0;
}