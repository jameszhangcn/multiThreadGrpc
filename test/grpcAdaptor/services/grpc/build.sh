rm -rf include
mkdir include
GRPC_PATH=../../../../grpc
export PATH=$PATH:$GRPC_PATH/dist/bin/:$GRPC_PATH/dist/include:$GRPC_PATH/third_party/protobuf/src/
echo $PATH
protoc --grpc_out=./include --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` mav_common.proto
protoc --grpc_out=./include --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` common.proto
protoc --grpc_out=./include --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` mhdr.proto
protoc --grpc_out=./include --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` structure.proto
protoc --grpc_out=./include --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` srvEbm.proto
protoc --grpc_out=./include --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` srvCp.proto
protoc --cpp_out=./include mav_common.proto
protoc --cpp_out=./include common.proto
protoc --cpp_out=./include mhdr.proto
protoc --cpp_out=./include structure.proto
protoc --cpp_out=./include srvEbm.proto
protoc --cpp_out=./include srvCp.proto
cd objs; make
