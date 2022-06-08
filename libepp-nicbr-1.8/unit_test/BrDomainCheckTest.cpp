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
/* $Id: BrDomainCheckTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#if USE_BR_DOMAINS
#include <string>
#include <memory>
#include <iostream>

#include "libepp_nicbr.H"
#include "StrUtil.H"

#include "BrDomainCheckTest.H"
#include "BrDomainCheck.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

using std::auto_ptr;

using std::cout;
using std::endl;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(BrDomainCheckTest);

BrDomainCheckTest::BrDomainCheckTest() {}

BrDomainCheckTest::~BrDomainCheckTest() {}

void BrDomainCheckTest::setUp() {}

void BrDomainCheckTest::tearDown() {}

void BrDomainCheckTest::set_xml_template_test()
{
  string to_be_parsed =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<check>"
    "<domain:check"
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "$(domains_list)$"
    "</domain:check>"
    "</check>"
    "$(extension)$"
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
    "<check>"
    "<domain:check"
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name>example.com.br</domain:name>"
    "<domain:name>example.net.br</domain:name>"
    "<domain:name>example.org.br</domain:name>"
    "</domain:check>"
    "</check>"
    "<extension>"
    "<brdomain:check "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>"
    "<brdomain:organization>"
    "005.506.560/0001-36"
    "</brdomain:organization>"
    "</brdomain:check>"
    "</extension>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";

  BrDomainCheck br_domain_check;
  BrDomainCheckCmd* br_domain_check_cmd = br_domain_check.get_command();
  br_domain_check_cmd->insert_domain("example.com.br");
  br_domain_check_cmd->insert_domain("example.net.br");
  br_domain_check_cmd->insert_domain("example.org.br");

  br_domain_check_cmd->set_organization("005.506.560/0001-36");

  br_domain_check.get_command()->set_clTRID("ABC-12345");
  br_domain_check.set_xml_template(to_be_parsed);

  CPPUNIT_ASSERT_EQUAL(expected, br_domain_check.get_xml());

  //tests reset method
  br_domain_check_cmd->reset();

  //parent attributes
  CPPUNIT_ASSERT(br_domain_check_cmd->get_domains_list().empty());

  //BrDomainCheckCmd specific attributes
  CPPUNIT_ASSERT_EQUAL((string)"", br_domain_check_cmd->get_organization());
}

void BrDomainCheckTest::command_test()
{
  BrDomainCheck br_domain_check;
  BrDomainCheckCmd *br_domain_check_cmd = br_domain_check.get_command();

  br_domain_check_cmd->insert_domain("registro.br");
  br_domain_check_cmd->insert_domain("du.eti.br");
  br_domain_check_cmd->insert_domain("nic.br");

  br_domain_check_cmd->set_organization("005.506.560/0001-36");

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/br_domain_check.xml");

    br_domain_check.get_command()->set_clTRID("ABC-12345");
    br_domain_check.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(br_domain_check.get_xml());
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

void BrDomainCheckTest::response_test()
{
  string expected=
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
    "<domain:chkData "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:cd>"
    "<domain:name avail='0'>e-xample.net.br</domain:name>"
    "<domain:reason>In use</domain:reason>"
    "</domain:cd>"
    "<domain:cd>"
    "<domain:name avail='1'>example.com.br</domain:name>"
    "</domain:cd>"
    "<domain:cd>"
    "<domain:name avail='1'>example.ind.br</domain:name>"
    "</domain:cd>"
    "<domain:cd>"
    "<domain:name avail='0'>example.org.br</domain:name>"
    "</domain:cd>"
    "</domain:chkData>"
    "</resData>"
    "<extension>"
    "<brdomain:chkData "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>"
    "<brdomain:cd>"
    "<brdomain:name>e-xample.net.br</brdomain:name>"
    "<brdomain:equivalentName>"
    "example.net.br"
    "</brdomain:equivalentName>"
    "<brdomain:organization>"
    "043.828.151/0001-45"
    "</brdomain:organization>"
    "</brdomain:cd>"
    "<brdomain:cd hasConcurrent='1'>"
    "<brdomain:name>example.com.br</brdomain:name>"
    "<brdomain:ticketNumber>123456</brdomain:ticketNumber>"
    "</brdomain:cd>"
    "<brdomain:cd inReleaseProcess='1'>"
    "<brdomain:name>example.ind.br</brdomain:name>"
    "</brdomain:cd>"
    "<brdomain:cd>"
    "<brdomain:name>example.org.br</brdomain:name>"
    "<brdomain:organization>"
    "043.828.151/0001-45"
    "</brdomain:organization>"
    "</brdomain:cd>"
    "</brdomain:chkData>"
    "</extension>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54322-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  DomParser parser;
  parser.enable_validation("../docs/schemas");

  BrDomainCheck br_domain_check;

  try {
    br_domain_check.set_response(expected, &parser);
  } catch (XmlException e) {
    cout << "[" << e.get_low_level_msg() << "]" << endl;
  }

  CPPUNIT_ASSERT_NO_THROW(br_domain_check.set_response(expected, &parser));
  BrDomainCheckRsp* br_domain_check_rsp = br_domain_check.get_response();

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = br_domain_check_rsp->get_result_list();
  r_it = results.begin();
  CPPUNIT_ASSERT(r_it != results.end());

  string response =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<response>"
    "<result code='" +
    StrUtil::to_string("%d", r_it->first) + "'>"
    "<msg";
  
  string result_lang = br_domain_check_rsp->get_result_lang();
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
  map< string, DomainCheckRsp::Availability, less<string> >::const_iterator
    av_it;
  avail = br_domain_check_rsp->get_availability_list();

  for (av_it = avail.begin(); av_it != avail.end(); av_it++) {
    response += "<domain:cd><domain:name avail='" +
      (*av_it).second.available + "'>" + (*av_it).first + "</domain:name>";
    if ((*av_it).second.reason != "") {
      response += "<domain:reason>" + (*av_it).second.reason + 
	"</domain:reason>";
    }
    response += "</domain:cd>";
  }
  response += "</domain:chkData>"
    "</resData>";

  //extension job
  response += "<extension>"
    "<brdomain:chkData "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>";

  map< string, BrDomainCheckRsp::Unavailability, less<string> > unavail;
  map< string, BrDomainCheckRsp::Unavailability,
    less<string> >::const_iterator unav_it;
  unavail = br_domain_check_rsp->get_unavailability_list();

  for (unav_it = unavail.begin(); unav_it != unavail.end(); unav_it++) {
    response += "<brdomain:cd";

    //attributes
    if ((*unav_it).second.hasConcurrent &&
	(*unav_it).second.inReleaseProcess) {
      response += " hasConcurrent='1' inReleaseProcess='1'";
    } else if ((*unav_it).second.hasConcurrent) {
      response += " hasConcurrent='1'";
    } else if ((*unav_it).second.inReleaseProcess) {
      response += " inReleaseProcess='1'";
    }

    response += ">"
      "<brdomain:name>" + (*unav_it).first +
      "</brdomain:name>";

    // equivalentName
    if ((*unav_it).second.equivalentName != "") {
      response += "<brdomain:equivalentName>" +
	(*unav_it).second.equivalentName + "</brdomain:equivalentName>";
    }

    // organization
    if ((*unav_it).second.organization != "") {
      response += "<brdomain:organization>" + (*unav_it).second.organization +
	"</brdomain:organization>";
    }

    // ticket set
    set<int> tickets = (*unav_it).second.tickets;
    set<int>::const_iterator tkt_it;

    for (tkt_it = tickets.begin(); tkt_it != tickets.end(); tkt_it++) {
      response += "<brdomain:ticketNumber>" +
	StrUtil::to_string("%d",(*tkt_it)) + "</brdomain:ticketNumber>";
    }

    response += "</brdomain:cd>";
  }
  response += "</brdomain:chkData></extension>";

  response +=
    "<trID>"
    "<clTRID>" + br_domain_check_rsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + br_domain_check_rsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(expected, response);

  //tests reset method
  br_domain_check_rsp->reset();

  //parent attributes
  CPPUNIT_ASSERT(br_domain_check_rsp->get_availability_list().empty());

  //BrDomainCheckRsp specific
  CPPUNIT_ASSERT(br_domain_check_rsp->get_unavailability_list().empty());
}
#endif //USE_BR_DOMAINS
