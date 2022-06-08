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
/* $Id: AsnRenewRsp.H 947 2008-12-09 18:31:03Z rafael $ */
/** @file AsnRenewRsp.H
 *  @brief EPP AsnRenewRsp Class
 */

#ifndef __ASNRENEWRSP_H__
#define __ASNRENEWRSP_H__

#include "libepp_nicbr.H"

#include "Response.H"

LIBEPP_NICBR_NS_BEGIN

/// EPP AnsRenewRsp Class
class AsnRenewRsp : public Response
{
public:
  AsnRenewRsp(bool reset = true) : Response(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Sets asn
  /**
     @param asn   autonomous system number
  */
  void set_asn(const int asn)
  {
    _asn = asn;
  }

  /// Returns asn
  /**
     @return autonomous system number
  */
  int get_asn() const
  {
    return _asn;
  }

  /// Sets expiration date
  /**
     @param expDate   date and time identifying the end of the AS object's 
                      registration period
  */
  void set_expDate(const string &expDate)
  {
    _expDate = expDate;
  }

  /// Returns expiration date
  /**
     @return date and time identifying the end of the AS object's 
             registration period
  */
  string get_expDate() const
  {
    return _expDate;
  }

  void reset()
  {
    Response::reset();
    _asn = 0;
    _expDate = "";
  }

protected:
  /// autonomous system number
  int _asn;
  
  /// date and time identifying the end of the AS object's registration period
  string _expDate;
};

LIBEPP_NICBR_NS_END
#endif // __ASNRENEWRSP_H__
