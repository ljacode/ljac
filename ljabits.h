#ifndef  BITS_H
#define  BITS_H

#include  <string.h>
#include  <stdlib.h>
#include  <assert.h>
#include  "ljabits.h"
#include  "ljatype.h"

/**
 * @brief bits结构体
 */
typedef struct Bits{
	u_int32 size;  //!操作的位的总数
	u_int32 used;  //!已经被置为1的位数
	char *ptr;     //!位的内存起始位置
}Bits;

/**
 * @brief 初始化bits。根据size的值分配适当的内存，不再使用后需要调用bits_destroy释放相应内存。
 *
 * @param bits 被初始化的bits
 * @param size 分配的位的总数
 *
 * @retval 0 正常 -1 内存分配失败
 */
int8 bits_init(Bits *bits,u_int32 size);

/**
 * @brief 设置指定的位。
 *
 * @param bits bits
 * @param pos 要设置的位。pos超出范围将触发断言。
 *
 */
void bits_set(Bits *bits, u_int32 pos);
/**
 * @brief 获取指定的位的值。
 *
 * @param bits bits
 * @param pos 要获取的位。pos超出范围将触发断言。
 *
 * @retval
 */
u_int8 bits_get(Bits *bits, u_int32 pos);
/**
 * @brief 清除指定的位。
 *
 * @param bits bits
 * @param pos 要清除的位。pos超出范围将触发断言。
 */
void bits_clear(Bits *bits, u_int32 pos);
/**
 * @brief 释放bits。不再使用bits时需要调用这个函数进行释放。
 *
 * @param bits bits
 */
void bits_destroy(Bits *bits);

#endif  /*BITS_H*/
