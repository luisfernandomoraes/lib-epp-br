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
/* $Id: TransportExceptionTestClient.H 391 2006-02-10 12:17:37Z koji $ */
/** @file TransportExceptionTestClient.H
 *  @brief Transport Exception Test
 */

#ifndef __TRANSPORT_EXCEPTION_TEST_CLIENT_H__
#define __TRANSPORT_EXCEPTION_TEST_CLIENT_H__

#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace CppUnit;

class TransportExceptionTestClient : public CppUnit::TestFixture 
{
public:
  /// Constructor
  TransportExceptionTestClient();

  /// Destructor
  ~TransportExceptionTestClient();

  /// Allocate resource
  void setUp();

  /// Release resource
  void tearDown();

  /// Test Connect Transport Exception
  void connect_exception_test();

  /// Test BIO_READ_ERR Transport Exception
  void bio_read_err_exception_test();

  /// Test MISSING_TOTAL_LENGTH Transport Exception
  void missing_total_length_test();

  /// Test INVALID_TOTAL_LENGTH Transport Exception
  void invalid_total_length_exception_test();

  /// Test PAYLOAD_INCOMPLETE Transport Exception
  void payload_incomplete_exception_test();

  /// Test BIO_WRITE_ERR Transport Exception
  void bio_write_exception_test();

private:
  
  CPPUNIT_TEST_SUITE(TransportExceptionTestClient);
  CPPUNIT_TEST(connect_exception_test);
  CPPUNIT_TEST(bio_read_err_exception_test);
  CPPUNIT_TEST(missing_total_length_test);
  CPPUNIT_TEST(invalid_total_length_exception_test);
  CPPUNIT_TEST(payload_incomplete_exception_test); 
  CPPUNIT_TEST(bio_write_exception_test);
  CPPUNIT_TEST_SUITE_END();
};

#endif //__TRANSPORT_EXCEPTION_TEST_CLIENT_H__
