

#ifndef _EVENTREACTOR_H_
#define _EVENTREACTOR_H_

#include <sdtype.h>
#include <sdhashmap.h>
#include <sdtime.h>
#include <sdobjectpool.h>
#include <list>
#include <map>

namespace SGDP {
    static const UINT32 MAX_ENABLE_TIME = 0xFFFFFFFF - 1000;	///< 允许用户设置的超时时间最大值
    static const UINT32 WHEELARRAY_COUNT = 5;					///< 时间序列层数,时间粒度最细层为0层
    static const UINT32 WHEEL_ARRAY_LEN = 1000;					///< 时间序列每层有几格

    /**
     * @brief 反应器激发方式
     */
    enum EExecuteType
    {
	    EXECUTE_LAZY = 0,		///< 懒惰执行，会丢失激发事件，下一次激发时间会变成当前激发时间加周期
	    EXECUTE_LOOSE,			///< 不严格执行（默认），会丢失激发事件，但执行时间不变
	    EXECUTE_STRICT			///< 严格执行
    };

    /**
     * @brief 事件状态
     */
    enum EEventStatus
    {
        EVENT_READY = 0,		///< 尚未加入反应器
        EVENT_RUNNING,			///< 正在反应器中运行
        EVENT_HALTING			///< 正在终止
    };

    class CEvent;

    class CEventMeter;
    class CCommonEventMeter;

    template <typename element>
    class CEventPump;

    template <typename element>
    class CListPump;

    class CWheelItem;
    class CWheelLevel;

    template <typename element>
    class CWheelPump;

    template <typename element, typename meter = CCommonEventMeter, typename pump = CWheelPump<element> >
    class CEventReactor;

    /**
     * @brief 周期反应器元素基类
     */
    class CEvent
    {
    public:
        CEvent();
        virtual ~CEvent();

        /**
         * @brief
         * 周期性处理
         */
        virtual UINT32 OnEvent() = 0;

        /**
         * @brief 结束事件
         * @return 结束情况，0（默认）
         */
        virtual BOOL OnDestroy() = 0;

    public:
        enum EEventStatus m_eEventStatus;           ///< 事件状态
        UINT64 m_qwNextTick;                        ///< 下次处理时间
        UINT32 m_dwInterval;                        ///< 周期

        // 向周期反应器开放周期和下一次触发时间等内部成员
        template <typename element, typename meter, typename pump>
        friend class CEventReactor;
        // 用于卸载资源
        friend class CWheelItem;
        // 向各个泵开放周期和下一次触发时间等内部成员
        template <typename element>
        friend class CWheelPump;
        template <typename element>
        friend class CListPump;
    };

    /**
     * @brief 周期反应器
     * @param element : 对象类型
     * @param pump 反应器所使用的泵类型
     */
    template <typename element, typename meter, typename pump>
    class CEventReactor
    {
    public:
	    /**
	     * @brief 构造函数
	     * @param dwInitCount : 对象池初始化大小
	     * @param eExecuteType : 反应器激发方式
	     */
	    CEventReactor(UINT32 dwInitCount, EExecuteType eExecuteType = EXECUTE_LOOSE) :
		    m_oObjectPool(dwInitCount),
		    m_oPump(eExecuteType)
	    {
            m_oPump.SetMeter(&m_oMeter);
            m_oPump.m_poObjectPool = &m_oObjectPool;
	    }

	    /**
	     * @brief 析构函数
	     */
	    virtual ~CEventReactor()
	    {
		    m_oPump.Clear();
	    }

    public:
	    /**
	     * @brief 创建一个元素
	     * @return 元素指针
	     */
	    element* CreateEvent()
	    {
		    return m_oObjectPool.Alloc();
	    }

	    /**
	     * @brief 创建一个元素
	     * @param p1 : 构造参数
	     * @return 元素指针
	     */
	    template<typename P1>
	    element* CreateEvent(P1 p1)
	    {
		    return m_oObjectPool.Alloc(p1);
	    }

