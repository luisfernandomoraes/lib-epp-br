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
/* $Id: AsnUpdateCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file AsnUpdateCmd.H
 *  @brief EPP AsnUpdateCmd Class
 */

#ifndef __ASNUPDATECMD_H__
#define __ASNUPDATECMD_H__

#include <map>
#include <string>

#include "libepp_nicbr.H"

#include "Command.H"

using std::map;
using std::string;

LIBEPP_NICBR_NS_BEGIN

/// EPP AsnUpdate Class
class AsnUpdateCmd : public Command
{
public:
  /// Default constructor
  AsnUpdateCmd(bool reset = true) : Command(false)
  {
    if (reset) {
      this->reset();
    }
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

  /// Sets organization
  /**
     @param organization   organization to be associated with the asn
  */
  void set_organization(const string &organization)
  {
    _organization = organization;
  }

  /// Returns organization associated with the asn
  /**
     @return organization owner of the asn
  */
  string get_organization() const
  {
    return _organization;
  }

  /// Inserts a contact to be added
  /**
     @param type             contact type
     @param identification   contact identification
  */
  void insert_contact_add(const string &type, const string &identification)
  {
    _contacts_add[type] = identification;
  }

  /// Returns map of other contacts to be added
  /**
     @return map of other contacts
  */
  map< string, string, less<string> > get_contacts_add() const
  {
    return _contacts_add;
  }

  /// Inserts a contact to be removed
  /**
     @param type             contact type
     @param identification   contact identification
  */
  void insert_contact_rem(const string &type, const string &identification)
  {
    _contacts_rem[type] = identification;
  }

  /// Returns map of other contacts to be removed
  /**
     @return map of other contacts
  */
  map< string, string, less<string> > get_contacts_rem() const
  {
    return _contacts_rem;
  }

  void reset()
  {
    Command::reset();
    _asn = 0;
    _organization = "";
    _contacts_add.clear();
    _contacts_rem.clear();
  }

protected:
  /// autonomous system number
  int _asn;

  /// organization associated with the asn
  string _organization;

  /// contacts to be added
  map< string, string, less<string> > _contacts_add;

  /// contacts to be removed
  map< string, string, less<string> > _contacts_rem;
};

LIBEPP_NICBR_NS_END
#endif // __ASNUPDATECMD_H__
