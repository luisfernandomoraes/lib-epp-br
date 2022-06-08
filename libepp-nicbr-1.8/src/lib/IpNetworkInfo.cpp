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
/* $Id: IpNetworkInfo.cpp 963 2008-12-11 15:31:29Z rafael $ */
#if USE_IP_MANAGEMENT
#include "IpNetworkInfo.H"
#include "StrUtil.H"

LIBEPP_NICBR_NS_BEGIN

void IpNetworkInfo::set_xml_template(const string &xml_template)
{
  StrUtil su;
  Action::set_xml_template_common(xml_template);
  map < string, string, less<string> > to_parse;

  // Ip Range
  string ipRangeVersion = get_command()->get_ipRange().get_version();
  to_parse["ipRange.version"] = "";
  if (ipRangeVersion != "") {
    to_parse["ipRange.version"] = "version='" +
      su.esc_xml_markup(get_command()->get_ipRange().get_version()) + "'";
  }

  to_parse["ipRange.begin"] = 
    su.esc_xml_markup(get_command()->get_ipRange().get_ipBegin());
  to_parse["ipRange.end"] = 
    su.esc_xml_markup(get_command()->get_ipRange().get_ipEnd());

  // Roid
  to_parse["roid"] = su.esc_xml_markup(get_command()->get_roid());

  _xml = StrUtil::parse(_xml, to_parse, "$(", ")$");
}

LIBEPP_NICBR_NS_END
#endif //USE_IP_MANAGEMENT
