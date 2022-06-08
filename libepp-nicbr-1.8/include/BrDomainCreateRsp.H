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
/* $Id: BrDomainCreateRsp.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file BrDomainCreateRsp.H
 *  @brief EPP BrDomainCreateRsp extension Class
 */

#ifndef __BR_DOMAIN_CREATE_RSP_H__
#define __BR_DOMAIN_CREATE_RSP_H__

#include "BrDomainCommon.H"
#include "DomainCreateRsp.H"

LIBEPP_NICBR_NS_BEGIN

/// EPP BrDomainCreateRsp extension Class
class BrDomainCreateRsp : public DomainCreateRsp
{
public:

  /// Default constructor
  BrDomainCreateRsp(bool reset = true) : DomainCreateRsp(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Copies data from the parent
  void copy_parent_data(const DomainCreateRsp &parent) 
  {
    _name = parent.get_name();    
    _crDate = parent.get_crDate();
    _exDate = parent.get_exDate();
  }
  
  /// Sets the Ticket Number
  /**
     @param ticketNumber Ticket Number
  */
  void set_ticketNumber(int ticketNumber)
  {
    _ticketNumber = ticketNumber;
  }

  /// Returns the Ticket Number
  /*
    @param Ticket Number 
  */
  int get_ticketNumber() const
  {
    return _ticketNumber;
  }

  /// Inserts a Doc Pending to the ticket
  /**
     @param p new Doc Pending
  */
  void insert_doc_pending(const struct PendingDoc& p)
  {
    _doc_pendings.insert(_doc_pendings.end(), p);
  }

  /// Returns the ticket's Doc Pendings
  /**
     @return Doc Pendings
  */
  PENDING_CONTAINER<struct PendingDoc> get_doc_pendings() const
  {
    return _doc_pendings;
  }

  /// Inserts a Dns Pending to the ticket
  /**
     @param p new Dns Pending
  */
  void insert_dns_pending(const struct PendingDns& p)
  {
    _dns_pendings.insert(_dns_pendings.end(), p);
  }

  /// Returns the ticket's Dns Pendings
  /**
     @return Dns Pendings
  */
  PENDING_CONTAINER<struct PendingDns> get_dns_pendings() const
  {
    return _dns_pendings;
  }

  /// Sets the ticket's Release Process Pending
  /**
     @param p Release Process Pending
  */
  void set_rel_pending(const struct PendingReleaseProc& p)
  {
    _rel_pending.status = p.status;
    _rel_pending.limit = p.limit;
  }

  /// Returns the ticket's Release Process Pending
  /**
     @return Release Process Pending
  */
  struct PendingReleaseProc get_rel_pending() const
  {
    return _rel_pending;
  }

  /// Inserts a concurrent ticket
  /**
     @param ticket Concurrent Ticket Number
  */
  void insert_concurrent_ticket(int ticket)
  {
    _concurrent_tickets.insert(ticket);
  }

  /// Returns the Concurrent Tickets
  /**
     @return Concurrent Tickets
  */
  set<int> get_concurrent_tickets() const
  {
    return _concurrent_tickets;
  }

  /// Resets object attributes
  void reset()
  {
    DomainCreateRsp::reset();
    _ticketNumber = 0;
    _doc_pendings.clear();
    _dns_pendings.clear();
    _rel_pending.status = "";
    _rel_pending.limit = "";
    _concurrent_tickets.clear();
  }

protected:
  /// ticket number
  int _ticketNumber;

  // Pendings
  /// doc
  PENDING_CONTAINER<struct PendingDoc> _doc_pendings;

  /// dns
  PENDING_CONTAINER<struct PendingDns> _dns_pendings;

  /// releaseProc
  struct PendingReleaseProc _rel_pending;
  
  /// ticketNumberConc
  set<int> _concurrent_tickets;

};

LIBEPP_NICBR_NS_END
#endif //__BR_DOMAIN_CREATE_RSP_H__
