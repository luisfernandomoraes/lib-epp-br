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
/* $Id: SheppCommandFunctions.H 1010 2009-03-12 17:50:50Z eduardo $ */
/** @file SheppCommandFunctions.H
 *  @brief EPP command-line shell client command functions include file
 */
#ifndef __SHEPP_COMMAND_FUNCTIONS_H__
#define __SHEPP_COMMAND_FUNCTIONS_H__

#include <fstream>
#include <unistd.h>   //alarm

//used by getpasswd
#include <termios.h>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include "SheppGlobal.H"
#include "SheppStrUtil.H"
#include "SheppObjSet.H"
#include "SheppPrint.H"

#include "IoException.H"
#include "TransportException.H"
#include "EppException.H"
#include "XmlException.H"

#include "Login.H"
#include "Logout.H"

using std::ifstream;

// Functions for eppsh commands
int cmd_source(vector<string> &);
int cmd_login(vector<string> &);
int cmd_logout(vector<string> &);
int cmd_hello(vector<string> &);
int cmd_poll(vector<string> &);
int cmd_domain(vector<string> &);
int cmd_contact(vector<string> &);
#if USE_BR_ORG
int cmd_brorg(vector<string> &);
#endif //USE_BR_ORG
#if USE_IP_MANAGEMENT
int cmd_asn(vector<string> &);
int cmd_ipnetwork(vector<string> &);
#endif //USE_IP_MANAGEMENT

int cmd_xmlcmd(vector<string> &);
int cmd_xmlrsp(vector<string> &);
int cmd_runxml(vector<string> &);

int cmd_server(vector<string> &);
int cmd_port(vector<string> &);
int cmd_client_pem(vector<string> &);
int cmd_root_pem(vector<string> &);
int cmd_pass(vector<string> &);
int cmd_user(vector<string> &);
int cmd_login_pw(vector<string> &);
int cmd_login_new_pw(vector<string> &);
int cmd_lang(vector<string> &);
int cmd_connect(vector<string> &);
int cmd_disconnect(vector<string> &);

int cmd_help(vector<string> &);
int cmd_about(vector<string> &);
int cmd_quit(vector<string> &);

int cmd_beauty(vector<string> &);
int cmd_debug(vector<string> &);
int cmd_autorsp(vector<string> &);
int cmd_keepalive(vector<string> &);
int cmd_peer_crt_check(vector<string> &);

string getpasswd(string prompt);

/// Populates set of available commands
void init_commands()
{
  SheppCommand cmd;

  //non-EPP commands
  cmd.reset("?", cmd_help, "This help screen");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("help", cmd_help, "This help screen");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("about", cmd_about, "shepp version information");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("exit", cmd_quit, "Exit shepp\n");
  _commands.insert(_commands.end(), cmd);

  //runtime flags
  cmd.reset("beauty", cmd_beauty, "Turn XML beautifier on/off");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("debug", cmd_debug, "Turn debug messages on/off");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("autorsp", cmd_autorsp, "Turn auto-show response on/off");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("keepalive", cmd_keepalive, "Turn keep-alive on/off");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("peer-crt-check", cmd_peer_crt_check,
	    "Turn peer's certificate check on/off\n");
  _commands.insert(_commands.end(), cmd);

  //connection related commands
  cmd.reset("source", cmd_source, "Read and execute file with shepp commands");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("server", cmd_server, "Sets server address");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("port", cmd_port, "Sets server port");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("client-pem", cmd_client_pem,
	    "Sets client.pem certificate file location");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("root-pem", cmd_root_pem,
	    "Sets root.pem certificate file location");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("pass", cmd_pass, "Sets SSL certificate passphrase");
  _commands.insert(_commands.end(), cmd);  
  cmd.reset("user", cmd_user, "Sets EPP Login username");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("pw", cmd_login_pw, "Sets EPP Login password");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("newpw", cmd_login_new_pw, "Sets new EPP Login password");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("lang", cmd_lang, "Sets text language");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("connect", cmd_connect, "Establishes connection");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("disconnect", cmd_disconnect, "Closes connection\n");
  _commands.insert(_commands.end(), cmd);

  //EPP commands
  cmd.reset("login", cmd_login, "EPP Login");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("logout", cmd_logout, "EPP Logout");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("hello", cmd_hello, "EPP Hello");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("poll", cmd_poll, "EPP Poll related actions");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("domain", cmd_domain, "EPP Domain related actions");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("contact", cmd_contact,
#if USE_BR_ORG
	    "EPP Contact related actions"
#else
	    "EPP Contact related actions\n"
#endif
);
  _commands.insert(_commands.end(), cmd);
#if USE_IP_MANAGEMENT
  cmd.reset("asn", cmd_asn, "Autonomous System related actions");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("ipnetwork", cmd_ipnetwork, "IP network related actions");
  _commands.insert(_commands.end(), cmd);
#endif //USE_IP_MANAGEMENT
#if USE_BR_ORG
  cmd.reset("brorg", cmd_brorg, "EPP BrOrg related actions\n");
  _commands.insert(_commands.end(), cmd);
#endif //USE_BR_ORG

  //XML related commands
  cmd.reset("xmlcmd", cmd_xmlcmd, "Show last command XML");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("xmlrsp", cmd_xmlrsp, "Show last response XML");
  _commands.insert(_commands.end(), cmd);
  cmd.reset("runxml", cmd_runxml, "Run XML input (good for debugging)");
  _commands.insert(_commands.end(), cmd);
}

