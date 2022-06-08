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
/* $Id: DomParser.H 1021 2009-05-11 18:56:22Z eduardo $ */
/** @file DomParser.H
 *  @brief EPP DomParser Class
 */

#ifndef __DOMPARSER_H__
#define __DOMPARSER_H__

#include "DomParserCommon.H"

#include "Response.H"
#include "Greeting.H"
#include "ContactCheckRsp.H"
#include "ContactCreateRsp.H"
#include "ContactInfoRsp.H"
#include "DomainCheckRsp.H"
#include "DomainCreateRsp.H"
#include "DomainInfoRsp.H"
#include "DomainRenewRsp.H"
#if USE_IP_MANAGEMENT
#include "IpNetworkCreateRsp.H"
#include "IpNetworkCheckRsp.H"
#include "IpNetworkRenewRsp.H"
#include "IpNetworkTransferRsp.H"
#include "IpNetworkInfoRsp.H"
#include "AsnCheckRsp.H"
#include "AsnCreateRsp.H"
#include "AsnRenewRsp.H"
#include "AsnTransferRsp.H"
#include "AsnInfoRsp.H"
#endif //USE_IP_MANAGEMENT
#include "PollRsp.H"
#include "PanDataRsp.H"
#if USE_BR_DOMAINS
#include "BrDomainCheckRsp.H"
#include "BrDomainInfoRsp.H"
#include "BrDomainCreateRsp.H"
#include "BrDomainRenewRsp.H"
#include "BrDomainUpdateRsp.H"
#include "BrDomainPanDataRsp.H"
#endif //USE_BR_DOMAINS
#if USE_BR_ORG
#include "BrOrgInfoRsp.H"
#include "BrOrgCheckRsp.H"
#include "BrOrgPanDataRsp.H"
#include "BrOrgCreateRsp.H"
#endif //USE_BR_ORG

LIBEPP_NICBR_NS_BEGIN

/// EPP DomParser Class
class DomParser : public DomParserCommon
{
public:
  /// Parse method to be used by the Unit Test to validate commands
  /**
     @param xml_payload Command (in XML format) to be validated
   */
  void parse_command(const string &xml_payload);

  /// Parse the response to a login command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a response
   */
  void parse_login_rsp(const string &xml_payload, Response *rsp);

  /// Parse the response to a logout command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a response
   */
  void parse_logout_rsp(const string &xml_payload, Response *rsp);

  /// Parse the response to a connect or hello command
  /**
     @param xml_payload XML to be parsed
     @param greeting Pointer to a greeting response
   */
  void parse_greeting(const string &xml_payload, Greeting *greeting);

  /// Parse the response to a contact check command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a contact check response
   */
  void parse_contact_check_rsp(const string &xml_payload,
			       ContactCheckRsp *rsp);

  /// Parse the response to a contact create command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a contact create response
   */
  void parse_contact_create_rsp(const string &xml_payload, 
				ContactCreateRsp *rsp);

  /// Parse the response to a contact info command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a contact info response
   */
  void parse_contact_info_rsp(const string &xml_payload, ContactInfoRsp *rsp);

  /// Parse the response to a contact update command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a response
   */
  void parse_contact_update_rsp(const string &xml_payload, Response *rsp);

  /// Parse the response to a domain check command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a domain check response
   */
  void parse_domain_check_rsp(const string &xml_payload, DomainCheckRsp *rsp);

  /// Parse the response to a domain create command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a domain create response
   */
  void parse_domain_create_rsp(const string &xml_payload,
			       DomainCreateRsp *rsp);

  /// Parse the response to a domain info command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a domain info response
   */
  void parse_domain_info_rsp(const string &xml_payload, DomainInfoRsp *rsp);

  /// Parse the response to a domain renew command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a domain renew response
   */
  void parse_domain_renew_rsp(const string &xml_payload, DomainRenewRsp *rsp);

  /// Parse the response to a domain update command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a response
   */
  void parse_domain_update_rsp(const string &xml_payload, Response *rsp);

  /// Parse the response to a domain delete command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a response
   */
  void parse_domain_delete_rsp(const string &xml_payload, Response *rsp);

#if USE_IP_MANAGEMENT
  /// Parse the response to a ip range create command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a ip range create response
  */
  void parse_ipnetwork_create_rsp(const string &xml_payload, 
				IpNetworkCreateRsp *rsp);