	    /**
	     * @brief 创建一个元素
	     * @param p1 : 构造参数
	     * @param p2 : 构造参数
	     * @return 元素指针
	     */
	    template<typename P1, typename P2>
	    element* CreateEvent(P1 p1, P2 p2)
	    {
		    return m_oObjectPool.Alloc(p1, p2);
	    }

	    /**
	     * @brief 创建一个元素
	     * @param p1 : 构造参数
	     * @param p2 : 构造参数
	     * @param p3 : 构造参数
	     * @return 元素指针
	     */
	    template<typename P1, typename P2, typename P3>
	    element* CreateEvent(P1 p1, P2 p2, P3 p3)
	    {
		    return m_oObjectPool.Alloc(p1, p2, p3);
	    }

	    /**
	     * @brief 创建一个元素
	     * @param p1 : 构造参数
	     * @param p2 : 构造参数
	     * @param p3 : 构造参数
	     * @param p4 : 构造参数
	     * @return 元素指针
	     */
	    template<typename P1, typename P2, typename P3, typename P4>
	    element* CreateEvent(P1 p1, P2 p2, P3 p3, P4 p4)
	    {
		    return m_oObjectPool.Alloc(p1, p2, p3, p4);
	    }

	    /**
	     * @brief 创建一个元素
	     * @param p1 : 构造参数
	     * @param p2 : 构造参数
	     * @param p3 : 构造参数
	     * @param p4 : 构造参数
	     * @param p5 : 构造参数
	     * @return 元素指针
	     */
	    template<typename P1, typename P2, typename P3, typename P4, typename P5>
	    element* CreateEvent(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	    {
		    return m_oObjectPool.Alloc(p1, p2, p3, p4, p5);
	    }

        /**
         * @brief 开始元素周期性激发
         * @param poEvent : 之前创建的事件对象指针
         * @param bDoOnDestroy : 是否激发销毁回调
         * @return 执行结果，TRUE成功，FALSE失败
         */
        BOOL DestroyEvent(element* poEvent, BOOL bDoOnDestroy = TRUE)
        {
            if(EVENT_HALTING == poEvent->m_eEventStatus)
            {
                return FALSE;
            }
            else if(EVENT_READY == poEvent->m_eEventStatus)
            {
                m_oObjectPool.Free(poEvent);
                return TRUE;
            }
            else
            {
                poEvent->m_eEventStatus = EVENT_HALTING;
                if(FALSE == bDoOnDestroy)
                {
                    return TRUE;
                }
                else
                {
                    return poEvent->OnDestroy();
                }
            }
        }

	    /**
	     * @brief 开始元素周期性激发
	     * @param Element : 被添加的元素
	     * @param dwInterval : 激发间隔时间(ms)
	     * @param qwStartTime : 开始激发时间
	     * @return 执行结果，TRUE成功，FALSE失败
	     */
	    BOOL SetEvent(element* poElement, UINT32 dwInterval, UINT64 qwStartTime = SDTimeSecs())
	    {
			dwInterval = 0 == dwInterval ? 1 : dwInterval;
            if(NULL == poElement)
            {
                return FALSE;
            }
            CEvent* poEventElement = (CEvent*)poElement;
            if(EVENT_READY != poEventElement->m_eEventStatus)
            {
                // 防止重复添加
                return FALSE;
            }
		    poEventElement->m_qwNextTick = qwStartTime + dwInterval;
		    poEventElement->m_dwInterval = dwInterval;
		    poEventElement->m_eEventStatus = EVENT_RUNNING;
		    m_oPump.AddElement(poElement);
		    return TRUE;
	    }

	    /**
	     * @brief 重置执行周期
	     * @param Element : 被添加的元素
	     * @param dwInterval : 激发间隔时间(ms)
	     * @return 执行结果，TRUE成功，FALSE失败
	     */
	    BOOL ResetInterval(element* poElement, UINT32 dwInterval)
	    {	
		    return m_oPump.ResetInterval(poElement, dwInterval);
	    }

	    /**
	     * @brief 周期性处理
	     * @param qwCurTick : 当前系统Tick
	     */
	    VOID Run(UINT64 qwCurTick = SGDP::SDTimeSecs())
	    {
			//CAutoCycle oAutoCycle("CEventReactor::Run");

		    m_oMeter.BeginLoop();
		    UINT32 dwEventCount = m_oPump.Run(qwCurTick);
		    m_oMeter.EndLoop(dwEventCount);
	    }

	    /**
	     * @brief 清空元素
	     */
	    VOID Clear()
	    {
		    m_oPump.Clear();
	    }

        /**
         * @brief 设置是否开启统计器
         * @param bStatus : 统计器是否开启
         */
        VOID SetMeterStatus(BOOL bStatus)
        {
            m_oMeter.m_bStatus = bStatus;
        }

        /**
         * @brief 获取统计器
         * @return 统计器引用
         */
        inline meter& GetMeter()
        {
            return m_oMeter;
        }

    protected:
	    pump        m_oPump;                        ///< 元素激发泵
        meter       m_oMeter;                       ///< 统计器
	    SGDP::CSDObjectPool<element> m_oObjectPool; ///< 对象池实例
    };

