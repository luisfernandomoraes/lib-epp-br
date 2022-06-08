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
/* $Id: CppUnitTestMain.cpp 1006 2009-02-19 20:29:57Z rafael $ */
/** @file CppUnitTestMain.cpp
 *  @brief CppUnit main program
 */
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <stdexcept>
#include <fstream>

int main(int argc, char* argv[])
{
  // Create the event manager and test controller
  CPPUNIT_NS::TestResult controller;

  // Add a listener that colllects test result
  CPPUNIT_NS::TestResultCollector result;
  controller.addListener(&result);

  // Add a listener that print dots as test run.
  CPPUNIT_NS::BriefTestProgressListener progress;
  controller.addListener(&progress);

  // Add the top suite to the test runner
  CPPUNIT_NS::TestRunner runner;
  runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());

  try {
    std::cout << "Running \n";
    runner.run(controller, "StrUtilTest");
    runner.run(controller, "StringEncodingExceptionTest");
    runner.run(controller, "DomParserTest");
    runner.run(controller, "LoginTest");
    runner.run(controller, "LogoutTest");
    runner.run(controller, "GreetingTest");
    runner.run(controller, "ContactCheckTest");
    runner.run(controller, "ContactCreateTest");
    runner.run(controller, "ContactInfoTest");
    runner.run(controller, "ContactUpdateTest");
    runner.run(controller, "DomainCheckTest");
    runner.run(controller, "DomainCreateTest");
    runner.run(controller, "DomainInfoTest");
    runner.run(controller, "DomainRenewTest");
    runner.run(controller, "DomainUpdateTest");
    runner.run(controller, "DomainDeleteTest");
    runner.run(controller, "PollTest");
    runner.run(controller, "TransportTLSTestClient");
    runner.run(controller, "TransportExceptionTestClient");
    runner.run(controller, "SessionTestClient");
    runner.run(controller, "XmlExceptionTest");
#if USE_BR_DOMAINS
    runner.run(controller, "BrDomainCheckTest");
    runner.run(controller, "BrDomainInfoTest");
    runner.run(controller, "BrDomainCreateTest");
    runner.run(controller, "BrDomainRenewTest");
    runner.run(controller, "BrDomainUpdateTest");
#endif //USE_BR_DOMAINS
#if USE_BR_ORG
    runner.run(controller, "BrOrgCheckTest");
    runner.run(controller, "BrOrgInfoTest");
    runner.run(controller, "BrOrgCreateTest");
    runner.run(controller, "BrOrgUpdateTest");
#endif //USE_BR_ORG
#if USE_IP_MANAGEMENT
    runner.run(controller, "IpNetworkCreateTest");
    runner.run(controller, "IpNetworkDeleteTest");
    runner.run(controller, "IpNetworkRenewTest");
    runner.run(controller, "IpNetworkTransferTest");
    runner.run(controller, "IpNetworkCheckTest");
    runner.run(controller, "IpNetworkInfoTest");
    runner.run(controller, "IpNetworkUpdateTest");
    runner.run(controller, "AsnCreateTest");
    runner.run(controller, "AsnDeleteTest");
    runner.run(controller, "AsnRenewTest");
    runner.run(controller, "AsnTransferTest");
    runner.run(controller, "AsnCheckTest");
    runner.run(controller, "AsnInfoTest");
    runner.run(controller, "AsnUpdateTest");
#endif //USE_IP_MANAGEMENT
    std::cout << std::endl;

    // Print test in a compiler compatible format.
    CPPUNIT_NS::CompilerOutputter outputter(&result, std::cerr);
    outputter.write();
  } catch (std::invalid_argument &e) {
    std::cerr << std::endl << "ERROR: " << e.what() << std::endl;
    return 2;
  }

  return result.wasSuccessful() ? 0 : 1;
}
