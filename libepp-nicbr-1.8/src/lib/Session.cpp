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
/* $Id: Session.cpp 1040 2009-10-01 17:49:12Z fneves $ */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include "Session.H"
#include "EppException.H"
#include "IoException.H"
#include "StrUtil.H"
#include "FileUtil.H"
#include "Login.H"
#include "Hello.H"

LIBEPP_NICBR_NS_BEGIN

Session::Session(const string &server, const int port, 
		 const string &templates_dir) : _server(server), _port(port)
{
  try {    
    _parser = auto_ptr<DomParser>(new DomParser());
    _greeting = auto_ptr<Greeting>(new Greeting());
    _cert_common_name_check_enabled = false;
    read_templates(templates_dir);
    srandomdev();
  } catch (const GeneralException &e) {
    throw; 
  }
}

Session::~Session() {}

void Session::enable_xml_validation(const string &schemas_dir)
{
  if (schemas_dir == "")
    _parser->enable_validation();
  else
    _parser->enable_validation(schemas_dir);
}

void Session::disable_xml_validation()
{
  _parser->disable_validation();
}

void Session::enable_cert_common_name_check()
{
  _cert_common_name_check_enabled = true;
}

void Session::disable_cert_common_name_check()
{
  _cert_common_name_check_enabled = false;
}

void Session::connect(const string &client_cert_file,
		      const string &root_ca_file,
		      const string &pem_passphrase) 
{
  try {
    _transport = auto_ptr<TransportTLS>(new TransportTLS(_server, _port));
    if (_cert_common_name_check_enabled) {
      _transport->enable_cert_common_name_check();
    } else {
      _transport->disable_cert_common_name_check();
    }
    _transport->connect(client_cert_file, root_ca_file, pem_passphrase);
    string greeting;
    _transport->read(greeting);
    _last_response = greeting;
    _parser->parse_greeting(greeting, _greeting.get());
  } catch (const GeneralException &e) {
    throw;  
  }
}

void Session::disconnect()
{
  _transport->disconnect();
}

void Session::send_hello()
{
  Hello hello;
  string greeting;
  try {
    _last_command = hello.get_xml_format();
    _transport->write(hello.get_xml_format());
    _transport->read(greeting);
    _last_response = greeting;
    _parser->parse_greeting(greeting, _greeting.get());
  } catch (const GeneralException &e) {
    throw;  
  }
}

Greeting *Session::get_greeting() 
{
  return _greeting.get();
}

void Session::process_action(Action *client_action, 
			     const string clTRID)
{
  try {
    // Check Client Transaction ID
    string cmd_clTRID = clTRID;
    if (cmd_clTRID == "")
      cmd_clTRID = StrUtil::to_string("%u", random());

    // XML Construction
    string xml_template = _templates[client_action->who_am_i()];
    client_action->get_command()->set_clTRID(cmd_clTRID);
    client_action->set_xml_template(xml_template);   

    // Store command in XML format 
    _last_command = client_action->get_xml();

    // Write Command
    _transport->write(client_action->get_xml());      

    // Read Command
    string xml_payload;    
    _transport->read(xml_payload);

    // Store response in XML format (directly from wire)
    _last_response = xml_payload;

    // XML Parse
    client_action->set_response(xml_payload, _parser.get());    

    // Client Transaction ID Check
    string rsp_clTRID = client_action->get_response()->get_clTRID();
    if (cmd_clTRID != rsp_clTRID) {
      throw EppException(EppException::INVALID_CLTRID, 
			 "Invalid Client Transaction ID in the response");
    }
  } catch (const GeneralException &e) {
    throw;
  }
}

void Session::runXML(const string& input)
{
  try {
    // Store command in XML format 
    _last_command = input;

    // Write Command
    _transport->write(input);

    // Read Command
    string xml_payload;    
    _transport->read(xml_payload);

    // Store response in XML format (directly from wire)
    _last_response = xml_payload;
  } catch (const GeneralException &e) {
    throw;
  }
}