    /**
     * @brief 周期元素统计信息结构体
     */
    typedef struct tagElementMeterInfo
    {
	    UINT32 dwEventCount;		///< 本轮有效元素周期次数
	    UINT64 qwEventTick;		    ///< 本轮有效元素周期总耗时
	    UINT64 qwEventMaxTick;		///< 本轮最大耗时
	    UINT64 qwHistoryMaxTick;	///< 历时最大耗时
    }SElementMeterInfo;


    /**
     * @brief 周期反应器统计器基类
     */
    class CEventMeter
    {
    public:
	    CEventMeter();
	    virtual ~CEventMeter();

	    /**
	     * @brief 开始记录泵的周期循环
	     * @param qwCurTick : 当前系统Tick
	     */
	    virtual VOID BeginLoop(UINT64 qwCurTick = SGDP::SDTimeSecs()) = 0;

	    /**
	     * @brief 结束记录泵的周期循环
	     * @param dwEventCount : 泵执行周期中执行了多少有效元素周期
	     * @param qwCurTick : 当前系统Tick
	     */
	    virtual VOID EndLoop(UINT32 dwEventCount, UINT64 qwCurTick = SGDP::SDTimeSecs()) = 0;

	    /**
	     * @brief 对单个元素周期进行抽样
	     * @param dwElementType : 元素类型，由用户返回
	     * @param qwTickCount : 本地元素周期持续时间
	     */
	    virtual INT32 AddSamplePoint(UINT32 dwElementType, UINT64 qwTickCount) = 0;

	    /**
	     * @brief 重置最近统计数据
	     */
	    virtual VOID Reset() = 0;

	    /**
	     * @brief 清空全部统计数据
	     */
	    virtual VOID Clear() = 0;

	    /**
	     * @brief 开始统计
	     */
	    virtual INT32 Start() = 0;

	    /**
	     * @brief 暂停统计
	     */
	    virtual INT32 Stop() = 0;

    public:
        /**
         * @brief 是否开启统计
         */
        BOOL m_bStatus;
    };

    const static UINT32 DEFAULT_METERINFO_SIZE = 100;
    /**
     * @brief 默认时钟周期统计器
     */
    class CCommonEventMeter:public CEventMeter
    {
    public:
	    CCommonEventMeter();
	    virtual ~CCommonEventMeter();

	    /**
	     * @brief 开始记录泵的周期循环
	     * @param qwCurTick : 当前系统Tick
	     */
	    VOID BeginLoop(UINT64 qwCurTick = SGDP::SDTimeSecs());

	    /**
	     * @brief 结束记录泵的周期循环
	     * @param dwEventCount : 泵执行周期中执行了多少有效元素周期
	     * @param qwCurTick : 当前系统Tick
	     */
	    VOID EndLoop(UINT32 dwEventCount, UINT64 qwCurTick = SGDP::SDTimeSecs());

	    /**
	     * @brief 对单个元素周期进行抽样
	     * @param dwElementType : 元素类型，由用户返回
	     * @param qwTickCount : 本地元素周期持续时间
	     */
	    INT32 AddSamplePoint(UINT32 dwElementType, UINT64 qwTickCount);

