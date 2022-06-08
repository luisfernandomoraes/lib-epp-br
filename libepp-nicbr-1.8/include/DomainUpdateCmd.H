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
/* $Id: DomainUpdateCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file  DomainUpdateCmd.H
 *  @brief EPP DomainUpdateCmd Class
 */

#ifndef __DOMAIN_UPDATE_CMD_H__
#define __DOMAIN_UPDATE_CMD_H__

#include <string>
#include <vector>
#include <set>
#include <map>

#include "libepp_nicbr.H"

#include "Command.H"
#include "CommonData.H" // struct NameServer

using std::string;
using std::set;
using std::map;
using std::less;

LIBEPP_NICBR_NS_BEGIN

/// EPP DomainUpdateCmd Class
class DomainUpdateCmd : public Command
{
public:
  
  struct Status {
    string s;
    string lang;
    string msg;
    bool operator<(const Status &st) const {
      return s < st.s;
    }
  };

  /// Default constructor
  DomainUpdateCmd(bool reset = true) : Command(false)
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

  /// Inserts a nameserver_add to the list of nameservers
  /**
     @param nameserver_add   fully qualified domain name
  */
  void insert_nameserver_add(const struct NameServer &nameserver_add)
  {
    _nameserver_add.push_back(nameserver_add);
  }
  
  /// Returns a list of nameserver_add
  /**
     @return list of nameserver_add associated with domain object
  */
  vector<struct NameServer> get_nameserver_add() const 
  { 
    return _nameserver_add;
  }
  
  /// Inserts a nameserver_rem to the list of nameservers
  /**
     @param nameserver_rem   fully qualified domain name
   */
  void insert_nameserver_rem(const struct NameServer &nameserver_rem)
  {
    _nameserver_rem.push_back(nameserver_rem);
  }
  
  /// Returns a list of nameserver_rem
  /**
     @return list of nameserver_rem associated with domain object
  */
  vector<struct NameServer> get_nameserver_rem() const 
  {
    return _nameserver_rem;
  }

  /// Inserts a contact_add in the map of contacts
  /**
     @param type_add             contact type
     @param identification_add   contact identification
  */
  void insert_contact_add(const string& type, const string& identification)
  {
    _contact_add[type] = identification;
  }
  
  /// Returns map of contacts
  /**
     @return map of contacts
  */
  map< string, string, less<string> > get_contact_add() const 
  { 
    return _contact_add; 
  }
  
  /// Inserts a contact_rem in the map of contacts
  /**
     @param type_rem             contact type
     @param identification_rem   contact identification
  */
  void insert_contact_rem(const string& type, const string& identification)
  {
    _contact_rem[type] = identification;
  }
  
  /// Returns map of contacts
  /**
     @return map of contacts
  */
  map< string, string, less<string> > get_contact_rem() const 
  { 
    return _contact_rem; 
  }
  
  /// Changes a status_add
  /**
     @param status_add status to be changed
  */
  void insert_status_add(const struct Status &status_add) 
  { 
    _status_add.insert(status_add);
  }
  
  /// Returns set of domain status_add
  /**
     @return set of domain status_add
  */
  set<struct Status> get_status_add() const { return _status_add; }

  /// Changes a status_rem in _status_rem
  /**
     @param status_rem   the status_rem to be changed
  */
  void insert_status_rem(const struct Status &status_rem) 
  { 
    _status_rem.insert(status_rem); 
  }
  
  /// Returns set of domain status_rem
  /**
     @return set of domain status_rem
  */
  set<struct Status> get_status_rem() const { return _status_rem; }
  
  /// Sets registrant
  /**
     @param registrant The domain registrant
   */
  void set_registrant(const string &registrant) 
  {
    _registrant = registrant; 
    _registrant_f = true;
  };
  
  /// Returns registrant
  /**
     @return registrant The domain registrant
   */
  string get_registrant() const { return _registrant; };

