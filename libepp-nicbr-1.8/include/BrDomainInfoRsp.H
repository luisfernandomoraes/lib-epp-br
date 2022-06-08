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
/* $Id: BrDomainInfoRsp.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file BrDomainInfoRsp.H
 *  @brief EPP BrDomainInfoRsp extension Class
 */

#ifndef __BR_DOMAIN_INFO_RSP_H__
#define __BR_DOMAIN_INFO_RSP_H__

#include <set>

#include "BrDomainCommon.H"
#include "DomainInfoRsp.H"

using std::set;

LIBEPP_NICBR_NS_BEGIN

/// EPP BrDomainInfoRsp extension Class
class BrDomainInfoRsp : public DomainInfoRsp
{
public:

  /// Default constructor
  BrDomainInfoRsp(bool reset = true) : DomainInfoRsp(false)
  {
    if (reset) {
      this->reset();
    }
  }
  
  /// Copies data from the parent
  void copy_parent_data(const DomainInfoRsp &parent) 
  {
    _name = parent.get_name();
    _roid = parent.get_roid();
    _status_set = parent.get_status_set();
    _registrant = parent.get_registrant();
    _contacts = parent.get_contacts();
    _nameservers = parent.get_nameservers();
    _clID = parent.get_clID();
    _crID = parent.get_crID();
    _crDate = parent.get_crDate();
    _upID = parent.get_upID();
    _exDate = parent.get_exDate();
    _upDate = parent.get_upDate();
    _trDate = parent.get_trDate();
    _authInfo = parent.get_authInfo();
    _dsInfo = parent.get_dsInfo();
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

  /// Sets organization attribute
  /**
     @param organization organization attribute
   */
  void set_organization(const string& organization)
  {
    _organization = organization;
  }

  /// Returns organization attribute
  /**
     @return organization attribute
   */
  string get_organization() const
  {
    return _organization;
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
  PENDING_CONTAINER<struct PendingDns> get_dns_pendings()
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

  /// Sets publication flag attribute
  /**
     @param publication flag attribute
   */
  void set_publication_flag(const string& publicationFlag)
  {
    _publicationFlag = publicationFlag;
  }

  /// Returns publication flag attribute
  /**
     @return publication flag attribute
   */
  string get_publication_flag() const
  {
    return _publicationFlag;
  }

  /// Inserts a onHold Reason
  /**
     @param reason onHold Reason
  */
  
  void insert_onhold_reason(const string& reason)
  {
    _onhold_reasons.insert(reason);
  }

  /// Returns the onHold Reasons
  /**
     @return onHold Reasons
  */
  set<string> get_onhold_reasons() const
  {
    return _onhold_reasons;
  }

  ///  Actives/Inactives domain auto renewal 
  /**
     @param auto_renew attribute
  */
  void set_auto_renew(int auto_renew)
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
  
  /// Resets object attributes
  void reset()
  {
    DomainInfoRsp::reset();
    _ticketNumber = 0;
    _organization = "";
    _releaseProcessFlags.flag1 = 0;
    _releaseProcessFlags.flag2 = 0;
    _releaseProcessFlags.flag3 = 0;
    _doc_pendings.clear();
    _dns_pendings.clear();
    _rel_pending.status = "";
    _rel_pending.limit = "";
    _concurrent_tickets.clear();
    _publicationFlag = "";
    _onhold_reasons.clear();
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

protected:
  /// ticket number
  int _ticketNumber;

  /// organization
  string _organization;

  /// release process flags
  struct ReleaseProcessFlags _releaseProcessFlags;

  // Pendings
  /// doc
  PENDING_CONTAINER<struct PendingDoc> _doc_pendings;

  /// dns
  PENDING_CONTAINER<struct PendingDns> _dns_pendings;

  /// releaseProc
  struct PendingReleaseProc _rel_pending;

  /// ticketNumberConc
  set<int> _concurrent_tickets;

  /// publicationFlag
  string _publicationFlag;

  /// onHoldReason
  set<string> _onhold_reasons;

  /// domain auto renewal flag
  int _auto_renew;  
};

LIBEPP_NICBR_NS_END
#endif //__BR_DOMAIN_INFO_RSP_H__
