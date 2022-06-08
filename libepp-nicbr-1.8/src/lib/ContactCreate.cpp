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
/* $Id: ContactCreate.cpp 756 2006-06-09 17:42:34Z koji $ */

#include <set>

#include "config.h"

#include "ContactCreate.H"
#include "StrUtil.H"

using std::set;

LIBEPP_NICBR_NS_BEGIN

void ContactCreate::set_xml_template(const string &xml_template)
{
  StrUtil su;
  Action::set_xml_template_common(xml_template);
  map < string, string, less<string> > to_parse;
  
  to_parse["id"] = 
    su.esc_xml_markup(get_command()->get_common_data().get_id());

  vector<PostalInfo> postal_info = 
    get_command()->get_common_data().get_postal_info();
  string pf_str;
  string org;
  string str2;
  string str3;
  string sp;
  string pc;

  for (int i = 0; i < (int) postal_info.size(); i++) {
    if (postal_info[i].get_org() == "") {
      org = "";
    } else {
      org = "<contact:org>" + su.esc_xml_markup(postal_info[i].get_org()) +
	"</contact:org>";
    }
    if (postal_info[i].get_str2() == "") {
      str2 = "";
    } else {
      str2 = "<contact:street>" + su.esc_xml_markup(postal_info[i].get_str2()) +
	"</contact:street>";
    }
    if (postal_info[i].get_str3() == "") {
      str3 = "";
    } else {
      str3 = "<contact:street>" + su.esc_xml_markup(postal_info[i].get_str3()) +
	"</contact:street>";
    }
    if (postal_info[i].get_sp() == "") {
      sp = "";
    } else {
      sp = "<contact:sp>" + su.esc_xml_markup(postal_info[i].get_sp()) +
	"</contact:sp>";
    }
    if (postal_info[i].get_pc() == "") {
      pc = "";
    } else {
      pc = "<contact:pc>" + su.esc_xml_markup(postal_info[i].get_pc()) +
	"</contact:pc>";
    }
    
    pf_str += "<contact:postalInfo type='" +
      su.esc_xml_markup(postal_info[i].get_type()) + "'>" +
      "<contact:name>" + su.esc_xml_markup(postal_info[i].get_name()) +
      "</contact:name>" + org + "<contact:addr><contact:street>" +
      su.esc_xml_markup(postal_info[i].get_str1()) +
      "</contact:street>" + str2 + str3 + "<contact:city>" +
      su.esc_xml_markup(postal_info[i].get_city()) + "</contact:city>" +
      sp + pc + "<contact:cc>" + su.esc_xml_markup(postal_info[i].get_cc()) + 
      "</contact:cc></contact:addr></contact:postalInfo>";
  }

  to_parse["postal_info"] = pf_str;

  CommonData::Phone voice;
  voice = get_command()->get_common_data().get_voice();
  string voice_str;
  if(strcmp(voice.number.c_str(), "") == 0) {
    to_parse["voice"] = "";
  }
  else {
    if(strcmp(voice.ext.c_str(), "") == 0)
      voice_str = "<contact:voice>" + su.esc_xml_markup(voice.number) +
	"</contact:voice>";
    else
      voice_str = "<contact:voice x='" + su.esc_xml_markup(voice.ext) + "'>" + 
	su.esc_xml_markup(voice.number) + "</contact:voice>";
    to_parse["voice"] = voice_str;
  }

  CommonData::Phone fax;
  fax = get_command()->get_common_data().get_fax();
  string fax_str;
  if(strcmp(fax.number.c_str(), "") == 0) {
    to_parse["fax"] = "";
  }
  else {
    if(strcmp(fax.ext.c_str(), "") == 0)
      fax_str = "<contact:fax>" + su.esc_xml_markup(fax.number) +
	"</contact:fax>";
    else
      fax_str = "<contact:fax x='" + su.esc_xml_markup(fax.ext) + "'>" + 
	su.esc_xml_markup(fax.number) + "</contact:fax>";
    to_parse["fax"] = fax_str;
  }

  to_parse["email"] =
    su.esc_xml_markup(get_command()->get_common_data().get_email());

  AuthInfo authInfo;
  authInfo = get_command()->get_authInfo();
  string auth_info_str;
  auth_info_str = "<contact:authInfo>";
  if (strcmp(authInfo.get_roid().c_str(), "") == 0) {
    auth_info_str += "<contact:pw>" + su.esc_xml_markup(authInfo.get_pw()) +
      "</contact:pw>";
  } else {
    auth_info_str += "<contact:pw roid='" +
      su.esc_xml_markup(authInfo.get_roid()) + "'>" + 
      su.esc_xml_markup(authInfo.get_pw()) + "</contact:pw>";
  }
  auth_info_str += "</contact:authInfo>";
  to_parse["auth_info"] = auth_info_str;
  
  CommonData::Disclose disclose = 
    get_command()->get_common_data().get_disclose();

  string disclose_str;
  if(disclose.name_int == true)
    disclose_str += "<contact:name type =\"int\"/>";
  if(disclose.name_loc == true)
    disclose_str += "<contact:name type=\"loc\"/>";
  if(disclose.org_int == true)
    disclose_str += "<contact:org type=\"int\"/>";
  if(disclose.org_loc == true)
    disclose_str += "<contact:org type=\"loc\"/>";
  if(disclose.addr_int == true)
    disclose_str += "<contact:addr type=\"int\"/>";
  if(disclose.addr_loc == true)
    disclose_str += "<contact:addr type=\"loc\"/>";
  if(disclose.voice == true)
    disclose_str += "<contact:voice/>";
  if(disclose.fax == true)
    disclose_str += "<contact:fax/>";
  if(disclose.email == true)
    disclose_str += "<contact:email/>";

  string disclose_total("");
  if(strcmp(disclose_str.c_str(), "") != 0) {
    disclose_total = "<contact:disclose flag='" + 
      StrUtil::to_string("%d", disclose.flag) + "'>" + 
      disclose_str + "</contact:disclose>";
  }
  
  to_parse["disclose"] = disclose_total;
  
  _xml = StrUtil::parse(_xml, to_parse, "$(", ")$");
}

LIBEPP_NICBR_NS_END
