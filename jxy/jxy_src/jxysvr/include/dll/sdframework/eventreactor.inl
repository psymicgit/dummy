namespace SGDP {

    /**
     * @brief
     * 周期性处理
     */
  //  template <typename element>
  //  UINT32 CListPump<element>::Run(UINT64 qwCurTick)
  //  {
		////避免空转
		//if(qwCurTick <= m_qwLastRunTime)
		//{
		//	return 0;
		//}
	 //   UINT32 dwElementType; // 元素类型
	 //   UINT64 qwTick = 0;
	 //   UINT32 dwRet = 0;
	 //   for(typename std::list<element*>::iterator iter = m_lsElements.begin(); iter != m_lsElements.end();)
	 //   {
		//    UINT64 qwElementTick = (*iter)->m_dwNextTick;
		//    UINT32 dwInterval = (*iter)->m_dwInterval;
  //          if(EVENT_HALTING == (*iter)->m_eEventStatus)
		//    {
		//	    // 删除被确认无效的元素
  //              typename std::list<element*>::iterator iterTemp = iter;
  //              iter++;
  //              this->m_poObjectPool->Free(*iterTemp);
  //              m_lsElements.erase(iterTemp);
		//    }
		//    else if(qwElementTick <= qwCurTick) // 处理超时
		//    {
		//	    if(EXECUTE_LOOSE == m_eExecuteType)
		//	    {
		//		    // 不严格执行，会丢失激发事件，但执行时间不变
		//		    if(FALSE == this->m_bMeterStatus)
		//		    {
		//			    dwElementType = (*iter)->OnEvent();
		//		    }
		//		    else
		//		    {
		//			    qwTick = SGDP::SDTimeSecs();
		//			    dwElementType = (*iter)->OnEvent();
  //                      qwCurTick = SDTimeSecs();

  //                      UINT64 qwTickTime = GetMsFromLast(qwTick);
		//			    this->m_poMeter->AddSamplePoint(dwElementType, qwTickTime);
		//		    }
		//		    dwRet++;
		//		    while(qwElementTick <= qwCurTick)
		//		    {
		//			    qwElementTick += dwInterval;
		//		    }
  //                  (*iter)->m_dwNextTick = qwElementTick;
		//	    }
		//	    else if(EXECUTE_LAZY == m_eExecuteType)
		//	    {
		//		    // 不严格执行，会丢失激发事件，下一次激发时间会变成当前激发时间加周期
  //                  if(FALSE == this->m_bMeterStatus)
  //                  {
  //                      dwElementType = (*iter)->OnEvent();
  //                  }
  //                  else
  //                  {
  //                      qwTick = SGDP::SDTimeSecs();
  //                      dwElementType = (*iter)->OnEvent();

  //                      qwCurTick = SDTimeSecs();
  //                      UINT64 qwTickTime = GetMsFromLast(qwTick);
  //                      this->m_poMeter->AddSamplePoint(dwElementType, qwTickTime);
  //                  }
  //                  dwRet++;
  //                  // 
		//		    (*iter)->m_dwNextTick = qwCurTick + dwInterval;
		//	    }
		//	    else if(EXECUTE_STRICT == m_eExecuteType)
		//	    {
		//		    // 严格执行
		//		    while(qwElementTick < qwCurTick)
		//		    {
  //                      if(EVENT_HALTING == (*iter)->m_eEventStatus)
  //                      {
  //                          break;
  //                      }
		//			    if(FALSE == this->m_bMeterStatus)
		//			    {
		//				    dwElementType = (*iter)->OnEvent();
		//			    }
		//			    else
		//			    {
		//				    qwTick = SGDP::SDTimeSecs();
		//				    dwElementType = (*iter)->OnEvent();
  //                          qwCurTick = SDTimeSecs();

  //                          UINT64 qwTickTime = GetMsFromLast(qwTick);
		//				    this->m_poMeter->AddSamplePoint(dwElementType, qwTickTime);
		//			    }
		//			    dwRet++;
		//			    qwElementTick += dwInterval;
		//		    }
		//		    (*iter)->m_dwNextTick = qwElementTick;
		//	    }
		//	    iter++;
		//    }
		//    else
		//    {
		//	    iter++;
		//    }
	 //   }
		//m_qwLastRunTime = SDTimeSecs();

	 //   return dwRet;
  //  }


    /**
     * @brief 处理定时器超时
     */
    template <typename element>
    UINT32 CWheelPump<element>::TrigerItemsForCurSlot()
    {
        INT32 dwElementType = 0;
        UINT64 qwCurTick = 0;
        std::list<CWheelItem*> &oCurSlot = m_aoSlotLevels[0].GetCurSlot();
        UINT32 dwTimeOutCount = 0;

        while(FALSE == oCurSlot.empty())
        {
            CWheelItem* poWheelItem = oCurSlot.front();
            if(EVENT_HALTING == poWheelItem->m_poElementHandler->m_eEventStatus)
            {
                m_aoSlotLevels[poWheelItem->m_byWheelLevel].DelItem(poWheelItem);
                this->m_poObjectPool->Free(dynamic_cast<element*>(poWheelItem->m_poElementHandler));
                m_oWheelItemPool.Free(poWheelItem);
                continue;
            }
            if(EXECUTE_LOOSE ==this->m_eExecuteType)
            {
                // 不严格执行（默认），会丢失激发事件，但执行时间不变
                if(FALSE != this->m_bMeterStatus)
                {
                    UINT64 qwTick = SGDP::SDTimeSecs();
                    dwElementType = poWheelItem->OnEvent();
                    qwCurTick = SDTimeSecs();

                    UINT64 qwTickTime = GetMsFromLast(qwTick);
                    this->m_poMeter->AddSamplePoint(dwElementType, qwTickTime);
                }
                else
                {
                    dwElementType = poWheelItem->OnEvent();
                    qwCurTick = SGDP::SDTimeSecs();
                }
                dwTimeOutCount++;
                // 设置下一次执行时间
                UINT64 qwElementTick = poWheelItem->m_qwLastExpTime;
                while(qwElementTick <= qwCurTick)
                {
                    qwElementTick += poWheelItem->m_poElementHandler->m_dwInterval;
                }
                poWheelItem->SetLastExpdTime(qwElementTick - poWheelItem->m_poElementHandler->m_dwInterval);
            }
            else if(EXECUTE_LAZY == this->m_eExecuteType)
            {
                // 懒惰执行，会丢失激发事件，下一次激发时间会变成当前激发时间加周期
                if(FALSE != this->m_bMeterStatus)
                {
                    UINT64 qwTick = SGDP::SDTimeSecs();
                    dwElementType = poWheelItem->OnEvent();
                    qwCurTick = SDTimeSecs();

                    UINT64 qwTickTime = GetMsFromLast(qwTick);
                    this->m_poMeter->AddSamplePoint(dwElementType, qwTickTime);
                }
                else
                {
                    dwElementType = poWheelItem->OnEvent();
                    qwCurTick = SGDP::SDTimeSecs();
                }
                dwTimeOutCount++;
                // 设置下一次执行时间
                poWheelItem->SetLastExpdTime(qwCurTick);
            }
            else if(EXECUTE_STRICT == this->m_eExecuteType)
            {
                // 严格执行
                UINT64 qwElementTick = poWheelItem->m_qwLastExpTime;
                qwCurTick = SGDP::SDTimeSecs();
                while(qwElementTick < qwCurTick)
                {
                    if(EVENT_HALTING == poWheelItem->m_poElementHandler->m_eEventStatus)
                    {
                        break;
                    }
                    if(FALSE != this->m_bMeterStatus)
                    {
                        UINT64 qwTick = SGDP::SDTimeSecs();
                        dwElementType = poWheelItem->OnEvent();
                        qwCurTick = SDTimeSecs();

                        UINT64 qwTickTime = GetMsFromLast(qwTick);;
                        this->m_poMeter->AddSamplePoint(dwElementType, qwTickTime);
                    }
                    else
                    {
                        dwElementType = poWheelItem->OnEvent();
                        qwCurTick = SGDP::SDTimeSecs();
                    }
                    dwTimeOutCount++;
                    //
                    qwElementTick += poWheelItem->m_poElementHandler->m_dwInterval;
                }
                poWheelItem->SetLastExpdTime(qwCurTick);
            }

            // 判断是否在OnEvent里面删除了该元素
            if (oCurSlot.empty() || (oCurSlot.front() != poWheelItem))
            {
                continue;
            }
            // 循环定时器，需要再次插入到合适位置
            oCurSlot.pop_front();
            AddItemForNextTrigger(poWheelItem);
            // dwTimeOutCount++;
        }
        if(FALSE == m_aoSlotLevels[0].NextSlot(TRUE))
        {
            ReloadSlotLevel();
        }
        return dwTimeOutCount;
    }

}

