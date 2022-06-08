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
/* $Id: AsnInfoRsp.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file AsnInfoRsp.H
 *  @brief EPP AsnInfoRsp Class
 */

#ifndef __ASNINFORSP_H__
#define __ASNINFORSP_H__

#include "libepp_nicbr.H"

#include "Response.H"

LIBEPP_NICBR_NS_BEGIN

/// EPP AnsInfoRsp Class
class AsnInfoRsp : public Response
{
public:
  /// Default constructor
  AsnInfoRsp(bool reset = true) : Response(false)
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

  /// Sets repository object ID
  /**
     @param roid   Repository Object IDentifier assigned to the AS 
                   object when the it was created
  */
  void set_roid(const string &roid)
  {
    _roid = roid;
  }

  /// Returns repository object ID
  /**
     @return Repository Object IDentifier assigned to the AS 
             object when the it was created
  */
  string get_roid() const
  {
    return _roid;
  }

  /// Sets organization associated with the AS
  /**
     @param organization   identifier for the organization object associated 
                           with the AS object as the AS manager
  */
  void set_organization(const string &organization)
  {
    _organization = organization;
  }
  
  /// Returns organization associated with the AS
  /**
     @return identifier for the organization object associated 
             with the AS object as the AS manager
  */
  string get_organization() const
  {
    return _organization;
  }
  
  /// Inserts a contact in the map of other contacts
  /**
     @param type             contact type
     @param identification   contact identification
  */
  void insert_contact(const string &type, const string &identification)
  {
    _contacts[type] = identification;
  }
  
  /// Returns map of other contacts
  /**
     @return map of other contacts
  */
  map< string, string, less<string> > get_contacts() { return _contacts; }

  /// Sets sponsoring client
  /**
     @param clId   identifier of the client that created the AS object
  */
  void set_clId(const string &clId)
  {
    _clId = clId;
  }
  
  /// Returns sponsoring client
  /**
     @return identifier of the client that created the AS object
  */
  string get_clId() const
  {
    return _clId;
  }
  
  /// Sets client that created the AS
  /**
     @param crId   identifier of the client that created the AS object
  */
  void set_crId(const string &crId)
  {
    _crId = crId;
  }
  
  /// Returns client that created the AS
  /**
     @return identifier of the client that created the AS object
  */
  string get_crId() const
  {
    return _crId;
  }

  /// Sets date of the AS creation
  /**
     @param crDate   date and time of the AS object creation
  */
  void set_crDate(const string& crDate)
  {
    _crDate = crDate;
  }
  
  /// Returns date of the AS creation
  /**
     @return date and time of the AS object creation
  */
  string get_crDate() const
  {
    return _crDate;
  }
  
  /// Sets AS's expiration date
  /**
     @param exDate   date and time identifying the end of the AS 
                     object registration period
  */
  void set_exDate(const string &exDate)
  {
    _exDate = exDate;
  }
  
  /// Returns AS's expiration date
  /**
     @return date and time identifying the end of the AS object 
             registration period
  */
  string get_exDate() const
  {
    return _exDate;
  }
  
  /// Sets last client that updated the AS
  /**
     @param upId   identifier of the client that last updated the AS object
  */
  void set_upId(const string &upId)
  {
    _upId = upId;
  }
  
  /// Returns last client that updated  the AS
  /**
     @return identifier of the client that last updated the AS object
  */
  string get_upId() const
  {
    return _upId;
  }
  
  /// Sets date of the last update
  /**
     @param upDate   date and time of the most recent AS object modification
  */
    void set_upDate(const string &upDate)
  {
    _upDate = upDate;
  }
  
  /// Returns date of the last update
  /**
     @return date and time of the most recent AS object modification
  */
  string get_upDate() const
  {
    return _upDate;
  }
  
  /// Sets most recent successful AS object transfer
  /**
     @param trDate   most recent successful AS object transfer
  */
  void set_trDate(const string &trDate)
  {
    _trDate = trDate;
  }
  
  /// Returns most recent successful AS object transfer
  /**
     @return most recent successful AS object transfer
  */
  string get_trDate() const
  {
    return _trDate;
  }

  void reset()
  {
    Response::reset();
    _asn = 0;
    _roid = "";
    _organization = "";
    _contacts.clear();
    _clId = "";
    _crId = "";
    _crDate = "";
    _exDate = "";
    _upId = "";
    _upDate = "";
    _trDate = "";
  }

protected:
  /// Autonomous System Number
  int _asn;

  /// Repository Object IDentifier assigned to the AS object when the 
  /// it was created
  string _roid;

  /// identifier for the organization object associated with the AS object 
  /// as the AS manager
  string _organization;
  
  /// identifiers for contact objects associated with the AS object
  map< string, string, less<string> > _contacts;
  
  /// identifier of the sponsoring client
  string _clId;
  
  /// identifier of the client that created the AS object
  string _crId;
  
  /// date and time of the AS object creation
  string _crDate;

  /// date and time identifying the end of the AS object registration period
  string _exDate;
  
  /// identifier of the client that last updated the AS object
  string _upId;
  
  /// date and time of the most recent AS object modification
  string _upDate;
  
  /// most recent successful AS object transfer
  string _trDate;
};

LIBEPP_NICBR_NS_END
#endif // __ASNINFORSP_H__
