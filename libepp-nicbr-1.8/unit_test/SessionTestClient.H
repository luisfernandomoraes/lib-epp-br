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
/* $Id: SessionTestClient.H 385 2006-02-09 20:59:12Z milena $ */
/** @file SessionTestClient.H
 *  @brief Session Test Client
 */

#ifndef __SESSION_TEST_CLIENT_H__
#define __SESSION_TEST_CLIENT_H__

#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Session.H"

using std::auto_ptr;

using namespace CppUnit;

LIBEPP_NICBR_NS_USE

/// Session Test Client Class
class SessionTestClient : public CppUnit::TestFixture  
{
public:
  /// Constructor
  SessionTestClient();

  /// Destructor
  ~SessionTestClient();

  /// Allocate resources
  void setUp();

  /// Release resources
  void tearDown();

  /// Main Session Test method the only one included in the Unit Test suite
  void session_test();  

private:
  /// Do not put theses methods in cppunit test suite
  /// because the cppunit framework closes the connection  
  void connect_test();  
  void login_test();
  void create_org_contact_test();
  void send_hello_test();
  void logout_test();

  CPPUNIT_TEST_SUITE(SessionTestClient);
  CPPUNIT_TEST(session_test);
  CPPUNIT_TEST_SUITE_END();

  // Session 
  auto_ptr<Session> _session;
  
  // XML Parser
  auto_ptr<DomParser> _command_parser;
};

#endif //__SESSION_TEST_CLIENT_H__
