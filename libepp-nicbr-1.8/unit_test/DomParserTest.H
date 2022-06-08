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
/* $Id: DomParserTest.H 303 2006-02-07 19:37:36Z milena $ */
/** @file DomParserTest.H
 *  @brief Dom Parser Test
 */

#ifndef __DOMPARSER_TEST_H__
#define __DOMPARSER_TEST_H__

#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "DomParser.H"

using namespace CppUnit;

LIBEPP_NICBR_NS_USE

/// Login Test Class
class DomParserTest : public CppUnit::TestFixture  
{
public:
  /// Constructor
  DomParserTest();

  /// Destructor
  ~DomParserTest();

  /// Allocate resources
  void setUp();

  /// Release resources
  void tearDown();

  // Enable XML Validation test
  void enable_validation_test();

  // Disable XML Validation test
  void disable_validation_test();

  // Parse Command test
  void parse_command_test();

protected:
  DomParser *_parser;

private:

  CPPUNIT_TEST_SUITE(DomParserTest);
  CPPUNIT_TEST(enable_validation_test);
  CPPUNIT_TEST(disable_validation_test);
  CPPUNIT_TEST(parse_command_test);
  CPPUNIT_TEST_SUITE_END();
};

#endif //__DOMPARSER_TEST_H__
