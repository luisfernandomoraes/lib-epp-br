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
/* $Id: IpNetworkUpdateCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file IpNetworkUpdateCmd.H
 *  @brief EPP IpNetworkUpdateCmd Class
 */

#ifndef __IPNETWORKUPDATECMD_H__
#define __IPNETWORKUPDATECMD_H__

#include <map>
#include <vector>

#include "libepp_nicbr.H"

#include "Command.H"

using std::map;
using std::vector;

LIBEPP_NICBR_NS_BEGIN

/// EPP IpNetworkUpdateCmd Class
class IpNetworkUpdateCmd : public Command
{
public:
  /// Default constructor
  IpNetworkUpdateCmd(bool reset = true) : Command(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Sets repository object ID
  /**
     @param roid   contains the server-unique id for the created IP network
  */
  void set_roid(const string &roid)
  {
    _roid = roid;
  }

  /// Returns repository object ID
  /**
     @return server-unique id for the created IP network
  */
  string get_roid() const
  {
    return _roid;
  }

  /// Sets organization
  /**
     @param organization   organization to be associated with the asn
  */
  void set_organization(const string &organization)
  {
    _organization = organization;
  }

  /// Returns organization associated with the ip network
  /**
     @return organization owner of the ip network
  */
  string get_organization() const
  {
    return _organization;
  }

  /// Sets allocation type
  /**
     @param allocType   ip range allocation type
  */
  void set_allocType(const string &allocType)
  {
    _allocType = allocType;
  }

  /// Returns allocation type
  /**
     @return ip range allocation type
  */
  string get_allocType() const
  {
    return _allocType;
  }

  /// Inserts a reverse DNS to be added
  /**
     @param reverseDns   reverse DNS object
  */
  void insert_reverseDNS_add(const ReverseDns &reverseDns)
  {
    _reverseDNS_add.push_back(reverseDns);
  }

  /// Returns all the reverse DNS to be added
  /**
     @return vector of reverseDns objects
  */
  vector<ReverseDns> get_reverseDns_add() const
  {
    return _reverseDNS_add;
  }

  /// Inserts a reverse DNS to be removed
  /**
     @param reverseDns   reverse DNS object
  */
  void insert_reverseDNS_rem(const ReverseDns &reverseDns)
  {
    _reverseDNS_rem.push_back(reverseDns);
  }

  /// Returns all the reverse DNS to be removed
  /**
     @return vector of reverseDns objects
  */
  vector<ReverseDns> get_reverseDns_rem() const
  {
    return _reverseDNS_rem;
  }

  /// Inserts a contact to be added
  /**
     @param type             contact type
     @param identification   contact identification
  */
  void insert_contact_add(const string &type, const string &identification)
  {
    _contacts_add[type] = identification;
  }

  /// Returns map of other contacts to be added
  /**
     @return map of other contacts
  */
  map< string, string, less<string> > get_contacts_add() const
  {
    return _contacts_add;
  }

  /// Inserts a contact to be removed
  /**
     @param type             contact type
     @param identification   contact identification
  */
  void insert_contact_rem(const string &type, const string &identification)
  {
    _contacts_rem[type] = identification;
  }

  /// Returns map of other contacts to be removed
  /**
     @return map of other contacts
  */
  map< string, string, less<string> > get_contacts_rem() const
  {
    return _contacts_rem;
  }

  void reset()
  {
    Command::reset();
    _roid = "";
    _organization = "";
    _allocType = "";
    _reverseDNS_add.clear();
    _reverseDNS_rem.clear();
    _contacts_add.clear();
    _contacts_rem.clear();
  }

protected:
  /// server-unique id for the created IP network
  string _roid;

  /// Reverses DNS to be added
  vector<ReverseDns> _reverseDNS_add;

  /// Reverses DNS to be removed
  vector<ReverseDns> _reverseDNS_rem;

  /// contacts to be added
  map< string, string, less<string> > _contacts_add;

  /// contacts to be removed
  map< string, string, less<string> > _contacts_rem;

  /// organization associated with the ip network
  string _organization;

  /// allocation type associated with the ip network
  string _allocType;
};

LIBEPP_NICBR_NS_END
#endif // __IPNETWORKUPDATECMD_H__
