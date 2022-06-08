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
/* $Id: IpNetworkUpdateTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#if USE_IP_MANAGEMENT
#include <memory>
#include <string>

#include "libepp_nicbr.H"

#include "IpNetworkUpdateTest.H"
#include "IpNetworkUpdate.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

#include "StrUtil.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(IpNetworkUpdateTest);

IpNetworkUpdateTest::IpNetworkUpdateTest() {}

IpNetworkUpdateTest::~IpNetworkUpdateTest() {}

void IpNetworkUpdateTest::setUp() {}

void IpNetworkUpdateTest::tearDown() {}

void IpNetworkUpdateTest::set_xml_template_test()
{
  string to_be_parsed = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<update>"
    "<ipnetwork:update "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 "
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:roid>$(roid)$</ipnetwork:roid>"
    "$(ipnetwork_add)$"
    "$(ipnetwork_rem)$"
    "$(ipnetwork_chg)$"
    "</ipnetwork:update>"
    "</update>"
    "$(clTRID)$"
    "</command>"
    "</epp>";

  IpNetworkUpdate ipNetworkUpdate;
  IpNetworkUpdateCmd *ipNetworkUpdateCmd = ipNetworkUpdate.get_command();
  
  ipNetworkUpdateCmd->set_roid("b_123456-LACNIC");
  ipNetworkUpdateCmd->insert_contact_add("admin", "AAA1");
  ipNetworkUpdateCmd->set_organization("BR-DEF-LACNIC");
  ipNetworkUpdateCmd->set_allocType("assignment");
  ipNetworkUpdateCmd->set_clTRID("ABC-12345");
  ipNetworkUpdate.set_xml_template(to_be_parsed);

  string expected = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<update>"
    "<ipnetwork:update "
    "xmlns:ipnetwork='urn:ietf:params:xml:ns:ipnetwork-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:ipnetwork-1.0 "
    "ipnetwork-1.0.xsd'>"
    "<ipnetwork:roid>b_123456-LACNIC</ipnetwork:roid>"
    "<ipnetwork:add>"
    "<ipnetwork:contact type='admin'>AAA1</ipnetwork:contact>"
    "</ipnetwork:add>"
    "<ipnetwork:chg>"
    "<ipnetwork:organization>BR-DEF-LACNIC</ipnetwork:organization>"
    "<ipnetwork:allocType>assignment</ipnetwork:allocType>"
    "</ipnetwork:chg>"
    "</ipnetwork:update>"
    "</update>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";
  
  CPPUNIT_ASSERT_EQUAL(expected, ipNetworkUpdate.get_xml());

  // tests reset method
  ipNetworkUpdateCmd = ipNetworkUpdate.get_command();
  ipNetworkUpdateCmd->reset();
  CPPUNIT_ASSERT(ipNetworkUpdateCmd->get_roid() == "");
  CPPUNIT_ASSERT(ipNetworkUpdateCmd->get_organization() == "");
  CPPUNIT_ASSERT(ipNetworkUpdateCmd->get_allocType() == "");
  CPPUNIT_ASSERT(ipNetworkUpdateCmd->get_contacts_add().empty());
  CPPUNIT_ASSERT(ipNetworkUpdateCmd->get_contacts_rem().empty());
  CPPUNIT_ASSERT(ipNetworkUpdateCmd->get_reverseDns_add().empty());
  CPPUNIT_ASSERT(ipNetworkUpdateCmd->get_reverseDns_rem().empty());
  
  // parent attribute
  CPPUNIT_ASSERT(ipNetworkUpdateCmd->get_clTRID() == "");
}

void IpNetworkUpdateTest::command_test()
{
  IpNetworkUpdate ipNetworkUpdate;
  IpNetworkUpdateCmd *ipNetworkUpdateCmd = ipNetworkUpdate.get_command();

  ipNetworkUpdateCmd->set_roid("b_123456-LACNIC");
  ipNetworkUpdateCmd->insert_contact_add("admin", "AAA1");
  ipNetworkUpdateCmd->set_organization("BR-DEF-LACNIC");
  ipNetworkUpdateCmd->set_allocType("assignment");
  ipNetworkUpdateCmd->set_clTRID("ABC-12345");

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/ipnetwork_update.xml");
    ipNetworkUpdate.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(ipNetworkUpdate.get_xml());
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

void IpNetworkUpdateTest::response_test()
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
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54321-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  DomParser parser;
  parser.enable_validation("../docs/schemas");

  IpNetworkUpdate ipNetworkUpdate;
  CPPUNIT_ASSERT_NO_THROW(ipNetworkUpdate.set_response(expected, &parser));
  Response *ipNetworkUpdateRsp = ipNetworkUpdate.get_response();

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = ipNetworkUpdateRsp->get_result_list();
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
  
  string result_lang = ipNetworkUpdateRsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response += ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<trID>"
    "<clTRID>" + ipNetworkUpdateRsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + ipNetworkUpdateRsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";
  
  CPPUNIT_ASSERT_EQUAL(expected, response);
}
#endif //USE_IP_MANAGEMENT
