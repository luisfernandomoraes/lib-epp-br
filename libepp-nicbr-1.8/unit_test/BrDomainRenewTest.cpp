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
/* $Id: BrDomainRenewTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#if USE_BR_DOMAINS
#include <string>
#include <memory>
#include <iostream>

#include "libepp_nicbr.H"
#include "StrUtil.H"

#include "BrDomainRenewTest.H"
#include "BrDomainRenew.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

using std::auto_ptr;

using std::cout;
using std::endl;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(BrDomainRenewTest);

BrDomainRenewTest::BrDomainRenewTest() {}

BrDomainRenewTest::~BrDomainRenewTest() {}

void BrDomainRenewTest::setUp() {}

void BrDomainRenewTest::tearDown() {}

void BrDomainRenewTest::response_test()
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
    "<domain:renData "
    "xmlns:domain=\"urn:ietf:params:xml:ns:domain-1.0\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd\">"
    "<domain:name>example.com.br</domain:name>"
    "<domain:exDate>2007-04-03T00:00:00.0Z</domain:exDate>"
    "</domain:renData>"
    "</resData>"
    "<extension>"
    "<brdomain:renData "
    "xmlns:brdomain=\"urn:ietf:params:xml:ns:brdomain-1.0\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd\">"
    "<brdomain:publicationStatus publicationFlag=\"onHold\">"
    "<brdomain:onHoldReason>billing</brdomain:onHoldReason>"
    "<brdomain:onHoldReason>dns</brdomain:onHoldReason>"
    "</brdomain:publicationStatus>"
    "</brdomain:renData>"
    "</extension>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54322-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";


  DomParser parser;
  //parser.enable_validation("../docs/schemas");
  
  BrDomainRenew domain_renew;

  bool exception_caught = false;
  try {
    domain_renew.set_response(expected, &parser);
  } catch (const XmlException &e) {
    exception_caught = true;
    printf("\nXml Exception: code [%d] message [%s] low level message [%s]\n",
	   e.get_code(), e.get_msg().c_str(), e.get_low_level_msg().c_str());
    cout << endl << domain_renew.get_xml() << endl;
  }
  CPPUNIT_ASSERT(!exception_caught);
  
  BrDomainRenewRsp* rsp = domain_renew.get_response();

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = rsp->get_result_list();
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
    "<result code=\"" + result_code + "\">"
    "<msg>" + r_it->second.msg +  "</msg>"
    "</result>"
    "<resData>"
    "<domain:renData "
    "xmlns:domain=\"urn:ietf:params:xml:ns:domain-1.0\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:domain-1.0 "
    "domain-1.0.xsd\">"
    "<domain:name>" + rsp->get_name() + "</domain:name>"
    "<domain:exDate>" + rsp->get_exDate() + "</domain:exDate>"
    "</domain:renData>"
    "</resData>";

  // brdomain:renew extension specific stuff
  response +=
    "<extension>"
    "<brdomain:renData "
    "xmlns:brdomain=\"urn:ietf:params:xml:ns:brdomain-1.0\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd\">";
  
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
    "</brdomain:renData>"
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

  //BrDomainRenewRsp specific information
  CPPUNIT_ASSERT_EQUAL((string)"", rsp->get_publication_flag());
  CPPUNIT_ASSERT_EQUAL(true, rsp->get_onhold_reasons().empty());
}
#endif //USE_BR_DOMAINS
