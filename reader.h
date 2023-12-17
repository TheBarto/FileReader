#ifndef READER_H
#define READER_H

#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define ONE_TIME_READ ((uint16_t) 512)

typedef struct {
    int8_t file_desc;
    uint8_t file_lec[(ONE_TIME_READ*2)];
    uint8_t* ptr_read; //Ptr to continue read the data
    uint8_t separator; /* Separator between elements*/
    uint8_t line_start; /* Line start */
    uint32_t total_file_size;
    uint32_t file_size_adj; /* File size after adjust */
} FileReader;

uint8_t open_file(FileReader* fr, uint8_t* file_name);

int32_t read_line(FileReader* fr, uint8_t* line_start, uint32_t* dst_arr);

#endif //READER_H
