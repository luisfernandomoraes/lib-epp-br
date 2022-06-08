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
/* $Id: AsnDeleteTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#if USE_IP_MANAGEMENT
#include <memory>
#include <string>

#include "libepp_nicbr.H"

#include "AsnDeleteTest.H"
#include "AsnDelete.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

#include "StrUtil.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(AsnDeleteTest);

AsnDeleteTest::AsnDeleteTest() {}

AsnDeleteTest::~AsnDeleteTest() {}

void AsnDeleteTest::setUp() {}

void AsnDeleteTest::tearDown() {}

void AsnDeleteTest::set_xml_template_test()
{
  string to_be_parsed = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<delete>"
    "<asn:delete "
    "xmlns:asn='urn:ietf:params:xml:ns:asn-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:asn-1.0 "
    "asn-1.0.xsd'>"
    "<asn:number>$(asn)$</asn:number>"
    "</asn:delete>"
    "</delete>"
    "$(clTRID)$"
    "</command>"
    "</epp>";

  AsnDelete asnDelete;
  AsnDeleteCmd *asnDeleteCmd = asnDelete.get_command();
  asnDeleteCmd->set_asn(64500);
  asnDeleteCmd->set_clTRID("ABC-12345");
  asnDelete.set_xml_template(to_be_parsed);

  string expected = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<delete>"
    "<asn:delete "
    "xmlns:asn='urn:ietf:params:xml:ns:asn-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:asn-1.0 "
    "asn-1.0.xsd'>"
    "<asn:number>64500</asn:number>"
    "</asn:delete>"
    "</delete>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";
  
  CPPUNIT_ASSERT_EQUAL(expected, asnDelete.get_xml());

  // tests reset method
  asnDeleteCmd = asnDelete.get_command();
  asnDeleteCmd->reset();
  CPPUNIT_ASSERT(asnDeleteCmd->get_asn() == 0);
  
  // parent attribute
  CPPUNIT_ASSERT(asnDeleteCmd->get_clTRID() == "");
}

void AsnDeleteTest::command_test()
{
  AsnDelete asnDelete;
  AsnDeleteCmd *asnDeleteCmd = asnDelete.get_command();
  asnDeleteCmd->set_asn(64500);
  asnDeleteCmd->set_clTRID("ABC-12345");

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/asn_delete.xml");
    asnDelete.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(asnDelete.get_xml());
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

void AsnDeleteTest::response_test()
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

  AsnDelete asnDelete;
  CPPUNIT_ASSERT_NO_THROW(asnDelete.set_response(expected, &parser));
  Response *asnDeleteRsp = asnDelete.get_response();

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = asnDeleteRsp->get_result_list();
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
  
  string result_lang = asnDeleteRsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response += ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<trID>"
    "<clTRID>" + asnDeleteRsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + asnDeleteRsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  CPPUNIT_ASSERT_EQUAL(expected, response);
}
#endif //USE_IP_MANAGEMENT
