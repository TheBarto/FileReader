#include "reader.h"

#include <errno.h>
#include <stdio.h>
uint8_t open_file(FileReader* fr, uint8_t* file_name)
{
    memset(fr, 0, sizeof(FileReader));
    fr->file_desc = open((char *)file_name, O_RDONLY);
    if(fr->file_desc == -1)
    {
       printf("Errno(%d) - %s\n", errno, strerror(errno)); 
        return -1;
    }

    fr->total_file_size = lseek(fr->file_desc, 0, SEEK_END);
    lseek(fr->file_desc, 0, SEEK_SET);

    fr->file_ptr = calloc(fr->total_file_size, sizeof(uint8_t));
    if(!fr->file_ptr)
        return -1;

    //Error control, close the file
    //if(fr->total_file_size != 
    //        read(fr->file_desc, fr->file_ptr, fr->total_file_size))
    //    return -1;
    printf("... %lu\n", read(fr->file_desc, fr->file_ptr, fr->total_file_size));

    fr->separator = ',';
    //fr->separator[1] = 0;
    
    fr->line_start = ':';

    //Need to add new end (0) and re-ajust the size
    fr->file_size_adj = parse_white_spaces(fr->file_ptr, fr->total_file_size);
    
    return 0;
}

//Error if the number starts with 0, like 001
int32_t parse_value(uint8_t* ptr_file, uint32_t size)
{
    uint32_t val = 0;

    uint8_t* aux_ptr = ptr_file;//strip_values(ptr_file, size);

    for(uint32_t i = 0; i < size; i++, aux_ptr++) {
        val*=10; 
        if((*aux_ptr >= '0') && (*aux_ptr <= '9')) //Add here error control
            val += (*aux_ptr -'0');
    }
    
    return val;
}

int32_t parse_hex_value(uint8_t* ptr_file, uint32_t size)
{
    uint32_t val = 0;

    //uint8_t* aux_ptr = strip_values(ptr_file, size);
    uint8_t* aux_ptr = ptr_file;

    for(uint32_t i = 0; i < size; i++, aux_ptr++) { //Add error control
        val *= 16;
        if((*aux_ptr >= 'A') && (*aux_ptr <= 'F'))
            val += ((*aux_ptr - 'A') + 10);
        else if((*aux_ptr >= 'a') && (*aux_ptr <= 'f'))
            val += ((*aux_ptr - 'a') + 10);
        else if((*aux_ptr >= '0') && (*aux_ptr <= '9'))
            val += (*aux_ptr - '0');
    }
    
    return val;
}

void parse_value_readed(uint8_t* ptr_file, uint32_t size, uint32_t* dst_arr)
{
    //If find the 0x value, transform hexadecimal value 
    if((*ptr_file == '0') && 
            (*(ptr_file+1) == 'x'))
        *dst_arr = parse_hex_value(&ptr_file[2], size-2); //Eliminate the 0x
    else
        *dst_arr = parse_value(ptr_file, size);

    return;
}

//Be carefull with the jump lines values, because it adds more undesired length 
int32_t read_line(FileReader* fr, uint8_t* line_start, uint32_t* dst_arr)
{
    //uint8_t* ptr_file = strip_values(fr->file_ptr, fr->total_file_size);
    //parse_white_spaces(&ptr_file); //strip better?
    uint8_t* ptr_file = fr->file_ptr;

    uint8_t i = 0;
    //Search the line start
    while((*ptr_file != fr->line_start))
        ptr_file++;
    /*line_start[i] != 0) {
        (*ptr_file == line_start[i])?i++:(i=0);
        ptr_file++;
    }*/

    ptr_file++;//Advance one to pass the ':'
    uint8_t* ptr_aux_file = ptr_file;
    uint32_t tot_vals = 0;
    while(*ptr_file) {
        while((*ptr_aux_file != fr->separator) && (*ptr_aux_file != 0)) ptr_aux_file++;
        parse_value_readed(ptr_file, (ptr_aux_file - ptr_file), &dst_arr[tot_vals]);
        tot_vals++;
        ptr_aux_file++;
        ptr_file = ptr_aux_file;
    }

    return tot_vals;
}

uint8_t* strip_values(uint8_t* file, uint32_t ssize)
{
    uint8_t* ptr = file;
    while((*ptr == ' ') || (*ptr == '\t'))
       ptr++; 

    return ptr;
}

uint32_t parse_white_spaces(uint8_t* file, uint32_t total_size)
{
    uint8_t* ptr1, *ptr2;
    uint8_t aux = 0;
    ptr1 = ptr2 = file;

    while(*ptr2 != 0) {
        //Advance to the first desired position
        /*if(((*ptr2 == 0x7E) ||
           (*ptr2 < 0x21)) &&
          (aux == 0)) {
            aux = 1;
            ptr1 = ptr2;
        }*/

        if(/*(aux == 1) &&*/ (*ptr2 >= 0x21) &&
           (*ptr2 <= 0x7E)) { 
            *ptr1 = *ptr2;
            ptr1++;
        }
        ptr2++;
    }

    ptr1++;
    *ptr1=0;

    //printf("Size: %d, New string readed: %s\n", (ptr1-file), file);

	return (ptr1-file);
}
