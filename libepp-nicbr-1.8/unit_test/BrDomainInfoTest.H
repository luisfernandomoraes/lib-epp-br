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
/* $Id: BrDomainInfoTest.H 901 2007-03-08 21:07:01Z eduardo $ */
/** @file BrDomainInfoTest.H
 *  @brief BrDomain Info Action Test
 */

#ifndef __BR_DOMAIN_INFO_TEST_H__
#define __BR_DOMAIN_INFO_TEST_H__

#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace CppUnit;

/// BrDomainInfo Test Class
class BrDomainInfoTest : public CppUnit::TestFixture  
{
public:
  /// Constructor
  BrDomainInfoTest();

  /// Destructor
  ~BrDomainInfoTest();

  /// Allocate resources
  void setUp();

  /// Release resources
  void tearDown();

  /// Test set_xml_template method
  void set_xml_template_test();

  // Tests a domain check command
  void command_test();

  // Tests a domain check response with a ticket number in the request
  void response_test();
  void response_test(bool withDNSSECExtension);

  // Tests a domain check response without a ticket number in the request
  void response_test2();

private:

  CPPUNIT_TEST_SUITE(BrDomainInfoTest);
  CPPUNIT_TEST(set_xml_template_test);
  CPPUNIT_TEST(command_test);
  CPPUNIT_TEST(response_test);
  CPPUNIT_TEST(response_test2);
  CPPUNIT_TEST_SUITE_END();
};

#endif //__BR_DOMAIN_INFO_TEST_H__
