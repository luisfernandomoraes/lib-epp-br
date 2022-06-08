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
/* $Id: ContactCheckRsp.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file ContactCheckRsp.H
 *  @brief EPP ContactCheckRsp Class
 */

#ifndef __CONTACT_CHECK_RSP_H__
#define __CONTACT_CHECK_RSP_H__

#include <string>
#include <map>

#include "libepp_nicbr.H"

#include "Response.H"

using std::string;
using std::map;
using std::less;

LIBEPP_NICBR_NS_BEGIN

/// EPP ContactCheckRsp Class
class ContactCheckRsp : public Response
{
public:
  
  struct Availability {
    string available;
    string reason;
  };
  
  /// Default constructor
  ContactCheckRsp(bool reset = true) : Response(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Inserts availability information for a contact to the list
  /**
     @param id          contact id
     @param available   contact availability ["0" | "1"]
     @param reason      reason of unavailability
  */
  void insert_availability(const string &id, 
			   const string &available,
			   const string &reason);
  
  /// Returns the contact's availability list 
  /**
     @return contact availability list
  */
  map< string, Availability, less<string> > get_availability_list() const;

  /// reset attributes
  void reset() 
  { 
    Response::reset();
    _availability_list.clear();
  }
  
protected:
  /// Contact availability list
  map< string, Availability, less<string> > _availability_list;
  
};

LIBEPP_NICBR_NS_END
#endif //__CONTACT_CHECK_RSP_H__
