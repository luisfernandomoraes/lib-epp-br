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
/* $Id: BrDomainUpdate.cpp 961 2008-12-11 15:11:25Z eduardo $ */
#if USE_BR_DOMAINS
#include "BrDomainUpdate.H"
#include "StrUtil.H"
#include "libepp_nicbr.H"

LIBEPP_NICBR_NS_BEGIN

void BrDomainUpdate::set_xml_template(const string &xml_template)
{
  string local_template = xml_template;
  map < string, string, less<string> > to_parse;
  if (get_command()->has_extension()) {
    to_parse["ext_begin"] = "<extension>";
    to_parse["ext_end"] = "</extension>";
    local_template = StrUtil::parse(local_template, to_parse, "$(", ")$");
  }

  DomainUpdate::set_xml_template(local_template);

  bool has_auto_renew = false;
  int auto_renew = get_command()->get_auto_renew();
  if (auto_renew == 0 || auto_renew == 1)
    has_auto_renew = true;

  to_parse["br_ext"] = "";
  if (get_command()->has_br_extension() == true) {
    to_parse["br_ext"] += "<brdomain:update "
      "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' "
      "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
      "brdomain-1.0.xsd'>";

    // ReleaseProcessFlags
    bool has_rpf = false;
    ReleaseProcessFlags rpf = get_command()->get_releaseProcessFlags();
    if (get_command()->get_ticketNumber() > 0) {
      to_parse["br_ext"] +=
	"<brdomain:ticketNumber>" +
	StrUtil::to_string("%d", get_command()->get_ticketNumber()) +
	"</brdomain:ticketNumber>";

      // ReleaseProcessFlags exist only with tickets
      if (rpf.flag1 != -1 || rpf.flag2 != -1 || rpf.flag3 != -1)
	has_rpf = true;
    }

    if (has_rpf || has_auto_renew) {
      to_parse["br_ext"] += "<brdomain:chg>";
      
      if (has_rpf) {
	to_parse["br_ext"] += "<brdomain:releaseProcessFlags";
	if (rpf.flag1 != -1) {
	  to_parse["br_ext"] += " flag1='";
	  to_parse["br_ext"] += rpf.flag1 == 0 ? "0'" : "1'";
	}
	if (rpf.flag2 != -1) {
	  to_parse["br_ext"] += " flag2='";
	  to_parse["br_ext"] += rpf.flag2 == 0 ? "0'" : "1'";
	}
	if (rpf.flag3 != -1) {
	  to_parse["br_ext"] += " flag3='";
	  to_parse["br_ext"] += rpf.flag3 == 0 ? "0'" : "1'";
	}
	to_parse["br_ext"] += "/>";
      }

      if (has_auto_renew) {
	to_parse["br_ext"] += "<brdomain:autoRenew active=\"" + 
	  StrUtil::to_string("%d", auto_renew) + "\"/>";
      }
    
      to_parse["br_ext"] += "</brdomain:chg>";
    }
    
    to_parse["br_ext"] += "</brdomain:update>";
  }

  _xml = StrUtil::parse(_xml, to_parse, "$(", ")$");
}

LIBEPP_NICBR_NS_END
#endif //USE_BR_DOMAINS
