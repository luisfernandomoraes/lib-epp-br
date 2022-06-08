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
/* $Id: DomainCreateTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#include <memory>
#include <string>

#include "libepp_nicbr.H"

#include "DomainCreateTest.H"
#include "DomainCreate.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

#include "StrUtil.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(DomainCreateTest);

DomainCreateTest::DomainCreateTest() {}

DomainCreateTest::~DomainCreateTest() {}

void DomainCreateTest::setUp() {}

void DomainCreateTest::tearDown() {}

void DomainCreateTest::set_xml_template_test()
{
  set_xml_template_test(true);
  set_xml_template_test(false);
}

void DomainCreateTest::set_xml_template_test(bool withDNSSECExtension)
{
  string to_be_parsed =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<create>"
    "<domain:create "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name>$(name)$</domain:name>"
    "$(period)$"
    "$(nameservers)$"
    "$(registrant)$"
    "$(other_contacts)$"
    "$(auth_info)$"
    "</domain:create>"
    "</create>"
    "$(ext_begin)$"
    "$(ds_ext)$"
    "$(ext_end)$"
    "$(clTRID)$"
    "</command>"
    "</epp>";

  DomainCreate domain_create;
  DomainCreateCmd* domain_create_cmd = domain_create.get_command();
  domain_create_cmd->set_name("asultimasnuvensazuisnoceudaalameda"
			      "principal.org");
  domain_create_cmd->set_period(12, "m");
  struct NameServer ns;
  ns.name = "ns1.nuvens.org";
  domain_create_cmd->insert_nameserver(ns);
  ns.name = "ns0.nuvens.org";
  domain_create_cmd->insert_nameserver(ns);
  ns.name = "ns3.nuvens.org";
  NSIPAddr ip;
  ip.addr="192.168.0.1";
  ns.ips.insert(ip);
  domain_create_cmd->insert_nameserver(ns);
  domain_create_cmd->set_registrant("John Nash");
  domain_create_cmd->insert_contact("tech", "EDS279");
  domain_create_cmd->insert_contact("billing", "XXX");
  domain_create_cmd->insert_contact("admin", "HKK");
  AuthInfo authInfo;
  authInfo.set_roid("");
  authInfo.set_pw("abc123");
  domain_create_cmd->set_authInfo(authInfo);

  if (withDNSSECExtension) {
    //DS information
    DSInfo ds;
    ds.set_key_tag(12345);
    ds.set_algo(3);
    ds.set_digest_type(1);
    ds.set_digest("49FD46E6C4B45C55D4AC");
    ds.set_max_sig_life(86400);
    domain_create_cmd->add_dsInfo(ds);
  }

  domain_create.get_command()->set_clTRID("ABC-12345");
  domain_create.set_xml_template(to_be_parsed);

  string expected =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<create>"
    "<domain:create "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name>asultimasnuvensazuisnoceudaalamedaprincipal.org"
    "</domain:name>"
    "<domain:period unit='m'>12</domain:period>"
    "<domain:ns>"
    "<domain:hostAttr>"
    "<domain:hostName>ns1.nuvens.org</domain:hostName>"
    "</domain:hostAttr>"
    "<domain:hostAttr>"
    "<domain:hostName>ns0.nuvens.org</domain:hostName>"
    "</domain:hostAttr>"
    "<domain:hostAttr>"
    "<domain:hostName>ns3.nuvens.org</domain:hostName>"
    "<domain:hostAddr>192.168.0.1</domain:hostAddr>"
    "</domain:hostAttr>"
    "</domain:ns>"
    "<domain:registrant>John Nash</domain:registrant>"
    "<domain:contact type='admin'>HKK</domain:contact>"
    "<domain:contact type='billing'>XXX</domain:contact>"
    "<domain:contact type='tech'>EDS279</domain:contact>"
    "<domain:authInfo>"
    "<domain:pw>abc123</domain:pw>"
    "</domain:authInfo>"
    "</domain:create>"
    "</create>";

  if (withDNSSECExtension) {
    expected +=
      "<extension>"
      "<secDNS:create "
      "xmlns:secDNS=\"urn:ietf:params:xml:ns:secDNS-1.0\" "
      "xsi:schemaLocation=\"urn:ietf:params:xml:ns:secDNS-1.0 "
      "secDNS-1.0.xsd\">"
      "<secDNS:dsData>"
      "<secDNS:keyTag>12345</secDNS:keyTag>"
      "<secDNS:alg>3</secDNS:alg>"
      "<secDNS:digestType>1</secDNS:digestType>"
      "<secDNS:digest>49FD46E6C4B45C55D4AC</secDNS:digest>"
      "<secDNS:maxSigLife>86400</secDNS:maxSigLife>"
      "</secDNS:dsData>"
      "</secDNS:create>"
      "</extension>";
  }

  expected +=
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(expected, domain_create.get_xml());

  // tests reset method
  domain_create_cmd = domain_create.get_command();
  domain_create_cmd->reset();
  CPPUNIT_ASSERT(domain_create_cmd->get_name() == "");
  CPPUNIT_ASSERT(domain_create_cmd->get_period().time == 0);
  CPPUNIT_ASSERT(domain_create_cmd->get_period().unit == "");
  CPPUNIT_ASSERT(domain_create_cmd->get_nameservers().empty());
  CPPUNIT_ASSERT(domain_create_cmd->get_registrant() == "");
  CPPUNIT_ASSERT(domain_create_cmd->get_contacts().empty());
  CPPUNIT_ASSERT(domain_create_cmd->get_authInfo().get_pw() == "");
  
  // parent attribute
  CPPUNIT_ASSERT(domain_create_cmd->get_clTRID() == "");
}

void DomainCreateTest::command_test()
{
  DomainCreate domain_create;
  DomainCreateCmd *domain_create_cmd = domain_create.get_command();

  domain_create_cmd->set_name("asultimasnuvensazuisnoceudaalameda"
			      "principal.org");
  domain_create_cmd->set_period(12, "m");
  struct NameServer ns;
  ns.name = "ns1.nuvens.org";
  domain_create_cmd->insert_nameserver(ns);
  ns.name = "ns0.nuvens.org";
  domain_create_cmd->insert_nameserver(ns);
  ns.name = "ns3.nuvens.org";
  NSIPAddr ip;
  ip.addr = "192.168.0.1";
  ns.ips.insert(ip);
  domain_create_cmd->insert_nameserver(ns);
  domain_create_cmd->set_registrant("John Nash");
  domain_create_cmd->insert_contact("tech", "EDS279");
  domain_create_cmd->insert_contact("billing", "XXX");
  domain_create_cmd->insert_contact("admin", "HKK");
  AuthInfo authInfo;
  authInfo.set_roid("");
  authInfo.set_pw("abc123");
  domain_create_cmd->set_authInfo(authInfo);
  
  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/domain_create.xml");

    domain_create.get_command()->set_clTRID("ABC-12345");
    domain_create.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(domain_create.get_xml());
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

void DomainCreateTest::response_test()
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
    "<domain:creData "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name>asultimasnuvensazuisnoceudaalamedaprincipal.org"
    "</domain:name>"
    "<domain:crDate>1999-04-03T22:00:00.0Z</domain:crDate>"
    "<domain:exDate>2001-04-03T22:00:00.0Z</domain:exDate>"
    "</domain:creData>"
    "</resData>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54321-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  DomParser parser;
  parser.enable_validation("../docs/schemas");

  DomainCreate domain_create;
  CPPUNIT_ASSERT_NO_THROW(domain_create.set_response(expected, &parser));
  DomainCreateRsp* domain_create_rsp = domain_create.get_response();

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = domain_create_rsp->get_result_list();
  r_it = results.begin();

  CPPUNIT_ASSERT(r_it != results.end());
  CPPUNIT_ASSERT_EQUAL(Response::OK, r_it->first);

  string result_code =
    StrUtil::to_string("%d", (int)r_it->first);

  string response =
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
    "<epp xmlns=\"urn:ietf:params:xml:ns:epp-1.0\" "
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd\">"
    "<response>"
    "<result code='" + result_code + "'>"
    "<msg";
  
  string result_lang = domain_create_rsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response +=
    ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<resData>"
    "<domain:creData "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name>" + domain_create_rsp->get_name() + "</domain:name>"
    "<domain:crDate>" + domain_create_rsp->get_crDate() + "</domain:crDate>";

    if (domain_create_rsp->get_exDate() != "") {
      response += "<domain:exDate>" + domain_create_rsp->get_exDate() +
	"</domain:exDate>";
    }

    response += "</domain:creData>"
      "</resData>"
      "<trID>"
      "<clTRID>" + domain_create_rsp->get_clTRID() + "</clTRID>"
      "<svTRID>" + domain_create_rsp->get_svTRID() + "</svTRID>"
      "</trID>"
      "</response>"
      "</epp>";
    
  CPPUNIT_ASSERT_EQUAL(expected, response);

  // tests reset method
  domain_create_rsp->reset();
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_create_rsp->get_name());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_create_rsp->get_crDate());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_create_rsp->get_exDate());
}
