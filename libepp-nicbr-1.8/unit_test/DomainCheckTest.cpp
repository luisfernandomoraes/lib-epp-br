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
/* $Id: DomainCheckTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#include <memory>
#include <string>

#include "libepp_nicbr.H"

#include "DomainCheckTest.H"
#include "DomainCheck.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(DomainCheckTest);

DomainCheckTest::DomainCheckTest() {}

DomainCheckTest::~DomainCheckTest() {}

void DomainCheckTest::setUp() {}

void DomainCheckTest::tearDown() {}

void DomainCheckTest::set_xml_template_test()
{
  string to_be_parsed =
    "<command>"
    "<check>"
    "<domain:check"
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0'"
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0"
    "domain-1.0.xsd'>"
    "$(domains_list)$"
    "</domain:check>"
    "</check>"
    "$(clTRID)$"
    "</command>";

  DomainCheck domain_check;
  DomainCheckCmd* domain_check_cmd = domain_check.get_command();
  domain_check_cmd->insert_domain("registro.br");
  domain_check_cmd->insert_domain("du.eti.br");
  domain_check_cmd->insert_domain("nic.br");

  domain_check.get_command()->set_clTRID("ABC-12345");
  domain_check.set_xml_template(to_be_parsed);

  string expected =
    "<command>"
    "<check>"
    "<domain:check"
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0'"
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0"
    "domain-1.0.xsd'>"
    "<domain:name>du.eti.br</domain:name>"
    "<domain:name>nic.br</domain:name>"
    "<domain:name>registro.br</domain:name>"
    "</domain:check>"
    "</check>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>";

  CPPUNIT_ASSERT_EQUAL(expected, domain_check.get_xml());

  // tests reset method
  domain_check_cmd->reset();
  CPPUNIT_ASSERT(domain_check_cmd->get_domains_list().empty());

  // parent attribute
  CPPUNIT_ASSERT(domain_check_cmd->get_clTRID() == "");
}

void DomainCheckTest::command_test()
{
  DomainCheck domain_check;
  DomainCheckCmd *domain_check_cmd = domain_check.get_command();

  domain_check_cmd->insert_domain("registro.br");
  domain_check_cmd->insert_domain("du.eti.br");
  domain_check_cmd->insert_domain("nic.br");

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/domain_check.xml");

    domain_check.get_command()->set_clTRID("ABC-12345");
    domain_check.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(domain_check.get_xml());
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

void DomainCheckTest::response_test()
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
    "<domain:chkData "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:cd>"
    "<domain:name avail='0'>du.eti.br</domain:name>"
    "<domain:reason>In use</domain:reason>"
    "</domain:cd>"
    "<domain:cd>"
    "<domain:name avail='1'>nic.br</domain:name>"
    "</domain:cd>"
    "<domain:cd>"
    "<domain:name avail='1'>registro.br</domain:name>"
    "</domain:cd>"
    "</domain:chkData>"
    "</resData>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54322-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  DomParser parser;
  parser.enable_validation("../docs/schemas");

  DomainCheck domain_check;

  CPPUNIT_ASSERT_NO_THROW(domain_check.set_response(expected, &parser));
  DomainCheckRsp* domain_check_rsp = domain_check.get_response();

  string result_code = "1000";

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = domain_check_rsp->get_result_list();
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
  
  string result_lang = domain_check_rsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response +=
    ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<resData>"
    "<domain:chkData "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>";

  map< string, DomainCheckRsp::Availability, less<string> > avail;
  map< string, DomainCheckRsp::Availability, less<string> >::const_iterator it;
  avail = domain_check_rsp->get_availability_list();

  for (it = avail.begin(); it != avail.end(); it++) {
    response += "<domain:cd><domain:name avail='" +
      (*it).second.available + "'>" + (*it).first + "</domain:name>";
    if (strcmp((*it).second.reason.c_str(), "") != 0) {
      response += "<domain:reason>" + (*it).second.reason + 
	"</domain:reason>";
    }
    response += "</domain:cd>";
  }
  response += "</domain:chkData>"
    "</resData>"
    "<trID>"
    "<clTRID>" + domain_check_rsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + domain_check_rsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(expected, response);

  // tests reset method
  CPPUNIT_ASSERT(!domain_check_rsp->get_availability_list().empty());
  domain_check_rsp->reset();
  CPPUNIT_ASSERT(domain_check_rsp->get_availability_list().empty());
}
