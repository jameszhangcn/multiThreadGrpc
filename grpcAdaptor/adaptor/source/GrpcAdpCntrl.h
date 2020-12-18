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
* File Name: TrilliumLibCntrl.h
*
* Subsystem: libCUUP
*
* Description: This file defines the RPC client for libCUUP
*
* Revision History:
* Date        Name         Description
* ----        ----         -----------
* 10/19/2020  Fisher       Initial Release
*
**********************************************************************/
#pragma once

#include "CucpService.h"
#include "GrpcAdp.h"
#include "GrpcAdpRpcServerThread.h"
#include "GrpcAdpRpcClientThread.h"

using namespace std;
class EbmGrpcClient{
    public:
        string getCpID() const {return cpID;}
        string getUpID() const {return upID;}
        long getCpInstID() const {return cpInst;}

        EbmGrpcClient(string m_cpID, long m_cpInst, string m_upID):cpID(m_cpID), cpInst(m_cpInst), upID(m_upID) 
        {}

        bool operator == (const EbmGrpcClient &client) const 
        {
            return (cpID == client.cpID) && (upID == client.upID) && (cpInst == client.cpInst); 
        }

    private:
        string cpID;
        string upID;
        long cpInst;
};

namespace std
{
    template<>
    struct hash<EbmGrpcClient>
    {
        size_t operator() (const EbmGrpcClient & s) const noexcept
        {
            return hash<decltype(s.getCpID())>() (s.getCpID()) +
                   hash<decltype(s.getCpInstID())>() (s.getCpInstID()) +
                   hash<decltype(s.getUpID())>() (s.getUpID());
        }
    };

}

namespace mav
{
        
    class GrpcWorker{
        public:
            GrpcWorker(unsigned char aThreadInstId);
            ~GrpcWorker()
                {
                    //delete context
                }
        private:
            unsigned char mThreadInstId;
    };

    class GrpcAdpCntrl
    {
    public:
        static GrpcAdpCntrl* getInstance();

        inline GrpcAdpRpcServerThread& getGrpcServerThread()
        {
            return mRpcServerThread;
        }

        inline GrpcAdpRpcClientThread& getGrpcClientThread()
        {
            return *mRpcClientThread[0];
        }

        inline GrpcAdpRpcClientThread& getGrpcClientThread_M(EbmGrpcClient &grpcClient)
        {
            return *mRpcClientThread[mEbmClientMap[grpcClient]];
        }

        bool init();
        bool deinit();
        void join_threads();
        void client_lock(int clientId);
        void client_unlock(int clientId);
        int create_client(char* cuCpID, long cuCpInst, char* cuUpID);
        int destroy_client(string cuCpID, long cuCpInst, string cuUpID);
        int get_free_client_idx(GrpcAdpRpcClientThread* threadID);
        int release_client_idx(int idx);


    private:
        static GrpcAdpCntrl* mInstance;
        static int mInstId;
        int mThreadNum;
        pthread_mutex_t mClientMutex;
        std::vector<GrpcWorker *> mWorkerInstance;
        unsigned char mThreadInstanceIdGen;
        GrpcAdpRpcServerThread mRpcServerThread;
        GrpcAdpRpcClientThread* mRpcClientThread[MAX_GRPC_CLIENT_NUM];

        //client multi instance
        std::unordered_map<EbmGrpcClient, int > mEbmClientMap;

        GrpcAdpCntrl();
        ~GrpcAdpCntrl() {}

        int createWorkerThreads();
        void joinAllThreads();

        int initGrpcServerConfig();
        int initGrpcClientConfig();
        int startGrpcServerThread(unsigned char& aThreadInstanceId);
        int startGrpcClientThread(unsigned char& aThreadInstanceId);
        int createGrpcClient(string cuCpID, long cuCpInst, string cuUpID);
        int destroyGrpcClient(string cuCpID, long cuCpInst, string cuUpID);
    };
} // namespace mav
