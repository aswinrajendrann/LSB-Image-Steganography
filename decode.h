/*
  Author      : Aswin Rajendran
  Date        : 06/10/2026
  Description : This file contains the declarations and structures
                required for the decoding process.
*/


#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include"common.h"
#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 5

typedef struct _DecodeInfo
{

    char *output_fname;
    char output_filename[100];
    FILE *fptr_output;
    /* Secret File Info */
    char extn_secret_file[MAX_FILE_SUFFIX];
    int extn_size;
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

} DecodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);


/* Store Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

/*Encode secret file extension size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Encode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Encode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Encode function, which does the real encoding */
Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
Status decode_lsb_to_byte(char *data, char *image_buffer);

/*creating output file */
Status create_output_filename(DecodeInfo *decInfo);

#endif
