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
/* $Id: AsnCheckCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file AsnCheckCmd.H
 *  @brief EPP AsnCheckCmd Class
 */

#ifndef __ASNCHECKCMD_H__
#define __ASNCHECKCMD_H__

#include "libepp_nicbr.H"

#include "Command.H"

LIBEPP_NICBR_NS_BEGIN

/// EPP AsnCheck Class
class AsnCheckCmd : public Command
{
public:
  /// Default constructor
  AsnCheckCmd(bool reset = true) : Command(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Sets asn
  /**
     @param asn   autonomous system number
  */
  void add_asn(const int asn)
  {
    _asn_list.push_back(asn);
  }

  /// Returns asn
  /**
     @return autonomous system number
  */
  list<int> get_asn_list() const
  {
    return _asn_list;
  }

  void reset()
  {
    Command::reset();
    _asn_list.clear();
  }

protected:
  /// autonomous system number
  list<int> _asn_list;
};

LIBEPP_NICBR_NS_END
#endif // __ASNCHECKCMD_H__
