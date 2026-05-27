## Copyright (C) 2024-2026 Yu Hongbo <yuhongbo@member.fsf.org>,
##                    CNOCTAVE <cnoctave@qq.com>
##
## Copying and distribution of this file, with or without modification,
## are permitted in any medium without royalty provided the copyright
## notice and this notice are preserved.  This file is offered as-is,
## without any warranty.

AC_PREREQ([2.67])
AC_INIT([octave_boost], [1.1.0])

AC_CONFIG_HEADERS([config.h])

AC_PROG_SED

AC_PATH_PROG([OCTAVE], [octave])
if test -z "$OCTAVE"; then
  AC_MSG_ERROR([*** 'octave' not found.])
fi

AC_PATH_PROG([MKOCTFILE], [mkoctfile])
if test -z "$MKOCTFILE"; then
  AC_MSG_ERROR([*** 'mkoctfile' not found.])
fi

dnl CXX is the compiler including options such as -std=c++17.
CXX=`${MKOCTFILE} -p CXX`
AC_PROG_CXX

CXX17_SWITCH=$(echo "$CXX" | $SED "s,^$CXX,,")
MKOCTFILE="$MKOCTFILE $CXX17_SWITCH"

dnl Pick up any warnings as soon as possible.
XTRA_CXXFLAGS="-Wall -Wextra -std=c++17"

AC_LANG(C++)

dnl Check for Boost headers (header-only library, no linkage needed)
dnl User may pass BOOST_CPPFLAGS to specify non-standard include path.
AC_ARG_VAR([BOOST_CPPFLAGS], [C++ preprocessor flags for Boost headers])

dnl Auto-detect Boost headers in ../boost/ (common for local builds)
if test -z "$BOOST_CPPFLAGS"; then
  if test -d "$srcdir/../boost/boost"; then
    dnl Layout: ../boost/boost/headers.hpp (nested)
    BOOST_CPPFLAGS="-I$srcdir/../boost"
  elif test -d "$srcdir/../boost"; then
    dnl Layout: ../boost/ contains accumulators/, chrono/, etc.
    dnl For #include <boost/foo.hpp> we need -I$srcdir/..
    BOOST_CPPFLAGS="-I$srcdir/.."
  elif test -d "../boost/boost"; then
    BOOST_CPPFLAGS="-I../boost"
  elif test -d "../boost"; then
    BOOST_CPPFLAGS="-I.."
  fi
fi

AC_MSG_CHECKING([for boost/accumulators/accumulators.hpp])
cppflags_saved="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM([[#include <boost/accumulators/accumulators.hpp>]])],
  [have_boost=yes],
  [have_boost=no]
)
CPPFLAGS="$cppflags_saved"

if test "$have_boost" = "yes"; then
  dnl Found
  AC_MSG_RESULT([yes])
elif test -n "$BOOST_CPPFLAGS"; then
  AC_MSG_RESULT([yes (using $BOOST_CPPFLAGS)])
else
  AC_MSG_WARN([boost/accumulators/accumulators.hpp not found in standard locations])
  AC_MSG_WARN([Boost.Accumulators is header-only; install boost-devel or libboost-dev])
  dnl Use default include path and let the build fail with a clearer error
  BOOST_CPPFLAGS=""
fi

AC_SUBST([BOOST_CPPFLAGS])
AC_SUBST([XTRA_CXXFLAGS])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
