/*
  Author      : Aswin Rajendran
  Date        : 06/10/2026
  Description : This file contains the functions required to encode
                secret data into a BMP image.
*/




#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("INFO: width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("INFO: height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_encode_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	printf("ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
    printf("INFO: Opened input image file (.bmp)\n");

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }
    printf("INFO: Opened secret text file\n");

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }
    printf("INFO: Opened output image file (.bmp)\n");

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
    if(argv[2]!=NULL)
    {
        if(strrchr(argv[2],'.')!=NULL)
        {
            if(strcmp(strrchr(argv[2],'.'),".bmp")==0)
            encInfo->src_image_fname=argv[2];
            else
            return e_failure;
        }
        else
        {
            return e_failure;
        }
    }
    else
    {
        printf("ERROR: Source image file should be a .bmp file\n");
        return e_failure;
    }
    if(argv[3]!=NULL)
    {
        if(strrchr(argv[3],'.')!=NULL)
        {
            if(strcmp(strrchr(argv[3],'.'),".txt")==0)
            {
            encInfo->secret_fname=argv[3];
            strcpy(encInfo->extn_secret_file,strrchr(argv[3],'.'));
            }
            else
            {
            return e_failure;
            }
        }
        else
        { 
            return e_failure;
        }
    }
    else
    {
        printf("ERROR: Secret file should be a .txt file\n");
        return e_failure;
    }
    if(argv[4]!=NULL)
    {
        if(strrchr(argv[4],'.')!=NULL)
        {
            if(strcmp(strrchr(argv[4],'.'),".bmp")==0)
            encInfo->stego_image_fname=argv[4];
            else
            return e_failure;
        }
        else
        {
            return e_failure;
        }
    }
    else
    {
        encInfo->stego_image_fname="stego.bmp";
    }
    return e_success;
}

//All rest of the encoding is done here

