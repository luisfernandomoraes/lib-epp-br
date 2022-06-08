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
/* $Id: TransportExceptionTestClient.cpp 1046 2009-10-30 19:57:19Z eduardo $ */
#include <string>

#include "libepp_nicbr.H"

#include "TransportExceptionTestClient.H"
#include "TransportException.H"
#include "TransportTLS.H"

LIBEPP_NICBR_NS_USE

#define   FILENAME    "TransportExceptionTestServer.port"

CPPUNIT_TEST_SUITE_REGISTRATION(TransportExceptionTestClient);

TransportExceptionTestClient::TransportExceptionTestClient() {}

TransportExceptionTestClient::~TransportExceptionTestClient() {}

void TransportExceptionTestClient::setUp() {}

void TransportExceptionTestClient::tearDown() {}

void TransportExceptionTestClient::connect_exception_test() 
{
  bool exception_caught = false;
  TransportTLS transport;

  try {
    transport.set_server("");
    transport.connect("client.pem", "server.pem", "client passphrase");
  } catch (const TransportException &e) {
    exception_caught = true;
    int exception_code = TransportException::BIO_DO_CONNECT_ERR;
    CPPUNIT_ASSERT_EQUAL(exception_code, e.get_code());
    CPPUNIT_ASSERT("Connection could not be established" == e.get_msg());
    CPPUNIT_ASSERT("error:2006A066:BIO routines:BIO_get_host_ip:bad "
		   "hostname lookup" == e.get_low_level_msg());
  } 

  try {
    transport.set_server("localhost");
    transport.set_port(3120);
    transport.connect("client.pem", "server.pem", "client passphrase");
  } catch (const TransportException &e) {
    exception_caught = true;
    int exception_code = TransportException::BIO_DO_CONNECT_ERR;
    CPPUNIT_ASSERT_EQUAL(exception_code, e.get_code());
    CPPUNIT_ASSERT("Connection could not be established" == e.get_msg());
  } 

  CPPUNIT_ASSERT(exception_caught);
}

void TransportExceptionTestClient::bio_read_err_exception_test()
{
  bool exception_caught = false;
  TransportTLS transport;
  string payload;

  try {
    transport.read(payload);
  } catch (const TransportException &e) {
    exception_caught = true;
    int exception_code = TransportException::LOST_CONNECTION;
    CPPUNIT_ASSERT_EQUAL(exception_code, e.get_code());
    CPPUNIT_ASSERT_EQUAL((string) "Lost connection to peer", e.get_msg());
  }
  CPPUNIT_ASSERT(exception_caught);
}

void TransportExceptionTestClient::missing_total_length_test() 
{
  // Gets the port number from TransportExceptionTestServer.port file
  int port;
  FILE *fd = fopen(FILENAME, "r");
  CPPUNIT_ASSERT(fd != NULL);
  CPPUNIT_ASSERT(fscanf(fd, "%d", &port) == 1);
  fclose(fd);
  TransportTLS transport("localhost", port);

  bool exception_caught = false;
  
  try {
    transport.connect("client.pem", "root.pem", "client passphrase");
    transport.write("1");  
    string xml_payload;
    transport.read(xml_payload);    
  } catch (const TransportException &e) {
    exception_caught = true;
    int exception_code = TransportException::MISSING_TOTAL_LENGTH;
    CPPUNIT_ASSERT_EQUAL(exception_code, e.get_code());
    CPPUNIT_ASSERT("Missing total length of the data unit" == e.get_msg());
  }

  CPPUNIT_ASSERT(exception_caught);
}

void TransportExceptionTestClient::invalid_total_length_exception_test() 
{
  // Gets the port number from TransportExceptionTestServer.port file
  int port;
  FILE *fd = fopen(FILENAME, "r");
  CPPUNIT_ASSERT(fd != NULL);
  CPPUNIT_ASSERT(fscanf(fd, "%d", &port) == 1);
  fclose(fd);
  TransportTLS transport("localhost", port);

  bool exception_caught = false;
  
  try {
    transport.connect("client.pem", "root.pem", "client passphrase");
    transport.write("2");  
    string xml_payload;
    transport.read(xml_payload);    
  } catch (const TransportException &e) {
    exception_caught = true;
    int exception_code = TransportException::INVALID_TOTAL_LENGTH;
    CPPUNIT_ASSERT_EQUAL(exception_code, e.get_code());
    CPPUNIT_ASSERT("Invalid total length of the data unit" == e.get_msg());
  }

  CPPUNIT_ASSERT(exception_caught);
}

void TransportExceptionTestClient::payload_incomplete_exception_test() 
{
  // Gets the port number from TransportExceptionTestServer.port file
  int port;
  FILE *fd = fopen(FILENAME, "r");
  CPPUNIT_ASSERT(fd != NULL);
  CPPUNIT_ASSERT(fscanf(fd, "%d", &port) == 1);
  fclose(fd);
  TransportTLS transport("localhost", port);

  bool exception_caught = false;
  
  try {
    transport.connect("client.pem", "root.pem", "client passphrase");
    transport.write("3");  
    string xml_payload;
    transport.read(xml_payload, 2);    
  } catch (const TransportException &e) {
    exception_caught = true;
    int exception_code = TransportException::PAYLOAD_INCOMPLETE;
    CPPUNIT_ASSERT_EQUAL(exception_code, e.get_code());
    CPPUNIT_ASSERT("Incomplete XML payload" == e.get_msg());
  } 

  CPPUNIT_ASSERT(exception_caught);
}

void TransportExceptionTestClient::bio_write_exception_test()
{
  bool exception_caught = false;
  TransportTLS transport;
  string payload = "Hello World!!";

  try {
    transport.write(payload);
  } catch (const TransportException &e) {
    exception_caught = true;
    int exception_code = TransportException::LOST_CONNECTION;
    CPPUNIT_ASSERT_EQUAL(exception_code, e.get_code());
    CPPUNIT_ASSERT_EQUAL((string) "Lost connection to peer", e.get_msg());
  }

  CPPUNIT_ASSERT(exception_caught);
}
