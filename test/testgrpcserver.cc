#include "funset.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "helloworld.grpc.pb.h"

namespace {
 
// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public helloworld::Greeter::Service {
  	grpc::Status SayHello(grpc::ServerContext* context, const helloworld::HelloRequest* request, helloworld::HelloReply* reply) override {
    		std::string prefix("Hello ");
    		reply->set_message(prefix + request->name());
    		return grpc::Status::OK;
  	}
};
 
void RunServer() {
  	std::string server_address("0.0.0.0:50051");
  	GreeterServiceImpl service;
 
  	grpc::ServerBuilder builder;
  	// Listen on the given address without any authentication mechanism.
  	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  	// Register "service" as the instance through which we'll communicate with clients.
	// In this case it corresponds to an *synchronous* service.
  	builder.RegisterService(&service);
  	// Finally assemble the server.
  	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  	fprintf(stdout, "Server listening on: %s\n", server_address.c_str());
 
  	// Wait for the server to shutdown. Note that some other thread must be
  	// responsible for shutting down the server for this call to ever return.
  	server->Wait();
}
 
} // namespace
 
int test_grpc_server()
{
	fprintf(stdout, "server start\n");
	RunServer();
 
	return 0;
}