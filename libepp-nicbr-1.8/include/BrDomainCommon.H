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
/* $Id: BrDomainCommon.H 668 2006-04-12 15:18:01Z cesar $ */
/** @file BrDomainCommon.H
 *  @brief Common elements to .BR objects
 */

#ifndef __BR_DOMAIN_COMMON_H__
#define __BR_DOMAIN_COMMON_H__

// DON'T use "set", tests fail
// USE "list" or "vector"
#define PENDING_CONTAINER vector

#if PENDING_CONTAINER == list
#include <list>
#elif PENDING_CONTAINER == set
#include <set>
#elif PENDING_CONTAINER == vector
#include <vector>
#endif
using std::PENDING_CONTAINER;

struct ReleaseProcessFlags {
  int flag1;
  int flag2;
  int flag3;
  
  ReleaseProcessFlags()
  {
    flag1 = -1;
    flag2 = -1;
    flag3 = -1;
  }
};

struct PendingDoc {
  string status;
  string docType;
  string limit;
  string description;
  string description_lang;
#if PENDING_CONTAINER == set
  bool operator<(const PendingDoc &p) const {
    return limit < p.limit;
  }
#endif
};

struct PendingDns {
  string status;
  string hostName;
  string limit;
#if PENDING_CONTAINER == set
  bool operator<(const PendingDns &p) const {
    return limit < p.limit;
  }
#endif
};

struct PendingReleaseProc {
  string status;
  string limit;
};

struct HostStatus {
  string host;
  string status;
  bool operator<(const HostStatus &hs) const {
    return status < hs.status;
  }
};

#endif //__BR_DOMAIN_COMMON_H__
