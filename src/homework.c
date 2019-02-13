/**
 * @author : Niculescu Mihai Alexandru 335CB
 * @file : homework.c
 */
#include "homework.h"

int num_threads;
int resize_factor;

/**************** Declarearea variabilelor globale folosite *******************/
static void *global_old_matrix; /*** matricea primite de functia resize */
static void *global_new_matrix; /** matricea calculata de resize */
static int global_med;       /** Nr_pixeli_img_rezultata / Nr_treaduri */
static int global_remaind;   /** Nr_pixeli_img_rezultata % Nr_treaduri */
static unsigned int global_new_width; /** latimea noii imagini */
static unsigned int global_height; /** latimea imagini initiale */

static const int GaussKernel[] = {1, 2, 1, 2, 4, 2, 1, 2, 1};


/**
 * functia care realizeaza citirea si initilizare img
 * @param fileName numelele fisierului de unde se citeste
 * @param img adresa variabilei de tip image
 */
void readInput(const char *fileName, image *img) {
    FILE *input = fopen(fileName, "rb");
    assert(input != NULL);
    char type[3];
    int maxval;

    assert(input != NULL);
    assert(fscanf(input, "%s", type) == 1);
    img->typeImage = type[1] - '0';
    assert(fscanf(input, "%u %u", &img->width, &img->height) == 2);
    assert(fscanf(input, "%d", &maxval) == 1);
    fseek(input, 1, SEEK_CUR); /** Evitarea '\n' */

    if (img->typeImage == color) {
        /** daca citim o imagine color */
        PixelColor **matrixColor;

        matrixColor = malloc(sizeof(PixelColor *) * img->height);
        assert(matrixColor != NULL);
        for (int i = 0; i < img->height; i++) {
            matrixColor[i] = malloc(sizeof(PixelColor) * img->width);
            assert(matrixColor[i] != NULL);
        }

        for (int i = 0; i < img->height; i++) {
            assert(fread(matrixColor[i], sizeof(PixelColor), img->width,
                         input) == img->width);
        }
        img->matrix = matrixColor;

    } else if (img->typeImage == grayscale) {
        /** daca citim o imagine noncolor */
        PixelGray **matrixGray;

        matrixGray = malloc(sizeof(PixelGray *) * img->height);
        assert(matrixGray != NULL);
        for (int i = 0; i < img->height; i++) {
            matrixGray[i] = malloc(sizeof(PixelGray) * img->width);
            assert(matrixGray[i] != NULL);
        }

        for (int i = 0; i < img->height; i++) {
            assert(fread(matrixGray[i], sizeof(PixelGray), img->width,
                         input) == img->width);
        }
        img->matrix = matrixGray;

    } else {
        exit(EXIT_FAILURE);
    }

    assert(fclose(input) == 0);
}

/**
 * functia pentru dezalocarea memoriei unei matrice de tipul PixelColor
 * @param height inaltimea imaginei, matricei
 * @param matrix adresa matricei
 */
static void free_matrixColor(int heightOld, void *matrixOld, void *matrixNew) {
    PixelColor **matrixColorOld = (PixelColor **) matrixOld;
    PixelColor *matrixColorNew = (PixelColor *) matrixNew;

    for (int i = 0; i < heightOld; i++) {
        free(matrixColorOld[i]);
    }

    free(matrixColorOld);
    free(matrixColorNew);
}

/**
 * functia pentru dezalocarea memoriei unei matrice de tipul PixelGray
 * @param height inaltimea imaginei, matricei
 * @param matrix adresa matricei
 */
static void free_matrixGray(int heightOld, void *matrixOld, void *matrixNew) {
    PixelGray **matrixGrayOld = (PixelGray **) matrixOld;
    PixelGray *matrixGrayNew = (PixelGray *) matrixNew;

    for (int i = 0; i < heightOld; i++) {
        free(matrixGrayOld[i]);
    }

    free(matrixGrayOld);
    free(matrixGrayNew);
}


