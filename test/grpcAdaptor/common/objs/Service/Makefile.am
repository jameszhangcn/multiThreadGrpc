# Prefixes : specify 'install' directory
# bin_ lib_ include_ pkginclude_ noinst_ ...

# Primary : specify target type
# PROGRAMS LIBRARIES LTLIBRARIES HEADERS ...

# Variable : specify expansions
# SOURCES CFLAGS CPPFLAGS CXXFLAGS LIBADD LDADD LDFLAGS ...

VENDOR_PATH = $(top_srcdir)/../..

noinst_LIBRARIES = libService.a libServiceNoAck.a

libService_a_CPPFLAGS = -std=c++11 -fPIC \
			  			-I$(VENDOR_PATH)/grpc/dist/include \
						-I$(top_srcdir)/services/grpc/include \
						-I.. -I../inc \
						-I$(VENDOR_PATH)/grpc/third_party/protobuf/src

libService_a_SOURCES = ../../Service/ServiceServerImpl.cpp \
					   ../../Service/ServiceClientImpl.cpp \
					   ../../Service/ServiceCommonType.cpp

#########################################################
libServiceNoAck_a_CPPFLAGS = $(libService_a_CPPFLAGS)

libServiceNoAck_a_SOURCES = ../../Service/ServiceImplNoAck.cpp

