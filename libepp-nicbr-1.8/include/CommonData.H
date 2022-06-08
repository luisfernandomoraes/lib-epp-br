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
/* $Id: CommonData.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file CommonData.H
 *  @brief EPP CommonData Class
 */

#ifndef __COMMON_DATA_H__
#define __COMMON_DATA_H__

#include <string>
#include <vector>
#include <set>

#include "libepp_nicbr.H"

using std::string;
using std::vector;
using std::set;

LIBEPP_NICBR_NS_BEGIN

/// Action Types
enum ActionType {
  UNSET_ACTION = -1,
  LOGIN = 0,
  LOGOUT,
  HELLO,
  POLL,
  GREETING,
  CONTACT_CHECK = 10,
  CONTACT_CREATE,
  CONTACT_INFO,
  CONTACT_TRANSFER,
  CONTACT_UPDATE,
  CONTACT_PANDATA,
  DOMAIN_CHECK = 20,
  DOMAIN_CREATE,
  DOMAIN_INFO,
  DOMAIN_RENEW,
  DOMAIN_UPDATE,
  DOMAIN_PANDATA,
  DOMAIN_DELETE,
  BR_ORG_CHECK = 30,
  BR_ORG_INFO,
  BR_ORG_CREATE,
  BR_ORG_UPDATE,
  BR_ORG_PANDATA,
  BR_DOMAIN_CHECK = 40,
  BR_DOMAIN_INFO,
  BR_DOMAIN_CREATE,
  BR_DOMAIN_RENEW,
  BR_DOMAIN_UPDATE,
  BR_DOMAIN_PANDATA,
  IP_NETWORK_CREATE = 50,
  IP_NETWORK_CHECK,
  IP_NETWORK_DELETE,
  IP_NETWORK_RENEW,
  IP_NETWORK_TRANSFER,
  IP_NETWORK_INFO,
  IP_NETWORK_UPDATE,
  ASN_CHECK = 60,
  ASN_CREATE,
  ASN_DELETE,
  ASN_RENEW,
  ASN_TRANSFER,
  ASN_INFO,
  ASN_UPDATE
};

/// AuthInfo Class
class AuthInfo {
public:
  AuthInfo& operator=(const AuthInfo &authInfo) {
    _roid = authInfo._roid;
    _roid_f = authInfo._roid_f;
    _pw = authInfo._pw;
    _pw_f = authInfo._pw_f;
    return *this;
  }

  /// Default constructor
  AuthInfo()
  {
    this->reset();
  }

  /// Sets repository object ID
  /**
     @param roid   repository object id
   */
  void set_roid(const string& roid) 
  { 
    _roid = roid;
    if (roid != "") {
      _roid_f = true;
    }
  }

  /// Returns repository object ID
  /**
     @return repository object ID
   */
  string get_roid() const { return _roid; }

  /// Returns the roid change flag
  /**
     @return roid change flag
  */
  bool get_roid_f() const
  {
    return _roid_f;
  }

  /// Sets password
  /**
     @param pw   password
   */
  void set_pw(const string& pw) 
  { 
    _pw = pw; 
    _pw_f = true;
  }

  /// Returns the password
  /**
     @return password
   */
  string get_pw() const { return _pw; }
  
  /// Returns the password change flag
  /**
     @return password change flag
  */
  bool get_pw_f() const
  {
    return _pw_f;
  }

  /// Reset all object attributes
  void reset()
  {
    _roid = "";
    _roid_f = false;
    _pw = "";
    _pw_f = false;
  }

protected:
  /// repository object ID
  string _roid;

  /// roid change flag
  bool _roid_f;

  /// password
  string _pw;
  
  /// password change flag
  bool _pw_f;

};

struct NSIPAddr {
  string version;
  string addr;
  bool operator<(const NSIPAddr &ip) const {
    return addr < ip.addr;
  }
};

struct NameServer {
  string name;
  set<NSIPAddr> ips;
  bool operator<(const NameServer &n) const {
    return name < n.name;
  }
};

