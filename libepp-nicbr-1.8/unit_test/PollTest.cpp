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
/* $Id: PollTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#include <memory>
#include <string>

#include "libepp_nicbr.H"

#include "PollTest.H"
#include "Poll.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(PollTest);

PollTest::PollTest() {}

PollTest::~PollTest() {}

void PollTest::setUp() {}

void PollTest::tearDown() {}

void PollTest::set_xml_template_test()
{
  // tests request command
  string to_be_parsed =
     "<command>"
       "<poll op='$(op)$'$(msgID)$/>"
    "$(clTRID)$"
     "</command>";

  Poll poll;
  PollCmd* poll_cmd = poll.get_command();
  poll_cmd->set_op("req");
  poll_cmd->set_msgID("12345"); 
  
  poll.get_command()->set_clTRID("ABC-12345");
  poll.set_xml_template(to_be_parsed);
  
  string expected =
    "<command>"
      "<poll op='req'/>"
      "<clTRID>ABC-12345</clTRID>"
     "</command>";

  // tests acknowledgement command
  to_be_parsed =
     "<command>"
       "<poll op='$(op)$'$(msgID)$/>"
    "$(clTRID)$"
     "</command>";

  poll_cmd->reset();
  poll_cmd->set_op("ack");
  poll_cmd->set_msgID("12345");
  
  poll.get_command()->set_clTRID("ABC-12345");
  poll.set_xml_template(to_be_parsed);
  
  expected =
    "<command>"
      "<poll op='ack' msgID='12345'/>"
      "<clTRID>ABC-12345</clTRID>"
     "</command>";

  CPPUNIT_ASSERT(expected == poll.get_xml());
  
  // tests reset method
  CPPUNIT_ASSERT(poll_cmd->get_op() != "");
  CPPUNIT_ASSERT(poll_cmd->get_msgID() != "");
  poll_cmd->reset();
  CPPUNIT_ASSERT(poll_cmd->get_op() == "");
  CPPUNIT_ASSERT(poll_cmd->get_msgID() == "");
  // parent attribute
  CPPUNIT_ASSERT(poll_cmd->get_clTRID() == "");
}

void PollTest::command_test()
{
  Poll poll;
  PollCmd *poll_cmd = poll.get_command();

  poll_cmd->set_op("req");
  poll_cmd->set_msgID("12345"); 

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/poll.xml");

    poll.get_command()->set_clTRID("ABC-12345");
    poll.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(poll.get_xml());
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

void PollTest::response_test()
{
  string expected =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<response>"
    "<result code='1301'>"
    "<msg>Command completed successfully; ack to dequeue</msg>"
    "</result>"
    "<msgQ count='5' id='12345'>"
    "<qDate>2000-06-08T22:10:00.0Z</qDate>"
    "<msg lang='en'>"
    "<bal type='currency'>5</bal>"
    "<limit type='currency'>100</limit>"
    "<text>Credit Balance Low</text>"
    "</msg>"
    "</msgQ>"
    "<resData>"
    "<contact:chkData "
    "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd'>"
    "<contact:cd>"
    "<contact:id avail='1'>btw214</contact:id>"
    "</contact:cd>"
    "<contact:cd>"
    "<contact:id avail='0'>mrf348</contact:id>"
    "<contact:reason>In use</contact:reason>"
    "</contact:cd>"
    "<contact:cd>"
    "<contact:id avail='1'>sht145</contact:id>"
    "</contact:cd>"
    "</contact:chkData>"
    "</resData>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>DEF-54321</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  DomParser parser;
  parser.enable_validation("../docs/schemas");
  Poll poll;
  CPPUNIT_ASSERT_NO_THROW(poll.set_response(expected, &parser));

  string result_code = "1301";
  PollRsp* poll_rsp = poll.get_response();
 
  map< string, PollRsp::MsgContent, less<string> > content =
    poll_rsp->get_content();
  map< string, PollRsp::MsgContent, less<string> >::const_iterator it;
 
  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = poll_rsp->get_result_list();
  r_it = results.begin();

  CPPUNIT_ASSERT(r_it != results.end());
  CPPUNIT_ASSERT_EQUAL(Response::OK_ACK_DEQUEUE, r_it->first);
  
  string response =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<response>"
    "<result code='" + result_code + "'>"
    "<msg";
  
  string result_lang = poll_rsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response +=
    ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<msgQ count='" + poll_rsp->get_count() + "' id='" +
    poll_rsp->get_id() + "'>"
    "<qDate>" + poll_rsp->get_qDate() + "</qDate>"
    "<msg lang='" + poll_rsp->get_lang() + "'>" + poll_rsp->get_text();
  
  for (it = content.begin(); it != content.end(); it++) {
    string elem_name = (*it).first;
    PollRsp::MsgContent msg_content = (*it).second;
    map< string, string, less<string> > attrs = msg_content.attributes;
    map< string, string, less<string> >::const_iterator attrs_it;
    response += "<" + elem_name;
    for (attrs_it = attrs.begin(); attrs_it != attrs.end(); attrs_it++) {
      string attr_name = (*attrs_it).first;
      string attr_value = (*attrs_it).second;
      response += " " + attr_name + "='" + attr_value + "'";
    }
    response += ">" + msg_content.value + "</" + elem_name + ">";
  }
  
  response +=
    "</msg>"
    "</msgQ>"
    "<resData>"
    "<contact:chkData "
    "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd'>";
  
  CPPUNIT_ASSERT_EQUAL(CONTACT_CHECK, poll_rsp->get_response_type());
  
  ContactCheckRsp *contact_check_rsp =
    (ContactCheckRsp *)poll_rsp->get_response();
  map< string, ContactCheckRsp::Availability, less<string> > avail = 
    contact_check_rsp->get_availability_list();
  map< string, ContactCheckRsp::Availability, less<string> >::const_iterator avail_it;

  for (avail_it = avail.begin(); avail_it != avail.end(); avail_it++) {
    response += "<contact:cd><contact:id avail='" +
      (*avail_it).second.available + "'>"
      + (*avail_it).first + "</contact:id>";
    if (strcmp((*avail_it).second.reason.c_str(), "") != 0) {
      response += "<contact:reason>" + (*avail_it).second.reason + 
	"</contact:reason>";
    }
    response += "</contact:cd>";
  }
  response += 
    "</contact:chkData>"
    "</resData>"
    "<trID>"
    "<clTRID>" + poll_rsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + poll_rsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";
  
  CPPUNIT_ASSERT_EQUAL(expected, response);

  /* Reset Test */
  CPPUNIT_ASSERT(poll_rsp->get_count() != "");
  CPPUNIT_ASSERT(poll_rsp->get_id() != "");
  CPPUNIT_ASSERT(poll_rsp->get_lang() != "");
  CPPUNIT_ASSERT(poll_rsp->get_response_type() != UNSET_ACTION);
  CPPUNIT_ASSERT(!poll_rsp->get_content().empty());
  
  poll_rsp->reset();
  
  CPPUNIT_ASSERT(poll_rsp->get_count() == "");
  CPPUNIT_ASSERT(poll_rsp->get_id() == "");
  CPPUNIT_ASSERT(poll_rsp->get_lang() == "en");
  CPPUNIT_ASSERT(poll_rsp->get_text() == "");
  CPPUNIT_ASSERT(poll_rsp->get_response_type() == UNSET_ACTION);
  CPPUNIT_ASSERT(poll_rsp->get_content().empty());  
}

