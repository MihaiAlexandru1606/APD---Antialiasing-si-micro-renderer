/**
 * @author : Niculescu Mihai Alexandru 335CB
 * @file : homework1.c
 */
#include "homework1.h"

int num_threads;
int resolution;

/**************** Declarearea variabilelor globale folosite *******************/
static PixelGrayScale *global_matrix; /** matrice de pixeli */
static int global_med; /** numar pixel / numar_threas*/
static int global_remaind; /** numar pixel % numar_thread */

static const double min_dist = 3;

/**
 * functia care initializeaza imaginea
 * @param img
 */
void initialize(image *img) {
    img->matrix = (PixelGrayScale *) malloc(sizeof(PixelGrayScale) * resolution
                                            * resolution);
    assert(img->matrix != NULL);
}

/**
 * functia apelata de fiecare thread
 * functia caluleaza dinstanta la dreapta pentru fiecare punct corespunzator
 * intervalului [start, end], si daca este mai mica ca 3, atunci va fi negru,
 * altfel alb
 * @param args id-ul fiecarui thread
 * @return NULL
 */
static void *threadFunction(void *args) {
    int id = *(int *) args;
    int start, end;
    double dist;

    /** calculare "intervalelor pentru fiecare thread */
    start = id * global_med;
    end = start + global_med - 1;
    if (id < global_remaind) {
        start += id;
        end = start + global_med;
    } else {
        start += global_remaind;
        end += global_remaind;
    }
    /** cazul in care sunt mai multe thread-uri decat elemente */
    if (global_med == 0 && id < global_remaind) {
    	start = id;
        end = id;
    }

    for (int k = start; k <= end; k++) {
        /** calculare index-ilor in formatul de matrice */
        int i = k / resolution;
        int j = k % resolution;

        /** calcularea coordonatelor pentru "mijlocul pixelului in plan xOy */
        double x = j * (REAL_RESOLUTION / resolution) +
                   (REAL_RESOLUTION / resolution) * 0.5;
        double y = abs(resolution - 1 - i) * (REAL_RESOLUTION / resolution) +
                   (REAL_RESOLUTION / resolution) * 0.5;

        /** calculare distantei */
        dist = abs(A * x + B * y + C) / sqrt(A * A + B * B);

        if (dist < min_dist) {
            global_matrix[k].pix = BLACK_PIXEL;

            continue;
        }

        global_matrix[k].pix = WHITE_PIXEL;
    }

    return NULL;
}

/**
 * functia relizeaza crearea imaginii
 * @param img adresa imaginei
 */
void render(image *img) {
    pthread_t *tid;
    int *thread_id;

    tid = (pthread_t *) malloc(sizeof(pthread_t) * num_threads);
    thread_id = (int *) malloc(sizeof(int) * num_threads);
    assert(tid != NULL);
    assert(thread_id != NULL);

    for (int i = 0; i < num_threads; i++)
        thread_id[i] = i;

    global_matrix = img->matrix;
    int square = resolution * resolution;
    global_med = square / num_threads;
    global_remaind = square % num_threads;

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(tid[i], NULL);
    }

    free(tid);
    free(thread_id);
}

/**
 * functia pentru scrierea imaginii, apoi dezalocarea memoriei dupa printare
 * @param fileName numele fisierului unde se scrie
 * @param img adresa imaginei care urmeaza sa fie scrisa
 */
void writeData(const char *fileName, image *img) {
    FILE *output = fopen(fileName, "wb+");
    assert(output != NULL);
    char terminator[] = {'\n'};
    char space[] = {' '};
    char format[] = {'P', '5', '\n'};
    int maxl = MAX_PIXEL;

    fwrite(format, sizeof(char), sizeof(format), output);
    fprintf(output, "%d", resolution);
    fwrite(space, sizeof(char), sizeof(space), output);
    fprintf(output, "%d", resolution);
    fwrite(terminator, sizeof(char), sizeof(terminator), output);
    fprintf(output, "%d", maxl);
    fwrite(terminator, sizeof(char), sizeof(terminator), output);
    fwrite(img->matrix, sizeof(PixelGrayScale), resolution * resolution,
           output);

    free(img->matrix);
    assert(fclose(output) == 0);
}
