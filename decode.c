/*
  Author      : Aswin Rajendran
  Date        : 06/10/2026
  Description : This file contains the functions required to decode
                secret data from a BMP image.
*/


#include<stdio.h>
#include"decode.h"
#include<string.h>
#include"types.h"

//Function to read and validate decode arguments
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
    if(argv[2]!=NULL)
    {
        if(strrchr(argv[2],'.')!=NULL)
        {
            if(strcmp(strrchr(argv[2],'.'),".bmp")==0)
            decInfo->stego_image_fname=argv[2];
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
        printf("INFO: Stego image filename should be a .bmp file\n");
        return e_failure;
    }
    if(argv[3]!=NULL)
    {
        decInfo->output_fname=argv[3];
    }
    else
    {
        decInfo->output_fname=NULL;
    }
    return e_success;
}

/*The rest of the decoding happens here
  open files
  decode magic string
  decode secret file extension
  create output file if not mentioned
  decode secret file size
  decode secret file data*/

Status do_decoding(DecodeInfo *decInfo)
{
    printf("INFO: Opening required files\n");
    if(open_decode_files(decInfo)==e_success)
    {
        if(fseek(decInfo->fptr_stego_image,54,SEEK_SET)!=0)
        {
            printf("ERROR: Failed to seek past BMP header\n");
            fclose(decInfo->fptr_stego_image);
            return e_failure;
        }
        printf("INFO: Decode magic string\n");
        if(decode_magic_string(MAGIC_STRING,decInfo)==e_success)
        {
            printf("INFO: Done\n");
            printf("INFO: Decode secret file extension size\n");
            if(decode_secret_file_extn_size(decInfo)==e_success)
            {
                printf("INFO: Done\n");
                printf("INFO: Decode secret file extension\n");
                if(decode_secret_file_extn(decInfo)==e_success)
                {
                    printf("INFO: Done\n");
                    if(create_output_filename(decInfo)==e_success)
                    {
                        decInfo->fptr_output=fopen(decInfo->output_filename,"wb");
                        if(decInfo->fptr_output==NULL)
                        {
                            fclose(decInfo->fptr_stego_image);
                            return e_failure;
                        }
                        printf("INFO: Opened output file\n");
                        printf("INFO: Done. Opened all required files\n");
                        printf("INFO: Decode secret file size\n");
                        if(decode_secret_file_size(decInfo)==e_success)
                        {
                            printf("INFO: Done\n");
                            printf("INFO: Decode secret data\n");
                            if(decode_secret_file_data(decInfo)==e_success)
                            {
                                printf("INFO: Done\n");
                                //closing files
                                fclose(decInfo->fptr_stego_image);
                                fclose(decInfo->fptr_output);
                            }
                            else
                            {
                                printf("ERROR: Error in decoding secret file data\n");
                                fclose(decInfo->fptr_output);
                                fclose(decInfo->fptr_stego_image);
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("ERROR: Error in decoding secret file size\n");
                            fclose(decInfo->fptr_output);
                            fclose(decInfo->fptr_stego_image);
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("ERROR: Error in creating output file\n");
                        fclose(decInfo->fptr_stego_image);
                        return e_failure;
                    }
                }
                else
                {
                    printf("ERROR: Error in decoding secret file extension\n");
                    fclose(decInfo->fptr_stego_image);
                    return e_failure;
                }
            }
            else
            {
                printf("ERROR: Error in decoding secret file extension size\n");
                fclose(decInfo->fptr_stego_image);
                return e_failure;
            }
        }
        else
        {
            printf("ERROR: Error in decoding magic string\n");
            fclose(decInfo->fptr_stego_image);
            return e_failure;
        }
    }
    else
    {
        printf("ERROR: Error in opening files\n");
        return e_failure;
    }
    return e_success;
}

//function in opening files
Status open_decode_files(DecodeInfo *decInfo)
{
    
    decInfo->fptr_stego_image=fopen(decInfo->stego_image_fname,"rb");
    if(decInfo->fptr_stego_image!=NULL)
    {
        printf("INFO: Opened Steged image .bmp file\n");   
    }
    else
    {
        printf("ERROR: Failed to open steged .bmp file\n");
        return e_failure;
    }
    return e_success;
}

//Function to decode megic string
Status decode_magic_string(const char *magic_string,DecodeInfo *decInfo)
{
    char decoded_magic[strlen(magic_string)+1];
    if(decode_data_from_image(decoded_magic,strlen(magic_string),decInfo->fptr_stego_image)==e_failure)
    {
        return e_failure;
    }
    decoded_magic[strlen(magic_string)]='\0';
    if(strcmp(magic_string,decoded_magic)==0)
    return e_success;
    else
    return e_failure;
}


/*Function to decode the data to the image
  we read bytes and we call another function to decode lsb to byte*/

Status decode_data_from_image(char *data,int size,FILE *fptr_stego_image)
{
    char image_buffer[MAX_IMAGE_BUF_SIZE];
    for(int i=0;i<size;i++)
    {
        if(fread(image_buffer,1,MAX_IMAGE_BUF_SIZE,fptr_stego_image)!=MAX_IMAGE_BUF_SIZE)
        return e_failure;
        decode_lsb_to_byte((char *)&data[i],image_buffer);
    }
    return e_success;
}


/*function to decode lsb to byte 
  Here we decode the lsb of each bytes to data bit
  we use loop, after getting each bit we shift data byte to left
  we perform OR operation with the data and the lsb bit*/

Status decode_lsb_to_byte(char *data,char *image_buffer)
{
    *data=0;
    for(int i=0;i<8;i++)
    {
        *data=(*data<<1) | (image_buffer[i]&1);
    }
    return e_success;
}

//functon to decode secret file extension size
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    int size;
    if(decode_data_from_image((char *)&size,sizeof(int),decInfo->fptr_stego_image)==e_success)
    {
        //validating the extension size
        if(size<=0||size>=MAX_FILE_SUFFIX)
        {
            return e_failure;
        }
        decInfo->extn_size=size;
        return e_success;
    }
    return e_failure;
}

//Function to decode secret file extension
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    if(decode_data_from_image(decInfo->extn_secret_file,decInfo->extn_size,decInfo->fptr_stego_image)==e_success)
    {
        decInfo->extn_secret_file[decInfo->extn_size]='\0';
        return e_success;
    }
    return e_failure;
}

//function to create output filename
Status create_output_filename(DecodeInfo *decInfo)
{
    if(decInfo->output_fname==NULL)
    {
        printf("INFO: Output file not mentioned. Creating output file\n");
        strcpy(decInfo->output_filename,"decoded");
        strcat(decInfo->output_filename,decInfo->extn_secret_file);
        return e_success;
    }
    if(strrchr(decInfo->output_fname,'.')!=NULL)
    {
        strcpy(decInfo->output_filename,decInfo->output_fname);
    }
    else
    {
        strcpy(decInfo->output_filename,decInfo->output_fname);
        strcat(decInfo->output_filename,decInfo->extn_secret_file);
    }
    return e_success;
}

//Function to decode secret file size
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    return decode_data_from_image((char *)&decInfo->size_secret_file,sizeof(long),decInfo->fptr_stego_image);
}

//Function to decode secret file data
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    for(int i=0;i<decInfo->size_secret_file;i++)
    {
        if(decode_data_from_image(decInfo->secret_data,1,decInfo->fptr_stego_image)==e_failure)
        {
            return e_failure;
        }
        if(fwrite(decInfo->secret_data,1,1,decInfo->fptr_output)!=1)
        {
            return e_failure;
        }
    }
    return e_success;
}
