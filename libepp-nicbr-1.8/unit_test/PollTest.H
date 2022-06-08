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
/* $Id: PollTest.H 962 2008-12-11 15:20:55Z eduardo $ */
/** @file PollTest.H
 *  @brief Poll Action Test
 */

#ifndef __POLL_TEST_H__
#define __POLL_TEST_H__

#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace CppUnit;

/// Poll Test Class
class PollTest : public CppUnit::TestFixture  
{
public:
  /// Constructor
  PollTest();

  /// Destructor
  ~PollTest();

  /// Allocate resources
  void setUp();

  /// Release resources
  void tearDown();

  /// Test set_XML_template method
  void set_xml_template_test();

  // Tests a poll command
  void command_test();

  // Tests a poll response
  void response_test();

#if USE_BR_DOMAINS  
  // Tests a poll response with extension
  void extension_response_test();
#endif //USE_BR_DOMAINS

  // Tests a poll response with pandata
  void pandata_test();

#if USE_BR_DOMAINS  
  // Tests a poll response with brorg_pandata
  void brdomain_pandata_test();
#endif //USE_BR_DOMAINS
private:

  CPPUNIT_TEST_SUITE(PollTest);
  CPPUNIT_TEST(set_xml_template_test);
  CPPUNIT_TEST(command_test);
  CPPUNIT_TEST(response_test);
#if USE_BR_DOMAINS  
  CPPUNIT_TEST(extension_response_test);
#endif //USE_BR_DOMAINS
  CPPUNIT_TEST(pandata_test);
#if USE_BR_DOMAINS  
  CPPUNIT_TEST(brdomain_pandata_test);
#endif //USE_BR_DOMAINS
  CPPUNIT_TEST_SUITE_END();
};

#endif //__POLL_TEST_H__