	    /**
	     * @brief 重置最近统计数据
	     */
	    VOID Reset();

	    /**
	     * @brief 清空全部统计数据
	     */
	    VOID Clear();

	    /**
	     * @brief 开始统计
	     */
	    INT32 Start();

	    /**
	     * @brief 暂停统计
	     */
	    INT32 Stop();

    public:
        /**
        * @brief 最近一次泵周期消耗时间
        */
        UINT64 m_qwLastTickCount;

        /**
        * @brief 最近一次泵周期执行了多少有效元素周期
        */
        UINT32 m_dwLastEventCount;

        /**
        * @brief 历时泵周期消耗时间总和
        */
        UINT64 m_qwTotalTickCount;

        /**
        * @brief 历史泵周期执行了多少有效元素周期总和
        */
        UINT32 m_dwTotalEventCount;

        /**
        * @brief 历史泵周期执行了多少元素周期总和
        */
        UINT32 m_dwTotalLoopCount;

        /**
        * @brief 最近一段时间泵周期消耗时间总和
        */
        UINT64 m_qwRecentTickCount;

        /**
        * @brief 最近一段时间泵周期执行了多少有效元素周期总和
        */
        UINT32 m_dwRecentEventCount;

        /**
        * @brief 最近一段时间泵周期执行了多少元素周期总和
        */
        UINT32 m_dwRecentLoopCount;

        /**
        * @brief 周期元素统计信息
        */
        SElementMeterInfo m_aoElementMeterInfo[DEFAULT_METERINFO_SIZE];

    private:
	    /**
	     * @brief 开始统计泵循环的时间
	     */
	    UINT64 m_qwBeginTick;
    };

    /**
     * @brief 周期激发泵基类
     * @param element : 托管对象
     */
    template <typename element>
    class CEventPump
    {
    public:
        CEventPump()
        {
            m_bMeterStatus = FALSE;
        }

        virtual ~CEventPump()
        {

        }

	    /**
	     * @brief 添加元素
	     * @param poElement : 被添加的元素指针
	     * @return 执行结果，TRUE成功，FALSE失败
	     */
	    virtual BOOL AddElement(element* poElement) = 0;

	    /**
	     * @brief 重置执行周期
	     * @param Element : 被添加的元素
	     * @param dwInterval : 激发间隔时间(ms)
	     * @return 执行结果，TRUE成功，FALSE失败
	     */
	    virtual BOOL ResetInterval(element* poElement, UINT32 dwInterval) = 0;

	    /**
	     * @brief 清空元素
	     */
	    virtual VOID Clear() = 0;

	    /**
	     * @brief 周期性处理
	     * @param qwCurTick : 当前系统Tick
	     */
	    virtual UINT32 Run(UINT64 qwCurTick) = 0;

        /**
         * @brief 设置统计器
         * @param poMeter : 统计器指针
         */
        virtual VOID SetMeter(CEventMeter* poMeter)
        {
            m_poMeter = poMeter;
        }

    public:
	    /**
	     * @brief 是否开启统计器
	     */
	    BOOL m_bMeterStatus;

        /**
         * @brief 对象池指针
         */
        SGDP::CSDObjectPool<element> * m_poObjectPool; /**<对象池实例*/

	    /**
	     * @brief 统计器
	     */
	    CEventMeter* m_poMeter;
    };

    /**
     * @brief 链表泵
     * @param element 必须为托管对象指针
     */
    template <typename element>
    class CListPump : public CEventPump<element>
    {
    public:
	    CListPump(EExecuteType eExecuteType = EXECUTE_LOOSE)
	    {
		    m_eExecuteType = eExecuteType;
			m_qwLastRunTime = SDTimeSecs();
	    }

	    /**
	     * @brief
	     * 添加元素
	     * @param poElement : 被添加的元素指针
	     * @return 执行结果，TRUE成功，FALSE失败
	     */
	    BOOL AddElement(element* poElement)
	    {
		    m_lsElements.push_back(poElement);
		    return TRUE;
	    }

