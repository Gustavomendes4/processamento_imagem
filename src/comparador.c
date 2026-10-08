#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "comm/bmp.h"
#include "comm/histogram.h"


const char* HIST_PATH   = "C:\\Users\\Gustavo\\Desktop\\img\\out\\andre.csv";


BMPImage lbp(BMPImage src){

    /* Validate input */
    if( !src.isValid){
        return INVALID_BMP;
    }

    if( src.width <= 2 || src.height <= 2){
        return INVALID_BMP;
    }

    /* Create output image */
    BMPImage dst = bmp_create(src.width, src.height);

    if(!dst.isValid){
        return INVALID_BMP;
    }


    /* Apply LBP */
    for (int y = 1; y < src.height - 1; y++) {
        
        for (int x = 1; x < src.width - 1; x++) {
        
            uint8_t centro = src.pixelData[y][x];
                
            uint8_t codigo_lbp = 0;

            if (src.pixelData[y-1][x-1] >= centro) codigo_lbp |= 128;
            if (src.pixelData[y-1][x]   >= centro) codigo_lbp |= 64;
            if (src.pixelData[y-1][x+1] >= centro) codigo_lbp |= 32;
            if (src.pixelData[y][x+1]   >= centro) codigo_lbp |= 16;
            if (src.pixelData[y+1][x+1] >= centro) codigo_lbp |= 8;
            if (src.pixelData[y+1][x]   >= centro) codigo_lbp |= 4;
            if (src.pixelData[y+1][x-1] >= centro) codigo_lbp |= 2;
            if (src.pixelData[y][x-1]   >= centro) codigo_lbp |= 1;

            dst.pixelData[y][x] = codigo_lbp;

        }
    }

    return dst;
}


int main(int argc, char* argv[]){

    if(argc < 2){
        fprintf(stderr, "Not enough arguments. Use: me.exe {image_path}\n");
        return -1;
    }

    /* Open image */
    BMPImage imagem = bmp_load(argv[1]);

    if(imagem.isValid == 0){
        printf("Erro ao carregar a imagem %s\n", argv[1]);
        return 1;
    }

    /* Open CSV reference file */
    double histogramas_reff[BLOCOS_Y][BLOCOS_X][256] = {0};

    if(load_histogram_double(HIST_PATH, histogramas_reff) != 0){
        printf("Erro ao carregar o arquivo de histograma %s\n", HIST_PATH);
        bmp_free(&imagem);
        return 1;
    }


    /* Create LBP image */
    BMPImage img_lbp = lbp(imagem);

    if(img_lbp.isValid == 0){
        printf("Erro ao aplicar o algoritmo LBP na imagem\n");
        bmp_free(&imagem);
        return 1;
    }

    // /* Save LBP image */
    // if(!bmp_save(OUT_PATH, &img_lbp)){
    //     printf("Erro ao salvar a imagem LBP em %s\n", OUT_PATH);
    //     bmp_free(&imagem);
    //     bmp_free(&img_lbp);
    //     return 1;
    // }

    /* Calculate histograms */
    int histogramas_input[BLOCOS_Y][BLOCOS_X][256] = {0};

    calc_histograms(img_lbp, histogramas_input);
    
    /* Normalize histogram */
    double normalized_histogramas[BLOCOS_Y][BLOCOS_X][256] = {0};
    normalize_histogram(normalized_histogramas, histogramas_input);

    /* get distance */
    double distance;
    distance = euclidian_distance_double(normalized_histogramas, histogramas_reff);

    printf("Distancia Euclidiana: %f\n", distance);

    /* Free memory */
    bmp_free(&imagem);
    bmp_free(&img_lbp);

    return 0;
}
