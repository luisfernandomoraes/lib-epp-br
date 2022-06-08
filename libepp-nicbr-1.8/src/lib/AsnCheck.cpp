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
/* $Id: AsnCheck.cpp 1010 2009-03-12 17:50:50Z eduardo $ */
#if USE_IP_MANAGEMENT
#include "AsnCheck.H"
#include "StrUtil.H"

LIBEPP_NICBR_NS_BEGIN

void AsnCheck::set_xml_template(const string &xml_template)
{
  Action::set_xml_template_common(xml_template);
  map < string, string, less<string> > to_parse;

  // Asn
  string asn_list_xml("");

  list<int> asn_list = get_command()->get_asn_list();
  list<int>::const_iterator asn;
  for (asn = asn_list.begin(); asn != asn_list.end(); asn++) {
    asn_list_xml += "<asn:number>" + StrUtil::to_string("%d", *asn) +
      "</asn:number>";
  }

  to_parse["asn_list"] = asn_list_xml;

  _xml = StrUtil::parse(_xml, to_parse, "$(", ")$");
}

LIBEPP_NICBR_NS_END
#endif //USE_IP_MANAGEMENT
