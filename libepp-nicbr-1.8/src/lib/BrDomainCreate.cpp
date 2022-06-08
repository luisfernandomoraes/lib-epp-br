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
/* $Id: BrDomainCreate.cpp 961 2008-12-11 15:11:25Z eduardo $ */
#if USE_BR_DOMAINS
#include "BrDomainCreate.H"
#include "StrUtil.H"
#include "libepp_nicbr.H"

LIBEPP_NICBR_NS_BEGIN

void BrDomainCreate::set_xml_template(const string &xml_template)
{
  map < string, string, less<string> > to_parse;
  to_parse["ext_begin"] = "<extension>";
  to_parse["ext_end"] = "</extension>";

  string local_template = StrUtil::parse(xml_template, to_parse, "$(", ")$");
  DomainCreate::set_xml_template(local_template);

  to_parse["br_ext"] = "";
  to_parse["br_ext"] +=  "<brdomain:create "
    "xmlns:brdomain='urn:ietf:params:xml:ns:brdomain-1.0' "
    "xsi:schemaLocation='urn:ietf:params:xml:ns:brdomain-1.0 "
    "brdomain-1.0.xsd'>";

  // organization
  to_parse["br_ext"] += "<brdomain:organization>" + 
    StrUtil::esc_xml_markup(get_command()->get_organization()) + 
    "</brdomain:organization>";
    
  // releaseProcessFlags
  ReleaseProcessFlags rpf = get_command()->get_releaseProcessFlags();
  if (rpf.flag1 == 1 || rpf.flag2 == 1 || rpf.flag3 == 1) {
    to_parse["br_ext"] += "<brdomain:releaseProcessFlags";
      
    if (rpf.flag1 == 1) {
      to_parse["br_ext"] += " flag1='1'";
    }
      
    if (rpf.flag2 == 1) {
      to_parse["br_ext"] += " flag2='1'";
    }
      
    if (rpf.flag3 == 1) {
      to_parse["br_ext"] += " flag3='1'";
    }
    to_parse["br_ext"] += "/>";
  }
  
  int auto_renew = get_command()->get_auto_renew();
  if(auto_renew == 0 || auto_renew == 1)
    to_parse["br_ext"] += "<brdomain:autoRenew active=\"" + 
      StrUtil::to_string("%d", auto_renew) + "\"/>";

  to_parse["br_ext"] += "</brdomain:create>";
  
  _xml = StrUtil::parse(_xml, to_parse, "$(", ")$");
}

LIBEPP_NICBR_NS_END
#endif //USE_BR_DOMAINS
