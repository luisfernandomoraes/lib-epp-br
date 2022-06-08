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
/* $Id: DomainFunctions.H 1006 2009-02-19 20:29:57Z rafael $ */
/** @file DomainFunctions.H
 *  @brief EPP Domain related functions
 */
#ifndef __DOMAIN_FUNCTIONS_H__
#define __DOMAIN_FUNCTIONS_H__
#include "SheppCommandFunctions.H"

#if USE_BR_DOMAINS
#include "BrDomainCheck.H"
#include "BrDomainInfo.H"
#include "BrDomainCreate.H"
#include "BrDomainUpdate.H"
#include "BrDomainRenew.H"
#else
#include "DomainCheck.H"
#include "DomainInfo.H"
#include "DomainCreate.H"
#include "DomainUpdate.H"
#include "DomainRenew.H"
#endif //USE_BR_DOMAINS

#include "DomainDelete.H"
#include "RegistrationPeriod.H"

/// print domain command usage info
/**
   @param error_msg   error message to be printed
   @param specific    optional, choses specific sub-command
   @return 0 if ok, -1 otherwise
*/
int cmd_domain_help(string error_msg, string specific = "")
{
  if (error_msg != "") {
    printf("error: %s\n", error_msg.c_str());
  }

  printf("domain command syntax help:\n");
  printf("\n");

  if (specific == "" || specific == "check") {
    printf("  check <domain1> [domain2 ... domainN]"
#if USE_BR_DOMAINS
	   " [-o organization]"
#endif //USE_BR_DOMAINS
	   "\n");
   
    printf("\n");
  }
  if (specific == "" || specific == "info") {
    printf("  info <domain> [-auth authInfoPw[:authInfoRoid]]"
#if USE_BR_DOMAINS
	   " [-t ticketNumber]"
#endif //USE_BR_DOMAINS
	   "\n");
    printf("\n");
  }
  if (specific == "" || specific == "transfer") {
    printf("  transfer: not implemented\n");
    printf("\n");
  }
  if (specific == "" || specific == "create") {
    printf("  create <domain> [-period time:unit]\n"
	   "    [-ns nameserver[,ipversion:ipaddr][, ...] ...] "
	   "[-r registrant]"
	   "\n    [-contact type=value ...] [-auth authInfoPw[:authInfoRoid]]"
	   "\n    [-ds <keyTag> <alg> <digestType> <digest> "
	   "[-maxlife <maxSigLife>]"
	   "\n         [-keyData <flags> <protocol> <alg> <pubKey>] ...] "
#if USE_BR_DOMAINS
	   "\n    <-o organization> [-rpflags flag1:flag2:flag3]"
	   "\n    [-auto-renew <on|off>]"
#endif //USE_BR_DOMAINS
           "\n");

    printf("\n");
  }
  if (specific == "" || specific == "delete") {
    printf("  delete <domain>\n");
    printf("\n");
  }
  if (specific == "" || specific == "renew") {
    printf("  renew <domain> <-expdate curExpDate> [-period time:unit]\n");
    printf("\n");
  }
  if (specific == "" || specific == "update") {
    printf("  update <domain>\n"
	   "    [-add-ns nameserver[,ipversion:ipaddr][, ...] ...]\n"
	   "    [-add-contact type=value ...]\n"
	   "    [-add-status status:lang:\"description\" ...]\n"
	   "    [-add-ds <keyTag> <alg> <digestType> <digest> "
	   "[-maxlife <maxSigLife>]\n"
	   "        [-keyData <flags> <protocol> <alg> <pubKey>] ...]\n"
	   "    [-secDNS-urgent]\n"
	   "    [-rem-ns nameserver[,ipversion:ipaddr][, ...] ...]\n"
	   "    [-rem-contact type=value ...]\n"
	   "    [-rem-status status ...]\n"
	   "    [-rem-ds <keyTag>]\n"
	   "    [-chg-ds <keyTag> <alg> <digestType> <digest> "
	   "[-maxlife <maxSigLife>]\n"
	   "        [-keyData <flags> <protocol> <alg> <pubKey>] ...]\n"

	   "    [-r registrant] [-auth authInfoPw[:authInfoRoid]]"
	   
#if USE_BR_DOMAINS
 	   "\n    [-t ticketNumber] [-rpflags flag1:flag2:flag3]"
	   "\n    [-auto-renew <on|off>]"
#endif //USE_BR_DOMAINS
	   "\n");
    printf("\n");
  }

  if (error_msg != "") {
    return -1;
  }

  return 0;
}

