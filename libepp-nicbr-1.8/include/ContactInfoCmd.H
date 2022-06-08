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
/* $Id: ContactInfoCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file ContactInfoCmd.H
 *  @brief EPP ContactInfoCmd Class
 */

#ifndef __CONTACT_INFO_CMD_H__
#define __CONTACT_INFO_CMD_H__

#include <string>

#include "libepp_nicbr.H"

#include "Command.H"

using std::string;

LIBEPP_NICBR_NS_BEGIN

/// EPP ContactInfoCmd Class
class ContactInfoCmd : public Command
{
public:
  /// Default constructor
  ContactInfoCmd(bool reset = true) : Command(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Sets the contact's id
  /**
     @param id contact id
  */
  void set_id(const string &id) { _id = id; }

  /// Returns the contact's id
  /**
     @return contact id
  */
  string get_id() const { return _id; }

  /// reset attributes
  void reset() { 
    Command::reset();
    _id = "";
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
  AuthInfo get_authInfo() const { return _authInfo; }

protected:
  /// Contact id
  string _id;

  /// authorization information
  AuthInfo _authInfo;
};

LIBEPP_NICBR_NS_END
#endif //__CONTACT_INFO_CMD_H__
