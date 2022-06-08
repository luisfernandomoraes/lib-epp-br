/* 
 * Copyright (C) 2009 Registro.br. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistribution of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY REGISTRO.BR ``AS IS AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIE OF FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL REGISTRO.BR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */
/* $Id: TransportTLSTestClient.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#include <string>

#include "libepp_nicbr.H"

#include <iostream>

#include "TransportException.H"
#include "TransportTLSTestClient.H"
#include "TransportTLS.H"

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(TransportTLSTestClient);

#define FILENAME "TransportTLSTestServer.port"

TransportTLSTestClient::TransportTLSTestClient() {}

TransportTLSTestClient::~TransportTLSTestClient() {}

void TransportTLSTestClient::setUp() {}

void TransportTLSTestClient::tearDown() {}

void TransportTLSTestClient::transport_rw_test() 
{
  // Gets the port number from TransportTLSTestServer.port file
  int port;
  FILE *fd = fopen(FILENAME, "r");
  CPPUNIT_ASSERT(fd != NULL);
  CPPUNIT_ASSERT(fscanf(fd, "%d", &port) == 1);
  fclose(fd);
  TransportTLS transportTLS("localhost", port);
  bool unexpected_exception = false;

  try {
    string hello_world = "Hello World!! Olá!!!";
    string result;
    CPPUNIT_ASSERT_NO_THROW(transportTLS.connect("client.pem", "root.pem",
						 "client passphrase"));
    CPPUNIT_ASSERT_NO_THROW(transportTLS.write(hello_world));
    CPPUNIT_ASSERT_NO_THROW(transportTLS.read(result));
    string iso_result;
    StrUtil::utf8_to_iso88591(result, iso_result);
    CPPUNIT_ASSERT_EQUAL(hello_world, iso_result);
  } catch (const TransportException &e) {
    printf("\nTransport Exception: code [%d] message [%s] low "
	   "level message [%s]\n", e.get_code(), e.get_msg().c_str(), 
	   e.get_low_level_msg().c_str());
    unexpected_exception = true;
  }

  CPPUNIT_ASSERT(!unexpected_exception);
}
