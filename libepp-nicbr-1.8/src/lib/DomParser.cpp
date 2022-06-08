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
/* $Id: DomParser.cpp 1021 2009-05-11 18:56:22Z eduardo $ */

#include "config.h"
#include <iostream>

#include <map>

#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>

#include "DomParser.H"
#include "DomErrorHandler.H"
#include "XmlException.H"
#include "StrUtil.H"

using std::map;

XERCES_CPP_NAMESPACE_USE

LIBEPP_NICBR_NS_BEGIN

void DomParser::parse_command(const string &xml_payload)
{
  string utf8_xml_payload;
  StrUtil::iso88591_to_utf8(xml_payload, utf8_xml_payload);
  parse(utf8_xml_payload);
}

void DomParser::parse_login_rsp(const string &xml_payload, Response *rsp)
{
  parse_response(xml_payload, rsp);
}

void DomParser::parse_logout_rsp(const string &xml_payload, Response *rsp)
{
  parse_response(xml_payload, rsp);
}

void DomParser::parse_greeting(const string &xml_payload, Greeting *greeting)
{
  DOMNode *n = parse(xml_payload);

  fill_greeting(n, greeting);
}

void DomParser::parse_contact_check_rsp(const string &xml_payload, 
					ContactCheckRsp *rsp)
{
  DOMNode *n = parse(xml_payload);

  fill_contact_check_rsp(n, rsp);
}

void DomParser::parse_contact_create_rsp(const string &xml_payload,
					 ContactCreateRsp *rsp)
{
  DOMNode *n = parse(xml_payload);

  fill_contact_create_rsp(n, rsp);
}

void DomParser::parse_contact_info_rsp(const string &xml_payload,
				       ContactInfoRsp *rsp)
{
  DOMNode *n = parse(xml_payload);

  fill_contact_info_rsp(n, rsp);
}

void DomParser::parse_contact_update_rsp(const string &xml_payload,
					 Response *rsp)
{
  return parse_response(xml_payload, rsp);
}

void DomParser::parse_domain_check_rsp(const string &xml_payload,
				       DomainCheckRsp *rsp)
{
  DOMNode *n = parse(xml_payload);

  fill_domain_check_rsp(n, rsp);
}

void DomParser::parse_domain_create_rsp(const string &xml_payload,
					DomainCreateRsp *rsp)
{
  DOMNode *n = parse(xml_payload);

  fill_domain_create_rsp(n, rsp);
}

void DomParser::parse_domain_info_rsp(const string &xml_payload, 
				      DomainInfoRsp *rsp)
{
  DOMNode *n = parse(xml_payload);

  fill_domain_info_rsp(n, rsp);
}

void DomParser::parse_domain_renew_rsp(const string &xml_payload,
				       DomainRenewRsp *rsp)
{
  DOMNode *n = parse(xml_payload);

  fill_domain_renew_rsp(n, rsp);
}

void DomParser::parse_domain_update_rsp(const string &xml_payload,
					Response *rsp)
{
  return parse_response(xml_payload, rsp);
}

void DomParser::parse_domain_delete_rsp(const string &xml_payload, 
					Response *rsp)
{
  return parse_response(xml_payload, rsp);
}

#if USE_IP_MANAGEMENT
void DomParser::parse_ipnetwork_create_rsp(const string &xml_payload, 
					   IpNetworkCreateRsp *rsp)
{
  DOMNode *n = parse(xml_payload);

  fill_ipnetwork_create_rsp(n, rsp);
}

void DomParser::parse_ipnetwork_check_rsp(const string &xml_payload, 
					  IpNetworkCheckRsp *rsp)
{
  DOMNode *n = parse(xml_payload);

  fill_ipnetwork_check_rsp(n, rsp);
}

void DomParser::parse_ipnetwork_delete_rsp(const string &xml_payload, 
					   Response *rsp)
{
  return parse_response(xml_payload, rsp);
}

void DomParser::parse_ipnetwork_renew_rsp(const string &xml_payload, 
					  IpNetworkRenewRsp *rsp)
{
  DOMNode *n = parse(xml_payload);

  fill_ipnetwork_renew_rsp(n, rsp);
}

void DomParser::parse_ipnetwork_transfer_rsp(const string &xml_payload, 
					     IpNetworkTransferRsp *rsp)
{
  DOMNode *n = parse(xml_payload);
  
  fill_ipnetwork_transfer_rsp(n, rsp);
}

void DomParser::parse_ipnetwork_info_rsp(const string &xml_payload, 
					 IpNetworkInfoRsp *rsp)
{
  DOMNode *n = parse(xml_payload);
  
  fill_ipnetwork_info_rsp(n, rsp);
}

void DomParser::parse_ipnetwork_update_rsp(const string &xml_payload, 
					   Response *rsp)
{
  return parse_response(xml_payload, rsp);
}

void DomParser::parse_asn_check_rsp(const string &xml_payload, AsnCheckRsp *rsp)
{
  DOMNode *n = parse(xml_payload);
  
  fill_asn_check_rsp(n, rsp);
}

void DomParser::parse_asn_create_rsp(const string &xml_payload, 
				     AsnCreateRsp *rsp)
{
  DOMNode *n = parse(xml_payload);
  
  fill_asn_create_rsp(n, rsp);
}

void DomParser::parse_asn_update_rsp(const string &xml_payload, 
				     Response *rsp)
{
  return parse_response(xml_payload, rsp);
}

void DomParser::parse_asn_delete_rsp(const string &xml_payload, Response *rsp)
{
  return parse_response(xml_payload, rsp);
}

void DomParser::parse_asn_renew_rsp(const string &xml_payload, AsnRenewRsp *rsp)
{
  DOMNode *n = parse(xml_payload);
  
  fill_asn_renew_rsp(n, rsp);
}

void DomParser::parse_asn_transfer_rsp(const string &xml_payload, 
				       AsnTransferRsp *rsp)
{
  DOMNode *n = parse(xml_payload);
  
  fill_asn_transfer_rsp(n, rsp);
}

void DomParser::parse_asn_info_rsp(const string &xml_payload, AsnInfoRsp *rsp)
{
  DOMNode *n = parse(xml_payload);
  
  fill_asn_info_rsp(n, rsp);
}
#endif //USE_IP_MANAGEMENT

void DomParser::parse_poll_rsp(const string &xml_payload, PollRsp *rsp)
{
  DOMNode *n = parse(xml_payload);
  
  fill_poll_rsp(n, rsp);
}

void DomParser::parse_response(const string &xml_payload, Response *rsp)
{
  DOMNode *n = parse(xml_payload);
  
  fill_response(n, rsp);
}

void DomParser::get_extValue_info(DOMNode *n, string &value, string &xmlns,
				  string &reason)
{
  DOMNode *child;
  string elem_name;
  for (child = n->getFirstChild(); child != 0;
       child = child->getNextSibling()) {
    if (child->getNodeType() != DOMNode::ELEMENT_NODE) {
      continue;
    }
    elem_name = str_transcode(child->getNodeName());
    if (elem_name == "value") {
      value = str_transcode(child->getTextContent());
      map< string, string, less<string> > attributes = get_attributes(child);
      map< string, string, less<string> >::const_iterator it;
      it = attributes.begin();
      if (it != attributes.end() && it->first.substr(0, 5) == "xmlns") {
	xmlns = it->first + "=\"" + it->second + "\"";
      }
    } else if (elem_name == "reason") {
      reason = str_transcode(child->getTextContent());
    }
  }
}

void DomParser::fill_result(DOMNode *n, Response *rsp)
{
  string msg, lang, value, xmlns, reason, elem_name;
  Response::ResultCode code;

  map< string, string, less<string> > attributes = get_attributes(n);
  code = (Response::ResultCode) atoi(attributes["code"].c_str());
  DOMNode *child;
  for (child = n->getFirstChild(); child != 0;
       child = child->getNextSibling()) {
    if (child->getNodeType() == DOMNode::ELEMENT_NODE) {
      elem_name = str_transcode(child->getNodeName());
      if (elem_name == "msg" && msg == "") {
	attributes = get_attributes(child);
	lang = attributes["lang"];
	msg = str_transcode(child->getTextContent());
	rsp->insert_result(code, msg);
      } else if (elem_name == "value") {
	if (code != Response::UNSET && msg != "") {
	  value = str_transcode(child->getTextContent());
	  map< string, string, less<string> > attributes = get_attributes(n);
	  map< string, string, less<string> >::const_iterator it;
	  it = attributes.begin();
	  xmlns = "";
	  if (it != attributes.end() && it->first.substr(0, 5) == "xmlns") {
	    xmlns = it->first + "=\"" + it->second + "\"";
	  }
	  if (value != "" && xmlns != "") {
	    rsp->insert_result(code, msg, value, xmlns);
	  }
	}
      } else if (elem_name == "extValue" && msg != "") {
	value = xmlns = reason = "";
	get_extValue_info(child, value, xmlns, reason);
	if (value != "" && xmlns != "") {
	  rsp->insert_result(code, msg, value, xmlns, reason);
	}
      }
    }
  }
}

