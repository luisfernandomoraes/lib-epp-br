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
/* $Id: AsnCreateRsp.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file AsnCreateRsp.H
 *  @brief EPP AsnCreateRsp Class
 */

#ifndef __ASNCREATERSP_H__
#define __ASNCREATERSP_H__

#include <string>

#include "libepp_nicbr.H"

#include "Response.H"

using std::string;

LIBEPP_NICBR_NS_BEGIN

/// EPP AnsCreateRsp Class
class AsnCreateRsp : public Response
{
public:
  /// Default constructor
  AsnCreateRsp(bool reset = true) : Response(false)
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

  /// Sets repository object ID
  /**
     @param roid   contains the server-unique id for the created asn
  */
  void set_roid(const string &roid)
  {
    _roid = roid;
  }

  /// Returns repository object ID
  /**
     @return server-unique id for the created asn
  */
  string get_roid() const
  {
    return _roid;
  }

  /// Sets creation date
  /**
     @param crDate   object creation date
   */
  void set_crDate(const string& crDate) { _crDate = crDate; }

  /// Returns creation date
  /**
     @return creation date
   */
  string get_crDate() const { return _crDate; }

  /// Sets expiration date
  /**
     @param exDate   date and time identifying the end of the AS 
                     object's registration period
   */
  void set_exDate(const string& exDate) { _exDate = exDate; }

  /// Returns expiration date
  /**
     @return date and time identifying the end of the AS object's 
             registration period
   */
  string get_exDate() const { return _exDate; }

  void reset()
  {
    Response::reset();
    _asn = 0;
    _roid = "";
    _crDate = "";
    _exDate = "";
  }

protected:
  /// autonomous system number
  int _asn;
  
  /// server-unique id for the created IP network
  string _roid;

  /// date and time of object creation
  string _crDate;

  /// date and time identifying the end of the AS object's registration period
  string _exDate;
};

LIBEPP_NICBR_NS_END
#endif // __ASNCREATERSP_H__
