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
/* $Id: DomainCreate.cpp 919 2007-08-30 14:43:07Z eduardo $ */

#include "config.h"

#include "DomainCreate.H"
#include "StrUtil.H"

LIBEPP_NICBR_NS_BEGIN

void DomainCreate::set_xml_template(const string &xml_template)
{
  StrUtil su;
  Action::set_xml_template_common(xml_template);
  map < string, string, less<string> > to_parse;

  //name
  to_parse["name"] = su.esc_xml_markup(get_command()->get_name());

  //period
  RegistrationPeriod period = get_command()->get_period();
  to_parse["period"] = "";
  if (period.time > 0) {
    to_parse["period"] = "<domain:period unit='" + 
      su.esc_xml_markup(period.unit) + "'>" +
      StrUtil::to_string("%d", period.time) + "</domain:period>";
  }

  //nameservers
  vector<struct NameServer> nss = get_command()->get_nameservers();
  vector<struct NameServer>::const_iterator ns_it;
  set<NSIPAddr>::const_iterator ip_it;
  to_parse["nameservers"] = "";
  if (!nss.empty()) {
    to_parse["nameservers"] = "<domain:ns>";

    for (ns_it = nss.begin(); ns_it != nss.end(); ns_it++) {
      to_parse["nameservers"] += "<domain:hostAttr>";
      to_parse["nameservers"] += "<domain:hostName>" +
	su.esc_xml_markup(ns_it->name) + "</domain:hostName>";
      for (ip_it = ns_it->ips.begin(); ip_it != ns_it->ips.end(); ip_it++) {
	to_parse["nameservers"] += "<domain:hostAddr";
	if ((*ip_it).version != "") {
	  to_parse["nameservers"] += " ip='" + 
	    su.esc_xml_markup((*ip_it).version) + "'";
	}
	to_parse["nameservers"] += ">" + su.esc_xml_markup((*ip_it).addr) +
	  "</domain:hostAddr>";
      }
      to_parse["nameservers"] += "</domain:hostAttr>";
    }

    to_parse["nameservers"] += "</domain:ns>";
  }

  //registrant
  to_parse["registrant"] = "";
  if (get_command()->get_registrant() != "") {
    to_parse["registrant"] = "<domain:registrant>" +
      su.esc_xml_markup(get_command()->get_registrant()) +
      "</domain:registrant>";
  }

  //other contacts
  map< string, string, less<string> > contacts = get_command()->get_contacts();
  map< string, string, less<string> >::const_iterator it_contacts;
  to_parse["other_contacts"] = "";
  for (it_contacts = contacts.begin(); it_contacts != contacts.end();
       it_contacts++) {
    to_parse["other_contacts"] +=
      "<domain:contact type='" + su.esc_xml_markup((*it_contacts).first) +
      "'>" + su.esc_xml_markup((*it_contacts).second) + "</domain:contact>";
  }

  //auth info
  AuthInfo authInfo;
  authInfo = get_command()->get_authInfo();
  string auth_info_str;
  auth_info_str = "<domain:authInfo>";
  
  if (authInfo.get_roid() == "") {
    auth_info_str += "<domain:pw>" + su.esc_xml_markup(authInfo.get_pw()) +
      "</domain:pw>";
  } else {
    auth_info_str += "<domain:pw roid='" + 
      su.esc_xml_markup(authInfo.get_roid()) + "'>" + 
      su.esc_xml_markup(authInfo.get_pw()) + "</domain:pw>";
  }
  auth_info_str += "</domain:authInfo>";
  to_parse["auth_info"] = auth_info_str;

  to_parse["ext_begin"] = "";
  to_parse["ext_end"] = "";
  to_parse["ds_ext"] = "";

  /////////////////////////////////////////////////////////////////////////////
  // DS Extension Mapping - RFC4310
  if (get_command()->has_extension()) {
    to_parse["ext_begin"] = "<extension>";
    to_parse["ext_end"] = "</extension>";
  
    if (get_command()->has_ds_extension()) {
      to_parse["ds_ext"] +=  "<secDNS:create "
	"xmlns:secDNS=\"urn:ietf:params:xml:ns:secDNS-1.0\" "
	"xsi:schemaLocation=\"urn:ietf:params:xml:ns:secDNS-1.0 "
	"secDNS-1.0.xsd\">";

      list<DSInfo> ds_info_list = get_command()->get_dsInfo();
      list<DSInfo>::iterator it;

      for (it = ds_info_list.begin(); it != ds_info_list.end(); it++) {
	to_parse["ds_ext"] += it->get_xml_format();
      }

      to_parse["ds_ext"] += "</secDNS:create>"; 
    }
  }
  _xml = StrUtil::parse(_xml, to_parse, "$(", ")$");
}

LIBEPP_NICBR_NS_END
