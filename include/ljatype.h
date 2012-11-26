/**
 * @file ljatype.h
 * @brief ljac��ʹ�õ���������
 * @author LJA
 * @version 0.0.1
 * @date 2012-11-13
 */
#ifndef __LJATYPE_H__
#define __LJATYPE_H__

typedef unsigned char u_char;
typedef unsigned char u_int8;
typedef unsigned short int u_int16;
typedef unsigned int u_int32;
#if LJA_WORDSIZE == 64
typedef unsigned long int u_int64;
#else
typedef unsigned long long int u_int64;
#endif

//typedef signed char char;  char����ʹ��char
typedef signed char int8;
typedef signed short int int16;
typedef signed int int32;
#if LJA_WORDSIZE == 64
typedef signed long int int64;
#else
typedef signed long long int int64;
#endif

#endif 
