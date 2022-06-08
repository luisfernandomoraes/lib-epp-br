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
/* $Id: IpNetworkRenewRsp.H 958 2008-12-11 14:36:22Z eduardo $ */
/** @file IpNetworkRenewRsp.H
 *  @brief EPP IpNetworkRenewRsp Class
 */

#ifndef __IPNETWORKRENEWRSP_H__
#define __IPNETWORKRENEWRSP_H__

#include "libepp_nicbr.H"

#include "Response.H"

LIBEPP_NICBR_NS_BEGIN

/// EPP IpNetworkRenewRsp Class
class IpNetworkRenewRsp : public Response
{
public:
  /// Default constructor
  IpNetworkRenewRsp(bool reset = true) : Response(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Sets repository object ID
  /**
     @param roid   contains the server-unique id for the created IP network
  */
  void set_roid(const string &roid)
  {
    _roid = roid;
  }

  /// Returns repository object ID
  /**
     @return server-unique id for the created IP network
  */
  string get_roid() const
  {
    return _roid;
  }

  /// Sets expiration date
  /**
     @param expDate   date and  time identifying  the  end of  the IP  network
                      object's registration period
  */
  void set_expDate(const string &expDate)
  {
    _expDate = expDate;
  }

  /// Returns expiration date
  /**
     @return  date and  time identifying  the  end of  the IP  network
              object's registration period
  */
  string get_expDate() const
  {
    return _expDate;
  }

  void reset()
  {
    Response::reset();
    _roid = "";
    _expDate = "";
  }

protected:
  /// element that contains the server-unique identifier
  string _roid;

  /// date and  time identifying  the end of  the IP  network object's
  /// registration period
  string _expDate;
};

LIBEPP_NICBR_NS_END
#endif // __IPNETWORKRENEWRSP_H__
