##  Copyright (C) 2009 Registro.br. All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
# 1. Redistribution of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY REGISTRO.BR ``AS IS AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIE OF FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL REGISTRO.BR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
# TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
# USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
# DAMAGE.
 ## 
## $Id: configure.tmp 977 2008-12-12 20:04:19Z eduardo $ ##

AC_INIT(libepp-nicbr, 1.8) 

AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE(libepp-nicbr, 1.8, libepp-nicbr)

AC_CONFIG_HEADER([config.h])

if test "$prefix" = "NONE"
then
	prefix="/usr/local"
fi

schemasdir="${prefix}/share/libepp_nicbr/schemas"
templatesdir="${prefix}/share/libepp_nicbr/templates"
sslcertdir="${prefix}/share/libepp_nicbr/ssl"

AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_RANLIB

if test "$CFLAGS" = "-g -O2" || test "$CFLAGS" = "NONE"
then
	CFLAGS="-O -pipe"
fi

AC_ARG_WITH([devel-cflags],
	AC_HELP_STRING([--with-devel-cflags], [use more strict CFLAGS]),
	CFLAGS="-O -pipe -Wall -Werror -ansi -pedantic"
)

AC_ARG_WITH([no-br-extensions],
	AC_HELP_STRING([--with-no-br-extensions], [disable .br specific extensions]),
	CFLAGS="${CFLAGS} -DUSE_BR_DOMAINS=0",
	CFLAGS="${CFLAGS} -DUSE_BR_ORG=1 -DUSE_BR_DOMAINS=1"
)

AC_ARG_WITH([no-ip-extensions],
  AC_HELP_STRING([--with-no-ip-extensions], [disable IP specific extensions]),
  CFLAGS="${CFLAGS} -DUSE_IP_MANAGEMENT=0",
  CFLAGS="${CFLAGS} -DUSE_BR_ORG=1 -DUSE_IP_MANAGEMENT=1"
)

CFLAGS="$CFLAGS -I../include"
CXXFLAGS="${CFLAGS}"

#Check for xerves-c library
xercesc_path="/usr/local"
AC_ARG_WITH([xerces-c], 
	AC_HELP_STRING([--with-xerces-c=DIR],
			[look for the xerces-c library in DIR]),
	[xercesc_path=$withval]
)		

CPPFLAGS="$CPPFLAGS -I$xercesc_path/include"
LDFLAGS="$LDFLAGS -L$xercesc_path/lib"

AC_LANG(C++)

AC_CHECK_HEADER(
	[xercesc/dom/DOM.hpp],
	have_xercesc="yes",
	have_xercesc="no"
)

if test "$have_xercesc" = "no"
then
	AC_MSG_FAILURE([could not find xerces-c library. Maybe you could specify the correct path for xerces-c with the --with-xerces-c=DIR option.])
fi

# Check for OpenSSL
openssl_path="/usr/local"
AC_ARG_WITH([openssl], 
	AC_HELP_STRING([--with-openssl=DIR],
			[look for the openssl library in DIR]),
	[openssl_path=$withval]
)	

if test $openssl_path != $xercesc_path
then
	CPPFLAGS="$CPPFLAGS -I$openssl_path/include"
	LDFLAGS="$LDFLAGS -L$openssl_path/lib -lssl -lcrypto"
fi

AC_CHECK_HEADER([openssl/bio.h],,have_openssl="no")
AC_CHECK_LIB(ssl, BIO_new_connect, ,
  [AC_CHECK_LIB(crypto, BIO_new_connect, , have_openssl="no")]
)

if test "$have_openssl" = "no"
then
	AC_MSG_FAILURE([could not find openssl library. Maybe you could specify the correct path for openssl with the --with-openssl=DIR option.])
fi

if test "$have_openssl" != "no"
then
	openssl_incl=`pkg-config --cflags openssl`
	if test "${openssl_incl}" != ""
	then
		echo "OpenSSL CFLAGS: ${openssl_incl}"
		CPPFLAGS="$CPPFLAGS ${openssl_incl}"
	fi
fi

# Check for Cppunit
cppunit_path="/usr/local"
AC_ARG_WITH([cppunit],
	AC_HELP_STRING([--with-cppunit=DIR],
			[look for the cppunit library in DIR]),
	[cppunit_path=$withval]
)

if test $cppunit_path != $xercesc_path -a $cppunit_path != openssl_path
then
	CPPFLAGS="$CPPFLAGS -I$cppunit_path/include"
	LDFLAGS="$LDFLAGS -L$cppunit_path/lib"
fi

AC_CHECK_HEADER(
	[cppunit/TestRunner.h],
	have_cppunit="yes",
	have_cppunit="no"
)

if test "$have_cppunit" = "no"
then
	AC_MSG_WARN([could not find cppunit library. Without it, you will not be able to run the library tests with "make check". Maybe you could specify the correct path for cppunit with the --with-cppunit=DIR option.])
fi

# Check for function srandomdev
AC_CHECK_FUNCS(srandomdev)

# Check for readline headers
have_readline_includes="yes"
readline_includes_path="/usr/include"