/// domain check command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_domain_check(vector<string> &args)
{
#if USE_BR_DOMAINS
  BrDomainCheck act;
  BrDomainCheckCmd *cmd = act.get_command();
#else
  DomainCheck act;
  DomainCheckCmd *cmd = act.get_command();
#endif //USE_BR_DOMAINS
  string cmd_name = "check";

  while (!args.empty()) {
#if USE_BR_DOMAINS
    if (args[0] == "-o") {
      // set organization
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("organization parameter missing", cmd_name);
      }
      cmd->set_organization(args[0]);
      args.erase(args.begin());
    } else {
#endif //USE_BR_DOMAINS
      // insert domains
      cmd->insert_domain(args[0]);
      args.erase(args.begin());
#if USE_BR_DOMAINS
    }
#endif //USE_BR_DOMAINS
  }

  if (cmd->get_domains_list().empty()) {
    return cmd_domain_help("no domain names specified", cmd_name);
  }

  if (_debug) {
    set<string> domains = cmd->get_domains_list();
    set<string>::const_iterator it;
    printf("names to be checked:\n");
    for (it = domains.begin(); it != domains.end(); it++) {
      printf("  [%s]\n", (*it).c_str());
    }
    
#if USE_BR_DOMAINS
    if (cmd->get_organization() != "") {
      printf("  organization: [%s]\n", cmd->get_organization().c_str());
    }
#endif //USE_BR_DOMAINS
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// domain info command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_domain_info(vector<string> &args)
{
#if USE_BR_DOMAINS
  BrDomainInfo act;
  BrDomainInfoCmd *cmd = act.get_command();
#else
  DomainInfo act;
  DomainInfoCmd *cmd = act.get_command();
#endif //USE_BR_DOMAINS
  string cmd_name = "info";

  if (args.empty()) {
    return cmd_domain_help("no domain name specified", cmd_name);
  }
  
  // domain name MUST be the first attribute
  cmd->set_name(args[0]);
  args.erase(args.begin());

  while (!args.empty()) {
    if (args[0] == "-auth") {
      // set authInfo
      AuthInfo auth;
      if (SheppObjSet::authInfo(auth, args) != 0) {
	return cmd_domain_help("invalid auth", cmd_name);
      }
      cmd->set_authInfo(auth);
#if USE_BR_DOMAINS
    } else if (args[0] == "-t") {
      // ticketNumber
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("ticket number parameter missing", cmd_name);
      }
      if (atoi(args[0].c_str()) <= 0) {
	return cmd_domain_help("ticket parameter must be a positive integer",
			       cmd_name);
      }
      cmd->set_ticketNumber(atoi(args[0].c_str()));
      args.erase(args.begin());
#endif //USE_BR_DOMAINS
    } else {
      return cmd_domain_help("invalid syntax near \"" + args[0] + "\"",
			     cmd_name);
    }
  }

  if (_debug) {
    printf("name to get info: [%s]\n", cmd->get_name().c_str());

#if USE_BR_DOMAINS
    if (cmd->get_ticketNumber() != 0) {
      printf("  ticketNumber: [%d]\n", cmd->get_ticketNumber());
    }
#endif //USE_BR_DOMAINS
    
    SheppPrint::authInfo(cmd->get_authInfo());
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// domain transfer command function
/**
   @param args   vector with command-line arguments
   @return 0
*/
int cmd_domain_transfer(vector<string> &args)
{
  string cmd_name = "transfer";
  return cmd_domain_help("", cmd_name);
}

/// domain create command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_domain_create(vector<string> &args)
{
#if USE_BR_DOMAINS
  BrDomainCreate act;
  BrDomainCreateCmd *cmd = act.get_command();
#else
  DomainCreate act;
  DomainCreateCmd *cmd = act.get_command();
#endif //USE_BR_DOMAINS
  string cmd_name = "create";

  if (args.empty()) {
    return cmd_domain_help("domain name missing", cmd_name);
  }
  // domain name MUST be the first attribute
  cmd->set_name(args[0]);
  args.erase(args.begin());

  while (!args.empty()) {
    if (args[0] == "-period") {
      //registration period
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("period parameter missing", cmd_name);
      }

      string time;
      string unit;
      if (SheppStrUtil::split(args[0], time, unit, ":", false) != 0) {
	return cmd_domain_help("invalid period", cmd_name);
      }

      cmd->set_period(atoi(time.c_str()), unit);
      args.erase(args.begin());
    } else if (args[0] == "-ns") {
      //nameserver
      NameServer ns;
      if (SheppObjSet::nameServer(ns, args) != 0) {
	return cmd_domain_help("invalid nameserver", cmd_name);
      }
      cmd->insert_nameserver(ns);
    } else if (args[0] == "-r") {
      //registrant
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("registrant parameter missing", cmd_name);
      }
      cmd->set_registrant(args[0]);
      args.erase(args.begin());
    } else if (args[0] == "-contact") {
      //contact
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("contact parameter missing", cmd_name);
      }

      string type;
      string value;
      if (SheppStrUtil::split(args[0], type, value, "=", false) != 0) {
	return cmd_domain_help("invalid contact", cmd_name);
      }

      cmd->insert_contact(type, value);
      args.erase(args.begin());      

    } else if (args[0] == "-auth") {
      //authInfo
      AuthInfo auth;
      if (SheppObjSet::authInfo(auth, args) != 0) {
	return cmd_domain_help("invalid auth", cmd_name);
      }
      cmd->set_authInfo(auth);
    } else if (args[0] == "-ds") {
      //DNSSEC data
      DSInfo ds;
      string error_msg;
      if (SheppObjSet::dsInfo(ds, args, error_msg) != 0) {
	return cmd_domain_help(error_msg, cmd_name);
      }
      cmd->add_dsInfo(ds);
#if USE_BR_DOMAINS
    } else if (args[0] == "-o") {
      //organization
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("organization parameter missing", cmd_name);
      }
      cmd->set_organization(args[0]);
      args.erase(args.begin());
    } else if (args[0] == "-rpflags") {
      //releaseProcessFlags
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("rpflags parameter missing", cmd_name);
      }
      ReleaseProcessFlags rpf;
      string flag1;
      string flag2;
      string flag3;

      if (SheppStrUtil::split(args[0], flag1, flag2, ":", false) != 0) {
	return cmd_domain_help("invalid rpflags", cmd_name);
      }

      if (SheppStrUtil::split(flag2, flag2, flag3, ":", false) != 0) {
	return cmd_domain_help("invalid rpflags", cmd_name);
      }
      
      rpf.flag1 = (flag1 == "0") ? 0 : 1;
      rpf.flag2 = (flag2 == "0") ? 0 : 1;
      rpf.flag3 = (flag3 == "0") ? 0 : 1;

      cmd->set_releaseProcessFlags(rpf);
      args.erase(args.begin());
    } else if (args[0] == "-auto-renew") {
      //auto-renew
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("auto-renew parameter missing", cmd_name);
      }
      if (args[0] == "on") {
	cmd->set_auto_renew(1);
      } else if (args[0] == "off") {
	cmd->set_auto_renew(0);
      } else {
	return cmd_domain_help("invalid auto-renew", cmd_name);
      }
      args.erase(args.begin());
