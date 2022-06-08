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
/* $Id: SessionTestClient.cpp 1042 2009-10-16 19:07:39Z koji $ */
#include <string>
#include <memory>
#include <iostream>

#include "libepp_nicbr.H"

#include "XmlException.H"
#include "TransportException.H"
#include "IoException.H"
#include "EppException.H"
#include "SessionTestClient.H"
#include "Login.H"
#include "Logout.H"
#include "ContactCreate.H"

using std::cout;
using std::endl;

#define   PORT_FILE    "SessionTestServer.port"

LIBEPP_NICBR_NS_USE

CPPUNIT_TEST_SUITE_REGISTRATION(SessionTestClient);

SessionTestClient::SessionTestClient() 
{
  bool exception_caught = false;

  try {
    // Gets the port number from TestSrv.port file
    int port;
    FILE *fd = fopen(PORT_FILE, "r");
    CPPUNIT_ASSERT(fd != NULL);
    CPPUNIT_ASSERT(fscanf(fd, "%d", &port) == 1);
    fclose(fd);
    const string server = "localhost";
    // Session
    _session = auto_ptr<Session>(new Session(server, port, 
					     "../docs/templates"));
    _session->enable_xml_validation("../docs/schemas");

    // Command parser
    _command_parser = auto_ptr<DomParser>(new DomParser());
    _command_parser->enable_validation("../docs/schemas");

  } catch (const GeneralException &e) {
    exception_caught = true;
    printf("\nGeneral Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  }

  CPPUNIT_ASSERT(!exception_caught);
}

SessionTestClient::~SessionTestClient() {}

void SessionTestClient::setUp() {}

void SessionTestClient::tearDown() {}

void SessionTestClient::connect_test() 
{
  bool exception_caught = false;

  try {    
    _session->connect("client.pem", "root.pem", "client passphrase");
    Greeting *greeting = _session->get_greeting();  
    
    string expected_svID = "Nic.BR Server";
    string expected_svDate = "2006-01-27T15:48:00.0Z";
    string expected_version = "1.0";
    size_t expected_lang_num = 3;
    string expected_lang_pt = "pt";
    string expected_lang_en = "en";
    string expected_lang_sp = "sp";
    size_t expected_objURI_num = 3;
    string expected_contact_objectURI = "urn:ietf:params:xml:ns:contact";
    string expected_domain_objectURI = "urn:ietf:params:xml:ns:domain";
    string expected_org_objectURI = "urn:ietf:params:xml:ns:organization";
    size_t expected_extURI_num = 3;
    string expected_contact_extURI = "http://registro.br/contactext-1.0";
    string expected_domain_extURI = "http://registro.br/domainext-1.0";
    string expected_org_extURI = "http://registro.br/organizationext-1.0";
    Greeting::Access expected_access = Greeting::ALL;
    size_t expected_purpose_num = 3;
    Greeting::Purpose expected_purpose_adm = Greeting::ADMIN;
    Greeting::Purpose expected_purpose_contact = Greeting::CONTACT;
    Greeting::Purpose expected_purpose_prov = Greeting::PROV;
    size_t expected_recipient_num = 2;
    Greeting::Recipient expected_recipient_ours = Greeting::OURS;
    Greeting::Recipient expected_recipient_public = Greeting::PUBLIC;
    string expected_recDesc = "Provedores de Serviço";
    Greeting::Retention expected_retention = Greeting::NONE_RT;
    int expected_expiry_type = 0; // absolute
    string expected_expiry_date = "2008-01-01T00:00:00.0Z";

    // Server ID
    CPPUNIT_ASSERT_EQUAL(expected_svID, greeting->get_svID());
    // Server Date
    CPPUNIT_ASSERT_EQUAL(expected_svDate, greeting->get_svDate());
    // EPP version
    set <string>::const_iterator it_str = greeting->get_version().begin();
    string str = *it_str;
    CPPUNIT_ASSERT_EQUAL(expected_version, str);
    // Supported Languages
    set<string> lang = greeting->get_lang();
    CPPUNIT_ASSERT_EQUAL(expected_lang_num, lang.size());
    string vstr[3];
    int i = 0;
    for (it_str = lang.begin(); it_str != lang.end(); it_str++, i++) {
      vstr[i] = *it_str;
    }
    CPPUNIT_ASSERT_EQUAL(expected_lang_en, vstr[0]);
    CPPUNIT_ASSERT_EQUAL(expected_lang_pt, vstr[1]);
    CPPUNIT_ASSERT_EQUAL(expected_lang_sp, vstr[2]);
    // Object URI
    set<string> objURI = greeting->get_objURI();
    CPPUNIT_ASSERT_EQUAL(expected_objURI_num, objURI.size());
    for (it_str = objURI.begin(), i = 0;
	 it_str != objURI.end(); it_str++, i++) {
      vstr[i] = *it_str;
    }
    CPPUNIT_ASSERT_EQUAL(expected_contact_objectURI, vstr[0]);
    CPPUNIT_ASSERT_EQUAL(expected_domain_objectURI, vstr[1]);
    CPPUNIT_ASSERT_EQUAL(expected_org_objectURI, vstr[2]);
    // Extension URI
    set<string> extURI = greeting->get_extURI();
    CPPUNIT_ASSERT_EQUAL(expected_extURI_num, extURI.size());
    for (it_str = extURI.begin(), i = 0;
	 it_str != extURI.end();
	 it_str++, i++) {
      vstr[i] = *it_str;
    }
    CPPUNIT_ASSERT_EQUAL(expected_contact_extURI, vstr[0]);
    CPPUNIT_ASSERT_EQUAL(expected_domain_extURI, vstr[1]);
    CPPUNIT_ASSERT_EQUAL(expected_org_extURI, vstr[2]);
    // Access
    CPPUNIT_ASSERT_EQUAL(expected_access, greeting->get_access());
    // Purpose
    set<Greeting::Purpose> purpose = greeting->get_purpose();
    CPPUNIT_ASSERT_EQUAL(expected_purpose_num, purpose.size());
    set<Greeting::Purpose>::const_iterator it_purpose;
    Greeting::Purpose vpurpose[3];
    for (it_purpose = purpose.begin(), i = 0;
	 it_purpose != purpose.end();
	 it_purpose++, i++) {
      vpurpose[i] = *it_purpose;
    }
    CPPUNIT_ASSERT_EQUAL(expected_purpose_adm, vpurpose[0]);
    CPPUNIT_ASSERT_EQUAL(expected_purpose_contact, vpurpose[1]);
    CPPUNIT_ASSERT_EQUAL(expected_purpose_prov, vpurpose[2]);
    // Recipient
    set<Greeting::Recipient> recipient = greeting->get_recipient();
    CPPUNIT_ASSERT_EQUAL(expected_recipient_num, recipient.size());
    set<Greeting::Recipient>::const_iterator it_recipient;
    Greeting::Recipient vrecipient[2];
    for (it_recipient = recipient.begin(), i = 0;
	 it_recipient != recipient.end();
	 it_recipient++, i++) {
      vrecipient[i] = *it_recipient;
    }
    CPPUNIT_ASSERT_EQUAL(expected_recipient_ours, vrecipient[0]);
    CPPUNIT_ASSERT_EQUAL(expected_recipient_public, vrecipient[1]);
    CPPUNIT_ASSERT_EQUAL(expected_recDesc, greeting->get_recDesc());
    // Retention
    CPPUNIT_ASSERT_EQUAL(expected_retention, greeting->get_retention());
    // Expiry Type
    CPPUNIT_ASSERT_EQUAL(expected_expiry_type, greeting->get_type_expiry());
    // Expiry Date
    CPPUNIT_ASSERT_EQUAL(expected_expiry_date, greeting->get_expiry());
 
  } catch (const IoException &e) {
    exception_caught = true;
    printf("\nIO Exception: \ncode [%d]\n message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  } catch (const TransportException &e) {
    exception_caught = true;
    printf("\nTransport Exception: \ncode [%d]\n message [%s] \n "
	   "low level message [%s]\n", e.get_code(), e.get_msg().c_str(), 
	   e.get_low_level_msg().c_str());
  } catch (const GeneralException &e) {
    exception_caught = true;
    printf("\nGeneral Exception: \ncode [%d]\n message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  }

  CPPUNIT_ASSERT(!exception_caught);
}

void SessionTestClient::login_test() 
{
  bool exception_caught = false;
  Login my_login;
  
  try {
    // Action
    
    LoginCmd *login_cmd = my_login.get_command();
    login_cmd->set_clID("ClientX");
    login_cmd->set_pw("foo-BAR2");
    login_cmd->set_new_pw("bar-FOO2");

    // Process Action Login
    string cmd_clTRID = "ABC-12345";
    _session->process_action(&my_login, cmd_clTRID);

    // Validate the command in XML format
    _command_parser->parse_command(my_login.get_xml());

    // Response
    Response *login_resp = my_login.get_response();

    // Excepted values
    string expected_svTRID = "54321-XYZ";
    Response::ResultCode expected_result_code = Response::OK;
    string expected_result_msg = "Command completed successfully";

    map <Response::ResultCode, Response::ResultInfo> results;
    map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
    results = login_resp->get_result_list();
    r_it = results.begin();
    CPPUNIT_ASSERT(r_it != results.end());

    // Asserts
    CPPUNIT_ASSERT_EQUAL(cmd_clTRID, login_resp->get_clTRID());
    CPPUNIT_ASSERT_EQUAL(expected_svTRID, login_resp->get_svTRID());
    CPPUNIT_ASSERT_EQUAL(expected_result_code, r_it->first);
    CPPUNIT_ASSERT_EQUAL(expected_result_msg, r_it->second.msg);

  } catch (const IoException &e) {
    exception_caught = true;
    printf("\nTransport Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  } catch (const TransportException &e) {
    exception_caught = true;
    printf("\nTransport Exception: \ncode [%d]\n message [%s] \n "
	   "low level message [%s]\n", e.get_code(), e.get_msg().c_str(), 
	   e.get_low_level_msg().c_str());
  } catch (const XmlException &e) {
    exception_caught = true;
    printf("\nXML Exception: \ncode [%d]\n message [%s] \n "
	   "low level message [%s]\n", e.get_code(), e.get_msg().c_str(), 
	   e.get_low_level_msg().c_str());
    cout << my_login.get_xml() << endl;
  } catch (const GeneralException &e) {
    exception_caught = true;
    printf("\nGeneral Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  } 
  
  CPPUNIT_ASSERT(!exception_caught);
}

void SessionTestClient::create_org_contact_test() 
{
  bool exception_caught = false;

  try {
    // Command(arguments)    
    ContactCreate my_contact_create;
    ContactCreateCmd *cmd = my_contact_create.get_command();
    CommonData common;
    
    common.set_id("sh8013");

    PostalInfo postal_info; 
    postal_info.set_type("int");
    postal_info.set_name("Frederico Neves");
    postal_info.set_org("Núcleo de Informação e Coordenação do .BR");
    postal_info.set_str1("Av. das Nações Unidas, 11541");
    postal_info.set_str2("7º andar");
    postal_info.set_str3("");
    postal_info.set_city("São Paulo");
    postal_info.set_sp("SP");
    postal_info.set_pc("04578-000");
    postal_info.set_cc("BR");

    common.insert_postal_info(postal_info);

    CommonData::Phone phone;
    phone.ext = "1234";
    phone.number = "+55.1155093500";
    common.set_voice(phone);

    phone.ext = "";
    phone.number = "+55.1155093501";
    common.set_fax(phone);

    string email = "jdoe@example.com";
    common.set_email(email);
    
    AuthInfo authInfo;
    authInfo.set_roid("");
    authInfo.set_pw("2fooBAR");
    cmd->set_authInfo(authInfo);

    CommonData::Disclose disclose;
    disclose.flag = 1;
    disclose.name_int = true;
    common.set_disclose(disclose);

    cmd->set_common_data(common);
    // Action
    // Process Action Create Contact
    string cmd_clTRID = "ABC-12345";
    _session->process_action(&my_contact_create, cmd_clTRID);

    // Validate the command in XML format
    _command_parser->parse_command(my_contact_create.get_xml());

    // Response
    ContactCreateRsp* contact_create_resp = my_contact_create.get_response();

    // Expected Values
    Response::ResultCode expected_result_code = Response::OK;
    string expected_result_msg = "Command completed successfully";
    string expected_contact_id = "sh8013";
    string expected_crDate = "1999-04-03T22:00:00.0Z";
    string expected_svTRID = "54321-XYZ";

    map <Response::ResultCode, Response::ResultInfo> results;
    map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
    results = contact_create_resp->get_result_list();
    r_it = results.begin();
    CPPUNIT_ASSERT(r_it != results.end());

    // Asserts
    CPPUNIT_ASSERT_EQUAL(expected_result_code, r_it->first);
    CPPUNIT_ASSERT_EQUAL(expected_result_msg, r_it->second.msg);
    CPPUNIT_ASSERT_EQUAL(expected_contact_id, contact_create_resp->get_id());
    CPPUNIT_ASSERT_EQUAL(expected_crDate, contact_create_resp->get_crDate());
    CPPUNIT_ASSERT_EQUAL(cmd_clTRID, contact_create_resp->get_clTRID());
    CPPUNIT_ASSERT_EQUAL(expected_svTRID, contact_create_resp->get_svTRID());

  } catch (const EppException &e) {
    exception_caught = true;
    printf("\nEpp Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());  
  } catch (const GeneralException &e) {
    exception_caught = true;
    printf("\nGeneral Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  }
  
  CPPUNIT_ASSERT(!exception_caught);
}

void SessionTestClient::send_hello_test()
{
  bool exception_caught = false;
  
  try {
    _session->send_hello();

    Greeting *greeting = _session->get_greeting();  
    
    string expected_svID = "Nic.BR Server";
    string expected_svDate = "2006-01-27T15:48:00.0Z";
    string expected_version = "1.0";
    size_t expected_lang_num = 3;
    string expected_lang_pt = "pt";
    string expected_lang_en = "en";
    string expected_lang_sp = "sp";
    size_t expected_objURI_num = 3;
    string expected_contact_objectURI = "urn:ietf:params:xml:ns:contact";
    string expected_domain_objectURI = "urn:ietf:params:xml:ns:domain";
    string expected_org_objectURI = "urn:ietf:params:xml:ns:organization";
    size_t expected_extURI_num = 3;
    string expected_contact_extURI = "http://registro.br/contactext-1.0";
    string expected_domain_extURI = "http://registro.br/domainext-1.0";
    string expected_org_extURI = "http://registro.br/organizationext-1.0";
    Greeting::Access expected_access = Greeting::ALL;
    size_t expected_purpose_num = 3;
    Greeting::Purpose expected_purpose_adm = Greeting::ADMIN;
    Greeting::Purpose expected_purpose_contact = Greeting::CONTACT;
    Greeting::Purpose expected_purpose_prov = Greeting::PROV;
    size_t expected_recipient_num = 2;
    Greeting::Recipient expected_recipient_ours = Greeting::OURS;
    Greeting::Recipient expected_recipient_public = Greeting::PUBLIC;
    string expected_recDesc = "Provedores de Serviço";
    Greeting::Retention expected_retention = Greeting::NONE_RT;
    int expected_expiry_type = 0; // absolute
    string expected_expiry_date = "2008-01-01T00:00:00.0Z";

    // Server ID
    CPPUNIT_ASSERT_EQUAL(expected_svID, greeting->get_svID());
    // Server Date
    CPPUNIT_ASSERT_EQUAL(expected_svDate, greeting->get_svDate());
    // EPP version
    set <string>::const_iterator it_str = greeting->get_version().begin();
    string str = *it_str;
    CPPUNIT_ASSERT_EQUAL(expected_version, str);
    // Supported Languages
    set<string> lang = greeting->get_lang();
    CPPUNIT_ASSERT_EQUAL(expected_lang_num, lang.size());
    string vstr[3];
    int i = 0;
    for (it_str = lang.begin(); it_str != lang.end(); it_str++, i++) {
      vstr[i] = *it_str;
    }
    CPPUNIT_ASSERT_EQUAL(expected_lang_en, vstr[0]);
    CPPUNIT_ASSERT_EQUAL(expected_lang_pt, vstr[1]);
    CPPUNIT_ASSERT_EQUAL(expected_lang_sp, vstr[2]);
    // Object URI
    set<string> objURI = greeting->get_objURI();
    CPPUNIT_ASSERT_EQUAL(expected_objURI_num, objURI.size());
    for (it_str = objURI.begin(), i = 0;
	 it_str != objURI.end(); it_str++, i++) {
      vstr[i] = *it_str;
    }
    CPPUNIT_ASSERT_EQUAL(expected_contact_objectURI, vstr[0]);
    CPPUNIT_ASSERT_EQUAL(expected_domain_objectURI, vstr[1]);
    CPPUNIT_ASSERT_EQUAL(expected_org_objectURI, vstr[2]);
    // Extension URI
    set<string> extURI = greeting->get_extURI();
    CPPUNIT_ASSERT_EQUAL(expected_extURI_num, extURI.size());
    for (it_str = extURI.begin(), i = 0;
	 it_str != extURI.end();
	 it_str++, i++) {
      vstr[i] = *it_str;
    }
    CPPUNIT_ASSERT_EQUAL(expected_contact_extURI, vstr[0]);
    CPPUNIT_ASSERT_EQUAL(expected_domain_extURI, vstr[1]);
    CPPUNIT_ASSERT_EQUAL(expected_org_extURI, vstr[2]);
    // Access
    CPPUNIT_ASSERT_EQUAL(expected_access, greeting->get_access());
    // Purpose
    set<Greeting::Purpose> purpose = greeting->get_purpose();
    CPPUNIT_ASSERT_EQUAL(expected_purpose_num, purpose.size());
    set<Greeting::Purpose>::const_iterator it_purpose;
    Greeting::Purpose vpurpose[3];
    for (it_purpose = purpose.begin(), i = 0;
	 it_purpose != purpose.end();
	 it_purpose++, i++) {
      vpurpose[i] = *it_purpose;
    }
    CPPUNIT_ASSERT_EQUAL(expected_purpose_adm, vpurpose[0]);
    CPPUNIT_ASSERT_EQUAL(expected_purpose_contact, vpurpose[1]);
    CPPUNIT_ASSERT_EQUAL(expected_purpose_prov, vpurpose[2]);
    // Recipient
    set<Greeting::Recipient> recipient = greeting->get_recipient();
    CPPUNIT_ASSERT_EQUAL(expected_recipient_num, recipient.size());
    set<Greeting::Recipient>::const_iterator it_recipient;
    Greeting::Recipient vrecipient[2];
    for (it_recipient = recipient.begin(), i = 0;
	 it_recipient != recipient.end();
	 it_recipient++, i++) {
      vrecipient[i] = *it_recipient;
    }
    CPPUNIT_ASSERT_EQUAL(expected_recipient_ours, vrecipient[0]);
    CPPUNIT_ASSERT_EQUAL(expected_recipient_public, vrecipient[1]);
    CPPUNIT_ASSERT_EQUAL(expected_recDesc, greeting->get_recDesc());
    // Retention
    CPPUNIT_ASSERT_EQUAL(expected_retention, greeting->get_retention());
    // Expiry Type
    CPPUNIT_ASSERT_EQUAL(expected_expiry_type, greeting->get_type_expiry());
    // Expiry Date
    CPPUNIT_ASSERT_EQUAL(expected_expiry_date, greeting->get_expiry());
 
  } catch (const GeneralException &e) {
    exception_caught = true;
    printf("\nGeneral Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  }

  CPPUNIT_ASSERT(!exception_caught);
}

void SessionTestClient::logout_test() 
{
  bool exception_caught = false;

  try {
    // Action Logout does not have Command(arguments)
    Logout my_logout;
    string cmd_clTRID = "ABC-12345";
    
    // Process Action Logout
    _session->process_action(&my_logout, cmd_clTRID);

    // Validate the command in XML format
    _command_parser->parse_command(my_logout.get_xml());

    // Response
    Response *logout_rsp = my_logout.get_response();

    // Excepted values
    string expected_svTRID = "54321-XYZ";
    Response::ResultCode expected_result_code = Response::OK;
    string expected_result_msg = "Command completed successfully";

    map <Response::ResultCode, Response::ResultInfo> results;
    map <Response::ResultCode, Response::ResultInfo>::const_iterator r_it;
    results = logout_rsp->get_result_list();
    r_it = results.begin();
    CPPUNIT_ASSERT(r_it != results.end());

    // Asserts
    CPPUNIT_ASSERT_EQUAL(cmd_clTRID, logout_rsp->get_clTRID());
    CPPUNIT_ASSERT_EQUAL(expected_svTRID, logout_rsp->get_svTRID());
    CPPUNIT_ASSERT_EQUAL(expected_result_code, r_it->first);
    CPPUNIT_ASSERT_EQUAL(expected_result_msg, r_it->second.msg);

  } catch (const XmlException &e) {
    exception_caught = true;
    printf("\nXML Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  } catch (const IoException &e) {
    exception_caught = true;
    printf("\nTransport Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  } catch (const TransportException &e) {
    exception_caught = true;
    printf("\nTransport Exception: \ncode [%d]\n message [%s] \n "
	   "low level message [%s]\n", e.get_code(), e.get_msg().c_str(), 
	   e.get_low_level_msg().c_str());
  } catch (const GeneralException &e) {
    exception_caught = true;
    printf("\nGeneral Exception: code [%d] message [%s]\n", 
	   e.get_code(), e.get_msg().c_str());
  } 
  
  CPPUNIT_ASSERT(!exception_caught);
}

void SessionTestClient::session_test()
{
  connect_test();
  login_test();
  create_org_contact_test();
  send_hello_test();
  logout_test();
}
