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
/* $Id: AsnCreate.H 945 2008-12-09 16:44:26Z rafael $ */
/** @file AsnCreate.H
 *  @brief EPP AsnCreate Class
 */

#ifndef __ASNCREATE_H__
#define __ASNCREATE_H__

#include "libepp_nicbr.H"

#include "Action.H"
#include "AsnCreateCmd.H"
#include "AsnCreateRsp.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_BEGIN

/// EPP AsnCreate Class
class AsnCreate : public Action
{
public:
  /// Constructor
  AsnCreate(const ActionType type = ASN_CREATE) : Action(type)
  {
    if (type == ASN_CREATE) {
      _command = auto_ptr<AsnCreateCmd>(new AsnCreateCmd());
      _response = auto_ptr<AsnCreateRsp>(new AsnCreateRsp());
    }
  }

  /// Sets XML template
  /**
     @param xml_template   XML command template
  */
  void set_xml_template(const string &xml_template);

  /// Pure virtual method to set response from a XML document
  /**
     @param xml_payload   XML document
     @param parser        reference to the XML parser
   */
  void set_response(const string &xml_payload, DomParser *parser) 
  {
    get_response()->reset();
    parser->parse_asn_create_rsp(xml_payload, get_response());
  }

  /// Returns raw pointer to the command
  /**
     @return raw pointer to the command
  */
  AsnCreateCmd* get_command() 
  {
    return (AsnCreateCmd*) _command.get(); 
  }

  /// Returns raw pointer to the response
  /**
     @return raw pointer to the response
  */
  AsnCreateRsp* get_response() 
  { 
    return (AsnCreateRsp*) _response.get(); 
  }
};

LIBEPP_NICBR_NS_END
#endif // __ASNCREATE_H__
