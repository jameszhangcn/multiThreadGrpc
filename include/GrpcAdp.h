/*********************************************************************/
/*                                                                   */
/* Copyright (C) 2004-2020                                           */
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
* File Name: CUUPLib.h
*
* Subsystem: libCUUP
*
* Description: This file defines the interface of libCUUP
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 10/19/2020  Fisher       Initial Release
*
**********************************************************************/
#ifndef __GRPC_ADP_H
#define __GRPC_ADP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "GrpcAdpStru.h"

#define MAX_GRPC_CLIENT_NUM 20

#define CPU_SET(cpu, cpusetp)
#define CPU_CLR(cpu, cpusetp)
#define CPU_ISSET(cpu, cpusetp)
#define CPU_ZERO(cpusetp)

extern void mav_init();
extern void start_grpc_server();
extern void join_grpc_server();
extern int  send_sec_cfg_req(CuCpUpHdrG* cucp_up_hdr, SecurityConfigureG *secCfg);
extern int  send_sec_cfg_req_m(CuCpUpHdrG* cucp_up_hdr, SecurityConfigureG *secCfg);

extern int  start_grpc_client(uint32_t cp_id, uint32_t up_id, IpAddressG *up_ip, uint32_t dst_port);
extern void create_grpc_client(char* cp_id, pthread_t tid, char* up_id);

#ifdef __cplusplus
}
#endif

#endif  // __GRPC_ADP_H
