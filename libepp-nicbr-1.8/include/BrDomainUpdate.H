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
/* $Id: BrDomainUpdate.H 543 2006-03-07 12:31:53Z cesar $ */
/** @file BrDomainUpdate.H
 *  @brief EPP BrDomainUpdate extension Class
 */

#ifndef __BR_DOMAIN_UPDATE_H__
#define __BR_DOMAIN_UPDATE_H__

#include "DomainUpdate.H"
#include "BrDomainUpdateCmd.H"
#include "BrDomainUpdateRsp.H"

LIBEPP_NICBR_NS_BEGIN

/// EPP BrDomainUpdate extension Class
class BrDomainUpdate : public DomainUpdate
{
public:

  /// Constructor
  BrDomainUpdate(const ActionType type = BR_DOMAIN_UPDATE)
    : DomainUpdate(type)
  {
    if (type == BR_DOMAIN_UPDATE) {
      _command = auto_ptr<BrDomainUpdateCmd>(new BrDomainUpdateCmd());
      _response = auto_ptr<BrDomainUpdateRsp>(new BrDomainUpdateRsp());
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
  BrDomainUpdateCmd* get_command()
  {
    return (BrDomainUpdateCmd *) _command.get();
  }

  /// Returns raw pointer to the response
  /**
     @return raw pointer to the response
  */
  BrDomainUpdateRsp* get_response()
  {
    return (BrDomainUpdateRsp *) _response.get();
  }
};

LIBEPP_NICBR_NS_END
#endif //__BR_DOMAIN_UPDATE_H__
