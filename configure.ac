dnl MoboTV
dnl configure.ac

AC_INIT([mobotv], [0.2], [rene@renesp.com.br])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])


dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL


dnl Check for pkgconfig first
AC_CHECK_PROG(HAVE_PKGCONFIG, pkg-config, yes, no)

dnl Give error and exit if we don't have pkgconfig
if test "x$HAVE_PKGCONFIG" = "xno"; then
	AC_MSG_ERROR(you need to have pkgconfig installed !)
fi


dnl Checks for libraries.
PKG_CHECK_MODULES([GTK], [gtk+-2.0 >= 2.2 glib-2.0 >= 2.2])
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

PKG_CHECK_MODULES([HILDON], [hildon-1 >= 1.0])
AC_SUBST(HILDON_CFLAGS)
AC_SUBST(HILDON_LIBS)

AC_CHECK_LIB([conffile], new_conffile,, AC_MSG_ERROR(libconffile not found. See http://libconffile.sourceforge.net))
AC_SUBST([CONFFILE])

XML2CONFIG=xml2-config
AC_ARG_WITH(xml2-config,
[[  --with-xml2-config=FILE Use the given path to xml2-config when determining
                          libxml2 configuration; defaults to "xml2-config"]],
[
    if test "$withval" != "yes" -a "$withval" != ""; then
        XML2CONFIG=$withval
    fi
])
xml2version=0

AC_DEFUN([XML2TEST],
[
	AC_REQUIRE([AC_PROG_SED])
	AC_MSG_CHECKING([libxml2 version])
	if xml2version=`$XML2CONFIG --version`; then
		AC_MSG_RESULT([$xml2version])
		xml2version=`echo $xml2version | sed 's/\./0/g'`
	else
		AC_MSG_RESULT([not found])
		AC_MSG_ERROR([libxml2 is required. Try --with-xml2-config.])
	fi
])

dnl Call XML2TEST func
XML2TEST

dnl check libxml2 version
if test -z "$xml2version" || test "$xml2version" -lt 206000; then
	AC_MSG_ERROR([libxml2 2.6 or newer is required])
fi

LIBXML2_CFLAGS="`$XML2CONFIG --cflags`"
LIBXML2_LIBS="`$XML2CONFIG --libs`"

AC_SUBST(LIBXML2_CFLAGS)
AC_SUBST(LIBXML2_LIBS)

GST_VERSION_MAJOR=0
GST_VERSION_MINOR=10
GST_VERSION_RELEASE=0
GSTREAMER_REQ=$GST_VERSION_MAJOR.$GST_VERSION_MINOR.$GST_VERSION_RELEASE
GST_MAJORMINOR=$GST_VERSION_MAJOR.$GST_VERSION_MINOR
PKG_CHECK_MODULES(GST, gstreamer-$GST_MAJORMINOR >= $GSTREAMER_REQ
					   gstreamer-interfaces-$GST_MAJORMINOR >= $GSTREAMER_REQ,,
						AC_MSG_ERROR([gstreamer-$GST_MAJORMINOR not found. See http://gstreamer.freedesktop.org/]))
AC_SUBST(GST_CFLAGS)
AC_SUBST(GST_LIBS)


dnl Checks for header files.
AC_CHECK_HEADERS(dirent.h,, AC_MSG_ERROR([dirent.h header not found.]))
AC_CHECK_HEADERS(conffile/conffile.h,, AC_MSG_ERROR([conffile.h not found. Please install libconffile headers.]))

dnl Checks for typedefs, structures, and compiler characteristics.

dnl Checks for library functions.

dnl Output files.
AC_CONFIG_HEADER(config.h)
AC_CONFIG_FILES([Makefile src/Makefile pixmaps/Makefile])
AC_OUTPUT

