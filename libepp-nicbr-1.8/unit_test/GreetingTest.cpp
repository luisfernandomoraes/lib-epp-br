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
/* $Id: GreetingTest.cpp 1016 2009-03-12 19:42:10Z eduardo $ */
#include <cstring>
#include <iostream>
#include <string>

#include "libepp_nicbr.H"

#include "GreetingTest.H"

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(GreetingTest);

GreetingTest::GreetingTest() {}

GreetingTest::~GreetingTest() {}

void GreetingTest::setUp() {}

void GreetingTest::tearDown() {}

void GreetingTest::min_response_test()
{
  string min_response = 
  "<greeting>"
    "<svID>Nic.BR Server</svID>"
    "<svDate>2006-01-27T15:48:00.0Z</svDate>"
    "<svcMenu>"
     "<version>1.0</version>"
     "<lang>pt</lang>"
     "<objURI>urn:ietf:params:xml:ns:domain</objURI>"
    "</svcMenu>"
  "</greeting>";
      
  set<string> version;
  set<string> lang;
  set <string> objURI;

  Greeting greeting;
  greeting.set_svID("Nic.BR Server");
  greeting.set_svDate("2006-01-27T15:48:00.0Z");
 
  greeting.set_version("1.0");
  greeting.set_lang("pt");
  greeting.set_objURI("urn:ietf:params:xml:ns:domain");
  
  version = greeting.get_version();
  lang = greeting.get_lang();
  objURI =  greeting.get_objURI();
  
  set<string>::const_iterator it;
  
  string expected = "<greeting><svID>" + greeting.get_svID() + "</svID>"
    "<svDate>" + greeting.get_svDate() + "</svDate><svcMenu>";
  
  for(it = version.begin(); it != version.end(); it++)
    expected += "<version>" + (*it) + "</version>";
  for(it = lang.begin(); it != lang.end(); it++)
    expected += "<lang>" + (*it) + "</lang>";
  for(it = objURI.begin(); it != objURI.end(); it++)
    expected += "<objURI>" + (*it) + "</objURI>";

  expected+=  "</svcMenu></greeting>";

  CPPUNIT_ASSERT(expected == min_response);
}

