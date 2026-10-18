/*
  Project     : Steganography
  Author      : Aswin Rajendran
  Date        : 06/10/2026
  Description : This file controls the main mechanism of this project,like encoding and decoding operation
*/

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc,char* argv[])
{
    //checking operation type
    if(check_operation_type(argv)==e_encode)
    {
        EncodeInfo encInfo;
        printf("INFO: Selected Encoding\n");
        printf("INFO: Read and validating encode arguments\n");
        if(read_and_validate_encode_args(argv,&encInfo)==e_success)
        {
            printf("INFO: Done\n");
            if(do_encoding(&encInfo)==e_success)
            {
                printf("INFO: ## Encoding done successfully ##\n");
            }
            else
            {
                printf("ERROR: Error in encoding\n");
            }
        }
        else
        {
            printf("ERROR: Error in reading and validating encode arguments\n");
        }
    }
    else if(check_operation_type(argv)==e_decode)
    {
        DecodeInfo decInfo;
        printf("INFO: Selected Decoding\n");
        printf("INFO: Read and validating decode arguments\n");
        if(read_and_validate_decode_args(argv,&decInfo)==e_success)
        {
            printf("INFO: Done\n");
            printf("INFO: ## Decoding procedure started ##\n");
            if(do_decoding(&decInfo)==e_success)
            {
                printf("INFO: ## Decoding done successfully ##\n");
            }
            else
            {
                printf("ERROR: Error in decoding\n");
            }
        }
        else
        {
            printf("ERROR: Error in reading and validating decode arguments\n");
        }
    }
    else
    {
        printf("Unsupported operation type\n");
        printf("***************************Usage***************************\n");
        printf("Arguments must be of the following format\n\n");
        printf("To encode: ./a.out -e source_image.bmp secret.txt stego.bmp\n");
        printf("To decode: ./a.out -d stego.bmp output.txt\n");
        printf("***********************************************************\n");
    }
    return 0;
}

OperationType check_operation_type(char *argv[])
{
    //we need to check if argv[1] is present and check which operation is it
    if(argv[1]!=NULL && strcmp(argv[1],"-e")==0)
    return e_encode;
    else if(argv[1]!=NULL && strcmp(argv[1],"-d")==0)
    return e_decode;
    else
    return e_unsupported;
}