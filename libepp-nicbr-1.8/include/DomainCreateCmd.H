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
/* $Id: DomainCreateCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file  DomainCreateCmd.H
 *  @brief EPP DomainCreateCmd Class
 */

#ifndef __DOMAIN_CREATE_CMD_H__
#define __DOMAIN_CREATE_CMD_H__

#include <string>
#include <vector>
#include <map>
#include <list>

#include "libepp_nicbr.H"

#include "Command.H"
#include "CommonData.H" // struct NameServer
#include "RegistrationPeriod.H"

using std::string;
using std::set;
using std::map;
using std::list;
using std::less;

LIBEPP_NICBR_NS_BEGIN

/// EPP DomainCreateCmd Class
class DomainCreateCmd : public Command
{
public:
  /// Default constructor
  DomainCreateCmd(bool reset = true) : Command(false)
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

  /// Sets domain registration period
  /**
     @param time   amount of time
     @param unit   measure unit
   */
  void set_period(const int time, const string& unit)
  {
    _period.time = time;
    _period.unit = unit;
  }

  /// Returns domain registration period
  /**
     @return domain registration period
   */
  RegistrationPeriod get_period() const { return _period; }

  /// Inserts a nameserver to the list of nameservers
  /**
     @param nameserver   a name server
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
  ///  Sets ds info attribute 
  /**
     @param ds info attribute
  */
  void add_dsInfo(const DSInfo &ds_info)
  {
    _ds_info.push_back(ds_info);
  }
  
  /// Returns ds info attribute 
  /**
     @return ds info attribute
  */
  list<DSInfo> get_dsInfo() const { return _ds_info; }

  //******************** RFC 4310 END ********************

  /// Check if there is any extension
  bool has_extension() const {
    return has_ds_extension();
  }
  
  /// Check if there is secDNS extension
  bool has_ds_extension() const { 
    return !_ds_info.empty();
  }

  /// Reset object attributes
  void reset()
  {
    Command::reset();
    _name = "";
    _period.time = 0;
    _period.unit = "";
    _nameservers.clear();
    _registrant = "";
    _contacts.clear();
    _authInfo.reset();
    _ds_info.clear(); 
  }
  
protected:
  /// fully qualified domain name
  string _name;

  /// initial registration period of the domain object
  RegistrationPeriod _period;

  /// name servers associated with domain object
  vector<struct NameServer> _nameservers;

  /// registrant
  string _registrant;

  /// other contact objects
  map< string, string, less<string> > _contacts;

  /// Authorization information
  AuthInfo _authInfo;

  /// DS info
  list<DSInfo> _ds_info;
};

LIBEPP_NICBR_NS_END
#endif //__DOMAIN_CREATE_CMD_H__
