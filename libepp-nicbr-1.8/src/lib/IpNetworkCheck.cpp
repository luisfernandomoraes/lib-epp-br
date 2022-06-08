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
/* $Id: IpNetworkCheck.cpp 963 2008-12-11 15:31:29Z rafael $ */
#if USE_IP_MANAGEMENT
#include "IpNetworkCheck.H"
#include "StrUtil.H"

LIBEPP_NICBR_NS_BEGIN

void IpNetworkCheck::set_xml_template(const string &xml_template)
{
  StrUtil su;
  Action::set_xml_template_common(xml_template);
  map < string, string, less<string> > to_parse;

  // Ip Ranges
  vector<IpRange> ipRanges = get_command()->get_ipRanges();
  int ipRangesSize = ipRanges.size();

  to_parse["ipRanges"] = "";
  for (int i = 0; i < ipRangesSize; i++) {
    IpRange ipRange = ipRanges[i];

    if (ipRange.get_version() != "") {
      to_parse["ipRanges"] += "<ipnetwork:ipRange version='" + 
	su.esc_xml_markup(ipRange.get_version()) + "'>";
    } else {
      to_parse["ipRanges"] += "<ipnetwork:ipRange>";
    }

    to_parse["ipRanges"] += "<ipnetwork:startAddress>" + 
      su.esc_xml_markup(ipRange.get_ipBegin()) + "</ipnetwork:startAddress>";
    to_parse["ipRanges"] += "<ipnetwork:endAddress>" + 
      su.esc_xml_markup(ipRange.get_ipEnd()) + "</ipnetwork:endAddress>";
    to_parse["ipRanges"] += "</ipnetwork:ipRange>";
  }
  
  _xml = StrUtil::parse(_xml, to_parse, "$(", ")$");
}

LIBEPP_NICBR_NS_END
#endif //USE_IP_MANAGEMENT
