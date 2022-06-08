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
/* $Id: LoginTest.cpp 1015 2009-03-12 19:28:56Z eduardo $ */
#include <string>
#include <memory>
#include <iostream>

#include "libepp_nicbr.H"

#include "LoginTest.H"
#include "Login.H"
#include "FileUtil.H"
#include "IoException.H"
#include "XmlException.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(LoginTest);

LoginTest::LoginTest() {}

LoginTest::~LoginTest() {}

void LoginTest::setUp() {}

void LoginTest::tearDown() {}

void LoginTest::set_xml_template_test()
{
  string to_be_parsed = "<command>"
    "<login>"
      "<clID>$(clID)$</clID>"
      "<pw>$(pw)$</pw>"
      "$(newPW)$"
      "<options>"
        "<version>1.0</version>"
        "<lang>pt</lang>"
      "</options>"
      "<svcs> "
        "<objURI>urn:ietf:params:xml:ns:domain</objURI>"
        "<objURI>urn:ietf:params:xml:ns:contact</objURI>"
        "<objURI>urn:ietf:params:xml:ns:organization</objURI>"
        "<svcExtension>"
          "<extURI>http://registro.br/epp/domainext-1.0</extURI>"
        "</svcExtension>"
      "</svcs>"
    "</login>"
    "$(clTRID)$"
  "</command>";

  // tests expected template with new password
  Login login;
  LoginCmd *login_cmd = login.get_command();
  login_cmd->set_clID("ClientX");
  login_cmd->set_pw("foo-BAR2");
  login_cmd->set_new_pw("bar-FOO2");
  
  bool unexpected_exception = false;
  try {

    login.get_command()->set_clTRID("ABC-12345");
    login.set_xml_template(to_be_parsed);

  } catch(...) {
    unexpected_exception = true;
  }
  CPPUNIT_ASSERT(!unexpected_exception);

  string expected = "<command>"
    "<login>"
      "<clID>ClientX</clID>"
      "<pw>foo-BAR2</pw>"
      "<newPW>bar-FOO2</newPW>"
      "<options>"
        "<version>1.0</version>"
        "<lang>pt</lang>"
      "</options>"
      "<svcs> "
        "<objURI>urn:ietf:params:xml:ns:domain</objURI>"
        "<objURI>urn:ietf:params:xml:ns:contact</objURI>"
        "<objURI>urn:ietf:params:xml:ns:organization</objURI>"
        "<svcExtension>"
          "<extURI>http://registro.br/epp/domainext-1.0</extURI>"
        "</svcExtension>"
      "</svcs>"
    "</login>"
    "<clTRID>ABC-12345</clTRID>"
  "</command>";

  CPPUNIT_ASSERT(expected == login.get_xml());

  // tests reset method
  login_cmd->reset();
  CPPUNIT_ASSERT(login_cmd->get_clID() == "");
  CPPUNIT_ASSERT(login_cmd->get_pw() == "");
  CPPUNIT_ASSERT(login_cmd->get_new_pw() == "");

  // parent attribute
  CPPUNIT_ASSERT(login_cmd->get_clTRID() == "");

  // tests expected template without new password
  login_cmd->set_clID("ClientX");
  login_cmd->set_pw("foo-BAR2");

  try {

    login.get_command()->set_clTRID("ABC-12345");
    login.set_xml_template(to_be_parsed);

  } catch(...) {
    unexpected_exception = true;
  }
  CPPUNIT_ASSERT(!unexpected_exception);
  
  expected = "<command>"
    "<login>"
      "<clID>ClientX</clID>"
      "<pw>foo-BAR2</pw>"
      "<options>"
        "<version>1.0</version>"
        "<lang>pt</lang>"
      "</options>"
      "<svcs> "
        "<objURI>urn:ietf:params:xml:ns:domain</objURI>"
        "<objURI>urn:ietf:params:xml:ns:contact</objURI>"
        "<objURI>urn:ietf:params:xml:ns:organization</objURI>"
        "<svcExtension>"
          "<extURI>http://registro.br/epp/domainext-1.0</extURI>"
        "</svcExtension>"
      "</svcs>"
    "</login>"
    "<clTRID>ABC-12345</clTRID>"
  "</command>";

  CPPUNIT_ASSERT(expected == login.get_xml());
}

void LoginTest::command_test()
{
  Login login;
  LoginCmd *login_cmd = login.get_command();
  
  login_cmd->set_clID("ClientX");
  login_cmd->set_pw("foo-BAR2");
  login_cmd->set_new_pw("bar-FOO2");

  bool exception_caught = false;

  try {
    string xml_template = FileUtil::read_file("../docs/templates/login.xml");

    login.get_command()->set_clTRID("ABC-12345");
    login.set_xml_template(xml_template);

    DomParser parser;
    parser.enable_validation("../docs/schemas");
    parser.parse_command(login.get_xml());

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
