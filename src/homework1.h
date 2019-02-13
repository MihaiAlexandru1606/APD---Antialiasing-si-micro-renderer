/**
 * @author : Niculescu Mihai Alexandru 335CB
 * @file : homework1.h
 */
#ifndef HOMEWORK_H1
#define HOMEWORK_H1

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <pthread.h>

#pragma pack(1)

/**
 * structura pentru un pixel din imaginea grayscale
 */
typedef struct {
    unsigned char pix;
}PixelGrayScale;

/**
 *  structura pentru o imagine, se va folosi o matrice sub forma de vector de
 *  dimensiune resolution ^ 2
 */
typedef struct {
    PixelGrayScale *matrix; /** matrice sub forma de vector */
}image;

#pragma pack()

#define A       -1
#define B       2
#define C       0

#define WHITE_PIXEL         255
#define BLACK_PIXEL         0
#define MAX_PIXEL           255

#define REAL_RESOLUTION      100.0

void initialize(image *img);
void render(image *img);
void writeData(const char * fileName, image *img);

#endif /* HOMEWORK_H1 */