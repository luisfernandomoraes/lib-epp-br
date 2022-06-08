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
/* $Id: SheppPrint.H 672 2006-04-13 11:59:34Z eduardo $ */
/** @file SheppPrint.H
 *  @brief shepp object information screen printing class
 */

#ifndef __SHEPP_PRINT_H__
#define __SHEPP_PRINT_H__

#include "CommonData.H"

/// shepp object information screen printing class
class SheppPrint {
public:
  //used by both domain and contact

  /// prints an AuthInfo object
  /**
     @param auth   AuthInfo object to be printed
   */
  static void authInfo(AuthInfo auth) {
    if (auth.get_pw() != "") {
      printf("  authInfo pw: [%s]\n", auth.get_pw().c_str());
      if (auth.get_roid() != "") {
	printf("           roid: [%s]\n", auth.get_roid().c_str());
      }
    }
  }

  //used by domain only

  /// prints a NameServer object
  /**
     @param ns   NameServer object to be printed
   */
  static void nameserver(NameServer ns)
  {
    printf("  nameserver %s\n", ns.name.c_str());
  
    set<NSIPAddr>::const_iterator it;
    set<NSIPAddr> ips = ns.ips;
    for (it = ips.begin(); it != ips.end(); it++) {
      printf("    %s: %s\n", (*it).version.c_str(), (*it).addr.c_str());
    }
  }

  //used by contact only

  /// prints a PostalInfo object
  /**
     @param postal   PostalInfo object to be printed
   */
  static void postal_info(PostalInfo postal)
  {
    printf("  PostalInfo:\n");
    printf("    type: [%s]\n", postal.get_type().c_str());

    if (postal.get_name() != "") {
      printf("    name: [%s]\n", postal.get_name().c_str());
    }
    if (postal.get_org() != "") {
      printf("    org : [%s]\n", postal.get_org().c_str());
    }

    if (postal.get_str1() != "") {
      printf("    str1: [%s]\n", postal.get_str1().c_str());
    }
    if (postal.get_str2() != "") {
      printf("    str2: [%s]\n", postal.get_str2().c_str());
    }
    if (postal.get_str3() != "") {
      printf("    str3: [%s]\n", postal.get_str3().c_str());
    }

    if (postal.get_city() != "") {
      printf("    city: [%s]\n", postal.get_city().c_str());
    }

    if (postal.get_sp() != "") {
      printf("    s/p : [%s]\n", postal.get_sp().c_str());
    }

    if (postal.get_pc() != "") {
      printf("    pc  : [%s]\n", postal.get_pc().c_str());
    }

    if (postal.get_cc() != "") {
      printf("    cc  : [%s]\n", postal.get_cc().c_str());
    }
  }

  /// prints a Phone object
  /**
     @param phone   Phone object to be printed
   */
  static void phone(CommonData::Phone phone)
  {
    printf("  number: [%s]  ext: [%s]\n", phone.number.c_str(),
	   phone.ext.c_str());
  }

  /// prints a Disclose object
  /**
     @param   disclose object to be printed
  */
  static void disclose(CommonData::Disclose disclose)
  {
    printf("  disclose (flag=%d) [ ", disclose.flag);
    if (disclose.name_int) {
      printf("name_int ");
    }
    if (disclose.name_loc) {
      printf("name_loc ");
    }
    if (disclose.org_int) {
      printf("org_int ");
    }
    if (disclose.org_loc) {
      printf("org_loc ");
    }
    if (disclose.addr_int) {
      printf("addr_int ");
    }
    if (disclose.addr_loc) {
      printf("addr_loc ");
    }
    if (disclose.voice) {
      printf("voice ");
    }
    if (disclose.fax) {
      printf("fax ");
    }
    if (disclose.email) {
      printf("email ");
    }
    printf("]\n");
  }
};

#endif //__SHEPP_PRINT_H__
