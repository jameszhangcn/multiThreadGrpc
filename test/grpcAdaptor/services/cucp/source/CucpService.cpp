/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2004-2019                                           */
/* Mavenir Systems, Inc.                                             */
/* Richardson, TX, USA                                               */
/*                                                                   */
/* ALL RIGHTS RESERVED                                               */
/*                                                                   */
/* Permission is hereby granted to licensees of Mavenir Systems,     */
/* Inc. products to use or abstract this computer program for the    */
/* sole purpose of implementing a product based on                   */
/* Mavenir Systems, Inc. products.  No other rights to reproduce,    */
/* use, or disseminate this computer program, whether in part or in  */
/* whole, are granted.                                               */
/*                                                                   */
/* Mavenir Systems, Inc. makes no representation or warranties       */
/* with respect to the performance of this computer program, and     */
/* specifically disclaims any responsibility for any damages,        */
/* special or consequential, connected with the use of this program. */
/*                                                                   */
/*********************************************************************/

/**********************************************************************
*
* File Name: CucpService.cpp
*
* Subsystem: Cucp
*
* Description: This file defines the service layer for Iwf
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 11/04/2020  JianZhang       Initial Release
*
**********************************************************************/
#include "SvcLog.h"
#include "CucpService.h"

CucpServer::~CucpServer()
{
}

void CucpServer::OnInit()
{
    ccLog("CucpServer:: OnInit");
    AddServiceHandler(SEC_CFG_CFM);
}

grpc::Service* CucpServer::getService()
{
    return &mService;
}

ServiceServerCallData* CucpServer::allocCallData(int aServiceType)
{
    ccLog("CucpServer::allocCallData %d ", aServiceType);
    switch(aServiceType)
    {
    case SEC_CFG_CFM:
        ccLog("CucpServer::EbmSecCfgCfmCallData  ");
        return new EbmSecCfgCfmCallData();
    default:
        ccLog("Unknown service type %d", aServiceType);
        return nullptr;
    }
}

void CucpServer::deallocCallData(ServiceServerCallData* aCD)
{
    delete aCD;
}

void CucpServer::OnMessage(ServiceServerCallData& aCD)
{
    switch(aCD.getRpcType())
    {
        case SEC_CFG_CFM:
            OnSecCfgCfm(dynamic_cast<SecurityConfigureRsp&>(*aCD.getMessage()));
            break;
        
        default:
            ccWarning("Unknown service type %d", aCD.getRpcType());
            break;
    }
}

void CucpServer::OnSecCfgCfm(SecurityConfigureRsp& aInd)
{
    ccLog("msg received OnSecCfgCfm");
}

