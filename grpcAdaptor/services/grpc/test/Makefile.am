
# Prefixes : specify 'install' directory
# bin_ lib_ include_ pkginclude_ noinst_ ...

# Primary : specify target type
# PROGRAMS LIBRARIES LTLIBRARIES HEADERS ...

# Variable : specify expansions
# SOURCES CFLAGS CPPFLAGS CXXFLAGS LIBADD LDADD LDFLAGS ...

VENDOR_PATH = $(top_srcdir)/vendor

noinst_PROGRAMS = SctpIwf GW App

###############################################
SctpIwf_SOURCES=SctpIwf.cpp

SctpIwf_LDFLAGS = -Wl,--start-group

SctpIwf_CPPFLAGS = -I../ -I../include \
			  -std=c++11 \
			  -I$(VENDOR_PATH)/grpc/dist/include \
			  -I$(VENDOR_PATH)/grpc/third_party/protobuf/src \
			  -I$(top_srcdir)/common/Service

SctpIwf_LDADD = \
			   ../objs/libmavsctp.a\
			   $(top_srcdir)/common/objs/Service/libService.a \
			   -L$(VENDOR_PATH)/grpc/libs/opt \
			   -lgrpc \
			   -lgpr \
			   -lgrpc++ \
			   -lgrpc++_reflection

###############################################
GW_SOURCES=GW.cpp

GW_LDFLAGS = -Wl,--start-group

GW_CPPFLAGS = -I../ -I../include \
			  -std=c++11 \
			  -I$(VENDOR_PATH)/grpc/dist/include \
			  -I$(VENDOR_PATH)/grpc/third_party/protobuf/src \
			  -I$(top_srcdir)/common/Service

GW_LDADD = \
			   ../objs/libmavsctp.a\
			   $(top_srcdir)/common/objs/Service/libService.a \
			   -L$(VENDOR_PATH)/grpc/libs/opt \
			   -lgrpc \
			   -lgpr \
			   -lgrpc++ \
			   -lgrpc++_reflection

###############################################
App_SOURCES=App.cpp

App_LDFLAGS = -Wl,--start-group

App_CPPFLAGS = -I../ -I../include \
			  -std=c++11 \
			  -I$(VENDOR_PATH)/grpc/dist/include \
			  -I$(VENDOR_PATH)/grpc/third_party/protobuf/src \
			  -I$(top_srcdir)/common/Service

App_LDADD = \
			   ../objs/libmavsctp.a\
			   $(top_srcdir)/common/objs/Service/libService.a \
			   -L$(VENDOR_PATH)/grpc/libs/opt \
			   -lgrpc \
			   -lgpr \
			   -lgrpc++ \
			   -lgrpc++_reflection
