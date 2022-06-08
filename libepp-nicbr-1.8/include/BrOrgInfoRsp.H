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
/* $Id: BrOrgInfoRsp.H 997 2009-01-22 12:35:13Z rafael $ */
/** @file BrOrgInfoRsp.H
 *  @brief EPP BrOrgInfoRsp Class
 */

#ifndef __BR_ORG_INFO_RSP_H__
#define __BR_ORG_INFO_RSP_H__

#include <string>
#include <map>
#include <set>
#include <memory>

#include "libepp_nicbr.H"

#include "ContactInfoRsp.H"

using std::string;
using std::map;
using std::set;
using std::auto_ptr;

LIBEPP_NICBR_NS_BEGIN

/// EPP BrOrgInfoRsp Class
class BrOrgInfoRsp : public ContactInfoRsp
{
public:
  /// Default constructor
  BrOrgInfoRsp(bool reset = true) : ContactInfoRsp(false)
  {  
    if (reset) {
      this->reset();
    }
  } 

  /// Copies data from the parent
  void copy_parent_data(ContactInfoRsp &parent) 
  {
    _roid = parent.get_roid();
    _status_set = parent.get_status_set();
    _clID = parent.get_clID();
    _crID = parent.get_crID();
    _crDate = parent.get_crDate();
    _upID = parent.get_upID();
    _upDate = parent.get_upDate();
    _trDate = parent.get_trDate();
    _common = parent.get_common_data();
    _authInfo = parent.get_authInfo();
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
  string get_organization() 
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
  }
  
  /// Returns Responsible for the organization
  /**
     @return Person Responsible
  */
  string get_responsible() { return _responsible; };

  /// Inserts a Contact into the list
  /**
     @param id Contact Id
     @param type Contact Type
  */
  void insert_contact(const string &type, const string &id)
  {
    _contact_list[type] = id;
  }

  /// Returns the Contact List
  /**
     @return Contact List
  */
  map< string, string, less<string> > get_contact_list() 
  {
    return _contact_list;
  }

  /// Sets the Legal Representative
  /**
     @param proxy Local Legal Representative
  */
  void set_proxy(const string &proxy) 
  {
    _proxy = proxy;
  }
  
  /// Returns the Legal Representative
  /**
     @return Local Legal Representative
  */
  string get_proxy() 
  {
    return _proxy;
  }

  /// Inserts a domain name into the list
  /**
     @param fqdn fully qualified domain name
  */
  void insert_domainName(const string &fqdn)
  {
    _domain_list.insert(fqdn);
  }

  /// Returns the domain name list
  /**
     @return domain name list
  */
  set<string> get_domainName_list()
  {
    return _domain_list;
  }

  /// Sets  date and  time  identifying the  end  of the  organization
  /// object's registration period
  /**
     @param exDate expiration date
  */
  void set_exDate(const string &exDate)
  {
    _exDate = exDate;
  }

  /// Returns date  and time identifying  the end of  the organization
  /// object's registration period
  /**
     @return expiration date
  */
  string get_exDate() const
  {
    return _exDate;
  }

  /// Inserts   Autonomous   System   object  associated   with   this
  /// organization
  void insert_asn(const int &asn)
  {
    _asn_list.insert(asn);
  }

  /// Returns   Autonomous   System   object  associated   with   this
  /// organization
  set<int> get_asn_list() const
  {
    return _asn_list;
  }

  /// Inserts  IP addresses  of network  objects associated  with this
  /// organization
  void insert_ipRange(const IpRange &ipRange)
  {
    _ipRange_list.insert(ipRange);
  }
  
  /// Returns  IP addresses  of network  objects associated  with this
  /// organization
  set<IpRange> get_ipRange_list() const
  {
    return _ipRange_list;
  }

  /// reset attributes
  void reset() { 
    ContactInfoRsp::reset();
    _organization = "";
    _responsible = "";
    _proxy = "";
    _contact_list.clear();
    _domain_list.clear();
    _exDate = "";
    _asn_list.clear();
    _ipRange_list.clear();
  }

protected:
  /// Organization Identifier
  string _organization;

  /// Person responsible for the organization
  string _responsible;
  
  /// Human Contacts
  map<string, string, less<string> > _contact_list;

  /// Local Legal Representative
  string _proxy;

  /// Domain names
  set<string> _domain_list;

  /// Expiration date
  string _exDate;

  /// ASNs
  set<int> _asn_list;

  // IP Ranges
  set<IpRange> _ipRange_list;
};

LIBEPP_NICBR_NS_END
#endif //__BR_ORG_INFO_RSP_H__