/// Given a word check if it's a valid SheppCommand
/**
   @param   name of command check validity
   @return pointer to a SheppCommand if found, NULL pointer if not found
*/
const SheppCommand* find_command(string name)
{
  list<SheppCommand>::const_iterator it;
  for (it = _commands.begin(); it != _commands.end(); it++) {
    if ((*it).name == name) {
      return &(*it);
    }
  }

  return ((const SheppCommand *) NULL);
}

/// Given a input file, read and execute all shepp commands in this
/// file
/**
   @param inputFile   input file with shepp commands
   @return 0 if ok, -1 otherwise
*/
int executeInputFile(const string &file)
{
  ifstream inputFile;
  inputFile.open(file.c_str());
  if (inputFile.eof() == true || 
      inputFile.fail() == true || 
      inputFile.bad() == true) {
    printf("error reading input file.\n");
    return -1;
  }

  init_commands();

  while (inputFile.good() == true) {
    string cmdLine("");
    getline(inputFile, cmdLine);

    SheppStrUtil::trim((char*) cmdLine.c_str());
    if (cmdLine.size() == 0) {
      continue;
    }

    // Comments symbols
    if (cmdLine[0] == '#' || cmdLine[0] == ';') {
      continue;
    }

    // get the first word (command name)
    vector<string> command_args = 
      SheppStrUtil::parse_line((char*) cmdLine.c_str());
    
    // args will be empty if any word exceed MAX_WORD_LENGTH
    if (command_args.empty()) {
      continue;
    }
    
    const SheppCommand* cmd = find_command(command_args[0]);
    
    if (cmd == NULL) {
      fprintf(stderr, "Unknown command: %s\n", command_args[0].c_str());
      continue;
    }
    
    // commands that require raw command-line input
    if (command_args[0] == "runxml") {
      string rawcmd(cmdLine);
      rawcmd = rawcmd.substr(command_args[0].length());
      while (whitespace(rawcmd[0])) {
	rawcmd = rawcmd.substr(1);
      }
      
      if (rawcmd == "") {
	// read input
	printf("Enter XML code. Escape character is '^]'.\n");

	string unit;
	getline(cin, unit, (char)29);
	rawcmd += unit;
      }
      
      command_args.clear();
      command_args.push_back(rawcmd);
    } else {
      command_args.erase(command_args.begin());
    }

    (*(cmd->function))(command_args);
  }

  inputFile.close();

  return 0;
}

/// Given a input file, read and execute all shepp commands in this
/// file
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_source(vector<string> &args)
{
  if (args.size() != 0) {
    return executeInputFile(args[0]);
  }

  return 0;
}

/// Sets server address [and port] based on input command-line
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_server(vector<string> &args)
{
  if (args.size() != 0) {
    string new_port;
    string new_server;
    SheppStrUtil::split(args[0], new_server, new_port, ":", true);
    if (new_server != _server) {
      _server = new_server;
    }
    if (new_port != "" && _port != atoi(new_port.c_str())) {
      _port = atoi(new_port.c_str());
    }
  }

  if (_server != "") {
    printf("server %s:%d\n", _server.c_str(), _port);
  } else {
    printf("no server address given.\n");
  }

  return 0;
}

/// Sets server port based on input command-line
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_port(vector<string> &args)
{
  if (args.size() == 1) {
    if (_port != atoi(args[0].c_str())) {
      _port = atoi(args[0].c_str());
    }
  }

  printf("port %d\n", _port);

  return 0;
}

