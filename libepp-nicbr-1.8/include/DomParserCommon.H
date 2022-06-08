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
/* $Id: DomParserCommon.H 1041 2009-10-16 19:05:48Z koji $ */
/** @file DomParser.H
 *  @brief EPP DomParser Class
 */

#ifndef __DOMPARSERCOMMON_H__
#define __DOMPARSERCOMMON_H__

#include <string>
#include <map>
#include <memory>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include "DomErrorHandler.H"
#include "CommonData.H"
#include "DSInfo.H"

#include "libepp_nicbr.H"

using std::string;
using std::map;
using std::less;
using std::auto_ptr;

XERCES_CPP_NAMESPACE_USE

LIBEPP_NICBR_NS_BEGIN

/// EPP DomParser Class
class DomParserCommon
{
public:
  /// Constructor
  DomParserCommon();

  /// Destructor
  ~DomParserCommon();

  /// Enable Validation
  /**
     @param schemas_dir Path to the schemas directory
   */
  void enable_validation(const string& schemas_dir = SCHEMASDIR);

  /// Disable Validation
  void disable_validation();

  /// Transcodes a XMLCh to string
  /**
     @param to_transcode String to be transcoded
     @return Transcoded string
   */
  static string str_transcode(const XMLCh *const to_transcode);

protected:
  /// Do the actual parsing of the XML document
  /**
     @param xml_payload XML to be parsed
     @return Pointer to the root of the document tree
   */
  DOMNode *parse(const string &xml_payload);

  /// Fills a Postal Info struct
  /**
     @param n Pointer to a node of the document tree
     @param postal_info Pointer to a postal info struct
  */
  void fill_postal_info(DOMNode *n, PostalInfo *postal_info);
  
  /// Fills a DSInfo object
  /**
     @param n reference to a node of the document tree
     @param ds_info reference to a DSInfo object
  */
  void fill_ds_info(DOMNode *n, DSInfo *ds_info);
  
#if USE_IP_MANAGEMENT
  /// Load IP Range object
  /**
     @param n Pointer to a node of the document tree
  */
  IpRange fill_ipRange(DOMNode *n);

  /// Load reverse DNS object
  /**
     @param n Pointer to a node of the document tree
  */
  ReverseDns fill_reverseDns(DOMNode *n);
#endif // USE_IP_MANAGEMENT

#if USE_BR_ORG
  /// Load IP Range object for Organization
  /**
     @param n Pointer to a node of the document tree
  */
  IpRange fill_brorg_ipRange(DOMNode *n);
#endif // USE_BR_ORG

  /// Fills a map of children names with associated content text
  /**
     @param n   Pointer to a node of the document tree
     @return    map of children names with associated content text
   */
  map< string, string, less<string> > get_children_simple(DOMNode *n);

  /// Fills a map of attributes and associated values
  /**
     @param n   Pointer to a node of the document tree
     @return    map of attributes and associated values
  */
  map< string, string, less<string> > get_attributes(DOMNode *n);

  /// Object for parsing XML documents
  XercesDOMParser *_parser;

  /// DOM error handler
  DomErrorHandler _error_handler;
};

LIBEPP_NICBR_NS_END
#endif //__DOMPARSERCOMMON_H__
