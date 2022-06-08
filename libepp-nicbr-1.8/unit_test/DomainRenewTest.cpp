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
/* $Id: DomainRenewTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#include <memory>
#include <string>

#include "libepp_nicbr.H"

#include "DomainRenewTest.H"
#include "DomainRenew.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(DomainRenewTest);

DomainRenewTest::DomainRenewTest() {}

DomainRenewTest::~DomainRenewTest() {}

void DomainRenewTest::setUp() {}

void DomainRenewTest::tearDown() {}

void DomainRenewTest::set_xml_template_test()
{
  string to_be_parsed =
    "<command>"
    "<renew>"
    "<domain:renew"
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0'"
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0"
    "domain-1.0.xsd'>"
    "<domain:name>$(name)$</domain:name>"
    "<domain:curExpDate>$(curExpDate)$</domain:curExpDate>"
    "$(period)$"
    "</domain:renew>"
    "</renew>"
    "$(clTRID)$"
    "</command>";

  DomainRenew domain_renew;
  DomainRenewCmd* domain_renew_cmd = domain_renew.get_command();
  domain_renew_cmd->set_name("asultimasnuvensazuisnoceudaalamedaprincipal.org");
  domain_renew_cmd->set_curExpDate("2000-04-03");
  domain_renew_cmd->set_period(12, "m");

  domain_renew.get_command()->set_clTRID("ABC-12345");
  domain_renew.set_xml_template(to_be_parsed);


  string expected =
    "<command>"
    "<renew>"
    "<domain:renew"
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0'"
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0"
    "domain-1.0.xsd'>"
    "<domain:name>asultimasnuvensazuisnoceudaalamedaprincipal.org</domain:name>"
    "<domain:curExpDate>2000-04-03</domain:curExpDate>"
    "<domain:period unit='m'>12</domain:period>"
    "</domain:renew>"
    "</renew>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>";
  
  CPPUNIT_ASSERT(expected == domain_renew.get_xml());

  // tests reset method
  domain_renew_cmd = domain_renew.get_command();
  domain_renew_cmd->reset();
  CPPUNIT_ASSERT(domain_renew_cmd->get_name() == "");
  CPPUNIT_ASSERT(domain_renew_cmd->get_curExpDate() == "");
  CPPUNIT_ASSERT(domain_renew_cmd->get_period().time == 0);
  CPPUNIT_ASSERT(domain_renew_cmd->get_period().unit == "");

  // parent attribute
  CPPUNIT_ASSERT(domain_renew_cmd->get_clTRID() == "");
}

void DomainRenewTest::command_test()
{
  DomainRenew domain_renew;
  DomainRenewCmd *domain_renew_cmd = domain_renew.get_command();

  domain_renew_cmd->set_name("asultimasnuvensazuisnoceudaalamedaprincipal.org");
  domain_renew_cmd->set_curExpDate("2000-04-03");
  domain_renew_cmd->set_period(12, "m");

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/domain_renew.xml");

    domain_renew.get_command()->set_clTRID("ABC-12345");
    domain_renew.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(domain_renew.get_xml());
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

void DomainRenewTest::response_test()
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
    "<domain:renData "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name>asultimasnuvensazuisnoceudaalamedaprincipal.org"
    "</domain:name>"
    "<domain:exDate>2001-04-03T22:00:00.0Z</domain:exDate>"
    "</domain:renData>"
    "</resData>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54321-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  DomParser parser;
  parser.enable_validation("../docs/schemas");

  DomainRenew domain_renew;
  CPPUNIT_ASSERT_NO_THROW(domain_renew.set_response(expected, &parser));
  
  string result_code = "1000";
  DomainRenewRsp* domain_renew_rsp = domain_renew.get_response();
  
  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = domain_renew_rsp->get_result_list();
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
  
  string result_lang = domain_renew_rsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response +=
    ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<resData>"
    "<domain:renData "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name>" + domain_renew_rsp->get_name() + 
    "</domain:name>"
    "<domain:exDate>" + domain_renew_rsp->get_exDate() + "</domain:exDate>"
    "</domain:renData>"
    "</resData>"
    "<trID>"
    "<clTRID>" + domain_renew_rsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + domain_renew_rsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";
  
  CPPUNIT_ASSERT_EQUAL(expected, response);

  // tests reset method
  domain_renew_rsp->reset();
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_renew_rsp->get_name());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_renew_rsp->get_exDate());
}
