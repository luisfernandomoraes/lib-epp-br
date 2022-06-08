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
/* $Id: BrOrgInfoTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#if USE_BR_ORG
#include <string>
#include <memory>
#include <iostream>

#include "libepp_nicbr.H"
#include "StrUtil.H"

#include "BrOrgInfoTest.H"
#include "BrOrgInfo.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(BrOrgInfoTest);

BrOrgInfoTest::BrOrgInfoTest() {}

BrOrgInfoTest::~BrOrgInfoTest() {}

void BrOrgInfoTest::setUp() {}

void BrOrgInfoTest::tearDown() {}

void BrOrgInfoTest::set_xml_template_test()
{
  string to_be_parsed =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
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
    "<extension>"
    "<brorg:info "
    "xmlns:brorg='urn:ietf:params:xml:ns:brorg-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brorg-1.0 "
    "brorg-1.0.xsd'>"
    "<brorg:organization>"
    "$(organization)$"
    "</brorg:organization>"
    "</brorg:info>"
    "</extension>"
    "$(clTRID)$"
    "</command>"
    "</epp>";
    
  BrOrgInfo br_org_info;
  BrOrgInfoCmd* br_org_info_cmd = br_org_info.get_command();

  br_org_info_cmd->set_id("e654321");
  br_org_info_cmd->set_organization("005.506.560/0001-36");
    
  br_org_info.get_command()->set_clTRID("ABC-12345");
  br_org_info.set_xml_template(to_be_parsed);

  string expected =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<info>"
    "<contact:info "
    "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd'>"
    "<contact:id>e654321</contact:id>"
    "</contact:info>"
    "</info>"
    "<extension>"
    "<brorg:info "
    "xmlns:brorg='urn:ietf:params:xml:ns:brorg-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brorg-1.0 "
    "brorg-1.0.xsd'>"
    "<brorg:organization>"
    "005.506.560/0001-36"
    "</brorg:organization>"
    "</brorg:info>"
    "</extension>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(expected, br_org_info.get_xml());

  // tests reset method
  CPPUNIT_ASSERT(br_org_info_cmd->get_id() != "");
  br_org_info_cmd->reset();
  CPPUNIT_ASSERT(br_org_info_cmd->get_id() == "");
  CPPUNIT_ASSERT(br_org_info_cmd->get_authInfo().get_roid() == "");
  CPPUNIT_ASSERT(br_org_info_cmd->get_authInfo().get_pw() == "");
  CPPUNIT_ASSERT_EQUAL((string)"", br_org_info_cmd->get_organization());
}

void BrOrgInfoTest::command_test()
{
  BrOrgInfo br_org_info;
  BrOrgInfoCmd *br_org_info_cmd = br_org_info.get_command();

  br_org_info_cmd->set_id("cem456");

  AuthInfo authInfo;
  authInfo.set_roid("SH8013-REP");
  authInfo.set_pw("2fooBAR");
  br_org_info_cmd->set_authInfo(authInfo);
  
  br_org_info_cmd->set_organization("005.506.560/0001-36");

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/br_org_info.xml");

    br_org_info.get_command()->set_clTRID("ABC-12345");
    br_org_info.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(br_org_info.get_xml());
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

void BrOrgInfoTest::response_test()
{
  string expected =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<response>"
    "<result code='1000'>"
    "<msg>Command completed successfully</msg>"
    "</result>"
    "<resData>"
    "<contact:infData "
    "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd'>"
    "<contact:id>e654321</contact:id>"
    "<contact:roid>e654321-REP</contact:roid>"
    "<contact:status s='ok'/>"
    "<contact:postalInfo type='int'>"
    "<contact:name>John Doe</contact:name>"
    "<contact:org>Example Inc.</contact:org>"
    "<contact:addr>"
    "<contact:street>"
    "Av. Nações Unidas, 11541"
    "</contact:street>"
    "<contact:street>7º andar</contact:street>"
    "<contact:city>São Paulo</contact:city>"
    "<contact:sp>SP</contact:sp>"
    "<contact:pc>04578-000</contact:pc>"
    "<contact:cc>BR</contact:cc>"
    "</contact:addr>"
    "</contact:postalInfo>"
    "<contact:voice x='1234'>+55.1155093500</contact:voice>"
    "<contact:fax>+55.1155093501</contact:fax>"
    "<contact:email>jdoe@example.com.br</contact:email>"
    "<contact:clID>ClientY</contact:clID>"
    "<contact:crID>ClientX</contact:crID>"
    "<contact:crDate>2005-12-05T12:00:00.0Z</contact:crDate>"
    "<contact:upID>ClientX</contact:upID>"
    "<contact:upDate>2005-12-05T12:00:00.0Z</contact:upDate>"
    "<contact:disclose flag='0'>"
    "<contact:voice/>"
    "<contact:email/>"
    "</contact:disclose>"
    "</contact:infData>"
    "</resData>"
    "<extension>"
    "<brorg:infData "
    "xmlns:brorg='urn:ietf:params:xml:ns:brorg-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brorg-1.0 "
    "brorg-1.0.xsd'>"
    "<brorg:organization>"
    "005.506.560/0001-36"
    "</brorg:organization>"
    "<brorg:contact type='admin'>fan</brorg:contact>"
    "<brorg:responsible>João Cláudio da Silva</brorg:responsible>"
    "<brorg:proxy>EDS279</brorg:proxy>"
    "<brorg:exDate>2006-06-06T06:00:00.0Z</brorg:exDate>"
    "<brorg:domainName>nic.br</brorg:domainName>"
    "<brorg:domainName>ptt.br</brorg:domainName>"
    "<brorg:domainName>registro.br</brorg:domainName>"
    "<brorg:asNumber>64500</brorg:asNumber>"
    "<brorg:ipRange version='v4'>"
    "<brorg:startAddress>192.168.0.0</brorg:startAddress>"
    "<brorg:endAddress>192.168.0.255</brorg:endAddress>"
    "</brorg:ipRange>"
    "</brorg:infData>"
    "</extension>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54322-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  string utf8_expected;
  StrUtil::iso88591_to_utf8(expected, utf8_expected);

  DomParser parser;
  parser.enable_validation("../docs/schemas");
  
  BrOrgInfo br_org_info;
  CPPUNIT_ASSERT_NO_THROW(br_org_info.set_response(utf8_expected, &parser));
  
  BrOrgInfoRsp* rsp = br_org_info.get_response();
  
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
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
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
      "'>" + "<contact:name>" + postal_info[i].get_name() + "</contact:name>"
      + org + "<contact:addr><contact:street>" + postal_info[i].get_str1() +
      "</contact:street>" + str2 + str3 + "<contact:city>" +
      postal_info[i].get_city() + "</contact:city>" + sp + pc +
      "<contact:cc>" + postal_info[i].get_cc() +
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
    "</contact:upDate>";
  
  disclose = rsp->get_common_data().get_disclose();
  
  string disclose_str("");
  if (disclose.name_int) {
    disclose_str += "<contact:name type ='int'/>";
  }
  if (disclose.name_loc) {
    disclose_str += "<contact:name type='loc'/>";
  }
  if (disclose.org_int) {
    disclose_str += "<contact:org type='int'/>";
  }
  if (disclose.org_loc) {
    disclose_str += "<contact:org type='loc'/>";
  }
  if (disclose.addr_int) {
    disclose_str += "<contact:addr type='int'/>";
  }
  if (disclose.addr_loc) {
    disclose_str += "<contact:addr type='loc'/>";
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
    "</resData>";


  //BrOrgInfoRsp extension info
  response +=
    "<extension>"
    "<brorg:infData "
    "xmlns:brorg='urn:ietf:params:xml:ns:brorg-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brorg-1.0 "
    "brorg-1.0.xsd'>"
    
    //organization
    "<brorg:organization>" + rsp->get_organization() + "</brorg:organization>";

  //contacts
  map< string, string, less<string> > contacts;
  map< string, string, less<string> >::const_iterator it_contacts;
  contacts = rsp->get_contact_list();
  for (it_contacts = contacts.begin(); it_contacts != contacts.end();
       it_contacts++) {
    response += "<brorg:contact type='" + (*it_contacts).first + "'>" +
      (*it_contacts).second + "</brorg:contact>";
  }

  //responsible
  response +=  
    "<brorg:responsible>" + rsp->get_responsible() + "</brorg:responsible>";

  //proxy
  if (rsp->get_proxy() != "") {
    response += "<brorg:proxy>" + rsp->get_proxy() + "</brorg:proxy>";
  }

  // Expiration date
  if (rsp->get_exDate() != "") {
    response += "<brorg:exDate>" + rsp->get_exDate() + "</brorg:exDate>";
  }

  //domain list
  set<string> fqdns;
  set<string>::const_iterator it_fqdns;
  fqdns = rsp->get_domainName_list();
  for (it_fqdns = fqdns.begin(); it_fqdns != fqdns.end(); it_fqdns++) {
    response += "<brorg:domainName>" + (*it_fqdns) + "</brorg:domainName>";
  }

  // asn list
  set<int> asn = rsp->get_asn_list();
  set<int>::const_iterator it_asn;
  for (it_asn = asn.begin(); it_asn != asn.end(); it_asn++) {
    response += "<brorg:asNumber>" + StrUtil::to_string("%d", *it_asn)  + 
      "</brorg:asNumber>";
  }

  // ip range list
  set<IpRange> ipRanges = rsp->get_ipRange_list();
  set<IpRange>::const_iterator it_ipRanges;
  for (it_ipRanges = ipRanges.begin(); 
       it_ipRanges != ipRanges.end(); it_ipRanges++) {
    IpRange ipRange = *it_ipRanges;

    response += "<brorg:ipRange";
    if (ipRange.get_version() != "") {
      response += " version='" + ipRange.get_version() + "'";
    }
    
    response += ">"
      "<brorg:startAddress>" + ipRange.get_ipBegin() + 
      "</brorg:startAddress>"
      "<brorg:endAddress>" + ipRange.get_ipEnd() + "</brorg:endAddress>"
      "</brorg:ipRange>";
  }
  
  response += "</brorg:infData>"
    "</extension>";

  response += "<trID>"
    "<clTRID>" + rsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + rsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";
  
  CPPUNIT_ASSERT_EQUAL(expected, response);
  
  //reset test
  rsp->reset();
  
  //parent information
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_roid());
  CPPUNIT_ASSERT(rsp->get_status_set().empty());
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_clID());
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_crID());
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_crDate());
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_upID());
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_upDate());
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_trDate());

  //BrOrgInfoRsp specific information
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_organization());
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_responsible());
  CPPUNIT_ASSERT(rsp->get_contact_list().empty());
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_proxy());
  CPPUNIT_ASSERT_EQUAL((string) "", rsp->get_exDate());
  CPPUNIT_ASSERT(rsp->get_asn_list().empty());
  CPPUNIT_ASSERT(rsp->get_ipRange_list().empty());
}
#endif //USE_BR_ORG
