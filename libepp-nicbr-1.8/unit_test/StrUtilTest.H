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
/* $Id: StrUtilTest.H 753 2006-06-09 13:39:58Z koji $ */
/** @file StrUtilTest.H
 *  @brief StrUtil Test Library
 */

#ifndef __UTILS_TEST_H__
#define __UTILS_TEST_H__

#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace CppUnit;

/// StrUtil Test Class
class StrUtilTest : public CppUnit::TestFixture  
{
public:
  /// Constructor
  StrUtilTest();

  /// Destructor
  ~StrUtilTest();

  /// Allocate resources
  void setUp();

  /// Release resources
  void tearDown();

  /// Test parse method
  void parse_test();

  /// Test gsub method
  void gsub_test();

  /// Test to_string method
  void to_string_test();

  /// Test iso88591_to_utf8 method
  void iso88591_to_utf8_test();

  /// Test utf8_to_iso88591 method
  void utf8_to_iso88591_test();

  /// Test XML Beautifier method
  void xml_beautifier_test();

  /// Text esc_xml_markup method
  void xml_escape_test();

private:

  CPPUNIT_TEST_SUITE(StrUtilTest);
  CPPUNIT_TEST(parse_test);
  CPPUNIT_TEST(gsub_test);
  CPPUNIT_TEST(to_string_test);
  CPPUNIT_TEST(iso88591_to_utf8_test);
  CPPUNIT_TEST(utf8_to_iso88591_test);
  CPPUNIT_TEST(xml_beautifier_test);
  CPPUNIT_TEST(xml_escape_test);
  CPPUNIT_TEST_SUITE_END();
};

#endif
