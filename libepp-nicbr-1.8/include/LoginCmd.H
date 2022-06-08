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
/* $Id: LoginCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file LoginCmd.H
 *  @brief EPP LoginCmd Class
 */

#ifndef __LOGINCMD_H__
#define __LOGINCMD_H__

#include "libepp_nicbr.H"

#include "Command.H"

LIBEPP_NICBR_NS_BEGIN

/// EPP LoginCmd Class
class LoginCmd : public Command
{
public:
  /// Default constructor
  LoginCmd(bool reset = true) : Command(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Sets Client ID
  /**
     @param clID   Client ID
  */
  void set_clID(const string &clID) { _clID = clID; }

  /// Sets password
  /**
     @param pw   Password
  */
  void set_pw(const string &pw) { _pw = pw; }

  /// Sets new password
  /**
     @param new_pw   New password
  */
  void set_new_pw(const string &new_pw) { _new_pw = new_pw; }

  /// Sets protocol version
  /**
     @param version  Protocol version
   */
  void set_version(const string &version) { _version = version; }

  /// Sets text language
  /**
     @param lang Text language
   */
  void set_lang(const string &lang) { _lang = lang; }

  /// Returns Client ID
  /**
     @return Client ID
  */
  string get_clID() const { return _clID; }

  /// Returns password
  /**
     @return Password
  */
  string get_pw() const { return _pw; }

  /// Returns new password
  /**
     @return New password
  */
  string get_new_pw() const { return _new_pw; }

  /// Returns protocol version
  /**
     @return Protocol version
  */
  string get_version() const { return _version; }

  /// Returns text language
  /**
     @return Text language
   */
  string get_lang() const { return _lang; }

  /// Reset attributes
  /**
    reset attributes
  */
  void reset() 
  {
    Command::reset();
    _clID = "";
    _pw = "";
    _new_pw = "";
    _version = "";
    _lang = "en";
  }
  
protected:
  /// Client ID
  string _clID;

  /// Password
  string _pw;

  /// New Password (optional)
  string _new_pw;

  /// Protocol version
  string _version;

  /// Text language
  string _lang;
};

LIBEPP_NICBR_NS_END
#endif //__LOGINCMD_H__
