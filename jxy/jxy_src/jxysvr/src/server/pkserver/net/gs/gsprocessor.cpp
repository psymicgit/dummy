///<------------------------------------------------------------------------------
//< @file:   gsprocessor.cpp
//< @author: hongkunan
//< @date:   2014Äê1ÔÂ2ÈÕ 21:9:52
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "gsprocessor.h"

CGSProcessor::CGSProcessor()
{
    Init();
}

CGSProcessor::~CGSProcessor()
{
}

CSDProtocol* CGSProcessor::GetProtocol()
{
    return CProtoGTGS::Instance();
}

BOOL CGSProcessor::Init()
{
    return TRUE;
}

