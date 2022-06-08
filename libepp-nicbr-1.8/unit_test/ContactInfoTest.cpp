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
/* $Id: ContactInfoTest.cpp 1015 2009-03-12 19:28:56Z eduardo $$ */
#include <string>
#include <memory>
#include <iostream>

#include "libepp_nicbr.H"
#include "StrUtil.H"

#include "ContactInfoTest.H"
#include "ContactInfo.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(ContactInfoTest);

ContactInfoTest::ContactInfoTest() {}

ContactInfoTest::~ContactInfoTest() {}

void ContactInfoTest::setUp() {}

void ContactInfoTest::tearDown() {}

void ContactInfoTest::set_xml_template_test()
{
  string to_be_parsed =
    "<command>"
      "<info>"
        "<contact:info "
         "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0' "
         "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0 "
         "contact-1.0.xsd'>"
          "<contact:id>$(id)$</contact:id>"
         "$(auth_info)$"
        "</contact:info>"
      "</info>"
    "$(clTRID)$"
   "</command>";

  ContactInfo contact_info;
  ContactInfoCmd* contact_info_cmd = contact_info.get_command();
  contact_info_cmd->set_id("cem456");

  AuthInfo authInfo;
  authInfo.set_roid("SH8013-REP");
  authInfo.set_pw("2fooBAR");
  contact_info_cmd->set_authInfo(authInfo);
    
  contact_info.get_command()->set_clTRID("ABC-12345");
  contact_info.set_xml_template(to_be_parsed);

  string expected =
    "<command>"
      "<info>"
        "<contact:info "
         "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0' "
         "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0 "
         "contact-1.0.xsd'>"
          "<contact:id>cem456</contact:id>"
          "<contact:authInfo>"
            "<contact:pw roid='SH8013-REP'>2fooBAR</contact:pw>"
          "</contact:authInfo>"
        "</contact:info>"
      "</info>"
      "<clTRID>ABC-12345</clTRID>"
   "</command>";

  CPPUNIT_ASSERT_EQUAL(expected, contact_info.get_xml());

  // tests reset method
  CPPUNIT_ASSERT(contact_info_cmd->get_id() != "");
  CPPUNIT_ASSERT(contact_info_cmd->get_authInfo().get_roid() != "");
  CPPUNIT_ASSERT(contact_info_cmd->get_authInfo().get_pw() != "");
  contact_info_cmd->reset();
  CPPUNIT_ASSERT(contact_info_cmd->get_id() == "");
  CPPUNIT_ASSERT(contact_info_cmd->get_authInfo().get_roid() == "");
  CPPUNIT_ASSERT(contact_info_cmd->get_authInfo().get_pw() == "");

  // parent attribute
  CPPUNIT_ASSERT(contact_info_cmd->get_clTRID() == "");
}

