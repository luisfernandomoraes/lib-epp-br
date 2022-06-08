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
/* $Id: BrOrgUpdateTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#if USE_BR_ORG
#include <memory>
#include <string>

#include "libepp_nicbr.H"

#include "BrOrgUpdateTest.H"
#include "BrOrgUpdate.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(BrOrgUpdateTest);

BrOrgUpdateTest::BrOrgUpdateTest() {}

BrOrgUpdateTest::~BrOrgUpdateTest() {}

void BrOrgUpdateTest::setUp() {}

void BrOrgUpdateTest::tearDown() {}

void BrOrgUpdateTest::set_xml_template_test()
{
  string to_be_parsed =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<update>"
    "<contact:update "
    "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd'>"
    "<contact:id>$(id)$</contact:id>"
    "$(add)$"
    "$(rem)$"
    "$(chg)$"
    "</contact:update>"
    "</update>"
    "<extension>"
    "<brorg:update "
    "xmlns:brorg='urn:ietf:params:xml:ns:brorg-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brorg-1.0 "
    "brorg-1.0.xsd'>"
    "<brorg:organization>$(organization)$</brorg:organization>"
    "$(brorg_add)$"
    "$(brorg_rem)$"
    "$(brorg_chg)$"
    "</brorg:update>"
    "</extension>"
    "$(clTRID)$"
    "</command>"
    "</epp>";

  string expected =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<update>"
    "<contact:update "
    "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd'>"
    "<contact:id>e654321</contact:id>"
    "</contact:update>"
    "</update>"
    "<extension>"
    "<brorg:update "
    "xmlns:brorg='urn:ietf:params:xml:ns:brorg-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brorg-1.0 "
    "brorg-1.0.xsd'>"
    "<brorg:organization>"
    "005.506.560/0001-36"
    "</brorg:organization>"
    "<brorg:add>"
    "<brorg:contact type='admin'>hkk</brorg:contact>"
    "</brorg:add>"
    "<brorg:rem>"
    "<brorg:contact type='admin'>fan</brorg:contact>"
    "</brorg:rem>"
    "<brorg:chg>"
    "<brorg:responsible>Responsible Name</brorg:responsible>"
    "<brorg:exDate>2009-02-01T12:00:00.0Z</brorg:exDate>"
    "</brorg:chg>"
    "</brorg:update>"
    "</extension>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";

  BrOrgUpdate brorg_update;
  BrOrgUpdateCmd *cmd = brorg_update.get_command();

  CommonData common;
  common.set_id("e654321");
  cmd->set_organization("005.506.560/0001-36");
  cmd->set_responsible("Responsible Name");
  cmd->set_exDate("2009-02-01T12:00:00.0Z");
  cmd->insert_contact_add("admin", "hkk");
  cmd->insert_contact_rem("admin", "fan");

  brorg_update.get_command()->set_clTRID("ABC-12345");
  cmd->set_common_data(common);
  brorg_update.set_xml_template(to_be_parsed);

  CPPUNIT_ASSERT_EQUAL(expected, brorg_update.get_xml());
  
  //tests reset method
  cmd->reset();

  //parent information
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

  //BrOrgUpdate specific
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_organization());
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_responsible());
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_exDate());
  CPPUNIT_ASSERT(cmd->get_contact_list_add().empty());
  CPPUNIT_ASSERT(cmd->get_contact_list_rem().empty());
}

void BrOrgUpdateTest::command_test()
{
  BrOrgUpdate br_org_update;
  BrOrgUpdateCmd *cmd = br_org_update.get_command();
  
  CommonData common;
  //parent information
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
  voice.number = "+1.7035555555";
  common.set_voice(voice);

  CommonData::Phone fax;
  fax.ext = "3456";
  fax.number = "+1.7035555556";
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

  //BrOrgUpdate specific information
  cmd->set_organization("005.506.560/0001-36");
  cmd->set_exDate("2009-02-01T12:00:00.0Z");
  cmd->insert_contact_add("admin", "hkk");
  cmd->insert_contact_rem("admin", "fan");
  cmd->set_common_data(common);
  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/br_org_update.xml");

    br_org_update.get_command()->set_clTRID("ABC-12345");
    br_org_update.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(br_org_update.get_xml());
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
#endif //USE_BR_ORG