#endif //USE_BR_DOMAINS
    } else {
      return cmd_domain_help("invalid syntax near \"" + args[0] + "\"",
			     cmd_name);
    }
  }
#if USE_BR_DOMAINS
  if (cmd->get_organization() == "") {
    return cmd_domain_help("organization is mandatory", cmd_name);
  }
#endif //USE_BR_DOMAINS

  if (_debug) {
    printf("creating domain: %s\n", cmd->get_name().c_str());
    
#if USE_BR_DOMAINS
    printf("  organization: %s\n", cmd->get_organization().c_str());
#endif //USE_BR_DOMAINS
    
    if (cmd->get_period().time != 0) {
      printf("  period: %d %s\n", cmd->get_period().time,
	     cmd->get_period().unit.c_str());
    }
    
    vector<NameServer> nss = cmd->get_nameservers();
    vector<NameServer>::const_iterator nsit;
    for (nsit = nss.begin(); nsit != nss.end(); nsit++) {
      SheppPrint::nameserver(*nsit);
    }
    
    if (cmd->get_registrant() != "") {
      printf("  registrant: %s\n", cmd->get_registrant().c_str());
    }
    
    map< string, string, less<string> > contacts = cmd->get_contacts();
    map< string, string, less<string> >::const_iterator cit;
    for (cit = contacts.begin(); cit != contacts.end(); cit++) {
      printf("  contact %s: %s\n", (*cit).first.c_str(), (*cit).second.c_str());
    }
    
    SheppPrint::authInfo(cmd->get_authInfo());
    
#if USE_BR_DOMAINS
    if (cmd->get_releaseProcessFlags().flag1 == 1 ||
	cmd->get_releaseProcessFlags().flag2 == 1 ||
	cmd->get_releaseProcessFlags().flag3 == 1) {
      printf("  releaseProcessFlags: %d, %d, %d\n",
	     cmd->get_releaseProcessFlags().flag1,
	     cmd->get_releaseProcessFlags().flag2,
	     cmd->get_releaseProcessFlags().flag3);
    }

    printf("  auto-renew: %d\n", cmd->get_auto_renew());
#endif //USE_BR_DOMAINS
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }
  
  return 0;
}