	    /**
	     * @brief
	     * 重置执行周期
	     * @param Element : 被添加的元素
	     * @param dwInterval : 激发间隔时间(ms)
	     * @return 执行结果，TRUE成功，FALSE失败
	     */
	    BOOL ResetInterval(element* poElement, UINT32 dwInterval)
	    {
		    poElement->m_dwInterval = dwInterval;
		    return TRUE;
	    }

        UINT64 GetMsFromLast(UINT64 qwLastTime)
        {
            return SDTimeSecs() - qwLastTime;;
        }

	    /**
	     * @brief
	     * 清空元素
 	     */
	    VOID Clear()
	    {
		    m_lsElements.clear();
	    }

	    /**
	     * @brief 周期性处理
	     * @param qwCurTick : 当前系统Tick
	     */
	    UINT32 Run(UINT64 qwCurTick)
		{
			//避免空转
			if(qwCurTick <= m_qwLastRunTime)
			{
				return 0;
			}
			UINT32 dwElementType; // 元素类型
			UINT64 qwTick = 0;
			UINT32 dwRet = 0;
			for(typename std::list<element*>::iterator iter = m_lsElements.begin(); iter != m_lsElements.end();)
			{
				UINT64 qwElementTick = (*iter)->m_dwNextTick;
				UINT32 dwInterval = (*iter)->m_dwInterval;
				if(EVENT_HALTING == (*iter)->m_eEventStatus)
				{
					// 删除被确认无效的元素
					this->m_poObjectPool->Free(*iter);
					m_lsElements.erase(iter++);
				}
				else if(qwElementTick <= qwCurTick) // 处理超时
				{
					if(EXECUTE_LOOSE == m_eExecuteType)
					{
						// 不严格执行，会丢失激发事件，但执行时间不变
						if(FALSE == this->m_bMeterStatus)
						{
							dwElementType = (*iter)->OnEvent();
						}
						else
						{
							qwTick = SGDP::SDTimeSecs();
							dwElementType = (*iter)->OnEvent();
							qwCurTick = SDTimeSecs();

							UINT64 qwTickTime = GetMsFromLast(qwTick);
							this->m_poMeter->AddSamplePoint(dwElementType, qwTickTime);
						}
						dwRet++;
						while(qwElementTick <= qwCurTick)
						{
							qwElementTick += dwInterval;
						}
						(*iter)->m_dwNextTick = qwElementTick;
					}
					else if(EXECUTE_LAZY == m_eExecuteType)
					{
						// 不严格执行，会丢失激发事件，下一次激发时间会变成当前激发时间加周期
						if(FALSE == this->m_bMeterStatus)
						{
							dwElementType = (*iter)->OnEvent();
						}
						else
						{
							qwTick = SGDP::SDTimeSecs();
							dwElementType = (*iter)->OnEvent();

							qwCurTick = SDTimeSecs();
							UINT64 qwTickTime = GetMsFromLast(qwTick);
							this->m_poMeter->AddSamplePoint(dwElementType, qwTickTime);
						}
						dwRet++;
						// 
						(*iter)->m_dwNextTick = qwCurTick + dwInterval;
					}
					else if(EXECUTE_STRICT == m_eExecuteType)
					{
						// 严格执行
						while(qwElementTick < qwCurTick)
						{
							if(EVENT_HALTING == (*iter)->m_eEventStatus)
							{
								break;
							}
							if(FALSE == this->m_bMeterStatus)
							{
								dwElementType = (*iter)->OnEvent();
							}
							else
							{
								qwTick = SGDP::SDTimeSecs();
								dwElementType = (*iter)->OnEvent();
								qwCurTick = SDTimeSecs();

								UINT64 qwTickTime = GetMsFromLast(qwTick);
								this->m_poMeter->AddSamplePoint(dwElementType, qwTickTime);
							}
							dwRet++;
							qwElementTick += dwInterval;
						}
						(*iter)->m_dwNextTick = qwElementTick;
					}
					iter++;
				}
				else
				{
					iter++;
				}
			}
			m_qwLastRunTime = SDTimeSecs();

			return dwRet;
		}

