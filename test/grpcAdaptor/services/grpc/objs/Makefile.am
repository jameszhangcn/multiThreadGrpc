
# Prefixes : specify 'install' directory
# bin_ lib_ include_ pkginclude_ noinst_ ...

# Primary : specify target type
# PROGRAMS LIBRARIES LTLIBRARIES HEADERS ...

# Variable : specify expansions
# SOURCES CFLAGS CPPFLAGS CXXFLAGS LIBADD LDADD LDFLAGS ...

VENDOR_PATH = $(top_srcdir)/../..

noinst_LIBRARIES=  libgrpcproto.a

libgrpcproto_a_CPPFLAGS = -fPIC -std=c++11 \
						-I.. -I../include \
						-I$(VENDOR_PATH)/grpc/dist/include \
						 -I$(VENDOR_PATH)/grpc/third_party/protobuf/src

libgrpcproto_a_SOURCES= 		\
	../include/mhdr.pb.cc \
	../include/mhdr.grpc.pb.cc \
	../include/mav_common.pb.cc \
	../include/mav_common.grpc.pb.cc \
	../include/common.pb.cc \
	../include/common.grpc.pb.cc \
	../include/structure.pb.cc \
	../include/structure.grpc.pb.cc \
	../include/srvEbm.pb.cc \
	../include/srvEbm.grpc.pb.cc \
	../include/srvCp.pb.cc \
	../include/srvCp.grpc.pb.cc

