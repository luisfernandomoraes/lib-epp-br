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
/* $Id: XmlExceptionTest.cpp 1043 2009-10-16 21:01:07Z koji $ */
#include <string>

#include "libepp_nicbr.H"

#include "XmlExceptionTest.H"
#include "XmlException.H"
#include "DomParser.H"

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(XmlExceptionTest);

XmlExceptionTest::XmlExceptionTest() {}

XmlExceptionTest::~XmlExceptionTest() {}

void XmlExceptionTest::setUp() {}

void XmlExceptionTest::tearDown() {}

void XmlExceptionTest::invalid_xml_test() 
{
  DomParser parser;
  bool exception_caught = false;
  auto_ptr<Response> login_rsp(new Response());

  /* Empty String */
  string xml = "";
  try {
    parser.parse_login_rsp(xml, login_rsp.get());
  } catch(const XmlException &e) {
    exception_caught = true;
    int exception_code = XmlException::INVALID_XML_DOCUMENT;
    CPPUNIT_ASSERT_EQUAL(exception_code, e.get_code());    
  }
  CPPUNIT_ASSERT(exception_caught);
  
  /* Random Text */
  xml = "This is some random text to test the xml parser:**#(!@)NVKD(@)";
  exception_caught = false;
  try {
    parser.parse_login_rsp(xml, login_rsp.get());
  } catch(const XmlException &e) {
    exception_caught = true;
    int exception_code = XmlException::INVALID_XML_DOCUMENT;
    CPPUNIT_ASSERT_EQUAL(exception_code, e.get_code());
  }
  CPPUNIT_ASSERT(exception_caught);    

  /* Malformed XML Document */
  xml =
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
    "<epp xmlns=\"urn:ietf:params:xml:ns:epp-1.0\" "
         "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
         "xsi:schemaLocation=\"urn:ietf:params:xml:ns:epp-1.0 "
         "epp-1.0.xsd\">"
       "<response>"
        "<result code=\"1000\">"
          "<msg>Command completed successfully</msg>"
        "</result>"
        "<trID>"
         "<clTRID>ABC-12345</clTRID>"
         "<svTRID>54321-XYZ</svTRID>"
    //  "</trID>" //Tag <trID> not closed
      "</response>"
    "</epp>";

  exception_caught = false;
  try {
    parser.parse_login_rsp(xml, login_rsp.get());
  } catch(const XmlException &e) {
    exception_caught = true;
    int exception_code = XmlException::INVALID_XML_DOCUMENT;
    CPPUNIT_ASSERT_EQUAL(exception_code, e.get_code());
  }
  CPPUNIT_ASSERT(exception_caught);

  /* Xml content using invalid encoding */
  xml = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
    "<epp>utf-8 declaration, but iso-8859-1 accents: áção</epp>";

  exception_caught = false;
  try {
    parser.parse_login_rsp(xml, login_rsp.get());
  } catch (const XmlException &e) {
    exception_caught = true;
    int exception_code = XmlException::INVALID_XML_DOCUMENT;
    CPPUNIT_ASSERT_EQUAL(exception_code, e.get_code());
  }
  CPPUNIT_ASSERT(exception_caught);

  /* Invalid utf-8 character.
     This test is tricky, as depending on how xerces was compiled, it
     may crash the program or make it run indefinitely.
     To avoid this problem, compiling xerces with libicu is recommended.
   */
  xml = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
    "<epp>Helloï¿½</epp>";

  exception_caught = false;
  string result;
  try {
    StrUtil util;
    result = util.xml_beautifier(xml);
  } catch (const XmlException &e) {
    exception_caught = true;
    int exception_code = XmlException::INVALID_XML_DOCUMENT;
    CPPUNIT_ASSERT_EQUAL(exception_code, e.get_code());
  }
  CPPUNIT_ASSERT(exception_caught || result == "<epp>Hello</epp>");
}
