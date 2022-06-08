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
/* $Id: IpNetworkTransferRsp.H 958 2008-12-11 14:36:22Z eduardo $ */
/** @file  IpNetworkTransferRsp.H
 *  @brief EPP IpNetworkTransferRsp Class
 */

#ifndef __IPNETWORKTRANSFERRSP_H__
#define __IPNETWORKTRANSFERRSP_H__

#include <string>

#include "libepp_nicbr.H"

#include "Response.H"

using std::string;

LIBEPP_NICBR_NS_BEGIN

/// EPP IpNetworkTransferRsp Class
class IpNetworkTransferRsp : public Response
{
public:
  /// Default constructor
  IpNetworkTransferRsp(bool reset = true) : Response(false)
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

  /// Sets transfer status
  /**
     @param trStatus   state of the most recent transfer request
  */
  void set_trStatus(const string &trStatus)
  {
    _trStatus = trStatus;
  }

  /// Returns transfer status
  /**
     @return state of the most recent transfer request
  */
  string get_trStatus() const
  {
    return _trStatus;
  }
  
  /// Sets request client Id
  /**
     @param reId   identifier of the client that requested the object transfer
  */
  void set_reId(const string &reId)
  {
    _reId = reId;
  }

  /// Returns request client Id
  /**
     @return identifier of the client that requested the object transfer
  */
  string get_reId() const
  {
    return _reId;
  }

  /// Sets request client date
  /**
     @param reDate   date and time that the transfer was requested
  */
  void set_reDate(const string &reDate)
  {
    _reDate = reDate;
  }

  /// Returns request client date
  /**
     @return date and time that the transfer was requested
  */
  string get_reDate() const
  {
    return _reDate;
  }

  /// Sets acknowledge client Id
  /**
     @param acId   identifier of the client that SHOULD act upon the transfer 
                   request
  */
  void set_acId(const string &acId)
  {
    _acId = acId;
  }

  /// Returns acknowledge client Id
  /**
     @return identifier of the client that SHOULD act upon the transfer request
  */
  string get_acId() const
  {
    return _acId;
  }

  /// Sets acknowledge date
  /**
     @param acDate   date and time of a required or completed response
  */
  void set_acDate(const string &acDate)
  {
    _acDate = acDate;
  }

  /// Returns acknowledge date
  /**
     @return date and time of a required or completed response
  */
  string get_acDate() const
  {
    return _acDate;
  }

  void reset()
  {
    Response::reset();
    _roid = "";
    _trStatus = "";
    _reId = "";
    _reDate = "";
    _acId = "";
    _acDate = "";
  }

protected:
  /// server-unique id for the created IP network
  string _roid;

  /// state of the most recent transfer request
  string _trStatus;

  /// identifier of the client that requested the object transfer
  string _reId;

  /// date and time that the transfer was requested
  string _reDate;

  /// identifier of the client that SHOULD act upon the transfer request
  string _acId;

  /// date and time of a required or completed response
  string _acDate;
};

LIBEPP_NICBR_NS_END
#endif // __IPNETWORKTRANSFERRSP_H__