/**
 * functia pentru scrierea imaginii, apoi dezalocarea memoriei dupa printare
 * @param fileName numele fisierului unde se scrie
 * @param img adresa imaginei care urmeaza sa fie scrisa
 */
void writeData(const char *fileName, image *img) {
    FILE *output = fopen(fileName, "wb+");
    assert(output != NULL);

    /** printare header */
    assert(fprintf(output, "P%d\n", img->typeImage) != 0);
    assert(fprintf(output, "%u %u\n", img->width, img->height) != 0);
    assert(fprintf(output, "%d\n", MAX_PIXEL) != 0);

    /** printarea matricei de pixeli in functie de tipul de imagine */
    if (img->typeImage == color) {
        PixelColor *matrixColor = (PixelColor *) img->matrix;

        assert(fwrite(matrixColor, sizeof(PixelColor), img->width * img->height,
                      output) == (img->width * img->height));

        free_matrixColor(global_height, global_old_matrix, global_new_matrix);

    } else {
        PixelGray *matrixGray = (PixelGray *) img->matrix;

        assert(fwrite(matrixGray, sizeof(PixelGray), img->width * img->height,
                      output) == (img->width * img->height));

        free_matrixGray(global_height, global_old_matrix, global_new_matrix);

    }

    assert(fclose(output) == 0);
}

/**
 * functia executa de un thread cand imaginea este color
  * i_start, j_start, i_end, j_end reprezinta pozitia de start (elementul din
 *     stanga sus) si de oprire (dreata jos) in matrice originala pentru
 *     submatricea de dimensiune resize_factor x resize_factor,
 * index indica pozitia unde o sa fie scris valoarea pixelului calculat
 * new_pixR, new_pixG, new_pixB va retine valoarea sumei celor
 *    resize_factor x resize_factor pixeli, pentru fiecare canal de culoare
 * @param args va reprezenta id-ul unui thread
 * @return NULL
 */
static void *threadFunctionColor(void *args) {
    int id = *(int *) args; /** id-ul unui thread */
    int start, end;
    int i_start, j_start, i_end, j_end;
    int new_pixR, new_pixG, new_pixB;
    PixelColor **matrixOld = (PixelColor **) global_old_matrix;
    PixelColor *matrixNew = (PixelColor *) global_new_matrix;
    int resize_factor_square = resize_factor * resize_factor;

    INIT_START_END(start, end, id, global_med, global_remaind);
    for (int index = start; index <= end; index++) {
        INIT_INDEX(i_start, j_start, i_end, j_end, global_new_width, index,
                   resize_factor);

        new_pixR = 0;
        new_pixG = 0;
        new_pixB = 0;

        if (resize_factor == SPECIAL_RESIZE_FACTOR) {
            new_pixR = CALC_PIXEL(matrixOld, i_start, j_start, GaussKernel, R);
            new_pixG = CALC_PIXEL(matrixOld, i_start, j_start, GaussKernel, G);
            new_pixB = CALC_PIXEL(matrixOld, i_start, j_start, GaussKernel, B);

            matrixNew[index].r = (unsigned char) (new_pixR / GAUSS_KERNEL_SUM);
            matrixNew[index].g = (unsigned char) (new_pixG / GAUSS_KERNEL_SUM);
            matrixNew[index].b = (unsigned char) (new_pixB / GAUSS_KERNEL_SUM);
            continue;

        }

        for (int i = i_start; i < i_end; i++) {
            for (int j = j_start; j < j_end; j++) {
                new_pixR += matrixOld[i][j].r;
                new_pixG += matrixOld[i][j].g;
                new_pixB += matrixOld[i][j].b;
            }
        }

        matrixNew[index].r = (unsigned char) (new_pixR / resize_factor_square);
        matrixNew[index].g = (unsigned char) (new_pixG / resize_factor_square);
        matrixNew[index].b = (unsigned char) (new_pixB / resize_factor_square);
    }

    return NULL;
}