    private:
	    std::list<element*> m_lsElements;		///< 元素链表
	    EExecuteType	m_eExecuteType;			///< 执行方式
		UINT64			m_qwLastRunTime;		///< 上次执行时间
    };

    /**
     * @brief 定时器元素
     */
    class CWheelItem
    {
    public:
	    /**
	     * @brief 初始化单个定时器
	     * @param poPumpCtrl 传入的定时器控制器
	     * @param poElement 输入的定时器处理函数接口
	     * @param dwInterval 定时器的间隔
	     */
	    template <typename element>
	    VOID Init(CWheelPump<element>* poPumpCtrl, CEvent* poElement)
	    {
		    m_poElementHandler = poElement;
		    m_byWheelLevel = WHEELARRAY_COUNT;
		    m_qwLastExpTime = SGDP::SDTimeSecs();
		    for(UINT32 i = 0; i < WHEELARRAY_COUNT; i++)
		    {
			    m_aqwSlotIndexes[i] = poPumpCtrl->GetSlotLevel(i).GetCurSlotIndex();
		    }
	    }

	    /**
	     * @brief 建立下一次触发
	     * @param qwDelay : 当前时延
	     * @return 所在层级
	     */
	    UINT8 SetupForNextTrigger(UINT64 qwDelay, UINT32 dwInterval = 0);

	    /**
	     * @brief 获取下一次触发时间
	     * @return 下一次触发时间
	     */
	    inline UINT64 GetLastExpdTime()
	    {
		    return 	m_qwLastExpTime;
	    }

	    /**
	     * @brief 设置下一次触发时间
	     * @param dwExpTime : 下一次触发时间
	     */
	    inline VOID SetLastExpdTime(UINT64 qwExpTime)
	    {
		    m_qwLastExpTime = qwExpTime;
	    }

	    /**
	     * @brief 获取下一个激发时间
	     * @param byLevel : 定时器层级
	     * @return 下一个激发时间
	     */
	    UINT64 GetSlotIndexForNextTrigger(UINT8 byLevel);

	    /**
	     * @brief 周期性处理
	     */
	    INT32 OnEvent();

    public:
	    UINT8				m_byWheelLevel;						///< 时钟当前处于的时间序列层级
	    CEvent*             m_poElementHandler;					///< 用户设置的定时器超时处理函数
        UINT64				m_qwLastExpTime;					///< 上次超时时间

    private:
	    UINT64				m_aqwSlotIndexes[WHEELARRAY_COUNT]; ///< 定时器元素的下次超时时间
    };

    /**
     * @brief 定时器元素链表
     */
    typedef std::list<CWheelItem*> CWheelItemList;

    /**
     * @brief 定时器层级管理类
     */
    class CWheelLevel
    {
    public:
	    /**
	     * @brief 构造函数
	     */
	    CWheelLevel();

	    /**
	     * @brief 时间向后移动
	     * @param bRewindOnEnd : 是否在终点循环到初始位置
	     * @return 如果到达终点返回FALSE，否则返回TRUE
	     */
	    BOOL NextSlot(BOOL bRewindOnEnd);

	    /**
	     * @brief 加载新层级时间槽队列
	     * @param oSlotLevel : 待加载的时间槽层级对象
	     * @return 如果到达终点返回FALSE，否则返回TRUE
	     */
	    VOID ReloadFromLevel(CWheelLevel &oSlotLevel);

	    /**
	     * @brief 初始化层级
	     * @param level : 目标层级
	     */
	    inline VOID Init(UINT8 level)
	    {
		    m_byWheelLevel = level;
	    }

	    /**
	     * @brief 获取当前层级索引
	     * @return 当前层级索引
	     */
	    inline UINT32 GetCurSlotIndex()
	    {
		    return m_dwCurSlot;
	    }

	    /**
	     * @brief 获取当前层级索引
	     * @return 当前层级索引
	     */
	    inline std::list<CWheelItem*> &GetCurSlot()
	    {
		    return m_aoWheels[m_dwCurSlot];
	    }

