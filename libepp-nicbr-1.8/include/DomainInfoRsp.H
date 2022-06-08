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
/* $Id: DomainInfoRsp.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file  DomainInfoRsp.H
 *  @brief EPP DomainInfoRsp Class
 */

#ifndef __DOMAIN_INFO_RSP_H__
#define __DOMAIN_INFO_RSP_H__

#include <string>
#include <vector>
#include <map>

#include "libepp_nicbr.H"

#include "Response.H"
#include "CommonData.H" // struct NameServer; AuthInfo
#include "DSInfo.H"

using std::string;
using std::set;
using std::map;
using std::less;

LIBEPP_NICBR_NS_BEGIN

/// EPP DomainInfoRsp Class
class DomainInfoRsp : public Response
{
public:

  /// Default constructor
  DomainInfoRsp(bool reset = true) : Response(false)
  {
    if (reset) {
      this->reset();
    }
  }
  
  /// Sets domain name
  /**
     @param name   fully qualified domain name
   */
  void set_name(const string& name) { _name = name; }

  /// Returns domain name
  /**
     @return fully qualified domain name
  */
  string get_name() const { return _name; }

  /// Sets domain repository object identification
  /**
     @param roid   domain repository object identification
   */
  void set_roid(const string& roid) { _roid = roid; }

  /// Returns domain repository object identification
  /**
     @return domain repository object identification
  */
  string get_roid() const { return _roid; }

  /// Inserts a new status in _status_set
  /**
     @param status   the status to be inserted
   */
  void insert_status(const string& status) { _status_set.insert(status); }

  /// Returns set of domain status
  /**
     @return set of domain status
   */
  set<string> get_status_set() const { return _status_set; }

  /// Sets registrant
  /**
     @param registrant   registrant identification
   */
  void set_registrant(const string& registrant) { _registrant = registrant; }

  /// Returns registrant
  /**
     @return registrant identification
  */
  string get_registrant() const { return _registrant; }

  /// Inserts a contact in the map of other contacts
  /**
     @param type             contact type
     @param identification   contact identification
   */
  void insert_contact(const string& type, const string& identification)
  {
    _contacts[type] = identification;
  }

  /// Returns map of other contacts
  /**
     @return map of other contacts
   */
  map< string, string, less<string> > get_contacts() const { return _contacts; }

  /// Inserts a nameserver to the list of nameservers
  /**
     @param nameserver   fully qualified domain name
   */
  void insert_nameserver(const struct NameServer &nameserver)
  {
    _nameservers.push_back(nameserver);
  }

  /// Returns a list of nameservers
  /**
     @return list of nameservers associated with domain object
   */
  vector<struct NameServer> get_nameservers() const { return _nameservers; }

  /// Sets the sponsoring client
  /**
     @param clID   sponsoring client
   */
  void set_clID(const string& clID)
  {
    _clID = clID;
  }

  /// Returns sponsoring client
  /**
     @return sponsoring client
   */
  string get_clID() const { return _clID; }

  /// Sets the identifier of the client that created the domain object
  /**
     @param crID   id of the client that created the object
   */
  void set_crID(const string& crID) { _crID = crID; }

  /// Returns the identifier of the client that created the domain object
  /**
     @return client identification
   */
  string get_crID() const { return _crID; }

  /// Sets creation date
  /**
     @param crDate   object creation date
   */
  void set_crDate(const string& crDate) { _crDate = crDate; }

  /// Returns creation date
  /**
     @return creation date
   */
  string get_crDate() const { return _crDate; }


  /// Sets the identifier of the client that last updated the domain object
  /**
     @param upID   id of the client that created the object
   */
  void set_upID(const string& upID) { _upID = upID; }

  /// Returns the identifier of the client that last updated the domain object
  /**
     @return id of client that last updated the domain object
   */
  string get_upID() const { return _upID; }

  /// Sets expiration date
  /**
     @param exDate   expiration date
   */
  void set_exDate(const string& exDate) { _exDate = exDate; }

  /// Returns expiration date
  /**
     @return expiration date
   */
  string get_exDate() const { return _exDate; }

  /// Sets last modification date
  /**
     @param upDate   last modification date
   */
  void set_upDate(const string& upDate) { _upDate = upDate; }

  /// Returns last modification date
  /**
     @return last modification date
   */
  string get_upDate() const { return _upDate; }

  /// Sets last successfull transfer date
  /**
     @param trDate   last successfull transfer date
   */
  void set_trDate(const string& trDate) { _trDate = trDate; }

  /// Returns last successfull transfer date
  /**
     @return last successfull transfer date
   */
  string get_trDate() const { return _trDate; }

  /// Sets authorization information
  /**
     @param authInfo   domain authorization information
   */
  void set_authInfo(const AuthInfo &authInfo)
  {
    _authInfo = authInfo;
  }

  /// Returns authorization information
  /**
     @return authorization information
   */
  AuthInfo get_authInfo() const { return _authInfo; }

  //******************** RFC 4310 BEGIN ********************
  /// Adds a DSInfo object to the list
  /**
     @param DSInfo object
  */
  void add_dsInfo(const DSInfo &dsInfo)
  {
    _dsInfo.push_back(dsInfo);
  }
  
  /// Returns list of DSInfo objects
  /**
     @return list of DSInfo objects
  */
  list<DSInfo> get_dsInfo() const { return _dsInfo; }
  //******************** RFC 4310 END ********************

  /// Reset object attributes
  void reset()
  {
    Response::reset();
    _name = "";
    _roid = "";
    _status_set.clear();
    _registrant = "";
    _contacts.clear();
    _nameservers.clear();
    _clID = "";
    _crID = "";
    _crDate = "";
    _upID = "";
    _exDate = "";
    _upDate = "";
    _trDate = "";
    _authInfo.reset();
    _dsInfo.clear();
  }
  
protected:
  /// fully qualified domain name
  string _name;

  /// element that contains the Repository Object IDentifier assigned
  /// to the domain object when the object was created
  string _roid;

  /// set of domain status
  set<string> _status_set;

  /// registrant
  string _registrant;

  /// other contact objects
  map< string, string, less<string> > _contacts;

  /// name servers associated with domain object
  vector<struct NameServer> _nameservers;

  /// sponsoring client id
  string _clID;

  /// client that created object
  string _crID;

  /// creation date
  string _crDate;

  /// client that last updated object
  string _upID;

  /// expiration date
  string _exDate;

  /// last modification date
  string _upDate;

  /// last successfull transfer date
  string _trDate;

  /// authorization information
  AuthInfo _authInfo;

  /// DS info
  list<DSInfo> _dsInfo;
};

LIBEPP_NICBR_NS_END
#endif //__DOMAIN_INFO_RSP_H__
