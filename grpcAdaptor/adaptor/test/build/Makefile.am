
# Prefixes : specify 'install' directory
# bin_ lib_ include_ pkginclude_ noinst_ ...

# Primary : specify target type
# PROGRAMS LIBRARIES LTLIBRARIES HEADERS ...

# Variable : specify expansions
# SOURCES CFLAGS CPPFLAGS CXXFLAGS LIBADD LDADD LDFLAGS ...

APP_PATH = $(top_srcdir)/../5g_l2l3
VENDOR_PATH = $(APP_PATH)/vendor
TRILLIUM_PATH = $(APP_PATH)/trillium/src/5gran/cu

noinst_PROGRAMS = test

test_SOURCES = ../src/test.cpp

test_LDFLAGS = -Wl,--start-group

test_CPPFLAGS = -I../ -I../../include \
			  -std=c++11

test_LDADD = -L../../objs/.libs \
			 -lTrillium \
			 -L$(VENDOR_PATH)/grpc/libs/opt \
			 -lgrpc \
			 -lgpr \
			 -lgrpc++ \
			 -lgrpc++_reflection
