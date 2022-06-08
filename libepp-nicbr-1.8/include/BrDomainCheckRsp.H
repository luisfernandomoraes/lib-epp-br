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
/* $Id: BrDomainCheckRsp.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file BrDomainCheckRsp.H
 *  @brief EPP BrDomainCheckRsp extension Class
 */

#ifndef __BR_DOMAIN_CHECK_RSP_H__
#define __BR_DOMAIN_CHECK_RSP_H__

#include <set>

#include "DomainCheckRsp.H"

using std::set;

LIBEPP_NICBR_NS_BEGIN

/// EPP BrDomainCheckRsp extension Class
class BrDomainCheckRsp : public DomainCheckRsp
{
public:

  struct Unavailability {
    bool hasConcurrent;
    bool inReleaseProcess;
    string equivalentName;
    string organization;
    set<int> tickets;
  };

  /// Default constructor
  BrDomainCheckRsp(bool reset = true) : DomainCheckRsp(false)
  {
    if (reset) {
      this->reset();
    }
  }
  
  /// Copies data from the parent
  void copy_parent_data(const DomainCheckRsp& parent) 
  {
    _availability_list = parent.get_availability_list();	
  }

  /// Inserts unavailability information for a domain name
  /**
     @param domain    domain name
     @param unavail   unavailability information
  */
  void insert_unavailability(const string &domain,
			     struct Unavailability unavail);
  
  /// Returns the unavailable domains list
  /**
     @return unavailable domains list
  */
  map< string, Unavailability, less<string> > get_unavailability_list();

  /// Resets object attributes
  void reset() 
  {
    DomainCheckRsp::reset();
    _unavailability_list.clear();
  }

protected:
  /// Unavailable domains list
  map< string, Unavailability, less<string> > _unavailability_list ;

};

LIBEPP_NICBR_NS_END
#endif //__BR_DOMAIN_CHECK_RSP_H__