	    /**
	     * @brief 添加定时器元素
	     * @param poWheelItem 定时器元素指针
	     */
	    inline VOID AddItem(CWheelItem* poWheelItem)
	    {
		    poWheelItem->m_byWheelLevel = m_byWheelLevel;
		    m_aoWheels[poWheelItem->GetSlotIndexForNextTrigger(m_byWheelLevel)].push_back(poWheelItem);
	    }

	    /**
	     * @brief 删除定时器元素
	     * @param poWheelItem 定时器元素指针
	     */
	    inline VOID DelItem(CWheelItem* poWheelItem)
	    {
		    if(poWheelItem->m_byWheelLevel != m_byWheelLevel)
		    {
			    return;
		    }
		    m_aoWheels[poWheelItem->GetSlotIndexForNextTrigger(m_byWheelLevel)].remove(poWheelItem);
	    }

        inline CWheelItemList* GetWheelItemLists()
        {
            return m_aoWheels;
        }

    private:
	    UINT8					m_byWheelLevel;						///< 当前时间序列层级数
	    UINT32					m_dwCurSlot;						///< 当前格数
	    CWheelItemList	        m_aoWheels[WHEEL_ARRAY_LEN];	///< 时间槽链表
    };

    /**
     * @brief 时间槽泵
     * @param element 托管对象
     */
    template <typename element>
    class CWheelPump : public CEventPump<element>
    {
    public:
	    /**
	     * @brief 构造函数
	     * @param eExecuteType : 执行方式
	     */
	    CWheelPump(EExecuteType eExecuteType = EXECUTE_LOOSE) : m_oWheelItemPool()
	    {
		    for(UINT32 i = 0; i < WHEELARRAY_COUNT; i++)
		    {
			    m_aoSlotLevels[i].Init(i);
		    }

		    m_qwLastTickCount = SGDP::SDTimeSecs();
            m_eExecuteType = eExecuteType;
			m_qwLastRunTime = SDTimeSecs();
	    }

	    /**
	     * @brief 析构函数
	     */
	    ~CWheelPump()
	    {
	    }

	    /**
	     * @brief 添加元素
	     * @param poElement : 被添加的元素指针
	     * @return 执行结果，TRUE成功，FALSE失败
	     */
	    BOOL AddElement(element* poElement)
	    {
            CEvent* poEventElement = (CEvent*)poElement;
		    if(NULL == poElement || (poEventElement->m_dwInterval > MAX_ENABLE_TIME) || (0 == poEventElement->m_dwInterval))
		    {
			    return FALSE;
		    }
		    CWheelItem* poWheelItem = m_oWheelItemPool.Alloc();
		    if(NULL == poWheelItem)
		    {
			    return FALSE;
		    }
		    poWheelItem->Init(this, poEventElement);
            
            UINT64 qwTickCount = poEventElement->m_qwNextTick - SDTimeSecs();

		    return AddItemForNextTrigger(poWheelItem, (UINT32)qwTickCount);
	    }

	    /**
	     * @brief 重置执行周期
	     * @param poElement : 被添加的元素指针
	     * @param dwInterval : 激发间隔时间（毫秒）
	     * @return 执行结果，TRUE成功，FALSE失败
	     */
	    BOOL ResetInterval(element* poElement, UINT32 dwInterval)
	    {
            CEvent* poEventElement = (CEvent*)poElement;
            poEventElement->m_dwInterval = dwInterval;
            return TRUE;
	    }

	    /**
	     * @brief 清空元素
	     */
	    VOID Clear()
	    {
            for(UINT64 qwMsFromLast = GetMsFromLast(); qwMsFromLast > 0; qwMsFromLast--)
            {
                for(UINT16 i = 0; i < WHEELARRAY_COUNT; i++)
                {
                    do
                    {
                        CWheelItemList &oCurSlot = m_aoSlotLevels[i].GetCurSlot();
                        while(FALSE == oCurSlot.empty())
                        {
                            CWheelItem* poWheelItem = oCurSlot.front();
                            m_aoSlotLevels[poWheelItem->m_byWheelLevel].DelItem(poWheelItem);
                            this->m_poObjectPool->Free(dynamic_cast<element*>(poWheelItem->m_poElementHandler));
                            m_oWheelItemPool.Free(poWheelItem);

                            // 判断是否在OnEvent里面删除了该元素
                            if (oCurSlot.empty() || (oCurSlot.front() != poWheelItem))
                            {
                                continue;
                            }

                            oCurSlot.pop_front();
                        }
                    }while(m_aoSlotLevels->NextSlot(FALSE));
                }
            }

            /*
		    for(UINT32 i = 0; i < WHEELARRAY_COUNT; i++)
		    {
			    m_aoSlotLevels[i].Init(i);
		    }
		    */
	    }

