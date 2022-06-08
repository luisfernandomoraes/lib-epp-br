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
/* $Id: IpNetworkCheckRsp.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file  IpNetworkCheckRsp.H
 *  @brief EPP IpNetworkCheckRsp Class
 */

#ifndef __IPNETWORKCHECKRSP_H__
#define __IPNETWORKCHECKRSP_H__

#include <vector>
#include <string>

#include "libepp_nicbr.H"

#include "Response.H"

using std::vector;
using std::string;

LIBEPP_NICBR_NS_BEGIN

class IpNetworkCheckRsp : public Response
{
public:
  struct CheckData {
    IpRange ipRange;
    string reason;
    string reason_lang;
  };

  /// Default constructor
  IpNetworkCheckRsp(bool reset = true) : Response(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Inserts availability information for a ip range to the list
  /**
     @param ipNetwork     ip range
     @param reason      reason of unavailability
     @param reason_lang language used in reason
  */
  void insert_availability(const IpRange &ipRange, 
			   const string &reason, 
			   const string &reason_lang = "")
  {
    CheckData cd;
    cd.ipRange = ipRange;
    cd.reason = reason;
    cd.reason_lang = reason_lang;
    _availability_list.push_back(cd);
  }

  /// Returns the ip range availability list
  /**
     @return ip range availability list
  */
  vector<CheckData> get_availability_list() const
  {
    return _availability_list;
  }
  
  void reset()
  {
    Response::reset();
    _availability_list.clear();
  }

private:
  /// Ip range response list
  vector<CheckData> _availability_list;
};

LIBEPP_NICBR_NS_END
#endif // __IPNETWORKCHECKRSP_H__
