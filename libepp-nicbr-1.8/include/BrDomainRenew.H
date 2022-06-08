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
/* $Id: BrDomainRenew.H 696 2006-04-26 21:40:45Z koji $ */
/** @file BrDomainRenew.H
 *  @brief EPP BrDomainRenew extension Class
 */

#ifndef __BR_DOMAIN_RENEW_H__
#define __BR_DOMAIN_RENEW_H__

#include "DomainRenew.H"
#include "DomainRenewCmd.H"
#include "BrDomainRenewRsp.H"

LIBEPP_NICBR_NS_BEGIN

/// EPP BrDomainRenew extension Class
class BrDomainRenew : public DomainRenew
{
public:

  /// Constructor
  BrDomainRenew(const ActionType type = BR_DOMAIN_RENEW)
    : DomainRenew(type)
  {
    if (type == BR_DOMAIN_RENEW) {
      _command = auto_ptr<DomainRenewCmd>(new DomainRenewCmd());
      _response = auto_ptr<BrDomainRenewRsp>(new BrDomainRenewRsp());
    }
  }

  /// Returns raw pointer to the command
  /**
     @return raw pointer to the command
  */
  DomainRenewCmd* get_command()
  {
    return (DomainRenewCmd *) _command.get();
  }

  /// Returns raw pointer to the response
  /**
     @return raw pointer to the response
  */
  BrDomainRenewRsp* get_response()
  {
    return (BrDomainRenewRsp *) _response.get();
  }
};

LIBEPP_NICBR_NS_END
#endif //__BR_DOMAIN_RENEW_H__
