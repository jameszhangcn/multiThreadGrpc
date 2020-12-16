#include <unistd.h>
#include <netinet/sctp.h>
#include <thread>
#include <grpcpp/grpcpp.h>
#include "../include/mav_sctp.grpc.pb.h"
#include "SvcLog.h"
#include "ServiceImpl.h"

using namespace MavSctpAdaptor;

enum RpcType {
    // GW -> App
    SCTP_ASSOC_UP_IND,
    SCTP_ASSOC_DOWN_IND,
    SCTP_USER_MSG_RX_IND,
    SCTP_COMMON_MSG_RX_IND,

    // SctpIwf -> App
    SCTP_SERVER_SETUP_RSP,
    SCTP_SERVER_DELETE_RSP,
    SCTP_CLIENT_SETUP_RSP,
    SCTP_CLIENT_DELETE_RSP,
};

DEF_RPC_CLIENT_OBJ(SctpIwfSctpServerSetupReqClient, SCTP_IWF, SctpServerSetupReq, SctpServerSetupReqInfo);
DEF_RPC_CLIENT_OBJ(SctpIwfSctpServerDeleteReqClient, SCTP_IWF, SctpServerDeleteReq, SctpServerDeleteReqInfo);
DEF_RPC_CLIENT_OBJ(SctpIwfSctpClientSetupReqClient, SCTP_IWF, SctpClientSetupReq, SctpClientSetupReqInfo);
DEF_RPC_CLIENT_OBJ(SctpIwfSctpClientDeleteReqClient, SCTP_IWF, SctpClientDeleteReq, SctpClientDeleteReqInfo);
DEF_RPC_CLIENT_OBJ(SctpIwfSctpMsgTxReqClient, SCTP_IWF, SctpMsgTxReq, SctpMsgTxReqInfo);

DEF_RPC_SERVER_OBJ(SctpAssocUpIndCallData, SCTP_ASSOC_UP_IND, SctpAssocUpInd, App, SctpAssocUpIndInfo);
DEF_RPC_SERVER_OBJ(SctpAssocDownIndCallData, SCTP_ASSOC_DOWN_IND, SctpAssocDownInd, App, SctpAssocDownIndInfo);
DEF_RPC_SERVER_OBJ(SctpUserMsgRxIndCallData, SCTP_USER_MSG_RX_IND, SctpUserMsgRxInd, App, SctpUserMsgRxIndInfo);
DEF_RPC_SERVER_OBJ(SctpCommonMsgRxIndCallData, SCTP_COMMON_MSG_RX_IND, SctpCommonMsgRxInd, App, SctpCommonMsgRxIndInfo);

DEF_RPC_SERVER_OBJ(SctpServerSetupRspCallData, SCTP_SERVER_SETUP_RSP, SctpServerSetupRsp, App, SctpRsp);
DEF_RPC_SERVER_OBJ(SctpServerDeleteRspCallData, SCTP_SERVER_DELETE_RSP, SctpServerDeleteRsp, App, SctpRsp);
DEF_RPC_SERVER_OBJ(SctpClientSetupRspCallData, SCTP_CLIENT_SETUP_RSP, SctpClientSetupRsp, App, SctpRsp);
DEF_RPC_SERVER_OBJ(SctpClientDeleteRspCallData, SCTP_CLIENT_DELETE_RSP, SctpClientDeleteRsp, App, SctpRsp);

class AppServer : public ServiceServerImpl
{
public:
    ~AppServer() {};

    virtual void OnInit()
    {
        AddServiceHandler(SCTP_ASSOC_UP_IND);
        AddServiceHandler(SCTP_ASSOC_DOWN_IND);
        AddServiceHandler(SCTP_USER_MSG_RX_IND);
        AddServiceHandler(SCTP_COMMON_MSG_RX_IND);
        AddServiceHandler(SCTP_SERVER_SETUP_RSP);
        AddServiceHandler(SCTP_SERVER_DELETE_RSP);
        AddServiceHandler(SCTP_CLIENT_SETUP_RSP);
        AddServiceHandler(SCTP_CLIENT_DELETE_RSP);
    }

    virtual grpc::Service* getService()
    {
        return &mService;
    }

