#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "defs.h"
#include "rdebug.h"

void destroy_img(struct img_t* input) {
    if (input == NULL)
        return;

    if (input->img != NULL) {
        for (int i = 0; i < input->height; i++) {
            free(*(input->img + i));
        }
        free(input->img);
    }
    free(input);
}

struct img_t* load_image(const char* fname, enum error_code_t* errorCode) {
    if (errorCode == NULL)
        return NULL;

    if (fname == NULL) {
        *errorCode = ERROR_CODE_INCORRECT_PARAMETERS;
        return NULL;
    }

    FILE *f = fopen(fname, "rb");
    if (f == NULL) {
        *errorCode = ERROR_CODE_FILE_NOT_EXISTS;
        return NULL;
    }

    char magic[2];
    if (fread(magic, 1, 2, f) != 2) {
        fclose(f);
        *errorCode = ERROR_CODE_FILE_CORRUPTED;
        return NULL;
    }

    if (*magic != 'P' || *(magic + 1) != '2') {
        fclose(f);
        *errorCode = ERROR_CODE_FILE_CORRUPTED;
        return NULL;
    }

    int width = 0, height = 0;
    if (fread(&width, 4, 1, f) != 1 || fread(&height, 4, 1, f) != 1) {
        fclose(f);
        *errorCode = ERROR_CODE_FILE_CORRUPTED;
        return NULL;
    }

    if (width <= 0 || height <= 0) {
        fclose(f);
        *errorCode = ERROR_CODE_FILE_CORRUPTED;
        return NULL;
    }

    uint8_t max_val;
    if (fread(&max_val, 1, 1, f) != 1) {
        fclose(f);
        *errorCode = ERROR_CODE_FILE_CORRUPTED;
        return NULL;
    }

    if (max_val != 255) {
        fclose(f);
        *errorCode = ERROR_CODE_FILE_CORRUPTED;
        return NULL;
    }

    struct img_t* img = (struct img_t*)malloc(sizeof(struct img_t));
    if (img == NULL) {
        fclose(f);
        *errorCode = ERROR_CODE_FAILED_TO_ALLOCATE_MEMORY;
        return NULL;
    }

    img->width = width;
    img->height = height;

    img->img = (uint8_t**)malloc(sizeof(uint8_t*) * height);
    if (img->img == NULL) {
        free(img);
        fclose(f);
        *errorCode = ERROR_CODE_FAILED_TO_ALLOCATE_MEMORY;
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        *(img->img + i) = (uint8_t*)malloc(sizeof(uint8_t) * width);
        if (*(img->img + i) == NULL) {
            for (int j = 0; j < i; j++) {
                free(*(img->img + j));
            }
            free(img->img);
            free(img);
            fclose(f);
            *errorCode = ERROR_CODE_FAILED_TO_ALLOCATE_MEMORY;
            return NULL;
        }
    }

    for (int i = 0; i < height; i++) {
        size_t read_count = fread(*(img->img + i), 1, width, f);
        if ((int)read_count != width) {
            destroy_img(img);
            fclose(f);
            *errorCode = ERROR_CODE_FILE_CORRUPTED;
            return NULL;
        }
    }

    fclose(f);
    *errorCode = ERROR_CODE_OK;
    return img;
}

struct img_t* image_threshold(const struct img_t* input, enum error_code_t* errorCode) {
    if (errorCode == NULL)
        return NULL;

    if (input == NULL || input->img == NULL || input->width <= 0 || input->height <= 0) {
        *errorCode = ERROR_CODE_INCORRECT_PARAMETERS;
        return NULL;
    }

    long long sum = 0;
    for (int i = 0; i < input->height; i++) {
        for (int j = 0; j < input->width; j++) {
            sum += *(*(input->img + i) + j);
        }
    }

    long long total_pixels = (long long)input->width * input->height;
    long long mean = sum / total_pixels;

    struct img_t* result = (struct img_t*)malloc(sizeof(struct img_t));
    if (result == NULL) {
        *errorCode = ERROR_CODE_FAILED_TO_ALLOCATE_MEMORY;
        return NULL;
    }

    result->width = input->width;
    result->height = input->height;

    result->img = (uint8_t**)malloc(sizeof(uint8_t*) * result->height);
    if (result->img == NULL) {
        free(result);
        *errorCode = ERROR_CODE_FAILED_TO_ALLOCATE_MEMORY;
        return NULL;
    }

    for (int i = 0; i < result->height; i++) {
        *(result->img + i) = (uint8_t*)malloc(sizeof(uint8_t) * result->width);
        if (*(result->img + i) == NULL) {
            for (int j = 0; j < i; j++) {
                free(*(result->img + j));
            }
            free(result->img);
            free(result);
            *errorCode = ERROR_CODE_FAILED_TO_ALLOCATE_MEMORY;
            return NULL;
        }
    }

    for (int i = 0; i < result->height; i++) {
        for (int j = 0; j < result->width; j++) {
            if (*(*(input->img + i) + j) > mean)
                *(*(result->img + i) + j) = 255;
            else
                *(*(result->img + i) + j) = 0;
        }
    }

    *errorCode = ERROR_CODE_OK;
    return result;
}

static void flood_fill(const struct img_t* input, uint8_t* visited, int y, int x, uint8_t target_val, int* count) {
    if (y < 0 || y >= input->height || x < 0 || x >= input->width)
        return;

    if (*(visited + y * input->width + x))
        return;

    if (*(*(input->img + y) + x) != target_val)
        return;

    *(visited + y * input->width + x) = 1;
    (*count)++;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dy == 0 && dx == 0)
                continue;
            flood_fill(input, visited, y + dy, x + dx, target_val, count);
        }
    }
}

int area_statistics(const struct img_t* input, struct area_t** areas, int* counter) {
    if (input == NULL && areas == NULL && counter == NULL)
        return -1;

    if (counter != NULL)
        *counter = 0;

    if (areas != NULL)
        *areas = NULL;

    if (input == NULL || areas == NULL || counter == NULL)
        return -1;

    if (input->img == NULL || input->width <= 0 || input->height <= 0)
        return -1;

    int total = input->width * input->height;

    uint8_t* visited = (uint8_t*)calloc(total, sizeof(uint8_t));
    if (visited == NULL)
        return -2;

    int area_count = 0;
    for (int i = 0; i < input->height; i++) {
        for (int j = 0; j < input->width; j++) {
            if (!*(visited + i * input->width + j)) {
                int count = 0;
                flood_fill(input, visited, i, j, *(*(input->img + i) + j), &count);
                area_count++;
            }
        }
    }

    free(visited);

    struct area_t* result = (struct area_t*)malloc(sizeof(struct area_t) * area_count);
    if (result == NULL)
        return -2;

    visited = (uint8_t*)calloc(total, sizeof(uint8_t));
    if (visited == NULL) {
        free(result);
        return -2;
    }

    int idx = 0;
    for (int i = 0; i < input->height; i++) {
        for (int j = 0; j < input->width; j++) {
            if (!*(visited + i * input->width + j)) {
                int count = 0;
                flood_fill(input, visited, i, j, *(*(input->img + i) + j), &count);
                (result + idx)->top_y = i;
                (result + idx)->left_x = j;
                (result + idx)->size = count;
                idx++;
            }
        }
    }

    free(visited);
    *areas = result;
    *counter = area_count;
    return 0;
}
