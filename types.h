/*
  Author      : Aswin Rajendran
  Date        : 06/10/2026
  Description : This file contains the user-defined data types
                used throughout the project.
*/

#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_encode,
    e_decode,
    e_unsupported
} OperationType;

#endif
