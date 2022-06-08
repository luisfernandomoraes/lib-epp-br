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
/* $Id: ContactCheckTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#include <memory>
#include <string>

#include "libepp_nicbr.H"

#include "ContactCheckTest.H"
#include "ContactCheck.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

//using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(ContactCheckTest);

ContactCheckTest::ContactCheckTest() {}

ContactCheckTest::~ContactCheckTest() {}

void ContactCheckTest::setUp() {}

void ContactCheckTest::tearDown() {}

void ContactCheckTest::set_xml_template_test()
{
  string to_be_parsed = "<command>"
    "<check>"
    "<contact:check"
    "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0'"
    "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0"
    "contact-1.0.xsd'>"
    "$(id_list)$"
    "</contact:check>"
    "</check>"
    "$(clTRID)$"
    "</command>";

  ContactCheck contact_check;
  ContactCheckCmd *contact_check_cmd = contact_check.get_command(); 

  contact_check_cmd->insert_id("btw214");
  contact_check_cmd->insert_id("mrf348");
  contact_check_cmd->insert_id("sht145");

  contact_check.get_command()->set_clTRID("ABC-12345");
  contact_check.set_xml_template(to_be_parsed);

  string expected = "<command>"
    "<check>"
    "<contact:check"
    "xmlns:contact='urn:ietf:params:xml:ns:contact-1.0'"
    "xsi:schemaLocation='urn:ietf:params:xml:ns:contact-1.0"
    "contact-1.0.xsd'>"
    "<contact:id>btw214</contact:id>"
    "<contact:id>mrf348</contact:id>"
    "<contact:id>sht145</contact:id>"
    "</contact:check>"
    "</check>"
    "<clTRID>ABC-12345</clTRID>"
    "</command>";

  CPPUNIT_ASSERT(expected == contact_check.get_xml());

  // tests reset method
  CPPUNIT_ASSERT(!contact_check_cmd->get_id_list().empty());
  contact_check_cmd->reset();
  CPPUNIT_ASSERT(contact_check_cmd->get_id_list().empty());

  // parent attribute
  CPPUNIT_ASSERT(contact_check_cmd->get_clTRID() == "");
}

void ContactCheckTest::command_test()
{
  ContactCheck contact_check;
  ContactCheckCmd *contact_check_cmd = contact_check.get_command();

  contact_check_cmd->insert_id("btw214");
  contact_check_cmd->insert_id("mrf348");
  contact_check_cmd->insert_id("sht145");

  bool exception_caught = false;
  try {
    string xml_template = 
      FileUtil::read_file("../docs/templates/contact_check.xml");

    contact_check.get_command()->set_clTRID("ABC-12345");
    contact_check.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(contact_check.get_xml());
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

void ContactCheckTest::response_test()
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
    "<contact:chkData "
    "xmlns:contact=\"urn:ietf:params:xml:ns:contact-1.0\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd\">"
    "<contact:cd>"
    "<contact:id avail=\"1\">btw214</contact:id>"
    "</contact:cd>"
    "<contact:cd>"
    "<contact:id avail=\"0\">mrf348</contact:id>"
    "<contact:reason>In use</contact:reason>"
    "</contact:cd>"
    "<contact:cd>"
    "<contact:id avail=\"1\">sht145</contact:id>"
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

  ContactCheck contact_check;
  CPPUNIT_ASSERT_NO_THROW(contact_check.set_response(expected, &parser));

  string result_code = "1000";
  ContactCheckRsp* contact_check_rsp = contact_check.get_response();

  map< string, ContactCheckRsp::Availability, less<string> > avail;
  map< string, ContactCheckRsp::Availability, less<string> >::const_iterator it;
  avail = contact_check_rsp->get_availability_list();

  map <Response::ResultCode, Response::ResultInfo> results;
  map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
  results = contact_check_rsp->get_result_list();
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
  
  string result_lang = contact_check_rsp->get_result_lang();
  if (result_lang != "en") {
    response += " lang='" + result_lang + "'";
  }
  
  response +=
    ">" + r_it->second.msg + "</msg>"
    "</result>"
    "<resData>"
    "<contact:chkData "
    "xmlns:contact=\"urn:ietf:params:xml:ns:contact-1.0\" "
    "xsi:schemaLocation=\"urn:ietf:params:xml:ns:contact-1.0 "
    "contact-1.0.xsd\">";

  for (it = avail.begin(); it != avail.end(); it++) {
    response += "<contact:cd><contact:id avail=\"" +
      (*it).second.available + "\">"
      + (*it).first + "</contact:id>";
    if (strcmp((*it).second.reason.c_str(), "") != 0) {
      response += "<contact:reason>" + (*it).second.reason + 
	"</contact:reason>";
    }
    response += "</contact:cd>";
  }
  response += "</contact:chkData>"
    "</resData>"
    "<trID>"
    "<clTRID>" + contact_check_rsp->get_clTRID() + "</clTRID>"
    "<svTRID>" + contact_check_rsp->get_svTRID() + "</svTRID>"
    "</trID>"
    "</response>"
    "</epp>";
  
  CPPUNIT_ASSERT(response == expected);

  // tests reset method
  CPPUNIT_ASSERT(!contact_check_rsp->get_availability_list().empty());
  contact_check_rsp->reset();
  CPPUNIT_ASSERT(contact_check_rsp->get_availability_list().empty());
}
