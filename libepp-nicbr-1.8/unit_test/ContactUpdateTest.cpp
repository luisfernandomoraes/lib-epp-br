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
/* $Id: ContactUpdateTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#include <string>
#include <memory>
#include <iostream>

#include "libepp_nicbr.H"

#include "ContactUpdateTest.H"
#include "ContactUpdate.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(ContactUpdateTest);

ContactUpdateTest::ContactUpdateTest() {}

ContactUpdateTest::~ContactUpdateTest() {}

void ContactUpdateTest::setUp() {}

void ContactUpdateTest::tearDown() {}

void ContactUpdateTest::set_xml_template_test()
{
  string to_be_parsed =
    "<command>"
    "<update>"
    "<contact:update"
    "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0'"
    "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0"
    "contact-1.0.xsd'>"
    "<contact:id>$(id)$</contact:id>"
    "$(add)$"
    "$(rem)$"
    "$(chg)$"
    "</contact:update>"
    "</update>"
    "$(clTRID)$"
    "</command>";

  string expected =
    "<command>"
    "<update>"
    "<contact:update"
    "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0'"
    "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0"
    "contact-1.0.xsd'>"
    "<contact:id>cme254</contact:id>"
    "<contact:add>"
    "<contact:status s='clientDeleteProhibited'/>"
    "<contact:status s='linked'/>"
    "</contact:add>"
    "<contact:rem>"
    "<contact:status s='clientTransferProhibited'/>"
    "<contact:status s='clientUpdateProhibited'/>"
    "</contact:rem>"
    "<contact:chg>"
    "<contact:postalInfo type='int'>"
    "<contact:name>John Doe</contact:name>"
    "<contact:org>Example Inc.</contact:org>"
    "<contact:addr>"
    "<contact:street>123 Example Dr.</contact:street>"
    "<contact:street>Suite 100</contact:street>"
    "<contact:city>Dulles</contact:city>"
    "<contact:sp>VA</contact:sp>"
    "<contact:pc>20166-6503</contact:pc>"
    "<contact:cc>US</contact:cc>"
    "</contact:addr>"
    "</contact:postalInfo>"
    "<contact:postalInfo type='loc'>"
    "<contact:name>John Doe</contact:name>"
    "<contact:org>Other Inc.</contact:org>"
    "<contact:addr>"
    "<contact:street>123 Street</contact:street>"
    "<contact:street>7th floor</contact:street>"
    "<contact:street>Suite 123</contact:street>"
    "<contact:city>Miami</contact:city>"
    "<contact:cc>US</contact:cc>"
    "</contact:addr>"
    "</contact:postalInfo>"
    "<contact:voice x='1234'>+1.7035555555</contact:voice>"
    "<contact:fax x='3456'>+1.7035555556</contact:fax>"
    "<contact:email>jdoe@example.com</contact:email>"
    "<contact:authInfo>"
    "<contact:pw>2fooBAR</contact:pw>"
    "</contact:authInfo>"
    "<contact:disclose flag='1'>"
    "<contact:voice/>"
    "<contact:email/>"
    "</contact:disclose>"
    "</contact:chg>"
    "</contact:update>"
    "</update>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>";

  ContactUpdate contact_update;
  ContactUpdateCmd *cmd = contact_update.get_command();
  CommonData common;

  cmd->insert_status_list_add("clientDeleteProhibited");
  cmd->insert_status_list_add("linked");
  cmd->insert_status_list_rem("clientTransferProhibited");
  cmd->insert_status_list_rem("clientUpdateProhibited");
  common.set_id("cme254");
  PostalInfo postal_info;
  postal_info.set_type("int");
  postal_info.set_name("John Doe");
  postal_info.set_org("Example Inc.");
  postal_info.set_str1("123 Example Dr.");
  postal_info.set_str2("Suite 100");
  postal_info.set_city( "Dulles");
  postal_info.set_sp("VA");
  postal_info.set_pc("20166-6503");
  postal_info.set_cc("US");
  common.insert_postal_info(postal_info);
  postal_info.reset();
  postal_info.set_type("loc");
  postal_info.set_name("John Doe");
  postal_info.set_org("Other Inc.");
  postal_info.set_str1("123 Street");
  postal_info.set_str2("7th floor");
  postal_info.set_str3("Suite 123");
  postal_info.set_city( "Miami");
  postal_info.set_cc("US");
  common.insert_postal_info(postal_info);
  
  CommonData::Phone voice;
  voice.ext = "1234";
  voice.number =  "+1.7035555555";
  CommonData::Phone fax;
  fax.ext = "3456";
  fax.number = "+1.7035555556";
  common.set_voice(voice);
  common.set_fax(fax);
  common.set_email("jdoe@example.com");
  AuthInfo authInfo;
  authInfo.set_roid("");
  authInfo.set_pw("2fooBAR");
  cmd->set_authInfo(authInfo);
  
  CommonData::Disclose disclose;
  disclose.flag = 1;
  disclose.voice = true;
  disclose.email = true;
  common.set_disclose(disclose);
  cmd->set_common_data(common);

  contact_update.get_command()->set_clTRID("ABC-12345");
  contact_update.set_xml_template(to_be_parsed);

  CPPUNIT_ASSERT_EQUAL(expected, contact_update.get_xml());
  
  // tests reset method
  cmd->reset();

  CPPUNIT_ASSERT(cmd->get_status_list_add().empty());
  CPPUNIT_ASSERT(cmd->get_status_list_rem().empty());
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_common_data().get_id());
  CPPUNIT_ASSERT(cmd->get_common_data().get_postal_info().empty());
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_common_data().get_voice().ext);
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_common_data().get_voice().number);
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_common_data().get_fax().ext);
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_common_data().get_fax().number);
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_common_data().get_email());
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_authInfo().get_pw());
  CPPUNIT_ASSERT(cmd->get_common_data().get_disclose().flag == -1);
  CPPUNIT_ASSERT(cmd->get_common_data().get_disclose().name_int == false);
  CPPUNIT_ASSERT(cmd->get_common_data().get_disclose().name_loc == false);
  CPPUNIT_ASSERT(cmd->get_common_data().get_disclose().org_int == false);
  CPPUNIT_ASSERT(cmd->get_common_data().get_disclose().org_loc == false);
  CPPUNIT_ASSERT(cmd->get_common_data().get_disclose().addr_int == false);
  CPPUNIT_ASSERT(cmd->get_common_data().get_disclose().addr_loc == false);
  CPPUNIT_ASSERT(cmd->get_common_data().get_disclose().email == false);
  CPPUNIT_ASSERT(cmd->get_common_data().get_disclose().voice == false);
  CPPUNIT_ASSERT(cmd->get_common_data().get_disclose().fax == false);

  // parent attribute
  CPPUNIT_ASSERT(cmd->get_clTRID() == "");
}

void ContactUpdateTest::command_test()
{
  ContactUpdate contact_update;
  ContactUpdateCmd *cmd = contact_update.get_command();
  CommonData common;

  cmd->insert_status_list_add("clientDeleteProhibited");
  cmd->insert_status_list_add("linked");
  cmd->insert_status_list_rem("clientTransferProhibited");
  cmd->insert_status_list_rem("clientUpdateProhibited");
  common.set_id("cme254");
  PostalInfo postal_info;
  postal_info.set_type("int");
  postal_info.set_name("John Doe");
  postal_info.set_org("Example Inc.");
  postal_info.set_str1("123 Example Dr.");
  postal_info.set_str2("Suite 100");
  postal_info.set_str3("");
  postal_info.set_city( "Dulles");
  postal_info.set_sp("VA");
  postal_info.set_pc("20166-6503");
  postal_info.set_cc("US");
  common.insert_postal_info(postal_info);
  postal_info.set_type("loc");
  postal_info.set_name("John Doe");
  postal_info.set_org("Other Inc.");
  postal_info.set_str1("123 Street");
  postal_info.set_str2("7th floor");
  postal_info.set_str3("Suite 123");
  postal_info.set_city( "Miami");
  postal_info.set_sp("");
  postal_info.set_pc("");
  postal_info.set_cc("US");
  common.insert_postal_info(postal_info);
  
  CommonData::Phone voice;
  voice.ext = "1234";
  voice.number =  "+1.7035555555";
  CommonData::Phone fax;
  fax.ext = "3456";
  fax.number = "+1.7035555556";
  common.set_voice(voice);
  common.set_fax(fax);
  common.set_email("jdoe@example.com");
  AuthInfo authInfo;
  authInfo.set_roid("");
  authInfo.set_pw("2fooBAR");
  cmd->set_authInfo(authInfo);
  
  CommonData::Disclose disclose;
  disclose.flag = true;
  disclose.voice = true;
  disclose.email = true;
  common.set_disclose(disclose);
  cmd->set_common_data(common);

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/contact_update.xml");

    contact_update.get_command()->set_clTRID("ABC-12345");
    contact_update.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(contact_update.get_xml());
  } catch (const IoException &e) {
    exception_caught = true;
    printf("\nIO Exception: code [%d] message [%s]",
	e.get_code(), e.get_msg().c_str());
  } catch (const XmlException &e) {
    exception_caught = true;
    printf("\nXml Exception: code [%d] message [%s] low level message [%s]\n",
	   e.get_code(), e.get_msg().c_str(), e.get_low_level_msg().c_str());
  }

  CPPUNIT_ASSERT(!exception_caught);
}
