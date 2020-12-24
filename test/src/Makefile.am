AUTOMAKE_OPTIONS=foreign

INCLUDES=-I../include/

bin_PROGRAMS=testgrpc
GRPC_PATH=/root/code/grpc/grpc
testgrpc_SOURCES=testgrpc.c

testgrpc_LDFLAGS = \
		   -L. \
		   -L/usr/local/lib
testgrpc_CXXFLAGS = -std=c++11
testgrpc_LDADD=-L$(GRPC_PATH)/libs/opt \
	       -L/usr/local/lib \
	       -lpthread \
	       -lprotobuf \
	       -lgrpc \
	       -lgpr \
	       -lgrpc++ \
	       -lgrpc++_reflection \
	       -lCUUP
