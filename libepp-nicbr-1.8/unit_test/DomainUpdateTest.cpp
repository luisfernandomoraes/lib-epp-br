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
/* $Id: DomainUpdateTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#include <memory>
#include <string>

#include "libepp_nicbr.H"

#include "DomainUpdateTest.H"
#include "DomainUpdate.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(DomainUpdateTest);

DomainUpdateTest::DomainUpdateTest() {}

DomainUpdateTest::~DomainUpdateTest() {}

void DomainUpdateTest::setUp() {}

void DomainUpdateTest::tearDown() {}

void DomainUpdateTest::set_xml_template_test()
{
  set_xml_template_test(true);
  set_xml_template_test(false);
}

void DomainUpdateTest::set_xml_template_test(bool withDNSSECExtension)
{
  string to_be_parsed =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<update>"
    "<domain:update "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name>$(name)$</domain:name>"
    "$(add)$"
    "$(rem)$"
    "$(chg)$"
    "</domain:update>"
    "</update>"
    "$(ext_begin)$"
    "$(ds_ext)$"
    "$(ext_end)$"
    "$(clTRID)$"
    "</command>"
    "</epp>";

  DomainUpdate domain_update;
  DomainUpdateCmd* domain_update_cmd = domain_update.get_command();
  domain_update_cmd->set_name("example.org");
  
  struct NameServer ns;
  struct NSIPAddr ip;

  ns.name = "ns0.example.org";
  domain_update_cmd->insert_nameserver_add(ns);

  ns.name = "ns1.example.org";
  domain_update_cmd->insert_nameserver_add(ns);

  ns.name = "ns3.example.org";
  ip.addr = "192.168.0.1";
  ns.ips.insert(ip);
  domain_update_cmd->insert_nameserver_add(ns);

  ns.ips.clear();
  ns.name = "ns2.example.org";
  domain_update_cmd->insert_nameserver_rem(ns);

  domain_update_cmd->insert_contact_add("tech", "XXX");
  domain_update_cmd->insert_contact_rem("admin", "ABC");

  struct DomainUpdateCmd::Status st;
  st.s = "clientHold";
  st.lang = "en";
  st.msg = "Payment overdue.";
  domain_update_cmd->insert_status_add(st);

  st.s = "clientUpdateProhibited";
  st.lang = "";
  st.msg = "";
  domain_update_cmd->insert_status_rem(st);

  domain_update_cmd->set_registrant("registro.br");

  AuthInfo authInfo;
  authInfo.set_pw("abc123");
  domain_update_cmd->set_authInfo(authInfo);

  if (withDNSSECExtension) {
    //DSInfo
    DSInfo ds;
    ds.set_key_tag(12346);
    ds.set_algo(3);
    ds.set_digest_type(1);
    ds.set_digest("38EC35D5B3A34B44C39B");
    domain_update_cmd->insert_ds_add(ds);

    domain_update_cmd->insert_ds_rem(12345);

    ds.set_key_tag(12345);
    ds.set_digest("49FD46E6C4B45C55D4AC");
    domain_update_cmd->insert_ds_chg(ds);

    domain_update_cmd->setUrgentFlag(true);
  }

  domain_update.get_command()->set_clTRID("ABC-12345");
  domain_update.set_xml_template(to_be_parsed);

  string expected =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<update>"
    "<domain:update "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name>example.org</domain:name>"
    "<domain:add>"
    "<domain:ns>"
    "<domain:hostAttr>"
    "<domain:hostName>ns0.example.org</domain:hostName>"
    "</domain:hostAttr>"
    "<domain:hostAttr>"
    "<domain:hostName>ns1.example.org</domain:hostName>"
    "</domain:hostAttr>"
    "<domain:hostAttr>"
    "<domain:hostName>ns3.example.org</domain:hostName>"
    "<domain:hostAddr>192.168.0.1</domain:hostAddr>"
    "</domain:hostAttr>"
    "</domain:ns>"
    "<domain:contact type='tech'>XXX</domain:contact>"
    "<domain:status s='clientHold' lang='en'>Payment overdue.</domain:status>"
    "</domain:add>"
    "<domain:rem>"
    "<domain:ns>"
    "<domain:hostAttr>"
    "<domain:hostName>ns2.example.org</domain:hostName>"
    "</domain:hostAttr>"
    "</domain:ns>"
    "<domain:contact type='admin'>ABC</domain:contact>"
    "<domain:status s='clientUpdateProhibited'/>"
    "</domain:rem>"
    "<domain:chg>"
    "<domain:registrant>"
    "registro.br"
    "</domain:registrant>"
    "<domain:authInfo>"
    "<domain:pw>abc123</domain:pw>"
    "</domain:authInfo>"
    "</domain:chg>"
    "</domain:update>"
    "</update>";

  if (withDNSSECExtension) {
    expected +=
      "<extension>"
      "<secDNS:update urgent=\"1\" "
      "xmlns:secDNS=\"urn:ietf:params:xml:ns:secDNS-1.0\" "
      "xsi:schemaLocation=\"urn:ietf:params:xml:ns:secDNS-1.0 "
      "secDNS-1.0.xsd\">"
      "<secDNS:add>"
      "<secDNS:dsData>"
      "<secDNS:keyTag>12346</secDNS:keyTag>"
      "<secDNS:alg>3</secDNS:alg>"
      "<secDNS:digestType>1</secDNS:digestType>"
      "<secDNS:digest>38EC35D5B3A34B44C39B</secDNS:digest>"
      "</secDNS:dsData>"
      "</secDNS:add>"
      "<secDNS:rem>"
      "<secDNS:keyTag>12345</secDNS:keyTag>"
      "</secDNS:rem>"
      "<secDNS:chg>"
      "<secDNS:dsData>"
      "<secDNS:keyTag>12345</secDNS:keyTag>"
      "<secDNS:alg>3</secDNS:alg>"
      "<secDNS:digestType>1</secDNS:digestType>"
      "<secDNS:digest>49FD46E6C4B45C55D4AC</secDNS:digest>"
      "</secDNS:dsData>"
      "</secDNS:chg>"
      "</secDNS:update>"
      "</extension>";
  }

  expected +=
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(expected, domain_update.get_xml());

  // tests reset method
  domain_update_cmd = domain_update.get_command();
  domain_update_cmd->reset();
  CPPUNIT_ASSERT(domain_update_cmd->get_name() == "");
  CPPUNIT_ASSERT(domain_update_cmd->get_nameserver_add().empty());
  CPPUNIT_ASSERT(domain_update_cmd->get_nameserver_rem().empty());
  CPPUNIT_ASSERT(domain_update_cmd->get_contact_add().empty());
  CPPUNIT_ASSERT(domain_update_cmd->get_contact_rem().empty());
  CPPUNIT_ASSERT(domain_update_cmd->get_status_add().empty());
  CPPUNIT_ASSERT(domain_update_cmd->get_status_rem().empty());
  CPPUNIT_ASSERT(domain_update_cmd->get_registrant() == "");
  CPPUNIT_ASSERT(domain_update_cmd->get_authInfo().get_pw() == "");
  CPPUNIT_ASSERT(domain_update_cmd->get_authInfo().get_roid() == "");

  // parent attribute
  CPPUNIT_ASSERT(domain_update_cmd->get_clTRID() == "");
}

void DomainUpdateTest::command_test()
{
  DomainUpdate domain_update;
  DomainUpdateCmd *domain_update_cmd = domain_update.get_command();

  domain_update_cmd->set_name("example.org");
  
  struct NameServer ns;
  struct NSIPAddr ip;
  ns.name = "ns0.example.org";
  domain_update_cmd->insert_nameserver_add(ns);
  ns.name = "ns1.example.org";
  domain_update_cmd->insert_nameserver_add(ns);
  ns.name = "ns3.example.org";
  ip.addr = "192.168.0.1";
  ns.ips.insert(ip);
  domain_update_cmd->insert_nameserver_add(ns);
  ns.name = "ns2.example.org";
  domain_update_cmd->insert_nameserver_rem(ns);
  domain_update_cmd->insert_contact_add("tech", "XXX");
  domain_update_cmd->insert_contact_rem("admin", "ABC");

  struct DomainUpdateCmd::Status st;
  st.s = "clientHold";
  st.lang = "en";
  st.msg = "Payment overdue.";
  domain_update_cmd->insert_status_add(st);

  st.s = "clientUpdateProhibited";
  st.lang = "";
  st.msg = "";
  domain_update_cmd->insert_status_rem(st);

  domain_update_cmd->set_registrant("registro.br");

  AuthInfo authInfo;
  authInfo.set_pw("abc123");
  domain_update_cmd->set_authInfo(authInfo);

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/domain_update.xml");

    domain_update.get_command()->set_clTRID("ABC-12345");
    domain_update.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(domain_update.get_xml());
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

void DomainUpdateTest::response_test()
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
  DomainUpdate domain_update;
  Response* domain_update_rsp = domain_update.get_response();

  CPPUNIT_ASSERT_NO_THROW(domain_update.set_response(expected, &parser));
  domain_update_rsp = domain_update.get_response();

  string result_code = "1000";

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = domain_update_rsp->get_result_list();
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
  
  string result_lang = domain_update_rsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response +=
    ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<trID>"
    "<clTRID>" + domain_update_rsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + domain_update_rsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(expected, response);

  // tests reset method
  domain_update_rsp->reset();
  CPPUNIT_ASSERT_EQUAL((string)"", domain_update_rsp->get_clTRID());
  CPPUNIT_ASSERT_EQUAL((string)"", domain_update_rsp->get_svTRID());
  results = domain_update_rsp->get_result_list();
  r_it = results.begin();
  CPPUNIT_ASSERT(r_it == results.end());
}
