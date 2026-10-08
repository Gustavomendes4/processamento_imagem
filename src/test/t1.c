#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bmp.h"

#define BLOCOS_Y 6
#define BLOCOS_X 5

const char* IMAGE_PATH = "C:\\Users\\Gustavo\\Desktop\\img\\images\\icon.bmp";

const char* OUTPUT_PATH = "C:\\Users\\Gustavo\\Desktop\\img\\images\\icon_out.bmp";

int main(){

    int w, h;
    uint8_t** imagem_lbp;

    BMPImage imagem = bmp_load(IMAGE_PATH);
    
    if( bmp_save(OUTPUT_PATH, &imagem) ){
        printf("Imagem salva com sucesso em %s\n", OUTPUT_PATH);
    } else {
        printf("Falha ao salvar a imagem em %s\n", OUTPUT_PATH);
    }

    bmp_free(&imagem);
    
    return 0;
}