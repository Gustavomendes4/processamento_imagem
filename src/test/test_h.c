

#include <stdio.h>
#include <stdlib.h>

#include "../bmp.h"

#define BLOCOS_Y 6
#define BLOCOS_X 5

const char* PATH_1  = "C:\\Users\\Gustavo\\Desktop\\img\\images\\histogramas.csv";

const char* PATH_2 = "C:\\Users\\Gustavo\\Desktop\\img\\images\\histogramas2.csv";

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

        if (c != '\n' && c != EOF){
            fclose(arquivo_hist);
            return -2;
        }
    }

    fclose(arquivo_hist);
    return 0;
}


int main(){

    int histogramas_1 [BLOCOS_Y][BLOCOS_X][256] = {0};
    int histogramas_2[BLOCOS_Y][BLOCOS_X][256] = {0};


    load_histogram(PATH_1, histogramas_1);

    save_histograms(PATH_2, histogramas_1);

    
    load_histogram(PATH_2, histogramas_2);


    for( int i = 0; i < BLOCOS_Y; i++ ){

        for( int j = 0; j < BLOCOS_X; j++ ){

            for( int k = 0; k < 256; k++ ){

                if( histogramas_1[i][j][k] != histogramas_2[i][j][k] ){

                    printf("Error: histogramas_1[%d][%d][%d] = %d, histogramas_2[%d][%d][%d] = %d\n", i, j, k, histogramas_1[i][j][k], i, j, k, histogramas_2[i][j][k]);
                    return -1;

                }

            }

        }

    }

    printf("HISTOGRAMAS IGUALS !!!!!!\n");

    return 0;
}