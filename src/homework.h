/**
 * @author : Niculescu Mihai Alexandru 335CB
 * @file : homework.h
 */
#ifndef HOMEWORK_H
#define HOMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

typedef enum {
    grayscale = 5,
    color = 6,
} TypeImage;

#pragma pack(1)

/**
 * Structura unui pixel "colorat"
 */
typedef struct {
    unsigned char r;  /** culoarea rosu */
    unsigned char g;  /** culoarea verde */
    unsigned char b;  /** culoarea albastru */
} PixelColor;

/**
 * Structura unui pixel "gri"
 */
typedef struct {
    unsigned char pix;
} PixelGray;

/**
 * Structura unei imagini pnm sau pgm
 */
typedef struct {
    unsigned int width;  /** latimea imaginei */
    unsigned int height; /** inaltinea imaginei */
    void *matrix;        /** matricea de pixeli */
    TypeImage typeImage; /** tipul de imagine citita */
} image;

#pragma pack()

void readInput(const char *fileName, image *img);

void writeData(const char *fileName, image *img);

void resize(image *in, image *out);

#define SPECIAL_RESIZE_FACTOR   3
#define GAUSS_KERNEL_SUM        16

#define MAX_PIXEL               255

/**
 * macroul pt determinarea start si end pentru fiecare, index-ul pixelului care
 * urmeaza sa fie calculat, fiind cuprins in [start, end]
 * se ia in considerare si cazul in care sunt mai multe thred-uri decat pixeli
 * in noua imagine
 */
#define INIT_START_END(start, end, id, med, remaind)    \
    start = id * med;                                   \
    end = start + med - 1;                              \
    if (id < remaind) {                                 \
        start += id;                                    \
        end = start + med;                              \
    } else {                                            \
        start += remaind;                               \
        end += remaind;                                 \
    }                                                   \
    if (med == 0 && id < remaind) {                     \
    	start = id;                                     \
        end = id;                                       \
    }

/**
 * macro-ul pt determminarea indexilor de start si stop care incadreaza matricea
 * de dimeniunile resize_factor x resize_factor din matricea citita
 */
#define INIT_INDEX(i_start, j_start, i_end, j_end, width, index,resize_factor) \
    i_start = (index / width) * resize_factor;                                 \
    j_start = (index % width) * resize_factor;                                 \
    i_end = i_start + resize_factor;                                           \
    j_end = j_start + resize_factor;

#define R           r
#define B           b
#define G           g
#define PIX         pix

/**
 * macro-ul utilizat pentru calcularea valorii pe un canal de culoare, cand
 * factorul de rezise este 3
 * */
#define CALC_PIXEL(matrixColor, i_start, j_start, GaussianKernel, member)      \
    ( matrixColor[i_start][j_start].member * GaussianKernel[0] +               \
      matrixColor[i_start][j_start + 1].member * GaussianKernel[1] +           \
      matrixColor[i_start][j_start + 2].member * GaussianKernel[2] +           \
      matrixColor[i_start + 1][j_start].member * GaussianKernel[3] +           \
      matrixColor[i_start + 1][j_start + 1].member * GaussianKernel[4] +       \
      matrixColor[i_start + 1][j_start + 2].member * GaussianKernel[5] +       \
      matrixColor[i_start + 2][j_start].member * GaussianKernel[6] +           \
      matrixColor[i_start + 2][j_start + 1].member * GaussianKernel[7] +       \
      matrixColor[i_start + 2][j_start + 2].member * GaussianKernel[8] )       \

#endif /* HOMEWORK_H */
