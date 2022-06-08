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
/* $Id: IpNetworkRenewTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#if USE_IP_MANAGEMENT
#include <memory>
#include <string>

#include "libepp_nicbr.H"

#include "IpNetworkRenewTest.H"
#include "IpNetworkRenew.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

#include "StrUtil.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(IpNetworkRenewTest);

IpNetworkRenewTest::IpNetworkRenewTest() {}

IpNetworkRenewTest::~IpNetworkRenewTest() {}

void IpNetworkRenewTest::setUp() {}

void IpNetworkRenewTest::tearDown() {}

void IpNetworkRenewTest::set_xml_template_test()
{
  string to_be_parsed = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<renew>"
    "<ipnetwork:renew "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 "
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:roid>$(roid)$</ipnetwork:roid>"
    "<ipnetwork:curExpDate>"
    "$(expDate)$"
    "</ipnetwork:curExpDate>"
    "$(period)$"
    "</ipnetwork:renew>"
    "</renew>"
    "$(clTRID)$"
    "</command>"
    "</epp>";

  IpNetworkRenew ipNetworkRenew;
  IpNetworkRenewCmd *ipNetworkRenewCmd = ipNetworkRenew.get_command();
  ipNetworkRenewCmd->set_roid("b_12345-LACNIC");
  ipNetworkRenewCmd->set_expDate("2008-04-03T00:00:00.0Z");
  ipNetworkRenewCmd->set_period(3, "y");
  ipNetworkRenewCmd->set_clTRID("ABC-12345");

  ipNetworkRenew.set_xml_template(to_be_parsed);

  string expected = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<renew>"
    "<ipnetwork:renew "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 "
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:roid>b_12345-LACNIC</ipnetwork:roid>"
    "<ipnetwork:curExpDate>"
    "2008-04-03T00:00:00.0Z"
    "</ipnetwork:curExpDate>"
    "<ipnetwork:period unit='y'>3</ipnetwork:period>"
    "</ipnetwork:renew>"
    "</renew>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";
  
  CPPUNIT_ASSERT_EQUAL(expected, ipNetworkRenew.get_xml());

  // tests reset method
  ipNetworkRenewCmd = ipNetworkRenew.get_command();
  ipNetworkRenewCmd->reset();
  CPPUNIT_ASSERT(ipNetworkRenewCmd->get_roid() == "");
  CPPUNIT_ASSERT(ipNetworkRenewCmd->get_expDate() == "");
  CPPUNIT_ASSERT(ipNetworkRenewCmd->get_period().time == 0);
  CPPUNIT_ASSERT(ipNetworkRenewCmd->get_period().unit == "");
  
  // parent attribute
  CPPUNIT_ASSERT(ipNetworkRenewCmd->get_clTRID() == "");
}

void IpNetworkRenewTest::command_test()
{
  IpNetworkRenew ipNetworkRenew;
  IpNetworkRenewCmd *ipNetworkRenewCmd = ipNetworkRenew.get_command();

  ipNetworkRenewCmd->set_roid("b_12345-LACNIC");
  ipNetworkRenewCmd->set_expDate("2008-04-03T00:00:00.0Z");
  ipNetworkRenewCmd->set_period(3, "y");
  ipNetworkRenewCmd->set_clTRID("ABC-12345");

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/ipnetwork_renew.xml");
    ipNetworkRenew.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(ipNetworkRenew.get_xml());
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

void IpNetworkRenewTest::response_test()
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
    "<ipnetwork:renData "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 "
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:roid>b_12345-LACNIC</ipnetwork:roid>"
    "<ipnetwork:exDate>2011-04-03T00:00:00.0Z</ipnetwork:exDate>"
    "</ipnetwork:renData>"
    "</resData>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54322-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  DomParser parser;
  parser.enable_validation("../docs/schemas");

  IpNetworkRenew ipNetworkRenew;
  CPPUNIT_ASSERT_NO_THROW(ipNetworkRenew.set_response(expected, &parser));
  IpNetworkRenewRsp *ipNetworkRenewRsp = ipNetworkRenew.get_response();

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = ipNetworkRenewRsp->get_result_list();
  r_it = results.begin();

  CPPUNIT_ASSERT(r_it != results.end());
  CPPUNIT_ASSERT_EQUAL(Response::OK, r_it->first);

  string result_code =
    StrUtil::to_string("%d", (int)r_it->first);

  string response =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<response>"
    "<result code='1000'>"
    "<msg";
  
  string result_lang = ipNetworkRenewRsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response += ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<resData>"
    "<ipnetwork:renData "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 "
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:roid>" + ipNetworkRenewRsp->get_roid() + "</ipnetwork:roid>"
    "<ipnetwork:exDate>" + ipNetworkRenewRsp->get_expDate() + 
    "</ipnetwork:exDate>"
    "</ipnetwork:renData>"
    "</resData>"
    "<trID>"
    "<clTRID>" + ipNetworkRenewRsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + ipNetworkRenewRsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(expected, response);

  // tests reset method
  ipNetworkRenewRsp->reset();
  CPPUNIT_ASSERT_EQUAL((const string)"", ipNetworkRenewRsp->get_roid());
  CPPUNIT_ASSERT_EQUAL((const string)"", ipNetworkRenewRsp->get_expDate());
}
#endif //USE_IP_MANAGEMENT
