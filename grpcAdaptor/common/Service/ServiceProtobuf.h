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
* File Name: ServiceProtobuf.h
*
* Subsystem: gRPC Service
*
* Description: This file defines the service layer
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 11/22/2019  Fisher       Initial Release
*
**********************************************************************/
#pragma once

#include <grpcpp/grpcpp.h>
#include "mav_common.grpc.pb.h"
#include "common.grpc.pb.h"
#include "structure.grpc.pb.h"
#include "srvCp.grpc.pb.h"
#include "srvEbm.grpc.pb.h"
using namespace MavCommonAdaptor;
using namespace common;
using namespace srvEbm;
using namespace srvCp;
using namespace structure;