	    /**
	     * @brief 周期性处理
	     * @param qwCurTick : 当前系统Tick
	     */
	    UINT32 Run(UINT64 qwCurTick)
	    {
			//避免空转
			if(qwCurTick <= m_qwLastRunTime)
			{
				return 0;
			}
		    UINT32 dwEventCount = 0;
		    for(UINT64 qwMsFromLast = GetMsFromLast(); qwMsFromLast > 0; qwMsFromLast--)
		    {
			    dwEventCount += TrigerItemsForCurSlot();
		    }

			m_qwLastRunTime = SDTimeSecs();
		    return dwEventCount;
	    }

    private:
	    /**
	     * @brief 得到某个时钟轮结构
	     * @param nLevel : 时钟轮的级别
	     * @return 时钟层级
	     */
	    inline CWheelLevel& GetSlotLevel(UINT32 nLevel)
	    {
		    return m_aoSlotLevels[nLevel];
	    }

	    /**
	     * @brief 用下一个循环将本层时钟填充
	     * @param dwLevel : 填充的目标层
	     * @return 成功返回TRUE，失败返回FALSE
	     */
	    BOOL ReloadSlotLevel(UINT8 dwLevel = 0)
	    {
		    if(dwLevel >= WHEELARRAY_COUNT-1)
		    {
			    return FALSE;
		    }
		    CWheelLevel& oNextLevel = m_aoSlotLevels[dwLevel + 1];

		    if(oNextLevel.NextSlot(TRUE) == FALSE)
		    {
			    ReloadSlotLevel(dwLevel + 1);
		    }
		    m_aoSlotLevels[dwLevel].ReloadFromLevel(oNextLevel);
		    return TRUE;
	    }

	    /**
	     * @brief 处理定时器超时
	     */
	    UINT32 TrigerItemsForCurSlot();

	    /**
	     * @brief 根据当前时间和超时时间，将定时器放置到合适的位置
	     * @param poWheelItem : 指向timer结构的指针
	     * @return 成功返回TRUE，失败返回FALSE
	     */
	    BOOL AddItemForNextTrigger(CWheelItem *poWheelItem, UINT32 dwInterval = 0)
	    {
		    UINT64 qwMsFromLast = GetMsFromLast(m_qwLastTickCount);
		    UINT32 dwLevel = poWheelItem->SetupForNextTrigger(qwMsFromLast, dwInterval);
		    m_aoSlotLevels[dwLevel].AddItem(poWheelItem);
		    return TRUE;
	    }

	    UINT64 GetMsFromLast()
	    {
		    UINT64 qwCurTick = SGDP::SDTimeSecs();
			qwCurTick -= m_qwLastTickCount;
			m_qwLastTickCount = SGDP::SDTimeSecs();

			return qwCurTick;
	    }

	    UINT64 GetMsFromLast(UINT64 qwLastTime)
	    {
			return SDTimeSecs() - qwLastTime;
	    }

    private:
	    EExecuteType			m_eExecuteType;							///< 执行方式
	    SGDP::CSDObjectPool<CWheelItem, SGDP::CSDMutex> m_oWheelItemPool;	///< 定时器的缓冲池
	    CWheelLevel             m_aoSlotLevels[WHEELARRAY_COUNT];		///< 存放时间槽的数组
	    UINT64					m_qwLastTickCount;						///< 上一次调用时间	  
		UINT64					m_qwLastRunTime;		///< 上次执行时间

    // 向时钟元素开放成员
    friend class CWheelItem;
    };

}

#include "eventreactor.inl"

#endif // #ifndef _EVENTREACTOR_H_

