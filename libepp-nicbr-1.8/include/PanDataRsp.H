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
/* $Id: PanDataRsp.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file PanDataRsp.H
 *  @brief EPP PanDataRsp Class to be used with PollRsp
 */

#ifndef __PANDATA_RSP_H__
#define __PANDATA_RSP_H__

#include <string>

#include "libepp_nicbr.H"

LIBEPP_NICBR_NS_BEGIN

using std::string;

/// EPP PanDataRsp Class to be used with PollRsp
class PanDataRsp : public Response
{
public:  
  /// Constructor
  PanDataRsp(bool reset = true) : Response(false) 
  {
    if (reset) {
      this->reset();
    }
  }

  /// Sets the object id
  /**
     @param object_id object id
  */
  void set_object_id(const string &object_id) 
  {
    _object_id = object_id;
  }

  /// Returns the object id
  /**
     @return object id
  */
  string get_object_id() const
  {
    return _object_id;
  }

  /// Sets the Pending Action Result
  /**
     @param paResult Pending Action Result
  */
  void set_paResult(const bool paResult)
  {
    _paResult = paResult;
  }

  /// Returns the Pending Action Result
  /**
     @return Pending Action Result
  */
  bool get_paResult() const
  {
    return _paResult;
  }  
  
  /// Sets the paDate
  /**
     @param paDate Pending Action Review Date
  */
  void set_paDate(const string &paDate)
  {
    _paDate = paDate;
  }

  /// Returns the paDate
  /**
     @return Pending Action Review Date
  */
  string get_paDate() const
  {
    return _paDate;
  }
  
  /// reset attributes
  void reset() 
  {
    Response::reset();
    _object_id = "";
    _paResult = false;
    _paDate = "";
  }
  
  
protected:
  /// object id: either Domain Name or Contact Id
  string _object_id;
  
  /// Pending Action Result
  bool _paResult;
  
  /// Pending Action Review Date
  string _paDate;
  
 
};

LIBEPP_NICBR_NS_END
#endif //__PANDATA_RSP_H__
