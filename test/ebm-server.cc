#include <iostream>
#include <memory>
#include <string>
 
#include <grpcpp/grpcpp.h>

#include "mhdr.grpc.pb.h"
#include "common.grpc.pb.h"
#include "structure.grpc.pb.h"
#include "srvEbm.grpc.pb.h"

 
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using srvEbm::SecurityConfigureReq;
using srvEbm::ReqReply;
using structure::SecurityConfigure;
 
// Logic and data behind the server's behavior.
class EbmServiceImpl final : public srvEbm::Ebm::Service {
  Status SecurityConfigure(ServerContext* context, const SecurityConfigureReq* request,
                  ReqReply* reply) override {
    std::string prefix("Hello ");
    //reply->set_message(prefix + request->name());
    static uint32_t seq = 0;
    printf("Recv SecurityConfigureReq algorithm_type %d \n", request->security_config().algorithm_type());
    srvEbm::ReqReply aMsg;

    ::mhdr::MsgHdr * hdr = aMsg.mutable_hdr();
    hdr->mutable_torid()->set_sn("CUCP");
    hdr->mutable_fromrid()->set_sn("EBM");
    hdr->mutable_fromloc()->set_address("0.0.0.0");
    hdr->mutable_fromloc()->set_port(9027);

    hdr->mutable_toloc()->set_address("0.0.0.0");
    hdr->mutable_toloc()->set_port(9021);

    hdr->set_transid(seq++);

    printf("Send SecurityConfigureRsp %d \n", seq);

    return Status::OK;
  }
};
 
void RunServer() {
  std::string server_address("0.0.0.0:9027");
  EbmServiceImpl service;
 
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
 
  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}
 
int main(int argc, char** argv) {
  RunServer();
 
  return 0;
}