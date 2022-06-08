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
/* $Id: DomParserCommon.cpp 1044 2009-10-19 14:39:10Z koji $ */

#include "config.h"

#include <map>

#include <xercesc/framework/MemBufInputSource.hpp>

#include "DomParserCommon.H"
#include "DomErrorHandler.H"
#include "XmlException.H"
#include "StrUtil.H"

using std::map;

XERCES_CPP_NAMESPACE_USE

LIBEPP_NICBR_NS_BEGIN

DomParserCommon::DomParserCommon(): _parser(0)
{
  try {
    XMLPlatformUtils::Initialize();
  } catch (const XMLException& to_catch) {
    // throws XmlException
    string message = str_transcode(to_catch.getMessage());
    throw XmlException(XmlException::PARSER_INITIALIZE_EXCEPTION,
		       "Could not initialize the XML parser", message);
  }

  /// Instantiate the DOM parser.
  _parser = new XercesDOMParser();

  _parser->setErrorHandler(&_error_handler);
}

DomParserCommon::~DomParserCommon()
{
  delete _parser;

  // Call the termination method
  XMLPlatformUtils::Terminate();
}

void DomParserCommon::enable_validation(const string& schemas_dir)
{
  string local_schemas_dir = schemas_dir;
  if (schemas_dir == "")
    local_schemas_dir = SCHEMASDIR;

  _parser->setDoNamespaces(true);
  _parser->setDoSchema(true);
  _parser->setValidationSchemaFullChecking(true);
  _parser->setValidationScheme(XercesDOMParser::Val_Always);
  
  string schema_location =
    "urn:ietf:params:xml:ns:epp-1.0 " + 
    local_schemas_dir + "/epp-1.0.xsd "
    "urn:ietf:params:xml:ns:eppcom-1.0 " + 
    local_schemas_dir + "/eppcom-1.0.xsd "
    "urn:ietf:params:xml:ns:contact-1.0 " + 
    local_schemas_dir + "/contact-1.0.xsd "
    "urn:ietf:params:xml:ns:domain-1.0 " + 
    local_schemas_dir + "/domain-1.0.xsd "
    "urn:ietf:params:xml:ns:host-1.0 " + 
    local_schemas_dir + "/host-1.0.xsd " +
    "urn:ietf:params:xml:ns:brdomain-1.0 " +
    local_schemas_dir + "/brdomain-1.0.xsd "
    "urn:ietf:params:xml:ns:brorg-1.0 " +
    local_schemas_dir + "/brorg-1.0.xsd " +
    "urn:ietf:params:xml:ns:secDNS-1.0 " +
    local_schemas_dir + "/secDNS-1.0.xsd " +
    "urn:ietf:params:xml:ns:ipnetwork-1.0 " +
    local_schemas_dir + "/ipnetwork-1.0.xsd "
    "urn:ietf:params:xml:ns:asn-1.0 " +
    local_schemas_dir + "/asn-1.0.xsd";

  _parser->setExternalSchemaLocation(schema_location.c_str());
}

void DomParserCommon::disable_validation()
{
  _parser->setDoNamespaces(false);
  _parser->setDoSchema(false);
  _parser->setValidationSchemaFullChecking(false);
  _parser->setValidationScheme(XercesDOMParser::Val_Never);
}

string DomParserCommon::str_transcode(const XMLCh *const to_transcode)
{
  // XMLString::transcode() already converts from UTF-8 to the local
  // code-page, which we're assuming is ISO-8859-1.
  char *str = XMLString::transcode(to_transcode);
  string ret_str;
  if (str == 0) {
    ret_str = "";
  } else {
    ret_str = str;
  }
  XMLString::release(&str);
  return ret_str;
}