void ContactInfoTest::command_test()
{
  ContactInfo contact_info;
  ContactInfoCmd *contact_info_cmd = contact_info.get_command();

  contact_info_cmd->set_id("cem456");

  AuthInfo authInfo;
  authInfo.set_roid("SH8013-REP");
  authInfo.set_pw("2fooBAR");
  contact_info_cmd->set_authInfo(authInfo);

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/contact_info.xml");

    contact_info.get_command()->set_clTRID("ABC-12345");
    contact_info.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(contact_info.get_xml());
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

void ContactInfoTest::response_test()
{
  string expected =
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
    "<epp xmlns=\"urn:ietf:params:xml:ns:epp-1.0\" "
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd\">"
    "<response>"
    "<result code='1000'>"
    "<msg>Command completed successfully</msg>"
    "</result>"
    "<resData>"
    "<contact:infData "
    "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd'>"
    "<contact:id>cme254</contact:id>"
    "<contact:roid>SH8013-REP</contact:roid>"
    "<contact:status s='clientDeleteProhibited'/>"
    "<contact:status s='linked'/>"
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
    "<contact:clID>ClientY</contact:clID>"
    "<contact:crID>ClientX</contact:crID>"
    "<contact:crDate>1999-04-03T22:00:00.0Z</contact:crDate>"
    "<contact:upID>ClientX</contact:upID>"
    "<contact:upDate>1999-12-03T09:00:00.0Z</contact:upDate>"
    "<contact:trDate>2000-04-08T09:00:00.0Z</contact:trDate>"
    "<contact:authInfo>"
    "<contact:pw>2fooBAR</contact:pw>"
    "</contact:authInfo>"
    "<contact:disclose flag='0'>"
    "<contact:voice/>"
    "<contact:email/>"
    "</contact:disclose>"
    "</contact:infData>"
    "</resData>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>DEF-54321</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";
  
  DomParser parser;
  parser.enable_validation("../docs/schemas");
  
  ContactInfo contact_info;
  CPPUNIT_ASSERT_NO_THROW(contact_info.set_response(expected, &parser));
  
  ContactInfoRsp* rsp = contact_info.get_response();
  
  CommonData::Disclose disclose;
  
  CPPUNIT_ASSERT(rsp != 0);
  string result_code = "1000";

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = rsp->get_result_list();
  r_it = results.begin();

  CPPUNIT_ASSERT(r_it != results.end());
  CPPUNIT_ASSERT_EQUAL(Response::OK, r_it->first);
  
  string response = 
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
    "<epp xmlns=\"urn:ietf:params:xml:ns:epp-1.0\" "
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd\">"
    "<response>"
    "<result code='" + result_code + "'>"
    "<msg";
  
  string result_lang = rsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response +=
    ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<resData>"
    "<contact:infData "
    "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd'>"
    "<contact:id>" + rsp->get_common_data().get_id() + 
    "</contact:id>"
    "<contact:roid>" + rsp->get_roid() + "</contact:roid>";
  set<string> status_list = rsp->get_status_set();
  set<string>::const_iterator it;
  for (it = status_list.begin(); it != status_list.end(); it++) {
    response += "<contact:status s='" + (*it) + "'/>";
  }
  
  vector<PostalInfo> postal_info = rsp->get_common_data().get_postal_info();
  string org;
  string str2;
  string str3;
  string sp;
  string pc;

  for (int i = 0; i < (int) postal_info.size(); i++) {
    if (postal_info[i].get_org() == "") {
      org = "";
    } else {
      org = "<contact:org>" + postal_info[i].get_org() + "</contact:org>";
    }
    if (postal_info[i].get_str2() == "") {
      str2 = "";
    } else {
      str2 = "<contact:street>" + postal_info[i].get_str2() +
	"</contact:street>";
    }
    if (postal_info[i].get_str3() == "") {
      str3 = "";
    } else {
      str3 = "<contact:street>" + postal_info[i].get_str3() +
	"</contact:street>";
    }
    if (postal_info[i].get_sp() == "") {
      sp = "";
    } else {
      sp = "<contact:sp>" + postal_info[i].get_sp() + "</contact:sp>";
    }
    if (postal_info[i].get_pc() == "") {
      pc = "";
    } else {
      pc = "<contact:pc>" + postal_info[i].get_pc() + "</contact:pc>";
    }
    
    response += "<contact:postalInfo type='" + postal_info[i].get_type() +
      "'>" + "<contact:name>" + postal_info[i].get_name() + "</contact:name>" +
      org + "<contact:addr><contact:street>" + postal_info[i].get_str1() +
      "</contact:street>" + str2 + str3 + "<contact:city>" +
      postal_info[i].get_city() + "</contact:city>" + sp + pc + "<contact:cc>"
      + postal_info[i].get_cc() +
      "</contact:cc></contact:addr></contact:postalInfo>";
  }
  
  CommonData::Phone voice;
  voice = rsp->get_common_data().get_voice();
  string voice_str;
  if (strcmp(voice.number.c_str(), "") == 0) {
    voice_str = "";
  }
  else {
    if (strcmp(voice.ext.c_str(), "") == 0)
      voice_str = "<contact:voice>" + voice.number + "</contact:voice>";
    else
      voice_str = "<contact:voice x='" + voice.ext + "'>" + 
	voice.number + "</contact:voice>";
    response += voice_str;
  }
  
  CommonData::Phone fax;
  fax = rsp->get_common_data().get_fax();
  string fax_str;
  if (strcmp(fax.number.c_str(), "") == 0) {
    fax_str = "";
  }
  else {
    if (strcmp(fax.ext.c_str(), "") == 0)
      fax_str = "<contact:fax>" + fax.number + "</contact:fax>";
    else
      fax_str = "<contact:fax x='" + fax.ext + "'>" + 
	fax.number + "</contact:fax>";
    response += fax_str;
  }
  
  response += "<contact:email>" + 
    rsp->get_common_data().get_email() + "</contact:email>"
    "<contact:clID>" + rsp->get_clID() + "</contact:clID>"
    "<contact:crID>" + rsp->get_crID() + "</contact:crID>"
    "<contact:crDate>" + rsp->get_crDate() + 
    "</contact:crDate>"
    "<contact:upID>" + rsp->get_upID() + "</contact:upID>"
    "<contact:upDate>" + rsp->get_upDate() + 
    "</contact:upDate>"
    "<contact:trDate>" + rsp->get_trDate() + 
    "</contact:trDate>";
  
  AuthInfo authInfo;
  authInfo = rsp->get_authInfo();
  string auth_info_str;
  response += "<contact:authInfo>";
  if (strcmp(authInfo.get_roid().c_str(), "") == 0) {
    response += "<contact:pw>" + authInfo.get_pw() + "</contact:pw>";
  } else {
    response += "<contact:pw roid='" + authInfo.get_roid() + "'>" + 
      authInfo.get_pw() + "</contact:pw>";
  }
  response += "</contact:authInfo>";
  
  disclose = rsp->get_common_data().get_disclose();
  
  string disclose_str("");
  if (disclose.name_int) {
    disclose_str += "<contact:name type =\"int\"/>";
  }
  if (disclose.name_loc) {
    disclose_str += "<contact:name type=\"loc\"/>";
  }
  if (disclose.org_int) {
    disclose_str += "<contact:org type=\"int\"/>";
  }
  if (disclose.org_loc) {
    disclose_str += "<contact:org type=\"loc\"/>";
  }
  if (disclose.addr_int) {
    disclose_str += "<contact:addr type=\"int\"/>";
  }
  if (disclose.addr_loc) {
    disclose_str += "<contact:addr type=\"loc\"/>";
  }
  if (disclose.voice) {
    disclose_str += "<contact:voice/>";
  }
  if (disclose.fax) {
    disclose_str += "<contact:fax/>";
  }
  if (disclose.email) {
    disclose_str += "<contact:email/>";
  }
  
  if (disclose_str != "") {
    disclose_str = "<contact:disclose flag='" + 
      StrUtil::to_string("%d", disclose.flag) + "'>" + 
      disclose_str + "</contact:disclose>";
  }
  
  response += disclose_str +
    "</contact:infData>"
    "</resData>"
    "<trID>"
    "<clTRID>" + rsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + rsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";
  
  CPPUNIT_ASSERT_EQUAL(expected, response);
  
  /* Reset Test */
  rsp->reset();
  
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_roid());
  CPPUNIT_ASSERT(rsp->get_status_set().empty());
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_clID());
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_crID());
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_crDate());
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_upID());
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_upDate());
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_trDate());
}