  /// Sets the registrant change flag
  /**
     @param registrant_f registrant change flag
   */
  void set_registrant_f(const bool registrant_f)
  {
    _registrant_f = registrant_f;
  };
  
  /// Returns the registrant change flag
  /**
     @return The registrant change flag
   */
  bool get_registrant_f() const
  {
    return _registrant_f;
  };

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

  /// Inserts one ds into the ds addition list 
  /**
     @param ds_info   ds information
  */
  void insert_ds_add(const DSInfo &ds_info)
  {
    _ds_add.push_back(ds_info);
  }
  
  /// Returns the ds addition list
  /**
     @return list of ds records to be inserted into the domain object
  */
  list<DSInfo> get_ds_add() const { return _ds_add; }
  
  /// Inserts one ds into the ds removal list
  /**
     @param key_tag   key tag to identify the ds to be removed
   */
  void insert_ds_rem(const unsigned int &key_tag)
  {
    _ds_rem.push_back(key_tag);
  }
  
  /// Returns the ds removal list
  /**
     @return list of ds records to be removed from the domain object
  */
  list<unsigned int> get_ds_rem() const { return _ds_rem; }  

  /// Inserts one ds into the ds change list
  /**
     @param ds_info   ds information
   */
  void insert_ds_chg(const DSInfo &ds_info)
  {
    _ds_chg.push_back(ds_info);
  }
  
  /// Returns the ds change list
  /**
     @return list of ds records to be removed from the domain object
  */
  list<DSInfo> get_ds_chg() const { return _ds_chg; }

  /// Sets RFC4310 optional "urgent" attribute for domain:update
  /**
     @param urgentFlag domain:update urgent attribute value
   */
  void setUrgentFlag(const bool urgentFlag)
  {
    _urgentFlag = urgentFlag;
  }

  /// Getter for RFC4310 optional "urgent" attribute for domain:update
  /**
     @return RFC4310 optional "urgent" attribute for domain:update
   */
  bool isUrgent() const
  {
    return _urgentFlag;
  }

  //******************** RFC 4310 END ********************

  /// Check if there is any extension
  bool has_extension() const {
    return has_ds_extension();
  };
  
  /// Check if there is secDNS extension
  bool has_ds_extension() const {
    return (!_ds_add.empty() || !_ds_rem.empty() || !_ds_chg.empty());
  }

  /// Reset object attributes
  void reset()
  {
    Command::reset();
    _name = "";
    _nameserver_add.clear();
    _nameserver_rem.clear();
    _contact_add.clear();
    _contact_rem.clear();
    _status_add.clear();
    _status_rem.clear();
    _registrant = "";
    _registrant_f = false;

    // RFC 4310
    _ds_add.clear();
    _ds_rem.clear();
    _ds_chg.clear();
    _urgentFlag = false;

    _authInfo.reset();
  }

protected:
  /// fully qualified domain name
  string _name;
  
  /// name servers to be added to the domain object
  vector<struct NameServer> _nameserver_add;
  
  /// name servers to be removed from the domain object
  vector<struct NameServer> _nameserver_rem;
  
  /// contacts to be added to the domain object
  map< string, string, less<string> > _contact_add;

  /// contacts to be removed from the domain object
  map< string, string, less<string> > _contact_rem;
  
  /// set of status to be added to the domain object
  set<struct Status> _status_add;

  /// set of status to be removed from the domain object
  set<struct Status> _status_rem;

  /// registrant
  string _registrant;
  
  /// registrant change flag
  bool _registrant_f;
  
  /// authorization information
  AuthInfo _authInfo;

  /// list of ds to be added to the domain object
  list<DSInfo> _ds_add;
  
  /// list of ds (identified by key tags) to be removed from the domain object
  list<unsigned int> _ds_rem;

  /// list of ds to be changed in the domain object
  list<DSInfo> _ds_chg;

  /// optinal domain:update urgent flag
  bool _urgentFlag;
};

LIBEPP_NICBR_NS_END
#endif //__DOMAIN_UPDATE_CMD_H__
