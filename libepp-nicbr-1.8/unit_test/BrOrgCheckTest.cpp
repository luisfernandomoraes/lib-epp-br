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
/* $Id: BrOrgCheckTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#if USE_BR_ORG
#include <string>
#include <memory>
#include <iostream>

#include "libepp_nicbr.H"
#include "StrUtil.H"

#include "BrOrgCheckTest.H"
#include "BrOrgCheck.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

using std::cout;
using std::endl;


LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(BrOrgCheckTest);

BrOrgCheckTest::BrOrgCheckTest() {}

BrOrgCheckTest::~BrOrgCheckTest() {}

void BrOrgCheckTest::setUp() {}

void BrOrgCheckTest::tearDown() {}

void BrOrgCheckTest::set_xml_template_test()
{
  string to_be_parsed = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<check>"
    "<contact:check "
    "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd'>"
    "$(id_list)$"
    "</contact:check>"
    "</check>"
    "<extension>"
    "<brorg:check"
    "xmlns:brorg='urn:ietf:params:xml:ns:brorg-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brorg-1.0 "
    "brorg-1.0.xsd'>"
    "$(organization_list)$"
    "</brorg:check>"
    "</extension>"
    "$(clTRID)$"
    "</command>"
    "</epp>";


  BrOrgCheck brorg_check;
  BrOrgCheckCmd *brorg_check_cmd = brorg_check.get_command(); 

  brorg_check_cmd->insert_id("e123456");
  brorg_check_cmd->insert_id("e654321");
  brorg_check_cmd->insert_organization("e123456", "043.828.151/0001-45");
  brorg_check_cmd->insert_organization("e654321", "005.506.560/0001-36");

  brorg_check.get_command()->set_clTRID("ABC-12345");
  brorg_check.set_xml_template(to_be_parsed);

  string expected = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<check>"
    "<contact:check "
    "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd'>"
    "<contact:id>e123456</contact:id>"
    "<contact:id>e654321</contact:id>"
    "</contact:check>"
    "</check>"
    "<extension>"
    "<brorg:check"
    "xmlns:brorg='urn:ietf:params:xml:ns:brorg-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brorg-1.0 "
    "brorg-1.0.xsd'>"
    "<brorg:cd>"
    "<brorg:id>e123456</brorg:id>"
    "<brorg:organization>"
    "043.828.151/0001-45"
    "</brorg:organization>"
    "</brorg:cd>"
    "<brorg:cd>"
    "<brorg:id>e654321</brorg:id>"
    "<brorg:organization>"
    "005.506.560/0001-36"
    "</brorg:organization>"
    "</brorg:cd>"
    "</brorg:check>"
    "</extension>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(expected, brorg_check.get_xml());

  // tests reset method
  CPPUNIT_ASSERT(!brorg_check_cmd->get_id_list().empty());
  CPPUNIT_ASSERT(!brorg_check_cmd->get_organization_list().empty());
  
  brorg_check_cmd->reset();
  CPPUNIT_ASSERT(brorg_check_cmd->get_id_list().empty());
  CPPUNIT_ASSERT(brorg_check_cmd->get_organization_list().empty());
}

void BrOrgCheckTest::command_test()
{
  BrOrgCheck brorg_check;
  BrOrgCheckCmd *brorg_check_cmd = brorg_check.get_command();

  brorg_check_cmd->insert_id("btw214");
  brorg_check_cmd->insert_id("mrf348");
  brorg_check_cmd->insert_id("sht145");
  
  brorg_check_cmd->insert_organization("btw214", "043.828.151/0001-45");
  brorg_check_cmd->insert_organization("mrf348", "005.506.560/0001-36");
  brorg_check_cmd->insert_organization("sht145", "123.456.789/4321-00");
  
  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/br_org_check.xml");

    brorg_check.get_command()->set_clTRID("ABC-12345");
    brorg_check.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(brorg_check.get_xml());
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

void BrOrgCheckTest::response_test()
{
  string expected = 
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
    "<epp xmlns=\"urn:ietf:params:xml:ns:epp-1.0\" " 
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" " 
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:epp-1.0 epp-1.0.xsd\">"  
    "<response>"
    "<result code=\"1000\">"
    "<msg>Command completed successfully</msg>"
    "</result>"
    "<resData>"
    "<contact:chkData "
    "xmlns:contact=\"urn:ietf:params:xml:ns:contact-1.0\" " 
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd\">" 
    "<contact:cd>"
    "<contact:id avail=\"0\">004138888000184</contact:id>"
    "<contact:reason>In use</contact:reason>"
    "</contact:cd>"
    "<contact:cd>"
    "<contact:id avail=\"0\">006994175000148</contact:id>"
    "<contact:reason>Temporary organization in use</contact:reason>"
    "</contact:cd>"
    "<contact:cd>"
    "<contact:id avail=\"0\">067774281000100</contact:id>"
    "<contact:reason>Temporary organization in use</contact:reason>"
    "</contact:cd>"
    "</contact:chkData>"
    "</resData>"
    "<extension>"
    "<brorg:chkData "
    "xmlns:brorg=\"urn:ietf:params:xml:ns:brorg-1.0\" " 
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:brorg-1.0 "
    "brorg-1.0.xsd\">" 
    "<brorg:ticketInfo>"
    "<brorg:organization>006.994.175/0001-48</brorg:organization>"
    "<brorg:ticketNumber>2822407</brorg:ticketNumber>"
    "<brorg:domainName>doremisolfalasi.com.br</brorg:domainName>"
    "</brorg:ticketInfo>"
    "<brorg:ticketInfo>"
    "<brorg:organization>067.774.281/0001-00</brorg:organization>"
    "<brorg:ticketNumber>2822403</brorg:ticketNumber>"
    "<brorg:domainName>edpgviva.com.br</brorg:domainName>"
    "</brorg:ticketInfo>"
    "</brorg:chkData>"
    "</extension>"
    "<trID>"
    "<clTRID>424238335</clTRID>"
    "<svTRID>20060822152406-015-0011</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";
  
  
  DomParser parser;
  parser.enable_validation("../docs/schemas");
    
  BrOrgCheck br_org_check;
  
  try {
    br_org_check.set_response(expected, &parser);
  } catch (XmlException e) {
    printf("\nXml Exception: code [%d] message [%s] low level message [%s]\n",
	   e.get_code(), e.get_msg().c_str(), e.get_low_level_msg().c_str());
  }

  CPPUNIT_ASSERT_NO_THROW(br_org_check.set_response(expected, &parser));
  BrOrgCheckRsp* br_org_check_rsp = br_org_check.get_response();

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = br_org_check_rsp->get_result_list();
  r_it = results.begin();
  CPPUNIT_ASSERT(r_it != results.end());

  string response =
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
    "<epp xmlns=\"urn:ietf:params:xml:ns:epp-1.0\" "
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd\">"
    "<response>"
    "<result code=\"" +
    StrUtil::to_string("%d", r_it->first) + "\">"
    "<msg";
  
  string result_lang = br_org_check_rsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang=\"" + result_lang + "\"";
  }
  
  response +=
    ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<resData>"
    "<contact:chkData "
    "xmlns:contact=\"urn:ietf:params:xml:ns:contact-1.0\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd\">";

  map< string, ContactCheckRsp::Availability, less<string> > avail;
  map< string, ContactCheckRsp::Availability, less<string> >::const_iterator
    av_it;
  avail = br_org_check_rsp->get_availability_list();

  for (av_it = avail.begin(); av_it != avail.end(); av_it++) {
    response += "<contact:cd><contact:id avail=\"" +
      (*av_it).second.available + "\">" + (*av_it).first + "</contact:id>";
    if ((*av_it).second.reason != "") {
      response += "<contact:reason>" + (*av_it).second.reason + 
	"</contact:reason>";
    }
    response += "</contact:cd>";
  }
  response += "</contact:chkData>"
    "</resData>";

  //extension job
  response += "<extension>"
    "<brorg:chkData "
    "xmlns:brorg=\"urn:ietf:params:xml:ns:brorg-1.0\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:brorg-1.0 "
    "brorg-1.0.xsd\">";

  map< string, BrOrgCheckRsp::Unavailability, less<string> > unavail;
  map< string, BrOrgCheckRsp::Unavailability,
    less<string> >::const_iterator unav_it;
  unavail = br_org_check_rsp->get_unavailability_list();

  for (unav_it = unavail.begin(); unav_it != unavail.end(); unav_it++) {
    response += "<brorg:ticketInfo>"
      "<brorg:organization>" + unav_it->first +
      "</brorg:organization>";

    // ticketNumber
    if (unav_it->second.tkt_num != 0) {
      response += "<brorg:ticketNumber>" +
	StrUtil::to_string("%d", unav_it->second.tkt_num) + 
	"</brorg:ticketNumber>";
    }

    // domainName
    if (unav_it->second.tkt_fqdn != "") {
      response += "<brorg:domainName>" + unav_it->second.tkt_fqdn +
	"</brorg:domainName>";
    }
    response += "</brorg:ticketInfo>";
  }
  response += "</brorg:chkData></extension>";

  response +=
    "<trID>"
    "<clTRID>" + br_org_check_rsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + br_org_check_rsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(expected, response);

  //tests reset method
  br_org_check_rsp->reset();

  //parent attributes
  CPPUNIT_ASSERT(br_org_check_rsp->get_availability_list().empty());

  //BrOrgCheckRsp specific
  CPPUNIT_ASSERT(br_org_check_rsp->get_unavailability_list().empty());
} 
#endif //USE_BR_ORG
