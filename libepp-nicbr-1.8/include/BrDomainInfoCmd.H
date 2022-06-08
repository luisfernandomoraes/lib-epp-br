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
/* $Id: BrDomainInfoCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file BrDomainInfoCmd.H
 *  @brief EPP BrDomainInfoCmd extension Class
 */

#ifndef __BR_DOMAIN_INFO_CMD_H__
#define __BR_DOMAIN_INFO_CMD_H__

#include "DomainInfoCmd.H"

LIBEPP_NICBR_NS_BEGIN

/// EPP BrDomainInfoCmd extension Class
class BrDomainInfoCmd : public DomainInfoCmd
{
public:
  /// Default constructor
  BrDomainInfoCmd(bool reset = true) : DomainInfoCmd(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Sets ticket number
  /**
     @param ticketNumber   ticket number
   */
  void set_ticketNumber(int ticketNumber) 
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

  /// Reset object attributes
  void reset()
  {
    DomainInfoCmd::reset();
    _ticketNumber = 0;
  }
  
protected:
  /// ticket number
  int _ticketNumber;
};

LIBEPP_NICBR_NS_END
#endif //__BR_DOMAIN_INFO_CMD_H__