/// Sets client.pem certificate file location
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_client_pem(vector<string> &args)
{
  if (args.size() == 1) {
    if (_client_pem != args[0]) {
      _client_pem = args[0];
      printf("Warning: changing cetificates requires you to reconnect.\n");
    }
  } else if (args.size() > 1) {
    printf("usage: client-pem <file>\n");
    return -1;
  }

  printf("Current client.pem is %s\n", _client_pem.c_str());
  return 0;
}

/// Sets root.pem certificate file location
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_root_pem(vector<string> &args)
{
  if (args.size() == 1) {
    if (_root_pem != args[0]) {
      _root_pem = args[0];
      printf("Warning: changing cetificates requires you to reconnect.\n");
    }
  } else if (args.size() > 1) {
    printf("usage: root-pem <file>\n");
    return -1;
  }

  printf("Current root.pem is %s\n", _root_pem.c_str());
  return 0;
}

/// Sets SSL certificate passphrase based on input command-line
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_pass(vector<string> &args)
{
  if (!args.empty()) {
    _passphrase = args[0];
  } else {
    _passphrase = getpasswd("Passphrase");
  }

  return 0;
}

/// Sets login user based on input command-line
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_user(vector<string> &args)
{
  if (args.size() == 0) {
    if (_user != "") {
      printf("username: %s\n", _user.c_str());
      return 0;
    } else {
      printf("no username given.\n");
      return -1;
    }
  }

  _user = args[0];

  return 0;
}

/// Sets EPP login password based on input command-line
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_login_pw(vector<string> &args)
{
  if (!args.empty()) {
    _login_pw = args[0];
  } else {
    _login_pw = getpasswd("Password");
  }

  return 0;
}

/// Sets new EPP login password based on input command-line
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_login_new_pw(vector<string> &args)
{
  if (!args.empty()) {
    _login_new_pw = args[0];
  } else {
    _login_new_pw = getpasswd("New Password");
  }

  _new_pw = true;
  return 0;
}

/// Sets text language based on input command-line
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_lang(vector<string> &args)
{
  if (args.empty()) {
    printf("usage: lang [en|pt]\n");
    return -1;
  } else {
    _lang = args[0];
  }

  return 0;
}

/// Connects to previously specified server and port
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_connect(vector<string> &args)
{
  if (cmd_server(args) != 0) {
    return -1;
  }

  bool exception = false;

  // Session code goes here
  try {
    _session = auto_ptr<Session>(new Session(_server, _port));
    if (_peer_crt_check) {
      _session->enable_cert_common_name_check();
    } else {
      _session->disable_cert_common_name_check();
    }
    _session->connect(_client_pem, _root_pem, _passphrase);
    printf("Connected to %s\n", _server.c_str());
    
    Greeting *greeting = _session->get_greeting();  
    if (_debug) {
      if (greeting) {
	printf("Greeting received\n");
      }
    } //_debug

    _xmlrsp = _session->get_last_response();
    _xmlcmd = _session->get_last_command();

    _connected = true;

  } catch (const IoException &e) {
    printf("ERROR! IO Exception [%d]:\n[%s]\n", 
	   e.get_code(), e.get_msg().c_str());
    exception = true;
  } catch (const TransportException &e) {
    printf("ERROR! Transport Exception [%d]:\n[%s]\n[%s]\n",
	   e.get_code(), e.get_msg().c_str(), e.get_low_level_msg().c_str());
    exception = true;
  } catch (const GeneralException &e) {
    printf("ERROR! General Exception [%d]:\n[%s]\n", 
	   e.get_code(), e.get_msg().c_str());
    exception = true;
  }

  if (exception) {
    return -1;
  }

  // print greeting
  vector<string> dummy;
  cmd_xmlrsp(dummy);

  return 0;
}

/// Closes connection
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_disconnect(vector<string> &args)
{
  if (_connected) {
    bool exception = false;

    // Session code goes here
    try {
      _session->disconnect();
      printf("disconnected from %s\n", _server.c_str());
      _connected = false;
    } catch (const TransportException &e) {
      printf("ERROR! Transport Exception [%d]:\n[%s]\n[%s]\n",
	     e.get_code(), e.get_msg().c_str(), e.get_low_level_msg().c_str());
      exception = true;
    } catch (const GeneralException &e) {
      printf("ERROR! General Exception [%d]:\n[%s]\n", 
	     e.get_code(), e.get_msg().c_str());
      exception = true;
    }

    if (exception) {
      return -1;
    }
  } else {
    printf("not connected\n");
  }

  return 0;
}

