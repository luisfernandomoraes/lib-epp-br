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
/* $Id: AsnDeleteTest.H 973 2008-12-12 11:03:03Z rafael $ */
/** @file AsnDeleteTest.H
 *  @brief AsnDelete EPP Action Test
 */

#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#ifndef __ASNDELETETEST_H__
#define __ASNDELETETEST_H__

using namespace CppUnit;

/// AsnDeleteTest Class
class AsnDeleteTest : public CppUnit::TestFixture
{
public:
  /// Constructor
  AsnDeleteTest();

  /// Destructor
  ~AsnDeleteTest();

  /// Allocate resources
  void setUp();

  /// Release resources
  void tearDown();

  /// Test set_xml_template method
  void set_xml_template_test();

  // Tests a ip range create command
  void command_test();

  // Tests a ip range create response
  void response_test();

private:
  CPPUNIT_TEST_SUITE(AsnDeleteTest);
  CPPUNIT_TEST(set_xml_template_test);
  CPPUNIT_TEST(command_test);
  CPPUNIT_TEST(response_test);
  CPPUNIT_TEST_SUITE_END();
};

#endif // __ASNDELETETEST_H__
