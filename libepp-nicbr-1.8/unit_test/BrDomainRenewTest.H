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
/* $Id: BrDomainRenewTest.H 693 2006-04-26 14:35:18Z cacm $ */
/** @file BrDomainRenewTest.H
 *  @brief BrDomain Renew Action Test
 */

#ifndef __BR_DOMAIN_RENEW_TEST_H__
#define __BR_DOMAIN_RENEW_TEST_H__

#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace CppUnit;

/// BrDomainRenew Test Class
class BrDomainRenewTest : public CppUnit::TestFixture  
{
public:
  /// Constructor
  BrDomainRenewTest();

  /// Destructor
  ~BrDomainRenewTest();

  /// Allocate resources
  void setUp();

  /// Release resources
  void tearDown();

  // Tests a domain renew response
  void response_test();

private:

  CPPUNIT_TEST_SUITE(BrDomainRenewTest);
  CPPUNIT_TEST(response_test);
  CPPUNIT_TEST_SUITE_END();
};

#endif //__BR_DOMAIN_RENEW_TEST_H__
