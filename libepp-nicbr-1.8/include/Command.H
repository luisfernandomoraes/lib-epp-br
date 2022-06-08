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
/* $Id: Command.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file Command.H
 *  @brief EPP Command Class
 */

#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>
#include "libepp_nicbr.H"

using std::string;

LIBEPP_NICBR_NS_BEGIN

/// EPP Command Class
class Command
{
public:
  /// Default constructor
  Command(bool reset = true)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Reset object attributes
  /**
     reset attributes
  */
  void reset()
  {
    _clTRID = "";
  }

  /// virtual destructor
  virtual ~Command() {}

  /// Sets clTRID
  /**
     @param clTRID clTRID attribute
  */
  void set_clTRID(const string &clTRID)
  {
    _clTRID = clTRID;
  }

  /// Returns clTRID
  /**
     @return clTRID attribute
  */
  string get_clTRID() const
  {
    return _clTRID;
  }

protected:
  /// Client Transaction ID
  string _clTRID;

};

LIBEPP_NICBR_NS_END
#endif //__COMMAND_H__