void DomParser::fill_response(DOMNode *n, Response *rsp)
{  
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, rsp);
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "extension") {
      look_children = true;
#if USE_BR_DOMAINS
    } else if (elem_name == "brdomain:updData") {
      fill_brdomain_update_rsp(n, (BrDomainUpdateRsp *) rsp);
#endif //USE_BR_DOMAINS
    }
  }
  
  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
	 child = child->getNextSibling()) {
      fill_response(child, rsp);
    }    
  }
}

void DomParser::fill_greeting(DOMNode *n, Greeting *greeting)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "greeting") {
      look_children = true;
    } else if (elem_name == "svID") {
      greeting->set_svID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svDate") {
      greeting->set_svDate(str_transcode(n->getTextContent()));
    } else if (elem_name == "svcMenu") {
      look_children = true;
    } else if (elem_name == "version") {
      greeting->set_version(str_transcode(n->getTextContent()));
    } else if (elem_name == "lang") {
      greeting->set_lang(str_transcode(n->getTextContent()));
    } else if (elem_name == "objURI") {
      greeting->set_objURI(str_transcode(n->getTextContent()));
    } else if (elem_name == "svcExtension") {
      look_children = true;
    } else if (elem_name == "extURI") {
      greeting->set_extURI(str_transcode(n->getTextContent()));
    } else if (elem_name == "dcp") {
      look_children = true;
    } else if (elem_name == "access") {
      DOMNode *child;
      for (child = n->getFirstChild(); child != 0;
	   child = child->getNextSibling()) {
	if (child->getNodeType() == DOMNode::ELEMENT_NODE) {
	  string e_name = str_transcode(child->getNodeName());
	  if (e_name == "all") {
	    greeting->set_access(Greeting::ALL);
	  } else if (e_name == "none") {
	    greeting->set_access(Greeting::NONE_AC);
	  } else if (e_name == "null") {
	    greeting->set_access(Greeting::NULL_AC);
	  } else if (e_name == "other") {
	    greeting->set_access(Greeting::OTHER_AC);
	  } else if (e_name == "personal") {
	    greeting->set_access(Greeting::PERSONAL);
	  } else if (e_name == "personalAndOther") {
	    greeting->set_access(Greeting::PERSONAL_AND_OTHER);
	  }
	}
      }
    } else if (elem_name == "statement") {
      look_children = true;
    } else if (elem_name == "purpose") {
      DOMNode *child;
      for (child = n->getFirstChild(); child != 0;
	   child = child->getNextSibling()) {
	if (child->getNodeType() == DOMNode::ELEMENT_NODE) {
	  string e_name = str_transcode(child->getNodeName());
	  if (e_name == "admin") {
	    greeting->set_purpose(Greeting::ADMIN);
	  } else if (e_name == "contact") {
	    greeting->set_purpose(Greeting::CONTACT);
	  } else if (e_name == "other") {
	    greeting->set_purpose(Greeting::OTHER_PR);
	  } else if (e_name == "prov") {
	    greeting->set_purpose(Greeting::PROV);
	  }
	}
      }      
    } else if (elem_name == "recipient") {
      DOMNode *child;
      for (child = n->getFirstChild(); child != 0;
	   child = child->getNextSibling()) {
	if (child->getNodeType() == DOMNode::ELEMENT_NODE) {
	  string e_name = str_transcode(child->getNodeName());
	  if (e_name == "other") {
	    greeting->set_recipient(Greeting::OTHER_RC);
	  } else if (e_name == "ours") {
	    greeting->set_recipient(Greeting::OURS);
	    DOMNode *recDesc = child->getFirstChild();
	    if (recDesc && recDesc->getNodeType() == DOMNode::ELEMENT_NODE) {
	      if (str_transcode(recDesc->getNodeName()) == "recDesc") {
		greeting->set_recDesc(str_transcode(recDesc->getTextContent()));
	      }
	    }
	  } else if (e_name == "public") {
	    greeting->set_recipient(Greeting::PUBLIC);
	  } else if (e_name == "same") {
	    greeting->set_recipient(Greeting::SAME);
	  } else if (e_name == "unrelated") {
	    greeting->set_recipient(Greeting::UNRELATED);
	  }
	}
      }
    } else if (elem_name == "retention") {
      DOMNode *child;
      for (child = n->getFirstChild(); child != 0;
	   child = child->getNextSibling()) {
	if (child->getNodeType() == DOMNode::ELEMENT_NODE) {
	  string e_name = str_transcode(child->getNodeName());
	  if (e_name == "business") {
	    greeting->set_retention(Greeting::BUSINESS);
	  } else if (e_name == "indefinite") {
	    greeting->set_retention(Greeting::INDEFINITE);
	  } else if (e_name == "legal") {
	    greeting->set_retention(Greeting::LEGAL);
	  } else if (e_name == "none") {
	    greeting->set_retention(Greeting::NONE_RT);
	  } else if (e_name == "stated") {
	    greeting->set_retention(Greeting::STATED);
	  }
	}
      }      
    } else if (elem_name == "expiry") {
      look_children = true;
    } else if (elem_name == "absolute") {
      greeting->set_expiry(0, str_transcode(n->getTextContent()));
    } else if (elem_name == "relative") {
      greeting->set_expiry(1, str_transcode(n->getTextContent()));
    }
  }
  
  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
	 child = child->getNextSibling()) {
      fill_greeting(child, greeting);
    }
  }
}

void DomParser::fill_contact_check_rsp(DOMNode *n, ContactCheckRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "resData") {
      look_children = true;
    } else if (elem_name == "contact:chkData") {
      look_children = true;
    } else if (elem_name == "contact:cd") {
      DOMNode *child;
      string id, avail, reason;
      for (child = n->getFirstChild(); child != 0;
	   child = child->getNextSibling()) {
	if (child->getNodeType() != DOMNode::ELEMENT_NODE) {
	  continue;
	}
	string e_name = str_transcode(child->getNodeName());
	if (e_name == "contact:id") {

	  map< string, string, less<string> > attributes =
	    get_attributes(child);
	  if (attributes["avail"] != "") {
	    avail = attributes["avail"];
	  }

	  id = str_transcode(child->getTextContent());
	} else if (e_name == "contact:reason") {
	  reason = str_transcode(child->getTextContent());
	}
      }
      rsp->insert_availability(id, avail, reason);
    } else if (elem_name == "extension") {
      look_children = true;
#if USE_BR_ORG
    } else if (elem_name == "brorg:chkData") {
      //brorg extension
      fill_brorg_check_rsp(n, (BrOrgCheckRsp*) rsp);
#endif //USE_BR_ORG
    }
  }  

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_contact_check_rsp(child, rsp);
    }
  }
}

void DomParser::fill_contact_info_rsp(DOMNode *n, ContactInfoRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    CommonData common = rsp->get_common_data();
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "resData") {
      look_children = true;
    } else if (elem_name == "contact:infData") {
      look_children = true;
    } else if (elem_name == "contact:id") {
      common.set_id(str_transcode(n->getTextContent()));
    } else if (elem_name == "contact:roid") {
      rsp->set_roid(str_transcode(n->getTextContent()));
    } else if (elem_name == "contact:status") {

      map< string, string, less<string> > attributes = get_attributes(n);
      if (attributes["s"] != "") {
	rsp->insert_status(attributes["s"]);
      }
    } else if (elem_name == "contact:postalInfo") {
      PostalInfo pi;
      fill_postal_info(n, &pi);
      
      common.insert_postal_info(pi);
    } else if (elem_name == "contact:voice") {

      struct CommonData::Phone phone;
      map< string, string, less<string> > attributes = get_attributes(n);
      if (attributes["x"] != "") {
	phone.ext = attributes["x"];
      }

      phone.number = str_transcode(n->getTextContent());
      common.set_voice(phone);
    } else if (elem_name == "contact:fax") {

      struct CommonData::Phone phone;
      map< string, string, less<string> > attributes = get_attributes(n);
      if (attributes["x"] != "") {
	phone.ext = attributes["x"];
      }

      phone.number = str_transcode(n->getTextContent());
      common.set_fax(phone);
    } else if (elem_name == "contact:email") {
      string email = str_transcode(n->getTextContent());
      common.set_email(email);
    } else if (elem_name == "contact:clID") {
      rsp->set_clID(str_transcode(n->getTextContent()));
    } else if (elem_name == "contact:crID") {
      rsp->set_crID(str_transcode(n->getTextContent()));
    } else if (elem_name == "contact:crDate") {
      rsp->set_crDate(str_transcode(n->getTextContent()));
    } else if (elem_name == "contact:upID") {
      rsp->set_upID(str_transcode(n->getTextContent()));
    } else if (elem_name == "contact:upDate") {
      rsp->set_upDate(str_transcode(n->getTextContent()));
    } else if (elem_name == "contact:trDate") {
      rsp->set_trDate(str_transcode(n->getTextContent()));
    } else if (elem_name == "contact:authInfo") {

      struct AuthInfo authInfo;
      map< string, string, less<string> > attributes = get_attributes(n);
      if (attributes["roid"] != "") {
	authInfo.set_roid(attributes["roid"]);
      }
      
      DOMNode *child;
      for (child = n->getFirstChild(); child != 0;
	   child = child->getNextSibling()) {
	if (child->getNodeType() != DOMNode::ELEMENT_NODE) {
	  continue;
	}
	string e_name = str_transcode(child->getNodeName());
	if (e_name == "contact:pw") {
	  authInfo.set_pw(str_transcode(child->getTextContent()));
	  break;
	}
      }
      rsp->set_authInfo(authInfo);
    } else if (elem_name == "contact:disclose") {
      CommonData::Disclose disc;

      bool has_flag = false;
      map< string, string, less<string> > attributes = get_attributes(n);
      if (attributes["flag"] != "") {
	has_flag = true;
	if (attributes["flag"] == "0") {
	  disc.flag = false;
	} else {
	  disc.flag = true;
	}
      }

      if (has_flag) {
	DOMNode *child;
	for (child = n->getFirstChild(); child != 0;
	     child = child->getNextSibling()) {
	  if (child->getNodeType() != DOMNode::ELEMENT_NODE) {
	    continue;
	  }
	  string e_name = str_transcode(child->getNodeName());
	  if (e_name == "contact:name") {
	    attributes = get_attributes(n);
	    if (!attributes.empty()) {
	      if (attributes.count("loc") > 0) {
		disc.name_loc = true;
	      } else if (attributes.count("int") > 0) {
		disc.name_int = true;
	      }
	    }

	  } else if (e_name == "contact:org") {
	    attributes = get_attributes(n);
	    if (!attributes.empty()) {
	      if (attributes.count("loc") > 0) {
		disc.org_loc = true;
	      } else if (attributes.count("int") > 0) {
		disc.org_int = true;
	      }
	    }
	  } else if (e_name == "contact:addr") {
	    attributes = get_attributes(n);
	    if (!attributes.empty()) {
	      if (attributes.count("loc") > 0) {
		disc.addr_loc = true;
	      } else if (attributes.count("int") > 0) {
		disc.addr_int = true;
	      }
	    }
	  } else if (e_name == "contact:voice") {
	    disc.voice = true;
	  } else if (e_name == "contact:fax") {
	    disc.fax = true;
	  } else if (e_name == "contact:email") {
	    disc.email = true;
	  }
	}
	common.set_disclose(disc);
      }
    }
    else if (elem_name == "extension") {
      look_children = true;
#if USE_BR_ORG
    } else if (elem_name == "brorg:infData") {
      fill_brorg_info_rsp(n, (BrOrgInfoRsp *) rsp);
#endif //USE_BR_ORG
    }
    
    rsp->set_common_data(common);
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_contact_info_rsp(child, rsp);
    }
  }
}

