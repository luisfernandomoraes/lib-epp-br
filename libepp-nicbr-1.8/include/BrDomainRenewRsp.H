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
/* $Id: BrDomainRenewRsp.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file BrDomainRenewRsp.H
 *  @brief EPP BrDomainRenewRsp extension Class
 */

#ifndef __BR_DOMAIN_RENEW_RSP_H__
#define __BR_DOMAIN_RENEW_RSP_H__

#include "BrDomainCommon.H"
#include "DomainRenewRsp.H"

LIBEPP_NICBR_NS_BEGIN

/// EPP BrDomainRenewRsp extension Class
class BrDomainRenewRsp : public DomainRenewRsp
{
public:

  /// Default constructor
  BrDomainRenewRsp(bool reset = true) : DomainRenewRsp(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Copies data from the parent
  void copy_parent_data(const DomainRenewRsp &parent) 
  {
    _name = parent.get_name();    
    _exDate = parent.get_exDate();
  }
   
  /// Sets publication flag attribute
  /**
     @param publication flag attribute
  */
  void set_publication_flag(const string& publicationFlag)
  {
    _publicationFlag = publicationFlag;
  }

  /// Returns publication flag attribute
  /**
     @return publication flag attribute
   */
  string get_publication_flag() const
  {
    return _publicationFlag;
  }

  /// Inserts a onHold Reason
  /**
     @param reason onHold Reason
  */
  
  void insert_onhold_reason(const string& reason)
  {
    _onhold_reasons.insert(reason);
  }

  /// Returns the onHold Reasons
  /**
     @return onHold Reasons
  */
  set<string> get_onhold_reasons() const
  {
    return _onhold_reasons;
  }
  
  /// Resets object attributes
  void reset()
  {
    DomainRenewRsp::reset();
    _publicationFlag = "";
    _onhold_reasons.clear();
  }

protected:
  
  /// publicationFlag
  string _publicationFlag;
  
  /// onHoldReason
  set<string> _onhold_reasons;
};

LIBEPP_NICBR_NS_END
#endif //__BR_DOMAIN_RENEW_RSP_H__