void GreetingTest::max_response_test()
{
  string max_response = 
  "<greeting>"
    "<svID>Nic.BR Server</svID>"
    "<svDate>2006-01-27T15:48:00.0Z</svDate>"
    "<svcMenu>"
      "<version>1.0</version>"
      "<version>1.3</version>"
      "<version>2.0</version>"
      "<lang>en</lang>"
      "<lang>pt</lang>"
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
     "<statement>"
       "<access><all/></access>"
       "<purpose><admin/><contact/><prov/></purpose>"
       "<recipient>"
         "<ours>"
          "<recDesc>Service Providers</recDesc>"
         "</ours>"
         "<public/>"
       "</recipient>"
       "<retention><none/></retention>"
      "</statement>"
      "<expiry>"
       "<absolute/>2008-01-01T00:00:00.0Z</absolute>"
      "</expiry>"
     "</dcp>" 
   "</greeting>";
  
  
  /// SETS
  Greeting greeting;
  greeting.set_svID("Nic.BR Server");
  greeting.set_svDate("2006-01-27T15:48:00.0Z");
  
  greeting.set_version("1.0");
  greeting.set_version("1.3");
  greeting.set_version("2.0");
  greeting.set_lang("en");
  greeting.set_lang("pt");
  greeting.set_lang("sp"); 
  greeting.set_objURI("urn:ietf:params:xml:ns:contact");
  greeting.set_objURI("urn:ietf:params:xml:ns:domain");
  greeting.set_objURI("urn:ietf:params:xml:ns:organization");
  greeting.set_extURI("http://registro.br/contactext-1.0");
  greeting.set_extURI("http://registro.br/domainext-1.0");
  greeting.set_extURI("http://registro.br/organizationext-1.0");
  
  greeting.set_access(Greeting::ALL); 
  
  greeting.set_purpose(Greeting::ADMIN);
  greeting.set_purpose(Greeting::CONTACT);
  greeting.set_purpose(Greeting::PROV);
  
  greeting.set_recipient(Greeting::OURS);
  greeting.set_recipient(Greeting::PUBLIC);
  
  greeting.set_recDesc("Service Providers");
  
  greeting.set_retention(Greeting::NONE_RT);
  
  greeting.set_expiry(0,"2008-01-01T00:00:00.0Z");
  
   /// GETS  
  set<string> version;
  set<string> lang;
  set<string> objURI;
  set<string> extURI;
  
  version = greeting.get_version();
  lang = greeting.get_lang();
  objURI = greeting.get_objURI();
  extURI = greeting.get_extURI();
  
  string access;
  if(greeting.get_access() == Greeting::ALL)
    access = "<all/>";
  
  set<Greeting::Purpose> purpose;
  purpose = greeting.get_purpose();
  set<string> purpose_str;
  set<Greeting::Purpose>::const_iterator itp;
  for(itp = purpose.begin(); itp != purpose.end(); itp++) {
    if((*itp) == Greeting::ADMIN)
       purpose_str.insert("<admin/>");
    else if((*itp) == Greeting::CONTACT)
      purpose_str.insert("<contact/>");
    else if((*itp) == Greeting::PROV)
      purpose_str.insert("<prov/>");
    else
      purpose_str.insert("<error/>");
  }
  
  set<Greeting::Recipient> recipient;
  recipient = greeting.get_recipient();
  set<string> recipient_str;
  set<Greeting::Recipient>::const_iterator itr;
  for(itr = recipient.begin(); itr != recipient.end(); itr++) {
    if((*itr) == Greeting::OURS) {
      if(strcmp(greeting.get_recDesc().c_str(),"") == 0) 
	recipient_str.insert("<ours/>");
      else
	recipient_str.insert("<ours><recDesc>" + greeting.get_recDesc() + 
			     "</recDesc></ours>");
    }
    else if((*itr) == Greeting::PUBLIC)
      recipient_str.insert("<public/>");
    else
       recipient_str.insert("<error/>");
  }
  
  string retention;
  if(greeting.get_retention() == Greeting::NONE_RT)
    retention = "<none/>";
  
  string expiry;
  if(greeting.get_type_expiry() == 0)
    expiry = "<absolute/>" + greeting.get_expiry() + "</absolute>";
  else expiry = "<relative/>" + greeting.get_expiry() + "</relative>";
  
  set<string>::const_iterator it;
  
  string expected = "<greeting><svID>" + greeting.get_svID() + "</svID>"
    "<svDate>" + greeting.get_svDate() + "</svDate><svcMenu>";
  
  for(it = version.begin(); it != version.end(); it++)
    expected += "<version>" + (*it) + "</version>";
  for(it = lang.begin(); it != lang.end(); it++)
    expected += "<lang>" + (*it) + "</lang>";
  for(it = objURI.begin(); it != objURI.end(); it++)
    expected += "<objURI>" + (*it) + "</objURI>";
  expected += "<svcExtension>";
  for(it = extURI.begin(); it != extURI.end(); it++)
    expected += "<extURI>" + (*it) + "</extURI>";
  
  expected +=  "</svcExtension></svcMenu><dcp><statement><access>" +
    access +"</access>";
  
  expected += "<purpose>";
  for(it = purpose_str.begin(); it != purpose_str.end(); it++)
    expected += (*it);
  expected += "</purpose>";
  
  expected += "<recipient>";
  for(it = recipient_str.begin(); it != recipient_str.end(); it++)
    expected += (*it);
  expected += "</recipient>";
  
  expected +=  "<retention>" + retention + "</retention></statement>";
  
  expected += "<expiry>" + expiry + "</expiry></dcp>";
  
  expected += "</greeting>";
  
  CPPUNIT_ASSERT(expected == max_response);

  // tests reset method
  CPPUNIT_ASSERT(greeting.get_svID() != "");
  CPPUNIT_ASSERT(greeting.get_svDate() != "");
  CPPUNIT_ASSERT(!greeting.get_version().empty());
  CPPUNIT_ASSERT(!greeting.get_lang().empty());
  CPPUNIT_ASSERT(!greeting.get_objURI().empty());
  CPPUNIT_ASSERT(!greeting.get_extURI().empty());
  CPPUNIT_ASSERT(greeting.get_access() != Greeting::UNSET_AC);
  CPPUNIT_ASSERT(!greeting.get_purpose().empty());
  CPPUNIT_ASSERT(!greeting.get_recipient().empty());
  CPPUNIT_ASSERT(greeting.get_recDesc() != "");
  CPPUNIT_ASSERT(greeting.get_retention() != Greeting::UNSET_RT);
  CPPUNIT_ASSERT(greeting.get_type_expiry() != -1);
  CPPUNIT_ASSERT(greeting.get_expiry() != "");

  greeting.reset();

  CPPUNIT_ASSERT(greeting.get_svID() == "");
  CPPUNIT_ASSERT(greeting.get_svDate() == "");
  CPPUNIT_ASSERT(greeting.get_version().empty());
  CPPUNIT_ASSERT(greeting.get_lang().empty());
  CPPUNIT_ASSERT(greeting.get_objURI().empty());
  CPPUNIT_ASSERT(greeting.get_extURI().empty());
  CPPUNIT_ASSERT(greeting.get_access() == Greeting::UNSET_AC);
  CPPUNIT_ASSERT(greeting.get_purpose().empty());
  CPPUNIT_ASSERT(greeting.get_recipient().empty());
  CPPUNIT_ASSERT(greeting.get_recDesc() == "");
  CPPUNIT_ASSERT(greeting.get_retention() == Greeting::UNSET_RT);
  CPPUNIT_ASSERT(greeting.get_type_expiry() == -1);
  CPPUNIT_ASSERT(greeting.get_expiry() == "");
}