void DomParser::fill_contact_create_rsp(DOMNode *n, ContactCreateRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "resData") {
      look_children = true;
    } else if (elem_name == "contact:creData") {
      look_children = true;
    } else if (elem_name == "contact:id") {
      rsp->set_id(str_transcode(n->getTextContent()));
    } else if (elem_name == "contact:crDate") {
      rsp->set_crDate(str_transcode(n->getTextContent()));
    } else if (elem_name == "extension") {
      look_children = true;
#if USE_BR_ORG
    } else if (elem_name == "brorg:creData") {
      fill_brorg_create_rsp(n, (BrOrgCreateRsp*) rsp);
#endif //USE_BR_ORG
    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_contact_create_rsp(child, rsp);
    }
  }
}

void DomParser::fill_contact_pandata_rsp(DOMNode *n, PanDataRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "contact:panData") {
      look_children = true;
    } else if (elem_name == "contact:id") {
      rsp->set_object_id(str_transcode(n->getTextContent()));
      // attribute
      map< string, string, less<string> > attributes = get_attributes(n);
      if (attributes["paResult"] == "1") {
	rsp->set_paResult(true);
      }
      
    } else if (elem_name == "contact:paTRID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "contact:paDate") {
      rsp->set_paDate(str_transcode(n->getTextContent()));
    }
    
    if (look_children) {
      DOMNode *child;
      for (child = n->getFirstChild(); child != 0;
	   child = child->getNextSibling()) {
	fill_contact_pandata_rsp(child, rsp);
      }
    }
  }
}

void DomParser::fill_domain_check_rsp(DOMNode *n, DomainCheckRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "resData") {
      look_children = true;
    } else if (elem_name == "domain:chkData") {
      look_children = true;
    } else if (elem_name == "domain:cd") {
      DOMNode *child;
      string name, avail, reason;
      for (child = n->getFirstChild(); child != 0;
	   child = child->getNextSibling()) {
	if (child->getNodeType() != DOMNode::ELEMENT_NODE) {
	  continue;
	}
	string e_name = str_transcode(child->getNodeName());
	if (e_name == "domain:name") {

	  map< string, string, less<string> > attributes =
	    get_attributes(child);
	  if (attributes["avail"] != "") {
	    avail = attributes["avail"];
	  }
	  name = str_transcode(child->getTextContent());
	} else if (e_name == "domain:reason") {
	  reason = str_transcode(child->getTextContent());
	}
      }
      rsp->insert_availability(name, avail, reason);
    } else if (elem_name == "extension") {
      look_children = true;
#if USE_BR_DOMAINS
    } else if (elem_name == "brdomain:chkData") {
      //brdomain extension
      fill_brdomain_check_rsp(n, (BrDomainCheckRsp*) rsp);
#endif //USE_BR_DOMAINS
    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_domain_check_rsp(child, rsp);
    }
  }
}

void DomParser::fill_domain_info_rsp(DOMNode *n, DomainInfoRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "resData") {
      look_children = true;
    } else if (elem_name == "domain:infData") {
      look_children = true;
    } else if (elem_name == "domain:name") {
      rsp->set_name(str_transcode(n->getTextContent()));
    } else if (elem_name == "domain:roid") {
      rsp->set_roid(str_transcode(n->getTextContent()));
    } else if (elem_name == "domain:status") {

      map< string, string, less<string> > attributes = get_attributes(n);
      if (attributes["s"] != "") {
	rsp->insert_status(attributes["s"]);
      }
    } else if (elem_name == "domain:registrant") {
      rsp->set_registrant(str_transcode(n->getTextContent()));
    } else if (elem_name == "domain:contact") {
      map< string, string, less<string> > attributes = get_attributes(n);
      rsp->insert_contact(attributes["type"],
			  str_transcode(n->getTextContent()));
    } else if (elem_name == "domain:ns") {
      look_children = true;
    } else if (elem_name == "domain:hostAttr") {
      NameServer ns;
      DOMNode *child;
      for (child = n->getFirstChild(); child != 0;
	   child = child->getNextSibling()) {
	if (child->getNodeType() != DOMNode::ELEMENT_NODE) {
	  continue;
	}
	string e_name = str_transcode(child->getNodeName());
	if (e_name == "domain:hostName") {
	  ns.name = str_transcode(child->getTextContent());
	} else if (e_name == "domain:hostAddr") {

	  NSIPAddr ip;
	  ip.addr = str_transcode(child->getTextContent());

	  map<string, string, less<string> > attributes =
	    get_attributes(child);
	  ip.version = attributes["ip"];
	  
	  ns.ips.insert(ip);
	}
      }
      rsp->insert_nameserver(ns);
    } else if (elem_name == "domain:clID") {
      rsp->set_clID(str_transcode(n->getTextContent()));
    } else if (elem_name == "domain:crID") {
      rsp->set_crID(str_transcode(n->getTextContent()));
    } else if (elem_name == "domain:crDate") {
      rsp->set_crDate(str_transcode(n->getTextContent()));
    } else if (elem_name == "domain:upID") {
      rsp->set_upID(str_transcode(n->getTextContent()));
    } else if (elem_name == "domain:upDate") {
      rsp->set_upDate(str_transcode(n->getTextContent()));
    } else if (elem_name == "domain:exDate") {
      rsp->set_exDate(str_transcode(n->getTextContent()));
    } else if (elem_name == "domain:trDate") {
      rsp->set_trDate(str_transcode(n->getTextContent()));
    } else if (elem_name == "domain:authInfo") {
      map< string, string, less<string> > attributes = get_attributes(n);
      AuthInfo authInfo;
      authInfo.set_roid(attributes["roid"]);
      authInfo.set_pw(str_transcode(n->getTextContent()));
      rsp->set_authInfo(authInfo);
    } else if (elem_name == "domain:host") {
      // Ignored
    } else if (elem_name == "domain:hostObj") {
      // Ignored
    } else if (elem_name == "extension") {
      look_children = true;
#if USE_BR_DOMAINS
    } else if (elem_name == "brdomain:infData") {
      //brdomain extension
      fill_brdomain_info_rsp(n, (BrDomainInfoRsp *) rsp);
#endif //USE_BR_DOMAINS
    } else if (elem_name == "secDNS:infData") {
      look_children = true;
    } else if (elem_name == "secDNS:dsData") {
      DSInfo dsInfo;
      fill_ds_info(n, &dsInfo);
      rsp->add_dsInfo(dsInfo);
    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_domain_info_rsp(child, rsp);
    }
  }
}

