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
/* $Id: DomParserTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#include <string>
#include <memory>
#include <map>

#include "libepp_nicbr.H"

#include "XmlException.H"
#include "StrUtil.H"
#include "DomParserTest.H"

using std::auto_ptr;
using std::map;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(DomParserTest);

DomParserTest::DomParserTest(): _parser(0)
{
  try {
    _parser = new DomParser();
  } catch (...) {
  }
}

DomParserTest::~DomParserTest()
{
  if (_parser) {
    delete _parser;
  }
}

void DomParserTest::setUp() {}

void DomParserTest::tearDown() {}

void DomParserTest::enable_validation_test()
{
  bool exception_caught = false;
  _parser->enable_validation("../docs/schemas");
  string xml_payload = "<epp>Greeting</epp>";
  try {
    auto_ptr<Greeting> greeting(new Greeting());
    _parser->parse_greeting(xml_payload, greeting.get());
  } catch (const XmlException &e) {
    exception_caught = true;
  }
  CPPUNIT_ASSERT(exception_caught);
}

void DomParserTest::disable_validation_test()
{
  bool exception_caught = false;
  _parser->disable_validation();
  string xml_payload = "<epp>Greeting</epp>";
  try {
    auto_ptr<Greeting> greeting(new Greeting());
    _parser->parse_greeting(xml_payload, greeting.get());
  } catch (const XmlException &e) {
    exception_caught = true;
  }
  CPPUNIT_ASSERT(!exception_caught);
}

void DomParserTest::parse_command_test()
{
  bool exception_caught = false;
  _parser->enable_validation("../docs/schemas/");

  string xml_payload = 
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
    "<epp xmlns=\"urn:ietf:params:xml:ns:epp-1.0\" "
         "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
         "xsi:schemaLocation=\"urn:ietf:params:xml:ns:epp-1.0 "
         "epp-1.0.xsd\">"
      "<greeting>"
        "<svID>Nic.BR Server</svID>"
        "<svDate>2006-01-27T15:48:00.0Z</svDate>"
        "<svcMenu>"
          "<version>1.0</version>"
          "<lang>pt</lang>"
          "<lang>en</lang>"
          "<lang>sp</lang>"
          "<objURI>urn:ietf:params:xml:ns:contact</objURI>"
          "<objURI>urn:ietf:params:xml:ns:domain</objURI>"
          "<objURI>urn:ietf:params:xml:ns:organization</objURI>"
          "<svcExtension>"
            "<extURI>http://registro.br/contactext-1.0</extURI>"
            "<extURI>http://registro.br/domainext-1.0</extURI>"
            "<extURI>http://registro.br/organizationext-1.0</extURI>"
          "</svcExtension>"
        "</svcMenu>"
        "<dcp>"
          "<access><all/></access>"
          "<statement>"
            "<purpose><admin/><contact/><prov/></purpose>"
            "<recipient>"
              "<ours>"
                "<recDesc>Provedor de Serviços</recDesc>"
              "</ours>"
              "<public/>"
            "</recipient>"
            "<retention><none/></retention>"
          "</statement>"
          "<expiry>"
            "<absolute>2008-01-01T00:00:00.0Z</absolute>"
          "</expiry>"
        "</dcp>" 
      "</greeting>"
    "</epp>";

  try {
    _parser->parse_command(xml_payload);
  } catch (const XmlException &e) {
    exception_caught = true;
  }
  CPPUNIT_ASSERT(!exception_caught);
}
