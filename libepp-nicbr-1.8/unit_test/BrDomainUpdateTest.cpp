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
/* $Id: BrDomainUpdateTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#if USE_BR_DOMAINS
#include <string>
#include <memory>
#include <iostream>

#include "libepp_nicbr.H"
#include "StrUtil.H"

#include "BrDomainUpdateTest.H"
#include "BrDomainUpdate.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

using std::auto_ptr;

using std::cout;
using std::endl;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(BrDomainUpdateTest);

BrDomainUpdateTest::BrDomainUpdateTest() {}

BrDomainUpdateTest::~BrDomainUpdateTest() {}

void BrDomainUpdateTest::setUp() {}

void BrDomainUpdateTest::tearDown() {}

void BrDomainUpdateTest::set_xml_template_test()
{
  set_xml_template_test(true);
  set_xml_template_test(false);
}

void BrDomainUpdateTest::set_xml_template_test(bool withDNSSECExtension)
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
    "</domain:update>"
    "</update>"
    "$(ext_begin)$"
    "$(ds_ext)$"
    "$(br_ext)$"
    "$(ext_end)$"
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
    "<update>"
    "<domain:update "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name>example.com.br</domain:name>"
    "<domain:add>"
    "<domain:ns>"
    "<domain:hostAttr>"
    "<domain:hostName>ns2.example.com</domain:hostName>"
    "</domain:hostAttr>"
    "</domain:ns>"
    "<domain:contact type='tech'>hkk</domain:contact>"
    "</domain:add>"
    "<domain:rem>"
    "<domain:ns>"
    "<domain:hostAttr>"
    "<domain:hostName>ns1.example.com.br</domain:hostName>"
    "</domain:hostAttr>"
    "</domain:ns>"
    "<domain:contact type='tech'>fan</domain:contact>"
    "</domain:rem>"
    "</domain:update>"
    "</update>"
    "<extension>";

  if (withDNSSECExtension) {
    expected +=
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
      "</secDNS:update>";
  }

  expected +=
    "<brdomain:update "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>"
    "<brdomain:ticketNumber>123456</brdomain:ticketNumber>"
    "<brdomain:chg>"
    "<brdomain:releaseProcessFlags flag2='1'/>"
    "<brdomain:autoRenew active=\"1\"/>"
    "</brdomain:chg>"
    "</brdomain:update>"
    "</extension>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";

  BrDomainUpdate br_domain_update;
  BrDomainUpdateCmd* cmd = br_domain_update.get_command();

  //parent info
  cmd->set_name("example.com.br");

  struct NameServer ns;
  ns.name = "ns2.example.com";
  cmd->insert_nameserver_add(ns);

  cmd->insert_contact_add("tech", "hkk");

  ns.name = "ns1.example.com.br";
  cmd->insert_nameserver_rem(ns);

  cmd->insert_contact_rem("tech", "fan");

  if (withDNSSECExtension) {
    //DSInfo
    DSInfo ds;
    ds.set_key_tag(12346);
    ds.set_algo(3);
    ds.set_digest_type(1);
    ds.set_digest("38EC35D5B3A34B44C39B");
    cmd->insert_ds_add(ds);

    cmd->insert_ds_rem(12345);

    ds.set_key_tag(12345);
    ds.set_digest("49FD46E6C4B45C55D4AC");
    cmd->insert_ds_chg(ds);

    cmd->setUrgentFlag(true);
  }

  //BrDomain specific info
  cmd->set_ticketNumber(123456);

  struct ReleaseProcessFlags rpf;
  rpf.flag2 = 1;
  cmd->set_releaseProcessFlags(rpf);

  cmd->set_auto_renew(1);

  br_domain_update.get_command()->set_clTRID("ABC-12345");
  br_domain_update.set_xml_template(to_be_parsed);

  CPPUNIT_ASSERT_EQUAL(expected, br_domain_update.get_xml());

  //tests reset method
  cmd->reset();

  //parent info
  CPPUNIT_ASSERT(cmd->get_name() == "");
  CPPUNIT_ASSERT(cmd->get_nameserver_add().empty());
  CPPUNIT_ASSERT(cmd->get_nameserver_rem().empty());
  CPPUNIT_ASSERT(cmd->get_contact_add().empty());
  CPPUNIT_ASSERT(cmd->get_contact_rem().empty());
  CPPUNIT_ASSERT(cmd->get_status_add().empty());
  CPPUNIT_ASSERT(cmd->get_status_rem().empty());
  CPPUNIT_ASSERT(cmd->get_registrant() == "");
  CPPUNIT_ASSERT(cmd->get_authInfo().get_pw() == "");
  CPPUNIT_ASSERT(cmd->get_authInfo().get_roid() == "");

  //BrDomainUpdateCmd specific info
  CPPUNIT_ASSERT_EQUAL(0, cmd->get_ticketNumber());
  rpf = cmd->get_releaseProcessFlags();
  CPPUNIT_ASSERT_EQUAL(-3, rpf.flag1 + rpf.flag2 + rpf.flag3 );
  CPPUNIT_ASSERT_EQUAL(-1, cmd->get_auto_renew());
}

void BrDomainUpdateTest::command_test()
{
  BrDomainUpdate domain_update;
  BrDomainUpdateCmd *cmd = domain_update.get_command();

  cmd->set_name("example.org");
  
  struct NameServer ns;
  struct NSIPAddr ip;
  ns.name = "ns0.example.org";
  cmd->insert_nameserver_add(ns);
  ns.name = "ns1.example.org";
  cmd->insert_nameserver_add(ns);
  ns.name = "ns3.example.org";
  ip.addr = "192.168.0.1";
  ns.ips.insert(ip);
  cmd->insert_nameserver_add(ns);
  ns.name = "ns2.example.org";
  cmd->insert_nameserver_rem(ns);
  cmd->insert_contact_add("tech", "XXX");
  cmd->insert_contact_rem("admin", "ABC");

  struct DomainUpdateCmd::Status st;
  st.s = "clientHold";
  st.lang = "en";
  st.msg = "Payment overdue.";
  cmd->insert_status_add(st);

  st.s = "clientUpdateProhibited";
  st.lang = "";
  st.msg = "";
  cmd->insert_status_rem(st);

  cmd->set_registrant("registro.br");

  AuthInfo authInfo;
  authInfo.set_pw("abc123");
  cmd->set_authInfo(authInfo);

  //BrDomain specific info
  cmd->set_ticketNumber(123456);

  struct ReleaseProcessFlags rpf;
  rpf.flag1 = 1;
  cmd->set_releaseProcessFlags(rpf);

  cmd->set_auto_renew(1);

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/br_domain_update.xml");

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
    cout << endl << domain_update.get_xml() << endl;
  }

  CPPUNIT_ASSERT(!exception_caught);
}

void BrDomainUpdateTest::response_test_case1_and_reset()
{
  //tests <update> response of a command WITH <extension> element
  //WARNING! Not testing brdomain:releaseProcessFlags
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
    "<extension>"
    "<brdomain:updData "
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
    "</brdomain:pending>"
    "</brdomain:updData>"
    "</extension>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54321-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";
  
  DomParser parser;
  parser.enable_validation("../docs/schemas");
  
  BrDomainUpdate domain_update;
  BrDomainUpdateRsp* rsp = domain_update.get_response();

  CPPUNIT_ASSERT_NO_THROW(domain_update.set_response(expected, &parser));

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = rsp->get_result_list();
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
    "<result code='" + result_code + "'>"
    "<msg";
  
  string result_lang = rsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response +=
    ">" + r_it->second.msg + "</msg>"
    "</result>";


  // brdomain:update extension specific stuff
  response +=
    "<extension>"
    "<brdomain:updData "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' " 
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>"

    "<brdomain:ticketNumber>" + 
    StrUtil::to_string("%d", rsp->get_ticketNumber()) +
    "</brdomain:ticketNumber>";

    response += "<brdomain:pending>";

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

  response +=    
    "</brdomain:updData>"
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

  //parent info
  CPPUNIT_ASSERT_EQUAL((string)"", rsp->get_clTRID());
  CPPUNIT_ASSERT_EQUAL((string)"", rsp->get_svTRID());
  results = rsp->get_result_list();
  r_it = results.begin();
  CPPUNIT_ASSERT(r_it == results.end());

  //BrDomainUpdateRsp specific information
  CPPUNIT_ASSERT_EQUAL(0, rsp->get_ticketNumber());
  CPPUNIT_ASSERT(rsp->get_doc_pendings().empty());
  CPPUNIT_ASSERT(rsp->get_dns_pendings().empty());
  CPPUNIT_ASSERT_EQUAL((string)"", rsp->get_rel_pending().status);
  CPPUNIT_ASSERT_EQUAL((string)"", rsp->get_rel_pending().limit);
  CPPUNIT_ASSERT(rsp->get_host_status().empty());
}

void BrDomainUpdateTest::response_test_case2()
{
  //tests brdomain:hostStatus - response of a command WITH NO
  //<extension> element

  string expected =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<response>"
    "<result code='2308'>"
    "<msg>Data management policy violation</msg>"
    "<extValue>"
    "<value xmlns:domain=\"urn:ietf:params:xml:ns:domain-1.0\">"
    "<domain:hostName>ns2.example.com</domain:hostName>"
    "</value>"
    "<reason>Query refused</reason>"
    "</extValue>"
    "</result>"
    "<extension>"
    "<brdomain:updData "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>"
    "<brdomain:hostStatus>"
    "<brdomain:hostName>ns2.example.com</brdomain:hostName>"
    "<brdomain:dnsAnswer>Query refused</brdomain:dnsAnswer>"
    "</brdomain:hostStatus>"
    "<brdomain:publicationStatus publicationFlag=\"onHold\">"
    "<brdomain:onHoldReason>billing</brdomain:onHoldReason>"
    "<brdomain:onHoldReason>dns</brdomain:onHoldReason>"
    "</brdomain:publicationStatus>"
    "</brdomain:updData>"
    "</extension>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54321-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  DomParser parser;
  parser.enable_validation("../docs/schemas");
  
  BrDomainUpdate domain_update;
  BrDomainUpdateRsp* rsp = domain_update.get_response();

  CPPUNIT_ASSERT_NO_THROW(domain_update.set_response(expected, &parser));

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = rsp->get_result_list();
  r_it = results.begin();

  CPPUNIT_ASSERT(r_it != results.end());
  CPPUNIT_ASSERT_EQUAL(Response::DATA_MGMT_POLICY_VIOLATION, r_it->first);

  list <Response::ResultExtValue> ext_values = r_it->second.ext_values;
  list <Response::ResultExtValue>::const_iterator ev_it;
  ev_it = ext_values.begin();

  string value = "ns2.example.com";
  string xmlns = "xmlns:domain=\"urn:ietf:params:xml:ns:domain-1.0\"";
  string reason = "Query refused";
  CPPUNIT_ASSERT(ev_it != ext_values.end());
  CPPUNIT_ASSERT_EQUAL(value, ev_it->value);
  CPPUNIT_ASSERT_EQUAL(xmlns, ev_it->xmlns);
  CPPUNIT_ASSERT_EQUAL(reason, ev_it->reason);

  string result_code =
    StrUtil::to_string("%d", (int)r_it->first);

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
    "<extValue>"
    "<value " + xmlns + ">"
    "<domain:hostName>" + value + "</domain:hostName></value>"
    "<reason>" + reason + "</reason>"
    "</extValue>"
    "</result>";


  // brdomain:update extension specific stuff
  response +=
    "<extension>"
    "<brdomain:updData "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' " 
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>";

  set<struct HostStatus> host_status = rsp->get_host_status();
  if (!host_status.empty()) {
    set<struct HostStatus>::const_iterator it;
    for (it = host_status.begin();
	 it != host_status.end(); it++) {
      response += "<brdomain:hostStatus>"
	"<brdomain:hostName>" + (*it).host + "</brdomain:hostName>"
	"<brdomain:dnsAnswer>" + (*it).status + "</brdomain:dnsAnswer>"
	"</brdomain:hostStatus>";
    }
  }

  string publicationFlag = rsp->get_publication_flag();
  
  response += "<brdomain:publicationStatus publicationFlag=\"" + 
    publicationFlag + "\">";
  
  set<string> onhold_reasons = rsp->get_onhold_reasons();
  
  set<string>::const_iterator tkt_it;
  for (tkt_it = onhold_reasons.begin(); tkt_it != onhold_reasons.end();
       tkt_it++) {
    response +=
      "<brdomain:onHoldReason>" +  (*tkt_it) +
      "</brdomain:onHoldReason>";
  }

  response +=    
    "</brdomain:publicationStatus>"
    "</brdomain:updData>"
    "</extension>"
    "<trID>"
    "<clTRID>" + rsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + rsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(expected, response);

  //just to make sure host_status gets really empty (the other
  //attributes are tested in the previous method)
  rsp->reset();
  CPPUNIT_ASSERT(rsp->get_host_status().empty());
  CPPUNIT_ASSERT_EQUAL((string)"", rsp->get_publication_flag());
  CPPUNIT_ASSERT_EQUAL(true, rsp->get_onhold_reasons().empty());
}
#endif //USE_BR_DOMAINS
