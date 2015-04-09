/******************************************************************************
			

sdatomic.h - 处理原子操作的函数系列

******************************************************************************/

#ifndef SDATOMIC_H
#define SDATOMIC_H

/**
* @file sdatomic.h

* @brief 处理原子操作的函数系列
*
**/
#include "sdtype.h"

namespace SGDP
{
    /**
    * @defgroup groupatomic 原子操作
    * @ingroup  SGDP
    * @{
    */

    /*
    * 原子操作32位数值,注意不要在Win32模式下使用64位的原子操作函数。内部使用了64位的API，
		需要Windows Vista以上的版本
    * 需要注意的是：每个函数的内部实现都是由操作系统自己确认的
    * 在本组函数中，
    * Add代表加法
    * Sub代表减法
    * Inc代表加1
    * Dec代表减1
    */


    /**
    * @brief
    * 从内存中原子读取一个32位值
    * @param pdwMem : 指向需要被读取值的指针
    * @return mem的值
    */
    INT32  SDAtomicRead32(volatile INT32 *pdwMem);

	/**
	* @brief
	* 从内存中原子读取一个64位值, 需要Windows Vista以上操作系统
	* @param pqwMem : 指向需要被读取值的指针
	* @return mem的值
	*/
    INT64  SDAtomicRead64(volatile INT64 *pdwMem);

    /**
    * @brief
    * 在内存中原子设置一个32位的值
    * @param pdwMem : 指向需要设置值的指针
    * @param dwValue : 需要设置的值
    * @return VOID
    */
    VOID  SDAtomicSet32(volatile INT32 *pdwMem, INT32 dwValue);

	/**
	* @brief
	* 在内存中原子设置一个64位的值, 需要Windows Vista以上操作系统
	* @param pqwMem : 指向需要设置值的指针
	* @param dwValue : 需要设置的值
	* @return VOID
	*/
    VOID  SDAtomicSet64(volatile INT64 *pdwMem, INT64 dwValue);

    /**
    * @brief
    * 32位原子加法，将原始值添加一个val值
    * @param pdwMem : 指向被加数的指针
    * @param dwValue : 加数
    * @return mem中被加前的原始值
    */
    INT32  SDAtomicAdd32(volatile INT32 *pdwMem, INT32 dwValue);

	/**
	* @brief
	* 64位原子加法，将原始值添加一个val值, 需要Windows Vista以上操作系统
	* @param pdwMem : 指向被加数的指针
	* @param dwValue : 加数
	* @return mem中被加前的原始值
	*/
    INT64  SDAtomicAdd64(volatile INT64 *pdwMem, INT64 dwValue);

    /**
    * @brief
    * 32位原子减法，将原始值减去一个val值
    * @param pdwMem : 指向被减数的指针
    * @param dwValue : 减数
    * @return 被减前的原值
    */
    INT32  SDAtomicSub32(volatile INT32 *pdwMem, INT32 dwValue);

	/**
	* @brief
	* 64位原子减法，将原始值减去一个val值, 需要Windows Vista以上操作系统
	* @param pdwMem : 指向被减数的指针
	* @param dwValue : 减数
	* @return 被减前的原值
	*/
    INT64  SDAtomicSub64(volatile INT64 *pdwMem, INT64 dwValue);

    /**
    * @brief
    * 32位原子加1算法，将原始值加1
    * @param pdwMem : 指向被加数的指针
    * @return mem中被加前的原始值
    */
    INT32  SDAtomicInc32(volatile INT32 *pdwMem);
	
	/**
	* @brief
	* 64位原子加1算法，将原始值加1, 需要Windows Vista以上操作系统
	* @param pdwMem : 指向被加数的指针
	* @return mem中被加前的原始值
	*/
    INT64  SDAtomicInc64(volatile INT64 *pdwMem);

    /**
    * @brief
    * 32位原子减1算法，将原始值减1
    * @param pdwMem : 指向被减数的指针
    * @return mem中被减前的原始值
    */
    INT32  SDAtomicDec32(volatile INT32 *pdwMem);

	/**
	* @brief
	* 64位原子减1算法，将原始值减1, 需要Windows Vista以上操作系统
	* @param pdwMem : 指向被减数的指针
	* @return mem中被减前的原始值
	*/
    INT64  SDAtomicDec64(volatile INT64 *pdwMem);

    /**
    * @brief
    * 32位原子的CAS(Compare and Swap)算法，将32位值pdwMem指向的值与dwCmp比较，
    * 如果一致，则将dwValue的值赋值给*pdwMem
    * @param pdwMem : 指向被比较的值的指针
    * @param dwValue : 若mem指向的值与cmp一致，被赋值的值
    * @param dwCmp : 比较的值
    * @return mem中被赋值前的原始值
    */
    INT32  SDAtomicCas32(volatile INT32 *pdwMem, INT32 dwValue, INT32 dwCmp);

	/**
	* @brief
	* 64位原子的CAS(Compare and Swap)算法，将64位值mem指向的值与cmp比较, 需要Windows Vista以上操作系统
	* 如果一致，则将with的值赋值给*pdwMem
	* @param pdwMem : 指向被比较的值的指针
	* @param dwValue : 若mem指向的值与cmp一致，被赋值的值
	* @param dwCmp : 比较的值
	* @return mem中被赋值前的原始值
	*/
    INT64  SDAtomicCas64(volatile INT64 *pdwMem, INT64 dwValue, INT64 dwCmp);

    /**
    * @brief
    * 交换算法，交换val与mem指向的值
    * @param pdwMem : 指向被交换的值的指针
    * @param dwValue : 交换的值
    * @return mem中被交换前的原始值
    */
    INT32  SDAtomicXchg32(volatile INT32 *pdwMem, INT32 dwValue);

	/**
	* @brief
	* 交换算法，交换val与mem指向的值, 需要Windows Vista以上操作系统
	* @param pdwMem : 指向被交换的值的指针
	* @param dwValue : 交换的值
	* @return mem中被交换前的原始值
	*/
    INT64  SDAtomicXchg64(volatile INT64 *pdwMem, INT64 dwValue);

    /**
    * @brief
    * 原子的指针的CAS(Compare and Swap)算法，将pPtr指向的指针与pCmp指针比较，
    * 如果一致，则将pWith指针与pPtr指向的指针交换
    * @param pPtr : 指向被比较的指针的指针
    * @param pWith : 若pPtr指向的指针与pCmp一致，被赋值的指针
    * @param pCmp : 比较的指针
    * @return 原始的指针
    */
    VOID*  SDAtomicCasptr(volatile VOID **pPtr, VOID *pWith, const VOID *pCmp);

    /**
    * @brief
    * 交换指针算法
    * @param pPtr : pointer to the pointer指向被交换的指针的指针
    * @param pWith : what to swap it with被交换的指针
    * @return 被交换的原始指针
    */
    VOID*  SDAtomicXchgptr(volatile VOID **pPtr, VOID *pWith);

    /** @} */
}

#endif


