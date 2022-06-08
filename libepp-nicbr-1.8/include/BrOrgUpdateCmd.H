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
/* $Id: BrOrgUpdateCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file BrOrgUpdateCmd.H
 *  @brief EPP BrOrgUpdateCmd Class
 */

#ifndef __BR_ORG_UPDATE_CMD_H__
#define __BR_ORG_UPDATE_CMD_H__

#include <string>
#include <map>


#include "libepp_nicbr.H"

#include "ContactUpdateCmd.H"

using std::string;
using std::map;

LIBEPP_NICBR_NS_BEGIN

/// EPP BrOrgUpdateCmd Class
class BrOrgUpdateCmd : public ContactUpdateCmd
{
public:
  /// Default constructor
  BrOrgUpdateCmd(bool reset = true) : ContactUpdateCmd(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Sets Organization attribute
  /**
     @param organization   Organization Identifier
  */
  void set_organization(const string &organization)
  {
    _organization = organization;
  }

  /// Returns the Organization
  /**
     @return organization attribute
  */
  string get_organization() const
  {
    return _organization;
  }

  /// Sets Responsible for the organization
  /**
     @param responsible   Person Responsible
  */
  void set_responsible(const string &responsible) 
  {
    _responsible = responsible;
    _responsible_f = true;
  }

  /// Returns Responsible for the organization
  /**
     @return Person Responsible
   */
  string get_responsible() const { return _responsible; };

  /// Sets the responsible change flag
  /**
     @param responsible_f responsible change flag
   */
  void set_responsible_f(const bool responsible_f)
  {
    _responsible_f = responsible_f;
  };
  
  /// Returns the responsible change flag
  /**
     @return The responsible change flag
   */
  bool get_responsible_f() const
  {
    return _responsible_f;
  };

  /// Sets expiration date
  /**
     @param exDate   object expiration date
   */
  void set_exDate(const string& exDate) { _exDate = exDate; }

  /// Returns expiration date
  /**
     @return expiration date
   */
  string get_exDate() const { return _exDate; }

  /// Inserts a Contact into the list of additions
  /**
     @param id Contact Id
     @param type Contact Type
  */
  void insert_contact_add(const string &type, const string &id)
  {
    _contact_list_add[type] = id;
  }

  /// Returns the Contact List of additions
  /**
     @return Contact List
  */
  map< string, string, less<string> > get_contact_list_add() const
  {
    return _contact_list_add;
  }

  /// Inserts a Contact into the list of deletions
  /**
     @param id Contact Id
     @param type Contact Type
  */
  void insert_contact_rem(const string &type, const string &id)
  {
    _contact_list_rem[type] = id;
  }

  /// Returns the Contact List
  /**
     @return Contact List
  */
  map< string, string, less<string> > get_contact_list_rem() const
  {
    return _contact_list_rem;
  }

  /// reset attributes
  void reset()
  {
    ContactUpdateCmd::reset();
    _organization = "";
    _contact_list_add.clear();
    _contact_list_rem.clear();
    _responsible = "";
    _responsible_f = false;
    _exDate = "";
  }
  
protected:
  /// map with organization attribute
  string _organization;

  /// Person responsible for the organization
  string _responsible;

  /// Responsible change flag
  bool _responsible_f;

  /// date and  time identifying the end of  the organization object's
  /// registration period
  string _exDate;

  /// Human Contacts to add
  map<string, string, less<string> > _contact_list_add;

  /// Human Contacts to remove
  map<string, string, less<string> > _contact_list_rem;
};

LIBEPP_NICBR_NS_END
#endif //__BR_ORG_UPDATE_CMD_H__
