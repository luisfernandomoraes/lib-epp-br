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
## $Id: Makefile.tmp 1011 2009-03-12 18:49:24Z eduardo $ ##

AM_CPPFLAGS = -I$(top_srcdir)/include
EXTRA_DIST = client.pem server.pem root.pem SessionTestServer.data 
TESTS = TransportTLSTestServer TransportExceptionTestServer \
	SessionTestServer CppUnitTest
check_PROGRAMS = TransportTLSTestServer TransportExceptionTestServer \
	SessionTestServer CppUnitTest

TransportTLSTestServer_SOURCES = TransportTLSTestServer.cpp
TransportTLSTestServer_LDFLAGS = -lssl

TransportExceptionTestServer_SOURCES = TransportExceptionTestServer.cpp
TransportExceptionTestServer_LDFLAGS = -lssl

SessionTestServer_SOURCES = SessionTestServer.cpp
SessionTestServer_LDADD = $(top_srcdir)/lib/libepp-nicbr-1.8.a
SessionTestServer_LDFLAGS = -lssl -lxerces-c

CppUnitTest_SOURCES = CppUnitTestMain.cpp \
		StrUtilTest.cpp StrUtilTest.H \
		StringEncodingExceptionTest.cpp StringEncodingExceptionTest.H \
		DomParserTest.cpp DomParserTest.H \
		LoginTest.cpp LoginTest.H \
		LogoutTest.cpp LogoutTest.H \
		GreetingTest.cpp GreetingTest.H \
		ContactCheckTest.cpp ContactCheckTest.H \
		ContactCreateTest.cpp ContactCreateTest.H \
		ContactInfoTest.cpp ContactInfoTest.H \
		ContactUpdateTest.cpp ContactUpdateTest.H \
		DomainCheckTest.cpp DomainCheckTest.H \
		DomainCreateTest.cpp DomainCreateTest.H \
		DomainInfoTest.cpp DomainInfoTest.H \
		DomainRenewTest.cpp DomainRenewTest.H \
		DomainUpdateTest.cpp DomainUpdateTest.H \
		DomainDeleteTest.cpp DomainDeleteTest.H \
		PollTest.cpp PollTest.H \
		TransportTLSTestClient.cpp TransportTLSTestClient.H \
		TransportExceptionTestClient.cpp \
		TransportExceptionTestClient.H \
		SessionTestClient.cpp SessionTestClient.H \
		XmlExceptionTest.cpp XmlExceptionTest.H \
		BrDomainCheckTest.cpp BrDomainCheckTest.H \
		BrDomainInfoTest.cpp BrDomainInfoTest.H \
		BrDomainCreateTest.cpp BrDomainCreateTest.H \
		BrDomainRenewTest.cpp BrDomainRenewTest.H \
		BrDomainUpdateTest.cpp BrDomainUpdateTest.H \
		BrOrgCheckTest.cpp BrOrgCheckTest.H \
		BrOrgInfoTest.cpp BrOrgInfoTest.H \
		BrOrgCreateTest.cpp BrOrgCreateTest.H \
		BrOrgUpdateTest.cpp BrOrgUpdateTest.H \
		IpNetworkCreateTest.cpp IpNetworkCreateTest.H \
		IpNetworkDeleteTest.cpp IpNetworkDeleteTest.H \
		IpNetworkRenewTest.cpp IpNetworkRenewTest.H \
		IpNetworkTransferTest.cpp IpNetworkTransferTest.H \
		IpNetworkCheckTest.cpp IpNetworkCheckTest.H \
		IpNetworkInfoTest.cpp IpNetworkInfoTest.H \
		IpNetworkUpdateTest.cpp IpNetworkUpdateTest.H \
		AsnCreateTest.cpp AsnCreateTest.H \
		AsnDeleteTest.cpp AsnDeleteTest.H \
		AsnRenewTest.cpp AsnRenewTest.H \
		AsnTransferTest.cpp AsnTransferTest.H \
		AsnCheckTest.cpp AsnCheckTest.H \
		AsnInfoTest.cpp AsnInfoTest.H \
		AsnUpdateTest.cpp AsnUpdateTest.H

CppUnitTest_LDADD = $(top_srcdir)/lib/libepp-nicbr-1.8.a
CppUnitTest_LDFLAGS = -lcppunit -lssl -lxerces-c
