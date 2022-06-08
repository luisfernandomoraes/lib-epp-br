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
/* $Id: AsnUpdate.cpp 986 2008-12-30 12:32:38Z eduardo $ */
#if USE_IP_MANAGEMENT
#include "AsnUpdate.H"
#include "StrUtil.H"

LIBEPP_NICBR_NS_BEGIN

void AsnUpdate::set_xml_template(const string &xml_template)
{
  StrUtil su;
  Action::set_xml_template_common(xml_template);
  map < string, string, less<string> > to_parse;

  // ASN
  to_parse["asn"] = StrUtil::to_string("%d", get_command()->get_asn());

  // asn:chg (Organization)
  to_parse["asn_chg"] = "";
  if (get_command()->get_organization() != "") {
    to_parse["asn_chg"] =
      "<asn:chg>"
      "<asn:organization>" +
      su.esc_xml_markup(get_command()->get_organization()) +
      "</asn:organization>"
      "</asn:chg>";
  }

  // asn:add (Contacts)
  to_parse["asn_add"] = "";
  map< string, string, less<string> > contacts_add =
    get_command()->get_contacts_add();
  if (!contacts_add.empty()) {
    map< string, string, less<string> >::const_iterator it_contacts;
    to_parse["asn_add"] = "<asn:add>";
    for (it_contacts = contacts_add.begin();
	 it_contacts != contacts_add.end();
	 it_contacts++) {
      to_parse["asn_add"] +=
	"<asn:contact type='" + su.esc_xml_markup((*it_contacts).first) +
	"'>" + su.esc_xml_markup((*it_contacts).second) + "</asn:contact>";
    }
    to_parse["asn_add"] += "</asn:add>";
  }

  // asn:rem (Contacts)
  to_parse["asn_rem"] = "";
  map< string, string, less<string> > contacts_rem =
    get_command()->get_contacts_rem();
  if (!contacts_rem.empty()) {
    map< string, string, less<string> >::const_iterator it_contacts;
    to_parse["asn_rem"] = "<asn:rem>";
    for (it_contacts = contacts_rem.begin();
	 it_contacts != contacts_rem.end();
	 it_contacts++) {
      to_parse["asn_rem"] +=
	"<asn:contact type='" + su.esc_xml_markup((*it_contacts).first) +
	"'>" + su.esc_xml_markup((*it_contacts).second) + "</asn:contact>";
    }
    to_parse["asn_rem"] += "</asn:rem>";
  }

  _xml = StrUtil::parse(_xml, to_parse, "$(", ")$");
}

LIBEPP_NICBR_NS_END
#endif //USE_IP_MANAGEMENT
