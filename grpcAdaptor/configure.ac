AC_PREREQ(2.59)
AC_INIT(data,0.5,tech-support@mavenir.com)

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR()
AC_CONFIG_HEADER([config/config.h:config/config.in])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_MAKE_SET
AC_PROG_RANLIB

AC_PROG_LIBTOOL

AC_PROG_LN_S
AC_PATH_PROG(GPERF,gperf)
AC_CHECK_PROG(GPERFFLAGS, gperf, [-L 'ANSI-C' -cptlTD])

AC_ARG_ENABLE(debug,
[  --disable-debug  Turn off debugging],
[case "${enableval}" in
	yes) debug=true ;;
	no) debug=false ;;
	*) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
esac],[debug=true])

if test "x$debug" = "xtrue"; then
	MAVBASECFLAGS='-g -D_DEBUG'
	MAVBASECXXFLAGS='-g -D_DEBUG -Wall -Wcast-align -fno-strict-aliasing -fcheck-new -Wno-deprecated -Woverloaded-virtual -Wnon-virtual-dtor -Wno-invalid-offsetof'
else 
	MAVBASECFLAGS='-O2 -DIEINLINE'
	MAVBASECXXFLAGS='-O2 -Wall -Wcast-align -DIEINLINE -fno-strict-aliasing' 
fi

AM_CONDITIONAL(DEBUG_ENABLED, test "x$debug" = "xtrue")
AM_CONDITIONAL(TOOLS_ENABLED, test "x$tools" = "xtrue")
AM_CONDITIONAL(BEARER_ENABLED, test "x$bearer" = "xtrue")

AM_CONDITIONAL(TARGET_RHEL_7_2_X86_64, test "x$PLATFORM" = "xRHEL_7_2_X86_64")
AM_CONDITIONAL(TCMALLOC_TRUE, test "$TCMALLOC" = "TRUE")

case $PRODUCT in
        ENBCU)
  MAVBASECFLAGS=${MAVBASECFLAGS}' -DENBCU'
  MAVBASECXXFLAGS=${MAVBASECXXFLAGS}' -DENBCU'
  MAVBASECPPFLAGS=${MAVBASECPPFLAGS}' -DENBCU'
        ;;
        *) echo "no product input"
        ;;
esac

AM_CONDITIONAL(PRODUCT_UAG, test [ "x$PRODUCT" = "xUAG" ] )

RELEASE_INCLUDES='-I$(top_srcdir) -I$(top_srcdir)/../grpc'

AC_SUBST(DEFAULT_INCLUDES, $RELEASE_INCLUDES)                                                                                                                            

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_mutex_init])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([sys/socket.h])
AC_CHECK_HEADERS([arpa/inet.h memory.h netinet/in.h netdb.h unistd.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_C_BIGENDIAN
AC_HEADER_TIME
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_SELECT_ARGTYPES
#AC_FUNC_MALLOC
#AC_FUNC_MEMCMP
#AC_FUNC_REALLOC
AC_TYPE_SIGNAL
AC_CHECK_FUNCS([memset strdup strcasecmp strchr])
AC_CHECK_FUNCS([select socket gethostbyaddr gethostbyname inet_ntoa])
AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_FUNC([epoll_create], [AC_DEFINE([HAVE_EPOLL], 1, [Define if the epoll interface is supported])])

MMX_CFLAGS=${MAVBASECFLAGS}' '${MMX_CFLAGS}
MMX_CXXFLAGS=${MAVBASECXXFLAGS}' '${MMX_CXXFLAGS}
MMX_CPPFLAGS=${MAVBASECPPFLAGS}' '${MMX_CPPFLAGS}
MMX_LDFLAGS=${LDFLAGS}' -rdynamic '${MMX_LDFLAGS}
MMX_LIBS=${LIBS}' -lpthread -no-install '${MMX_LIBS}

AC_SUBST(AM_CFLAGS, $MMX_CFLAGS)
AC_SUBST(AM_CXXFLAGS, $MMX_CXXFLAGS)
AC_SUBST(AM_CPPFLAGS, $MMX_CPPFLAGS)
AC_SUBST(LDFLAGS, $MMX_LDFLAGS)
AC_SUBST(LIBS, $MMX_LIBS)

# Checks for system services.

# Done 
AC_CONFIG_FILES(
    Makefile \
    services/Makefile \
    services/grpc/objs/Makefile \
    services/grpc/test/Makefile \
    services/cucp/Makefile \
    services/cucp/objs/Makefile \
    common/Makefile \
    common/objs/Makefile \
    common/objs/Service/Makefile \
    adaptor/Makefile \
    adaptor/objs/Makefile
)
AC_OUTPUT
