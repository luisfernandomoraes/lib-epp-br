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
/* $Id: AsnTransferTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#if USE_IP_MANAGEMENT
#include <memory>
#include <string>

#include "libepp_nicbr.H"

#include "AsnTransferTest.H"
#include "AsnTransfer.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

#include "StrUtil.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(AsnTransferTest);

AsnTransferTest::AsnTransferTest() {}

AsnTransferTest::~AsnTransferTest() {}

void AsnTransferTest::setUp() {}

void AsnTransferTest::tearDown() {}

void AsnTransferTest::set_xml_template_test()
{
  string to_be_parsed = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<transfer op='$(op)$'>"
    "<asn:transfer "
    "xmlns:asn='urn:ietf:params:xml:ns:asn-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:asn-1.0 "
    "asn-1.0.xsd'>"
    "<asn:number>$(asn)$</asn:number>"
    "</asn:transfer>"
    "</transfer>"
    "$(clTRID)$"
    "</command>"
    "</epp>";

  AsnTransfer asnTransfer;
  AsnTransferCmd *asnTransferCmd = asnTransfer.get_command();
  asnTransferCmd->set_asn(64500);
  asnTransferCmd->set_operation("request");
  asnTransferCmd->set_clTRID("ABC-12345");
  asnTransfer.set_xml_template(to_be_parsed);

  string expected = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<transfer op='request'>"
    "<asn:transfer "
    "xmlns:asn='urn:ietf:params:xml:ns:asn-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:asn-1.0 "
    "asn-1.0.xsd'>"
    "<asn:number>64500</asn:number>"
    "</asn:transfer>"
    "</transfer>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";
  
  CPPUNIT_ASSERT_EQUAL(expected, asnTransfer.get_xml());

  // tests reset method
  asnTransferCmd = asnTransfer.get_command();
  asnTransferCmd->reset();
  CPPUNIT_ASSERT(asnTransferCmd->get_asn() == 0);
  CPPUNIT_ASSERT(asnTransferCmd->get_operation() == "");
  
  // parent attribute
  CPPUNIT_ASSERT(asnTransferCmd->get_clTRID() == "");
}

void AsnTransferTest::command_test()
{
  AsnTransfer asnTransfer;
  AsnTransferCmd *asnTransferCmd = asnTransfer.get_command();
  asnTransferCmd->set_asn(64500);
  asnTransferCmd->set_operation("request");
  asnTransferCmd->set_clTRID("ABC-12345");

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/asn_transfer.xml");
    asnTransfer.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(asnTransfer.get_xml());
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

void AsnTransferTest::response_test()
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
    "<asn:trnData "
    "xmlns:asn='urn:ietf:params:xml:ns:asn-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:asn-1.0 "
    "asn-1.0.xsd'>"
    "<asn:number>64500</asn:number>"
    "<asn:trStatus>pending</asn:trStatus>"
    "<asn:reID>ClientX</asn:reID>"
    "<asn:reDate>2000-06-08T22:00:00.0Z</asn:reDate>"
    "<asn:acID>ClientY</asn:acID>"
    "<asn:acDate>2000-06-13T22:00:00.0Z</asn:acDate>"
    "</asn:trnData>"
    "</resData>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54322-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  DomParser parser;
  parser.enable_validation("../docs/schemas");

  AsnTransfer asnTransfer;
  CPPUNIT_ASSERT_NO_THROW(asnTransfer.set_response(expected, &parser));
  AsnTransferRsp *asnTransferRsp = asnTransfer.get_response();

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = asnTransferRsp->get_result_list();
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
  
  string result_lang = asnTransferRsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response += ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<resData>"
    "<asn:trnData "
    "xmlns:asn='urn:ietf:params:xml:ns:asn-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:asn-1.0 "
    "asn-1.0.xsd'>"
    "<asn:number>" + StrUtil::to_string("%d", asnTransferRsp->get_asn()) + 
    "</asn:number>"
    "<asn:trStatus>" + asnTransferRsp->get_trStatus() + "</asn:trStatus>"
    "<asn:reID>" + asnTransferRsp->get_reId() + "</asn:reID>"
    "<asn:reDate>" + asnTransferRsp->get_reDate() + "</asn:reDate>"
    "<asn:acID>" + asnTransferRsp->get_acId() + "</asn:acID>"
    "<asn:acDate>" + asnTransferRsp->get_acDate() + "</asn:acDate>"
    "</asn:trnData>"
    "</resData>"
    "<trID>"
    "<clTRID>" + asnTransferRsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + asnTransferRsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(expected, response);

  // tests reset method
  asnTransferRsp->reset();
  CPPUNIT_ASSERT(asnTransferRsp->get_asn() == 0);
  CPPUNIT_ASSERT_EQUAL((const string)"", asnTransferRsp->get_trStatus());
  CPPUNIT_ASSERT_EQUAL((const string)"", asnTransferRsp->get_reId());
  CPPUNIT_ASSERT_EQUAL((const string)"", asnTransferRsp->get_reDate());
  CPPUNIT_ASSERT_EQUAL((const string)"", asnTransferRsp->get_acId());
  CPPUNIT_ASSERT_EQUAL((const string)"", asnTransferRsp->get_acDate());
}
#endif //USE_IP_MANAGEMENT
