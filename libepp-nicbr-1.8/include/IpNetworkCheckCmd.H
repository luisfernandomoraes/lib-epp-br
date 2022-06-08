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
/* $Id: IpNetworkCheckCmd.H 963 2008-12-11 15:31:29Z rafael $ */
/** @file  IpNetworkCheckCmd.H
 *  @brief EPP IpNetworkCheckCmd Class
 */

#ifndef __IPNETWORKCHECKCMD_H__
#define __IPNETWORKCHECKCMD_H__

#include <vector>

#include "libepp_nicbr.H"

#include "Command.H"

using std::vector;

LIBEPP_NICBR_NS_BEGIN

/// EPP IpNetworkCheckCmd Class
class IpNetworkCheckCmd : public Command
{
public:
  /// Default constructor
  IpNetworkCheckCmd(bool reset = true) : Command(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Add an ip range
  /**
     @param ipRange  ip range
  */
  void insert_ipRange(const IpRange &ipRange)
  {
    _ipRanges.push_back(ipRange);
  }

  /// Returns list of ip range
  /**
     @return list of ip ranges
  */
  vector<IpRange> get_ipRanges() const
  {
    return _ipRanges;
  }

  void reset()
  {
    Command::reset();
    _ipRanges.clear();
  }

protected:
  /// list of ip range
  vector<IpRange> _ipRanges;
};

LIBEPP_NICBR_NS_END
#endif // __IPNETWORKCHECKCMD_H__
