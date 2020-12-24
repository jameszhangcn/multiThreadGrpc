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
* File Name: ServiceCommonType.h
*
* Subsystem: gRPC Service
*
* Description: This file defines the service layer
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 11/04/2019  Fisher       Initial Release
*
**********************************************************************/
#pragma once

/*
#include "mav_sctp.pb.h"
#include "gnb_common_types.h"
#include "sctp_cntrl_msg_types.h"
#include "sctp_tx_msg_types.h"

using namespace MavSctpAdaptor;
*/

namespace mav
{
    class ServiceCommonType
    {
    public:
        /*
        // gRPC -> Trillium
        static gnb::sctp_cntrl_ref_id& fill(gnb::sctp_cntrl_ref_id& aTrillium, const SctpCntrlRefId& aMav);

        static gnb::sctp_user get(SctpUser aUser);
        static gnb::sctp_user getUser(const char* aUser);
        static gnb::sctp_ppid getPpid(gnb::sctp_user aUser);

        // Trillium -> gRPC
        static SctpCntrlRefId& fill(SctpCntrlRefId& aMav, const gnb::sctp_cntrl_ref_id& aTrillium);
        static SctpEndpoint& fill(SctpEndpoint& aMav, const gnb::sctp_endpoint& aTrillium);
        static IpAddress& fill(IpAddress& aMav, const gnb::ip_address& aTrillium);
        static SctpMsg_SctpSndrcvinfo& fill(SctpMsg_SctpSndrcvinfo& aMav, const struct sctp_sndrcvinfo& aTrillium);

        static SctpServerSetupReqInfo& fill(SctpServerSetupReqInfo& aMav, const gnb::sctp_server_setup_req_info& aTrillium);
        static SctpClientSetupReqInfo& fill(SctpClientSetupReqInfo& aMav, const gnb::sctp_client_setup_req_info& aTrillium);
        static SctpMsgTxReqInfo& fill(SctpMsgTxReqInfo& aMav, const gnb::sctp_msg_tx_req_info& aTrillium);

        static SctpUser get(gnb::sctp_user aUser);
        static SctpPpid get(gnb::sctp_ppid aPpid);
        static IpAddressType get(gnb::ip_address_type aType);
        */
    };
}
