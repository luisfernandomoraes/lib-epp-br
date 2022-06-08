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
/* $Id: ContactCreateCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file ContactCreateCmd.H
 *  @brief EPP ContactCreateCmd Class
 */

#ifndef __CONTACT_CREATE_CMD_H__
#define __CONTACT_CREATE_CMD_H__

#include <string>
#include <set>
#include <memory>

#include "libepp_nicbr.H"

#include "Command.H"
#include "CommonData.H"

using std::string;
using std::set;
using std::auto_ptr;

LIBEPP_NICBR_NS_BEGIN

/// EPP ContactCreateCmd Class
class ContactCreateCmd : public Command
{
public:

  /// Default constructor
  ContactCreateCmd(bool reset = true) : Command(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Sets the common data object
  /**
     @param common common data object
  */
  void set_common_data(const CommonData &common) 
  {
    _common = common;
  }

  /// Returns a common data object raw pointer
  /**
     @return a common data object raw pointer
  */
  CommonData get_common_data() const { return _common; }

  /// Resets object attributes
  void reset()
  {
    Command::reset();
    _common.reset();
    _authInfo.reset();
  }

  /// Sets authorization information
  /**
     @param authInfo   domain authorization information
   */
  void set_authInfo(const AuthInfo &authInfo)
  {
    _authInfo = authInfo;
  }

  /// Returns authorization information
  /**
     @return authorization information
   */
  AuthInfo get_authInfo() { return _authInfo; }

protected:
  /// Common data object
  CommonData _common;

  /// authorization information
  AuthInfo _authInfo;
};

LIBEPP_NICBR_NS_END
#endif //__CONTACT_CREATE_CMD_H__
