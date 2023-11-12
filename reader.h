#ifndef READER_H
#define READER_H

#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    int8_t file_desc;
    uint8_t* file_ptr; //The first position of the data
    uint8_t* ptr_read; //Ptr to read the data
    uint8_t separator; /* Separator between elements*/
    uint8_t line_start; /* Line start */
    uint32_t total_file_size;
    uint32_t file_size_adj; /* File size after adjust */
} FileReader;

uint8_t open_file(FileReader* fr, uint8_t* file_name);

int32_t parse_value(uint8_t* ptr_file, uint32_t size);

int32_t parse_hex_value(uint8_t* ptr_file, uint32_t size);

void parse_value_readed(uint8_t* ptr_file, uint32_t size, uint32_t* dst_arr);

int32_t read_line(FileReader* fr, uint8_t* line_start, uint32_t* dst_arr);

uint32_t parse_white_spaces(uint8_t* file, uint32_t total_size);

uint8_t* strip_values(uint8_t* file, uint32_t ssize);

#endif //READER_H
