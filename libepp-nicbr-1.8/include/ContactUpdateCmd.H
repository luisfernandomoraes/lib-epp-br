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
/* $Id: ContactUpdateCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file ContactUpdateCmd.H
 *  @brief EPP ContactUpdateCmd Class
 */

#ifndef __CONTACT_UPDATE_CMD_H__
#define __CONTACT_UPDATE_CMD_H__

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

/// EPP ContactUpdateCmd Class
class ContactUpdateCmd : public Command
{
public:

  /// Default constructor
  ContactUpdateCmd(bool reset = true) : Command(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Inserts a status to the object
  /**
     @param status_add status to be added to the object
  */
  void insert_status_list_add(const string &status_add) 
  { 
    _status_list_add.insert(status_add); 
  }
  
  /// Removes a status from the object
  /**
     @param status_rem status to be removed to the object
  */
  void insert_status_list_rem(const string &status_rem) 
  { 
    _status_list_rem.insert(status_rem); 
  }

  /// Returns the list of status to be added to the object
  /**
     @return status list to be added to the object
  */
  set<string> get_status_list_add() const { return _status_list_add; }

  /// Returns the list of status to be removed from the object
  /**
     @return status list to be removed to the object
  */
  set<string> get_status_list_rem() const { return _status_list_rem; }

  /// Sets the common data object
  /**
     @param common common data object
  */
  void set_common_data(const CommonData &common) 
  {
    _common = common;
  }

  /// Returns the common data object
  /**
     @return a common data object raw pointer
  */
  CommonData get_common_data() const { return _common; }
  
  /// Resets the data in the object
  void reset() 
  { 
    Command::reset();
    _status_list_add.clear();
    _status_list_rem.clear();
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
  AuthInfo get_authInfo() const { return _authInfo; }

protected:
  /// Status List to be added to the object
  set<string> _status_list_add;

  /// Status list to be removed to the object
  set<string> _status_list_rem;

  /// Common data object
  CommonData _common;

  /// authorization information
  AuthInfo _authInfo;
};

LIBEPP_NICBR_NS_END
#endif //__CONTACT_UPDATE_CMD_H__
