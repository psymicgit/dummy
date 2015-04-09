
#ifndef SDMATH_H
#define SDMATH_H
/**
* @file sdmath.h
* @brief 数学计算类
*
**/
namespace SGDP
{
    /**
    * @defgroup 取最大最小值
    * @ingroup  SGDP
    * @{
    */

    /**
    * @brief
    * 比较两个数据，返回最小值
    * @param const T & __a ,const T & __b : 进行比较的两个值
    * @return  T &  : 返回最小值
    */
    template<typename T >
    inline const T SDMin(const T & __a, const T & __b)
    {
        return (__b < __a ) ? __b: __a;
    }

    /**
    * @brief
    * 比较两个数据，返回最大值
    * @param const T & __a ,const T & __b : 进行比较的两个值
    * @return T &  : 返回最大值
    */
    template<typename T >
    inline const T SDMax(const T & __a, const T & __b)
    {
        return (__a < __b)? __b : __a;
    }

    /** @} */

} //namespace SGDP

#endif // 


