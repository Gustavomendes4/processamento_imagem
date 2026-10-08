
CC=gcc
AR=ar -rcs

CFLAGS=-Wall -Icomm

OUT_FOLDER=./out

COMM_LIB= $(OUT_FOLDER)/libcomm.a


.PHONY: clean comm

all: $(OUT_FOLDER) build

build: lbph.c comparador.c $(COMM_LIB)
	
	$(CC) lbph.c $(COMM_LIB) $(CFLAGS) -o $(OUT_FOLDER)/lbph.out
	$(CC) comparador.c $(COMM_LIB) $(CFLAGS) -o $(OUT_FOLDER)/compare.out


# Transformamos o 'comm' em uma regra real de arquivo. 
$(COMM_LIB): ./comm/bmp.c ./comm/histogram.c
	$(CC) ./comm/bmp.c -c -o $(OUT_FOLDER)/bmp.o
	$(CC) ./comm/histogram.c -c -o $(OUT_FOLDER)/histogram.o
	$(AR) $(COMM_LIB) $(OUT_FOLDER)/bmp.o $(OUT_FOLDER)/histogram.o
	rm -f $(OUT_FOLDER)/bmp.o $(OUT_FOLDER)/histogram.o

# Regra para criar a pasta 'out' caso ela não exista
$(OUT_FOLDER):
	mkdir -p $(OUT_FOLDER)

clean:
	rm -rf $(OUT_FOLDER)

