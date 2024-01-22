#ifndef STD_TYPES_H
#define STD_TYPES_H

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned long int uint32;
typedef unsigned long long int uint64;

typedef signed char sint8;
typedef signed short int sint16;
typedef signed long int sint32;
typedef signed long long int sint64;

typedef float float32;
typedef double float64;
typedef long double float128;

typedef void (*pf)(void);

#define NULL_PTR ((void*)0)

typedef enum{
    E_OK,
    E_NOK_PARAMETER_OUT_OF_RANGE,
    E_NOK_NULL_POINTER,
    E_NOK
}tenuErrorStatus;

#endif
