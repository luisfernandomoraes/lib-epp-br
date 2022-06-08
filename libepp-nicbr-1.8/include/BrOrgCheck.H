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
/* $Id: BrOrgCheck.H 818 2006-08-22 18:10:42Z milena $ */
/** @file BrOrgCheck.H
 *  @brief EPP BrOrgCheck Class
 */

#ifndef __BR_ORG_CHECK_H__
#define __BR_ORG_CHECK_H__

#include "ContactCheck.H"
#include "BrOrgCheckCmd.H"
#include "BrOrgCheckRsp.H"

LIBEPP_NICBR_NS_BEGIN

/// EPP BrOrgCheck Class
class BrOrgCheck : public ContactCheck
{
public:
  /// Constructor
  BrOrgCheck(const ActionType type = BR_ORG_CHECK) : ContactCheck(type)
  {
    if (type == BR_ORG_CHECK) {
      _command = auto_ptr<BrOrgCheckCmd>(new BrOrgCheckCmd());
      _response = auto_ptr<BrOrgCheckRsp>(new BrOrgCheckRsp());
    }
  }
  
  /// Sets XML template
  /**
     @param xml_template   XML command template
  */
  void set_xml_template(const string &xml_template);

  /// Returns raw pointer to the response
  /**
     @return raw pointer to the response
  */
  BrOrgCheckRsp* get_response() 
  { 
    return (BrOrgCheckRsp*) _response.get(); 
  }
  
  /// Returns raw pointer to the command
  /**
     @return raw pointer to the command
  */
  BrOrgCheckCmd* get_command() 
  { 
    return (BrOrgCheckCmd*) _command.get(); 
  }
 
};

LIBEPP_NICBR_NS_END
#endif //__BR_ORG_CHECK_H__
