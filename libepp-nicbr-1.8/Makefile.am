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
## $Id: Makefile.am 770 2006-06-19 21:36:24Z koji $ ##

SUBDIRS = lib $(SHEPP_DIR) unit_test
DIST_SUBDIRS = lib src/shepp unit_test

libepp_nicbr_includedir = $(includedir)/libepp_nicbr
dist_libepp_nicbr_include_HEADERS = include/*.H

EXTRA_DIST = config/libepp_dox samples

templatesdir = $(prefix)/share/libepp_nicbr/templates
dist_templates_DATA = docs/templates/*.xml

schemasdir = $(prefix)/share/libepp_nicbr/schemas
dist_schemas_DATA = docs/schemas/*.xsd

sslcertdir = $(prefix)/share/libepp_nicbr/ssl
dist_sslcert_DATA = unit_test/root.pem unit_test/client.pem

doc:
	doxygen config/libepp_dox

dist-hook:
	rm -rf $(distdir)/samples/.svn

