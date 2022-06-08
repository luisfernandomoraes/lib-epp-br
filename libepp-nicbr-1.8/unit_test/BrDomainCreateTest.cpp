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
/* $Id: BrDomainCreateTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#if USE_BR_DOMAINS
#include <string>
#include <memory>
#include <iostream>

#include "libepp_nicbr.H"
#include "StrUtil.H"

#include "BrDomainCreateTest.H"
#include "BrDomainCreate.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

using std::auto_ptr;

using std::cout;
using std::endl;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(BrDomainCreateTest);

BrDomainCreateTest::BrDomainCreateTest() {}

BrDomainCreateTest::~BrDomainCreateTest() {}

void BrDomainCreateTest::setUp() {}

void BrDomainCreateTest::tearDown() {}

void BrDomainCreateTest::set_xml_template_test()
{
  set_xml_template_test(true);
  set_xml_template_test(false);
}

void BrDomainCreateTest::set_xml_template_test(bool withDNSSECExtension)
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
    "$(ext_begin)$"
    "$(ds_ext)$"
    "$(br_ext)$"
    "$(ext_end)$" 
    "</create>"
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
    "<create>"
    "<domain:create "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name>example.com.br</domain:name>"
    "<domain:ns>"
    "<domain:hostAttr>"
    "<domain:hostName>ns1.example.com.br</domain:hostName>"
    "<domain:hostAddr ip='v4'>192.0.2.1</domain:hostAddr>"
    "</domain:hostAttr>"
    "<domain:hostAttr>"
    "<domain:hostName>ns1.example.net.br</domain:hostName>"
    "</domain:hostAttr>"
    "</domain:ns>"
    "<domain:contact type='admin'>fan</domain:contact>"
    "<domain:contact type='billing'>fan</domain:contact>"
    "<domain:contact type='tech'>fan</domain:contact>"
    "<domain:authInfo>"
    "<domain:pw>2fooBAR</domain:pw>"
    "</domain:authInfo>"
    "</domain:create>"
    "<extension>";

  if (withDNSSECExtension) {
    expected += "<secDNS:create "
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
      "</secDNS:create>";
  }

  expected +=
    "<brdomain:create "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>"
    "<brdomain:organization>"
    "005.506.560/0001-36"
    "</brdomain:organization>"
    "<brdomain:releaseProcessFlags flag1='1'/>"
    "<brdomain:autoRenew active=\"1\"/>"
    "</brdomain:create>"
    "</extension>"
    "</create>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";

  //parent information
  BrDomainCreate brdcreate;
  BrDomainCreateCmd* cmd = brdcreate.get_command();
  cmd->set_name("example.com.br");

  struct NameServer ns;
  struct NSIPAddr ip;
  ns.name = "ns1.example.com.br";
  ip.version = "v4";
  ip.addr = "192.0.2.1";
  ns.ips.insert(ip);
  cmd->insert_nameserver(ns);

  ns.name = "ns1.example.net.br";
  ns.ips.clear();
  cmd->insert_nameserver(ns);

  cmd->insert_contact("admin", "fan");
  cmd->insert_contact("tech", "fan");
  cmd->insert_contact("billing", "fan");
  
  AuthInfo ai;
  ai.set_pw("2fooBAR");
  cmd->set_authInfo(ai);

  if (withDNSSECExtension) {
    //DS information
    DSInfo ds;
    ds.set_key_tag(12345);
    ds.set_algo(3);
    ds.set_digest_type(1);
    ds.set_digest("49FD46E6C4B45C55D4AC");
    ds.set_max_sig_life(86400);
    cmd->add_dsInfo(ds);
  }

  //BrDomainCreate specific information
  cmd->set_organization("005.506.560/0001-36");

  struct ReleaseProcessFlags rpf;
  rpf.flag1 = 1;
  
  cmd->set_releaseProcessFlags(rpf);

  cmd->set_auto_renew(1);

  //set_xml_template/get_xml
  cmd->set_clTRID("ABC-12345");
  brdcreate.set_xml_template(to_be_parsed);

  CPPUNIT_ASSERT_EQUAL(expected, brdcreate.get_xml());

  //tests reset method
  cmd->reset();

  //parent information
  CPPUNIT_ASSERT_EQUAL((string)"", cmd->get_name());
  CPPUNIT_ASSERT(cmd->get_nameservers().empty());
  CPPUNIT_ASSERT(cmd->get_contacts().empty());
  CPPUNIT_ASSERT_EQUAL((string)"", cmd->get_authInfo().get_pw());
  CPPUNIT_ASSERT_EQUAL((string)"", cmd->get_authInfo().get_roid());

  //BrDomainCreate specific information
  CPPUNIT_ASSERT_EQUAL((string)"", cmd->get_organization());
  CPPUNIT_ASSERT(cmd->get_releaseProcessFlags().flag1 == 0);
  CPPUNIT_ASSERT(cmd->get_releaseProcessFlags().flag2 == 0);
  CPPUNIT_ASSERT(cmd->get_releaseProcessFlags().flag3 == 0);
  CPPUNIT_ASSERT(cmd->get_auto_renew() == -1);
}

void BrDomainCreateTest::command_test()
{
  BrDomainCreate domain_create;
  BrDomainCreateCmd *cmd = domain_create.get_command();

  //parent information
  cmd->set_name("asultimasnuvensazuisnoceudaalamedaprincipal.org.br");
  cmd->set_period(12, "m");
  struct NameServer ns;
  struct NSIPAddr ip;
  ns.name = "ns1.nuvens.org";
  cmd->insert_nameserver(ns);
  ns.name = "ns0.nuvens.org";
  cmd->insert_nameserver(ns);
  ns.name = "ns3.nuvens.org";
  ip.addr = "192.168.0.1";
  ns.ips.insert(ip);
  cmd->insert_nameserver(ns);
  cmd->set_registrant("João Basílio 1º");
  cmd->insert_contact("tech", "EDS279");
  cmd->insert_contact("billing", "XXX");
  cmd->insert_contact("admin", "HKK");
  AuthInfo authInfo;
  authInfo.set_roid("");
  authInfo.set_pw("abc123");
  cmd->set_authInfo(authInfo);

  //BrDomainCreateCmd specific information
  cmd->set_organization("005.506.560/0001-36");

  struct ReleaseProcessFlags rpf;
  rpf.flag1 = 1;
  cmd->set_releaseProcessFlags(rpf);


  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/br_domain_create.xml");

    domain_create.get_command()->set_clTRID("ABC-12345");
    domain_create.set_xml_template(xml_template);

    DomParser parser;
    //parser.enable_validation("../docs/schemas");

    parser.parse_command(domain_create.get_xml());
  } catch (const IoException &e) {
    exception_caught = true;
    printf("\nIO Exception: code [%d] message [%s]",
	   e.get_code(), e.get_msg().c_str());
  } catch (const XmlException &e) {
    exception_caught = true;
    printf("\nXml Exception: code [%d] message [%s] low level message [%s]\n",
	   e.get_code(), e.get_msg().c_str(), e.get_low_level_msg().c_str());
    cout << endl << domain_create.get_xml() << endl;
  }
  
  CPPUNIT_ASSERT(!exception_caught);
}

void BrDomainCreateTest::response_test()
{
  string expected =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<response>"
    "<result code='1001'>"
    "<msg>Command completed successfully; action pending</msg>"
    "</result>"
    "<resData>"
    "<domain:creData "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name>example.com.br</domain:name>"
    "<domain:crDate>2006-01-30T22:00:00.0Z</domain:crDate>"
    "</domain:creData>"
    "</resData>"
    "<extension>"
    "<brdomain:creData "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>"
    "<brdomain:ticketNumber>123456</brdomain:ticketNumber>"
    "<brdomain:pending>"
    "<brdomain:doc status='notReceived'>"
    "<brdomain:docType>CNPJ</brdomain:docType>"
    "<brdomain:limit>2006-03-01T22:00:00.0Z</brdomain:limit>"
    "<brdomain:description lang='pt'>"
    "Cadastro Nacional da Pessoa Juridica"
    "</brdomain:description>"
    "</brdomain:doc>"
    "<brdomain:dns status='queryTimeOut'>"
    "<brdomain:hostName>"
    "ns1.example.com.br"
    "</brdomain:hostName>"
    "<brdomain:limit>2006-02-13T22:00:00.0Z</brdomain:limit>"
    "</brdomain:dns>"
    "</brdomain:pending>"
    "<brdomain:ticketNumberConc>"
    "123451"
    "</brdomain:ticketNumberConc>"
    "<brdomain:ticketNumberConc>"
    "123455"
    "</brdomain:ticketNumberConc>"
    "</brdomain:creData>"
    "</extension>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54321-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";
  
  DomParser parser;
  parser.enable_validation("../docs/schemas");
  
  BrDomainCreate domain_create;

  bool exception_caught = false;
  try {
    domain_create.set_response(expected, &parser);
  } catch (const XmlException &e) {
    exception_caught = true;
    printf("\nXml Exception: code [%d] message [%s] low level message [%s]\n",
	   e.get_code(), e.get_msg().c_str(), e.get_low_level_msg().c_str());
    cout << endl << domain_create.get_xml() << endl;
  }
  CPPUNIT_ASSERT(!exception_caught);

  BrDomainCreateRsp* rsp = domain_create.get_response();

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = rsp->get_result_list();
  r_it = results.begin();

  CPPUNIT_ASSERT(r_it != results.end());
  CPPUNIT_ASSERT_EQUAL(Response::OK_ACTION_PENDING, r_it->first);

  string result_code = StrUtil::to_string("%d", (int)r_it->first);
  
  string response =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<response>"
    "<result code='" + result_code + "'>"
    "<msg";
  
  string result_lang = rsp->get_result_lang();
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
    "<domain:name>" + rsp->get_name() + "</domain:name>"
    "<domain:crDate>" + rsp->get_crDate() + "</domain:crDate>";

    if (rsp->get_exDate() != "") {
      response += "<domain:exDate>" + rsp->get_exDate() +
	"</domain:exDate>";
    }

    response += "</domain:creData>"
      "</resData>";

  //BrDomainCreate specific information
  response += "<extension>"
    "<brdomain:creData "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>";

  response += "<brdomain:ticketNumber>" + 
    StrUtil::to_string("%d", rsp->get_ticketNumber()) +
    "</brdomain:ticketNumber>"
    "<brdomain:pending>";  
  
  PENDING_CONTAINER<struct PendingDoc> doc_pendings = rsp->get_doc_pendings();
  PENDING_CONTAINER<struct PendingDoc>::const_iterator doc_it;

  for (doc_it = doc_pendings.begin(); doc_it != doc_pendings.end(); doc_it++) {
    response +=
      "<brdomain:doc status='" + (*doc_it).status + "'>"
      "<brdomain:docType>" + (*doc_it).docType + "</brdomain:docType>"
      "<brdomain:limit>" + (*doc_it).limit + "</brdomain:limit>"

      "<brdomain:description";
    if ((*doc_it).description_lang != "") {
      response += " lang='" + (*doc_it).description_lang + "'";
    }

    response += ">" + (*doc_it).description +
      "</brdomain:description>"

      "</brdomain:doc>";
  }

  PENDING_CONTAINER<struct PendingDns> dns_pendings = rsp->get_dns_pendings();
  PENDING_CONTAINER<struct PendingDns>::const_iterator dns_it;

  for (dns_it = dns_pendings.begin(); dns_it != dns_pendings.end(); dns_it++) {
    response += 
      "<brdomain:dns status='" + (*dns_it).status + "'>"
      "<brdomain:hostName>" + (*dns_it).hostName + "</brdomain:hostName>"
      "<brdomain:limit>" + (*dns_it).limit + "</brdomain:limit>"
      "</brdomain:dns>";
  }

  struct PendingReleaseProc rel_pending = rsp->get_rel_pending();
  if (rel_pending.status != "") {
    response +=
      "<brdomain:releaseProc status='" + rel_pending.status + "'>"
      "<brdomain:limit>" + rel_pending.limit + "</brdomain:limit>"
      "</brdomain:releaseProc>";
  }
  
  response += "</brdomain:pending>";
  
  set<int> concurrent_tickets = rsp->get_concurrent_tickets();
  set<int>::const_iterator tkt_it;
  for (tkt_it = concurrent_tickets.begin(); tkt_it != concurrent_tickets.end();
       tkt_it++) {
    response +=
      "<brdomain:ticketNumberConc>" + StrUtil::to_string("%d", (*tkt_it)) +
      "</brdomain:ticketNumberConc>";
  }
  
  response +=    
    "</brdomain:creData>"
    "</extension>"
    "<trID>"
    "<clTRID>" + rsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + rsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";



  CPPUNIT_ASSERT_EQUAL(expected, response);
  
  //tests reset method
  rsp->reset();

  //parent specific
  CPPUNIT_ASSERT_EQUAL((const string)"", rsp->get_name());
  CPPUNIT_ASSERT_EQUAL((const string)"", rsp->get_crDate());
  CPPUNIT_ASSERT_EQUAL((const string)"", rsp->get_exDate());

  //BrDomainCreate
  CPPUNIT_ASSERT_EQUAL(0, rsp->get_ticketNumber());
  CPPUNIT_ASSERT(rsp->get_doc_pendings().empty());
  CPPUNIT_ASSERT(rsp->get_dns_pendings().empty());
  CPPUNIT_ASSERT_EQUAL((const string) "", rsp->get_rel_pending().status);
  CPPUNIT_ASSERT_EQUAL((const string) "", rsp->get_rel_pending().limit);
}
#endif //USE_BR_DOMAINS
