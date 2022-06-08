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
/* $Id: DomainInfoTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#include <memory>
#include <string>

#include "libepp_nicbr.H"

#include "DomainInfoTest.H"
#include "DomainInfo.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

using std::auto_ptr;

#include <iostream>
using namespace std;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(DomainInfoTest);

DomainInfoTest::DomainInfoTest() {}

DomainInfoTest::~DomainInfoTest() {}

void DomainInfoTest::setUp() {}

void DomainInfoTest::tearDown() {}

void DomainInfoTest::set_xml_template_test()
{
  string to_be_parsed =
    "<command>"
      "<info>"
        "<domain:info "
          "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
          "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
          "domain-1.0.xsd'>"
          "<domain:name hosts='$(hosts_control)$'>"
          "$(name)$</domain:name>"
          "$(auth_info)$"
        "</domain:info>"
      "</info>"
    "$(clTRID)$"
    "</command>";

  DomainInfo domain_info;
  DomainInfoCmd* domain_info_cmd = domain_info.get_command();
  domain_info_cmd->set_name("EmmaWatsonIsCharming.com");
  domain_info_cmd->set_hosts_control("sub");

  AuthInfo auth;
  auth.set_roid("SH8013-REP");
  auth.set_pw("abc123");
  domain_info_cmd->set_authInfo(auth);

  domain_info.get_command()->set_clTRID("ABC-12345");
  domain_info.set_xml_template(to_be_parsed);

  string expected =
    "<command>"
      "<info>"
        "<domain:info "
          "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
          "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
          "domain-1.0.xsd'>"
          "<domain:name hosts='sub'>EmmaWatsonIsCharming.com</domain:name>"
          "<domain:authInfo>"
            "<domain:pw roid='SH8013-REP'>abc123</domain:pw>"
          "</domain:authInfo>"
        "</domain:info>"
        "</info>"
      "<clTRID>ABC-12345</clTRID>"
    "</command>";

  CPPUNIT_ASSERT_EQUAL(expected, domain_info.get_xml());

  // tests reset method
  domain_info_cmd = domain_info.get_command();
  domain_info_cmd->reset();

  CPPUNIT_ASSERT(domain_info_cmd->get_name() == "");
  CPPUNIT_ASSERT(domain_info_cmd->get_hosts_control() == "all");

  CPPUNIT_ASSERT(domain_info_cmd->get_authInfo().get_pw() == "");
  CPPUNIT_ASSERT(domain_info_cmd->get_authInfo().get_roid() == "");

  // parent attribute
  CPPUNIT_ASSERT(domain_info_cmd->get_clTRID() == "");
}

void DomainInfoTest::command_test()
{
  DomainInfo domain_info;
  DomainInfoCmd *domain_info_cmd = domain_info.get_command();

  domain_info_cmd->set_name("EmmaWatsonIsCharming.com");
  domain_info_cmd->set_hosts_control("sub");

  AuthInfo auth;
  auth.set_roid("SH8013-REP");
  auth.set_pw("abc123");
  domain_info_cmd->set_authInfo(auth);

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/domain_info.xml");

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

void DomainInfoTest::response_test()
{
  response_test(true);
  response_test(false);
}

void DomainInfoTest::response_test(bool withDNSSECExtension)
{
  string expected =
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
    "<epp xmlns=\"urn:ietf:params:xml:ns:epp-1.0\" "
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd\">"
    "<response>"
    "<result code=\"1000\">"
    "<msg>Command completed successfully</msg>"
    "</result>"
    "<resData>"
    "<domain:infData "
    "xmlns:domain=\"urn:ietf:params:xml:ns:domain-1.0\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd\">"
    "<domain:name>EmmaWatsonIsCharming.org</domain:name>"
    "<domain:roid>YADAYADAYADA-ABC</domain:roid>"
    "<domain:status s=\"clientHold\"/>"
    "<domain:status s=\"serverUpdateProhibited\"/>"
    "<domain:registrant>SH8013-REP</domain:registrant>"
    "<domain:contact type=\"admin\">Voldemort</domain:contact>"
    "<domain:contact type=\"tech\">Dumbledore</domain:contact>"
    "<domain:ns>"
    "<domain:hostAttr>"
    "<domain:hostName>ns1.EmmaWatsonIsCharming.org"
    "</domain:hostName>"
    "<domain:hostAddr ip='v4'>192.168.0.1</domain:hostAddr>"
    "</domain:hostAttr>"
    "<domain:hostAttr>"
    "<domain:hostName>ns1.granger.com</domain:hostName>"
    "</domain:hostAttr>"
    "</domain:ns>"
    "<domain:clID>MrPotter</domain:clID>"
    "<domain:crID>Weasley</domain:crID>"
    "<domain:crDate>1999-04-03T22:00:00.0Z</domain:crDate>"
    "<domain:upID>Sirius</domain:upID>"
    "<domain:upDate>2002-12-12T22:00:00.0Z</domain:upDate>"
    "<domain:exDate>2001-04-03T22:00:00.0Z</domain:exDate>"
    "<domain:trDate>2005-01-03T22:00:00.0Z</domain:trDate>"
    "<domain:authInfo>"
    "<domain:pw>FooBar321</domain:pw>"
    "</domain:authInfo>"
    "</domain:infData>"
    "</resData>";

  if (withDNSSECExtension) {
    expected +=
      "<extension>"
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
      "</secDNS:infData>"
      "</extension>";
  }

  expected +=
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54321-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  DomParser parser;
  parser.enable_validation("../docs/schemas");

  DomainInfo domain_info;
  CPPUNIT_ASSERT_NO_THROW(domain_info.set_response(expected, &parser));

  DomainInfoRsp* domain_info_rsp = domain_info.get_response();

  CPPUNIT_ASSERT(domain_info_rsp != 0);
  string result_code = "1000";

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = domain_info_rsp->get_result_list();
  r_it = results.begin();

  CPPUNIT_ASSERT(r_it != results.end());
  CPPUNIT_ASSERT_EQUAL(Response::OK, r_it->first);

  string response =
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
    "<epp xmlns=\"urn:ietf:params:xml:ns:epp-1.0\" "
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd\">"
    "<response>"
    "<result code=\"" + result_code + "\">"
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
    "xmlns:domain=\"urn:ietf:params:xml:ns:domain-1.0\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd\">";

  //get_name
  //get_roid
  response +=
    "<domain:name>" + domain_info_rsp->get_name() + "</domain:name>"
    "<domain:roid>" + domain_info_rsp->get_roid() + "</domain:roid>";

  //get_status_set
  set<string> status_set = domain_info_rsp->get_status_set();
  set<string>::const_iterator it;
  for (it = status_set.begin(); it != status_set.end(); it++) {
    response += "<domain:status s=\"" + (*it) + "\"/>";
  }

  //get_registrant
  response +=
    "<domain:registrant>" + domain_info_rsp->get_registrant() +
    "</domain:registrant>";

  //get_contacts
  map< string, string, less<string> > contacts = domain_info_rsp->get_contacts();
  map< string, string, less<string> >::const_iterator it_map;
  for (it_map = contacts.begin(); it_map != contacts.end(); it_map++) {
    response += "<domain:contact type=\"" + (*it_map).first + "\">" +
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
  //get_exDate
  //get_trDate
  response +=
    "<domain:upDate>" + domain_info_rsp->get_upDate() + "</domain:upDate>"
    "<domain:exDate>" + domain_info_rsp->get_exDate() + "</domain:exDate>"
    "<domain:trDate>" + domain_info_rsp->get_trDate() + "</domain:trDate>";

  //get_pw
  response +=
    "<domain:authInfo>"
    "<domain:pw>" + domain_info_rsp->get_authInfo().get_pw() + "</domain:pw>"
    "</domain:authInfo>";

  response +=
    "</domain:infData>"
    "</resData>";

  // secDNS extension
  list<DSInfo> dsInfoList = domain_info_rsp->get_dsInfo();

  if (!dsInfoList.empty()) {
    response += "<extension>";
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
    response += "</extension>";
  }

  response +=
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
}
