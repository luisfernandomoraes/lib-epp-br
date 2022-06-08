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
/* $Id: BrDomainInfoTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#if USE_BR_DOMAINS
#include <string>
#include <memory>
#include <iostream>

#include "libepp_nicbr.H"
#include "StrUtil.H"

#include "BrDomainInfoTest.H"
#include "BrDomainInfo.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

using std::auto_ptr;

using std::cout;
using std::endl;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(BrDomainInfoTest);

BrDomainInfoTest::BrDomainInfoTest() {}

BrDomainInfoTest::~BrDomainInfoTest() {}

void BrDomainInfoTest::setUp() {}

void BrDomainInfoTest::tearDown() {}

void BrDomainInfoTest::set_xml_template_test()
{
  string to_be_parsed =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<info>"
    "<domain:info "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name hosts='$(hosts_control)$'>$(name)$</domain:name>"
    "$(auth_info)$"
    "</domain:info>"
    "</info>"
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
    "<info>"
    "<domain:info "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name hosts='all'>example.com.br</domain:name>"
    "</domain:info>"
    "</info>"
    "<extension>"
    "<brdomain:info "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>"
    "<brdomain:ticketNumber>123456</brdomain:ticketNumber>"
    "</brdomain:info>"
    "</extension>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";

  BrDomainInfo br_domain_info;
  BrDomainInfoCmd* br_domain_info_cmd = br_domain_info.get_command();
  br_domain_info_cmd->set_name("example.com.br");
  br_domain_info_cmd->set_hosts_control("all");
  br_domain_info_cmd->set_ticketNumber(123456);

  br_domain_info.get_command()->set_clTRID("ABC-12345");
  br_domain_info.set_xml_template(to_be_parsed);
   
  CPPUNIT_ASSERT_EQUAL(expected, br_domain_info.get_xml());

  // tests reset method
  br_domain_info_cmd->reset();

  //parent attributes
  CPPUNIT_ASSERT(br_domain_info_cmd->get_name() == "");
  CPPUNIT_ASSERT(br_domain_info_cmd->get_hosts_control() == "all");
  CPPUNIT_ASSERT(br_domain_info_cmd->get_authInfo().get_pw() == "");
  CPPUNIT_ASSERT(br_domain_info_cmd->get_authInfo().get_roid() == "");

  //BrDomainInfo specific
  CPPUNIT_ASSERT_EQUAL(0, br_domain_info_cmd->get_ticketNumber());
}

void BrDomainInfoTest::command_test()
{
  BrDomainInfo domain_info;
  BrDomainInfoCmd *domain_info_cmd = domain_info.get_command();

  domain_info_cmd->set_name("EmmaWatsonIsCharming.com");
  domain_info_cmd->set_hosts_control("sub");

  AuthInfo auth;
  auth.set_roid("SH8013-REP");
  auth.set_pw("abc123");
  domain_info_cmd->set_authInfo(auth);

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/br_domain_info.xml");

    domain_info.get_command()->set_clTRID("ABC-12345");
    domain_info.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(domain_info.get_xml());
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

void BrDomainInfoTest::response_test()
{
  response_test(true);
  response_test(false);
}

void BrDomainInfoTest::response_test(bool withDNSSECExtension)
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
    "<domain:infData "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name>example.com.br</domain:name>"
    "<domain:roid>EXAMPLE1-REP</domain:roid>"
    "<domain:status s='pendingCreate'/>"
    "<domain:contact type='admin'>fan</domain:contact>"
    "<domain:contact type='billing'>fan</domain:contact>"
    "<domain:contact type='tech'>fan</domain:contact>"    
    "<domain:ns>"
    "<domain:hostAttr>"
    "<domain:hostName>ns1.example.net.br</domain:hostName>"
    "</domain:hostAttr>"
    "<domain:hostAttr>"
    "<domain:hostName>ns1.example.com.br</domain:hostName>"
    "<domain:hostAddr ip='v4'>192.0.2.1</domain:hostAddr>"
    "</domain:hostAttr>"
    "</domain:ns>"
    "<domain:clID>ClientX</domain:clID>"
    "<domain:crID>ClientX</domain:crID>"
    "<domain:crDate>2006-01-30T22:00:00.0Z</domain:crDate>"
    "<domain:upID>ClientX</domain:upID>"
    "<domain:upDate>2006-01-31T09:00:00.0Z</domain:upDate>"
    "</domain:infData>"
    "</resData>"
    "<extension>";

  if (withDNSSECExtension) {
    expected +=
      "<secDNS:infData "
      "xmlns:secDNS=\"urn:ietf:params:xml:ns:secDNS-1.0\" "
      "xsi:schemaLocation=\"urn:ietf:params:xml:ns:secDNS-1.0 "
      "secDNS-1.0.xsd\">"
      "<secDNS:dsData>"
      "<secDNS:keyTag>12345</secDNS:keyTag>"
      "<secDNS:alg>3</secDNS:alg>"
      "<secDNS:digestType>1</secDNS:digestType>"
      "<secDNS:digest>49FD46E6C4B45C55D4AC</secDNS:digest>"
      "<secDNS:maxSigLife>604800</secDNS:maxSigLife>"
      "<secDNS:keyData>"
      "<secDNS:flags>256</secDNS:flags>"
      "<secDNS:protocol>3</secDNS:protocol>"
      "<secDNS:alg>1</secDNS:alg>"
      "<secDNS:pubKey>AQPJ////4Q==</secDNS:pubKey>"
      "</secDNS:keyData>"
      "</secDNS:dsData>"
      "<secDNS:dsData>"
      "<secDNS:keyTag>12345</secDNS:keyTag>"
      "<secDNS:alg>3</secDNS:alg>"
      "<secDNS:digestType>1</secDNS:digestType>"
      "<secDNS:digest>49FD46E6C4B45C55D4AC</secDNS:digest>"
      "</secDNS:dsData>"
      "</secDNS:infData>";
  }

  expected +=
    "<brdomain:infData "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' " 
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>"
    "<brdomain:ticketNumber>123456</brdomain:ticketNumber>"
    "<brdomain:organization>"
    "005.506.560/0001-36"
    "</brdomain:organization>"
    "<brdomain:releaseProcessFlags flag2='1'/>"
    "<brdomain:pending>"    
    "<brdomain:doc status='notReceived'>"
    "<brdomain:docType>CNPJ</brdomain:docType>"
    "<brdomain:limit>2006-03-01T22:00:00.0Z</brdomain:limit>"
    "<brdomain:description lang='pt'>"
    "Cadastro Nacional da Pessoa Jurídica"
    "</brdomain:description>"
    "</brdomain:doc>"
    "<brdomain:dns status='queryTimeOut'>"
    "<brdomain:hostName>"
    "ns1.example.com.br"
    "</brdomain:hostName>"
    "<brdomain:limit>2006-02-13T22:00:00.0Z</brdomain:limit>"
    "</brdomain:dns>"
    "<brdomain:dns status='notInformed'>"
    "<brdomain:limit>2006-02-13T22:00:00.0Z</brdomain:limit>"
    "</brdomain:dns>"
    "<brdomain:releaseProc status='waiting'>"
    "<brdomain:limit>2006-02-01T22:00:00.0Z</brdomain:limit>"
    "</brdomain:releaseProc>"
    "</brdomain:pending>"
    "<brdomain:ticketNumberConc>"
    "123451"
    "</brdomain:ticketNumberConc>"
    "<brdomain:ticketNumberConc>"
    "123455"
    "</brdomain:ticketNumberConc>"
    "</brdomain:infData>"
    "</extension>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54322-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  string utf8_expected;
  StrUtil::iso88591_to_utf8(expected, utf8_expected);

  DomParser parser;
  parser.enable_validation("../docs/schemas");

  BrDomainInfo domain_info;

  bool exception_caught = false;
  try {
    domain_info.set_response(utf8_expected, &parser);
  } catch (const XmlException &e) {
    exception_caught = true;
    printf("\nXml Exception: code [%d] message [%s] low level message [%s]\n",
	   e.get_code(), e.get_msg().c_str(), e.get_low_level_msg().c_str());
    cout << endl << domain_info.get_xml() << endl;
  }
  CPPUNIT_ASSERT(!exception_caught);

  BrDomainInfoRsp* domain_info_rsp = domain_info.get_response();

  CPPUNIT_ASSERT(domain_info_rsp != 0);

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = domain_info_rsp->get_result_list();
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
  
  string result_lang = domain_info_rsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response +=
    ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<resData>"
    "<domain:infData "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>";

  //get_name
  //get_roid
  response +=
    "<domain:name>" + domain_info_rsp->get_name() + "</domain:name>"
    "<domain:roid>" + domain_info_rsp->get_roid() + "</domain:roid>";

  //get_status_set
  set<string> status_set = domain_info_rsp->get_status_set();
  set<string>::const_iterator it;
  for (it = status_set.begin(); it != status_set.end(); it++) {
    response += "<domain:status s='" + (*it) + "'/>";
  }

  //get_contacts
  map< string, string, less<string> > contacts = 
    domain_info_rsp->get_contacts();
  map< string, string, less<string> >::const_iterator it_map;
  for (it_map = contacts.begin(); it_map != contacts.end(); it_map++) {
    response += "<domain:contact type='" + (*it_map).first + "'>" +
      (*it_map).second + "</domain:contact>";
  }

  //get_nameservers
  response += "<domain:ns>";
  vector<struct NameServer> nameservers = domain_info_rsp->get_nameservers();
  vector<struct NameServer>::const_iterator it_ns;
  set<struct NSIPAddr>::const_iterator it_ip;
  for (it_ns = nameservers.begin(); it_ns != nameservers.end(); it_ns++) {
    response += "<domain:hostAttr>" ;
    response += "<domain:hostName>" + it_ns->name + "</domain:hostName>";
    for (it_ip = it_ns->ips.begin(); it_ip != it_ns->ips.end(); it_ip++) {
      response += "<domain:hostAddr";
      if ((*it_ip).version != "") {
	response += " ip='" + (*it_ip).version + "'";
      }
      response += ">" + (*it_ip).addr + "</domain:hostAddr>";
    }
    response += "</domain:hostAttr>";
  }
  response += "</domain:ns>";

  //get_clID
  response += "<domain:clID>" + domain_info_rsp->get_clID() +
    "</domain:clID>";

  //get_crID
  //get_crDate
  response += "<domain:crID>" + domain_info_rsp->get_crID() + "</domain:crID>"
    "<domain:crDate>" + domain_info_rsp->get_crDate() + "</domain:crDate>";

  //get_upID
  response += "<domain:upID>" + domain_info_rsp->get_upID() + "</domain:upID>";

  //get_upDate
  response +=
    "<domain:upDate>" + domain_info_rsp->get_upDate() + "</domain:upDate>";

  response +=
    "</domain:infData>"
    "</resData>";

  response +=
    "<extension>";

  // secDNS extension
  list<DSInfo> dsInfoList = domain_info_rsp->get_dsInfo();

  if (!dsInfoList.empty()) {
    response += "<secDNS:infData "
      "xmlns:secDNS=\"urn:ietf:params:xml:ns:secDNS-1.0\" "
      "xsi:schemaLocation=\"urn:ietf:params:xml:ns:secDNS-1.0 "
      "secDNS-1.0.xsd\">";

    list<DSInfo>::iterator ds_it;
    for (ds_it = dsInfoList.begin(); ds_it != dsInfoList.end(); ds_it++) {
      response +=
	"<secDNS:dsData>"
	"<secDNS:keyTag>" + StrUtil::to_string("%d", (*ds_it).get_key_tag()) +
	"</secDNS:keyTag>"
	"<secDNS:alg>" + StrUtil::to_string("%d", (*ds_it).get_algo()) +
	"</secDNS:alg>"
	"<secDNS:digestType>" +
	StrUtil::to_string("%d", (*ds_it).get_digest_type()) +
	"</secDNS:digestType>"
	"<secDNS:digest>" + (*ds_it).get_digest() + "</secDNS:digest>";

      unsigned int maxSigLife = (*ds_it).get_max_sig_life();
      if (maxSigLife > 0) {
	response += "<secDNS:maxSigLife>" +
	  StrUtil::to_string("%d", maxSigLife) + "</secDNS:maxSigLife>";
      } 

      if ((*ds_it).hasKeyData()) {
	DSInfo::KeyData kd = (*ds_it).get_key_data();
	response +=
	  "<secDNS:keyData>"
	  "<secDNS:flags>" + StrUtil::to_string("%d", kd._flags) +
	  "</secDNS:flags>"
	  "<secDNS:protocol>" + StrUtil::to_string("%d", kd._protocol) +
	  "</secDNS:protocol>"
	  "<secDNS:alg>" + StrUtil::to_string("%d", kd._algorithm) +
	  "</secDNS:alg>"
	  "<secDNS:pubKey>" + kd._pub_key + "</secDNS:pubKey>"
	  "</secDNS:keyData>";
      }

      response += "</secDNS:dsData>";
    }
    response += "</secDNS:infData>";
  }

  // brdomain extension
  response +=
    "<brdomain:infData "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' " 
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>"
    "<brdomain:ticketNumber>" + 
    StrUtil::to_string("%d", domain_info_rsp->get_ticketNumber()) +
    "</brdomain:ticketNumber>"
    "<brdomain:organization>" + domain_info_rsp->get_organization() +
    "</brdomain:organization>";
  
  // Release Process Flags
  struct ReleaseProcessFlags rpf = domain_info_rsp->get_releaseProcessFlags();
  if (rpf.flag1 == 1 || rpf.flag2 == 1 || rpf.flag3 == 1) {
    response +=
      "<brdomain:releaseProcessFlags";
    if (rpf.flag1 == 1) {
      response += " flag1='1'";
    }
    if (rpf.flag2 == 1) {
      response += " flag2='1'";
    }
    if (rpf.flag3 == 1) {
      response += " flag3='1'";
    }
    response += "/>";
  }
  
  // Pendings
  response +=
    "<brdomain:pending>";  

  PENDING_CONTAINER<struct PendingDoc> doc_pendings =
    domain_info_rsp->get_doc_pendings();
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

  PENDING_CONTAINER<struct PendingDns> dns_pendings =
    domain_info_rsp->get_dns_pendings();
  PENDING_CONTAINER<struct PendingDns>::const_iterator dns_it;
  
  for (dns_it = dns_pendings.begin(); dns_it != dns_pendings.end(); dns_it++) {
    response += "<brdomain:dns status='" + (*dns_it).status + "'>";
    if ((*dns_it).hostName != "") {
      response += "<brdomain:hostName>" + (*dns_it).hostName +
	"</brdomain:hostName>";
    }
    response += "<brdomain:limit>" + (*dns_it).limit + "</brdomain:limit>"
      "</brdomain:dns>";
  }

  struct PendingReleaseProc rel_pending = domain_info_rsp->get_rel_pending();
  if (rel_pending.status != "") {
    response +=
      "<brdomain:releaseProc status='" + rel_pending.status + "'>"
      "<brdomain:limit>" + rel_pending.limit + "</brdomain:limit>"
      "</brdomain:releaseProc>";
  }
  
  response += "</brdomain:pending>";
  
  set<int> concurrent_tickets = domain_info_rsp->get_concurrent_tickets();
  set<int>::const_iterator tkt_it;
  for (tkt_it = concurrent_tickets.begin(); tkt_it != concurrent_tickets.end();
       tkt_it++) {
    response +=
      "<brdomain:ticketNumberConc>" + StrUtil::to_string("%d", (*tkt_it)) +
      "</brdomain:ticketNumberConc>";
  }

  response +=    
    "</brdomain:infData>"
    "</extension>"
    "<trID>"
    "<clTRID>" + domain_info_rsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + domain_info_rsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(expected, response);

  // tests reset method
  domain_info_rsp->reset();
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_name());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_roid());
  CPPUNIT_ASSERT(domain_info_rsp->get_status_set().empty());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_registrant());
  CPPUNIT_ASSERT(domain_info_rsp->get_contacts().empty());
  CPPUNIT_ASSERT(domain_info_rsp->get_nameservers().empty());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_clID());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_crID());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_crDate());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_upID());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_exDate());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_upDate());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_trDate());
  CPPUNIT_ASSERT_EQUAL((const string)"",
		       domain_info_rsp->get_authInfo().get_roid());
  CPPUNIT_ASSERT_EQUAL((const string)"",
		       domain_info_rsp->get_authInfo().get_pw());

  // BrDomainInfoRsp specific attributes
  CPPUNIT_ASSERT_EQUAL(0, domain_info_rsp->get_ticketNumber());
  CPPUNIT_ASSERT_EQUAL((string)"", domain_info_rsp->get_organization());
  
  rpf = domain_info_rsp->get_releaseProcessFlags();
  CPPUNIT_ASSERT_EQUAL(0, rpf.flag1 + rpf.flag2 + rpf.flag3);

  CPPUNIT_ASSERT_EQUAL(true, domain_info_rsp->get_doc_pendings().empty());
  CPPUNIT_ASSERT_EQUAL(true, domain_info_rsp->get_dns_pendings().empty());
  CPPUNIT_ASSERT_EQUAL((string)"", domain_info_rsp->get_rel_pending().status);
  CPPUNIT_ASSERT_EQUAL((string)"", domain_info_rsp->get_rel_pending().limit);
  CPPUNIT_ASSERT_EQUAL(true, domain_info_rsp->get_concurrent_tickets().empty());
}


void BrDomainInfoTest::response_test2()
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
    "<domain:infData "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name>example.com.br</domain:name>"
    "<domain:roid>EXAMPLE1-REP</domain:roid>"
    "<domain:status s='pendingCreate'/>"
    "<domain:contact type='admin'>fan</domain:contact>"
    "<domain:contact type='billing'>fan</domain:contact>"
    "<domain:contact type='tech'>fan</domain:contact>"    
    "<domain:ns>"
    "<domain:hostAttr>"
    "<domain:hostName>ns1.example.com.br</domain:hostName>"
    "<domain:hostAddr ip='v4'>192.0.2.1</domain:hostAddr>"
    "</domain:hostAttr>"
    "<domain:hostAttr>"
    "<domain:hostName>ns1.example.net.br</domain:hostName>"
    "</domain:hostAttr>"
    "</domain:ns>"
    "<domain:clID>ClientX</domain:clID>"
    "<domain:crID>ClientX</domain:crID>"
    "<domain:crDate>2006-01-30T22:00:00.0Z</domain:crDate>"
    "<domain:upID>ClientX</domain:upID>"
    "<domain:upDate>2006-01-31T09:00:00.0Z</domain:upDate>"
    "</domain:infData>"
    "</resData>"
    "<extension>"
    "<brdomain:infData "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' " 
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>"
    "<brdomain:organization>"
    "005.506.560/0001-36"
    "</brdomain:organization>"
    "<brdomain:publicationStatus publicationFlag=\"onHold\">"
    "<brdomain:onHoldReason>billing</brdomain:onHoldReason>"
    "<brdomain:onHoldReason>dns</brdomain:onHoldReason>"
    "</brdomain:publicationStatus>"
    "<brdomain:autoRenew active=\"1\"/>"
    "</brdomain:infData>"
    "</extension>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54322-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  string utf8_expected;
  StrUtil::iso88591_to_utf8(expected, utf8_expected);

  DomParser parser;
  parser.enable_validation("../docs/schemas");

  BrDomainInfo domain_info;

  bool exception_caught = false;
  try {
    domain_info.set_response(utf8_expected, &parser);
  } catch (const XmlException &e) {
    exception_caught = true;
    printf("\nXml Exception: code [%d] message [%s] low level message [%s]\n",
	   e.get_code(), e.get_msg().c_str(), e.get_low_level_msg().c_str());
    cout << endl << domain_info.get_xml() << endl;
  }
  CPPUNIT_ASSERT(!exception_caught);

  BrDomainInfoRsp* domain_info_rsp = domain_info.get_response();

  CPPUNIT_ASSERT(domain_info_rsp != 0);

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = domain_info_rsp->get_result_list();
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
  
  string result_lang = domain_info_rsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response +=
    ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<resData>"
    "<domain:infData "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>";

  //get_name
  //get_roid
  response +=
    "<domain:name>" + domain_info_rsp->get_name() + "</domain:name>"
    "<domain:roid>" + domain_info_rsp->get_roid() + "</domain:roid>";

  //get_status_set
  set<string> status_set = domain_info_rsp->get_status_set();
  set<string>::const_iterator it;
  for (it = status_set.begin(); it != status_set.end(); it++) {
    response += "<domain:status s='" + (*it) + "'/>";
  }

  //get_contacts
  map< string, string, less<string> > contacts = 
    domain_info_rsp->get_contacts();
  map< string, string, less<string> >::const_iterator it_map;
  for (it_map = contacts.begin(); it_map != contacts.end(); it_map++) {
    response += "<domain:contact type='" + (*it_map).first + "'>" +
      (*it_map).second + "</domain:contact>";
  }

  //get_nameservers
  response += "<domain:ns>";
  vector<struct NameServer> nameservers = domain_info_rsp->get_nameservers();
  vector<struct NameServer>::const_iterator it_ns;
  set<struct NSIPAddr>::const_iterator it_ip;
  for (it_ns = nameservers.begin(); it_ns != nameservers.end(); it_ns++) {
    response += "<domain:hostAttr>" ;
    response += "<domain:hostName>" + it_ns->name + "</domain:hostName>";
    for (it_ip = it_ns->ips.begin(); it_ip != it_ns->ips.end(); it_ip++) {
      response += "<domain:hostAddr";
      if ((*it_ip).version != "") {
	response += " ip='" + (*it_ip).version + "'";
      }
      response += ">" + (*it_ip).addr + "</domain:hostAddr>";
    }
    response += "</domain:hostAttr>";
  }
  response += "</domain:ns>";

  //get_clID
  response += "<domain:clID>" + domain_info_rsp->get_clID() +
    "</domain:clID>";

  //get_crID
  //get_crDate
  response += "<domain:crID>" + domain_info_rsp->get_crID() + "</domain:crID>"
    "<domain:crDate>" + domain_info_rsp->get_crDate() + "</domain:crDate>";

  //get_upID
  response += "<domain:upID>" + domain_info_rsp->get_upID() + "</domain:upID>";

  //get_upDate
  response +=
    "<domain:upDate>" + domain_info_rsp->get_upDate() + "</domain:upDate>";

  response +=
    "</domain:infData>"
    "</resData>";

  response +=
    "<extension>"
    "<brdomain:infData "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' " 
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>"
    "<brdomain:organization>" + domain_info_rsp->get_organization() +
    "</brdomain:organization>";
  
  string publicationFlag = domain_info_rsp->get_publication_flag();

  response += "<brdomain:publicationStatus publicationFlag=\"" + 
    publicationFlag + "\">";
  
  set<string> onhold_reasons = domain_info_rsp->get_onhold_reasons();

  set<string>::const_iterator tkt_it;
  for (tkt_it = onhold_reasons.begin(); tkt_it != onhold_reasons.end();
       tkt_it++) {
    response +=
      "<brdomain:onHoldReason>" +  (*tkt_it) +
      "</brdomain:onHoldReason>";
  }

  response +=    
    "</brdomain:publicationStatus><brdomain:autoRenew active=\""
     + StrUtil::to_string("%d", domain_info_rsp->get_auto_renew()) + 
    "\"/></brdomain:infData>"
    "</extension>"
    "<trID>"
    "<clTRID>" + domain_info_rsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + domain_info_rsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(expected, response);

  // tests reset method
  domain_info_rsp->reset();
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_name());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_roid());
  CPPUNIT_ASSERT(domain_info_rsp->get_status_set().empty());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_registrant());
  CPPUNIT_ASSERT(domain_info_rsp->get_contacts().empty());
  CPPUNIT_ASSERT(domain_info_rsp->get_nameservers().empty());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_clID());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_crID());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_crDate());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_upID());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_exDate());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_upDate());
  CPPUNIT_ASSERT_EQUAL((const string)"", domain_info_rsp->get_trDate());
  CPPUNIT_ASSERT_EQUAL((const string)"",
		       domain_info_rsp->get_authInfo().get_roid());
  CPPUNIT_ASSERT_EQUAL((const string)"",
		       domain_info_rsp->get_authInfo().get_pw());

  // BrDomainInfoRsp specific attributes
  CPPUNIT_ASSERT_EQUAL((string)"", domain_info_rsp->get_organization());
  CPPUNIT_ASSERT_EQUAL((string)"", domain_info_rsp->get_publication_flag());
  CPPUNIT_ASSERT_EQUAL(true, domain_info_rsp->get_onhold_reasons().empty());
  CPPUNIT_ASSERT_EQUAL(-1, domain_info_rsp->get_auto_renew());
}
#endif //USE_BR_DOMAINS
