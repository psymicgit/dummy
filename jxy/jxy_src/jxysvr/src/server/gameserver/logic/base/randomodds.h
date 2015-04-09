///<------------------------------------------------------------------------------
//< @file:   randomodds.h
//< @author: yewenhui
//< @date:   2014年6月16日 15点40分
//< @brief:  随机概率抽取
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------
#include <map>
#include "common/server/utility.h"

using namespace std;

template<typename OddType>
class RandomOdds
{
    typedef typename std::map<unsigned int, OddType> MapOddWeight;
    typedef typename MapOddWeight::iterator MapOddWeightItor;

public:
    RandomOdds() : m_unTotalWeight(0) {}
    ~RandomOdds() {}

    void		PushOdds(OddType odd, unsigned int unOddWeight) {
        m_unTotalWeight += unOddWeight;
        m_mpOddWeight.insert(make_pair(m_unTotalWeight, odd));

    }

    OddType*	GetHitOdds() {
		if (0 == m_unTotalWeight) {
			return NULL;
		}

        unsigned int unRandomVal = (unsigned int)Random(0, (INT32)m_unTotalWeight);
        for (MapOddWeightItor itor = m_mpOddWeight.begin(); itor != m_mpOddWeight.end()
            ; ++itor)
        {
            if (itor->first >= unRandomVal) {
                return &itor->second;
            }
        }
        return NULL;
    }

private:
    MapOddWeight m_mpOddWeight;
    unsigned int m_unTotalWeight;

};
