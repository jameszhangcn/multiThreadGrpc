#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
#AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_INIT(testgrpc, 1.0, jianzhang)
AM_INIT_AUTOMAKE(testgrpc, 1.0)

AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL
# Checks for libraries.
# FIXME: Replace `main' with a function in `-lCUUP':
AC_CHECK_LIB([CUUP], [main])
# FIXME: Replace `main' with a function in `-lgpr':
AC_CHECK_LIB([gpr], [main])
# FIXME: Replace `main' with a function in `-lgrpc':
AC_CHECK_LIB([grpc], [main])
# FIXME: Replace `main' with a function in `-lprotobuf':
AC_CHECK_LIB([protobuf], [main])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])
AC_DEFINE(PROTOBUF_USE_DLLS,1,"protobuf")

AC_PATH_TOOL(PKG_CONFIG, pkg-config)
# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_CONFIG_SUBDIRS([grpcAdaptor])
AC_OUTPUT
