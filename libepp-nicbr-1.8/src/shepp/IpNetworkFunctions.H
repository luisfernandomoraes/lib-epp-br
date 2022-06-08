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
/* $Id: IpNetworkFunctions.H 1010 2009-03-12 17:50:50Z eduardo $ */
/** @file IpNetworkFunctions.H
 *  @brief EPP IP Network related functions
 */
#ifndef __IPNETWORK_FUNCTIONS_H__
#define __IPNETWORK_FUNCTIONS_H__
#include "SheppCommandFunctions.H"

#include "IpNetworkCheck.H"
#include "IpNetworkCreate.H"
#include "IpNetworkDelete.H"
#include "IpNetworkInfo.H"
#include "IpNetworkRenew.H"
#include "IpNetworkTransfer.H"
#include "IpNetworkUpdate.H"

/// print ipnetwork command usage info
/**
   @param error_msg   error message to be printed
   @param specific    optional, choses specific sub-command
   @return 0 if ok, -1 otherwise
*/
int cmd_ipnetwork_help(string error_msg, string specific = "")
{
  if (error_msg != "") {
    printf("error: %s\n", error_msg.c_str());
  }

  printf("ipnetwork command syntax help:\n");
  printf("\n");

  if (specific == "" || specific == "check") {
    printf("  check <<v4|v6>=<startAddr>-<endAddr>> "
	   "[<v4|v6>=<startAddr>-<endAddr> ...]\n");
    printf("\n");
  }
  if (specific == "" || specific == "create") {
    printf("  create <v4|v6>=<startAddr>-<endAddr> <-o organization> "
	   "[-t allocType]\n"
	   "    [-a asn] [-contact type=value ...] \n"
	   "    [-rev-ns "
	   "<v4|v6>=<startAddr>-<endAddr>,hostname[,hostname ...] ...]\n");
    printf("\n");
  }
  if (specific == "" || specific == "delete") {
    printf("  delete <-roid repository-id>\n");
    printf("\n");
  }
  if (specific == "" || specific == "info") {
    printf("  info <<v4|v6>=<startAddr>-<endAddr>> [-roid repository-id]\n");
    printf("\n");
  }
  if (specific == "" || specific == "renew") {
    printf("  renew <-roid repository-id> <-expdate expDate> "
	   "[-period time:unit]\n");
    printf("\n");
  }
  if (specific == "" || specific == "transfer") {
    printf("  transfer <-op [query|request]> <-roid repository-id>\n");
    printf("\n");
  }
  if (specific == "" || specific == "update") {
    printf("  update <roid> [-add-contact type=value ...]\n"
	   "    [-rem-contact type=value ...] [-o organization] "
	   "[-t allocationType]\n"
	   "    [-add-rev-ns "
	   "<v4|v6>=<startAddr>-<endAddr>,hostname[,hostname ...] ...]\n"
	   "    [-rem-rev-ns "
	   "<v4|v6>=<startAddr>-<endAddr>,hostname[,hostname ...] ...]\n");
    printf("\n");
  }

  if (error_msg != "") {
    return -1;
  }

  return 0;
}

/// reads an IpRange object from args
/**
   @param input     IP renge command-line param
   @param ipRange   IpRange object to be filled
   @return 0 if ok, -1 ortherwise
 */
int parse_iprange(const string &input, IpRange &ipRange) {
  string error_msg("");

  string version;
  string range;
  string ipBegin;
  string ipEnd;

  if (SheppStrUtil::split(input, version, range, "=", false) != 0 ||
      SheppStrUtil::split(range, ipBegin, ipEnd, "-", false) != 0) {
    return -1;
  }

  ipRange.set_version(version);
  ipRange.set_ipBegin(ipBegin);
  ipRange.set_ipEnd(ipEnd);

  return 0;
}