/// domain check delete function
/**
   @param args   vector with command-line arguments
   @return 0
*/
int cmd_domain_delete(vector<string> &args)
{
  DomainDelete act;
  DomainDeleteCmd *cmd = act.get_command();

  string cmd_name = "delete";

  if (args.empty()) {
    return cmd_domain_help("domain name missing", cmd_name);
  }
  
  // domain name MUST be the first attribute
  cmd->set_name(args[0]);
  args.erase(args.begin());

  if (_debug) {
    printf("deleting domain: %s\n", cmd->get_name().c_str());
  }

  if (process_action(act) != 0) {
    return -1;
  }
  
  return 0;
}

/// domain renew command function
/**
   @param args   vector with command-line arguments
   @return 0
*/
int cmd_domain_renew(vector<string> &args)
{
#if USE_BR_DOMAINS
  BrDomainRenew act;
#else
  DomainRenew act;
#endif //USE_BR_DOMAINS
  DomainRenewCmd *cmd = act.get_command();
  string cmd_name = "renew";

  if (args.empty()) {
    return cmd_domain_help("domain name missing", cmd_name);
  }
  // domain name MUST be the first attribute
  cmd->set_name(args[0]);
  args.erase(args.begin());

  while (!args.empty()) {
    if (args[0] == "-expdate") {
      //-expdate
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("curExpDate missing", cmd_name);
      }
      cmd->set_curExpDate(args[0]);
      args.erase(args.begin());      
    } else if (args[0] == "-period") {
      args.erase(args.begin());      
      if (args.empty()) {
	return cmd_domain_help("period missing", cmd_name);
      }
      string str_time;
      string unit;
      if (SheppStrUtil::split(args[0], str_time, unit, ":", false) != 0) {
	return cmd_domain_help("error setting period", cmd_name);
      }
      RegistrationPeriod period;
      period.time = atoi(str_time.c_str());
      period.unit = unit;
      cmd->set_period(period);
      args.erase(args.begin());
    } else {
      return cmd_domain_help("invalid syntax near \"" + args[0] + "\"",
			     cmd_name);
    }
  }

  if (cmd->get_curExpDate() == "") {
    return cmd_domain_help("curExpDate is mandatory", cmd_name);
  }

  if (_debug) {
    printf("domain to renew: [%s]\n", cmd->get_name().c_str());
    printf("  curExpDate   : [%s]\n", cmd->get_curExpDate().c_str());
    if (cmd->get_period().time != 0 || cmd->get_period().unit != "") {
      printf("  period       : [%d %s]\n", cmd->get_period().time,
	     cmd->get_period().unit.c_str());
    }
  } //_debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// domain update command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_domain_update(vector<string> &args)
{
#if USE_BR_DOMAINS
  BrDomainUpdate act;
  BrDomainUpdateCmd *cmd = act.get_command();
#else
  DomainUpdate act;
  DomainUpdateCmd *cmd = act.get_command();
#endif //USE_BR_DOMAINS
  string cmd_name = "update";

  if (args.empty()) {
    return cmd_domain_help("domain name missing", cmd_name);
  }
  // domain name MUST be the first attribute
  cmd->set_name(args[0]);
  args.erase(args.begin());

  while (!args.empty()) {
    if (args[0] == "-add-ns") {
      //nameserver to add
      NameServer ns;
      if (SheppObjSet::nameServer(ns, args) != 0) {
	return cmd_domain_help("invalid nameserver to add", cmd_name);
      }
      cmd->insert_nameserver_add(ns);
    } else if (args[0] == "-rem-ns") {
      //nameserver to remove
      NameServer ns;
      if (SheppObjSet::nameServer(ns, args) != 0) {
	return cmd_domain_help("invalid nameserver to remove", cmd_name);
      }
      cmd->insert_nameserver_rem(ns);
    } else if (args[0] == "-add-contact") {
      //contact to add
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("add-contact parameter missing", cmd_name);
      }

      string type;
      string value;
      if (SheppStrUtil::split(args[0], type, value, "=", false) != 0) {
	return cmd_domain_help("invalid contact to add", cmd_name);
      }

      cmd->insert_contact_add(type, value);
      args.erase(args.begin());      

    } else if (args[0] == "-rem-contact") {
      //contact to remove
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("rem-contact parameter missing", cmd_name);
      }

      string type;
      string value;
      if (SheppStrUtil::split(args[0], type, value, "=", false) != 0) {
	return cmd_domain_help("invalid contact to remove", cmd_name);
      }

      cmd->insert_contact_rem(type, value);
      args.erase(args.begin());      

    } else if (args[0] == "-add-status") {
      //status to add
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("add-status parameter missing", cmd_name);
      }

      DomainUpdateCmd::Status st;
      if (SheppObjSet::status(st, args) != 0) {
	return cmd_domain_help("invalid status to add", cmd_name);
      }
      cmd->insert_status_add(st);

    } else if (args[0] == "-rem-status") {
      //status to remove
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("rem-status parameter missing", cmd_name);
      }

      DomainUpdateCmd::Status st;
      st.s = args[0];
      args.erase(args.begin());

      cmd->insert_status_rem(st);

    } else if (args[0] == "-add-ds") {
      //DS to add
      DSInfo ds;
      string error_msg;
      if (SheppObjSet::dsInfo(ds, args, error_msg) != 0) {
	return cmd_domain_help(error_msg, cmd_name);
      }
      cmd->insert_ds_add(ds);
    } else if (args[0] == "-rem-ds") {
      //DS to remove
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("rem-ds parameter missing", cmd_name);
      }
      cmd->insert_ds_rem(atoi(args[0].c_str()));
      args.erase(args.begin());
    } else if (args[0] == "-chg-ds") {
      //DS to change
      DSInfo ds;
      string error_msg;
      if (SheppObjSet::dsInfo(ds, args, error_msg) != 0) {
	return cmd_domain_help(error_msg, cmd_name);
      }
      cmd->insert_ds_chg(ds);
    } else if (args[0] == "-secDNS-urgent") {
      cmd->setUrgentFlag(true);
      args.erase(args.begin());
    } else if (args[0] == "-r") {
      //registrant
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("registrant parameter missing", cmd_name);
      }
      cmd->set_registrant(args[0]);
      args.erase(args.begin());
    } else if (args[0] == "-auth") {
      //authInfo
      AuthInfo auth;
      if (SheppObjSet::authInfo(auth, args) != 0) {
	return cmd_domain_help("invalid auth", cmd_name);
      }
      cmd->set_authInfo(auth);
#if USE_BR_DOMAINS
    } else if (args[0] == "-t") {
      // ticketNumber
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("ticket number parameter missing", cmd_name);
      }
      if (atoi(args[0].c_str()) <= 0) {
	return cmd_domain_help("ticket parameter must be a positive integer",
			       cmd_name);
      }
      cmd->set_ticketNumber(atoi(args[0].c_str()));
      args.erase(args.begin());
    } else if (args[0] == "-rpflags") {
      //releaseProcessFlags
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("rpflags parameter missing", cmd_name);
      }
      ReleaseProcessFlags rpf;
      string flag1;
      string flag2;
      string flag3;

      if (SheppStrUtil::split(args[0], flag1, flag2, ":", false) != 0) {
	return cmd_domain_help("invalid rpflags", cmd_name);
      }

      if (SheppStrUtil::split(flag2, flag2, flag3, ":", false) != 0) {
	return cmd_domain_help("invalid rpflags", cmd_name);
      }
      
      if (flag1 == "0" || flag1 == "1") {
	rpf.flag1 = atoi(flag1.c_str());
      } else if (flag1 != "-1") {
	return cmd_domain_help("rpflags invalid parameter. "
			       "Expected '-1', '0' or '1'.", cmd_name);
      }
      if (flag2 == "0" || flag2 == "1") {
	rpf.flag2 = atoi(flag2.c_str());
      } else if (flag2 != "-1") {
	return cmd_domain_help("rpflags invalid parameter. "
			       "Expected '-1', '0' or '1'.", cmd_name);
      }
      if (flag3 == "0" || flag3 == "1") {
	rpf.flag3 = atoi(flag3.c_str());
      } else if (flag3 != "-1") {
	return cmd_domain_help("rpflags invalid parameter. "
			       "Expected '-1', '0' or '1'.", cmd_name);
      }

      cmd->set_releaseProcessFlags(rpf);
      args.erase(args.begin());
    } else if (args[0] == "-auto-renew") {
      //auto-renew
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_domain_help("auto-renew parameter missing", cmd_name);
      }
      if (args[0] == "on") {
	cmd->set_auto_renew(1);
      } else if (args[0] == "off") {
	cmd->set_auto_renew(0);
      } else {
	return cmd_domain_help("invalid auto-renew", cmd_name);
      }
      args.erase(args.begin());
