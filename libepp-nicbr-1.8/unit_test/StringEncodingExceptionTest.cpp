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
/* $Id: StringEncodingExceptionTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#include <string>

#include "libepp_nicbr.H"

#include "StrUtil.H"
#include "StringEncodingExceptionTest.H"
#include "StringEncodingException.H"


LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(StringEncodingExceptionTest);

StringEncodingExceptionTest::StringEncodingExceptionTest() {}

StringEncodingExceptionTest::~StringEncodingExceptionTest() {}

void StringEncodingExceptionTest::setUp() {}

void StringEncodingExceptionTest::tearDown() {}

void StringEncodingExceptionTest::missing_char_test()
{
  string dummy;

  unsigned char utf8[] = { 0xC2, 0x00 };
  bool exception_caught = false;
  try {
    StrUtil::utf8_to_iso88591(string((char *)utf8), dummy);
  } catch (StringEncodingException &e) {
    exception_caught = true;
    CPPUNIT_ASSERT_EQUAL((int)StringEncodingException::MISSING_CHAR,
			 e.get_code());
  }
  CPPUNIT_ASSERT(exception_caught);

  unsigned char utf8_2[] = { 0xF0, 0x84, 0x00 };
  exception_caught = false;
  try {
    StrUtil::utf8_to_iso88591(string((char *)utf8_2), dummy);
  } catch (StringEncodingException &e) {
    exception_caught = true;
    CPPUNIT_ASSERT_EQUAL((int)StringEncodingException::MISSING_CHAR,
			 e.get_code());
  }
  CPPUNIT_ASSERT(exception_caught);
}

void StringEncodingExceptionTest::ascii_encoding_test()
{
  string dummy;
  unsigned char utf8[] = { 0xC1, 0xAF, 0x00 }; // 0x6F encoded
  bool exception_caught = false;
  try {
    StrUtil::utf8_to_iso88591(string((char *)utf8), dummy);
  } catch (StringEncodingException &e) {
    exception_caught = true;
    CPPUNIT_ASSERT_EQUAL((int)StringEncodingException::ASCII_ENCODING,
			 e.get_code());
  }
  CPPUNIT_ASSERT(exception_caught);
}

void StringEncodingExceptionTest::encoding_sequence_test()
{
  string dummy;

  unsigned char utf8[] = { 0xC2, 0x1F, 0x00 };
  bool exception_caught = false;
  try {
    StrUtil::utf8_to_iso88591(string((char *)utf8), dummy);
  } catch (StringEncodingException &e) {
    exception_caught = true;
    CPPUNIT_ASSERT_EQUAL((int)StringEncodingException::ENCODING_SEQUENCE,
			 e.get_code());
  }
  CPPUNIT_ASSERT(exception_caught);

  unsigned char utf8_2[] = { 0xF0, 0x84, 0x10, 0x00 };
  exception_caught = false;
  try {
    StrUtil::utf8_to_iso88591(string((char *)utf8_2), dummy);
  } catch (StringEncodingException &e) {
    exception_caught = true;
    CPPUNIT_ASSERT_EQUAL((int)StringEncodingException::ENCODING_SEQUENCE,
			 e.get_code());
  }
  CPPUNIT_ASSERT(exception_caught);
}

void StringEncodingExceptionTest::first_octet_test()
{
  string dummy;
  unsigned char utf8[] = { 0xF8, 0x00 };
  bool exception_caught = false;
  try {
    StrUtil::utf8_to_iso88591(string((char *)utf8), dummy);
  } catch (StringEncodingException &e) {
    exception_caught = true;
    CPPUNIT_ASSERT_EQUAL((int)StringEncodingException::FIRST_OCTET,
			 e.get_code());
  }
  CPPUNIT_ASSERT(exception_caught);
}