/// Turns XML beautifier ON and OFF
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_beauty(vector<string> &args)
{
  if (args.empty()) {
    if (_beauty) {
      printf("XML beautifier is ON\n");
    } else {
      printf("XML beautifier is OFF\n");
    }
    return 0;
  } else if (args.size() > 1) {
    printf("usage: beauty [on|off]\n");
    return -1;
  }

  if (args[0] == "on") {
    _beauty = true;
    printf("XML beautifier now ON\n");
  } else if (args[0] == "off") {
    _beauty = false;
    printf("XML beautifier now OFF\n");
  } else {
    printf("usage: beauty [on|off]\n");
    return -1;
  }

  return 0;
}

/// Turns debug messages ON and OFF
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_debug(vector<string> &args)
{
  if (args.empty()) {
    if (_debug) {
      printf("Debug is ON\n");
    } else {
      printf("Debug is OFF\n");
    }
    return 0;
  } else if (args.size() > 1) {
    printf("usage: debug [on|off]\n");
    return -1;
  }

  if (args[0] == "on") {
    _debug = true;
    printf("Debug now ON\n");
  } else if (args[0] == "off") {
    _debug = false;
    printf("Debug now OFF\n");
  } else {
    printf("usage: beauty [on|off]\n");
    return -1;
  }

  return 0;
}

/// Turns auto-show response ON and OFF
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_autorsp(vector<string> &args)
{
  if (args.empty()) {
    if (_autorsp) {
      printf("autorsp is ON\n");
    } else {
      printf("autorsp is OFF\n");
    }
    return 0;
  } else if (args.size() > 1) {
    printf("usage: autorsp [on|off]\n");
    return -1;
  }

  if (args[0] == "on") {
    _autorsp = true;
    printf("autorsp now ON\n");
  } else if (args[0] == "off") {
    _autorsp = false;
    printf("autorsp now OFF\n");
  } else {
    printf("usage: autorsp [on|off]\n");
    return -1;
  }

  return 0;
}

/// Turns keep-alive ON and OFF
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_keepalive(vector<string> &args)
{
  if (args.empty()) {
    if (_keepalive) {
      printf("Keep-alive is ON\n");
    } else {
      printf("Keep-alive is OFF\n");
    }
    return 0;
  } else if (args.size() > 1) {
    printf("usage: keepalive [on|off]\n");
    return -1;
  }

  if (args[0] == "on") {
    _keepalive = true;
    printf("Keep-alive now ON\n");
  } else if (args[0] == "off") {
    _keepalive = false;
    printf("Keep-alive now OFF\n");
  } else {
    printf("usage: keepalive [on|off]\n");
    return -1;
  }

  if (_keepalive) {
    alarm(_keepalive_timer);
  } else {
    alarm(0);
  }

  return 0;
}

/// Turn peer's certificate check ON and OFF
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_peer_crt_check(vector<string> &args)
{
  if (args.empty()) {
    if (_peer_crt_check) {
      printf("Peer's certificate check is ON\n");
    } else {
      printf("Peer's certificate check is OFF\n");
    }
    return 0;
  } else if (args.size() > 1) {
    printf("usage: peer-crt-check [on|off]\n");
    return -1;
  }

  if (args[0] == "on") {
    _peer_crt_check = true;
    printf("Peer's certificate check now ON\n");
  } else if (args[0] == "off") {
    _peer_crt_check = false;
    printf("Peer's certificate check now OFF\n");
  } else {
    printf("usage: peer-crt-check [on|off]\n");
    return -1;
  }

  return 0;
}

/// SIGALRM signal handler for keepalive command
static void sig_alrm(int signo)
{
  if (_connected && !_cmd_running) {
    _ka_running = true;
    vector<string> args;
    args.push_back((string) "keepalive");
    cmd_hello(args);
    _ka_running = false;
  }
  alarm(_keepalive_timer);
}

/// Shows last XML command
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_xmlcmd(vector<string> &args)
{
  if (!_connected) {
    printf("not connected\n");
    return -1;
  }

  if (_beauty) {
    // Convert to UTF8
    string last_command("");
    StrUtil::iso88591_to_utf8(_xmlcmd, last_command);
    
    if (_session->get_last_command() == "") {
      printf("\n");
      return 0;
    }
    try {
      StrUtil str_util;
      printf("%s\n", str_util.xml_beautifier(last_command).c_str());
    } catch (const XmlException &e) {
      printf("ERROR! XML Exception [%d]:\n[%s]\n", e.get_code(),
	     e.get_msg().c_str());
    }
  } else {
    printf("%s\n", _session->get_last_command().c_str());
  }
  return 0;
}

