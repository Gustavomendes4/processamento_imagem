
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

const char* REGISTER_PROGRAM = "C:\\Users\\Gustavo\\Desktop\\img\\src\\lbph.exe";

const char* BASE_INPUT_PATH = "C:\\Users\\Gustavo\\Desktop\\img\\images\\perfil";

const char* BASE_OUTPUT_PATH = "C:\\Users\\Gustavo\\Desktop\\img\\out2";


typedef struct _Cases{

    const char* name;

    const char* image;

}Case;

static const Case cases[] = {

    { "andre",   "andre_1.bmp" },
    { "gabriel", "gabriel_camargo_1.bmp" },
    { "gustavo", "gustavo_mendes_1.bmp"},
    { "iara",    "iara_1.bmp"},
    { "kauan",   "kauan_1.bmp"},
    { "leonardo", "leonardo_mittmann_pagnossin_1.bmp"},
    { "leticia", "leticia_1.bmp"},
    { "luana",   "luana_1.bmp"},
    { "manuela", "manu_1.bmp"},
    { "matheus", "matheus_fick_1.bmp"},
    { "natan",   "natan_1.bmp"},
    { "rafael",  "rafael_borba_1.bmp"},

    {NULL, NULL}
};

char* alloc_command_space(){

    size_t total = 0;
    
    total += (3 * PATH_MAX); // space to .exe, .bmp e .csv

    return calloc(total, sizeof(char));
}

int main(){

    char* command = alloc_command_space();

    if(command == NULL){
        fprintf(stderr, "Erro ao allocar memoria");
    }

    for(int i = 0; cases[i].image; i++){

        sprintf(
            command,
            "%s %s\\%s %s\\%s.csv",

            REGISTER_PROGRAM,

            BASE_INPUT_PATH,
            cases[i].image,

            BASE_OUTPUT_PATH,
            cases[i].name
        );


        system(command);
        // printf("%s\n", command);
        
    }

    return 0;

}