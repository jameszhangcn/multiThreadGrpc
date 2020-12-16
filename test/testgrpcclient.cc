//#include "funset.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "helloworld.grpc.pb.h"
 
// reference: grpc/examples/cpp/helloworld
namespace {
 
class GreeterClient {
public:
	GreeterClient(std::shared_ptr<grpc::Channel> channel) : stub_(helloworld::Greeter::NewStub(channel)) {}
 
	// Assembles the client's payload, sends it and presents the response back from the server.
  	std::string SayHello(const std::string& user) {
    		// Data we are sending to the server.
    		helloworld::HelloRequest request;
    		request.set_name(user);
 
    		// Container for the data we expect from the server.
    		helloworld::HelloReply reply;
 
    		// Context for the client. It could be used to convey extra information to the server and/or tweak certain RPC behaviors.
    		grpc::ClientContext context;
 
    		// The actual RPC.
    		grpc::Status status = stub_->SayHello(&context, request, &reply);
 
    		// Act upon its status.
    		if (status.ok()) {
      			return reply.message();
    		} else {
      			fprintf(stderr, "error code: %d, error message: %s\n", status.error_code(), status.error_message().c_str());
      			return "RPC failed";
   		}	
  	}	
 
private:
  	std::unique_ptr<helloworld::Greeter::Stub> stub_;
};
 
} // namespace
 
int test_grpc_client()
{
	fprintf(stdout, "client start\n");
  	// Instantiate the client. It requires a channel, out of which the actual RPCs are created.
	// This channel models a connection to an endpoint (in this case, localhost at port 50051).
	// We indicate that the channel isn't authenticated(use of InsecureChannelCredentials()).
  	GreeterClient greeter(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
  	std::string user("world");
  	std::string reply = greeter.SayHello(user);
  	fprintf(stdout, "Greeter received: %s\n", reply.c_str());
 
	return 0;
}

int main(int argc, char** argv){
	test_grpc_client();
	return 0;
}