/// Describes IpRange structure
class IpRange
{
public:
  IpRange() {
    this->reset();
  }

  bool operator<(const IpRange &ipRange) const 
  {
    if (_ipBegin.compare(ipRange._ipBegin) < 0) {
      return true;
    }

    if (_ipBegin.compare(ipRange._ipBegin) == 0) {
      if (_ipEnd.compare(ipRange._ipEnd) < 0) {
	return true;
      }
      
      return false;
    }
    
    return false;
  }

  void reset()
  {
    _version = "";
    _ipBegin = "";
    _ipEnd = "";
    _available = false;
  }

  void set_version(const string &version)
  {
    _version = version;
  }

  string get_version() const
  {
    return _version;
  }

  void set_ipBegin(const string &ipBegin)
  {
    _ipBegin = ipBegin;
  }

  string get_ipBegin() const
  {
    return _ipBegin;
  }

  void set_ipEnd(const string &ipEnd)
  {
    _ipEnd = ipEnd;
  }

  string get_ipEnd() const
  {
    return _ipEnd;
  }

  void set_available(const bool &available)
  {
    _available = available;
  }

  bool get_available() const
  {
    return _available;
  }

private:
  string _version;
  string _ipBegin;
  string _ipEnd;

  // Used in ip range check command
  bool _available;
};

#if USE_IP_MANAGEMENT
struct ReverseDns {
  IpRange ipRange;
  vector<string> nameservers;
};
#endif //USE_IP_MANAGEMENT

/// PostalInfo class
class PostalInfo {
public:
  PostalInfo() {
    this->reset();
  }
 
  void reset() {
    _type_f = false;
    _name_f = false;
    _org_f = false;
    _str1_f = false;
    _str2_f = false;
    _str3_f = false;
    _city_f = false;
    _sp_f = false;
    _pc_f = false;
    _cc_f = false;
  }

  string get_type() const
  {
    return _type;
  }

  void set_type(const string& type)
  {
    _type = type;
    _type_f = true;
  }

  string get_name() const
  {
    return _name;
  }

  void set_name(const string& name)
  {
    _name = name;
    _name_f = true;
  }

  string get_org() const
  {
    return _org;
  }

  void set_org(const string& org)
  {
    _org = org;
    _org_f = true;
  }

  string get_str1() const
  {
    return _str1;
  }

  void set_str1(const string& str1)
  {
    _str1 = str1;
    _str1_f = true;
  }

  string get_str2() const
  {
    return _str2;
  }

  void set_str2(const string& str2)
  {
    _str2 = str2;
    _str2_f = true;
  }

  string get_str3() const
  {
    return _str3;
  }

  void set_str3(const string& str3)
  {
    _str3 = str3;
    _str3_f = true;
  }

  string get_city() const
  {
    return _city;
  }

  void set_city(const string& city)
  {
    _city = city;
    _city_f = true;
  }

  string get_sp() const
  {
    return _sp;
  }

  void set_sp(const string& sp)
  {
    _sp = sp;
    _sp_f = true;
  }

  string get_pc() const
  {
    return _pc;
  }

  void set_pc(const string& pc)
  {
    _pc = pc;
    _pc_f = true;
  }

  string get_cc() const
  {
    return _cc;
  }

  void set_cc(const string& cc)
  {
    _cc = cc;
    _cc_f = true;
  }

  bool get_type_f() const
  {
    return _type_f;
  }

  bool get_name_f() const
  {
    return _name_f;
  }

  bool get_org_f() const
  {
    return _org_f;
  }

  bool get_str1_f() const
  {
    return _str1_f;
  }

  bool get_str2_f() const
  {
    return _str2_f;
  }

  bool get_str3_f() const
  {
    return _str3_f;
  }

  bool get_city_f() const
  {
    return _city_f;
  }

  bool get_sp_f() const
  {
    return _sp_f;
  }

  bool get_pc_f() const
  {
    return _pc_f;
  }

