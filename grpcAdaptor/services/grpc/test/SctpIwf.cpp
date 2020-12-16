#include <unistd.h>
#include <netinet/sctp.h>
#include <thread>
#include <grpcpp/grpcpp.h>
#include "../include/mav_sctp.grpc.pb.h"
#include "SvcLog.h"
#include "ServiceImpl.h"

using namespace MavSctpAdaptor;

enum RpcType {
    SCTP_SERVER_SETUP_REQ,
    SCTP_SERVER_DELETE_REQ,
    SCTP_CLIENT_SETUP_REQ,
    SCTP_CLIENT_DELETE_REQ,
    SCTP_MSG_TX_REQ
};

DEF_RPC_CLIENT_OBJ(GwSctpAssocUpIndClient, GW, SctpAssocUpInd, SctpAssocUpIndInfo);
DEF_RPC_CLIENT_OBJ(GwSctpAssocDownIndClient, GW, SctpAssocDownInd, SctpAssocDownIndInfo);
DEF_RPC_CLIENT_OBJ(GwSctpMsgRxIndClient, GW, SctpMsgRxInd, SctpMsg);

DEF_RPC_CLIENT_OBJ(AppSctpServerSetupRspClient, App, SctpServerSetupRsp, SctpRsp);
DEF_RPC_CLIENT_OBJ(AppSctpServerDeleteRspClient, App, SctpServerDeleteRsp, SctpRsp);
DEF_RPC_CLIENT_OBJ(AppSctpClientSetupRspClient, App, SctpClientSetupRsp, SctpRsp);
DEF_RPC_CLIENT_OBJ(AppSctpClientDeleteRspClient, App, SctpClientDeleteRsp, SctpRsp);

DEF_RPC_SERVER_OBJ(SctpServerSetupReqCallData, SCTP_SERVER_SETUP_REQ, SctpServerSetupReq, SCTP_IWF, SctpServerSetupReqInfo);
DEF_RPC_SERVER_OBJ(SctpServerDeleteReqCallData, SCTP_SERVER_DELETE_REQ, SctpServerDeleteReq, SCTP_IWF, SctpServerDeleteReqInfo);
DEF_RPC_SERVER_OBJ(SctpClientSetupReqCallData, SCTP_CLIENT_SETUP_REQ, SctpClientSetupReq, SCTP_IWF, SctpClientSetupReqInfo);
DEF_RPC_SERVER_OBJ(SctpClientDeleteReqCallData, SCTP_CLIENT_DELETE_REQ, SctpClientDeleteReq, SCTP_IWF, SctpClientDeleteReqInfo);
DEF_RPC_SERVER_OBJ(SctpMsgTxReqCallData, SCTP_MSG_TX_REQ, SctpMsgTxReq, SCTP_IWF, SctpMsgTxReqInfo);

class SctpIwfServer : public ServiceServerImpl
{
public:
    ~SctpIwfServer() {};

    virtual void OnInit()
    {
        AddServiceHandler(SCTP_SERVER_SETUP_REQ);
        AddServiceHandler(SCTP_SERVER_DELETE_REQ);
        AddServiceHandler(SCTP_CLIENT_SETUP_REQ);
        AddServiceHandler(SCTP_CLIENT_DELETE_REQ);
        AddServiceHandler(SCTP_MSG_TX_REQ);
    }

    virtual grpc::Service* getService()
    {
        return &mService;
    }

    virtual ServiceServerCallData* allocCallData(int aServiceType)
    {
        switch(aServiceType)
        {
        case SCTP_SERVER_SETUP_REQ:
            return new SctpServerSetupReqCallData();
        case SCTP_SERVER_DELETE_REQ:
            return new SctpServerDeleteReqCallData();
        case SCTP_CLIENT_SETUP_REQ:
            return new SctpClientSetupReqCallData();
        case SCTP_CLIENT_DELETE_REQ:
            return new SctpClientDeleteReqCallData();
        case SCTP_MSG_TX_REQ:
            return new SctpMsgTxReqCallData();
        default:
            ccLog("Unknown service type %d", aServiceType);
            return nullptr;
        }
    };
    virtual void deallocCallData(ServiceServerCallData* aCD)
    {
        delete aCD;
    };
    virtual void OnMessage(ServiceServerCallData& aCD)
    {
        switch(aCD.getRpcType())
        {
        case SCTP_SERVER_SETUP_REQ:
            OnSctpServerSetupReq(dynamic_cast<SctpServerSetupReqInfo&>(*aCD.getMessage()));
            break;
        case SCTP_SERVER_DELETE_REQ:
            OnSctpServerDeleteReq(dynamic_cast<SctpServerDeleteReqInfo&>(*aCD.getMessage()));
            break;
        case SCTP_CLIENT_SETUP_REQ:
            OnSctpClientSetupReq(dynamic_cast<SctpClientSetupReqInfo&>(*aCD.getMessage()));
            break;
        case SCTP_CLIENT_DELETE_REQ:
            OnSctpClientDeleteReq(dynamic_cast<SctpClientDeleteReqInfo&>(*aCD.getMessage()));
            break;
        case SCTP_MSG_TX_REQ:
            OnSctpMsgTxReq(dynamic_cast<SctpMsg&>(*aCD.getMessage()));
            break;
        default:
            ccWarning("Unknown service type %d", aCD.getRpcType());
            break;
        }
    }

private:
    virtual void OnSctpServerSetupReq(SctpServerSetupReqInfo& aInd)
    {
        ccLog("SctpServerSetupReq");
        sleep(2);
    }
    virtual void OnSctpServerDeleteReq(SctpServerDeleteReqInfo& aInd)
    {
        ccLog("SctpServerDeleteReq");
        sleep(2);
    }
    virtual void OnSctpClientSetupReq(SctpClientSetupReqInfo& aInd)
    {
        ccLog("SctpClientSetupReq");
        sleep(2);
    }
    virtual void OnSctpClientDeleteReq(SctpClientDeleteReqInfo& aInd)
    {
        ccLog("SctpClientDeleteReq");
        sleep(2);
    }
    virtual void OnSctpMsgTxReq(SctpMsg& aInd)
    {
        ccLog("SctpMsgTxReq: sctp_sndrcvinfo: %d|%d msg_len:%zu|%zu data:%s"
             , aInd.sctp_sndrcvinfo().sinfo_stream()
             , aInd.sctp_sndrcvinfo().sinfo_ssn()
             , aInd.data().length()
             , aInd.data().size()
             , aInd.data().c_str());
        sleep(2);
    }

    SCTP_IWF::AsyncService mService;
};

int main(int argc, char** argv)
{
    // Start GRPC Server
    SctpIwfServer server;
    server.Init("0.0.0.0:50050");
    server.StartThread();

    // GRPC Client
    ServiceClientImpl client;
    client.Connect(0, std::string("localhost") + ":" + std::to_string(50051));
    client.Connect(1, "localhost:50052");
    client.StartThread();

    GwSctpMsgRxIndClient gwSctpMsgRxInd(client.getChannel(0), client.getCompletionQueue());
    AppSctpServerSetupRspClient appSctpServerSetupRsp(client.getChannel(1), client.getCompletionQueue());

    for(int i = 0; i < 100; i++)
    {
        SctpMsg sctp;
        gwSctpMsgRxInd.Send(sctp);
        ccLog("Send SctpMsg %d to GW", i);

        SctpRsp rsp;
        appSctpServerSetupRsp.Send(rsp);
        ccLog("Send SctpRsp %d to App", i);

        sleep(2);
    }

    server.ThreadJoin();
    client.ThreadJoin();
    ccLog("Press control-c to quit");
    return 0;
}