void DomParser::fill_domain_create_rsp(DOMNode *n, DomainCreateRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "resData") {
      look_children = true;
    } else if (elem_name == "domain:creData") {
      look_children = true;
    } else if (elem_name == "domain:name") {
      rsp->set_name(str_transcode(n->getTextContent()));
    } else if (elem_name == "domain:crDate") {
      rsp->set_crDate(str_transcode(n->getTextContent()));
    } else if (elem_name == "domain:exDate") {
      rsp->set_exDate(str_transcode(n->getTextContent()));
    } else if (elem_name == "extension") {
      look_children = true;
#if USE_BR_DOMAINS
    } else if (elem_name == "brdomain:creData") {
      fill_brdomain_create_rsp(n, (BrDomainCreateRsp *) rsp);
#endif //USE_BR_DOMAINS
    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_domain_create_rsp(child, rsp);
    }
  }
}

void DomParser::fill_domain_renew_rsp(DOMNode *n, DomainRenewRsp *rsp) 
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "resData") {
      look_children = true;
    } else if (elem_name == "domain:renData") {
      look_children = true;
    } else if (elem_name == "domain:name") {
      rsp->set_name(str_transcode(n->getTextContent()));
    } else if (elem_name == "domain:exDate") {
      rsp->set_exDate(str_transcode(n->getTextContent()));
    } else if (elem_name == "extension") {
      look_children = true;
#if USE_BR_DOMAINS
    } else if (elem_name == "brdomain:renData") {
      //brdomain extension
      fill_brdomain_renew_rsp(n, (BrDomainRenewRsp*) rsp);
#endif //USE_BR_DOMAINS
    }    
  }      
    
  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
	 child = child->getNextSibling()) {
      fill_domain_renew_rsp(child, rsp);
    }
  }
}

void DomParser::fill_domain_pandata_rsp(DOMNode *n, PanDataRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "domain:panData") {
      look_children = true;
    } else if (elem_name == "domain:name") {
      rsp->set_object_id(str_transcode(n->getTextContent()));
      // attribute
      map< string, string, less<string> > attributes = get_attributes(n);
      if (attributes["paResult"] == "1") {
	rsp->set_paResult(true);
      }
      
    } else if (elem_name == "domain:paTRID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "domain:paDate") {
      rsp->set_paDate(str_transcode(n->getTextContent()));
    }
    
    if (look_children) {
      DOMNode *child;
      for (child = n->getFirstChild(); child != 0;
	   child = child->getNextSibling()) {
	fill_domain_pandata_rsp(child, rsp);
      }
    }
  }
}

#if USE_IP_MANAGEMENT
void DomParser::fill_ipnetwork_create_rsp(DOMNode *n, IpNetworkCreateRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "resData") {
      look_children = true;
    } else if (elem_name == "ipnetwork:creData") {
      look_children = true;
    } else if (elem_name == "ipnetwork:crDate") {
      rsp->set_crDate(str_transcode(n->getTextContent()));
    } else if (elem_name == "ipnetwork:exDate") {
      rsp->set_exDate(str_transcode(n->getTextContent()));
    } else if (elem_name == "ipnetwork:ipRange") {
      rsp->set_ipRange(fill_ipRange(n));
    } else if (elem_name == "ipnetwork:roid") {
      rsp->set_roid(str_transcode(n->getTextContent()));
    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_ipnetwork_create_rsp(child, rsp);
    }
  }
}

void DomParser::fill_ipnetwork_check_rsp(DOMNode *n, IpNetworkCheckRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "resData") {
      look_children = true;
    } else if (elem_name == "ipnetwork:chkData") {
      look_children = true;
    } else if (elem_name == "ipnetwork:cd") {
      DOMNode *child;

      IpRange ipRange;
      string reason = "";
      string reason_lang = "";

      for (child = n->getFirstChild(); child != 0; 
	   child = child->getNextSibling()) {
	if (child->getNodeType() != DOMNode::ELEMENT_NODE) {
	  continue;
	}

	string e_name = str_transcode(child->getNodeName());
	if (e_name == "ipnetwork:ipRange") {
	  ipRange = fill_ipRange(child);
	} else if (e_name == "ipnetwork:reason") {
	  map<string, string, less<string> > attributes = get_attributes(child);
	  string lang = attributes["lang"];

	  if (lang != "") {
	    reason_lang = lang;
	  }

	  reason = str_transcode(child->getTextContent());
	}
      }

      rsp->insert_availability(ipRange, reason, reason_lang);
    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_ipnetwork_check_rsp(child, rsp);
    }
  }
}

void DomParser::fill_ipnetwork_renew_rsp(DOMNode *n, IpNetworkRenewRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "resData") {
      look_children = true;
    } else if (elem_name == "ipnetwork:renData") {
      look_children = true;
    } else if (elem_name == "ipnetwork:roid") {
      rsp->set_roid(str_transcode(n->getTextContent()));
    } else if (elem_name == "ipnetwork:exDate") {
      rsp->set_expDate(str_transcode(n->getTextContent()));
    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_ipnetwork_renew_rsp(child, rsp);
    }
  }
}

void DomParser::fill_ipnetwork_transfer_rsp(DOMNode *n, 
					    IpNetworkTransferRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "resData") {
      look_children = true;
    } else if (elem_name == "ipnetwork:trnData") {
      look_children = true;
    } else if (elem_name == "ipnetwork:roid") {
      rsp->set_roid(str_transcode(n->getTextContent()));
    } else if (elem_name == "ipnetwork:trStatus") {
      rsp->set_trStatus(str_transcode(n->getTextContent()));
    } else if (elem_name == "ipnetwork:reID") {
      rsp->set_reId(str_transcode(n->getTextContent()));
    } else if (elem_name == "ipnetwork:reDate") {
      rsp->set_reDate(str_transcode(n->getTextContent()));
    } else if (elem_name == "ipnetwork:acID") {
      rsp->set_acId(str_transcode(n->getTextContent()));
    } else if (elem_name == "ipnetwork:acDate") {
      rsp->set_acDate(str_transcode(n->getTextContent()));
    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_ipnetwork_transfer_rsp(child, rsp);
    }
  }
}

void DomParser::fill_ipnetwork_info_rsp(DOMNode *n, IpNetworkInfoRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "resData") {
      look_children = true;
    } else if (elem_name == "ipnetwork:infData") {
      look_children = true;
    } else if (elem_name == "ipnetwork:ipRange") {
      rsp->set_ipRange(fill_ipRange(n));
    } else if (elem_name == "ipnetwork:ipRangeInfo") {
      IpNetworkInfoRsp::IpRangeInfo ipRangeInfo;
      DOMNode *child;
      
      for (child = n->getFirstChild(); child != 0; 
	   child = child->getNextSibling()) {
	if (child->getNodeType() != DOMNode::ELEMENT_NODE) {
	  continue;
	}

	string e_name = str_transcode(child->getNodeName());
	if (e_name == "ipnetwork:roid") {
	  ipRangeInfo.set_roid(str_transcode(child->getTextContent()));
	} else if (e_name == "ipnetwork:organization") {
	  ipRangeInfo.set_organization(str_transcode(child->getTextContent()));
	} else if (e_name == "ipnetwork:allocType") {
	  ipRangeInfo.set_allocType(str_transcode(child->getTextContent()));
	} else if (e_name == "ipnetwork:contact") {
	  map<string, string, less<string> > attributes = get_attributes(child);
	  ipRangeInfo.insert_contact(attributes["type"], 
				       str_transcode(child->getTextContent()));
	} else if (e_name == "ipnetwork:asn") {
	  int asn = atoi(str_transcode(child->getTextContent()).c_str());
	  ipRangeInfo.set_asn(asn);
	} else if (e_name == "ipnetwork:reverseDNS") {
	  ipRangeInfo.insert_reverseDns(fill_reverseDns(child));
	} else if (e_name == "ipnetwork:parentNetwork") {
	  IpNetworkInfoRsp::Network parentNetwork;
	  DOMNode *blockChild;

	  for (blockChild = child->getFirstChild(); blockChild != 0; 
	       blockChild = blockChild->getNextSibling()) {
	    if (blockChild->getNodeType() != DOMNode::ELEMENT_NODE) {
	      continue;
	    }
	    
	    string block_e_name = str_transcode(blockChild->getNodeName());
	    if (block_e_name == "ipnetwork:ipRange") {
	      parentNetwork.set_ipRange(fill_ipRange(blockChild));
	    } else if (block_e_name == "ipnetwork:roid") {
	      parentNetwork.set_roid(str_transcode(blockChild->getTextContent()));
	    }
	  }

	  ipRangeInfo.set_parentNetwork(parentNetwork);
	} else if (e_name == "ipnetwork:childNetwork") {
	  IpNetworkInfoRsp::Network childNetwork;
	  DOMNode *blockChild;

	  for (blockChild = child->getFirstChild(); blockChild != 0; 
	       blockChild = blockChild->getNextSibling()) {
	    if (blockChild->getNodeType() != DOMNode::ELEMENT_NODE) {
	      continue;
	    }
	    
	    string block_e_name = str_transcode(blockChild->getNodeName());
	    if (block_e_name == "ipnetwork:ipRange") {
	      childNetwork.set_ipRange(fill_ipRange(blockChild));
	    } else if (block_e_name == "ipnetwork:roid") {
	      childNetwork.set_roid(str_transcode(blockChild->getTextContent()));
	    }
	  }

	  ipRangeInfo.insert_childNetwork(childNetwork);
	} else if (e_name == "ipnetwork:clID") {
	  ipRangeInfo.set_clId(str_transcode(child->getTextContent()));
	} else if (e_name == "ipnetwork:crID") {
	  ipRangeInfo.set_crId(str_transcode(child->getTextContent()));
	} else if (e_name == "ipnetwork:crDate") {
	  ipRangeInfo.set_crDate(str_transcode(child->getTextContent()));
	} else if (e_name == "ipnetwork:exDate") {
	  ipRangeInfo.set_exDate(str_transcode(child->getTextContent()));
	} else if (e_name == "ipnetwork:upID") {
	  ipRangeInfo.set_upId(str_transcode(child->getTextContent()));
	} else if (e_name == "ipnetwork:upDate") {
	  ipRangeInfo.set_upDate(str_transcode(child->getTextContent()));
	} else if (e_name == "ipnetwork:trDate") {
	  ipRangeInfo.set_trDate(str_transcode(child->getTextContent()));
	}
      }
      
      rsp->insert_ipRangeInfo(ipRangeInfo);
    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_ipnetwork_info_rsp(child, rsp);
    }
  }
}