/// Shows last XML response
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_xmlrsp(vector<string> &args)
{
  if (!_connected) {
    printf("not connected\n");
    return -1;
  }

  if (_beauty) {
    // No need to convert to UTF8
    if (_xmlrsp == "") {
      printf("\n");
      return 0;
    }
    try {
      StrUtil str_util;
      printf("%s\n", str_util.xml_beautifier(_xmlrsp).c_str());
    } catch (const XmlException &e) {
      printf("ERROR! XML Exception [%d]:\n[%s]\n", e.get_code(),
	     e.get_msg().c_str());
    }
  } else {
    printf("%s\n", _xmlrsp.c_str());
  }
  return 0;
}

// EPP action processing functions

/// Prints "command sent ok" message
void print_cmd_sent_ok()
{
  if (!_autorsp) {
    printf("Ok! Use 'xmlcmd' and 'xmlrsp' to view command/response "
	   "XML code.\n");
  } else {
    vector<string> dummy;
    cmd_xmlrsp(dummy);
  }
}

/// Send and EPP Action
/**
   @return 0 if ok, -1 otherwise
*/
int process_action(Action &act)
{
  if (!_connected) {
    printf("not connected\n");
    return -1;
  }

  bool exception = false;

  // prevents concurrency with keep-alive
  if (!_ka_running) {
    _cmd_running = true;
  } else {
    printf("ERROR! Keep-alive is running. Please try again.\n");
    return -1;
  }

  try {
    _session->process_action(&act);
    _xmlrsp = _session->get_last_response();
    _xmlcmd = _session->get_last_command();
  } catch (const EppException &e) {
    printf("ERROR! EPP Exception [%d]:\n[%s]\n", e.get_code(),
	   e.get_msg().c_str());  
    exception = true;
  } catch (const IoException &e) {
    printf("ERROR! IO Exception [%d]:\n[%s]\n", 
	   e.get_code(), e.get_msg().c_str());
    exception = true;
    _connected = false;
  } catch (const TransportException &e) {
    printf("ERROR! Transport Exception [%d]:\n[%s]\n[%s]\n",
	   e.get_code(), e.get_msg().c_str(), e.get_low_level_msg().c_str());
    exception = true;
    _connected = false;
  } catch (const GeneralException &e) {
    printf("ERROR! General Exception [%d]:\n[%s]\n", 
	   e.get_code(), e.get_msg().c_str());
    exception = true;
  }

  if (exception) {
    _cmd_running = false;
    return -1;
  }

  print_cmd_sent_ok();

  // a command was just run; postpone keepalive alarm
  if (_keepalive) {
    alarm(_keepalive_timer);
  }

  _cmd_running = false;
  return 0;
}

/// Run XML input command
/**
   @return 0 if ok, -1 otherwise
*/
int cmd_runxml(vector<string> &args)
{
  if (!_connected) {
    printf("not connected\n");
    return -1;
  }

  bool exception = false;

  // prevents concurrency with keep-alive
  if (!_ka_running) {
    _cmd_running = true;
  } else {
    printf("ERROR! Keep-alive is running. Please try again.\n");
    return -1;
  }

  try {
    _session->runXML(args[0]);
    _xmlrsp = _session->get_last_response();
    _xmlcmd = _session->get_last_command();
  } catch (const EppException &e) {
    printf("ERROR! EPP Exception [%d]:\n[%s]\n", e.get_code(),
	   e.get_msg().c_str());  
    exception = true;
  } catch (const IoException &e) {
    printf("ERROR! IO Exception [%d]:\n[%s]\n", 
	   e.get_code(), e.get_msg().c_str());
    exception = true;
    _connected = false;
  } catch (const TransportException &e) {
    printf("ERROR! Transport Exception [%d]:\n[%s]\n[%s]\n",
	   e.get_code(), e.get_msg().c_str(), e.get_low_level_msg().c_str());
    exception = true;
    _connected = false;
  } catch (const GeneralException &e) {
    printf("ERROR! General Exception [%d]:\n[%s]\n", 
	   e.get_code(), e.get_msg().c_str());
    exception = true;
  }

  if (exception) {
    _cmd_running = false;
    return -1;
  }

  print_cmd_sent_ok();

  // a command was just run; postpone keepalive alarm
  if (_keepalive) {
    alarm(_keepalive_timer);
  }

  _cmd_running = false;
  return 0;
}

