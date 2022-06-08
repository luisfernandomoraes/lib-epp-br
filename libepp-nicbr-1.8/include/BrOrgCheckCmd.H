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
/* $Id: BrOrgCheckCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file BrOrgCheckCmd.H
 *  @brief EPP BrOrgCheckCmd Class
 */

#ifndef __BR_ORG_CHECK_CMD_H__
#define __BR_ORG_CHECK_CMD_H__

#include <string>
#include <map>


#include "libepp_nicbr.H"

#include "ContactCheckCmd.H"

using std::string;
using std::map;

LIBEPP_NICBR_NS_BEGIN

/// EPP BrOrgCheckCmd Class
class BrOrgCheckCmd : public ContactCheckCmd
{
public:
  /// Default constructor
  BrOrgCheckCmd(bool reset = true) : ContactCheckCmd(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Inserts an Organization attribute
  /**
     @param id Object Id
     @param organization Organization Identifier
  */
  void insert_organization(const string &id, const string &organization)
  {
    _organization_list[id] = organization;
  }

  /// Returns the Organization List
  /**
     @return Id->Organization list
  */
  map< string, string, less<string> > get_organization_list() const
  {
    return _organization_list;
  }
  
  /// reset attributes
  void reset()
  {
    ContactCheckCmd::reset();
    _organization_list.clear();
  }
  
protected:
  /// map with organization attribute
  map< string, string, less<string> > _organization_list;
  
};

LIBEPP_NICBR_NS_END
#endif //__BR_ORG_CHECK_CMD_H__
