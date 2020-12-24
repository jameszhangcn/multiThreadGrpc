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
* File Name: ServiceCommonType.cpp
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
#include "ServiceCommonType.h"
#if E1_SPLIT_ENABLED_CU_CP
//#include "ngap_codec/include/ngap_msg_types.h"
#endif
#include "SvcLog.h"

namespace mav
{
    /*
    gnb::sctp_cntrl_ref_id& ServiceCommonType::fill(
            gnb::sctp_cntrl_ref_id& aTrillium,
            const SctpCntrlRefId& aMav)
    {
        aTrillium.sctp_user = get(aMav.sctp_user());
        aTrillium.ref_id = aMav.ref_id();
        return aTrillium;
    }

    gnb::sctp_user ServiceCommonType::get(SctpUser aUser)
    {
        switch(aUser)
        {
        case X2AP:
            return gnb::sctp_user::X2AP;
        case NGAP:
            return gnb::sctp_user::NGAP;
        case XNAP:
            return gnb::sctp_user::XNAP;
        case F1AP:
            return gnb::sctp_user::F1AP;
        case E1AP:
            return gnb::sctp_user::E1AP;
        default:
            return gnb::sctp_user::UNDEFINED;
        }
    }

    gnb::sctp_user ServiceCommonType::getUser(const char* aUser)
    {
        if(0 == strcmp(aUser, "X2AP"))
            return gnb::sctp_user::X2AP;
        if(0 == strcmp(aUser, "NGAP"))
            return gnb::sctp_user::NGAP;
        if(0 == strcmp(aUser, "XNAP"))
            return gnb::sctp_user::XNAP;
        if(0 == strcmp(aUser, "F1AP"))
            return gnb::sctp_user::F1AP;
        if(0 == strcmp(aUser, "E1AP"))
            return gnb::sctp_user::E1AP;

        return gnb::sctp_user::UNDEFINED;
    }

    gnb::sctp_ppid ServiceCommonType::getPpid(gnb::sctp_user aUser)
    {
        switch(aUser)
        {
        case gnb::sctp_user::X2AP:
            return gnb::sctp_ppid::X2AP_PPID;
        case gnb::sctp_user::NGAP:
            return gnb::sctp_ppid::NGAP_PPID;
        case gnb::sctp_user::XNAP:
            return gnb::sctp_ppid::XNAP_PPID;
        case gnb::sctp_user::F1AP:
            return gnb::sctp_ppid::F1AP_PPID;
        case gnb::sctp_user::E1AP:
            return gnb::sctp_ppid::E1AP_PPID;
        default:
            ccError("Unknown %d", aUser);
            return gnb::sctp_ppid::E1AP_PPID;
        }
    }

    SctpCntrlRefId& ServiceCommonType::fill(SctpCntrlRefId& aMav, const gnb::sctp_cntrl_ref_id& aTrillium)
    {
        aMav.set_sctp_user(get(aTrillium.sctp_user));
        aMav.set_ref_id(aTrillium.ref_id);
        return aMav;
    }

    SctpEndpoint& ServiceCommonType::fill(SctpEndpoint& aMav, const gnb::sctp_endpoint& aTrillium)
    {
        for(uint8_t i = 0; i < gnb::MAX_SCTP_ENDPOINT_ADDRESSES; ++i)
        {
            fill(*(aMav.mutable_ip_addr(i)), aTrillium.ip_addr[i]);
        }

        aMav.set_port_num(aTrillium.port_num);
        return aMav;
    }

    IpAddress& ServiceCommonType::fill(IpAddress& aMav, const gnb::ip_address& aTrillium)
    {
        aMav.set_type(get(aTrillium.type));

        if(aTrillium.type == gnb::ip_address_type::IP_ADDR_TYPE_IPV6)
        {
            aMav.set_addr(aTrillium.ipv6_addr.addr);
        }
        else    // gnb::ip_address_type::IP_ADDR_TYPE_IPV4
        {
            aMav.set_addr(aTrillium.ipv4_addr.addr);
        }
        return aMav;
    }

    SctpMsg_SctpSndrcvinfo& ServiceCommonType::fill(SctpMsg_SctpSndrcvinfo& aMav, const struct sctp_sndrcvinfo& aTrillium)
    {
        aMav.set_sinfo_stream(aTrillium.sinfo_stream);
        aMav.set_sinfo_ssn(aTrillium.sinfo_ssn);
        aMav.set_sinfo_flags(aTrillium.sinfo_flags);
        aMav.set_sinfo_ppid(aTrillium.sinfo_ppid);
        aMav.set_sinfo_context(aTrillium.sinfo_context);
        aMav.set_sinfo_timetolive(aTrillium.sinfo_timetolive);
        aMav.set_sinfo_tsn(aTrillium.sinfo_tsn);
        aMav.set_sinfo_cumtsn(aTrillium.sinfo_cumtsn);
        aMav.set_sinfo_assoc_id(aTrillium.sinfo_assoc_id);
        return aMav;
    }

    SctpServerSetupReqInfo& ServiceCommonType::fill(SctpServerSetupReqInfo& aMav, const gnb::sctp_server_setup_req_info& aTrillium)
    {
        fill(*(aMav.mutable_cntrl_ref_id()), aTrillium.cntrl_ref_id);
        aMav.set_ppid(get(aTrillium.ppid));
        fill(*(aMav.mutable_local_endpoint()), aTrillium.local_endpoint);
        aMav.set_msg_identification_required(aTrillium.msg_identification_required);
        return aMav;
    }

    SctpClientSetupReqInfo& ServiceCommonType::fill(SctpClientSetupReqInfo& aMav, const gnb::sctp_client_setup_req_info& aTrillium)
    {
        fill(*(aMav.mutable_cntrl_ref_id()), aTrillium.cntrl_ref_id);
        aMav.set_ppid(get(aTrillium.ppid));
        fill(*(aMav.mutable_local_endpoint()), aTrillium.local_endpoint);
        fill(*(aMav.mutable_remote_endpoint()), aTrillium.remote_endpoint);
        aMav.set_msg_identification_required(aTrillium.msg_identification_required);
        return aMav;
    }

    SctpMsgTxReqInfo& ServiceCommonType::fill(SctpMsgTxReqInfo& aMav, const gnb::sctp_msg_tx_req_info& aTrillium)
    {
        fill(*(aMav.mutable_cntrl_ref_id()), aTrillium.cntrl_ref_id);
        aMav.set_msg(aTrillium.msg, aTrillium.len);
        aMav.set_flow_id(aTrillium.flow_id);
        return aMav;
    }

    SctpUser ServiceCommonType::get(gnb::sctp_user aUser)
    {
        switch(aUser)
        {
        case gnb::sctp_user::X2AP:
            return X2AP;
        case gnb::sctp_user::NGAP:
            return NGAP;
        case gnb::sctp_user::XNAP:
            return XNAP;
        case gnb::sctp_user::F1AP:
            return F1AP;
        case gnb::sctp_user::E1AP:
            return E1AP;
        case gnb::sctp_user::UNDEFINED:
        default:
            return UNDEFINED;
        }
    }

    SctpPpid ServiceCommonType::get(gnb::sctp_ppid aPpid)
    {
        switch(aPpid)
        {
        case gnb::sctp_ppid::X2AP_PPID:
            return X2AP_PPID;
        case gnb::sctp_ppid::NGAP_PPID:
            return NGAP_PPID;
        case gnb::sctp_ppid::XNAP_PPID:
            return XNAP_PPID;
        case gnb::sctp_ppid::F1AP_PPID:
            return F1AP_PPID;
        case gnb::sctp_ppid::E1AP_PPID:
            return E1AP_PPID;
        default:
            return NONE_PPID;
        }
    }

    IpAddressType ServiceCommonType::get(gnb::ip_address_type aType)
    {
        switch(aType)
        {
        case gnb::ip_address_type::IP_ADDR_TYPE_IPV4:
            return IP_ADDR_TYPE_IPV4;
        case gnb::ip_address_type::IP_ADDR_TYPE_IPV6:
            return IP_ADDR_TYPE_IPV6;
        default:
            return IP_ADDR_TYPE_IPV4;
        }
    }
    */
}