void DomParserCommon::fill_postal_info(DOMNode *n, PostalInfo *postal_info)
{
  bool look_children = false;
  if (str_transcode(n->getNodeName()) == "contact:postalInfo") {
    look_children = true;

    map< string, string, less<string> > attributes = get_attributes(n);
    if (attributes["type"] != "") {
      postal_info->set_type(attributes["type"]);
    }
  }

  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "contact:name") {
      postal_info->set_name(str_transcode(n->getTextContent()));
    } else if (elem_name == "contact:org") {
      postal_info->set_org(str_transcode(n->getTextContent()));
    } else if (elem_name == "contact:addr") {
      look_children = true;
    } else if (elem_name == "contact:street") {
      if (postal_info->get_str1() == "") {
	postal_info->set_str1(str_transcode(n->getTextContent()));
      } else if (postal_info->get_str2() == "") {
	postal_info->set_str2(str_transcode(n->getTextContent()));
      } else if (postal_info->get_str3() == "") {
	postal_info->set_str3(str_transcode(n->getTextContent()));
      }
    } else if (elem_name == "contact:city") {
      postal_info->set_city(str_transcode(n->getTextContent()));
    } else if (elem_name == "contact:sp") {
      postal_info->set_sp(str_transcode(n->getTextContent()));
    } else if (elem_name == "contact:pc") {
      postal_info->set_pc(str_transcode(n->getTextContent()));
    } else if (elem_name == "contact:cc") {
      postal_info->set_cc(str_transcode(n->getTextContent()));
    }
  }

  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
	 child = child->getNextSibling()) {
      fill_postal_info(child, postal_info);
    }
  }
}

void DomParserCommon::fill_ds_info(DOMNode *n, DSInfo *ds_info) 
{
  bool look_children = false;

  if (n->getNodeType() == DOMNode::ELEMENT_NODE) {
    string elem_name = str_transcode(n->getNodeName());
    if (elem_name == "secDNS:dsData") {
      look_children = true;
    } else if (elem_name == "secDNS:keyTag") {
      ds_info->set_key_tag(atoi(str_transcode(n->getTextContent()).c_str()));
    } else if (elem_name == "secDNS:alg") {
      ds_info->set_algo(atoi(str_transcode(n->getTextContent()).c_str()));
    } else if (elem_name == "secDNS:digestType") {
      int digest_type = atoi(str_transcode(n->getTextContent()).c_str());
      ds_info->set_digest_type(digest_type);
    } else if (elem_name == "secDNS:digest") {
      ds_info->set_digest(str_transcode(n->getTextContent()));
    } else if (elem_name == "secDNS:maxSigLife") {
      ds_info->set_max_sig_life(atoi(str_transcode(n->getTextContent()).c_str()));
    } else if (elem_name == "secDNS:keyData") {
      DSInfo::KeyData key_data;
      DOMNode *child;

      for (child = n->getFirstChild(); child != 0;
	   child = child->getNextSibling()) {
	if (child->getNodeType() != DOMNode::ELEMENT_NODE) {
	  continue;
	}
	string curr_name = str_transcode(child->getNodeName());
	if (curr_name == "secDNS:flags") {
	  key_data._flags =
	    atoi(str_transcode(child->getTextContent()).c_str());
	} else if (curr_name == "secDNS:protocol") {
	  key_data._protocol =
	    atoi(str_transcode(child->getTextContent()).c_str());
	} else if (curr_name == "secDNS:alg") {
	  key_data._algorithm = 
	    atoi(str_transcode(child->getTextContent()).c_str());
	} else if (curr_name == "secDNS:pubKey") {
	  key_data._pub_key = str_transcode(child->getTextContent());
	}       
      }
     
      ds_info->set_key_data(key_data);
    }
  }
 
  if (look_children) {
    DOMNode *child;
    for (child = n->getFirstChild(); child != 0;
	 child = child->getNextSibling()) {
      fill_ds_info(child, ds_info);
    }
  }
}

#if USE_IP_MANAGEMENT
IpRange DomParserCommon::fill_ipRange(DOMNode *n)
{
  IpRange ipRange;
  
  map< string, string, less<string> > attributes = get_attributes(n);

  if (attributes["version"] != "") {
    ipRange.set_version(attributes["version"]);
  }
  
  if (attributes["avail"] != "") {
    if (attributes["avail"] == "1") {
      ipRange.set_available(true);
    } else {
      ipRange.set_available(false);
    }
  }

  DOMNode *child;
  for (child = n->getFirstChild(); child != 0; child = child->getNextSibling()) {
    string e_name = str_transcode(child->getNodeName());
    if (e_name == "ipnetwork:startAddress") {
      ipRange.set_ipBegin(str_transcode(child->getTextContent()));
    } else if (e_name == "ipnetwork:endAddress") {
      ipRange.set_ipEnd(str_transcode(child->getTextContent()));
    }
  }

  return ipRange;
}

