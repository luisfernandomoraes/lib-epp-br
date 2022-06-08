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
/* $Id: StringEncodingExceptionTest.H 527 2006-03-01 22:11:47Z koji $ */
/** @file StringEncodingExceptionTest.H
 *  @brief String Encoding Exception Test
 */

#ifndef __STRING_ENCODING_EXCEPTION_TEST_H__
#define __STRING_ENCODING_EXCEPTION_TEST_H__

#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace CppUnit;

class StringEncodingExceptionTest : public CppUnit::TestFixture 
{
public:
  /// Constructor
  StringEncodingExceptionTest();

  /// Destructor
  ~StringEncodingExceptionTest();

  /// Allocate resource
  void setUp();

  /// Release resource
  void tearDown();
    
  void missing_char_test();

  void ascii_encoding_test();

  void encoding_sequence_test();

  void first_octet_test();
private:
  
  CPPUNIT_TEST_SUITE(StringEncodingExceptionTest);
  CPPUNIT_TEST(missing_char_test);
  CPPUNIT_TEST(ascii_encoding_test);
  CPPUNIT_TEST(encoding_sequence_test);
  CPPUNIT_TEST(first_octet_test);
  CPPUNIT_TEST_SUITE_END();
};

#endif //__STRING_ENCODING_EXCEPTION_TEST_H__
