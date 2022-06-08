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
/* $Id: IpNetworkTransferTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#if USE_IP_MANAGEMENT
#include <memory>
#include <string>

#include "libepp_nicbr.H"

#include "IpNetworkTransferTest.H"
#include "IpNetworkTransfer.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

#include "StrUtil.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(IpNetworkTransferTest);

IpNetworkTransferTest::IpNetworkTransferTest() {}

IpNetworkTransferTest::~IpNetworkTransferTest() {}

void IpNetworkTransferTest::setUp() {}

void IpNetworkTransferTest::tearDown() {}

void IpNetworkTransferTest::set_xml_template_test()
{
  string to_be_parsed = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<transfer op='$(op)$'>"
    "<ipnetwork:transfer "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 " 
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:roid>$(roid)$</ipnetwork:roid>"
    "</ipnetwork:transfer>"
    "</transfer>"
    "$(clTRID)$"
    "</command>"
    "</epp>";

  IpNetworkTransfer ipNetworkTransfer;
  IpNetworkTransferCmd *ipNetworkTransferCmd = ipNetworkTransfer.get_command();
  ipNetworkTransferCmd->set_operation("request");
  ipNetworkTransferCmd->set_roid("b_12345-LACNIC");
  ipNetworkTransferCmd->set_clTRID("ABC-12345");
  ipNetworkTransfer.set_xml_template(to_be_parsed);

  string expected = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<transfer op='request'>"
    "<ipnetwork:transfer "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 " 
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:roid>b_12345-LACNIC</ipnetwork:roid>"
    "</ipnetwork:transfer>"
    "</transfer>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";
  
  CPPUNIT_ASSERT_EQUAL(expected, ipNetworkTransfer.get_xml());

  // tests reset method
  ipNetworkTransferCmd = ipNetworkTransfer.get_command();
  ipNetworkTransferCmd->reset();
  CPPUNIT_ASSERT(ipNetworkTransferCmd->get_operation() == "");
  CPPUNIT_ASSERT(ipNetworkTransferCmd->get_roid() == "");
  
  // parent attribute
  CPPUNIT_ASSERT(ipNetworkTransferCmd->get_clTRID() == "");
}

void IpNetworkTransferTest::command_test()
{
  IpNetworkTransfer ipNetworkTransfer;
  IpNetworkTransferCmd *ipNetworkTransferCmd = ipNetworkTransfer.get_command();

  ipNetworkTransferCmd->set_operation("request");
  ipNetworkTransferCmd->set_roid("b_12345-LACNIC");
  ipNetworkTransferCmd->set_clTRID("ABC-12345");

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/ipnetwork_transfer.xml");
    ipNetworkTransfer.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(ipNetworkTransfer.get_xml());
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

void IpNetworkTransferTest::response_test()
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
    "<ipnetwork:trnData "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 " 
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:roid>b_12345-LACNIC</ipnetwork:roid>"
    "<ipnetwork:trStatus>pending</ipnetwork:trStatus>"
    "<ipnetwork:reID>ClientX</ipnetwork:reID>"
    "<ipnetwork:reDate>2000-06-08T22:00:00.0Z</ipnetwork:reDate>"
    "<ipnetwork:acID>ClientY</ipnetwork:acID>"
    "<ipnetwork:acDate>2000-06-13T22:00:00.0Z</ipnetwork:acDate>"
    "</ipnetwork:trnData>"
    "</resData>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54322-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  DomParser parser;
  parser.enable_validation("../docs/schemas");

  IpNetworkTransfer ipNetworkTransfer;
  CPPUNIT_ASSERT_NO_THROW(ipNetworkTransfer.set_response(expected, &parser));
  IpNetworkTransferRsp *ipNetworkTransferRsp = ipNetworkTransfer.get_response();

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = ipNetworkTransferRsp->get_result_list();
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
  
  string result_lang = ipNetworkTransferRsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response += ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<resData>"
    "<ipnetwork:trnData "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 " 
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:roid>" + ipNetworkTransferRsp->get_roid() + "</ipnetwork:roid>"
    "<ipnetwork:trStatus>" + ipNetworkTransferRsp->get_trStatus() + 
    "</ipnetwork:trStatus>"
    "<ipnetwork:reID>" + ipNetworkTransferRsp->get_reId() + "</ipnetwork:reID>"
    "<ipnetwork:reDate>" + ipNetworkTransferRsp->get_reDate() + 
    "</ipnetwork:reDate>"
    "<ipnetwork:acID>" + ipNetworkTransferRsp->get_acId() + "</ipnetwork:acID>"
    "<ipnetwork:acDate>" + ipNetworkTransferRsp->get_acDate() + 
    "</ipnetwork:acDate>"
    "</ipnetwork:trnData>"
    "</resData>"
    "<trID>"
    "<clTRID>" + ipNetworkTransferRsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + ipNetworkTransferRsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(expected, response);

  // tests reset method
  ipNetworkTransferRsp->reset();
  CPPUNIT_ASSERT_EQUAL((const string)"", ipNetworkTransferRsp->get_roid());
  CPPUNIT_ASSERT_EQUAL((const string)"", ipNetworkTransferRsp->get_trStatus());
  CPPUNIT_ASSERT_EQUAL((const string)"", ipNetworkTransferRsp->get_reId());
  CPPUNIT_ASSERT_EQUAL((const string)"", ipNetworkTransferRsp->get_reDate());
  CPPUNIT_ASSERT_EQUAL((const string)"", ipNetworkTransferRsp->get_acId());
  CPPUNIT_ASSERT_EQUAL((const string)"", ipNetworkTransferRsp->get_acDate());
}
#endif //USE_IP_MANAGEMENT
