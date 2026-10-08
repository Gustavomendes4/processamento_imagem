#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "comm/bmp.h"
#include "comm/histogram.h"

const char* DATABASE_PATH = "C:\\Users\\Gustavo\\Desktop\\img\\out2";

typedef struct _DB{

    const char* name;
    const char* path;

}DB;

static const DB db[] = {
    
    {"andre",   "andre.csv" },
    {"gabriel", "gabriel.csv" },
    {"gustavo", "gustavo.csv" },
    {"iara",    "iara.csv" },
    {"kauan",   "kauan.csv" },
    {"leonardo", "leonardo.csv" },
    {"leticia", "leticia.csv" },
    {"luana",   "luana.csv" },
    {"manuela", "manuela.csv" },
    {"matheus", "matheus.csv" },
    {"natan",   "natan.csv" },
    {"rafael",  "rafael.csv" },
    {"maryanna",  "maryanna.csv" },

    {NULL, NULL}
};

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

    /* Create LBP image */
    BMPImage img_lbp = lbp(imagem);

    if(img_lbp.isValid == 0){
        printf("Erro ao aplicar o algoritmo LBP na imagem\n");
        bmp_free(&imagem);
        return 1;
    }

    /* Calculate histograms */
    int raw_hist[BLOCOS_Y][BLOCOS_X][256] = {0};

    calc_histograms(img_lbp, raw_hist);
    
    /* Normalize histogram */
    double input_hist[BLOCOS_Y][BLOCOS_X][256] = {0};
    normalize_histogram(input_hist, raw_hist);

    /* close images */
    bmp_free(&img_lbp);
    bmp_free(&imagem);

    

    /* Varre DB comparando LBPH */
    char path[PATH_MAX];
    double curr_hist[BLOCOS_Y][BLOCOS_X][256] = {0};

    const char *min_n, *max_n;
    double min, max;
    
    for( size_t i = 0; db[i].name != NULL; i++){

        /* cria PATH do csv */
        sprintf(path, "%s\\%s\0", DATABASE_PATH, db[i].path);

        /* Open CSV reference file */
        if(load_histogram_double(path, curr_hist) != 0){
            printf("Erro ao carregar o arquivo de histograma %s\n", path);
            return 1;
        }

        /* Compare histograms*/
        double distance = euclidian_distance_double(curr_hist, input_hist);

        printf("[%02d] (%.4lf) %s\n", i, distance, db[i].name);

        /* save metrics */
        if( min > distance || i == 0){
            min = distance;
            min_n = db[i].name;
        }

        if( max < distance || i == 0){
            max = distance;
            max_n = db[i].name;
        }


    }

    printf("\n\nMais proximo:  (%lf) %s", min, min_n);
    printf("\nMais distante: (%lf) %s", max, max_n);

    return 0;
}
