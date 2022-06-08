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
/* $Id: BrOrgCreateTest.cpp 1021 2009-05-11 18:56:22Z eduardo $ */
#if USE_BR_ORG
#include <memory>
#include <string>

#include "libepp_nicbr.H"

#include "BrOrgCreateTest.H"
#include "BrOrgCreate.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(BrOrgCreateTest);

BrOrgCreateTest::BrOrgCreateTest() {}

BrOrgCreateTest::~BrOrgCreateTest() {}

void BrOrgCreateTest::setUp() {}

void BrOrgCreateTest::tearDown() {}

void BrOrgCreateTest::set_xml_template_test()
{
  string to_be_parsed =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<create>"
    "<contact:create "
    "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd'>"
    "<contact:id>$(id)$</contact:id>"
    "$(postal_info)$"
    "$(voice)$"
    "$(fax)$"
    "<contact:email>$(email)$</contact:email>"
    "$(auth_info)$"
    "$(disclose)$"
    "</contact:create>"
    "</create>"
    "<extension>"
    "<brorg:create "
    "xmlns:brorg='urn:ietf:params:xml:ns:brorg-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brorg-1.0 "
    "brorg-1.0.xsd'>"
    "<brorg:organization>"
    "$(organization)$"
    "</brorg:organization>"
    "$(brorg_contact_list)$"
    "$(responsible)$"
    "</brorg:create>"
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
    "<create>"
    "<contact:create "
    "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd'>"
    "<contact:id>e123456</contact:id>"
    "<contact:postalInfo type='int'>"
    "<contact:name>John Doe</contact:name>"
    "<contact:org>Example Inc.</contact:org>"
    "<contact:addr>"
    "<contact:street>"
    "Av. Nacoes Unidas, 11541"
    "</contact:street>"
    "<contact:street>7o. andar</contact:street>"
    "<contact:city>Sao Paulo</contact:city>"
    "<contact:sp>SP</contact:sp>"
    "<contact:pc>04578-000</contact:pc>"
    "<contact:cc>BR</contact:cc>"
    "</contact:addr>"
    "</contact:postalInfo>"
    "<contact:voice x='1234'>+55.1155093500</contact:voice>"
    "<contact:fax>+55.1155093501</contact:fax>"
    "<contact:email>jdoe@example.com</contact:email>"
    "<contact:authInfo><contact:pw roid='SH8013-REP'>"
    "2fooBAR</contact:pw></contact:authInfo>"
    "<contact:disclose flag='0'>"
    "<contact:voice/>"
    "<contact:email/>"
    "</contact:disclose>"
    "</contact:create>"
    "</create>"
    "<extension>"
    "<brorg:create "
    "xmlns:brorg='urn:ietf:params:xml:ns:brorg-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brorg-1.0 "
    "brorg-1.0.xsd'>"
    "<brorg:organization>"
    "005.506.560/0001-36"
    "</brorg:organization>"
    "<brorg:contact type='admin'>fan</brorg:contact>"
    "<brorg:responsible>John Doe</brorg:responsible>"
    "</brorg:create>"
    "</extension>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";

  BrOrgCreate brorg_create;
  BrOrgCreateCmd *cmd = brorg_create.get_command();
  CommonData common;
  
  //parent information  
  common.set_id("e123456");
  PostalInfo postal_info;
  postal_info.set_type("int");
  postal_info.set_name("John Doe");
  postal_info.set_org("Example Inc.");
  postal_info.set_str1("Av. Nacoes Unidas, 11541");
  postal_info.set_str2("7o. andar");
  postal_info.set_str3("");
  postal_info.set_city( "Sao Paulo");
  postal_info.set_sp("SP");
  postal_info.set_pc("04578-000");
  postal_info.set_cc("BR");
  common.insert_postal_info(postal_info);
  
  CommonData::Phone voice;
  voice.ext = "1234";
  voice.number =  "+55.1155093500";
  CommonData::Phone fax;
  fax.ext = "";
  fax.number = "+55.1155093501";
  common.set_voice(voice);
  common.set_fax(fax);
  common.set_email("jdoe@example.com");
  AuthInfo authInfo;
  authInfo.set_roid("SH8013-REP");
  authInfo.set_pw("2fooBAR");
  cmd->set_authInfo(authInfo);
  
  CommonData::Disclose disclose;
  disclose.flag = false;
  disclose.name_int = false;
  disclose.voice = true;
  disclose.email = true;
  common.set_disclose(disclose);
  
  //BrOrg specific
  cmd->set_organization("005.506.560/0001-36");
  cmd->set_responsible("John Doe");
  cmd->insert_contact("admin", "fan");
  cmd->set_common_data(common);
  brorg_create.get_command()->set_clTRID("ABC-12345");
  brorg_create.set_xml_template(to_be_parsed);

  CPPUNIT_ASSERT_EQUAL(expected, brorg_create.get_xml());

  // tests reset method
  cmd->reset();

  //parent information
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_common_data().get_id());
  CPPUNIT_ASSERT(cmd->get_common_data().get_postal_info().empty());
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_common_data().get_voice().ext);
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_common_data().get_voice().number);
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_common_data().get_fax().ext);
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_common_data().get_fax().number);
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_common_data().get_email());
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_authInfo().get_roid());
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

  //BrOrgCreate specific information
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_organization());
  CPPUNIT_ASSERT_EQUAL((string) "", cmd->get_responsible());  
  CPPUNIT_ASSERT(cmd->get_contact_list().empty());
}

