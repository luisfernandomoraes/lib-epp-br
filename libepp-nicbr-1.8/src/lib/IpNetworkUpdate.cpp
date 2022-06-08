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
/* $Id: IpNetworkUpdate.cpp 1010 2009-03-12 17:50:50Z eduardo $ */
#if USE_IP_MANAGEMENT
#include "IpNetworkUpdate.H"
#include "StrUtil.H"

LIBEPP_NICBR_NS_BEGIN

void IpNetworkUpdate::set_xml_template(const string &xml_template)
{
  StrUtil su;
  Action::set_xml_template_common(xml_template);
  map < string, string, less<string> > to_parse;
  
  // Roid
  to_parse["roid"] = su.esc_xml_markup(get_command()->get_roid());

  // ipnetwork:chg (Organization and Allocation Type)
  to_parse["ipnetwork_chg"] = "";
  if (get_command()->get_organization() != "" || 
      get_command()->get_allocType() != "") {
    to_parse["ipnetwork_chg"] = "<ipnetwork:chg>";
    if (get_command()->get_organization() != "") {
      to_parse["ipnetwork_chg"] += "<ipnetwork:organization>" +
	su.esc_xml_markup(get_command()->get_organization()) +
	"</ipnetwork:organization>";
    }
    if (get_command()->get_allocType() != "") {
      to_parse["ipnetwork_chg"] += "<ipnetwork:allocType>" +
	su.esc_xml_markup(get_command()->get_allocType()) +
	"</ipnetwork:allocType>";
    }
    to_parse["ipnetwork_chg"] += "</ipnetwork:chg>";
  }

  // ipnetwork:add (Contacts and Reverse DNS)
  to_parse["ipnetwork_add"] = "";
  vector<ReverseDns> reverseDns_add = get_command()->get_reverseDns_add();
  map< string, string, less<string> > contacts_add =
    get_command()->get_contacts_add();
  if (!contacts_add.empty() || !reverseDns_add.empty()) {
    map< string, string, less<string> >::const_iterator it_contacts;
    to_parse["ipnetwork_add"] = "<ipnetwork:add>";

    for (it_contacts = contacts_add.begin();
	 it_contacts != contacts_add.end();
	 it_contacts++) {
      to_parse["ipnetwork_add"] +=
	"<ipnetwork:contact type='" + su.esc_xml_markup((*it_contacts).first) +
	"'>" + su.esc_xml_markup((*it_contacts).second) + "</ipnetwork:contact>";
    }

    for (unsigned int i = 0; i < reverseDns_add.size(); i++) {
      IpRange ipRange = reverseDns_add[i].ipRange;
      
      to_parse["ipnetwork_add"] += "<ipnetwork:reverseDNS>";
      
      if (ipRange.get_version() != "") {
	to_parse["ipnetwork_add"] += "<ipnetwork:ipRange version='" + 
	  su.esc_xml_markup(ipRange.get_version()) + "'>";
      } else {
	to_parse["ipnetwork_add"] += "<ipnetwork:ipRange>";
      }
      
      to_parse["ipnetwork_add"] += "<ipnetwork:startAddress>" + 
	su.esc_xml_markup(ipRange.get_ipBegin()) + "</ipnetwork:startAddress>";
      to_parse["ipnetwork_add"] += "<ipnetwork:endAddress>" + 
	su.esc_xml_markup(ipRange.get_ipEnd()) + "</ipnetwork:endAddress>";
      to_parse["ipnetwork_add"] += "</ipnetwork:ipRange>";
      
      vector<string> nameservers = reverseDns_add[i].nameservers;
      int nameserversSize = nameservers.size();
      
      for (int j = 0; j < nameserversSize; j++) {
	to_parse["ipnetwork_add"] += "<ipnetwork:hostName>" + 
	  su.esc_xml_markup(nameservers[j]) + "</ipnetwork:hostName>";
      }
      
      to_parse["ipnetwork_add"] += "</ipnetwork:reverseDNS>";
    }
    
    to_parse["ipnetwork_add"] += "</ipnetwork:add>";
  }

  // ipnetwork:rem (Contacts and Reverse DNS)
  to_parse["ipnetwork_rem"] = "";
  vector<ReverseDns> reverseDns_rem = get_command()->get_reverseDns_rem();
  map< string, string, less<string> > contacts_rem =
    get_command()->get_contacts_rem();
  if (!contacts_rem.empty() || !reverseDns_rem.empty()) {
    map< string, string, less<string> >::const_iterator it_contacts;
    to_parse["ipnetwork_rem"] = "<ipnetwork:rem>";

    for (it_contacts = contacts_rem.begin();
	 it_contacts != contacts_rem.end();
	 it_contacts++) {
      to_parse["ipnetwork_rem"] +=
	"<ipnetwork:contact type='" + su.esc_xml_markup((*it_contacts).first) +
	"'>" + su.esc_xml_markup((*it_contacts).second) + "</ipnetwork:contact>";
    }
    
    for (unsigned int i = 0; i < reverseDns_rem.size(); i++) {
      IpRange ipRange = reverseDns_rem[i].ipRange;
      
      to_parse["ipnetwork_rem"] += "<ipnetwork:reverseDNS>";
      
      if (ipRange.get_version() != "") {
	to_parse["ipnetwork_rem"] += "<ipnetwork:ipRange version='" + 
	  su.esc_xml_markup(ipRange.get_version()) + "'>";
      } else {
	to_parse["ipnetwork_rem"] += "<ipnetwork:ipRange>";
      }
      
      to_parse["ipnetwork_rem"] += "<ipnetwork:startAddress>" + 
	su.esc_xml_markup(ipRange.get_ipBegin()) + "</ipnetwork:startAddress>";
      to_parse["ipnetwork_rem"] += "<ipnetwork:endAddress>" + 
	su.esc_xml_markup(ipRange.get_ipEnd()) + "</ipnetwork:endAddress>";
      to_parse["ipnetwork_rem"] += "</ipnetwork:ipRange>";
      
      vector<string> nameservers = reverseDns_rem[i].nameservers;
      int nameserversSize = nameservers.size();
      
      for (int j = 0; j < nameserversSize; j++) {
	to_parse["ipnetwork_rem"] += "<ipnetwork:hostName>" + 
	  su.esc_xml_markup(nameservers[j]) + "</ipnetwork:hostName>";
      }
      
      to_parse["ipnetwork_rem"] += "</ipnetwork:reverseDNS>";
    }

    to_parse["ipnetwork_rem"] += "</ipnetwork:rem>";
  }

  _xml = StrUtil::parse(_xml, to_parse, "$(", ")$");
}

LIBEPP_NICBR_NS_END
#endif //USE_IP_MANAGEMENT