  /// Parse the response to a ip range check command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a ip range check response
  */
  void parse_ipnetwork_check_rsp(const string &xml_payload, IpNetworkCheckRsp *rsp);

  /// Parse the response to a ip range delete command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a ip range delete response
  */
  void parse_ipnetwork_delete_rsp(const string &xml_payload, Response *rsp);

  /// Parse the response to a ip range renew command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a ip range renew response
  */
  void parse_ipnetwork_renew_rsp(const string &xml_payload, 
			       IpNetworkRenewRsp *rsp);

  /// Parse the response to a ip range transfer command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a ip range transfer response
  */
  void parse_ipnetwork_transfer_rsp(const string &xml_payload, 
				  IpNetworkTransferRsp *rsp);

  /// Parse the response to a ip range info command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a ip range info response
  */
  void parse_ipnetwork_info_rsp(const string &xml_payload, IpNetworkInfoRsp *rsp);

  /// Parse the response to a ip range update command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a ip range update response
  */
  void parse_ipnetwork_update_rsp(const string &xml_payload, Response *rsp);

  /// Parse the response to an asn check command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a asn check response
  */
  void parse_asn_check_rsp(const string &xml_payload, AsnCheckRsp *rsp);

  /// Parse the response to an asn create command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a asn create response
  */
  void parse_asn_create_rsp(const string &xml_payload, AsnCreateRsp *rsp);

  /// Parse the response to an asn delete command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a asn delete response
  */
  void parse_asn_delete_rsp(const string &xml_payload, Response *rsp);

  /// Parse the response to an asn renew command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a asn renew response
  */
  void parse_asn_renew_rsp(const string &xml_payload, AsnRenewRsp *rsp);

  /// Parse the response to an asn transfer command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a asn transfer response
  */
  void parse_asn_transfer_rsp(const string &xml_payload, AsnTransferRsp *rsp);

  /// Parse the response to an asn info command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a asn info response
  */
  void parse_asn_info_rsp(const string &xml_payload, AsnInfoRsp *rsp);

  /// Parse the response to an asn update command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a asn update response
  */
  void parse_asn_update_rsp(const string &xml_payload, Response *rsp);
#endif //USE_IP_MANAGEMENT

  /// Parse the response to a poll command
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a poll response
  */
  void parse_poll_rsp(const string &xml_payload, PollRsp *rsp);

protected:
  /// Parse the response
  /**
     @param xml_payload XML to be parsed
     @param rsp Pointer to a response
   */
  void parse_response(const string &xml_payload, Response *rsp);

  /// Gets data from an extValue element
  /**
     @param n Pointer to a node of the document tree
     @param value value sub-element of extValue
     @param xmlns value sub-element's attribute
     @param reason reason sub-element of extValue
   */
  void get_extValue_info(DOMNode *n, string &value, string &xmlns,
			 string &reason);

  /// Fills a result part of a Response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a Response object
  */
  void fill_result(DOMNode *n, Response *rsp);

  /// Fills a Response object 
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a Response object
   */
  void fill_response(DOMNode *n, Response *rsp);  

  /// Fills a Greeting object
  /**
     @param n Pointer to a node of the document tree
     @param greeting Pointer to a greeting object
   */
  void fill_greeting(DOMNode *n, Greeting *greeting);

  /// Fills a Contact check response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a contact check response
   */
  void fill_contact_check_rsp(DOMNode *n, ContactCheckRsp *rsp);

  /// Fills a Contact info response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a contact info response
   */
  void fill_contact_info_rsp(DOMNode *n, ContactInfoRsp *rsp);

  /// Fills a Contact create response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a contact create response
   */
  void fill_contact_create_rsp(DOMNode *n, ContactCreateRsp *rsp);  

  /// Fills a Contact PanData response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a PanData response
   */
  void fill_contact_pandata_rsp(DOMNode *n, PanDataRsp *rsp);

  /// Fills a Domain check response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a domain check response
   */
  void fill_domain_check_rsp(DOMNode *n, DomainCheckRsp *rsp);

  /// Fills a Domain info response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a domain info response
   */
  void fill_domain_info_rsp(DOMNode *n, DomainInfoRsp *rsp);
  
  /// Fills a Domain create response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a domain create response
   */
  void fill_domain_create_rsp(DOMNode *n, DomainCreateRsp *rsp);
  
  /// Fills a Domain renew response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a domain renew response
  */
  void fill_domain_renew_rsp(DOMNode *n, DomainRenewRsp *rsp);

