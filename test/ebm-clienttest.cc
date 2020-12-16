#include <iostream>
#include <memory>
#include <string>
 
#include <grpcpp/grpcpp.h>
 
#include "mhdr.grpc.pb.h"
#include "structure.grpc.pb.h"
#include "mav_cp_ebm.grpc.pb.h"
 
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using MavEbmAdaptor::SecurityConfigureRsp;
using MavEbmAdaptor::Response;
using MavEbmAdaptor::CpUpHdr;
using MavEbmAdaptor::CuCp;
 
class CuCpClient {
 public:
  CuCpClient(std::shared_ptr<Channel> channel)
      : stub_(CuCp::NewStub(channel)) {}
 
  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string SecurityConfigureCfm() {
    // Data we are sending to the server.
    SecurityConfigureRsp aMsg;
    static uint32_t seq = 0;
    printf("send SecurityConfigureCfm \n");

    ::mhdr::MsgHdr * hdr = aMsg.mutable_hdr();
    CpUpHdr* cpuphdr = aMsg.mutable_cp_up_hdr();
    
    hdr->mutable_fromrid()->set_sn("EBM");
    hdr->mutable_fromloc()->set_address("127.0.0.1");
    hdr->mutable_fromloc()->set_port(9020);

    hdr->mutable_torid()->set_sn("CUCP");
    hdr->mutable_toloc()->set_address("127.0.0.1");
    hdr->mutable_toloc()->set_port(9021);

    hdr->set_transid(seq++);

    cpuphdr->mutable_cu_cp_id()->set_cp_id(123);
    cpuphdr->mutable_cu_up_id()->set_up_id(456);

    aMsg.set_status(true);
    aMsg.set_reason("OK");
 
    // Container for the data we expect from the server.
    Response reply;
 
    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;
 
    // The actual RPC.
    Status status = stub_->SecurityConfigureCfm(&context, aMsg, &reply);
 
    // Act upon its status.
    if (status.ok()) {
      //print
      //
      printf("SecurityConfigureCfm reply OK\n");
      return "Cfm reply OK";
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }
 
 private:
  std::unique_ptr<CuCp::Stub> stub_;
};
 
int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50051). We indicate that the channel isn't authenticated
  // (use of InsecureChannelCredentials()).
  CuCpClient ebm(grpc::CreateChannel(
      "127.0.0.1:9020", grpc::InsecureChannelCredentials()));
  std::string user("world");
  std::string reply = ebm.SecurityConfigureCfm();
  std::cout << "SecurityConfigureCfm rsp received: " << reply << std::endl;
 
  return 0;
}