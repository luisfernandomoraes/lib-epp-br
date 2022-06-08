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
/* $Id: DomainRenewCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file  DomainRenewCmd.H
 *  @brief EPP DomainRenewCmd Class
 */

#ifndef __DOMAIN_RENEW_CMD_H__
#define __DOMAIN_RENEW_CMD_H__

#include <string>
#include <set>
#include <map>

#include "libepp_nicbr.H"

#include "Command.H"
#include "RegistrationPeriod.H"

using std::string;
using std::set;
using std::map;
using std::less;

LIBEPP_NICBR_NS_BEGIN

/// EPP DomainRenewCmd Class
class DomainRenewCmd : public Command
{
public:
  /// Default constructor
  DomainRenewCmd(bool reset = true) : Command(false)
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

  // Sets the date of the next renewal
  /**
     @param curExpDate date of the renewal
   */
  void set_curExpDate(const string& curExpDate) { _curExpDate = curExpDate; }

  /// Returns date of the next renewal
  /**
     @return date of the renewal
   */
  string get_curExpDate() const { return _curExpDate; }

  /// Sets domain renewal period
  /**
     @param time   amount of time
     @param unit   measure unit
   */
  void set_period(const int time, const string& unit)
  {
    _period.time = time;
    _period.unit = unit;
  }

  /// Sets domain registration period
  /**
     @param period   structure with time and measure unit
   */
  void set_period(const RegistrationPeriod& period)
  {
    _period.time = period.time;
    _period.unit = period.unit;
  }

  /// Returns domain registration period
  /**
     @return domain registration period
   */
  RegistrationPeriod get_period() const { return _period; }


  /// Reset object attributes
  void reset()
  {
    Command::reset();
    _name = "";
    _curExpDate = "";
    _period.time = 0;
    _period.unit = "";
  }
  
protected:
  /// Fully qualified domain name
  string _name;

  /// Date of the next renewal
  string _curExpDate;

  /// Renewal period of the domain object
  RegistrationPeriod _period;
};

LIBEPP_NICBR_NS_END
#endif //__DOMAIN_RENEW_CMD_H__