/// Send an EPP Login
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_login(vector<string> &args)
{
  if (!_connected) {
    printf("not connected\n");
    return -1;
  }

  if (_user == "") {
    printf("no username given.\n");
    return -1;
  }

  Login act;
  LoginCmd *cmd = act.get_command();

  cmd->set_clID(_user);
  cmd->set_pw(_login_pw);

  if (_new_pw) {
    _new_pw = false;
    cmd->set_new_pw(_login_new_pw);
  }

  if (_lang != "" && _lang != "en") {
    cmd->set_lang(_lang);
  }

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// Send an EPP Logout
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_logout(vector<string> &args)
{
  if (!_connected) {
    printf("not connected\n");
    return -1;
  }

  Logout act;

  if (process_action(act) != 0) {
    return -1;
  }  

  return 0;
}

/// Send an EPP HELLO
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_hello(vector<string> &args)
{
  if (!_connected) {
    printf("not connected\n");
    return -1;
  }

  if (_ka_running &&
      !(args.size() == 1 && args[0] == "keepalive")) {
    printf("ERROR! Keep-alive is running. Please try again.\n");
    return -1;
  }

  bool exception = false;

  // prevents concurrency with keep-alive
  if (!_ka_running) {
    _cmd_running = true;
  }

  try {
    if (_debug) {
      printf("Sending EPP Hello\n");
    }
    _session->send_hello();
    // updates _xmlcmd and _xmlrsp only if it's not a keep-alive run
    if (!_ka_running) {
      _xmlrsp = _session->get_last_response();
      _xmlcmd = _session->get_last_command();
    }
  } catch (const EppException &e) {
    printf("ERROR! EPP Exception [%d]:\n[%s]\n", e.get_code(),
	   e.get_msg().c_str());  
    exception = true;
  } catch (const IoException &e) {
    printf("ERROR! IO Exception [%d]:\n[%s]\n", 
	   e.get_code(), e.get_msg().c_str());
    exception = true;
    _connected = false;
  } catch (const TransportException &e) {
    printf("ERROR! Transport Exception [%d]:\n[%s]\n[%s]\n",
	   e.get_code(), e.get_msg().c_str(), e.get_low_level_msg().c_str());
    exception = true;
    _connected = false;
  } catch (const GeneralException &e) {
    printf("ERROR! General Exception [%d]:\n[%s]\n", 
	   e.get_code(), e.get_msg().c_str());
    exception = true;
  }

  if (exception) {
    if (_cmd_running) {
      _cmd_running = false;
    }
    return -1;
  }

  if (!_ka_running) {
    print_cmd_sent_ok();
  }

  if (_cmd_running) {
    // a command was just run; postpone keepalive alarm
    if (_keepalive) {
      alarm(_keepalive_timer);
    }
    _cmd_running = false;
  }
  return 0;
}

// non-EPP commands

/// Leaves shepp
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_quit(vector<string> &args)
{
  if (_connected) {
    _session->disconnect();
  }
  printf("Bye\n");
  exit(0);
}

/// Prints help
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_help(vector<string> &args)
{
  list<SheppCommand>::const_iterator it;
  for (it = _commands.begin(); it != _commands.end(); it++) {
    printf("%-16s %s\n", (*it).name.c_str(), (*it).brief.c_str());
  }

  return 0;
}

/// Prints version information
/**
   @param args   input command-line
   @return 0 if ok, -1 otherwise
*/
int cmd_about(vector<string> &args)
{
  about();
  return 0;
}

/// Function to read from stdin without echoing
/**
   @param prompt   prompt to be echoed
   @return content read from stdin
*/
string getpasswd(string prompt) {
  printf("%s:", prompt.c_str());

  string passwd("");
  struct termios old_opts;
  struct termios new_opts;

  int res = 0;

  // store old settings
  res = tcgetattr(STDIN_FILENO, &old_opts);
  assert(res == 0);

  // set new terminal parms
  memcpy(&new_opts, &old_opts, sizeof(new_opts));
  new_opts.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
  new_opts.c_lflag |= ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);

  // read password
  getline(cin, passwd);
  cout << endl;

  // restore old settings
  res = tcsetattr(STDIN_FILENO, TCSANOW, &old_opts);
  assert(res == 0);

  return passwd;
}

#endif //__SHEPP_COMMAND_FUNCTIONS_H__
