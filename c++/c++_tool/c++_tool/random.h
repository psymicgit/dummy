/********************************************************************
            Copyright (c) 2010, 欢乐连线工作室
                   All rights reserved
         
    创建日期：  2010年4月6日 20时16分
    文件名称：  Random.hpp
    说    明：  随机数生成器     
    
    当前版本：  1.00
    作    者：  SiZhi Huang
    概    述：  创建    

*********************************************************************/
#pragma once

namespace nsTool
{
	class RandomGen
	{
	public:
		RandomGen();
		virtual ~RandomGen();

		// 获得随机数
		unsigned int getRand();

		// 获取[0-uiMax)区间的随机数
		unsigned int getRand(unsigned int uiMax);

		unsigned int getRandMax();

		// 获取[0-iMax)区间的随机数
		int getRandInt(int iMax);

		// 获取随机float[0-fRangle);
		float getRandFloat(float fRangle);

		// 正态分布[中值,振幅]
		double gaussian(double mu, double sigma);
		int gaussian(int mu, unsigned int sigma, unsigned int rangle);
		int gaussianHalf(int mu, unsigned int sigma, unsigned int rangle);
	};
}