AC_ARG_WITH([readline_includes],
        AC_HELP_STRING([--with-readline-includes=DIR],
                        [look for the readline includes in DIR]),
        [readline_includes_path=$withval]
)

AC_CHECK_HEADER(
	[$readline_includes_path/readline/readline.h], ,have_readline_includes="no"
)

AC_CHECK_HEADER(
	[$readline_includes_path/readline/history.h], ,have_readline_includes="no"
)

# Check for ncurses and readline libs
AC_CHECK_LIB(ncurses, tgetnum, , have_ncurses_lib="no")
AC_CHECK_LIB(readline, readline, , have_readline_lib="no")

# Outputs shepp information in case it cannot be compiled
SHEPP_DIR=
if test "$have_ncurses_lib" = "no"
then
	AC_MSG_WARN([ncurses library not found; shepp, the EPP shell client, won't be built.])

elif test "$have_readline_lib" = "no"
then
	AC_MSG_WARN([readline library not found; shepp, the EPP shell client, won't be built.])

elif test "$have_readline_includes" = "no"
then
	AC_MSG_WARN([readline includes not found; shepp, the EPP shell client, won't be built unless you specify the correct path for them using '--with-readline-includes=DIR'.])
else
	SHEPP_DIR=src/shepp
fi
AC_SUBST([SHEPP_DIR])


AC_CONFIG_COMMANDS(
	[schemasdir templatesdir sslcertdir],
	[rm -f include/libepp_nicbr.H
	echo "/*" >> include/libepp_nicbr.H
	echo " * Copyright (C) 2006 Registro.br. All rights reserved." >> include/libepp_nicbr.H
	echo " * " >> include/libepp_nicbr.H
	echo " * Redistribution and use in source and binary forms, with or without " >> include/libepp_nicbr.H
	echo " * modification, are permitted provided that the following conditions are " >> include/libepp_nicbr.H
	echo " * met: " >> include/libepp_nicbr.H
	echo " * 1. Redistribution of source code must retain the above copyright " >> include/libepp_nicbr.H
	echo " *    notice, this list of conditions and the following disclaimer. " >> include/libepp_nicbr.H
	echo " * 2. Redistributions in binary form must reproduce the above copyright " >> include/libepp_nicbr.H
	echo " *    notice, this list of conditions and the following disclaimer in the " >> include/libepp_nicbr.H
	echo " *    documentation and\/or other materials provided with the distribution. " >> include/libepp_nicbr.H
	echo " *  " >> include/libepp_nicbr.H
	echo " * THIS SOFTWARE IS PROVIDED BY REGISTRO.BR ``AS IS'' AND ANY EXPRESS OR " >> include/libepp_nicbr.H
	echo " * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED " >> include/libepp_nicbr.H
	echo " * WARRANTIE OF FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO " >> include/libepp_nicbr.H
	echo " * EVENT SHALL REGISTRO.BR BE LIABLE FOR ANY DIRECT, INDIRECT, " >> include/libepp_nicbr.H
	echo " * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, " >> include/libepp_nicbr.H
	echo " * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS " >> include/libepp_nicbr.H
	echo " * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND " >> include/libepp_nicbr.H
	echo " * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR " >> include/libepp_nicbr.H
	echo " * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE " >> include/libepp_nicbr.H
	echo " * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH " >> include/libepp_nicbr.H
	echo " * DAMAGE. " >> include/libepp_nicbr.H
	echo " */" >> include/libepp_nicbr.H
	echo "/** @file libepp_nicbr.H" >> include/libepp_nicbr.H
	echo " *  @brief Project defines" >> include/libepp_nicbr.H
	echo " */" >> include/libepp_nicbr.H
	echo >> include/libepp_nicbr.H
	echo \#ifndef __LIBEPP_NICBR_H__ >> include/libepp_nicbr.H
	echo \#define __LIBEPP_NICBR_H__ >> include/libepp_nicbr.H
	echo >> include/libepp_nicbr.H
	echo \#define LIBEPP_NICBR_NS_BEGIN namespace libepp_nicbr { >> include/libepp_nicbr.H
	echo \#define LIBEPP_NICBR_NS_END } >> include/libepp_nicbr.H
	echo \#define LIBEPP_NICBR_NS_USE using namespace libepp_nicbr\; >> include/libepp_nicbr.H
	echo >> include/libepp_nicbr.H
	echo \#define SCHEMASDIR \"$schemasdir\" >> include/libepp_nicbr.H
	echo \#define TEMPLATESDIR \"$templatesdir\" >> include/libepp_nicbr.H
	echo \#define SSLCERTDIR \"$sslcertdir\" >> include/libepp_nicbr.H
	echo >> include/libepp_nicbr.H
	echo \#endif //__LIBEPP_NICBR_H__ >> include/libepp_nicbr.H],
	[schemasdir=$schemasdir templatesdir=$templatesdir sslcertdir=$sslcertdir]
)
	
AC_CONFIG_FILES(Makefile lib/Makefile src/shepp/Makefile unit_test/Makefile)
AC_OUTPUT