void DomParser::fill_asn_check_rsp(DOMNode *n, AsnCheckRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "resData") {
      look_children = true;
    } else if (elem_name == "asn:chkData") {
      look_children = true;
    } else if (elem_name == "asn:cd") {
      DOMNode *child;

      int asn = 0;
      bool available = false;
      string reason = "";
      string reason_lang = "";

      for (child = n->getFirstChild(); child != 0; 
	   child = child->getNextSibling()) {
	if (child->getNodeType() != DOMNode::ELEMENT_NODE) {
	  continue;
	}

	string e_name = str_transcode(child->getNodeName());
	if (e_name == "asn:number") {
	  map<string, string, less<string> > attributes = get_attributes(child);

	  string avail = attributes["avail"];
	  if (avail == "1") {
	    available = true;
	  } else {
	    available = false;
	  }

	  asn = atoi(str_transcode(child->getTextContent()).c_str());
	} else if (e_name == "asn:reason") {
	  map<string, string, less<string> > attributes = get_attributes(child);
	  if (attributes["lang"] != "") {
	    reason_lang = attributes["lang"];
	  }

	  reason = str_transcode(child->getTextContent()).c_str();
	}
      }

      rsp->insert_availability(asn, available, reason, reason_lang);
    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_asn_check_rsp(child, rsp);
    }
  }
}

void DomParser::fill_asn_create_rsp(DOMNode *n, AsnCreateRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "resData") {
      look_children = true;
    } else if (elem_name == "asn:creData") {
      look_children = true;
    } else if (elem_name == "asn:number") {
      rsp->set_asn(atoi(str_transcode(n->getTextContent()).c_str()));
    } else if (elem_name == "asn:roid") {
      rsp->set_roid(str_transcode(n->getTextContent()).c_str());
    } else if (elem_name == "asn:crDate") {
      rsp->set_crDate(str_transcode(n->getTextContent()).c_str());
    } else if (elem_name == "asn:exDate") {
      rsp->set_exDate(str_transcode(n->getTextContent()).c_str());
    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_asn_create_rsp(child, rsp);
    }
  }
}

void DomParser::fill_asn_renew_rsp(DOMNode *n, AsnRenewRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "resData") {
      look_children = true;
    } else if (elem_name == "asn:renData") {
      look_children = true;
    } else if (elem_name == "asn:number") {
      rsp->set_asn(atoi(str_transcode(n->getTextContent()).c_str()));
    } else if (elem_name == "asn:exDate") {
      rsp->set_expDate(str_transcode(n->getTextContent()).c_str());
    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_asn_renew_rsp(child, rsp);
    }
  }
}

void DomParser::fill_asn_transfer_rsp(DOMNode *n, AsnTransferRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "resData") {
      look_children = true;
    } else if (elem_name == "asn:trnData") {
      look_children = true;
    } else if (elem_name == "asn:number") {
      rsp->set_asn(atoi(str_transcode(n->getTextContent()).c_str()));
    } else if (elem_name == "asn:trStatus") {
      rsp->set_trStatus(str_transcode(n->getTextContent()).c_str());
    } else if (elem_name == "asn:reID") {
      rsp->set_reId(str_transcode(n->getTextContent()).c_str());
    } else if (elem_name == "asn:reDate") {
      rsp->set_reDate(str_transcode(n->getTextContent()).c_str());
    } else if (elem_name == "asn:acID") {
      rsp->set_acId(str_transcode(n->getTextContent()).c_str());
    } else if (elem_name == "asn:acDate") {
      rsp->set_acDate(str_transcode(n->getTextContent()).c_str());
    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_asn_transfer_rsp(child, rsp);
    }
  }
}

void DomParser::fill_asn_info_rsp(DOMNode *n, AsnInfoRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "resData") {
      look_children = true;
    } else if (elem_name == "asn:infData") {
      look_children = true;
    } else if (elem_name == "asn:number") {
      rsp->set_asn(atoi(str_transcode(n->getTextContent()).c_str()));
    } else if (elem_name == "asn:roid") {
      rsp->set_roid(str_transcode(n->getTextContent()).c_str());
    } else if (elem_name == "asn:organization") {
      rsp->set_organization(str_transcode(n->getTextContent()).c_str());
    } else if (elem_name == "asn:contact") {
      map<string, string, less<string> > attributes = get_attributes(n);
      rsp->insert_contact(attributes["type"], 
			  str_transcode(n->getTextContent()).c_str());
    } else if (elem_name == "asn:clID") {
      rsp->set_clId(str_transcode(n->getTextContent()).c_str());
    } else if (elem_name == "asn:crID") {
      rsp->set_crId(str_transcode(n->getTextContent()).c_str());
    } else if (elem_name == "asn:crDate") {
      rsp->set_crDate(str_transcode(n->getTextContent()).c_str());
    } else if (elem_name == "asn:exDate") {
      rsp->set_exDate(str_transcode(n->getTextContent()).c_str());
    } else if (elem_name == "asn:upID") {
      rsp->set_upId(str_transcode(n->getTextContent()).c_str());
    } else if (elem_name == "asn:upDate") {
      rsp->set_upDate(str_transcode(n->getTextContent()).c_str());
    } else if (elem_name == "asn:trDate") {
      rsp->set_trDate(str_transcode(n->getTextContent()).c_str());
    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_asn_info_rsp(child, rsp);
    }
  }
}
#endif //USE_IP_MANAGEMENT