  /// Fills a Domain PanData response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a PanData response
   */
  void fill_domain_pandata_rsp(DOMNode *n, PanDataRsp *rsp);

#if USE_IP_MANAGEMENT
  /// Fills a ip range create response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a ip range create response
   */ 
  void fill_ipnetwork_create_rsp(DOMNode *n, IpNetworkCreateRsp *rsp);

  /// Fills a ip range check response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a ip range check response
   */ 
  void fill_ipnetwork_check_rsp(DOMNode *n, IpNetworkCheckRsp *rsp);

  /// Fills a ip range renew response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a ip range renew response
   */ 
  void fill_ipnetwork_renew_rsp(DOMNode *n, IpNetworkRenewRsp *rsp);

  /// Fills a ip range transfer response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a ip range transfer response
   */ 
  void fill_ipnetwork_transfer_rsp(DOMNode *n, IpNetworkTransferRsp *rsp);

  /// Fills a ip range info response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a ip range info response
   */ 
  void fill_ipnetwork_info_rsp(DOMNode *n, IpNetworkInfoRsp *rsp);

  /// Fills an asn check response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to an asn check response
   */ 
  void fill_asn_check_rsp(DOMNode *n, AsnCheckRsp *rsp);

  /// Fills an asn create response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to an asn create response
   */ 
  void fill_asn_create_rsp(DOMNode *n, AsnCreateRsp *rsp);

  /// Fills an asn renew response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to an asn renew response
   */ 
  void fill_asn_renew_rsp(DOMNode *n, AsnRenewRsp *rsp);

  /// Fills an asn transfer response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to an asn transfer response
   */ 
  void fill_asn_transfer_rsp(DOMNode *n, AsnTransferRsp *rsp);

  /// Fills an asn info response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to an asn info response
   */ 
  void fill_asn_info_rsp(DOMNode *n, AsnInfoRsp *rsp);
#endif //USE_IP_MANAGEMENT

  /// Fills a Poll response object
  /**
     @param n Pointer to a node of the document tree
     @param poll_rsp Pointer to a poll response
  */
  void fill_poll_rsp(DOMNode *n, PollRsp *poll_rsp);

#if USE_BR_ORG
  /// Fills a BrOrg Info response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a BrOrg Info response
   */
  void fill_brorg_info_rsp(DOMNode *n, BrOrgInfoRsp *rsp);

  /// Fills a BrOrg Check response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a BrOrg Check response
   */
  void fill_brorg_check_rsp(DOMNode *n, BrOrgCheckRsp *rsp);

  /// Fills a BrOrg PanData response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a BrOrgPanData response
   */
  void fill_brorg_pandata_rsp(DOMNode *n, BrOrgPanDataRsp *rsp);

  /// Fills a BrOrgCreate response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a BrOrgCreate response
   */
  void fill_brorg_create_rsp(DOMNode *n, BrOrgCreateRsp *rsp);
#endif //USE_BR_ORG

#if USE_BR_DOMAINS
  /// Fills a BrDomain check response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a BrDomain check response
   */
  void fill_brdomain_check_rsp(DOMNode *n, BrDomainCheckRsp *rsp);
  
  /// Fills a BrDomain info response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a domain info response
   */
  void fill_brdomain_info_rsp(DOMNode *n, BrDomainInfoRsp *rsp);
  
  /// Fills a BrDomain create response object
  /**
     @param n Pointer to a node of the document tree
     @param d_rsp Pointer to a BrDomain create response
   */
  void fill_brdomain_create_rsp(DOMNode *n, BrDomainCreateRsp *rsp);
  
  /// Fills a BrDomain renew response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a domain renew response
  */
  void fill_brdomain_renew_rsp(DOMNode *n, BrDomainRenewRsp *rsp);
  
  /// Fills a BrDomain update response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a BrDomain update response
  */
  void fill_brdomain_update_rsp(DOMNode *n, BrDomainUpdateRsp *rsp);

  /// Fills a BrDomain PanData response object
  /**
     @param n Pointer to a node of the document tree
     @param rsp Pointer to a BrDomainPanData response
   */
  void fill_brdomain_pandata_rsp(DOMNode *n, BrDomainPanDataRsp *rsp);
#endif //USE_BR_DOMAINS
};

LIBEPP_NICBR_NS_END
#endif //__DOMPARSER_H__