    virtual ServiceServerCallData* allocCallData(int aServiceType)
    {
        switch(aServiceType)
        {
        case SCTP_ASSOC_UP_IND:
            return new SctpAssocUpIndCallData();
        case SCTP_ASSOC_DOWN_IND:
            return new SctpAssocDownIndCallData();
        case SCTP_USER_MSG_RX_IND:
            return new SctpUserMsgRxIndCallData();
        case SCTP_COMMON_MSG_RX_IND:
            return new SctpCommonMsgRxIndCallData();
        case SCTP_SERVER_SETUP_RSP:
            return new SctpServerSetupRspCallData();
        case SCTP_SERVER_DELETE_RSP:
            return new SctpServerDeleteRspCallData();
        case SCTP_CLIENT_SETUP_RSP:
            return new SctpClientSetupRspCallData();
        case SCTP_CLIENT_DELETE_RSP:
            return new SctpClientDeleteRspCallData();
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
        case SCTP_ASSOC_UP_IND:
            OnSctpAssocUpInd(dynamic_cast<SctpAssocUpIndInfo&>(*aCD.getMessage()));
            break;
        case SCTP_ASSOC_DOWN_IND:
            OnSctpAssocDownInd(dynamic_cast<SctpAssocDownIndInfo&>(*aCD.getMessage()));
            break;
        case SCTP_USER_MSG_RX_IND:
            OnSctpUserMsgRxInd(dynamic_cast<SctpUserMsgRxIndInfo&>(*aCD.getMessage()));
            break;
        case SCTP_COMMON_MSG_RX_IND:
            OnSctpCommonMsgRxInd(dynamic_cast<SctpCommonMsgRxIndInfo&>(*aCD.getMessage()));
            break;
        case SCTP_SERVER_SETUP_RSP:
            OnSctpServerSetupRsp(dynamic_cast<SctpRsp&>(*aCD.getMessage()));
            break;
        case SCTP_SERVER_DELETE_RSP:
            OnSctpServerDeleteRsp(dynamic_cast<SctpRsp&>(*aCD.getMessage()));
            break;
        case SCTP_CLIENT_SETUP_RSP:
            OnSctpClientSetupRsp(dynamic_cast<SctpRsp&>(*aCD.getMessage()));
            break;
        case SCTP_CLIENT_DELETE_RSP:
            OnSctpClientDeleteRsp(dynamic_cast<SctpRsp&>(*aCD.getMessage()));
            break;
        default:
            ccWarning("Unknown service type %d", aCD.getRpcType());
            break;
        }
    }

private:
    virtual void OnSctpAssocUpInd(SctpAssocUpIndInfo& aInd)
    {
        ccLog("SctpAssocUpInd");
        sleep(2);
    }
    virtual void OnSctpAssocDownInd(SctpAssocDownIndInfo& aInd)
    {
        ccLog("SctpAssocDownInd");
        sleep(2);
    }
    virtual void OnSctpUserMsgRxInd(SctpUserMsgRxIndInfo& aInd)
    {
        ccLog("SctpUserMsgRxInd");
        sleep(2);
    }
    virtual void OnSctpCommonMsgRxInd(SctpCommonMsgRxIndInfo& aInd)
    {
        ccLog("SctpCommonMsgRxInd");
        sleep(2);
    }
    virtual void OnSctpServerSetupRsp(SctpRsp& aRsp)
    {
        ccLog("SctpServerSetupRsp");
        sleep(2);
    }
    virtual void OnSctpServerDeleteRsp(SctpRsp& aRsp)
    {
        ccLog("SctpServerDeleteRsp");
        sleep(2);
    }
    virtual void OnSctpClientSetupRsp(SctpRsp& aRsp)
    {
        ccLog("SctpClientSetupRsp");
        sleep(2);
    }
    virtual void OnSctpClientDeleteRsp(SctpRsp& aRsp)
    {
        ccLog("SctpClientDeleteRsp");
        sleep(2);
    }

    App::AsyncService mService;
};

int main(int argc, char** argv)
{
    // Start GRPC Server
    AppServer server;
    server.Init("0.0.0.0:50052");
    server.StartThread();

    // GRPC Client
    ServiceClientImpl client;
    client.Connect("localhost:50050");
    client.StartThread();

    SctpIwfSctpMsgTxReqClient iwfSctpMsgTxReq(client.getChannel(), client.getCompletionQueue());
    for(int i = 0; i < 100; i++)
    {
        SctpMsgTxReqInfo sctp;
        iwfSctpMsgTxReq.Send(sctp);
        ccLog("Send SctpMsg %d to IWF", i);
        sleep(2);
    }

    server.ThreadJoin();
    client.ThreadJoin();
    ccLog("Press control-c to quit");
    return 0;
}
