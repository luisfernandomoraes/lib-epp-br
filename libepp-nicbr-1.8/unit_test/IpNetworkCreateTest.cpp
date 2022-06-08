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
/* $Id: IpNetworkCreateTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#if USE_IP_MANAGEMENT
#include <memory>
#include <string>

#include "libepp_nicbr.H"

#include "IpNetworkCreateTest.H"
#include "IpNetworkCreate.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

#include "StrUtil.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(IpNetworkCreateTest);

IpNetworkCreateTest::IpNetworkCreateTest() {}

IpNetworkCreateTest::~IpNetworkCreateTest() {}

void IpNetworkCreateTest::setUp() {}

void IpNetworkCreateTest::tearDown() {}

void IpNetworkCreateTest::set_xml_template_test()
{
  string to_be_parsed = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<create>"
    "<ipnetwork:create "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 "
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:ipRange $(ipRange.version)$>"
    "<ipnetwork:startAddress>$(ipRange.begin)$</ipnetwork:startAddress>"
    "<ipnetwork:endAddress>$(ipRange.end)$</ipnetwork:endAddress>"
    "</ipnetwork:ipRange>"
    "<ipnetwork:organization>$(organization)$</ipnetwork:organization>"
    "$(allocType)$"
    "$(asn)$"
    "$(contacts)$"
    "$(reversesDns)$"
    "</ipnetwork:create>"
    "</create>"
    "$(clTRID)$"
    "</command>"
    "</epp>";

  IpNetworkCreate ipNetworkCreate;
  IpNetworkCreateCmd *ipNetworkCreateCmd = ipNetworkCreate.get_command();
  
  IpRange ipRange;
  ipRange.set_version("v4");
  ipRange.set_ipBegin("192.168.16.0");
  ipRange.set_ipEnd("192.168.31.255");
  ipNetworkCreateCmd->set_ipRange(ipRange);

  ipNetworkCreateCmd->set_organization("BR-ABC-LACNIC");
  ipNetworkCreateCmd->set_allocType("assignment");
  ipNetworkCreateCmd->insert_contact("admin", "ABC123");
  
  ipRange.set_version("v4");
  ipRange.set_ipBegin("192.168.16.0");
  ipRange.set_ipEnd("192.168.17.255");
  
  ReverseDns reverseDns;
  reverseDns.ipRange = ipRange;
  reverseDns.nameservers.push_back("a.example.com");
  reverseDns.nameservers.push_back("b.example.com");
  ipNetworkCreateCmd->insert_reverseDns(reverseDns);

  ipNetworkCreateCmd->set_clTRID("ABC-12345");

  ipNetworkCreate.set_xml_template(to_be_parsed);

  string expected = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<create>"
    "<ipnetwork:create "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 "
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:ipRange version='v4'>"
    "<ipnetwork:startAddress>192.168.16.0</ipnetwork:startAddress>"
    "<ipnetwork:endAddress>192.168.31.255</ipnetwork:endAddress>"
    "</ipnetwork:ipRange>"
    "<ipnetwork:organization>BR-ABC-LACNIC</ipnetwork:organization>"
    "<ipnetwork:allocType>assignment</ipnetwork:allocType>"
    "<ipnetwork:contact type='admin'>ABC123</ipnetwork:contact>"
    "<ipnetwork:reverseDNS>"
    "<ipnetwork:ipRange version='v4'>"
    "<ipnetwork:startAddress>192.168.16.0</ipnetwork:startAddress>"
    "<ipnetwork:endAddress>192.168.17.255</ipnetwork:endAddress>"
    "</ipnetwork:ipRange>"
    "<ipnetwork:hostName>a.example.com</ipnetwork:hostName>"
    "<ipnetwork:hostName>b.example.com</ipnetwork:hostName>"
    "</ipnetwork:reverseDNS>"
    "</ipnetwork:create>"
    "</create>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";
  
  CPPUNIT_ASSERT_EQUAL(expected, ipNetworkCreate.get_xml());

  // tests reset method
  ipNetworkCreateCmd = ipNetworkCreate.get_command();
  ipNetworkCreateCmd->reset();
  CPPUNIT_ASSERT(ipNetworkCreateCmd->get_ipRange().get_version() == "");
  CPPUNIT_ASSERT(ipNetworkCreateCmd->get_ipRange().get_ipBegin() == "");
  CPPUNIT_ASSERT(ipNetworkCreateCmd->get_ipRange().get_ipEnd() == "");
  CPPUNIT_ASSERT(ipNetworkCreateCmd->get_organization() == "");
  CPPUNIT_ASSERT(ipNetworkCreateCmd->get_asn() == 0);
  CPPUNIT_ASSERT(ipNetworkCreateCmd->get_allocType() == "");
  CPPUNIT_ASSERT(ipNetworkCreateCmd->get_contacts().empty());
  CPPUNIT_ASSERT(ipNetworkCreateCmd->get_reversesDns().empty());
  
  // parent attribute
  CPPUNIT_ASSERT(ipNetworkCreateCmd->get_clTRID() == "");
}

void IpNetworkCreateTest::command_test()
{
  IpNetworkCreate ipNetworkCreate;
  IpNetworkCreateCmd *ipNetworkCreateCmd = ipNetworkCreate.get_command();

  IpRange ipRange;
  ipRange.set_version("v4");
  ipRange.set_ipBegin("192.168.16.0");
  ipRange.set_ipEnd("192.168.31.255");
  ipNetworkCreateCmd->set_ipRange(ipRange);

  ipNetworkCreateCmd->set_organization("BR-ABC-LACNIC");
  ipNetworkCreateCmd->set_allocType("assignment");
  ipNetworkCreateCmd->insert_contact("admin", "ABC123");
  ipNetworkCreateCmd->insert_contact("tech", "XXX");
  
  ipRange.set_version("v4");
  ipRange.set_ipBegin("192.168.16.0");
  ipRange.set_ipEnd("192.168.17.255");
  
  ReverseDns reverseDns;
  reverseDns.ipRange = ipRange;
  reverseDns.nameservers.push_back("a.example.com");
  reverseDns.nameservers.push_back("b.example.com");
  reverseDns.nameservers.push_back("c.example.com");
  ipNetworkCreateCmd->insert_reverseDns(reverseDns);
  
  ipNetworkCreateCmd->set_clTRID("ABC-12345");

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/ipnetwork_create.xml");
    ipNetworkCreate.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(ipNetworkCreate.get_xml());
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

void IpNetworkCreateTest::response_test()
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
    "<ipnetwork:creData "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 "
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:ipRange version='v4'>"
    "<ipnetwork:startAddress>192.168.16.0</ipnetwork:startAddress>"
    "<ipnetwork:endAddress>192.168.31.255</ipnetwork:endAddress>"
    "</ipnetwork:ipRange>"
    "<ipnetwork:roid>b_123456-LACNIC</ipnetwork:roid>"
    "<ipnetwork:crDate>1999-04-03T22:00:00.0Z</ipnetwork:crDate>"
    "</ipnetwork:creData>"
    "</resData>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54321-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  DomParser parser;
  parser.enable_validation("../docs/schemas");

  IpNetworkCreate ipNetworkCreate;
  CPPUNIT_ASSERT_NO_THROW(ipNetworkCreate.set_response(expected, &parser));
  IpNetworkCreateRsp *ipNetworkCreateRsp = ipNetworkCreate.get_response();

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = ipNetworkCreateRsp->get_result_list();
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
  
  string result_lang = ipNetworkCreateRsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response += ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<resData>"
    "<ipnetwork:creData "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 "
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:ipRange";

  IpRange ipRange = ipNetworkCreateRsp->get_ipRange();
  if (ipRange.get_version() != "") {
    response += " version='" + ipRange.get_version() + "'";
  }

  response += ">"
    "<ipnetwork:startAddress>" + ipRange.get_ipBegin() + 
    "</ipnetwork:startAddress>"
    "<ipnetwork:endAddress>" + ipRange.get_ipEnd() + 
    "</ipnetwork:endAddress>"
    "</ipnetwork:ipRange>"
    "<ipnetwork:roid>" + ipNetworkCreateRsp->get_roid() + 
    "</ipnetwork:roid>"
    "<ipnetwork:crDate>" + ipNetworkCreateRsp->get_crDate() + 
    "</ipnetwork:crDate>";

  if (ipNetworkCreateRsp->get_exDate() != "") {
    response += "<ipnetwork:exDate>" + ipNetworkCreateRsp->get_exDate() +
      "</ipnetwork:exDate>";
  }

  response += "</ipnetwork:creData>"
    "</resData>"
    "<trID>"
    "<clTRID>" + ipNetworkCreateRsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + ipNetworkCreateRsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(expected, response);

  // tests reset method
  ipNetworkCreateRsp->reset();
  CPPUNIT_ASSERT_EQUAL((const string)"", 
		       ipNetworkCreateRsp->get_ipRange().get_version());
  CPPUNIT_ASSERT_EQUAL((const string)"", 
		       ipNetworkCreateRsp->get_ipRange().get_ipBegin());
  CPPUNIT_ASSERT_EQUAL((const string)"", 
		       ipNetworkCreateRsp->get_ipRange().get_ipEnd());
  CPPUNIT_ASSERT_EQUAL((const string)"", ipNetworkCreateRsp->get_roid());
  CPPUNIT_ASSERT_EQUAL((const string)"", ipNetworkCreateRsp->get_crDate());
  CPPUNIT_ASSERT_EQUAL((const string)"", ipNetworkCreateRsp->get_exDate());
}
#endif //USE_IP_MANAGEMENT