  bool get_cc_f() const
  {
    return _cc_f;
  }

private:
  //Flags for PostalInfo attributes
  bool _type_f;
  bool _name_f;
  bool _org_f;
  bool _str1_f;
  bool _str2_f;
  bool _str3_f;
  bool _city_f;
  bool _sp_f;
  bool _pc_f;
  bool _cc_f;

  //PostalInfo attributes
  string _type;
  string _name;
  string _org;
  string _str1;
  string _str2;
  string _str3;
  string _city;
  string _sp;
  string _pc;
  string _cc;
};

/// EPP CommonData Class
class CommonData
{
public:
 
  struct Disclose {
    int flag;

    bool name_int;
    bool name_loc;
    bool org_int;
    bool org_loc;
    bool addr_int;
    bool addr_loc;
    bool voice;
    bool fax;
    bool email;

    Disclose() {
      flag = -1;
      name_int = false;
      name_loc = false;
      org_int = false;
      org_loc = false;
      addr_int = false;
      addr_loc = false;
      voice = false;
      fax = false;
      email = false;
    }

    bool is_set() {      
      return ((flag == 0 || flag == 1) &&
	      (name_int || name_loc || org_int || org_loc || addr_int || 
	       addr_loc || voice || fax || email));
    }
  };

  struct Phone {
    string ext;
    string number;
  };

  /// Constructor
  CommonData()
  {
    this->reset();
  }

  /// Sets the object's id
  /**
     @param id contact id
  */
  void set_id(const string &id);
  
  /// Inserts postal information into the object
  /**
    @param postal_info postal information
  */
  void insert_postal_info(const PostalInfo& postal_info);

  /// Sets the object's voice telephone number
  /**
     @param voice contact voice telephone
  */
  void set_voice(const Phone& voice);
  
  /// Sets the object's fax number
  /**
     @param  fax contact fax 
  */
  void set_fax(const Phone& fax);
  
  /// Sets the object's email
  /**
     @param email contact email
  */
  void set_email(const string &email);
  
  /// Sets the object's disclosure policy
  /**
     @param disclose disclose information
  */
  void set_disclose(const Disclose &disclose);
  
  /// Returns the object's id
  /**
     @return Contact id
  */
  string get_id() const;

  /// Returns the object's postal information
  /**
     @return Postal info list
  */
  vector<PostalInfo> get_postal_info() const;

  /// Returns the object's voice telephone number
  /**
     @return voice contact voice telephone number
  */
  Phone get_voice() const;
  
  /// Returns the object's voice change flag
  /**
     @return contact voice change flag
  */
  bool get_voice_f() const
  {
    return _voice_f;
  }

  /// Returns the object's fax number
  /**
     @return fax contact fax number
  */
  Phone get_fax() const;
  
  /// Returns the object's fax change flag
  /**
     @return contact fax change flag
  */
  bool get_fax_f() const
  {
    return _fax_f;
  }

  /// Returns the object's email
  /**
     @return contact email
  */
  string get_email() const;

  /// Returns the object's email change flag
  /**
     @return contact email change flag
  */
  bool get_email_f() const
  {
    return _email_f;
  }
  
  /// Returns the object's disclosure policy
  /**
     @return disclose disclose information
  */
  Disclose get_disclose() const;

  /// reset attributes
  void reset();
  
protected:
  
  /// Contact id 
  string _id;

  /// Postal Info list (just 1 or 2 elements)
  vector<PostalInfo> _postal_info;

  /// Contact's voice telephone number (optional)
  Phone _voice;

  /// Voice change flag
  bool _voice_f;

  /// Contact's fax (optional)
  Phone _fax;

  /// Fax change flag
  bool _fax_f;

  /// Contact's email
  string _email;

  /// E-mail change flag
  bool _email_f;

  /// Disclose information (optional)
  Disclose _disclose;
};

LIBEPP_NICBR_NS_END
#endif //__COMMON_DATA_H__
