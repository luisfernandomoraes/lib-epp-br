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
/* $Id: BrOrgInfo.H 543 2006-03-07 12:31:53Z cesar $ */
/** @file BrOrgInfo.H
 *  @brief EPP BrOrgInfo Class
 */

#ifndef __BR_ORG_INFO_H__
#define __BR_ORG_INFO_H__

#include <memory>

#include "libepp_nicbr.H"

#include "ContactInfo.H"
#include "BrOrgInfoCmd.H"
#include "BrOrgInfoRsp.H"

using std::auto_ptr;

LIBEPP_NICBR_NS_BEGIN

/// EPP BrOrgInfo Class
class BrOrgInfo : public ContactInfo
{
public:
  /// Constructor
  BrOrgInfo(const ActionType type = BR_ORG_INFO) : ContactInfo(type)
  {
    if (type == BR_ORG_INFO) {
      _command = auto_ptr<BrOrgInfoCmd>(new BrOrgInfoCmd());
      _response = auto_ptr<BrOrgInfoRsp>(new BrOrgInfoRsp());
    }
  }

  /// Sets XML template
  /**
     @param xml_template   XML command template
  */
  void set_xml_template(const string &xml_template);

  /// Returns raw pointer to the command
  /**
     @return raw pointer to the command
  */
  BrOrgInfoCmd* get_command() 
  { 
    return (BrOrgInfoCmd*) _command.get(); 
  }

  /// Returns raw pointer to the response
  /**
     @return raw pointer to the response
  */
  BrOrgInfoRsp* get_response() 
  {
    return (BrOrgInfoRsp*) _response.get(); 
  }

};

LIBEPP_NICBR_NS_END
#endif //__BR_ORG_INFO_H__
