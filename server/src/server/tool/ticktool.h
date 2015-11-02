///<------------------------------------------------------------------------------
//< @file:   server\tool\ticktool.h
//< @author: 洪坤安
//< @date:   2015年3月27日 21:42:31
//< @brief:	 时钟周期工具
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _ticktool_h_
#define _ticktool_h_

namespace ticktool
{
	// 返回时钟周期
	uint64 tick();

	// 返回两次时钟周期的秒差
	double tickDiff(uint64 old_tick);
}

// 计时器，用于测量时间耗费
class Tick
{
public:
	Tick(const char* jobText, int jobCnt = 0)
		: m_jobText(jobText)
		, m_jobCnt(jobCnt)
		, m_born(0)
		, m_retickNum(0)
		, m_historyMax(0)
	{
		// LOG_WARN << "------- start " << m_msg << std::endl;
		startTick();

		m_born = m_tick;
	}

	~Tick()
	{
		if (1 == m_retickNum) {
			int jobcnt = m_jobCnt;
			m_jobCnt = 0;

			print(jobcnt);
		} else {
			printHistoryMax();
		}
	}

	// 开始计时
	void startTick();

	// 结束计时，并返回当前时间与计时起点的时间差，单位：秒
	double endTick();

	// 返回本计时器出生到现在的时间，单位：秒
	double life();

	// 打印当前与计时起点的时间差、平均每次耗时、平均每秒执行次数
	void print(int jobCnt, const char *text = "")
	{
		if (jobCnt <= 0) {
			jobCnt = 1;
		}

		m_jobCnt += jobCnt;

		double costTime = endTick();

		double speed = costTime / jobCnt;
		double per = 1.0f / speed;

		LOG_WARN << "------ " << m_jobText << text << " jobcnt = " << m_jobCnt << ", cost time = " << costTime << ", avg cost = " << speed << ", per second = " << per << "" << std::endl;

		if (m_historyMax < per) {
			m_historyMax = per;
		}
	}

	// 打印出历史最高的每秒执行次数
	void printHistoryMax()
	{
		double lifetime = ticktool::tickDiff(m_born);
		double speed = lifetime / m_jobCnt;

		double avg = 1.0f / speed;

		LOG_WARN << "------ " << m_jobText << " avg per second = " << avg << ", history max excute per second = " << m_historyMax;
	}

public:
	std::string m_jobText;		// 本计时器的作业注释

private:
	uint64		m_born;			// 本计时器的出生时间

	uint64		m_tick;			// 计时起点，计时起点可以手动重设

	double		m_historyMax;	// 历史曾经达到过的最高每秒执行次数

	int			m_jobCnt;		// 历史执行过的作业数

	int			m_retickNum;	// 重新计时次数
};


#endif //_ticktool_h_