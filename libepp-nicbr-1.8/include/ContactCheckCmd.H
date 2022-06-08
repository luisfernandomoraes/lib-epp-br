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
/* $Id: ContactCheckCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file ContactCheckCmd.H
 *  @brief EPP ContactCheckCmd Class
 */

#ifndef __CONTACT_CHECK_CMD_H__
#define __CONTACT_CHECK_CMD_H__

#include <string>
#include <set>

#include "libepp_nicbr.H"

#include "Command.H"

using std::string;
using std::set;

LIBEPP_NICBR_NS_BEGIN

/// EPP ContactCheckCmd Class
class ContactCheckCmd : public Command
{
public:
  /// Default constructor
  ContactCheckCmd(bool reset = true) : Command(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Inserts an id into id list
  /**
     @param id   Contact id list
  */
  void insert_id(const string &id) { _id_list.insert(id); } 
  
  /// Returns contact id list
  /**
     @return contact id list
  */
  set<string> get_id_list() const { return _id_list; }
  
  /// reset attributes
  void reset()
  {
    Command::reset();
    _id_list.clear();
  }
  
protected:
  /// Contact id list
  set<string> _id_list;
};

LIBEPP_NICBR_NS_END
#endif //__CONTACT_CHECK_CMD_H__
