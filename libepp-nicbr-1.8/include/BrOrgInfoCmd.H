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
/* $Id: BrOrgInfoCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file BrOrgInfoCmd.H
 *  @brief EPP BrOrgInfoCmd Class
 */

#ifndef __BR_ORG_INFO_CMD_H__
#define __BR_ORG_INFO_CMD_H__

#include <string>

#include "libepp_nicbr.H"

#include "ContactInfoCmd.H"

using std::string;

LIBEPP_NICBR_NS_BEGIN

/// EPP BrOrgInfoCmd Class
class BrOrgInfoCmd : public ContactInfoCmd
{
public:
  /// Default constructor
  BrOrgInfoCmd(bool reset = true) : ContactInfoCmd(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Sets the Organization
  /**
     @param organization Organization Identifier
  */
  void set_organization(const string &organization)
  {
    _organization = organization;
  }

  /// Returns the Organization
  /**
     @return Organization Identifier
  */
  string get_organization() const
  {
    return _organization;
  }
  
  /// reset attributes
  void reset() 
  { 
    ContactInfoCmd::reset();
    _organization = "";
  }
  
protected:
  /// Organization Identifier
  string _organization;
};

LIBEPP_NICBR_NS_END
#endif //__BR_ORG_INFO_CMD_H__
