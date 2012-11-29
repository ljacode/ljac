#include  "ljabits.h"
#include  "ljatype.h"

/**
 * @brief 初始化bits。根据size的值分配适当的内存，不再使用后需要调用bits_destroy释放相应内存。
 *
 * @param bits 被初始化的bits
 * @param size 分配的位的总数
 *
 * @retval 0 正常 -1 内存分配失败
 */
int8 bits_init(Bits *bits,u_int32 size){
	assert(NULL != bits);
	assert(size>=0);
	char *ptr = (char *)malloc((size+7)>>3);
	if (NULL == ptr){
		return -1;
	}
	memset(ptr,'\0',(size+7)>>3);
	bits->size = size;
	bits->ptr = ptr;
	return 0;
}

/**
 * @brief 设置指定的位。
 *
 * @param bits bits
 * @param pos 要设置的位。pos超出范围将触发断言。
 *
 */
void bits_set(Bits *bits, u_int32 pos){
	assert(NULL != bits);
	assert(pos >0 && pos <= bits->size);

	u_int32 n = pos/8;
	u_int32 m = pos%8;
	if (m){
		*(bits->ptr+n) = *(bits->ptr+n) | (1<<(8-m));
	}else{
		*(bits->ptr+n) = *(bits->ptr+n) | 1;
	}
}

/**
 * @brief 获取指定的位的值。
 *
 * @param bits bits
 * @param pos 要获取的位。pos超出范围将触发断言。
 *
 * @retval
 */
u_int8 bits_get(Bits *bits, u_int32 pos){
	assert(NULL != bits);
	assert(pos > 0 && pos <= bits->size);

	u_int32 n = pos/8;
	u_int32 m = pos%8;

	char va = *(bits->ptr+n);
	if(m){
		return (va & (1<<(8-m)))>>(8-m);
	}else{
		return (va & 1);
	}
}

/**
 * @brief 清除指定的位。
 *
 * @param bits bits
 * @param pos 要清除的位。pos超出范围将触发断言。
 */
void bits_clear(Bits *bits, u_int32 pos){
	assert(NULL != bits);
	assert(pos >=0 && pos <= bits->size);

	u_int32 n = pos/8;
	u_int32 m = pos%8;
	if(m){
		*(bits->ptr+n) = *(bits->ptr+n) & (~(1<<(8-m)));
	}else{
		*(bits->ptr+n) = *(bits->ptr+n) & 0xfe;
	}
}

/**
 * @brief 释放bits。不再使用bits时需要调用这个函数进行释放。
 *
 * @param bits bits
 */
void bits_destroy(Bits *bits){
	assert(NULL != bits->ptr);
	free(bits->ptr);
	bits->size = 0;
}