void DomParser::fill_poll_rsp(DOMNode *n, PollRsp *poll_rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "epp" || elem_name == "response") {
      look_children = true;
    } else if (elem_name == "result") {
      fill_result(n, (Response *)poll_rsp);
    } else if (elem_name == "trID") {
      look_children = true;
    } else if (elem_name == "clTRID") {
      poll_rsp->set_clTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "svTRID") {
      poll_rsp->set_svTRID(str_transcode(n->getTextContent()));
    } else if (elem_name == "msgQ") {
      look_children = true;

      map< string, string, less<string> > attributes = get_attributes(n);
      if (attributes["count"] != "") {
	poll_rsp->set_count(attributes["count"]);
      }
      if (attributes["id"] != "") {
	poll_rsp->set_id(attributes["id"]);
      }
    } else if (elem_name == "qDate") {
      poll_rsp->set_qDate(str_transcode(n->getTextContent()));
    } else if (elem_name == "msg") {

      // Get the attribute lang
      map< string, string, less<string> > attributes = get_attributes(n);
      if (attributes["lang"] != "") {
	poll_rsp->set_lang(attributes["lang"]);
      }

      // Run thru the child elements, getting the attributes and 
      // text content
      DOMNode *child;
      int child_count = 0;
      map<string, PollRsp::MsgContent, less<string> > content;
      for (child = n->getFirstChild(); child != 0;
	   child = child->getNextSibling()) {
	if (child->getNodeType() == DOMNode::ELEMENT_NODE) {
	  child_count++;
	  PollRsp::MsgContent msg_content;

	  map< string, string, less<string> > attributes = 
	    get_attributes(child);	  
	  string child_name = str_transcode(child->getNodeName());
	  string child_text_content = str_transcode(child->getTextContent());
	  msg_content.value = child_text_content;
	  msg_content.attributes = attributes;
	  content[child_name] = msg_content;
	}
      }
      poll_rsp->set_content(content);
      
      // If there were no child elements, get text content
      if (child_count == 0) {
	string msg_text_content = str_transcode(n->getTextContent());
	if (msg_text_content != "") {
	  poll_rsp->set_text(msg_text_content);
	}
      }
      
    } else if (elem_name == "resData") { // resDatas
      look_children = true;
    } else if (elem_name == "domain:panData") {
      auto_ptr<PanDataRsp> rsp = auto_ptr<PanDataRsp>(new PanDataRsp());
      fill_domain_pandata_rsp(n, rsp.get());
      poll_rsp->set_response((Response *) rsp.release(), DOMAIN_PANDATA);
    } else if (elem_name == "contact:panData") {
      auto_ptr<PanDataRsp> rsp = auto_ptr<PanDataRsp>(new PanDataRsp());
      fill_contact_pandata_rsp(n, rsp.get());
      poll_rsp->set_response((Response *) rsp.release(), CONTACT_PANDATA);
    } else if (elem_name == "contact:chkData") {
      auto_ptr<ContactCheckRsp> rsp =
	auto_ptr<ContactCheckRsp>(new ContactCheckRsp());
      fill_contact_check_rsp(n, rsp.get());
      poll_rsp->set_response((Response *) rsp.release(), CONTACT_CHECK);
    } else if (elem_name == "contact:infData") {
      auto_ptr<ContactInfoRsp> rsp = 
	auto_ptr<ContactInfoRsp>(new ContactInfoRsp());
      fill_contact_info_rsp(n, rsp.get());
      poll_rsp->set_response((Response *) rsp.release(), CONTACT_INFO);      
    } else if (elem_name == "contact:creData") {
      auto_ptr<ContactCreateRsp> rsp = 
	auto_ptr<ContactCreateRsp>(new ContactCreateRsp());
      fill_contact_create_rsp(n, rsp.get());
      poll_rsp->set_response((Response *) rsp.release(), CONTACT_CREATE);
    } else if (elem_name == "domain:chkData") {
      auto_ptr<DomainCheckRsp> rsp = 
	auto_ptr<DomainCheckRsp>(new DomainCheckRsp());
      fill_domain_check_rsp(n, rsp.get());
      poll_rsp->set_response((Response *) rsp.release(), DOMAIN_CHECK);
    } else if (elem_name == "domain:infData") {
      auto_ptr<DomainInfoRsp> rsp = 
	auto_ptr<DomainInfoRsp>(new DomainInfoRsp());
      fill_domain_info_rsp(n, rsp.get());
      poll_rsp->set_response((Response *) rsp.release(), DOMAIN_INFO);
    } else if (elem_name == "domain:creData") {
      auto_ptr<DomainCreateRsp> rsp = 
	auto_ptr<DomainCreateRsp>(new DomainCreateRsp());
      fill_domain_create_rsp(n, rsp.get());
      poll_rsp->set_response((Response *) rsp.release(), DOMAIN_CREATE);
    } else if (elem_name == "domain:renData") {
      auto_ptr<DomainRenewRsp> rsp = 
	auto_ptr<DomainRenewRsp>(new DomainRenewRsp());
      fill_domain_renew_rsp(n, rsp.get());
      poll_rsp->set_response((Response *) rsp.release(), DOMAIN_RENEW);

    } else if (elem_name == "extension") {  // extensions
      look_children = true;
#if USE_BR_ORG
    } else if (elem_name == "brorg:panData") {
      if (poll_rsp->get_response_type() == CONTACT_PANDATA) {
	PanDataRsp *parent = (PanDataRsp *) poll_rsp->get_response();
	auto_ptr<BrOrgPanDataRsp> extension_rsp =
	  auto_ptr<BrOrgPanDataRsp>(new BrOrgPanDataRsp());
	extension_rsp->copy_parent_data(*parent);
	fill_brorg_pandata_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(), 
			       BR_ORG_PANDATA);
      }
#endif //USE_BR_ORG
#if USE_BR_DOMAINS
    } else if (elem_name == "brdomain:panData") {
      if (poll_rsp->get_response_type() == DOMAIN_PANDATA) {
	PanDataRsp *parent = (PanDataRsp *) poll_rsp->get_response();
	auto_ptr<BrDomainPanDataRsp> extension_rsp(new BrDomainPanDataRsp());
	extension_rsp->copy_parent_data(*parent);
	fill_brdomain_pandata_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(),
			       BR_DOMAIN_PANDATA);
      }
#endif //USE_BR_DOMAINS
#if USE_BR_ORG
    } else if (elem_name == "brorg:infData") {
      if (poll_rsp->get_response_type() == CONTACT_INFO) {
	ContactInfoRsp *parent = (ContactInfoRsp *) poll_rsp->get_response();
	auto_ptr<BrOrgInfoRsp> extension_rsp = 
	  auto_ptr<BrOrgInfoRsp>(new BrOrgInfoRsp());
	extension_rsp->copy_parent_data(*parent);
	fill_brorg_info_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(),
			       BR_ORG_INFO);
      }
#endif //USE_BR_ORG
#if USE_BR_DOMAINS
    } else if (elem_name == "brdomain:chkData") {
      if (poll_rsp->get_response_type() == DOMAIN_CHECK) {
	DomainCheckRsp *parent = (DomainCheckRsp *) poll_rsp->get_response();
	auto_ptr<BrDomainCheckRsp> extension_rsp = 
	  auto_ptr<BrDomainCheckRsp>(new BrDomainCheckRsp());
	extension_rsp->copy_parent_data(*parent);
	fill_brdomain_check_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(),
			       BR_DOMAIN_CHECK);
      }
    } else if (elem_name == "brdomain:infData") {
      if (poll_rsp->get_response_type() == DOMAIN_INFO) {
	DomainInfoRsp *parent = (DomainInfoRsp *) poll_rsp->get_response();
	auto_ptr<BrDomainInfoRsp> extension_rsp = 
	  auto_ptr<BrDomainInfoRsp>(new BrDomainInfoRsp());
	extension_rsp->copy_parent_data(*parent);
	fill_brdomain_info_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(),
			       BR_DOMAIN_INFO);
      }
    } else if (elem_name == "brdomain:creData") {
      if (poll_rsp->get_response_type() == DOMAIN_CREATE) {
	DomainCreateRsp *parent = (DomainCreateRsp *) poll_rsp->get_response();
	auto_ptr<BrDomainCreateRsp> extension_rsp = 
	  auto_ptr<BrDomainCreateRsp>(new BrDomainCreateRsp());
	extension_rsp->copy_parent_data(*parent);
	fill_brdomain_create_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(),
			       BR_DOMAIN_CREATE);
      }
    } else if (elem_name == "brdomain:renData") {
      if (poll_rsp->get_response_type() == DOMAIN_RENEW) {
	DomainRenewRsp *parent = (DomainRenewRsp *) poll_rsp->get_response();
	auto_ptr<BrDomainRenewRsp> extension_rsp = 
	  auto_ptr<BrDomainRenewRsp>(new BrDomainRenewRsp());
	extension_rsp->copy_parent_data(*parent);
	fill_brdomain_renew_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(),
			       BR_DOMAIN_RENEW);
      }
    } else if (elem_name == "brdomain:updData") {
      if (poll_rsp->get_response_type() == DOMAIN_UPDATE) {
	auto_ptr<BrDomainUpdateRsp> extension_rsp = 
	  auto_ptr<BrDomainUpdateRsp>(new BrDomainUpdateRsp());
	fill_brdomain_update_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(), 
			       BR_DOMAIN_UPDATE);
      }
#endif //USE_BR_DOMAINS
#if USE_IP_MANAGEMENT
    } else if (elem_name == "ipnetwork:create") {
      if (poll_rsp->get_response_type() == IP_NETWORK_CREATE) {
	auto_ptr<IpNetworkCreateRsp> extension_rsp = 
	  auto_ptr<IpNetworkCreateRsp>(new IpNetworkCreateRsp());
	fill_ipnetwork_create_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(), 
			       IP_NETWORK_CREATE);
      }
    } else if (elem_name == "ipnetwork:delete") {
      if (poll_rsp->get_response_type() == IP_NETWORK_DELETE) {
	auto_ptr<Response> extension_rsp = auto_ptr<Response>(new Response());
	fill_response(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(), 
			       IP_NETWORK_DELETE);
      }
    } else if (elem_name == "ipnetwork:renew") {
      if (poll_rsp->get_response_type() == IP_NETWORK_RENEW) {
	auto_ptr<IpNetworkRenewRsp> extension_rsp = 
	  auto_ptr<IpNetworkRenewRsp>(new IpNetworkRenewRsp());
	fill_ipnetwork_renew_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(), 
			       IP_NETWORK_RENEW);
      }
    } else if (elem_name == "ipnetwork:transfer") {
      if (poll_rsp->get_response_type() == IP_NETWORK_TRANSFER) {
	auto_ptr<IpNetworkTransferRsp> extension_rsp = 
	  auto_ptr<IpNetworkTransferRsp>(new IpNetworkTransferRsp());
	fill_ipnetwork_transfer_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(), 
			       IP_NETWORK_TRANSFER);
      }
    } else if (elem_name == "ipnetwork:check") {
      if (poll_rsp->get_response_type() == IP_NETWORK_CHECK) {
	auto_ptr<IpNetworkCheckRsp> extension_rsp = 
	  auto_ptr<IpNetworkCheckRsp>(new IpNetworkCheckRsp());
	fill_ipnetwork_check_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(), 
			       IP_NETWORK_CHECK);
      }
    } else if (elem_name == "ipnetwork:info") {
      if (poll_rsp->get_response_type() == IP_NETWORK_INFO) {
	auto_ptr<IpNetworkInfoRsp> extension_rsp = 
	  auto_ptr<IpNetworkInfoRsp>(new IpNetworkInfoRsp());
	fill_ipnetwork_info_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(), 
			       IP_NETWORK_INFO);
      }
    } else if (elem_name == "asn:create") {
      if (poll_rsp->get_response_type() == ASN_CREATE) {
	auto_ptr<AsnCreateRsp> extension_rsp = 
	  auto_ptr<AsnCreateRsp>(new AsnCreateRsp());
	fill_asn_create_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(), 
			       ASN_CREATE);
      }
    } else if (elem_name == "asn:delete") {
      if (poll_rsp->get_response_type() == ASN_DELETE) {
	auto_ptr<Response> extension_rsp = 
	  auto_ptr<Response>(new Response());
	fill_response(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(), 
			       ASN_DELETE);
      }
    } else if (elem_name == "asn:renew") {
      if (poll_rsp->get_response_type() == ASN_RENEW) {
	auto_ptr<AsnRenewRsp> extension_rsp = 
	  auto_ptr<AsnRenewRsp>(new AsnRenewRsp());
	fill_asn_renew_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(), 
			       ASN_RENEW);
      }
    } else if (elem_name == "asn:transfer") {
      if (poll_rsp->get_response_type() == ASN_TRANSFER) {
	auto_ptr<AsnTransferRsp> extension_rsp = 
	  auto_ptr<AsnTransferRsp>(new AsnTransferRsp());
	fill_asn_transfer_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(), 
			       ASN_TRANSFER);
      }
    } else if (elem_name == "asn:check") {
      if (poll_rsp->get_response_type() == ASN_CHECK) {
	auto_ptr<AsnCheckRsp> extension_rsp = 
	  auto_ptr<AsnCheckRsp>(new AsnCheckRsp());
	fill_asn_check_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(), 
			       ASN_CHECK);
      }
    } else if (elem_name == "asn:info") {
      if (poll_rsp->get_response_type() == ASN_INFO) {
	auto_ptr<AsnInfoRsp> extension_rsp = 
	  auto_ptr<AsnInfoRsp>(new AsnInfoRsp());
	fill_asn_info_rsp(n, extension_rsp.get());
	poll_rsp->set_response((Response *) extension_rsp.release(), 
			       ASN_INFO);
      }
#endif // USE_IP_MANAGEMENT
    }
  }
  
  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
	 child = child->getNextSibling()) {
      fill_poll_rsp(child, poll_rsp);
    }
  }
}

