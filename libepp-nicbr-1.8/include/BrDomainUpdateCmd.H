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
/* $Id: BrDomainUpdateCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file BrDomainUpdateCmd.H
 *  @brief EPP BrDomainUpdateCmd extension Class
 */

#ifndef __BR_DOMAIN_UPDATE_CMD_H__
#define __BR_DOMAIN_UPDATE_CMD_H__

#include "DomainUpdateCmd.H"

LIBEPP_NICBR_NS_BEGIN

/// EPP BrDomainUpdateCmd extension Class
class BrDomainUpdateCmd : public DomainUpdateCmd
{
public:
  /// Default constructor
  BrDomainUpdateCmd(bool reset = true) : DomainUpdateCmd(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Sets ticket number
  /**
     @param ticketNumber   ticket number
   */
  void set_ticketNumber(const int ticketNumber) 
  {
    _ticketNumber = ticketNumber;
  }

  /// Returns ticket number
  /**
     @return ticket number
   */
  int get_ticketNumber() const
  {
    return _ticketNumber;
  }

  /// Sets the Release Process Flags
  /**
     @param rpf Release Process Flags
  */
  void set_releaseProcessFlags(const struct ReleaseProcessFlags& rpf)
  {
    _releaseProcessFlags.flag1 = rpf.flag1;
    _releaseProcessFlags.flag2 = rpf.flag2;
    _releaseProcessFlags.flag3 = rpf.flag3;
  }

  /// Returns the Release Process Flags
  /**
     @return Release Process Flags
  */
  struct ReleaseProcessFlags get_releaseProcessFlags() const
  {
    return _releaseProcessFlags;
  }

  ///  Actives/Inactives domain auto renewal 
  /**
     @param auto_renew attribute
  */
  void set_auto_renew(const int auto_renew)
  {
    _auto_renew = auto_renew;
  }
  
  ///  Return if the domain is with or without auto renewal
  /**
     @return auto_renew attribute 
  */
  int get_auto_renew() const
  {
    return _auto_renew;
  }
  
  /// Reset object attributes
  void reset()
  {
    DomainUpdateCmd::reset();
    _ticketNumber = 0;
    _releaseProcessFlags.flag1 = -1;
    _releaseProcessFlags.flag2 = -1;
    _releaseProcessFlags.flag3 = -1;
    _auto_renew = -1;
 }
  
  ///  Sets active attribute (***DEPRECATED***)
  /**
     @param active attribute
  */
  void set_active(int active)
  {
    set_auto_renew(active);
  }
  
  /// Returns active attribute (***DEPRECATED***)
  /**
     @return active attribute
  */
  int get_active()
  {
    return get_auto_renew();
  }

  /// Check if there is any extension
  bool has_extension() {
    return (DomainUpdateCmd::has_extension() || has_br_extension());
  }
  
  /// Check if there is BR extension
  bool has_br_extension() {
    return (_ticketNumber > 0 || _auto_renew == 0 || _auto_renew == 1);
  }
  
protected:
  /// ticket number
  int _ticketNumber;

  /// release process flags
  struct ReleaseProcessFlags _releaseProcessFlags;

  /// domain auto renewal flag
  int _auto_renew;    
};

LIBEPP_NICBR_NS_END
#endif //__BR_DOMAIN_UPDATE_CMD_H__
