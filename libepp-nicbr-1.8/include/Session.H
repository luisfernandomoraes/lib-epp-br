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
/* $Id: Session.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file Session.H
 *  @brief EPP Session Class
 */

#ifndef __SESSION_H__
#define __SESSION_H__

#include <map>

#include "libepp_nicbr.H"

#include "DomParser.H"
#include "TransportTLS.H"
#include "Greeting.H"
#include "Action.H"

//for systems that don't have srandomdev (e.g. Linux)
#ifndef HAVE_SRANDOMDEV
#define srandomdev() srand((unsigned) time(NULL))
#endif

using std::auto_ptr;
using std::map;

LIBEPP_NICBR_NS_BEGIN

/// EPP Session Class
class Session
{
public:
  /// Constructor
  /**
     @param server Epp Server Name
     @param port Epp Server Port Number
     @param templates_dir Optional Path to the XML Templates Directory
  */
  Session(const string &server = "localhost", const int port = 700, 
	  const string &templates_dir = TEMPLATESDIR);
  
  /// Destructor
  ~Session();
  
  /// Enable XML Parser Validation
  /**
     @param schemas_dir Optional Path to the XML schemas
  */
  void enable_xml_validation(const string &schemas_dir = SCHEMASDIR);

  /// Disable XML Parser Validation
  void disable_xml_validation();

  /// Enables peer's certificate common name check
  void enable_cert_common_name_check();

  /// Disables peer's certificate common name check
  void disable_cert_common_name_check();

  /// Establish the Connection
  /**
     @param client_cert_file Client certificate file
     @param root_ca_file Root certificate file
     @param pem_passphrase Optional passphrase for an encrypted private key
  */
  void connect(const string &client_cert_file, const string &root_ca_file,
	       const string &pem_passphrase = "");

  /// Close the connection
  void disconnect();

  /// Send Hello 
  void send_hello();

  /// Get Greeting
  /**
     @return Pointer to the Greeting 
  */
  Greeting *get_greeting();

  /// Process Action
  /**
     @param action_to_be_processed Action to be processed
     @param clTRID Optional Client Transaction ID
  */
  void process_action(Action *action_to_be_processed, 
		      const string clTRID = "");

  /// Send XML input command
  /**
     @param input XML input command
  */
  void runXML(const string& input);

  /// Get last command
  /**
     @return Return the last command in XML format
   */
  string get_last_command() const;

  /// Get last response
  /**
     @return Return the last response in XML format
   */
  string get_last_response() const;

  /// Get EPP server
  /**
     @return EPP server name
  */
  string get_server() const;

  /// Get EPP server port
  /**
     @return EPP server port
  */
  int get_port() const;
  
  /// Set EPP server name
  /**
     @param server EPP server name
  */
  void set_server(const string &server);

  /// Set EPP server port
  /**
     @param port EPP server port
  */
  void set_port(const int &port);

private:
  /// Read XML Templates
  /**
     @param templates_dir Path to the XML templates directory
  */
  void read_templates(const string &templates_dir = TEMPLATESDIR);

  /// DOM Parser
  auto_ptr<DomParser> _parser;

  /// Transport
  auto_ptr<TransportTLS> _transport;
  
  /// Greeting
  auto_ptr<Greeting> _greeting;

  /// XML Templates
  map < ActionType, string, less<ActionType> > _templates;

  /// Last command in XML format
  string _last_command;

  /// Last response in XML format
  string _last_response;

  /// EPP server
  string _server;
  
  /// EPP server port
  int _port;

  /// Flag for peer's certificate CN check
  bool _cert_common_name_check_enabled;
};


LIBEPP_NICBR_NS_END
#endif //__SESSION_H__
