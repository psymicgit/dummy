#ifndef SD_IDPOOL_H
#define SD_IDPOOL_H
#include <list>
#include "sdtype.h"
#include "sdstring.h"
#include "sddebug.h"
#include "sddebug.h"
using namespace std;
namespace SGDP
{
    /**
    * @brief IDPool
    */
    template <typename T>
	class CSDIDPool
	{
        /*结点结构*/
		typedef struct Node
		{
			T obj;           //结点中存储的模板类型对象 
			UINT32 next;     //当前结点的下一个节点位置
		}Ele;
	public:
        /**
        * @brief 初始化IDPool
        * @param dwSize : ID池的大小
        * @return TRUE:成功初始化
        */
		BOOL Init(UINT32 dwSize)
		{			
			m_aObject = SDNew Ele[dwSize];
			memset(m_aObject,0,dwSize*sizeof(Ele));
			for ( UINT32 i = 0; i < dwSize; i++ )
				m_aObject[i].next = i+1;
			m_dwSize = m_freeSize = dwSize;
			m_dwFreeHead = 0;
			m_dwFreeTail = dwSize-1;
			return TRUE;
		}

		/**
		* @brief 构造函数，初始化ID对象池和大小
		*/
		CSDIDPool(): m_aObject(NULL), m_dwSize(0) {}

		/**
		* @brief 析构函数，释放ID对象池
		*/
		~CSDIDPool()
		{
			SDDelete [] m_aObject;
		}

        /**
        * @brief 向IDPool中添加一个对象
        * @param object : 要添加的对象
        * @return dwID 对象添加所在的位置
        */
		UINT32 Add(T object)
		{
			if ( 0 == m_freeSize) return (UINT32)(-1);
			UINT32 dwID = m_dwFreeHead;
			m_aObject[m_dwFreeHead].obj = object;   //从头拿一个
			m_dwFreeHead = m_aObject[m_dwFreeHead].next; // 更新队头
			m_aObject[dwID].next = (UINT32)-1;//next是－1表示在使用中
			--m_freeSize;		
			return dwID;
		}

        /**
        * @brief 根据所给ID和对象，在IDPool中查找是否匹配
        * @param dwID   : 要查找的位置
        * @param object : 要查找的对象
        * @return TRUE:查找成功 FALSE:查找失败
        */
		BOOL Find(UINT32 dwID, T& pObject)
		{
			if ((dwID >= m_dwSize) )
			{
				return FALSE;
			}

			if (m_aObject[dwID].next != (UINT32)(-1)) //不是在使用中
				 return FALSE;

			pObject = m_aObject[dwID].obj;
			return TRUE;
		}

        /**
        * @brief 删除指定位置的节点
        * @param dwID : 指定的位置
        * @return VOID
        */
		VOID Del(UINT32 dwID)
		{
			if ((dwID >= m_dwSize) )
			{
				return;
			}

			if (m_aObject[dwID].next != (UINT32)(-1))  //不是在使用中
				return;
			++m_freeSize;
			if ( m_freeSize == 1) //还回来的第一个节点，是头又是尾
			{
				m_dwFreeHead = m_dwFreeTail = dwID;
				m_aObject[m_dwFreeHead].next = m_dwSize;
				return;
			}
			m_aObject[dwID].next = m_dwSize;  //这个表示无后继了
			m_aObject[m_dwFreeTail].next = dwID; //放到队尾
			m_dwFreeTail = dwID;  //更新队尾		
		}

        /**
        * @brief 获取IDPool中最后一个实际存储的对象
        * @param oFunc : 保存取到的对象
        * @return TRUE:获取成功
        */
        template<class CFunc>
        BOOL For_Each(CFunc oFunc)
        {
            if(IsEmpty())
                return TRUE;

            UINT32 dwSize = 0;
            UINT32 dwCurrentSize = Capacity() - GetFreeCount();
            for(UINT32 i = 0; i < m_dwSize; ++i)
            {
                if(m_aObject[i].next == (UINT32)(-1))
                {
                    oFunc(m_aObject[i].obj);
                    dwSize++;

                    if(dwSize == dwCurrentSize)
                        break;
                }
            }
            return TRUE;
        }

        /**
        * @brief 获取IDPool中空闲的个数
        * @return 空闲个数
        */
		std::list<UINT32>::size_type GetFreeCount() const
		{
			return m_freeSize;
		}

        /**
        * @brief 判断IDPool中实际存储对象是否为空
        * @return TRUE:空 FALSE:非空
        */
		BOOL IsEmpty() const
		{
			return ( m_freeSize == m_dwSize );
		}

        /**
        * @brief 判断IDPool中实际存储对象是否满
        * @return TRUE:满 FALSE:有空闲
        */
		BOOL IsFull() const
		{
			return ( m_freeSize == 0 );
		}

        /**
        * @brief 获取IDPool容量
        * @return IDPool存储大小
        */
		std::list<UINT32>::size_type Capacity() const
		{
			return m_dwSize;
		}

        /**
        * @brief 获取当前使用率
        * @return 当前使用率
        */
		UINT32 GetUSG()
		{
			return 100 - m_freeSize*100/m_dwSize;
		}
		
	private:
		UINT32			m_dwSize;
		UINT32			m_freeSize;
		Ele *			m_aObject;
		UINT32			m_dwFreeHead;
		UINT32			m_dwFreeTail;	
	};
}
#endif