#if USE_BR_ORG
void DomParser::fill_brorg_info_rsp(DOMNode *n, BrOrgInfoRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "brorg:infData") {
      look_children = true;
    } else if (elem_name == "brorg:organization") {
      rsp->set_organization(str_transcode(n->getTextContent()));
    } else if (elem_name == "brorg:responsible") {
      rsp->set_responsible(str_transcode(n->getTextContent()));
    } else if (elem_name == "brorg:contact") {
      string id = str_transcode(n->getTextContent());
      map< string, string, less<string> > attributes = get_attributes(n);
      string type = attributes["type"];
      rsp->insert_contact(type, id);
    } else if (elem_name == "brorg:proxy") {
      rsp->set_proxy(str_transcode(n->getTextContent()));
    } else if (elem_name == "brorg:domainName") {
      rsp->insert_domainName(str_transcode(n->getTextContent()));
    } else if (elem_name == "brorg:exDate") {
      rsp->set_exDate(str_transcode(n->getTextContent()));
    } else if (elem_name == "brorg:asNumber") {
      rsp->insert_asn(atoi(str_transcode(n->getTextContent()).c_str()));
    } else if (elem_name == "brorg:ipRange") {
      rsp->insert_ipRange(fill_brorg_ipRange(n));
    }
  }
  
  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_brorg_info_rsp(child, rsp);
    }
  }
}

void DomParser::fill_brorg_check_rsp(DOMNode *n, BrOrgCheckRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "brorg:chkData") {
      look_children = true;
    } else if (elem_name == "brorg:ticketInfo") {
      string org;
      
      struct BrOrgCheckRsp::Unavailability unavail;
      unavail.tkt_num = 0;
      unavail.tkt_fqdn = "";
      DOMNode *child;
      for (child = n->getFirstChild(); child != 0;
	   child = child->getNextSibling()) {
	if (child->getNodeType() != DOMNode::ELEMENT_NODE) {
	  continue;
	}
	elem_name = str_transcode(child->getNodeName());
	if (elem_name == "brorg:organization") {
	  org = str_transcode(child->getTextContent());
	} else if (elem_name == "brorg:domainName") {
	  unavail.tkt_fqdn = str_transcode(child->getTextContent());
	} else if (elem_name == "brorg:ticketNumber") {
	  unavail.tkt_num = 
	    atoi(str_transcode(child->getTextContent()).c_str());
	} 
      }
      
      rsp->insert_unavailability(org, unavail);
    }
  }
  
  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_brorg_check_rsp(child, rsp);
    }
  }
}

void DomParser::fill_brorg_pandata_rsp(DOMNode *n, BrOrgPanDataRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "brorg:panData") {
      look_children = true;
    } else if (elem_name == "brorg:organization") {
      rsp->set_organization(str_transcode(n->getTextContent()));
    } else if (elem_name == "brorg:reason") {

      // Get the lang attribute
      map< string, string, less<string> > attributes = get_attributes(n);
      if (attributes["lang"] != "") {
	rsp->set_reason_lang(attributes["lang"]);
      }

      rsp->set_reason(str_transcode(n->getTextContent()));
    }
  }
  
  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
	 child = child->getNextSibling()) {
      fill_brorg_pandata_rsp(child, rsp);
    }
  }
}

void DomParser::fill_brorg_create_rsp(DOMNode *n, BrOrgCreateRsp *rsp)
{
  bool look_children = false;

  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "brorg:creData") {
      look_children = true;
    } else if (elem_name == "brorg:organization") {
      rsp->set_organization(str_transcode(n->getTextContent()));
    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
	 child = child->getNextSibling()) {
      fill_brorg_create_rsp(child, rsp);
    }
  }
}
#endif //USE_BR_ORG

#if USE_BR_DOMAINS
void DomParser::fill_brdomain_check_rsp(DOMNode *n, BrDomainCheckRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "brdomain:chkData") {
      look_children = true;
    } else if (elem_name == "brdomain:cd") {
      string domain;
      
      // attributes
      struct BrDomainCheckRsp::Unavailability unavail;
      unavail.hasConcurrent = false;
      unavail.inReleaseProcess = false;
      map< string, string, less<string> > attributes = get_attributes(n);
      if (attributes["hasConcurrent"] == "1") {
	unavail.hasConcurrent = true;
      }
      if (attributes["inReleaseProcess"] == "1") {
	unavail.inReleaseProcess = true;
      }

      unavail.organization = "";
      unavail.equivalentName = "";
      DOMNode *child;
      for (child = n->getFirstChild(); child != 0;
	   child = child->getNextSibling()) {
	if (child->getNodeType() != DOMNode::ELEMENT_NODE) {
	  continue;
	}
	elem_name = str_transcode(child->getNodeName());
	if (elem_name == "brdomain:name") {
	  domain = str_transcode(child->getTextContent());
	} else if (elem_name == "brdomain:organization") {
	  unavail.organization = str_transcode(child->getTextContent());
	} else if (elem_name == "brdomain:equivalentName") {
	  unavail.equivalentName = str_transcode(child->getTextContent());
	} else if (elem_name == "brdomain:ticketNumber") {
	  unavail.tickets.insert(atoi(str_transcode(
            child->getTextContent()).c_str()));
	}
      }
      
      rsp->insert_unavailability(domain, unavail);
    }
  }
  
  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
         child = child->getNextSibling()) {
      fill_brdomain_check_rsp(child, rsp);
    }
  }
}