#if USE_BR_DOMAINS
void PollTest::extension_response_test()
{
  string expected =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<response>"
    "<result code='1301'>"
    "<msg>Command completed successfully; ack to dequeue</msg>"
    "</result>"
    "<msgQ count='5' id='12345'>"
    "<qDate>2000-06-08T22:10:00.0Z</qDate>"
    "<msg lang='en'>"
    "<bal type='currency'>5</bal>"
    "<limit type='currency'>100</limit>"
    "<text>Credit Balance Low</text>"
    "</msg>"
    "</msgQ>"
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
    "<brdomain:ticketNumber>123456</brdomain:ticketNumber>"
    "<brdomain:organization>"
    "005.506.560/0001-36"
    "</brdomain:organization>"
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
    "<svTRID>DEF-54321</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  DomParser parser;
  parser.enable_validation("../docs/schemas");
  Poll poll;
  string expected_utf8;
  StrUtil::iso88591_to_utf8(expected, expected_utf8);
  CPPUNIT_ASSERT_NO_THROW(poll.set_response(expected_utf8, &parser));

  string result_code = "1301";
  PollRsp* poll_rsp = poll.get_response();
 
  map< string, PollRsp::MsgContent, less<string> > content =
    poll_rsp->get_content();
  map< string, PollRsp::MsgContent, less<string> >::const_iterator msg_it;
 
  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = poll_rsp->get_result_list();
  r_it = results.begin();

  CPPUNIT_ASSERT(r_it != results.end());
  CPPUNIT_ASSERT_EQUAL(Response::OK_ACK_DEQUEUE, r_it->first);
  
  string response =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<response>"
    "<result code='" + result_code + "'>"
    "<msg";
  
  string result_lang = poll_rsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response +=
    ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<msgQ count='" + poll_rsp->get_count() + "' id='" +
    poll_rsp->get_id() + "'>"
    "<qDate>" + poll_rsp->get_qDate() + "</qDate>"
    "<msg lang='" + poll_rsp->get_lang() + "'>" + poll_rsp->get_text();
  
  for (msg_it = content.begin(); msg_it != content.end(); msg_it++) {
    string elem_name = (*msg_it).first;
    PollRsp::MsgContent msg_content = (*msg_it).second;
    map< string, string, less<string> > attrs = msg_content.attributes;
    map< string, string, less<string> >::const_iterator attrs_it;
    response += "<" + elem_name;
    for (attrs_it = attrs.begin(); attrs_it != attrs.end(); attrs_it++) {
      string attr_name = (*attrs_it).first;
      string attr_value = (*attrs_it).second;
      response += " " + attr_name + "='" + attr_value + "'";
    }
    response += ">" + msg_content.value + "</" + elem_name + ">";
  }
  
  response +=
    "</msg>"
    "</msgQ>"
    "<resData>"
    "<domain:infData "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>";
  
  CPPUNIT_ASSERT_EQUAL(BR_DOMAIN_INFO, poll_rsp->get_response_type());
  BrDomainInfoRsp *br_domain_info_rsp = 
    (BrDomainInfoRsp *)poll_rsp->get_response();
  
  //get_name
  //get_roid
  response +=
    "<domain:name>" + br_domain_info_rsp->get_name() + "</domain:name>"
    "<domain:roid>" + br_domain_info_rsp->get_roid() + "</domain:roid>";

  //get_status_set
  set<string> status_set = br_domain_info_rsp->get_status_set();
  set<string>::const_iterator status_it;
  for (status_it = status_set.begin(); status_it != status_set.end(); 
       status_it++) {
    response += "<domain:status s='" + (*status_it) + "'/>";
  }

  //get_contacts
  map< string, string, less<string> > contacts = 
    br_domain_info_rsp->get_contacts();
  map< string, string, less<string> >::const_iterator it_map;
  for (it_map = contacts.begin(); it_map != contacts.end(); it_map++) {
    response += "<domain:contact type='" + (*it_map).first + "'>" +
      (*it_map).second + "</domain:contact>";
  }

  //get_nameservers
  response += "<domain:ns>";
  vector<struct NameServer> nameservers = br_domain_info_rsp->get_nameservers();
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
  response += "<domain:clID>" + br_domain_info_rsp->get_clID() +
    "</domain:clID>";

  //get_crID
  //get_crDate
  response += "<domain:crID>" + br_domain_info_rsp->get_crID() + 
    "</domain:crID>"
    "<domain:crDate>" + br_domain_info_rsp->get_crDate() + "</domain:crDate>";

  //get_upID
  response += "<domain:upID>" + br_domain_info_rsp->get_upID() +
    "</domain:upID>";

  //get_upDate
  response +=
    "<domain:upDate>" + br_domain_info_rsp->get_upDate() + "</domain:upDate>";

  response +=
    "</domain:infData>"
    "</resData>";

  response +=
    "<extension>"
    "<brdomain:infData "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' " 
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>"
    "<brdomain:ticketNumber>" + 
    StrUtil::to_string("%d", br_domain_info_rsp->get_ticketNumber()) +
    "</brdomain:ticketNumber>"
    "<brdomain:organization>" + br_domain_info_rsp->get_organization() +
    "</brdomain:organization>"
    "<brdomain:pending>";  

  PENDING_CONTAINER<struct PendingDoc> doc_pendings =
    br_domain_info_rsp->get_doc_pendings();
  PENDING_CONTAINER<struct PendingDoc>::const_iterator doc_it;
  
  for (doc_it = doc_pendings.begin(); doc_it != doc_pendings.end();
       doc_it++) {
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
    br_domain_info_rsp->get_dns_pendings();
  PENDING_CONTAINER<struct PendingDns>::const_iterator dns_it;
  
  for (dns_it = dns_pendings.begin(); dns_it != dns_pendings.end();
       dns_it++) {
    response += 
      "<brdomain:dns status='" + (*dns_it).status + "'>"
      "<brdomain:hostName>" + (*dns_it).hostName + "</brdomain:hostName>"
      "<brdomain:limit>" + (*dns_it).limit + "</brdomain:limit>"
      "</brdomain:dns>";
  }

  struct PendingReleaseProc rel_pending = 
    br_domain_info_rsp->get_rel_pending();
  if (rel_pending.status != "") {
    response +=
      "<brdomain:releaseProc status='" + rel_pending.status + "'>"
      "<brdomain:limit>" + rel_pending.limit + "</brdomain:limit>"
      "</brdomain:releaseProc>";
  }
  
  response += "</brdomain:pending>";
  
  set<int> concurrent_tickets = br_domain_info_rsp->get_concurrent_tickets();
  set<int>::const_iterator tkt_it;
  for (tkt_it = concurrent_tickets.begin(); tkt_it != concurrent_tickets.end();
       tkt_it++) {
    response +=
      "<brdomain:ticketNumberConc>" + StrUtil::to_string("%d", (*tkt_it)) +
      "</brdomain:ticketNumberConc>";
  }

  response +=    
    "</brdomain:infData>"
    "</extension>";

  response +=     
    "<trID>"
    "<clTRID>" + poll_rsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + poll_rsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";
  
  CPPUNIT_ASSERT_EQUAL(expected, response); 
  
}
#endif //USE_BR_DOMAINS

void PollTest::pandata_test() 
{
  string expected =    
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<response>"
    "<result code='1301'>"
    "<msg>Command completed successfully; ack to dequeue</msg>"
    "</result>"
    "<msgQ count='5' id='12345'>"
    "<qDate>1999-04-04T22:01:00.0Z</qDate>"
    "<msg lang='en'>Pending action completed successfully.</msg>"
    "</msgQ>"
    "<resData>"
    "<domain:panData "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name paResult='1'>example.com</domain:name>"
    "<domain:paTRID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54321-XYZ</svTRID>"
    "</domain:paTRID>"
    "<domain:paDate>1999-04-04T22:00:00.0Z</domain:paDate>"
    "</domain:panData>"
    "</resData>"
    "<trID>"
    "<clTRID>BCD-23456</clTRID>"
    "<svTRID>65432-WXY</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  DomParser parser;
  parser.enable_validation("../docs/schemas");
  Poll poll;
  CPPUNIT_ASSERT_NO_THROW(poll.set_response(expected, &parser));
  
  string result_code = "1301";
  PollRsp* poll_rsp = poll.get_response();
  
  map< string, PollRsp::MsgContent, less<string> > content =
    poll_rsp->get_content();
  map< string, PollRsp::MsgContent, less<string> >::const_iterator msg_it;
  
  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = poll_rsp->get_result_list();
  r_it = results.begin();

  CPPUNIT_ASSERT(r_it != results.end());
  CPPUNIT_ASSERT_EQUAL(Response::OK_ACK_DEQUEUE, r_it->first);
  
  string response =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<response>"
    "<result code='" + result_code + "'>"
    "<msg";

  string result_lang = poll_rsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }

  response +=
    ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<msgQ count='" + poll_rsp->get_count() + "' id='" +
    poll_rsp->get_id() + "'>"
    "<qDate>" + poll_rsp->get_qDate() + "</qDate>"
    "<msg lang='" + poll_rsp->get_lang() +"'>" +
    poll_rsp->get_text();  
  
  for (msg_it = content.begin(); msg_it != content.end(); msg_it++) {
    string elem_name = (*msg_it).first;
    PollRsp::MsgContent msg_content = (*msg_it).second;
    map< string, string, less<string> > attrs = msg_content.attributes;
    map< string, string, less<string> >::const_iterator attrs_it;
    response += "<" + elem_name;
    for (attrs_it = attrs.begin(); attrs_it != attrs.end(); attrs_it++) {
      string attr_name = (*attrs_it).first;
      string attr_value = (*attrs_it).second;
      response += " " + attr_name + "='" + attr_value + "'";
    }
    response += ">" + msg_content.value + "</" + elem_name + ">";
  }
  
  response +=
    "</msg>"
    "</msgQ>";

  // resData
  CPPUNIT_ASSERT_EQUAL(DOMAIN_PANDATA, poll_rsp->get_response_type());
  PanDataRsp * pandata_rsp = 
    (PanDataRsp *)poll_rsp->get_response();

  response +=
    "<resData>"
    "<domain:panData "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>";

  // domain:name
  response +=
    "<domain:name paResult='";
  
  if (pandata_rsp->get_paResult() == true) {
    response += "1'>";
  } else {
    response += "0'>";
  }

  response +=
    pandata_rsp->get_object_id() + "</domain:name>";
  
  // domain:paTRID
  response +=
    "<domain:paTRID>";
  string clTRID_pandata = pandata_rsp->get_clTRID();
  string svTRID_pandata = pandata_rsp->get_svTRID();
  if (clTRID_pandata != "") {
    response += 
      "<clTRID>" + clTRID_pandata + "</clTRID>";
  }

  response +=
    "<svTRID>" + svTRID_pandata + "</svTRID>"
    "</domain:paTRID>";
  
  // domain:paDate
  response +=
    "<domain:paDate>" + pandata_rsp->get_paDate() + "</domain:paDate>"
    "</domain:panData>"
    "</resData>"
    "<trID>"
    "<clTRID>" + poll_rsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + poll_rsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";
  
  CPPUNIT_ASSERT_EQUAL(expected, response);  
}