/**
 * functia executa de un thread cand imaginea este grayscale
 * i_start, j_start, i_end, j_end reprezinta pozitia de start (elementul din
 *     stanga sus) si de oprire (dreapta jos) in matricea originala pentru
 *     submatricea de dimensiune resize_factor x resize_factor,
 * index pozitia unde o sa fie scrisa valoarea pixelului calculat
 * new_pix va retine valoarea sumei celor resize_factor x resize_factor
 * @param args va reprezenta id-ul uni thread
 * @return NULL
 */
static void *threadFunctionGray(void *args) {
    int id = *(int *) args;
    int start, end;
    int i_start, j_start, i_end, j_end;
    int new_pix;
    PixelGray **matrixOld = (PixelGray **) global_old_matrix;
    PixelGray *matrixNew = (PixelGray *) global_new_matrix;
    int resize_factor_square = resize_factor * resize_factor;

    INIT_START_END(start, end, id, global_med, global_remaind);
    for (int index = start; index <= end; index++) {
        INIT_INDEX(i_start, j_start, i_end, j_end, global_new_width, index,
                   resize_factor);
        new_pix = 0;

        if (resize_factor == SPECIAL_RESIZE_FACTOR) {
            new_pix = CALC_PIXEL(matrixOld, i_start, j_start, GaussKernel, PIX);

            matrixNew[index].pix = (unsigned char) (new_pix / GAUSS_KERNEL_SUM);

            continue;

        }

        for (int i = i_start; i < i_end; i++) {
            for (int j = j_start; j < j_end; j++) {
                new_pix += matrixOld[i][j].pix;
            }
        }

        matrixNew[index].pix = (unsigned char) (new_pix / resize_factor_square);
    }

    return NULL;
}

/**
 * functia care realizeza resize
 * @param imageOld imaginea primita
 * @param imageNew imaginea rezultata
 */
void resize(image *imageOld, image *imageNew) {
    pthread_t *tid;
    int *thread_id;

    tid = (pthread_t *) malloc(sizeof(pthread_t) * num_threads);
    thread_id = (int *) malloc(sizeof(int) * num_threads);
    assert(tid != NULL);
    assert(thread_id != NULL);

    for (int i = 0; i < num_threads; i++)
        thread_id[i] = i;


    imageNew->typeImage = imageOld->typeImage;
    /** calcularea noilor dimesiuni */
    imageNew->width = imageOld->width / resize_factor;
    imageNew->height = imageOld->height / resize_factor;

    /** setarea varibilelor globale */
    global_old_matrix = imageOld->matrix;
    global_new_width = imageNew->width;
    global_height = imageOld->height;

    /** alocare memorie pentru noua matrice */
    /** am ales ca noua matrice sa fie sub forma de o line, pentru a economisi
     * memoria si timp la scriere*/
    if (imageOld->typeImage == grayscale) {

        global_new_matrix = malloc(sizeof(PixelGray) * imageNew->height *
                                   imageNew->width);

    } else if (imageOld->typeImage == color) {

        global_new_matrix = malloc(sizeof(PixelColor) * imageNew->height *
                                   imageNew->width);

    } else {
        exit(EXIT_FAILURE);
    }
    assert(global_new_matrix != NULL);

    /**
     * numrul de matrici de dimensiune resize_factor x resize_factor in care
     * poate sa fie impartita matricea originala, de asemenea si numarul de
     * pixeli pentru noua imagine (dintr-o matrice va rezulta un pixel)
     */
    int num_matrix = (imageOld->height / resize_factor) * (imageOld->width /
                                                           resize_factor);
    global_med = num_matrix / num_threads;
    global_remaind = num_matrix % num_threads;


    if (imageOld->typeImage == grayscale) {
        for (int i = 0; i < num_threads; i++) {
            pthread_create(&(tid[i]), NULL, threadFunctionGray,
                           &(thread_id[i]));
        }

    } else {
        for (int i = 0; i < num_threads; i++) {
            pthread_create(&(tid[i]), NULL, threadFunctionColor,
                           &(thread_id[i]));
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(tid[i], NULL);
    }
    imageNew->matrix = global_new_matrix;

    free(tid);
    free(thread_id);
}