#include <stdio.h>
#include <string.h>

#include "histogram.h"

int normalize_histogram(
    double dest[BLOCOS_Y][BLOCOS_X][256],
    int src[BLOCOS_Y][BLOCOS_X][256]
){
    for (int i = 0; i < BLOCOS_Y; i++) {

        for (int j = 0; j < BLOCOS_X; j++) {

            unsigned long total = 0;

            /* Soma apenas o histograma deste bloco */
            for (int k = 0; k < 256; k++) {
                total += src[i][j][k];
            }

            /* Normaliza este bloco */
            if (total > 0) {
                for (int k = 0; k < 256; k++) {
                    dest[i][j][k] =
                        (double)src[i][j][k] / (double)total;
                }
            }
            else {
                for (int k = 0; k < 256; k++) {
                    dest[i][j][k] = 0.0;
                }
            }
        }
    }

    return 0;
}

int calc_histograms(BMPImage imagem, int histogramas[BLOCOS_Y][BLOCOS_X][256]){

    if( !imagem.isValid){
        return -1;
    }

    memset(histogramas, 0, sizeof(int) * BLOCOS_Y * BLOCOS_X * 256);

     // 1. Calcula as dimensões base de cada bloco
    int bloco_h = imagem.height / BLOCOS_Y;
    int bloco_w = imagem.width / BLOCOS_X;

    // 2. Percorre cada um dos 20 blocos
    for (int by = 0; by < BLOCOS_Y; by++){

        for (int bx = 0; bx < BLOCOS_X; bx++) {

            // Define os limites de pixels do bloco atual
            int y_inicial = by * bloco_h;
        
            // Garante que o último bloco pegue pixels residuais de divisões não exatas
            int y_final = (by == BLOCOS_Y - 1) ? imagem.height : (by + 1) * bloco_h; 
        
            int x_inicial = bx * bloco_w;
            int x_final = (bx == BLOCOS_X - 1) ? imagem.width : (bx + 1) * bloco_w;

            
            // Varre os pixels de dentro do bloco atual
            for (int y = y_inicial; y < y_final; y++) {
                
                for (int x = x_inicial; x < x_final; x++) {
                    uint8_t valor_lbp = imagem.pixelData[y][x];
                    
                    histogramas[by][bx][valor_lbp]++;
                }
            }
        }
    }

    return 0;
}

int save_histograms(const char* filename, int histogramas[BLOCOS_Y][BLOCOS_X][256]){

    /* Open image */
    FILE* arquivo_hist = fopen(filename, "w");

    if (arquivo_hist == NULL) return -1;

    /* save histograms table */
    for(int bin = 0; bin < 256; bin++){

        for(int by = 0; by < BLOCOS_Y; by++){

            for(int bx = 0; bx < BLOCOS_X; bx++){

                int hist_id = by * BLOCOS_X + bx;

                fprintf(arquivo_hist, "%d%s", histogramas[by][bx][bin], hist_id < BLOCOS_Y * BLOCOS_X - 1 ? "," : "");

            }
        }

        fprintf(arquivo_hist, "\n");
    }

    fclose(arquivo_hist);
    return 0;
}

int save_histograms_double(const char* filename, double histogramas[BLOCOS_Y][BLOCOS_X][256]){

    /* Open image */
    FILE* arquivo_hist = fopen(filename, "w");

    if (arquivo_hist == NULL) return -1;

    /* save histograms table */
    for(int bin = 0; bin < 256; bin++){

        for(int by = 0; by < BLOCOS_Y; by++){

            for(int bx = 0; bx < BLOCOS_X; bx++){

                int hist_id = by * BLOCOS_X + bx;

                fprintf(
                    arquivo_hist,
                    "%.8f%s",
                    histogramas[by][bx][bin],
                    hist_id < BLOCOS_Y * BLOCOS_X - 1 ? "," : ""
                );

            }
        }

        fprintf(arquivo_hist, "\n");
    }

    fclose(arquivo_hist);
    return 0;
}

int load_histogram(const char* filename, int histogramas[BLOCOS_Y][BLOCOS_X][256]){

    /* Open histogram file */
    FILE* arquivo_hist = fopen(filename, "r");

    if (arquivo_hist == NULL) return -1;


    /* read histograms table */
    for(int bin = 0; bin < 256; bin++){

        for(int by = 0; by < BLOCOS_Y; by++){

            for(int bx = 0; bx < BLOCOS_X; bx++){

                int hist_id = by * BLOCOS_X + bx;

                if(
                    fscanf(
                        arquivo_hist,
                        hist_id < BLOCOS_Y * BLOCOS_X - 1 ? "%d," : "%d",
                        &histogramas[by][bx][bin]
                    ) != 1
                ){
                    fclose(arquivo_hist);
                    return -1;
                }

            }
        }

        /* consume newline */
        int c = fgetc(arquivo_hist);

        if(c == '\r') c = fgetc(arquivo_hist);

        if (c != '\n' && c != EOF){
            fclose(arquivo_hist);
            return -2;
        }
    }

    fclose(arquivo_hist);
    return 0;
}

double euclidian_distance_double(
    double h1[BLOCOS_Y][BLOCOS_X][256],
    double h2[BLOCOS_Y][BLOCOS_X][256]
){
    double distance = 0.0;

    for (int by = 0; by < BLOCOS_Y; by++) {
        for (int bx = 0; bx < BLOCOS_X; bx++) {
            for (int bin = 0; bin < 256; bin++) {

                double diff = h1[by][bx][bin] - h2[by][bx][bin];

                distance += diff * diff;
            }
        }
    }

    return (distance);
}

int load_histogram_double(const char* filename, double histogramas[BLOCOS_Y][BLOCOS_X][256]){

    /* Open histogram file */
    FILE* arquivo_hist = fopen(filename, "r");

    if (arquivo_hist == NULL) return -1;


    /* read histograms table */
    for(int bin = 0; bin < 256; bin++){

        for(int by = 0; by < BLOCOS_Y; by++){

            for(int bx = 0; bx < BLOCOS_X; bx++){

                int hist_id = by * BLOCOS_X + bx;

                if(
                    fscanf(
                        arquivo_hist,
                        hist_id < BLOCOS_Y * BLOCOS_X - 1 ? "%lf," : "%lf",
                        &histogramas[by][bx][bin]
                    ) != 1
                ){
                    fclose(arquivo_hist);
                    return -1;
                }

            }
        }

        /* consume newline */
        int c = fgetc(arquivo_hist);
        if (c == '\r') c = fgetc(arquivo_hist);

        if (c != '\n' && c != EOF){
            fclose(arquivo_hist);
            return -2;
        }
    }

    fclose(arquivo_hist);
    return 0;
}

double euclidian_distance(
    int h1[BLOCOS_Y][BLOCOS_X][256],
    int h2[BLOCOS_Y][BLOCOS_X][256]
){
    double distance = 0.0;

    for (int by = 0; by < BLOCOS_Y; by++) {
        for (int bx = 0; bx < BLOCOS_X; bx++) {
            for (int bin = 0; bin < 256; bin++) {

                double diff = h1[by][bx][bin] - h2[by][bx][bin];

                distance += diff * diff;
            }
        }
    }

    return (distance);
}
