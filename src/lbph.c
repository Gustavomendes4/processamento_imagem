#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "comm/bmp.h"
#include "comm/histogram.h"

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

    /* validate input */
    if(argc < 3){
        fprintf(stderr, "Not enough arguments. Use: me.exe {image_path} {out_csv_path}\n");
        return -1;
    }

    /* Open image */
    BMPImage imagem = bmp_load(argv[1]);

    if(imagem.isValid == 0){
        fprintf(stderr, "Erro ao carregar a imagem %s\n", argv[1]);
        return 1;
    }

    /* Create LBP image */
    BMPImage img_lbp = lbp(imagem);

    if(img_lbp.isValid == 0){
        fprintf(stderr, "Erro ao aplicar o algoritmo LBP na imagem\n");
        bmp_free(&imagem);
        return 1;
    }

    /* save LBP image */
    // bmp_save(OUTPUT_PATH, &img_lbp);


    /* Calculate histograms */
    int histogramas[BLOCOS_Y][BLOCOS_X][256] = {0};

    calc_histograms(img_lbp, histogramas);

    /* Normalize histograms */
    double normal_histograms[BLOCOS_Y][BLOCOS_X][256];

    normalize_histogram(normal_histograms, histogramas);

    /* Save histograms to file */
    int sts = save_histograms_double(argv[2], normal_histograms);

    if( sts != 0){
        fprintf(stderr, "Erro ao salvar histogramas em: %s\n", argv[2]);
        bmp_free(&imagem);
        bmp_free(&img_lbp);
        return -2;
    }
    
    /* Free memory */
    bmp_free(&imagem);
    bmp_free(&img_lbp);

    return 0;
}