void Session::read_templates(const string &templates_dir)
{
  try {
    _templates[LOGIN] = 
      FileUtil::read_file(templates_dir + "/login.xml");
    _templates[LOGOUT] = 
      FileUtil::read_file(templates_dir + "/logout.xml");
    _templates[POLL] =
      FileUtil::read_file(templates_dir + "/poll.xml");
    _templates[CONTACT_CHECK] = 
      FileUtil::read_file(templates_dir + "/contact_check.xml");
    _templates[CONTACT_CREATE] = 
      FileUtil::read_file(templates_dir + "/contact_create.xml");
    _templates[CONTACT_INFO] =
      FileUtil::read_file(templates_dir + "/contact_info.xml");
    _templates[CONTACT_UPDATE] =
      FileUtil::read_file(templates_dir + "/contact_update.xml");
    _templates[DOMAIN_CHECK] =
      FileUtil::read_file(templates_dir + "/domain_check.xml");
    _templates[DOMAIN_CREATE] =
      FileUtil::read_file(templates_dir + "/domain_create.xml");
    _templates[DOMAIN_INFO] =
      FileUtil::read_file(templates_dir + "/domain_info.xml");
    _templates[DOMAIN_RENEW] =
      FileUtil::read_file(templates_dir + "/domain_renew.xml");
    _templates[DOMAIN_UPDATE] =
      FileUtil::read_file(templates_dir + "/domain_update.xml");
    _templates[DOMAIN_DELETE] =
      FileUtil::read_file(templates_dir + "/domain_delete.xml");
#if USE_BR_DOMAINS
    _templates[BR_DOMAIN_CHECK] =
      FileUtil::read_file(templates_dir + "/br_domain_check.xml");
    _templates[BR_DOMAIN_INFO] =
      FileUtil::read_file(templates_dir + "/br_domain_info.xml");
    _templates[BR_DOMAIN_CREATE] =
      FileUtil::read_file(templates_dir + "/br_domain_create.xml");
    _templates[BR_DOMAIN_RENEW] = _templates[DOMAIN_RENEW];
    _templates[BR_DOMAIN_UPDATE] =
      FileUtil::read_file(templates_dir + "/br_domain_update.xml");
#endif //USE_BR_DOMAINS
#if USE_BR_ORG
    _templates[BR_ORG_CHECK] =
      FileUtil::read_file(templates_dir + "/br_org_check.xml");
    _templates[BR_ORG_INFO] =
      FileUtil::read_file(templates_dir + "/br_org_info.xml");
    _templates[BR_ORG_CREATE] =
      FileUtil::read_file(templates_dir + "/br_org_create.xml");
    _templates[BR_ORG_UPDATE] =
      FileUtil::read_file(templates_dir + "/br_org_update.xml");
#endif //USE_BR_ORG
#if USE_IP_MANAGEMENT
    _templates[IP_NETWORK_CREATE] =
      FileUtil::read_file(templates_dir + "/ipnetwork_create.xml");
    _templates[IP_NETWORK_CHECK] =
      FileUtil::read_file(templates_dir + "/ipnetwork_check.xml");
    _templates[IP_NETWORK_DELETE] =
      FileUtil::read_file(templates_dir + "/ipnetwork_delete.xml");
    _templates[IP_NETWORK_RENEW] =
      FileUtil::read_file(templates_dir + "/ipnetwork_renew.xml");
    _templates[IP_NETWORK_TRANSFER] =
      FileUtil::read_file(templates_dir + "/ipnetwork_transfer.xml");
    _templates[IP_NETWORK_INFO] =
      FileUtil::read_file(templates_dir + "/ipnetwork_info.xml");
    _templates[IP_NETWORK_UPDATE] =
      FileUtil::read_file(templates_dir + "/ipnetwork_update.xml");
    _templates[ASN_CHECK] =
      FileUtil::read_file(templates_dir + "/asn_check.xml");
    _templates[ASN_CREATE] =
      FileUtil::read_file(templates_dir + "/asn_create.xml");
    _templates[ASN_DELETE] =
      FileUtil::read_file(templates_dir + "/asn_delete.xml");
    _templates[ASN_RENEW] =
      FileUtil::read_file(templates_dir + "/asn_renew.xml");
    _templates[ASN_TRANSFER] =
      FileUtil::read_file(templates_dir + "/asn_transfer.xml");
    _templates[ASN_INFO] =
      FileUtil::read_file(templates_dir + "/asn_info.xml");
    _templates[ASN_UPDATE] =
      FileUtil::read_file(templates_dir + "/asn_update.xml");
#endif //USE_IP_MANAGEMENT
  } catch(const IoException &e) {
    throw;
  }
}

string Session::get_last_command() const
{
  return _last_command;
}

string Session::get_last_response() const
{
  return _last_response;
}

string Session::get_server() const
{
  return _server;
}

int Session::get_port() const
{
  return _port;
}

void Session::set_server(const string &server)
{
  _server = server;
}

void Session::set_port(const int &port)
{
  _port = port;
}

LIBEPP_NICBR_NS_END