#endif //USE_BR_DOMAINS
    } else {
      return cmd_domain_help("invalid syntax near \"" + args[0] + "\"",
			     cmd_name);
    }
  }

#if USE_BR_DOMAINS
  bool brdomain_rpf_update = false;
  //if it has brdomain attributes set it MUST have ticketNumber also
  if (cmd->get_releaseProcessFlags().flag1 != -1 ||
      cmd->get_releaseProcessFlags().flag2 != -1 ||
      cmd->get_releaseProcessFlags().flag3 != -1) {
    brdomain_rpf_update = true;
  }

  if (brdomain_rpf_update && cmd->get_ticketNumber() == 0) {
    return cmd_domain_help("BrDomain extension rpf updates "
			   "MUST contain ticketNumber", cmd_name);
  }
#endif //USE_BR_DOMAINS

  if (cmd->get_nameserver_add().empty() &&
      cmd->get_nameserver_rem().empty() &&
      cmd->get_ds_add().empty() &&
      cmd->get_ds_rem().empty() &&
      cmd->get_ds_chg().empty() &&
      cmd->get_contact_add().empty() &&
      cmd->get_contact_rem().empty() &&
      cmd->get_status_add().empty() &&
      cmd->get_status_rem().empty() &&
      cmd->get_registrant() == "" &&
      cmd->get_authInfo().get_pw() == ""
#if USE_BR_DOMAINS
      && !brdomain_rpf_update && cmd->get_auto_renew() == -1
#endif //USE_BR_DOMAINS
      ) {
    return cmd_domain_help("you didn't set a thing", cmd_name);
  }

  if (_debug) {
    printf("updating domain: %s\n", cmd->get_name().c_str());
    
    vector<NameServer> nss = cmd->get_nameserver_add();
    vector<NameServer>::const_iterator nsit;
    if (!nss.empty()) {
      printf("  nameservers to add:\n");
    }
    for (nsit = nss.begin(); nsit != nss.end(); nsit++) {
      SheppPrint::nameserver(*nsit);
    }
    
    nss = cmd->get_nameserver_rem();
    if (!nss.empty()) {
      printf("  nameservers to remove:\n");
    }
    for (nsit = nss.begin(); nsit != nss.end(); nsit++) {
      SheppPrint::nameserver(*nsit);
    }
    
    if (cmd->get_registrant() != "") {
      printf("  registrant: %s\n", cmd->get_registrant().c_str());
    }
    
    map< string, string, less<string> > contacts = cmd->get_contact_add();
    map< string, string, less<string> >::const_iterator cit;
    if (!contacts.empty()) {
      printf("  contacts to add:\n");
    }
    for (cit = contacts.begin(); cit != contacts.end(); cit++) {
      printf("  contact %s: %s\n", (*cit).first.c_str(),
	     (*cit).second.c_str());
    }
    
    contacts = cmd->get_contact_rem();
    if (!contacts.empty()) {
      printf("  contacts to remove:\n");
    }
    for (cit = contacts.begin(); cit != contacts.end(); cit++) {
      printf("  contact %s: %s\n", (*cit).first.c_str(),
	     (*cit).second.c_str());
    }
    
    set<DomainUpdateCmd::Status> statuses = cmd->get_status_add();
    set<DomainUpdateCmd::Status>::const_iterator stit;
    if (!statuses.empty()) {
      printf("  status to add:\n");
    }
    for (stit = statuses.begin(); stit != statuses.end(); stit++) {
      printf("    s   : [%s]\n", (*stit).s.c_str());
      printf("    lang: [%s]\n", (*stit).lang.c_str());
      printf("    msg : [%s]\n", (*stit).msg.c_str());
    }
    statuses = cmd->get_status_rem();
    if (!statuses.empty()) {
      printf("  status to remove:\n");
    }
    for (stit = statuses.begin(); stit != statuses.end(); stit++) {
      printf("    s   : [%s]\n", (*stit).s.c_str());
      printf("    lang: [%s]\n", (*stit).lang.c_str());
      printf("    msg : [%s]\n", (*stit).msg.c_str());
    }
    
    SheppPrint::authInfo(cmd->get_authInfo());
    
#if USE_BR_DOMAINS
    if (cmd->get_ticketNumber() != 0) {
      printf("  ticketNumber: [%d]\n", cmd->get_ticketNumber());
    }
    
    if (cmd->get_releaseProcessFlags().flag1 != -1 ||
	cmd->get_releaseProcessFlags().flag2 != -1 ||
	cmd->get_releaseProcessFlags().flag3 != -1) {
      printf("  releaseProcessFlags: %d, %d, %d\n",
	     cmd->get_releaseProcessFlags().flag1,
	     cmd->get_releaseProcessFlags().flag2,
	     cmd->get_releaseProcessFlags().flag3);
    }

    printf("  auto-renew: %d\n", cmd->get_auto_renew());
#endif //USE_BR_DOMAINS
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// main domain command
/**
   @param arg   command-line input arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_domain(vector<string> &args)
{
  // domain command processing
  if (!args.empty() && !(args[0] == "help")) {
    if (args[0] == "check") {
      args.erase(args.begin());
      return cmd_domain_check(args);
    } else if (args[0] == "info") {
      args.erase(args.begin());
      return cmd_domain_info(args);
    } else if (args[0] == "transfer") {
      args.erase(args.begin());
      return cmd_domain_transfer(args);
    } else if (args[0] == "create") {
      args.erase(args.begin());
      return cmd_domain_create(args);
    } else if (args[0] == "delete") {
      args.erase(args.begin());
      return cmd_domain_delete(args);
    } else if (args[0] == "renew") {
      args.erase(args.begin());
      return cmd_domain_renew(args);
    } else if (args[0] == "update") {
      args.erase(args.begin());
      return cmd_domain_update(args);
    } else {
      return cmd_domain_help("invalid command: domain " + args[0]);
    }
  }
  
  return cmd_domain_help("");
}

#endif //__DOMAIN_FUNCTIONS_H__