ReverseDns DomParserCommon::fill_reverseDns(DOMNode *n)
{
  ReverseDns reverseDns;
  DOMNode *child;
  for (child = n->getFirstChild(); child != 0;
       child = child->getNextSibling()) {
    string e_name = str_transcode(child->getNodeName()); 
    if (e_name == "ipnetwork:ipRange") {
      reverseDns.ipRange = fill_ipRange(child);
    } else if (e_name == "ipnetwork:hostName") {
      reverseDns.nameservers.push_back(str_transcode(child->getTextContent()));
    }
  }
  
  return reverseDns;
}
#endif // USE_IP_MANAGEMENT

#if USE_BR_ORG
IpRange DomParserCommon::fill_brorg_ipRange(DOMNode *n)
{
  IpRange ipRange;
  
  map< string, string, less<string> > attributes = get_attributes(n);

  if (attributes["version"] != "") {
    ipRange.set_version(attributes["version"]);
  }
  
  if (attributes["avail"] != "") {
    if (attributes["avail"] == "1") {
      ipRange.set_available(true);
    } else {
      ipRange.set_available(false);
    }
  }

  DOMNode *child;
  for (child = n->getFirstChild(); child != 0; child = child->getNextSibling()) {
    string e_name = str_transcode(child->getNodeName());
    if (e_name == "brorg:startAddress") {
      ipRange.set_ipBegin(str_transcode(child->getTextContent()));
    } else if (e_name == "brorg:endAddress") {
      ipRange.set_ipEnd(str_transcode(child->getTextContent()));
    }
  }

  return ipRange;
}
#endif // USE_BR_ORG

map< string, string, less<string> > DomParserCommon::get_attributes(DOMNode *n)
{
  map< string, string, less<string> > attributes;

  if (n->getNodeType() != DOMNode::ELEMENT_NODE) {
    return attributes;
  }
  
  DOMNamedNodeMap *attrs = n->getAttributes();
  for (unsigned int i = 0; i < attrs->getLength(); ++i) {
    DOMAttr *attr_node = (DOMAttr*) attrs->item(i);
    string curr_name = str_transcode(attr_node->getName());
    attributes[curr_name] = str_transcode(attr_node->getValue());
  }

  return attributes;
}

map< string, string, less<string> > DomParserCommon::get_children_simple(DOMNode *n)
{
  map< string, string, less<string> > children;

  if (n->getNodeType() != DOMNode::ELEMENT_NODE) {
    return children;
  }

  DOMNode *child;
  for (child = n->getFirstChild(); child != 0;
       child = child->getNextSibling()) {
    if (child->getNodeType() != DOMNode::ELEMENT_NODE) {
      continue;
    }
    string curr_name = str_transcode(child->getNodeName());
    children[curr_name] = str_transcode(child->getTextContent());
  }

  return children;
}

DOMNode *DomParserCommon::parse(const string &xml_payload)
{
  char *sysId = (char *)"";

  MemBufInputSource membuf((const XMLByte*)xml_payload.c_str(),
			   xml_payload.length(), sysId); 

  DOMDocument *doc = 0;
  try {
    // Reset document vector pool and release memory back to the system
    _parser->resetDocumentPool();

    _parser->parse(membuf);
    doc = _parser->getDocument();
  } catch(const XmlException &e) { 
    // if it is a XmlException, it was thrown by the ErrorHandler
    // just rethrow
    throw;
  } catch (...) {
    throw XmlException(XmlException::UNEXPECTED_PARSER_EXCEPTION,
				    "An unexpected exception occurred in XML "
				    "Parser");
  }
  
  if(doc == 0 || doc->getDocumentElement() == 0) {
    throw XmlException(XmlException::NULL_XML_DOCUMENT,
		       "The XML Parser returned a NULL document");
  }

  return (DOMNode*)doc->getDocumentElement();
}

LIBEPP_NICBR_NS_END
