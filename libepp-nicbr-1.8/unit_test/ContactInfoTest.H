/* ${copyri%ght}$ */
/* $Id: ContactInfoTest.H 321 2006-02-08 13:53:22Z milena $ */
/** @file ContactInfoTest.H
 *  @brief Contact Info Action Test
 */

#ifndef __CONTACT_INFO_TEST_H__
#define __CONTACT_INFO_TEST_H__

#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace CppUnit;

/// Contact Info Test Class
class ContactInfoTest : public CppUnit::TestFixture  
{
public:
  /// Constructor
  ContactInfoTest();

  /// Destructor
  ~ContactInfoTest();

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

  CPPUNIT_TEST_SUITE(ContactInfoTest);
  CPPUNIT_TEST(set_xml_template_test);
  CPPUNIT_TEST(command_test);
  CPPUNIT_TEST(response_test);
  CPPUNIT_TEST_SUITE_END();
};

#endif //__CONTACT_INFO_TEST_H__
