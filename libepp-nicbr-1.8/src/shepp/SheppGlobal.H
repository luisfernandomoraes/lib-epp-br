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
/* $Id: SheppGlobal.H 1028 2009-08-24 20:01:56Z eduardo $ */
/** @file SheppGlobal.H
 *  @brief shepp compiler definitions and global variables
 */
#ifndef __SHEPP_GLOBAL_H__
#define __SHEPP_GLOBAL_H__

#include <string>
#include <memory>

#include "libepp_nicbr.H"
#include "Session.H"

#include "SheppCommand.H"

#define SHEPP_VERSION  "1.8"
#define DEFAULT_PORT   700
#define CLIENT_PEM     SSLCERTDIR "/client.pem"
#define ROOT_PEM       SSLCERTDIR "/root.pem"

// Limits readline input size if LIMITED_READLINE is defined
#define MAX_TERM_DEV_LINE_SIZE 4096

// Maximmum length a single input unit can have. This value is used by
// SheppStrUtil.H only to allocate a temporary buffer.
#define MAX_WORD_LENGTH        2048

#include <list>

using std::list;
using std::string;
using std::auto_ptr;

LIBEPP_NICBR_NS_USE

/// Global container of available commands
list<SheppCommand> _commands;

/// Global connection setup variables
string _server     = "localhost";
int    _port       = DEFAULT_PORT;
string _user       = "test";
string _passphrase = "client passphrase";
string _client_pem = CLIENT_PEM;
string _root_pem   = ROOT_PEM;      
bool   _connected  = false;

string _login_pw     = "";
string _login_new_pw = "";
bool   _new_pw       = false;
string _lang         = "";

bool   _beauty     = true;
bool   _debug      = false;
bool   _autorsp    = true;

bool   _keepalive  = false;
bool   _ka_running = false;
bool   _cmd_running= false;
static int _keepalive_timer = 180;

bool   _peer_crt_check = true;

string _xmlrsp = "";
string _xmlcmd = "";

/// Reference to session
auto_ptr<Session> _session;

/// shepp build info
void about()
{
  printf("shepp version " SHEPP_VERSION ", an EPP client shell!\n");
  printf("Copyright 2006-2009 Registro.br <libepp@registro.br>\n");
  printf("shepp is distributed with libepp-nicbr: "
	 "http://registro.br/epp/index-EN.html\n");

  set<string> extensions;

#if USE_BR_ORG
  extensions.insert("brorg");
#endif //USE_BR_ORG
#if USE_BR_DOMAINS
  extensions.insert("brdomain");
#endif //USE_BR_DOMAINS
#if USE_IP_MANAGEMENT
  extensions.insert("ipnetwork");
  extensions.insert("asn");
#endif //USE_IP_MANAGEMENT

  printf("extensions: ");
  if (!extensions.empty()) {
    set<string>::iterator ext;
    for (ext = extensions.begin(); ext != extensions.end(); ext++) {
      printf("%s ", (*ext).c_str());
    }
    printf("\n");
  } else {
    printf("(none)\n");
  }
}
#endif //__SHEPP_GLOBAL_H__
