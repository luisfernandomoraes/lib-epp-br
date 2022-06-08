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
/* $Id: AsnCheckTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#if USE_IP_MANAGEMENT
#include <memory>
#include <string>

#include "libepp_nicbr.H"

#include "AsnCheckTest.H"
#include "AsnCheck.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

#include "StrUtil.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(AsnCheckTest);

AsnCheckTest::AsnCheckTest() {}

AsnCheckTest::~AsnCheckTest() {}

void AsnCheckTest::setUp() {}

void AsnCheckTest::tearDown() {}

void AsnCheckTest::set_xml_template_test()
{
  string to_be_parsed = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<check>"
    "<asn:check "
    "xmlns:asn='urn:ietf:params:xml:ns:asn-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:asn-1.0 "
    "asn-1.0.xsd'>"
    "$(asn_list)$"
    "</asn:check>"
    "</check>"
    "$(clTRID)$"
    "</command>"
    "</epp>";

  AsnCheck asnCheck;
  AsnCheckCmd *asnCheckCmd = asnCheck.get_command();
  asnCheckCmd->add_asn(64500);
  asnCheckCmd->set_clTRID("ABC-12345");
  asnCheck.set_xml_template(to_be_parsed);

  string expected = 
    "<?xml version='1.0' encoding='UTF-8' standalone='no'?>"
    "<epp xmlns='urn:ietf:params:xml:ns:epp-1.0' "
    "xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:epp-1.0 "
    "epp-1.0.xsd'>"
    "<command>"
    "<check>"
    "<asn:check "
    "xmlns:asn='urn:ietf:params:xml:ns:asn-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:asn-1.0 "
    "asn-1.0.xsd'>"
    "<asn:number>64500</asn:number>"
    "</asn:check>"
    "</check>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>"
    "</epp>";
  
  CPPUNIT_ASSERT_EQUAL(expected, asnCheck.get_xml());

  // tests reset method
  asnCheckCmd = asnCheck.get_command();
  asnCheckCmd->reset();
  CPPUNIT_ASSERT_EQUAL(asnCheckCmd->get_asn_list().empty(), true);
  
  // parent attribute
  CPPUNIT_ASSERT(asnCheckCmd->get_clTRID() == "");
}

void AsnCheckTest::command_test()
{
  AsnCheck asnCheck;
  AsnCheckCmd *asnCheckCmd = asnCheck.get_command();
  asnCheckCmd->add_asn(64500);
  asnCheckCmd->set_clTRID("ABC-12345");

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/asn_check.xml");
    asnCheck.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(asnCheck.get_xml());
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

void AsnCheckTest::response_test()
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
    "<asn:chkData "
    "xmlns:asn='urn:ietf:params:xml:ns:asn-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:asn-1.0 "
    "asn-1.0.xsd'>"
    "<asn:cd>"
    "<asn:number avail='0'>64500</asn:number>"
    "<asn:reason>In use</asn:reason>"
    "</asn:cd>"
    "</asn:chkData>"
    "</resData>"
    "<trID>"
    "<clTRID>ABC-12345</clTRID>"
    "<svTRID>54322-XYZ</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";

  DomParser parser;
  parser.enable_validation("../docs/schemas");

  AsnCheck asnCheck;
  CPPUNIT_ASSERT_NO_THROW(asnCheck.set_response(expected, &parser));
  AsnCheckRsp *asnCheckRsp = asnCheck.get_response();

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = asnCheckRsp->get_result_list();
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
  
  string result_lang = asnCheckRsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response += ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<resData>"
    "<asn:chkData "
    "xmlns:asn='urn:ietf:params:xml:ns:asn-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:asn-1.0 "
    "asn-1.0.xsd'>";

  vector<AsnCheckRsp::CheckData> checkData = 
    asnCheckRsp->get_availability_list();

  for (unsigned int i = 0; i < checkData.size(); i++) {
    response += "<asn:cd>"
      "<asn:number";
    
    if (checkData[i].available == true) {
      response += " avail='1'";
    } else {
      response += " avail='0'";
    }
    
    response += ">" + StrUtil::to_string("%d", checkData[i].asn) + 
      "</asn:number>";

    if (checkData[i].reason != "") {
      response += "<asn:reason";

      if (checkData[i].reason_lang != "") {
	response += " lang='" + checkData[i].reason_lang + "'";
      }

      response += ">" + checkData[i].reason + "</asn:reason>";
    }

    response += "</asn:cd>";
  }
    
  response += "</asn:chkData>"
    "</resData>"
    "<trID>"
    "<clTRID>" + asnCheckRsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + asnCheckRsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";
  
  CPPUNIT_ASSERT_EQUAL(expected, response);

  // tests reset method
  asnCheckRsp->reset();
  CPPUNIT_ASSERT(asnCheckRsp->get_availability_list().empty());
}
#endif //USE_IP_MANAGEMENT
