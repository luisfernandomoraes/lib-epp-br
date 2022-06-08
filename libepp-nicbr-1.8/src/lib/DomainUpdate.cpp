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
/* $Id: DomainUpdate.cpp 889 2007-03-07 13:46:58Z eduardo $ */

#include "config.h"

#include "DomainUpdate.H"
#include "StrUtil.H"

LIBEPP_NICBR_NS_BEGIN

void DomainUpdate::set_xml_template(const string &xml_template)
{
  StrUtil su;
  Action::set_xml_template_common(xml_template);
  map < string, string, less<string> > to_parse;

  //name
  to_parse["name"] = su.esc_xml_markup(get_command()->get_name());

  //aux containers and iterators
  vector<struct NameServer> ns_cnt;
  vector<struct NameServer>::const_iterator it_ns;
  set<NSIPAddr>::const_iterator it_ip;
  map< string, string, less<string> > contact_cnt;
  map< string, string, less<string> >::const_iterator it_contact;
  set<struct DomainUpdateCmd::Status> status_cnt;
  set<struct DomainUpdateCmd::Status>::const_iterator it_status;

  //stuff to add
  ns_cnt = get_command()->get_nameserver_add();
  contact_cnt = get_command()->get_contact_add();
  status_cnt = get_command()->get_status_add();

  to_parse["add"] = "";
  if (ns_cnt.size() > 0 ||
      contact_cnt.size() > 0 ||
      status_cnt.size() > 0) {
    
    to_parse["add"] = "<domain:add>";
    
    //nameservers to add
    if (ns_cnt.size() > 0) {
      to_parse["add"] += "<domain:ns>";
      for (it_ns = ns_cnt.begin(); it_ns != ns_cnt.end(); 
	   it_ns++) {
	to_parse["add"] += "<domain:hostAttr>";
	to_parse["add"] += "<domain:hostName>" + 
	  su.esc_xml_markup(it_ns->name) + "</domain:hostName>";
	for (it_ip = it_ns->ips.begin(); it_ip != it_ns->ips.end(); 
	     it_ip++) {
	  to_parse["add"] += "<domain:hostAddr";
	  if ((*it_ip).version != "") {
	    to_parse["add"] += " ip='" + su.esc_xml_markup((*it_ip).version) +
	      "'";
	  }
	  to_parse["add"] += ">" + su.esc_xml_markup((*it_ip).addr) +
	    "</domain:hostAddr>";
	}
	to_parse["add"] += "</domain:hostAttr>";
      }
      to_parse["add"] += "</domain:ns>";
    }
    
    //contacts to add
    if (contact_cnt.size() > 0) {
      for (it_contact = contact_cnt.begin(); 
	   it_contact != contact_cnt.end(); it_contact++) {
	to_parse["add"] += "<domain:contact type='" + 
	  su.esc_xml_markup((*it_contact).first) + "'>" +
	  su.esc_xml_markup((*it_contact).second) + 
	  "</domain:contact>";
      }
    }
    
    //status to add
    if (status_cnt.size() > 0) {
      for (it_status = status_cnt.begin(); it_status != status_cnt.end();
	   it_status++) {
	to_parse["add"] += "<domain:status s='" + 
	  su.esc_xml_markup((*it_status).s) + "' lang='" +
	  su.esc_xml_markup((*it_status).lang) + "'>" + 
	  su.esc_xml_markup((*it_status).msg) + "</domain:status>";
      }
    }
    
    to_parse["add"] += "</domain:add>";
  }
  
  //stuff to remove
  ns_cnt = get_command()->get_nameserver_rem();
  contact_cnt = get_command()->get_contact_rem();
  status_cnt = get_command()->get_status_rem();
  
  to_parse["rem"] = "";
  if (ns_cnt.size() > 0 ||
      contact_cnt.size() > 0 ||
      status_cnt.size() > 0) {
    
    to_parse["rem"] = "<domain:rem>";
    
    //nameservers to remove
    if (ns_cnt.size() > 0) {
      to_parse["rem"] += "<domain:ns>";
      for (it_ns = ns_cnt.begin(); it_ns != ns_cnt.end(); 
	   it_ns++) {
	to_parse["rem"] += "<domain:hostAttr>";
	to_parse["rem"] += "<domain:hostName>" +
	  su.esc_xml_markup(it_ns->name) + "</domain:hostName>";
	for (it_ip = it_ns->ips.begin(); it_ip != it_ns->ips.end(); 
	     it_ip++) {
	  to_parse["rem"] += "<domain:hostAddr";
	  if ((*it_ip).version != "") {
	    to_parse["rem"] += " ip='" + su.esc_xml_markup((*it_ip).version) +
	      "'";
	  }
	  to_parse["rem"] += ">" + su.esc_xml_markup((*it_ip).addr) +
	    "</domain:hostAddr>";
	}
	to_parse["rem"] += "</domain:hostAttr>";
      }
      to_parse["rem"] += "</domain:ns>";
    }

    //contacts to remove
    if (contact_cnt.size() > 0) {
      for (it_contact = contact_cnt.begin();
	   it_contact != contact_cnt.end(); it_contact++) {
	to_parse["rem"] += "<domain:contact type='" + 
	  su.esc_xml_markup((*it_contact).first) + "'>" +
	  su.esc_xml_markup((*it_contact).second) + 
	  "</domain:contact>";
      }
    }
    
    //status to remove
    if (status_cnt.size() > 0) {
      for (it_status = status_cnt.begin(); it_status != status_cnt.end();
	   it_status++) {
	to_parse["rem"] += "<domain:status s='" +
	  su.esc_xml_markup((*it_status).s) + "'/>";
      }
    }

    to_parse["rem"] += "</domain:rem>";
  }
  
  //stuff to change
  to_parse["chg"] = "";
  bool registrant_f = get_command()->get_registrant_f();
  AuthInfo authInfo = get_command()->get_authInfo();
  
  if (registrant_f || authInfo.get_pw_f() || authInfo.get_roid_f()) {
    //registrant
    string registrant_str = su.esc_xml_markup(get_command()->get_registrant());
    
    if (registrant_f) {
      registrant_str = "<domain:registrant>" + registrant_str
	+ "</domain:registrant>";
    }
    
    //authInfo
    string auth_info_str = "";

    if (authInfo.get_pw_f()) {
      auth_info_str = "<domain:authInfo>";
      
      if (authInfo.get_roid_f() == false) {
	auth_info_str += "<domain:pw>" + su.esc_xml_markup(authInfo.get_pw()) +
	  "</domain:pw>";
      } else {
	auth_info_str += "<domain:pw roid='" + 
	  su.esc_xml_markup(authInfo.get_roid()) + "'>" + 
	  su.esc_xml_markup(authInfo.get_pw()) + "</domain:pw>";
      }
      auth_info_str += "</domain:authInfo>";
    }

    to_parse["chg"] = "<domain:chg>" + registrant_str + auth_info_str + 
      "</domain:chg>";
  }

  to_parse["ext_begin"] = "";
  to_parse["ext_end"] = "";
  to_parse["ds_ext"] = "";

  /////////////////////////////////////////////////////////////////////////////
  // DS Extension Mapping - RFC4310
  if (get_command()->has_extension()) {
    to_parse["ext_begin"] = "<extension>";
    to_parse["ext_end"] = "</extension>";

    // DNSSEC Extension
    if (get_command()->has_ds_extension()) {
      to_parse["ds_ext"] += "<secDNS:update ";
      if (get_command()->isUrgent()) {
	to_parse["ds_ext"] += "urgent=\"1\" ";
      }
      to_parse["ds_ext"] +=
	"xmlns:secDNS=\"urn:ietf:params:xml:ns:secDNS-1.0\" "
	"xsi:schemaLocation=\"urn:ietf:params:xml:ns:secDNS-1.0 "
	"secDNS-1.0.xsd\">";      

      list<DSInfo> ds_list;
      list<DSInfo>::iterator ds_it;
    
      // DS Add List
      ds_list = get_command()->get_ds_add();
      if (ds_list.size() > 0) {
	to_parse["ds_ext"] += "<secDNS:add>";
	for (ds_it = ds_list.begin(); ds_it != ds_list.end(); ds_it++) {
	  to_parse["ds_ext"] += ds_it->get_xml_format();
	}
	to_parse["ds_ext"] += "</secDNS:add>";
      }

      // DS Rem list
      list<unsigned int> rem_ds_list = get_command()->get_ds_rem();
      list<unsigned int>::iterator rem_it;
      if (rem_ds_list.size() > 0) {
	to_parse["ds_ext"] += "<secDNS:rem>";
	for (rem_it = rem_ds_list.begin(); rem_it != rem_ds_list.end(); 
	     rem_it++) {
	  to_parse["ds_ext"] += "<secDNS:keyTag>" + 
	    su.to_string("%u", (*rem_it)) + "</secDNS:keyTag>";
	}
	to_parse["ds_ext"] += "</secDNS:rem>";
      }

      // DS Change List
      ds_list = get_command()->get_ds_chg();
      if (ds_list.size() > 0) {
	to_parse["ds_ext"] += "<secDNS:chg>";
	for (ds_it = ds_list.begin(); ds_it != ds_list.end(); ds_it++) {
	  to_parse["ds_ext"] += ds_it->get_xml_format();
	}
	to_parse["ds_ext"] += "</secDNS:chg>";
      }

      to_parse["ds_ext"] += "</secDNS:update>";
    }
  }
    
  _xml = StrUtil::parse(_xml, to_parse, "$(", ")$");
}

LIBEPP_NICBR_NS_END
