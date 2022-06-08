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
/* $Id: BrOrgPanDataRsp.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file BrOrgPanDataRsp.H
 *  @brief EPP BrOrgPanDataRsp Extesion Class to be used with PollRsp
 */

#ifndef __BR_ORG_PANDATA_RSP_H__
#define __BR_ORG_PANDATA_RSP_H__

#include <string>

#include "libepp_nicbr.H"
#include "PanDataRsp.H"

LIBEPP_NICBR_NS_BEGIN

using std::string;

/// EPP BrOrgPanDataRsp Extension Class to be used with PollRsp
class BrOrgPanDataRsp : public PanDataRsp
{
public:  
  /// Constructor
  BrOrgPanDataRsp(bool reset = true) : PanDataRsp(false) 
  {
    if (reset) {
      this->reset();
    }
  }

  /// Copies data from the parent
  void copy_parent_data(const PanDataRsp &parent) 
  {
    _clTRID = parent.get_clTRID();
    _svTRID = parent.get_svTRID();
    _object_id = parent.get_object_id();    
    _paResult = parent.get_paResult();
    _paDate = parent.get_paDate();
  }
  
  /// Sets the organization
  /**
     @param organization Organization Identifier
  */
  void set_organization(const string &organization)
  {
    _organization = organization;
  }

  /// Sets the reason
  /**
     @param reason Reason why the pending action was approved or denied
  */
  void set_reason(const string &reason)
  {
    _reason = reason;
  }

  /// Sets the reason language
  /**
     @param reason_lang Reason Language
  */
  void set_reason_lang(const string &reason_lang)
  {
    _reason_lang = reason_lang;
  }

  /// Returns the organization
  /**
     @return Organization Identifier
  */
  string get_organization() const
  {
    return _organization;
  }

  /// Returns the reason
  /**
     @return Reason why the pending action was approved or denied
  */
  string get_reason() const
  {
    return _reason;
  }

  /// Returns the reason language
  /**
     @return Reason Language
  */
  string get_reason_lang() const
  {
    return _reason_lang;
  }
  
  /// reset attributes
  void reset() 
  {
    PanDataRsp::reset();
    _organization = "";
    _reason = "";
    _reason_lang = "en";
  }
  
  
protected:
  /// organization identifier
  string _organization;

  /// reason for the denial or approval of the pending action
  string _reason;

  /// language of reason
  string _reason_lang;
};

LIBEPP_NICBR_NS_END

#endif //__BR_ORG_PANDATA_RSP_H__