void DomParser::fill_brdomain_info_rsp(DOMNode *n, BrDomainInfoRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "brdomain:infData") {
      look_children = true;
    } else if (elem_name == "brdomain:ticketNumber") {
      rsp->set_ticketNumber(atoi(str_transcode(n->getTextContent()).c_str()));
    } else if (elem_name == "brdomain:organization") {
      rsp->set_organization(str_transcode(n->getTextContent()));
    } else if (elem_name == "brdomain:releaseProcessFlags") {

      struct ReleaseProcessFlags rpf;
      rpf.flag1 = 0;
      rpf.flag2 = 0;
      rpf.flag3 = 0;

      map< string, string, less<string> > attributes = get_attributes(n);
      if (attributes["flag1"] == "1") {
	rpf.flag1 = 1;
      }
      if (attributes["flag2"] == "1") {
	rpf.flag2 = 1;
      }
      if (attributes["flag3"] == "1") {
	rpf.flag3 = 1;
      }
      rsp->set_releaseProcessFlags(rpf);

    } else if (elem_name == "brdomain:pending") {
      look_children = true;
    } else if (elem_name == "brdomain:doc") {
      struct PendingDoc pend;

      // attribute
      map< string, string, less<string> > attributes = get_attributes(n);
      pend.status = attributes["status"];

      // brdomain:doc child elements
      DOMNode *child;
      for (child = n->getFirstChild(); child != 0;
	   child = child->getNextSibling()) {
	if (child->getNodeType() != DOMNode::ELEMENT_NODE) {
	  continue;
	}
	string curr_name = str_transcode(child->getNodeName());
	if (curr_name == "brdomain:docType") {
	  pend.docType = str_transcode(child->getTextContent());
	} else if (curr_name == "brdomain:limit") {
	  pend.limit = str_transcode(child->getTextContent());
	} else if (curr_name == "brdomain:description") {
 	  map< string, string, less<string> > description_attributes;
	  description_attributes = get_attributes(child);
 	  pend.description_lang = description_attributes["lang"];
	  pend.description = str_transcode(child->getTextContent());
	}
      }

      rsp->insert_doc_pending(pend);

    } else if (elem_name == "brdomain:dns") {
      struct PendingDns pend;

      // attribute
      map< string, string, less<string> > attributes = get_attributes(n);
      pend.status = attributes["status"];

      // brdomain:dns child elements
      map< string, string, less<string> > children = get_children_simple(n);
      pend.hostName = children["brdomain:hostName"];
      pend.limit    = children["brdomain:limit"];

      rsp->insert_dns_pending(pend);

    } else if (elem_name == "brdomain:releaseProc") {
	  
      struct PendingReleaseProc pend;

      // attribute
      map< string, string, less<string> > attributes = get_attributes(n);
      pend.status = attributes["status"];

      // brdomain:releaseProc child element
      map< string, string, less<string> > children = get_children_simple(n);
      pend.limit = children["brdomain:limit"];

      rsp->set_rel_pending(pend);

    } else if (elem_name == "brdomain:ticketNumberConc") {

      int tkt = atoi(str_transcode(n->getTextContent()).c_str());
      rsp->insert_concurrent_ticket(tkt);

    } else if (elem_name == "brdomain:publicationStatus") {
      // attribute
      map< string, string, less<string> > attributes = get_attributes(n);
      rsp->set_publication_flag(attributes["publicationFlag"]);
      look_children = true;
    
    } else if (elem_name == "brdomain:onHoldReason") {
      
      rsp->insert_onhold_reason(str_transcode(n->getTextContent()));

    } else if (elem_name == "brdomain:autoRenew") {

      map< string, string, less<string> > attributes = get_attributes(n);
      rsp->set_active(atoi(attributes["active"].c_str()));

    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
	 child = child->getNextSibling()) {
      fill_brdomain_info_rsp(child, rsp);
    }
  }
}

void DomParser::fill_brdomain_create_rsp(DOMNode *n, BrDomainCreateRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "brdomain:creData") {
      look_children = true;
    } else if (elem_name == "brdomain:ticketNumber") {
      rsp->set_ticketNumber(atoi(str_transcode(n->getTextContent()).c_str()));
    } else if (elem_name == "brdomain:pending") {
      look_children = true;
    } else if (elem_name == "brdomain:doc") {
      struct PendingDoc pend;

      // attribute
      map< string, string, less<string> > attributes = get_attributes(n);
      pend.status = attributes["status"];

      // brdomain:doc child elements
      DOMNode *child;
      for (child = n->getFirstChild(); child != 0;
	   child = child->getNextSibling()) {
	if (child->getNodeType() != DOMNode::ELEMENT_NODE) {
	  continue;
	}
	string curr_name = str_transcode(child->getNodeName());
	if (curr_name == "brdomain:docType") {
	  pend.docType = str_transcode(child->getTextContent());
	} else if (curr_name == "brdomain:limit") {
	  pend.limit = str_transcode(child->getTextContent());
	} else if (curr_name == "brdomain:description") {
 	  map< string, string, less<string> > description_attributes;
	  description_attributes = get_attributes(child);
 	  pend.description_lang = description_attributes["lang"];
	  pend.description = str_transcode(child->getTextContent());
	}
      }

      rsp->insert_doc_pending(pend);

    } else if (elem_name == "brdomain:dns") {
      struct PendingDns pend;

      // attribute
      map< string, string, less<string> > attributes = get_attributes(n);
      pend.status = attributes["status"];

      // brdomain:dns child elements
      map< string, string, less<string> > children = get_children_simple(n);
      pend.hostName = children["brdomain:hostName"];
      pend.limit    = children["brdomain:limit"];

      rsp->insert_dns_pending(pend);

    } else if (elem_name == "brdomain:releaseProc") {
	  
      struct PendingReleaseProc pend;

      // attribute
      map< string, string, less<string> > attributes = get_attributes(n);
      pend.status = attributes["status"];

      // brdomain:releaseProc child element
      map< string, string, less<string> > children = get_children_simple(n);
      pend.limit = children["brdomain:limit"];

      rsp->set_rel_pending(pend);

    } else if (elem_name == "brdomain:ticketNumberConc") {

      int tkt = atoi(str_transcode(n->getTextContent()).c_str());
      rsp->insert_concurrent_ticket(tkt);

    } 
  }
  
  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
	 child = child->getNextSibling()) {
      fill_brdomain_create_rsp(child, rsp);
    }
  }
}

void DomParser::fill_brdomain_renew_rsp(DOMNode *n, BrDomainRenewRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "brdomain:renData") {
      look_children = true;
    } else if (elem_name == "brdomain:publicationStatus") {
      // attribute
      map< string, string, less<string> > attributes = get_attributes(n);
      rsp->set_publication_flag(attributes["publicationFlag"]);
      look_children = true;
    } else if (elem_name == "brdomain:onHoldReason") {
      rsp->insert_onhold_reason(str_transcode(n->getTextContent()));
    }
  }
  
  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
	 child = child->getNextSibling()) {
      fill_brdomain_renew_rsp(child, rsp);
    }
  }
}

void DomParser::fill_brdomain_update_rsp(DOMNode *n, BrDomainUpdateRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "brdomain:updData") {
      look_children = true;
    } else if (elem_name == "brdomain:ticketNumber") {
      rsp->set_ticketNumber(atoi(str_transcode(n->getTextContent()).c_str()));
    } else if (elem_name == "brdomain:pending") {
      look_children = true;
    } else if (elem_name == "brdomain:doc") {
      struct PendingDoc pend;

      // attribute
      map< string, string, less<string> > attributes = get_attributes(n);
      pend.status = attributes["status"];

      // brdomain:doc child elements
      DOMNode *child;
      for (child = n->getFirstChild(); child != 0;
	   child = child->getNextSibling()) {
	if (child->getNodeType() != DOMNode::ELEMENT_NODE) {
	  continue;
	}
	string curr_name = str_transcode(child->getNodeName());
	if (curr_name == "brdomain:docType") {
	  pend.docType = str_transcode(child->getTextContent());
	} else if (curr_name == "brdomain:limit") {
	  pend.limit = str_transcode(child->getTextContent());
	} else if (curr_name == "brdomain:description") {
 	  map< string, string, less<string> > description_attributes;
	  description_attributes = get_attributes(child);
 	  pend.description_lang = description_attributes["lang"];
	  pend.description = str_transcode(child->getTextContent());
	}
      }

      rsp->insert_doc_pending(pend);

    } else if (elem_name == "brdomain:dns") {
      struct PendingDns pend;

      // attribute
      map< string, string, less<string> > attributes = get_attributes(n);
      pend.status = attributes["status"];

      // brdomain:dns child elements
      map< string, string, less<string> > children = get_children_simple(n);
      pend.hostName = children["brdomain:hostName"];
      pend.limit    = children["brdomain:limit"];

      rsp->insert_dns_pending(pend);

    } else if (elem_name == "brdomain:releaseProc") {
	  
      struct PendingReleaseProc pend;

      // attribute
      map< string, string, less<string> > attributes = get_attributes(n);
      pend.status = attributes["status"];

      // brdomain:releaseProc child element
      map< string, string, less<string> > children = get_children_simple(n);
      pend.limit = children["brdomain:limit"];

      rsp->set_rel_pending(pend);

    } else if (elem_name == "brdomain:hostStatus") {
      map< string, string, less<string> > children = get_children_simple(n);
      struct HostStatus hs;
      hs.host = children["brdomain:hostName"];
      hs.status =  children["brdomain:dnsAnswer"];
      rsp->insert_host_status(hs);
    
    } else if (elem_name == "brdomain:publicationStatus") {
      // attribute
      map< string, string, less<string> > attributes = get_attributes(n);
      rsp->set_publication_flag(attributes["publicationFlag"]);
      look_children = true;
    
    } else if (elem_name == "brdomain:onHoldReason") {
      
      rsp->insert_onhold_reason(str_transcode(n->getTextContent()));
      
    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
	 child = child->getNextSibling()) {
      fill_brdomain_update_rsp(child, rsp);
    }
  }

}

void DomParser::fill_brdomain_pandata_rsp(DOMNode *n, BrDomainPanDataRsp *rsp)
{
  bool look_children = false;
  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "brdomain:panData") {
      look_children = true;
    } else if (elem_name == "brdomain:ticketNumber") {
      rsp->set_ticketNumber(atoi(str_transcode(n->getTextContent()).c_str()));
    } else if (elem_name == "brdomain:reason") {
      // Get the lang attribute
      map< string, string, less<string> > attributes = get_attributes(n);
      if (attributes["lang"] != "") {
	rsp->set_reason_lang(attributes["lang"]);
      }

      rsp->set_reason(str_transcode(n->getTextContent()));
    }
  }
  
  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
	 child = child->getNextSibling()) {
      fill_brdomain_pandata_rsp(child, rsp);
    }
  }
}
#endif //USE_BR_DOMAINS

LIBEPP_NICBR_NS_END
