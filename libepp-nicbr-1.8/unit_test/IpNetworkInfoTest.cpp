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
/* $Id: IpNetworkInfoTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#if USE_IP_MANAGEMENT
#include <memory>
#include <string>

#include "libepp_nicbr.H"

#include "IpNetworkInfoTest.H"
#include "IpNetworkInfo.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

#include "StrUtil.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(IpNetworkInfoTest);

IpNetworkInfoTest::IpNetworkInfoTest() {}

IpNetworkInfoTest::~IpNetworkInfoTest() {}

void IpNetworkInfoTest::setUp() {}

void IpNetworkInfoTest::tearDown() {}

void IpNetworkInfoTest::set_xml_template_test()
{
  string to_be_parsed = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<info>"
    "<ipnetwork:info "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 "
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:ipRange $(ipRange.version)$>"
    "<ipnetwork:startAddress>$(ipRange.begin)$</ipnetwork:startAddress>"
    "<ipnetwork:endAddress>$(ipRange.end)$</ipnetwork:endAddress>"
    "</ipnetwork:ipRange>"
    "<ipnetwork:roid>$(roid)$</ipnetwork:roid>"
    "</ipnetwork:info>"
    "</info>"
    "$(clTRID)$"
    "</command>"
    "</epp>";

  IpNetworkInfo ipNetworkInfo;
  IpNetworkInfoCmd *ipNetworkInfoCmd = ipNetworkInfo.get_command();
  
  IpRange ipRange;
  ipRange.set_version("v4");
  ipRange.set_ipBegin("192.168.0.0");
  ipRange.set_ipEnd("192.168.15.255");
  ipNetworkInfoCmd->set_ipRange(ipRange);
  ipNetworkInfoCmd->set_roid("b_123456-LACNIC");
  ipNetworkInfoCmd->set_clTRID("ABC-12345");
  ipNetworkInfo.set_xml_template(to_be_parsed);

  string expected = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<info>"
    "<ipnetwork:info "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 "
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:ipRange version='v4'>"
    "<ipnetwork:startAddress>192.168.0.0</ipnetwork:startAddress>"
    "<ipnetwork:endAddress>192.168.15.255</ipnetwork:endAddress>"
    "</ipnetwork:ipRange>"
    "<ipnetwork:roid>b_123456-LACNIC</ipnetwork:roid>"
    "</ipnetwork:info>"
    "</info>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";
  
  CPPUNIT_ASSERT_EQUAL(expected, ipNetworkInfo.get_xml());

  // tests reset method
  ipNetworkInfoCmd = ipNetworkInfo.get_command();
  ipNetworkInfoCmd->reset();
  CPPUNIT_ASSERT(ipNetworkInfoCmd->get_ipRange().get_version() == "");
  CPPUNIT_ASSERT(ipNetworkInfoCmd->get_ipRange().get_ipBegin() == "");
  CPPUNIT_ASSERT(ipNetworkInfoCmd->get_ipRange().get_ipEnd() == "");
  CPPUNIT_ASSERT(ipNetworkInfoCmd->get_roid() == "");
  
  // parent attribute
  CPPUNIT_ASSERT(ipNetworkInfoCmd->get_clTRID() == "");
}

void IpNetworkInfoTest::command_test()
{
  IpNetworkInfo ipNetworkInfo;
  IpNetworkInfoCmd *ipNetworkInfoCmd = ipNetworkInfo.get_command();

  IpRange ipRange;
  ipRange.set_version("v4");
  ipRange.set_ipBegin("192.168.0.0");
  ipRange.set_ipEnd("192.168.15.255");
  ipNetworkInfoCmd->set_ipRange(ipRange);
  ipNetworkInfoCmd->set_roid("b_123456-LACNIC");
  ipNetworkInfoCmd->set_clTRID("ABC-12345");

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/ipnetwork_info.xml");
    ipNetworkInfo.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(ipNetworkInfo.get_xml());
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

void IpNetworkInfoTest::response_test()
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
    "<ipnetwork:infData "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 "
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:ipRange version='v4'>"
    "<ipnetwork:startAddress>192.168.0.0</ipnetwork:startAddress>"
    "<ipnetwork:endAddress>192.168.15.255</ipnetwork:endAddress>"
    "</ipnetwork:ipRange>"
    "<ipnetwork:ipRangeInfo>"
    "<ipnetwork:roid>b_123456-LACNIC</ipnetwork:roid>"
    "<ipnetwork:allocType>allocation</ipnetwork:allocType>"
    "<ipnetwork:organization>BR-ABC-LACNIC</ipnetwork:organization>"
    "<ipnetwork:contact type='admin'>HKK</ipnetwork:contact>"
    "<ipnetwork:reverseDNS>"
    "<ipnetwork:ipRange version='v4'>"
    "<ipnetwork:startAddress>192.168.0.0</ipnetwork:startAddress>"
    "<ipnetwork:endAddress>192.168.0.255</ipnetwork:endAddress>"
    "</ipnetwork:ipRange>"
    "<ipnetwork:hostName>a.example.com</ipnetwork:hostName>"
    "<ipnetwork:hostName>b.example.com</ipnetwork:hostName>"
    "</ipnetwork:reverseDNS>"
    "<ipnetwork:reverseDNS>"
    "<ipnetwork:ipRange version='v4'>"
    "<ipnetwork:startAddress>192.168.2.0</ipnetwork:startAddress>"
    "<ipnetwork:endAddress>192.168.2.255</ipnetwork:endAddress>"
    "</ipnetwork:ipRange>"
    "<ipnetwork:hostName>d.example.com</ipnetwork:hostName>"
    "<ipnetwork:hostName>e.example.com</ipnetwork:hostName>"
    "</ipnetwork:reverseDNS>"
    "<ipnetwork:parentNetwork>"
    "<ipnetwork:ipRange version='v4'>"
    "<ipnetwork:startAddress>192.168.0.0</ipnetwork:startAddress>"
    "<ipnetwork:endAddress>192.168.255.255</ipnetwork:endAddress>"
    "</ipnetwork:ipRange>"
    "<ipnetwork:roid>b_12345-LACNIC</ipnetwork:roid>"
    "</ipnetwork:parentNetwork>"
    "<ipnetwork:childNetwork>"
    "<ipnetwork:ipRange version='v4'>"
    "<ipnetwork:startAddress>192.168.0.0</ipnetwork:startAddress>"
    "<ipnetwork:endAddress>192.168.0.127</ipnetwork:endAddress>"
    "</ipnetwork:ipRange>"
    "<ipnetwork:roid>b_234567-LACNIC</ipnetwork:roid>"
    "</ipnetwork:childNetwork>"
    "<ipnetwork:clID>ClientY</ipnetwork:clID>"
    "<ipnetwork:crID>ClientX</ipnetwork:crID>"
    "<ipnetwork:crDate>1999-04-03T22:00:00.0Z</ipnetwork:crDate>"
    "<ipnetwork:upID>ClientX</ipnetwork:upID>"
    "<ipnetwork:upDate>1999-12-03T09:00:00.0Z</ipnetwork:upDate>"
    "</ipnetwork:ipRangeInfo>"
    "</ipnetwork:infData>"
    "</resData>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54322-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  DomParser parser;
  parser.enable_validation("../docs/schemas");

  IpNetworkInfo ipNetworkInfo;
  CPPUNIT_ASSERT_NO_THROW(ipNetworkInfo.set_response(expected, &parser));

  IpNetworkInfoRsp *ipNetworkInfoRsp = ipNetworkInfo.get_response();

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = ipNetworkInfoRsp->get_result_list();
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
  
  string result_lang = ipNetworkInfoRsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response += ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<resData>"
    "<ipnetwork:infData "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 "
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:ipRange";

  IpRange ipRange = ipNetworkInfoRsp->get_ipRange();
  if (ipRange.get_version() != "") {
    response += " version='" + ipRange.get_version() + "'";
  }

  response += ">"
    "<ipnetwork:startAddress>" + ipRange.get_ipBegin() + 
    "</ipnetwork:startAddress>"
    "<ipnetwork:endAddress>" + ipRange.get_ipEnd() + "</ipnetwork:endAddress>"
    "</ipnetwork:ipRange>";
  
  vector<IpNetworkInfoRsp::IpRangeInfo> infos = 
    ipNetworkInfoRsp->get_ipRangeInfo();

  for (unsigned int i = 0; i < infos.size(); i++) {
    response += "<ipnetwork:ipRangeInfo>"
      "<ipnetwork:roid>" + infos[i].get_roid() + "</ipnetwork:roid>"
      "<ipnetwork:allocType>" + infos[i].get_allocType() + 
      "</ipnetwork:allocType>"
      "<ipnetwork:organization>" + infos[i].get_organization() + 
      "</ipnetwork:organization>";
      

    map< string, string, less<string> > contacts = infos[i].get_contacts();
    map< string, string, less<string> >::iterator it;
    for (it = contacts.begin(); it != contacts.end(); it++) {
      response += "<ipnetwork:contact type='" + it->first + "'>" + 
	it->second + "</ipnetwork:contact>";
    }
      
    vector<ReverseDns> reverses = infos[i].get_reversesDns();
    for (unsigned int j = 0; j < reverses.size(); j++) {
      response += "<ipnetwork:reverseDNS>"
	"<ipnetwork:ipRange";
      
      if (reverses[j].ipRange.get_version() != "") {
	response += " version='" + reverses[j].ipRange.get_version() + "'";
      }
      
      response += ">"
	"<ipnetwork:startAddress>" + reverses[j].ipRange.get_ipBegin() + 
	"</ipnetwork:startAddress>"
	"<ipnetwork:endAddress>" + reverses[j].ipRange.get_ipEnd() + 
	"</ipnetwork:endAddress>"
	"</ipnetwork:ipRange>";

      vector<string> ns = reverses[j].nameservers;
      for (unsigned int w = 0; w < ns.size(); w++) {
	response += "<ipnetwork:hostName>" + ns[w] + "</ipnetwork:hostName>";
      }

      response += "</ipnetwork:reverseDNS>";
    }

    IpNetworkInfoRsp::Network parentNetwork = infos[i].get_parentNetwork();
    if (parentNetwork.get_ipRange().get_ipBegin() != "" &&
	parentNetwork.get_ipRange().get_ipEnd() != "") {
      response += "<ipnetwork:parentNetwork>"
	"<ipnetwork:ipRange";
	
	string version = parentNetwork.get_ipRange().get_version();
	if (version != "") {
	  response += " version='" + version + "'";
	}

      response += ">"
	"<ipnetwork:startAddress>" + parentNetwork.get_ipRange().get_ipBegin() + 
	"</ipnetwork:startAddress>"
	"<ipnetwork:endAddress>" + parentNetwork.get_ipRange().get_ipEnd() + 
	"</ipnetwork:endAddress>"
	"</ipnetwork:ipRange>";

      if (parentNetwork.get_roid() != "") {
	response += "<ipnetwork:roid>" + parentNetwork.get_roid() + 
	  "</ipnetwork:roid>";
      }

      response += "</ipnetwork:parentNetwork>";
    }
    
    vector<IpNetworkInfoRsp::Network> childNetwork = 
      infos[i].get_childNetworks();
    for (unsigned int j = 0; j < childNetwork.size(); j++) {
      response += "<ipnetwork:childNetwork>"
	"<ipnetwork:ipRange";
      
      string version = childNetwork[j].get_ipRange().get_version();
      if (version != "") {
	response += " version='" + version + "'";
      }
      
      response += ">"
	"<ipnetwork:startAddress>" + 
	childNetwork[j].get_ipRange().get_ipBegin() + "</ipnetwork:startAddress>"
	"<ipnetwork:endAddress>" + childNetwork[j].get_ipRange().get_ipEnd() +
	"</ipnetwork:endAddress>"
	"</ipnetwork:ipRange>";

      if (childNetwork[j].get_roid() != "") {
	response += "<ipnetwork:roid>" + childNetwork[j].get_roid() + 
	  "</ipnetwork:roid>";
      }
      
      response += "</ipnetwork:childNetwork>";
    }

    response += "<ipnetwork:clID>" + infos[i].get_clId() + "</ipnetwork:clID>";

    if (infos[i].get_crId() != "") {
      response += "<ipnetwork:crID>" + infos[i].get_crId() + 
	"</ipnetwork:crID>";
    }
    
    if (infos[i].get_crDate() != "") {
      response += "<ipnetwork:crDate>" + infos[i].get_crDate() + 
	"</ipnetwork:crDate>";
    }

    if (infos[i].get_exDate() != "") {
      response += "<ipnetwork:exDate>" + infos[i].get_exDate() + 
	"</ipnetwork:exDate>";
    }
      
    if (infos[i].get_upId() != "") {
      response += "<ipnetwork:upID>" + infos[i].get_upId() + 
	"</ipnetwork:upID>";
    }

    if (infos[i].get_upDate() != "") {
      response += "<ipnetwork:upDate>" + infos[i].get_upDate() + 
	"</ipnetwork:upDate>";
    }

    if (infos[i].get_trDate() != "") {
      response += "<ipnetwork:trDate>" + infos[i].get_trDate() + 
	"</ipnetwork:trDate>";
	}

    response += "</ipnetwork:ipRangeInfo>";
  }

    
  response += "</ipnetwork:infData>"
    "</resData>"
    "<trID>"
    "<clTRID>" + ipNetworkInfoRsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + ipNetworkInfoRsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(expected, response);

  // tests reset method
  ipNetworkInfoRsp->reset();
  CPPUNIT_ASSERT_EQUAL((const string)"", 
		       ipNetworkInfoRsp->get_ipRange().get_version());
  CPPUNIT_ASSERT_EQUAL((const string)"", 
		       ipNetworkInfoRsp->get_ipRange().get_ipBegin());
  CPPUNIT_ASSERT_EQUAL((const string)"", 
		       ipNetworkInfoRsp->get_ipRange().get_ipEnd());
  CPPUNIT_ASSERT(ipNetworkInfoRsp->get_ipRangeInfo().empty());
}
#endif //USE_IP_MANAGEMENT
