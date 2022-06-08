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
/* $Id: Login.H 543 2006-03-07 12:31:53Z cesar $ */
/** @file Login.H
 *  @brief EPP Login Class
 */

#ifndef __LOGIN_H__
#define __LOGIN_H__

#include <memory>

#include "libepp_nicbr.H"

#include "Action.H"
#include "LoginCmd.H"
#include "Response.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_BEGIN

/// EPP Login Class
class Login : public Action
{
public:

  /// Constructor
  Login(const ActionType type = LOGIN) : Action(type)
  {
    if (type == LOGIN) {
      _command = auto_ptr<LoginCmd>(new LoginCmd());
      _response = auto_ptr<Response>(new Response());
    }
  }

  /// Sets the xml template and parses the tags
  /**
     @param xml_template XML command template
  */
  void set_xml_template(const string &xml_template);

  /// Sets the response attribute
  /**
     @param xml_payload   XML document
     @param parser        reference to the XML parser
  */
  void set_response(const string &xml_payload, DomParser *parser)
  {
    get_response()->reset();
    parser->parse_login_rsp(xml_payload, _response.get());
  }

  /// Returns raw pointer to the command
  /**
     @return raw pointer to the command
  */
  LoginCmd* get_command() { return (LoginCmd*) _command.get(); }  

};

LIBEPP_NICBR_NS_END
#endif //__LOGIN_H__
