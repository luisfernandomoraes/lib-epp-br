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
/* $Id: DomainInfoCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file  DomainInfoCmd.H
 *  @brief EPP DomainInfoCmd Class
 */

#ifndef __DOMAIN_INFO_CMD_H__
#define __DOMAIN_INFO_CMD_H__

#include <string>

#include "libepp_nicbr.H"

#include "Command.H"

using std::string;

LIBEPP_NICBR_NS_BEGIN

/// EPP DomainInfoCmd Class
class DomainInfoCmd : public Command
{
public:

  /// Default constructor
  DomainInfoCmd(bool reset = true) : Command(false)
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

  /*! @brief Sets "hosts" attribute which controls return of information
   *  describing hosts
   */
  /**
     @param hosts_control   "all" (default), "del", "sub", "none"
   */
  void set_hosts_control(const string& hosts_control)
  {
    _hosts_control = hosts_control;
  }

  /// Returns hosts control attribute
  /**
     @return hosts control attribute
   */
  string get_hosts_control() const { return _hosts_control; }

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

  /// Reset object attributes
  void reset()
  {
    Command::reset();
    _name = "";
    _hosts_control = "all";
    _authInfo.reset();
  }
  
protected:
  /// fully qualified domain name
  string _name;

  /// controls return of information describing hosts related to
  /// domain object
  string _hosts_control;

  /// authorization information
  AuthInfo _authInfo;
};

LIBEPP_NICBR_NS_END
#endif //__DOMAIN_INFO_CMD_H__
