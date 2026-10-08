#ifndef HISTOGRAM_H_INCLUDED
#define HISTOGRAM_H_INCLUDED

#include <stdint.h>

#include "bmp.h"

#define BLOCOS_Y 6
#define BLOCOS_X 5

int calc_histograms(BMPImage imagem, int histogramas[BLOCOS_Y][BLOCOS_X][256]);

int normalize_histogram(double dest[BLOCOS_Y][BLOCOS_X][256], int src[BLOCOS_Y][BLOCOS_X][256]);

int calc_histograms(BMPImage imagem, int histogramas[BLOCOS_Y][BLOCOS_X][256]);

int save_histograms(const char* filename, int histogramas[BLOCOS_Y][BLOCOS_X][256]);

int save_histograms_double(const char* filename, double histogramas[BLOCOS_Y][BLOCOS_X][256]);

int load_histogram(const char* filename, int histogramas[BLOCOS_Y][BLOCOS_X][256]);

int load_histogram_double(const char* filename, double histogramas[BLOCOS_Y][BLOCOS_X][256]);

double euclidian_distance_double(
    double h1[BLOCOS_Y][BLOCOS_X][256],
    double h2[BLOCOS_Y][BLOCOS_X][256]
);

double euclidian_distance(
    int h1[BLOCOS_Y][BLOCOS_X][256],
    int h2[BLOCOS_Y][BLOCOS_X][256]
);



#endif // BMP_H_INCLUDED