#if USE_BR_DOMAINS
void PollTest::brdomain_pandata_test()
{

  string expected =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<response>"
    "<result code='1301'>"
    "<msg>Command completed successfully; ack to dequeue</msg>"
    "</result>"
    "<msgQ count='5' id='12345'>"
    "<qDate>1999-04-04T22:01:00.0Z</qDate>"
    "<msg>Pending action completed successfully.</msg>"
    "</msgQ>"
    "<resData>"
    "<domain:panData "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>"
    "<domain:name paResult='1'>example.com.br</domain:name>"
    "<domain:paTRID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54321-XYZ</svTRID>"
    "</domain:paTRID>"
    "<domain:paDate>2006-01-30T22:00:00.0Z</domain:paDate>"
    "</domain:panData>"
    "</resData>"
    "<extension>"
    "<brdomain:panData "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>"
    "<brdomain:ticketNumber>123456</brdomain:ticketNumber>"
    "<brdomain:reason lang='pt'>"
    "Nao obtivemos uma resposta adequada durante o prazo "
    "fixado do servidor de DNS (ns1.example.com.br) para o "
    "presente dominio."
    "</brdomain:reason>"
    "</brdomain:panData>"    
    "</extension>"
    "<trID>"
    "<clTRID>BCD-23456</clTRID>"
    "<svTRID>65432-WXY</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";
  
  DomParser parser;
  parser.enable_validation("../docs/schemas");
  Poll poll;
  CPPUNIT_ASSERT_NO_THROW(poll.set_response(expected, &parser));
  
  string result_code = "1301";
  PollRsp* poll_rsp = poll.get_response();
  
  map< string, PollRsp::MsgContent, less<string> > content =
    poll_rsp->get_content();
  map< string, PollRsp::MsgContent, less<string> >::const_iterator msg_it;
  
  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = poll_rsp->get_result_list();
  r_it = results.begin();

  CPPUNIT_ASSERT(r_it != results.end());
  CPPUNIT_ASSERT_EQUAL(Response::OK_ACK_DEQUEUE, r_it->first);

  string response =
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<response>"
    "<result code='" + result_code + "'>"
    "<msg";

  string result_lang = poll_rsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }

  response +=
    ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<msgQ count='" + poll_rsp->get_count() + "' id='" +
    poll_rsp->get_id() + "'>"
    "<qDate>" + poll_rsp->get_qDate() + "</qDate>"
    "<msg";
  
  string msg_lang = poll_rsp->get_lang();
  if(msg_lang != "en") {
    response += " lang='" + msg_lang + "'";
  }
  
  response +=
    ">" + poll_rsp->get_text();
  
  for (msg_it = content.begin(); msg_it != content.end(); msg_it++) {
    string elem_name = (*msg_it).first;
    PollRsp::MsgContent msg_content = (*msg_it).second;
    map< string, string, less<string> > attrs = msg_content.attributes;
    map< string, string, less<string> >::const_iterator attrs_it;
    response += "<" + elem_name;
    for (attrs_it = attrs.begin(); attrs_it != attrs.end(); attrs_it++) {
      string attr_name = (*attrs_it).first;
      string attr_value = (*attrs_it).second;
      response += " " + attr_name + "='" + attr_value + "'";
    }
    response += ">" + msg_content.value + "</" + elem_name + ">";
  }
  
  response +=
    "</msg>"
    "</msgQ>";

  // resData
  CPPUNIT_ASSERT_EQUAL(BR_DOMAIN_PANDATA, poll_rsp->get_response_type());
  BrDomainPanDataRsp * brdomain_pandata_rsp = 
    (BrDomainPanDataRsp *)poll_rsp->get_response();  
  
  response +=
    "<resData>"
    "<domain:panData "
    "xmlns:domain='urn:ietf:params:xml:ns:domain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd'>";

  // domain:name
  response +=
    "<domain:name paResult='";
  
  if (brdomain_pandata_rsp->get_paResult() == true) {
    response += "1'>";
  } else {
    response += "0'>";
  }

  response +=
    brdomain_pandata_rsp->get_object_id() + "</domain:name>";
  
  // domain:paTRID
  response +=
    "<domain:paTRID>";
  string clTRID_pandata = brdomain_pandata_rsp->get_clTRID();
  string svTRID_pandata = brdomain_pandata_rsp->get_svTRID();
  if (clTRID_pandata != "") {
    response += 
      "<clTRID>" + clTRID_pandata + "</clTRID>";
  }

  response +=
    "<svTRID>" + svTRID_pandata + "</svTRID>"
    "</domain:paTRID>";
  
  // domain:paDate
  response +=
    "<domain:paDate>" + brdomain_pandata_rsp->get_paDate() + "</domain:paDate>"
    "</domain:panData>"
    "</resData>";

  // brdomain:panData extension
  
  response +=
    "<extension>"
    "<brdomain:panData "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>";
  
  // ticket number
  response +=
    "<brdomain:ticketNumber>" + 
    StrUtil::to_string("%d", brdomain_pandata_rsp->get_ticketNumber()) +
    "</brdomain:ticketNumber>";
  
  string reason = brdomain_pandata_rsp->get_reason();
  string reason_lang = brdomain_pandata_rsp->get_reason_lang();
  if (reason != "") {
    response += 
      "<brdomain:reason";
    if (reason_lang != "en") {
      response += " lang='" + reason_lang + "'";
    }
    response += ">" + reason + "</brdomain:reason>";
  }

  response +=
    "</brdomain:panData>"
    "</extension>"
    "<trID>"
    "<clTRID>" + poll_rsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + poll_rsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";
  
  CPPUNIT_ASSERT_EQUAL(expected, response);  
}
#endif //USE_BR_DOMAINS