Status do_encoding(EncodeInfo *encInfo)
{
    //opening files before encoding
    printf("INFO: Opening required files\n");
    if(open_encode_files(encInfo)==e_success)
    {
        printf("INFO: Done. Opened required files\n");
        //Extracting secret file size
        printf("INFO: ## ENCODING PROCESS STARTED ##\n");
        encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
        printf("INFO: Checking for secret.txt size\n");
        if(encInfo->size_secret_file==0)
        {
            printf("INFO: Secret file is empty\n");
        }
        else
        {
            printf("INFO: Secret file is not empty\n");
        }
        //before checking cpacity we need to find source image size
        encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
        rewind(encInfo->fptr_src_image);
        //checking whether source image have capacity to store secret data
        printf("INFO: Checking whether source image have capcity to handle secret text\n");
        if(check_capacity(encInfo)==e_success)
        {
            printf("INFO: Done. Source image have capacity\n");
            //copying 54 bytes from source to output file
            printf("INFO: Copying image header\n");
            if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
            {
                printf("INFO: Done\n");
                printf("INFO: Encode Magic string\n");
                if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
                {
                    printf("INFO: Done\n");
                    printf("INFO: Encode Secret file extension size\n");
                    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo)==e_success)
                    {
                        printf("INFO: Done\n");
                        printf("INFO: Encode secret file extension\n");
                        if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_success)
                        {
                            printf("INFO: Done\n");
                            printf("INFO: Encode secret file size\n");
                            if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
                            {
                                printf("INFO: Done\n");
                                printf("INFO: Encode secret file data\n");
                                if(encode_secret_file_data(encInfo)==e_success)
                                {
                                    printf("INFO: Done\n");
                                    printf("INFO: Copying remaining image data\n");
                                    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
                                    {
                                        printf("INFO: Done\n");
                                        //closing files
                                        fclose(encInfo->fptr_secret);
                                        fclose(encInfo->fptr_src_image);
                                        fclose(encInfo->fptr_stego_image);
                                    }
                                    else
                                    {
                                        printf("INFO: Unable to copy remaining data\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("INFO: Unable to encode secret file data\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("INFO: Unable to encode secret file size\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("INFO: Unable to encode secret file extension\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("INFO: Unable to encode secret file extension size\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("INFO: Magic String encoding unsuccessfull\n");
                    return e_failure;
                }
            }
            else
            {
                printf("INFO: Unable to copy header\n");
                return e_failure;
            }
        }
        else
        {
            printf("INFO: Capacity check unsuccessfull\n");
            return e_failure;
        }
    }
    else
    {
        printf("INFO: Error in opening files\n");
        return e_failure;
    }
    return e_success;
}

/*This function is used to get the required file size*/
uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    uint size=ftell(fptr);
    rewind(fptr);
    return size;
}

/*This function checks whether the source image have enough size to store the secret data */
Status check_capacity(EncodeInfo *encInfo)
{
    //req size = len of magic string + extension of secret file + size secret file + secret file data
 
    if(encInfo->image_capacity>=(strlen(MAGIC_STRING)+strlen(encInfo->extn_secret_file)+sizeof(encInfo->size_secret_file)+encInfo->size_secret_file)*8)
    {
        return e_success;
    }
    else
    {
        printf("ERROR: Source image does not have enough size to store secret data\n");
        return e_failure;
    }
}

/*This function copies 54 byte header from source image to output image*/
Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_dest_image)
{
    char header[54];
    if(fread(header,1,54,fptr_src_image)!=54)
    {
        return e_failure;
    }
    if(fwrite(header,1,54,fptr_dest_image)!=54)
    {
        return e_failure;
    }
    return e_success;
}

/*This function encodes magic string to the the output image*/
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    if(encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    return e_success;
    else
    return e_failure;
}

/*Function to encode the data to the image
  we read data and we call another function to encode byte to lsb
  after modifying data we write the modified data in the outpur file*/

Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char image_buffer[MAX_IMAGE_BUF_SIZE];
    for(int i=0;i<size;i++)
    {
        if(fread(image_buffer,1,MAX_IMAGE_BUF_SIZE,fptr_src_image)!=MAX_IMAGE_BUF_SIZE)
        return e_failure;
        encode_byte_to_lsb(data[i],image_buffer);
        if(fwrite(image_buffer,1,MAX_IMAGE_BUF_SIZE,fptr_stego_image)!=MAX_IMAGE_BUF_SIZE)
        return e_failure;
    }
    return e_success;
}

/*function to encode byte to lsb 
  Here we encode the data bits to the lsb of each bytes in the image buffer
  before changing lsb we need to clear the lsb of the image buffer
  we then change the lsb by performing OR operation with the right shited data bit*/
Status encode_byte_to_lsb(char data,char *image_buffer)
{
    unsigned mask=1<<7;
    for(int i=0;i<8;i++)
    {
        image_buffer[i]=(image_buffer[i]&0xFE)|(data&mask)>>(7-i);
        mask=mask>>1;
    }
    return e_success;
}

//function to encode secret file extension size
Status encode_secret_file_extn_size(int size,EncodeInfo *encInfo)
{
    return encode_data_to_image((char *)&size,sizeof(int),encInfo->fptr_src_image,encInfo->fptr_stego_image);
}

//function to encode secret file extension
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    return encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image);
}

//Function to encode secret file size
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    return encode_data_to_image((char *)&file_size,sizeof(long),encInfo->fptr_src_image,encInfo->fptr_stego_image);
}

//Function to encode secret file data
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    for(int i=0;i<encInfo->size_secret_file;i++)
    {
        if(fread(encInfo->secret_data,1,MAX_SECRET_BUF_SIZE,encInfo->fptr_secret)!=MAX_SECRET_BUF_SIZE)
        return e_failure;
        if(encode_data_to_image(encInfo->secret_data,MAX_SECRET_BUF_SIZE,encInfo->fptr_src_image,encInfo->fptr_stego_image)!=e_success)
        return e_failure;
    }
    return e_success;
}

//function to copy remaining rgb data from source image to output image
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char buffer[MAX_IMAGE_BUF_SIZE];
    uint bytes_read;
    while((bytes_read=fread(buffer,1,MAX_IMAGE_BUF_SIZE,fptr_src))>0)
    {
        if(fwrite(buffer,1,bytes_read,fptr_dest)!=bytes_read)
        return e_failure;
    }
    return e_success;
}