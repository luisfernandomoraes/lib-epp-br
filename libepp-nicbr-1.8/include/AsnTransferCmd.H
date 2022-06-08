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
/* $Id: AsnTransferCmd.H 948 2008-12-09 20:26:54Z rafael $ */
/** @file AsnTransferCmd.H
 *  @brief EPP AsnTransferCmd Class
 */

#ifndef __ASNTRANSFERCMD_H__
#define __ASNTRANSFERCMD_H__

#include "libepp_nicbr.H"

#include "Command.H"

LIBEPP_NICBR_NS_BEGIN

/// EPP AsnTransferCmd Class
class AsnTransferCmd : public Command
{
public:
  /// Default constructor
  AsnTransferCmd(bool reset = true) : Command(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Sets operation
  /**
     @param op   identifies the transfer operation to be performed
  */
  void set_operation(const string &op)
  {
    _operation = op;
  }

  /// Returns operation
  /**
     @return transfer operation to be performed
  */
  string get_operation() const
  {
    return _operation;
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

  void reset()
  {
    Command::reset();
    _operation = "";
    _asn = 0;
  }

protected:
  /// identifies the transfer operation to be performed
  string _operation;

  /// autonomous system number
  int _asn;
};

LIBEPP_NICBR_NS_END
#endif // __ASNTRANSFERCMD_H__
