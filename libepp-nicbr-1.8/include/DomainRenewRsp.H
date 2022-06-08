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
/* $Id: DomainRenewRsp.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file  DomainRenewRsp.H
 *  @brief EPP DomainRenewRsp Class
 */

#ifndef __DOMAIN_RENEW_RSP_H__
#define __DOMAIN_RENEW_RSP_H__

#include <string>

#include "libepp_nicbr.H"

#include "Response.H"

using std::string;

LIBEPP_NICBR_NS_BEGIN

/// EPP DomainRenewRsp Class
class DomainRenewRsp : public Response
{
public:
  /// Default constructor
  DomainRenewRsp(bool reset = true) : Response(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Sets domain name
  /**
     @param name   domain name
  */
  void set_name(const string& name) { _name = name; }

  /// Returns domain name
  /**
     @return domain name
  */
  string get_name() const { return _name; }

  /// Sets expiration date
  /**
     @param exDate   object expiration date
   */
  void set_exDate(const string& exDate) { _exDate = exDate; }

  /// Returns expiration date
  /**
     @return expiration date
   */
  string get_exDate() const { return _exDate; }

  void reset()
  {
    Response::reset();
    _name = "";
    _exDate = "";
  }

protected:

  /// fully qualified domain name
  string _name;

  /// date and time identifying the end of the domain object's
  /// expiration period
  string _exDate;
  
};

LIBEPP_NICBR_NS_END
#endif //__DOMAIN_RENEW_RSP_H__
