/* ${copyri%ght}$ */
/* $Id: BrOrgInfoTest.H 504 2006-02-24 16:58:23Z eduardo $ */
/** @file BrOrgInfoTest.H
 *  @brief BrOrg Info Action Test
 */

#ifndef __BR_ORG_INFO_TEST_H__
#define __BR_ORG_INFO_TEST_H__

#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace CppUnit;

/// Contact Info Test Class
class BrOrgInfoTest : public CppUnit::TestFixture  
{
public:
  /// Constructor
  BrOrgInfoTest();

  /// Destructor
  ~BrOrgInfoTest();

  /// Allocate resources
  void setUp();

  /// Release resources
  void tearDown();

  /// Test set_XML_template method
  void set_xml_template_test();

  // Tests a contact info command
  void command_test();

  // Tests a contact info response
  void response_test();
  
private:

  CPPUNIT_TEST_SUITE(BrOrgInfoTest);
  CPPUNIT_TEST(set_xml_template_test);
  CPPUNIT_TEST(command_test);
  CPPUNIT_TEST(response_test);
  CPPUNIT_TEST_SUITE_END();
};

#endif //__BR_ORG_INFO_TEST_H__