void BrOrgCreateTest::command_test()
{
  BrOrgCreate brorg_create;
  BrOrgCreateCmd *cmd = brorg_create.get_command();
  CommonData common;
  //parent information
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
  postal_info.set_name("Tião Macalé");
  postal_info.set_org("ISO-8859-1 Chars ¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹»¼½ Ltda");
  postal_info.set_str1("Rua ÝÞßàáâãäåæçèéêë");
  postal_info.set_str2("7º andar");
  postal_info.set_str3("Suite 123");
  postal_info.set_city("São Paulo");
  postal_info.set_sp("SP");
  postal_info.set_pc("");
  postal_info.set_cc("BR");
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
  authInfo.set_roid("SH8013-REP");
  authInfo.set_pw("2fooBAR");
  cmd->set_authInfo(authInfo);
  
  CommonData::Disclose disclose;
  disclose.flag = false;
  disclose.name_int = true;
  disclose.voice = true;
  disclose.email = true;
  common.set_disclose(disclose);

  //BrOrgCreate specific
  cmd->set_organization("005.506.560/0001-36");
  cmd->insert_contact("admin", "fan");
  
  cmd->set_common_data(common);
  
  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/br_org_create.xml");

    brorg_create.get_command()->set_clTRID("ABC-12345");
    brorg_create.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(brorg_create.get_xml());
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

void BrOrgCreateTest::response_test()
{
  string expected = 
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
    "<epp xmlns=\"urn:ietf:params:xml:ns:epp-1.0\" "
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd\">"
    "<response>"
    "<result code=\"1000\">"
    "<msg>Command completed successfully</msg>"
    "</result>"
    "<resData>"
    "<contact:creData "
    "xmlns:contact=\"urn:ietf:params:xml:ns:contact-1.0\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd\">"
    "<contact:id>cem456</contact:id>"
    "<contact:crDate>2006-01-30T22:00:00.0Z</contact:crDate>"
    "</contact:creData>"
    "</resData>"
    "<extension>"
    "<brorg:creData xmlns:brorg=\"urn:ietf:params:xml:ns:brorg-1.0\">"
    "<brorg:organization>005.506.560/0001-36</brorg:organization>"
    "</brorg:creData>"
    "</extension>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>DEF-54321</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";
  
  DomParser parser;
  parser.enable_validation("../docs/schemas");
  
  BrOrgCreate brorg_create;
  CPPUNIT_ASSERT_NO_THROW(brorg_create.set_response(expected, &parser));
  BrOrgCreateRsp *brorg_create_rsp = brorg_create.get_response();

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = brorg_create_rsp->get_result_list();
  r_it = results.begin();

  CPPUNIT_ASSERT(r_it != results.end());
  CPPUNIT_ASSERT_EQUAL(Response::OK, r_it->first);
 
  string result_code = "1000";
   
  string response = 
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
    "<epp xmlns=\"urn:ietf:params:xml:ns:epp-1.0\" "
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd\">"
    "<response>"
    "<result code=\"" + result_code + "\">"
    "<msg";
  
  string result_lang = brorg_create_rsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response +=
    ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<resData>"
    "<contact:creData "
    "xmlns:contact=\"urn:ietf:params:xml:ns:contact-1.0\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd\">"
    "<contact:id>" + brorg_create_rsp->get_id() + "</contact:id>"
    "<contact:crDate>" + brorg_create_rsp->get_crDate() + "</contact:crDate>"
    "</contact:creData>"
    "</resData>"
    "<extension>"
    "<brorg:creData xmlns:brorg=\"urn:ietf:params:xml:ns:brorg-1.0\">"
    "<brorg:organization>" +  brorg_create_rsp->get_organization() +
    "</brorg:organization>"
    "</brorg:creData>"
    "</extension>"
    "<trID>"
    "<clTRID>" + brorg_create_rsp->get_clTRID() +"</clTRID>"
    "<svTRID>" + brorg_create_rsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(response, expected);
  
  /* Reset Test */
  CPPUNIT_ASSERT(brorg_create_rsp->get_id() != "");
  CPPUNIT_ASSERT(brorg_create_rsp->get_crDate() != "");
  CPPUNIT_ASSERT(brorg_create_rsp->get_organization() != "");
  
  brorg_create_rsp->reset();
  
  CPPUNIT_ASSERT(brorg_create_rsp->get_id() == "");
  CPPUNIT_ASSERT(brorg_create_rsp->get_crDate() == "");
  CPPUNIT_ASSERT(brorg_create_rsp->get_organization() == "");
}
#endif //USE_BR_ORG