/// ipnetwork check command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_ipnetwork_check(vector<string> &args)
{
  string cmd_name = "check";

  IpNetworkCheck act;
  IpNetworkCheckCmd *cmd = act.get_command();

  while (!args.empty()) {
    if (args[0].substr(0,1) == "v") {
      //IP range
      IpRange ipRange;
      if (parse_iprange(args[0], ipRange) != 0) {
	return cmd_ipnetwork_help("invalid IP range", cmd_name);
      }
      args.erase(args.begin());
      cmd->insert_ipRange(ipRange);
    } else {
      return cmd_ipnetwork_help("invalid syntax near \"" + args[0] + "\"",
				cmd_name);
    }
  }

  // check mandatory fields
  if (cmd->get_ipRanges().empty()) {
    return cmd_ipnetwork_help("no IP ranges specified", cmd_name);
  }

  if (_debug) {
    vector<IpRange> ipRanges = cmd->get_ipRanges();
    printf("ranges to be checked:\n");
    for (unsigned int i = 0; i < ipRanges.size(); i++) {
      printf("  (%s) from [%s] to [%s]\n",
	     ipRanges[i].get_version().c_str(),
	     ipRanges[i].get_ipBegin().c_str(),
	     ipRanges[i].get_ipEnd().c_str());
    }
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// ipnetwork create command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_ipnetwork_create(vector<string> &args)
{
  string cmd_name = "create";

  IpNetworkCreate act;
  IpNetworkCreateCmd *cmd = act.get_command();

  // mandatory fields
  bool hasIpRange = false;
  bool hasOrganization = false;

  while (!args.empty()) {
    if (args[0].substr(0,1) == "v") {
      //IP range
      if (hasIpRange) {
	return cmd_ipnetwork_help("only one IP range allowed per command",
				  cmd_name);
      } else {
	hasIpRange = true;
      }
      IpRange ipRange;
      if (parse_iprange(args[0], ipRange) != 0) {
	return cmd_ipnetwork_help("invalid IP range", cmd_name);
      }
      args.erase(args.begin());
      cmd->set_ipRange(ipRange);
    } else if (args[0] == "-o") {
      //set organization
      if (hasOrganization) {
	return cmd_ipnetwork_help("only one organization allowed per command",
				  cmd_name);
      } else {
	hasOrganization = true;
      }
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("organization parameter missing", cmd_name);
      }
      cmd->set_organization(args[0]);
      args.erase(args.begin());
    } else if (args[0] == "-t") {
      //set allocType
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("allocType parameter missing", cmd_name);
      }
      cmd->set_allocType(args[0]);
      args.erase(args.begin());
    } else if (args[0] == "-a") {
      //set asn
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("asn parameter missing", cmd_name);
      }

      if (atoi(args[0].c_str()) <= 0) {
	return cmd_ipnetwork_help("asn parameter must be a positive integer",
			       cmd_name);
      }

      cmd->set_asn(atoi(args[0].c_str()));
      args.erase(args.begin());
    } else if (args[0] == "-contact") {
      //contact
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("contact parameter missing", cmd_name);
      }

      string type;
      string value;
      if (SheppStrUtil::split(args[0], type, value, "=", false) != 0) {
	return cmd_ipnetwork_help("invalid contact", cmd_name);
      }

      cmd->insert_contact(type, value);
      args.erase(args.begin());      
    } else if (args[0] == "-rev-ns") {
      //reverse ns
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("reverse DNS parameter missing", cmd_name);
      }

      ReverseDns revDns;
      string range;
      string nameservers;
      if (SheppStrUtil::split(args[0], range, nameservers, ",", false) != 0 ||
	  parse_iprange(range, revDns.ipRange) != 0) {
	return cmd_ipnetwork_help("invalid reverse DNS", cmd_name);
      }

      string tmp1 = nameservers;
      string tmp2;
      while (SheppStrUtil::split(tmp1, tmp1, tmp2, ",", true) == 0) {
	revDns.nameservers.push_back(tmp1);
	if (tmp2 == "") {
	  break;
	}
	tmp1 = tmp2;
      }

      cmd->insert_reverseDns(revDns);
      args.erase(args.begin());
    } else {
      return cmd_ipnetwork_help("invalid syntax near \"" + args[0] + "\"",
				cmd_name);
    }
  }

  // check mandatory fields
  if (!hasIpRange) {
    return cmd_ipnetwork_help("no IP range specified", cmd_name);
  }

  if (!hasOrganization) {
    return cmd_ipnetwork_help("no organization specified", cmd_name);
  }

  if (_debug) {
    IpRange ipRange = cmd->get_ipRange();
    printf("range to be created:\n");
    printf("  (%s) from [%s] to [%s]\n",
	   ipRange.get_version().c_str(),
	   ipRange.get_ipBegin().c_str(),
	   ipRange.get_ipEnd().c_str());
    printf("organization: [%s]\n", cmd->get_organization().c_str());
    printf("allocType: [%s]\n", cmd->get_allocType().c_str());
    if (cmd->get_asn() > 0) {
      printf("asn: [%d]\n", cmd->get_asn());
    }
    map< string, string, less<string> > contacts = cmd->get_contacts();
    map< string, string, less<string> >::const_iterator cit;
    for (cit = contacts.begin(); cit != contacts.end(); cit++) {
      printf("contact %s: %s\n", (*cit).first.c_str(), (*cit).second.c_str());
    }

    printf("reverse nameservers:\n");
    vector<ReverseDns> revNs = cmd->get_reversesDns();
    for (unsigned int i = 0; i < revNs.size(); i++) {
      printf("  range (%s) from [%s] to [%s]:\n",
	     revNs[i].ipRange.get_version().c_str(),
	     revNs[i].ipRange.get_ipBegin().c_str(),
	     revNs[i].ipRange.get_ipEnd().c_str());
      for (unsigned int j = 0; j < revNs[i].nameservers.size(); j++) {
	printf("    nameserver: [%s]\n", revNs[i].nameservers[j].c_str());
      }
    }
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// ipnetwork check delete function
/**
   @param args   vector with command-line arguments
   @return 0
*/
int cmd_ipnetwork_delete(vector<string> &args)
{
  string cmd_name = "delete";

  IpNetworkDelete act;
  IpNetworkDeleteCmd *cmd = act.get_command();

  // mandatory field
  bool hasRoid = false;

  while (!args.empty()) {
    if (args[0] == "-roid") {
      //roid
      if (hasRoid) {
	return cmd_ipnetwork_help("only one repository ID allowed per command",
				  cmd_name);
      } else {
	hasRoid = true;
      }
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("repository ID parameter missing", cmd_name);
      }

      cmd->set_roid(args[0]);
      args.erase(args.begin());
    } else {
      return cmd_ipnetwork_help("invalid syntax near \"" + args[0] + "\"",
				cmd_name);
    }
  }

  if (!hasRoid) {
    return cmd_ipnetwork_help("no repository ID specified", cmd_name);
  }

  if (_debug) {
    printf("roid: [%s]\n", cmd->get_roid().c_str());
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// ipnetwork info command function
/**
   @param args   vector with command-line arguments
   @return 0
*/
int cmd_ipnetwork_info(vector<string> &args)
{
  string cmd_name = "info";

  IpNetworkInfo act;
  IpNetworkInfoCmd *cmd = act.get_command();

  // mandatory field
  bool hasIpRange = false;

  while (!args.empty()) {
    if (args[0].substr(0,1) == "v") {
      //IP range
      if (hasIpRange) {
	return cmd_ipnetwork_help("only one IP range allowed per command",
				  cmd_name);
      } else {
	hasIpRange = true;
      }
      IpRange ipRange;
      if (parse_iprange(args[0], ipRange) != 0) {
	return cmd_ipnetwork_help("invalid IP range", cmd_name);
      }
      args.erase(args.begin());
      cmd->set_ipRange(ipRange);
    } else if (args[0] == "-roid") {
      //roid
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("repository ID parameter missing", cmd_name);
      }

      cmd->set_roid(args[0]);
      args.erase(args.begin());
    } else {
      return cmd_ipnetwork_help("invalid syntax near \"" + args[0] + "\"",
				cmd_name);
    }
  }

  // check mandatory field
  if (!hasIpRange) {
    return cmd_ipnetwork_help("no IP range specified", cmd_name);
  }

  if (_debug) {
    IpRange ipRange = cmd->get_ipRange();
    printf("range to get info:\n");
    printf("  (%s) from [%s] to [%s]\n",
	   ipRange.get_version().c_str(),
	   ipRange.get_ipBegin().c_str(),
	   ipRange.get_ipEnd().c_str());

    if (cmd->get_roid() != "") {
      printf("roid: [%s]\n", cmd->get_roid().c_str());
    }
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// ipnetwork renew command function
/**
   @param args   vector with command-line arguments
   @return 0
*/
int cmd_ipnetwork_renew(vector<string> &args)
{
  string cmd_name = "renew";

  IpNetworkRenew act;
  IpNetworkRenewCmd *cmd = act.get_command();

  // mandatory fields
  bool hasRoid = false;
  bool hasExpDate = false;

  while (!args.empty()) {
    if (args[0] == "-roid") {
      //roid
      if (hasRoid) {
	return cmd_ipnetwork_help("only one repository ID allowed per command",
				  cmd_name);
      } else {
	hasRoid = true;
      }
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("repository ID parameter missing", cmd_name);
      }

      cmd->set_roid(args[0]);
      args.erase(args.begin());
    } else if (args[0] == "-expdate") {
      //expdate
      if (hasExpDate) {
	return cmd_ipnetwork_help("only one expDate allowed per command",
				  cmd_name);
      } else {
	hasExpDate = true;
      }
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("expDate missing", cmd_name);
      }
      cmd->set_expDate(args[0]);
      args.erase(args.begin());      
    } else if (args[0] == "-period") {
      //period
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("period parameter missing", cmd_name);
      }

      string time;
      string unit;
      if (SheppStrUtil::split(args[0], time, unit, ":", false) != 0) {
	return cmd_ipnetwork_help("invalid period", cmd_name);
      }

      cmd->set_period(atoi(time.c_str()), unit);
      args.erase(args.begin());
    } else {
      return cmd_ipnetwork_help("invalid syntax near \"" + args[0] + "\"",
				cmd_name);
    }
  }

  // check mandatory fields
  if (!hasRoid) {
    return cmd_ipnetwork_help("no repository ID specified", cmd_name);
  }

  if (!hasExpDate) {
    return cmd_ipnetwork_help("no expDate specified", cmd_name);
  }

  if (_debug) {
    printf("roid   : [%s]\n", cmd->get_roid().c_str());
    printf("expDate: [%s]\n", cmd->get_expDate().c_str());
    if (cmd->get_period().time != 0 || cmd->get_period().unit != "") {
      printf("period : [%d %s]\n", cmd->get_period().time,
	     cmd->get_period().unit.c_str());
    }
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// ipnetwork transfer command function
/**
   @param args   vector with command-line arguments
   @return 0
*/
int cmd_ipnetwork_transfer(vector<string> &args)
{
  string cmd_name = "transfer";

  IpNetworkTransfer act;
  IpNetworkTransferCmd *cmd = act.get_command();

  // mandatory fields
  bool hasRoid = false;
  bool hasOp = false;

  while (!args.empty()) {
    if (args[0] == "-roid") {
      //roid
      if (hasRoid) {
	return cmd_ipnetwork_help("only one repository ID allowed per command",
				  cmd_name);
      } else {
	hasRoid = true;
      }
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("repository ID parameter missing", cmd_name);
      }

      cmd->set_roid(args[0]);
      args.erase(args.begin());
    } else if (args[0] == "-op") {
      //op
      if (hasOp) {
	return cmd_ipnetwork_help("only one operation allowed per command",
				  cmd_name);
      } else {
	hasOp = true;
      }
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("operation parameter missing", cmd_name);
      }

      if (args[0] != "query" && args[0] != "request") {
	return cmd_ipnetwork_help("invalid operation '" + args[0] + "'",
				  cmd_name);
      }

      cmd->set_operation(args[0]);
      args.erase(args.begin());
    } else {
      return cmd_ipnetwork_help("invalid syntax near \"" + args[0] + "\"",
				cmd_name);
    }
  }

  if (!hasRoid) {
    return cmd_ipnetwork_help("no repository ID specified", cmd_name);
  }

  if (!hasOp) {
    return cmd_ipnetwork_help("no operation specified", cmd_name);
  }

  if (_debug) {
    printf("roid: [%s]\n", cmd->get_roid().c_str());
    printf("op  : [%s]\n", cmd->get_operation().c_str());
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// ipnetwork update command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_ipnetwork_update(vector<string> &args)
{
  string cmd_name = "update";

  IpNetworkUpdate act;
  IpNetworkUpdateCmd *cmd = act.get_command();

  if (args.empty()) {
    return cmd_ipnetwork_help("no ROID specified", cmd_name);
  }

  cmd->set_roid(args[0]);
  args.erase(args.begin());
  
  bool hasOrganization = false;
  bool hasAllocType = false;
  bool hasContactsAdd = false;
  bool hasContactsRem = false;
  bool hasReverseDnsAdd = false;
  bool hasReverseDnsRem = false;

  while (!args.empty()) {
    if (args[0] == "-add-contact") {
      //contact(s) to add
      hasContactsAdd = true;
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("add-contact parameter missing", cmd_name);
      }

      string type;
      string value;
      if (SheppStrUtil::split(args[0], type, value, "=", false) != 0) {
	return cmd_ipnetwork_help("invalid contact", cmd_name);
      }

      cmd->insert_contact_add(type, value);
      args.erase(args.begin());      
    } else if (args[0] == "-rem-contact") {
      //contact(s) to remove
      hasContactsRem = true;
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("rem-contact parameter missing", cmd_name);
      }

      string type;
      string value;
      if (SheppStrUtil::split(args[0], type, value, "=", false) != 0) {
	return cmd_ipnetwork_help("invalid contact", cmd_name);
      }

      cmd->insert_contact_rem(type, value);
      args.erase(args.begin());      
    } else if (args[0] == "-o") {
      //set organization
      if (hasOrganization) {
	return cmd_ipnetwork_help("only one organization allowed per command",
				  cmd_name);
      } else {
	hasOrganization = true;
      }
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("organization parameter missing", cmd_name);
      }
      cmd->set_organization(args[0]);
      args.erase(args.begin());
    } else if (args[0] == "-t") {
      //set allocType
      if (hasAllocType) {
	return cmd_ipnetwork_help("only one allocation allowed per command",
			    cmd_name);
      } else {
	hasAllocType = true;
      }
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("allocType parameter missing", cmd_name);
      }
      cmd->set_allocType(args[0]);
      args.erase(args.begin());
    } else if (args[0] == "-add-rev-ns") {
      //reverse ns
      hasReverseDnsAdd = true;
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("reverse DNS parameter missing", cmd_name);
      }
      
      ReverseDns revDns;
      string range;
      string nameservers;
      if (SheppStrUtil::split(args[0], range, nameservers, ",", false) != 0 ||
	  parse_iprange(range, revDns.ipRange) != 0) {
	return cmd_ipnetwork_help("invalid reverse DNS", cmd_name);
      }
      
      string tmp1 = nameservers;
      string tmp2;
      while (SheppStrUtil::split(tmp1, tmp1, tmp2, ",", true) == 0) {
	revDns.nameservers.push_back(tmp1);
	if (tmp2 == "") {
	  break;
	}
	tmp1 = tmp2;
      }
      
      cmd->insert_reverseDNS_add(revDns);
      args.erase(args.begin());
    } else if (args[0] == "-rem-rev-ns") {
      hasReverseDnsRem = true;
      //reverse ns
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_ipnetwork_help("reverse DNS parameter missing", cmd_name);
      }
      
      ReverseDns revDns;
      string range;
      string nameservers;
      if (SheppStrUtil::split(args[0], range, nameservers, ",", false) != 0 ||
	  parse_iprange(range, revDns.ipRange) != 0) {
	return cmd_ipnetwork_help("invalid reverse DNS", cmd_name);
      }
      
      string tmp1 = nameservers;
      string tmp2;
      while (SheppStrUtil::split(tmp1, tmp1, tmp2, ",", true) == 0) {
	revDns.nameservers.push_back(tmp1);
	if (tmp2 == "") {
	  break;
	}
	tmp1 = tmp2;
      }
      
      cmd->insert_reverseDNS_rem(revDns);
      args.erase(args.begin());
    } else {
      return cmd_ipnetwork_help("invalid syntax near \"" + args[0] + "\"",
				cmd_name);
    }
  }

  if (!hasOrganization && !hasContactsAdd && !hasContactsRem && 
      !hasAllocType && !hasReverseDnsAdd && !hasReverseDnsRem) {
    return cmd_ipnetwork_help("nothing to update", cmd_name);
  }

  if (_debug) {
    printf("IP Network to be updated: [%s]\n", cmd->get_roid().c_str());
    printf("organization: [%s]\n", cmd->get_organization().c_str());
    printf("allocation type: [%s]\n", cmd->get_allocType().c_str());

    map< string, string, less<string> >::const_iterator cit;
    map< string, string, less<string> > contacts;

    contacts = cmd->get_contacts_add();
    for (cit = contacts.begin(); cit != contacts.end(); cit++) {
      printf("contact to add %s: %s\n",
	     (*cit).first.c_str(), (*cit).second.c_str());
    }

    contacts = cmd->get_contacts_rem();
    for (cit = contacts.begin(); cit != contacts.end(); cit++) {
      printf("contact to remove %s: %s\n",
	     (*cit).first.c_str(), (*cit).second.c_str());
    }

    vector<ReverseDns> reverseDns;

    reverseDns = cmd->get_reverseDns_add();
    for (unsigned int i = 0; i < reverseDns.size(); i++) {
      IpRange ipRange = reverseDns[i].ipRange;
      printf("reverse DNS to add: [%s] from [%s] to [%s]\n", 
	     ipRange.get_version().c_str(), 
	     ipRange.get_ipBegin().c_str(), 
	     ipRange.get_ipEnd().c_str());
      vector<string> nameservers = reverseDns[i].nameservers;
      for (unsigned int j = 0; j < nameservers.size(); j++) {
	printf("  nameserver [%s]\n", nameservers[j].c_str());
      }
    }

    reverseDns = cmd->get_reverseDns_rem();
    for (unsigned int i = 0; i < reverseDns.size(); i++) {
      IpRange ipRange = reverseDns[i].ipRange;
      printf("reverse DNS to remove: [%s] from [%s] to [%s]\n", 
	     ipRange.get_version().c_str(), 
	     ipRange.get_ipBegin().c_str(), 
	     ipRange.get_ipEnd().c_str());
      vector<string> nameservers = reverseDns[i].nameservers;
      for (unsigned int j = 0; j < nameservers.size(); j++) {
	printf("  nameserver [%s]\n", nameservers[j].c_str());
      }
    }
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// main ipnetwork command
/**
   @param arg   command-line input arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_ipnetwork(vector<string> &args)
{
  // ipnetwork command processing
  if (!args.empty() && !(args[0] == "help")) {
    if (args[0] == "check") {
      args.erase(args.begin());
      return cmd_ipnetwork_check(args);
    } else if (args[0] == "create") {
      args.erase(args.begin());
      return cmd_ipnetwork_create(args);
    } else if (args[0] == "delete") {
      args.erase(args.begin());
      return cmd_ipnetwork_delete(args);
    } else if (args[0] == "info") {
      args.erase(args.begin());
      return cmd_ipnetwork_info(args);
    } else if (args[0] == "renew") {
      args.erase(args.begin());
      return cmd_ipnetwork_renew(args);
    } else if (args[0] == "transfer") {
      args.erase(args.begin());
      return cmd_ipnetwork_transfer(args);
    } else if (args[0] == "update") {
      args.erase(args.begin());
      return cmd_ipnetwork_update(args);
    } else {
      return cmd_ipnetwork_help("invalid command: ipnetwork " + args[0]);
    }
  }
  
  return cmd_ipnetwork_help("");
}

#endif //__IPNETWORK_FUNCTIONS_H__
