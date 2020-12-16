# Prefixes : specify 'install' directory
# bin_ lib_ include_ pkginclude_ noinst_ ...

# Primary : specify target type
# PROGRAMS LIBRARIES LTLIBRARIES HEADERS ...

# Variable : specify expansions
# SOURCES CFLAGS CPPFLAGS CXXFLAGS LIBADD LDADD LDFLAGS ...

VENDOR_PATH = $(top_srcdir)/../..

noinst_LIBRARIES = libCucpService.a

libCucpService_a_CPPFLAGS = -std=c++11 -fPIC \
			  			-I$(VENDOR_PATH)/grpc/dist/include \
						-I$(top_srcdir)/services/grpc/include \
						-I$(top_srcdir)/common/Service \
						-I.. -I../inc \
						-I$(VENDOR_PATH)/grpc/third_party/protobuf/src

libCucpService_a_SOURCES = ../source/CucpService.cpp
