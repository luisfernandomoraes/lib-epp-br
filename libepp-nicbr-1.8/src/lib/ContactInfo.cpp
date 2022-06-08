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
/* $Id: ContactInfo.cpp 756 2006-06-09 17:42:34Z koji $ */

#include <set>

#include "config.h"

#include "ContactInfo.H"
#include "StrUtil.H"

using std::set;

LIBEPP_NICBR_NS_BEGIN

void ContactInfo::set_xml_template(const string &xml_template)
{
  StrUtil su;
  Action::set_xml_template_common(xml_template);
  map < string, string, less<string> > to_parse;
  
  to_parse["id"] = su.esc_xml_markup(get_command()->get_id());
  AuthInfo authInfo;
  authInfo = get_command()->get_authInfo();
  string auth_info_str = "";
  
  if (authInfo.get_pw() != "") {
    auth_info_str = "<contact:authInfo>";
    if (authInfo.get_roid() == "") {
      auth_info_str += "<contact:pw>" + su.esc_xml_markup(authInfo.get_pw()) +
	"</contact:pw>";
    } else {
      auth_info_str += "<contact:pw roid='" + 
	su.esc_xml_markup(authInfo.get_roid()) + "'>" + 
	su.esc_xml_markup(authInfo.get_pw()) + "</contact:pw>";
    }
    auth_info_str += "</contact:authInfo>";    
  }

  to_parse["auth_info"] = auth_info_str;
  
  _xml = StrUtil::parse(_xml, to_parse, "$(", ")$");
}

LIBEPP_NICBR_NS_END
