#include <unistd.h>
#include <memory>
#include <thread>
#include <netinet/sctp.h>
#include <grpcpp/grpcpp.h>
#include "../include/mav_sctp.grpc.pb.h"
#include "ServiceImpl.h"
#include "SvcLog.h"

using namespace MavSctpAdaptor;

enum RpcType {
    SCTP_ASSOC_UP_IND,
    SCTP_ASSOC_DOWN_IND,
    SCTP_MSG_RX_IND,
};

DEF_RPC_CLIENT_OBJ(AppSctpAssocUpIndClient, App, SctpAssocUpInd, SctpAssocUpIndInfo);
DEF_RPC_CLIENT_OBJ(AppSctpAssocDownIndClient, App, SctpAssocDownInd, SctpAssocDownIndInfo);
DEF_RPC_CLIENT_OBJ(AppSctpUserMsgRxIndClient, App, SctpUserMsgRxInd, SctpUserMsgRxIndInfo);
DEF_RPC_CLIENT_OBJ(AppSctpCommonMsgRxIndClient, App, SctpCommonMsgRxInd, SctpCommonMsgRxIndInfo);

DEF_RPC_SERVER_OBJ(SctpAssocUpIndCallData, SCTP_ASSOC_UP_IND, SctpAssocUpInd, GW, SctpAssocUpIndInfo);
DEF_RPC_SERVER_OBJ(SctpAssocDownIndCallData, SCTP_ASSOC_DOWN_IND, SctpAssocDownInd, GW, SctpAssocDownIndInfo);
DEF_RPC_SERVER_OBJ(SctpMsgRxIndCallData, SCTP_MSG_RX_IND, SctpMsgRxInd, GW, SctpMsg);

class GWServer : public ServiceServerImpl
{
public:
    ~GWServer() {};

    virtual void OnInit()
    {
        AddServiceHandler(SCTP_ASSOC_UP_IND);
        AddServiceHandler(SCTP_ASSOC_DOWN_IND);
        AddServiceHandler(SCTP_MSG_RX_IND);
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
        case SCTP_MSG_RX_IND:
            return new SctpMsgRxIndCallData();
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
        case SCTP_MSG_RX_IND:
            OnSctpMsgRxInd(dynamic_cast<SctpMsg&>(*aCD.getMessage()));
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
    virtual void OnSctpMsgRxInd(SctpMsg& aInd)
    {
        ccLog("SctpMsgRxInd: sctp_sndrcvinfo: %d|%d msg_len:%zu|%zu data:%s"
             , aInd.sctp_sndrcvinfo().sinfo_stream()
             , aInd.sctp_sndrcvinfo().sinfo_ssn()
             , aInd.data().length()
             , aInd.data().size()
             , aInd.data().c_str());
        sleep(2);
    }

    GW::AsyncService mService;
};

int main(int argc, char** argv)
{
    // Start GRPC Server
    GWServer server;
    server.Init("0.0.0.0:50051");
    server.StartThread();

    // GRPC Client
    ServiceClientImpl client;
    client.Connect("localhost:50052");
    client.StartThread();

    AppSctpUserMsgRxIndClient appSctpUserMsgRxInd(client.getChannel(), client.getCompletionQueue());
    for(int i = 0; i < 100; i++)
    {
        SctpUserMsgRxIndInfo sctp;
        appSctpUserMsgRxInd.Send(sctp);
        ccLog("Send SctpUserMsgRxIndInfo %d to App", i);
        sleep(2);
    }

    server.ThreadJoin();
    client.ThreadJoin();
    ccLog("Press control-c to quit");
    return 0;
}
