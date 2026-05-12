#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>

enum error_code_t {
    ERROR_CODE_OK = 0,
    ERROR_CODE_INCORRECT_PARAMETERS = 1,
    ERROR_CODE_FILE_NOT_EXISTS = 2,
    ERROR_CODE_FILE_CORRUPTED = 3,
    ERROR_CODE_COULDNT_CREATE_FILE = 4,
    ERROR_CODE_FAILED_TO_ALLOCATE_MEMORY = 5
};

struct img_t {
    uint8_t **img;
    int width;
    int height;
};

struct area_t {
    int size;
    int left_x;
    int top_y;
};

struct img_t* load_image(const char* fname, enum error_code_t* errorCode);
struct img_t* image_threshold(const struct img_t* input, enum error_code_t* errorCode);
int area_statistics(const struct img_t* input, struct area_t** areas, int* counter);
void destroy_img(struct img_t* input);

#endif
