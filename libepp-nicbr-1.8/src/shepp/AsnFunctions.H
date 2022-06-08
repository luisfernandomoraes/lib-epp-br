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
/* $Id: AsnFunctions.H 991 2009-01-06 16:29:22Z rafael $ */
/** @file AsnFunctions.H
 *  @brief EPP ASN related functions
 */
#ifndef __ASN_FUNCTIONS_H__
#define __ASN_FUNCTIONS_H__
#include "SheppCommandFunctions.H"

#include "AsnCheck.H"
#include "AsnCreate.H"
#include "AsnDelete.H"
#include "AsnInfo.H"
#include "AsnRenew.H"
#include "AsnTransfer.H"
#include "AsnUpdate.H"

/// print asn command usage info
/**
   @param error_msg   error message to be printed
   @param specific    optional, choses specific sub-command
   @return 0 if ok, -1 otherwise
*/
int cmd_asn_help(string error_msg, string specific = "")
{
  if (error_msg != "") {
    printf("error: %s\n", error_msg.c_str());
  }

  printf("asn command syntax help:\n");
  printf("\n");

  if (specific == "" || specific == "check") {
    printf("  check <asn> [asn ...]\n");
    printf("\n");
  }
  if (specific == "" || specific == "create") {
    printf("  create <asn> <-o organization> <-contact type=value> "
	   "[-contact type=value ...]\n");
    printf("\n");
  }
  if (specific == "" || specific == "delete") {
    printf("  delete <asn>\n");
    printf("\n");
  }
  if (specific == "" || specific == "info") {
    printf("  info <asn>\n");
    printf("\n");
  }
  if (specific == "" || specific == "renew") {
    printf("  renew <asn> <-expdate expDate> [-period time:unit]\n");
    printf("\n");
  }
  if (specific == "" || specific == "transfer") {
    printf("  transfer <asn> <-op [query|request]>\n");
    printf("\n");
  }
  if (specific == "" || specific == "update") {
    printf("  update <asn> [-add-contact type=value ...]\n"
	   "    [-rem-contact type=value ...] [-o organization]\n");
    printf("\n");
  }

  if (error_msg != "") {
    return -1;
  }

  return 0;
}

/// asn check command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_asn_check(vector<string> &args)
{
  string cmd_name = "check";

  AsnCheck act;
  AsnCheckCmd *cmd = act.get_command();

  if (args.empty()) {
    return cmd_asn_help("no ASNs specified", cmd_name);
  }

  while (!args.empty()) {
    int asn = atoi(args[0].c_str());
    if (asn > 0) {
      cmd->add_asn(asn);
      args.erase(args.begin());
    } else {
      return cmd_asn_help("invalid ASN: '" + args[0] + "'", cmd_name);
    }
  }

  if (_debug) {
    list<int> asns = cmd->get_asn_list();
    printf("ASNs to be checked:\n");
    list<int>::const_iterator asn;
    for (asn = asns.begin(); asn != asns.end(); asn++) {
      printf("  [%d]\n", (*asn));
    }
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// asn create command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_asn_create(vector<string> &args)
{
  string cmd_name = "create";

  AsnCreate act;
  AsnCreateCmd *cmd = act.get_command();

  if (args.empty()) {
    return cmd_asn_help("no ASN specified", cmd_name);
  }

  int asn = atoi(args[0].c_str());
  if (asn <= 0) {
    return cmd_asn_help("invalid ASN: '" + args[0] + "'", cmd_name);
  }
  cmd->set_asn(asn);
  args.erase(args.begin());

  bool hasOrganization = false;
  bool hasContacts = false;

  while (!args.empty()) {
    if (args[0] == "-contact") {
      //contact
      hasContacts = true;
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_asn_help("contact parameter missing", cmd_name);
      }

      string type;
      string value;
      if (SheppStrUtil::split(args[0], type, value, "=", false) != 0) {
	return cmd_asn_help("invalid contact", cmd_name);
      }

      cmd->insert_contact(type, value);
      args.erase(args.begin());      
    } else if (args[0] == "-o") {
      //set organization
      if (hasOrganization) {
	return cmd_asn_help("only one organization allowed per command",
			    cmd_name);
      } else {
	hasOrganization = true;
      }
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_asn_help("organization parameter missing", cmd_name);
      }
      cmd->set_organization(args[0]);
      args.erase(args.begin());
    } else {
      return cmd_asn_help("invalid syntax near \"" + args[0] + "\"",
			  cmd_name);
    }
  }

  if (!hasOrganization) {
    return cmd_asn_help("no organization specified", cmd_name);
  }

  if (!hasContacts) {
    return cmd_asn_help("no contact specified", cmd_name);
  }

  if (_debug) {
    printf("ASN to be created: [%d]\n", cmd->get_asn());
    printf("organization: [%s]\n", cmd->get_organization().c_str());

    map< string, string, less<string> > contacts = cmd->get_contacts();
    map< string, string, less<string> >::const_iterator cit;
    for (cit = contacts.begin(); cit != contacts.end(); cit++) {
      printf("contact %s: %s\n", (*cit).first.c_str(), (*cit).second.c_str());
    }
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// asn check delete function
/**
   @param args   vector with command-line arguments
   @return 0
*/
int cmd_asn_delete(vector<string> &args)
{
  string cmd_name = "delete";

  AsnDelete act;
  AsnDeleteCmd *cmd = act.get_command();

  if (args.size() != 1) {
    return cmd_asn_help("exactly one ASN must be specified", cmd_name);
  }

  int asn = atoi(args[0].c_str());
  if (asn <= 0) {
    return cmd_asn_help("invalid ASN: '" + args[0] + "'", cmd_name);
  }

  cmd->set_asn(asn);

  if (_debug) {
    printf("ASN to be deleted: [%d]\n", cmd->get_asn());
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// asn info command function
/**
   @param args   vector with command-line arguments
   @return 0
*/
int cmd_asn_info(vector<string> &args)
{
  string cmd_name = "info";

  AsnInfo act;
  AsnInfoCmd *cmd = act.get_command();

  if (args.size() != 1) {
    return cmd_asn_help("exactly one ASN must be specified", cmd_name);
  }

  int asn = atoi(args[0].c_str());
  if (asn <= 0) {
    return cmd_asn_help("invalid ASN: '" + args[0] + "'", cmd_name);
  }

  cmd->set_asn(asn);

  if (_debug) {
    printf("ASN to get info: [%d]\n", cmd->get_asn());
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// asn renew command function
/**
   @param args   vector with command-line arguments
   @return 0
*/
int cmd_asn_renew(vector<string> &args)
{
  string cmd_name = "renew";

  AsnRenew act;
  AsnRenewCmd *cmd = act.get_command();

  if (args.empty()) {
    return cmd_asn_help("no ASN specified", cmd_name);
  }

  int asn = atoi(args[0].c_str());
  if (asn <= 0) {
    return cmd_asn_help("invalid ASN: '" + args[0] + "'", cmd_name);
  }
  cmd->set_asn(asn);
  args.erase(args.begin());

  // mandatory field
  bool hasExpDate = false;

  while (!args.empty()) {
    if (args[0] == "-expdate") {
      //expdate
      if (hasExpDate) {
	return cmd_asn_help("only one expDate allowed per command",
			    cmd_name);
      } else {
	hasExpDate = true;
      }
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_asn_help("expDate missing", cmd_name);
      }
      cmd->set_expDate(args[0]);
      args.erase(args.begin());      
    } else if (args[0] == "-period") {
      //period
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_asn_help("period parameter missing", cmd_name);
      }

      string time;
      string unit;
      if (SheppStrUtil::split(args[0], time, unit, ":", false) != 0) {
	return cmd_asn_help("invalid period", cmd_name);
      }

      cmd->set_period(atoi(time.c_str()), unit);
      args.erase(args.begin());
    } else {
      return cmd_asn_help("invalid syntax near \"" + args[0] + "\"",
			  cmd_name);
    }
  }

  // check mandatory fields
  if (!hasExpDate) {
    return cmd_asn_help("no expDate specified", cmd_name);
  }

  if (_debug) {
    printf("asn    : [%d]\n", cmd->get_asn());
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

/// asn transfer command function
/**
   @param args   vector with command-line arguments
   @return 0
*/
int cmd_asn_transfer(vector<string> &args)
{
  string cmd_name = "transfer";

  AsnTransfer act;
  AsnTransferCmd *cmd = act.get_command();

  // mandatory field
  bool hasOp = false;

  if (args.empty()) {
    return cmd_asn_help("no ASN specified", cmd_name);
  }

  int asn = atoi(args[0].c_str());
  if (asn <= 0) {
    return cmd_asn_help("invalid ASN: '" + args[0] + "'", cmd_name);
  }
  cmd->set_asn(asn);
  args.erase(args.begin());

  while (!args.empty()) {
    if (args[0] == "-op") {
      //op
      if (hasOp) {
	return cmd_asn_help("only one operation allowed per command",
			    cmd_name);
      } else {
	hasOp = true;
      }
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_asn_help("operation parameter missing", cmd_name);
      }

      if (args[0] != "query" && args[0] != "request") {
	return cmd_asn_help("invalid operation '" + args[0] + "'",
			    cmd_name);
      }

      cmd->set_operation(args[0]);
      args.erase(args.begin());
    } else {
      return cmd_asn_help("invalid syntax near \"" + args[0] + "\"",
			  cmd_name);
    }
  }

  if (!hasOp) {
    return cmd_asn_help("no operation specified", cmd_name);
  }

  if (_debug) {
    printf("asn : [%d]\n", cmd->get_asn());
    printf("op  : [%s]\n", cmd->get_operation().c_str());
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// asn update command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_asn_update(vector<string> &args)
{
  string cmd_name = "update";

  AsnUpdate act;
  AsnUpdateCmd *cmd = act.get_command();

  if (args.empty()) {
    return cmd_asn_help("no ASN specified", cmd_name);
  }

  int asn = atoi(args[0].c_str());
  if (asn <= 0) {
    return cmd_asn_help("invalid ASN: '" + args[0] + "'", cmd_name);
  }
  cmd->set_asn(asn);
  args.erase(args.begin());

  bool hasOrganization = false;
  bool hasContactsAdd = false;
  bool hasContactsRem = false;

  while (!args.empty()) {
    if (args[0] == "-add-contact") {
      //contact(s) to add
      hasContactsAdd = true;
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_asn_help("add-contact parameter missing", cmd_name);
      }

      string type;
      string value;
      if (SheppStrUtil::split(args[0], type, value, "=", false) != 0) {
	return cmd_asn_help("invalid contact", cmd_name);
      }

      cmd->insert_contact_add(type, value);
      args.erase(args.begin());      
    } else if (args[0] == "-rem-contact") {
      //contact(s) to remove
      hasContactsRem = true;
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_asn_help("rem-contact parameter missing", cmd_name);
      }

      string type;
      string value;
      if (SheppStrUtil::split(args[0], type, value, "=", false) != 0) {
	return cmd_asn_help("invalid contact", cmd_name);
      }

      cmd->insert_contact_rem(type, value);
      args.erase(args.begin());      
    } else if (args[0] == "-o") {
      //set organization
      if (hasOrganization) {
	return cmd_asn_help("only one organization allowed per command",
			    cmd_name);
      } else {
	hasOrganization = true;
      }
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_asn_help("organization parameter missing", cmd_name);
      }
      cmd->set_organization(args[0]);
      args.erase(args.begin());
    } else {
      return cmd_asn_help("invalid syntax near \"" + args[0] + "\"",
			  cmd_name);
    }
  }

  if (!hasOrganization && !hasContactsAdd && !hasContactsRem) {
    return cmd_asn_help("nothing to update", cmd_name);
  }

  if (_debug) {
    printf("ASN to be updated: [%d]\n", cmd->get_asn());
    printf("organization: [%s]\n", cmd->get_organization().c_str());

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
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// main asn command
/**
   @param arg   command-line input arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_asn(vector<string> &args)
{
  // asn command processing
  if (!args.empty() && !(args[0] == "help")) {
    if (args[0] == "check") {
      args.erase(args.begin());
      return cmd_asn_check(args);
    } else if (args[0] == "create") {
      args.erase(args.begin());
      return cmd_asn_create(args);
    } else if (args[0] == "delete") {
      args.erase(args.begin());
      return cmd_asn_delete(args);
    } else if (args[0] == "info") {
      args.erase(args.begin());
      return cmd_asn_info(args);
    } else if (args[0] == "renew") {
      args.erase(args.begin());
      return cmd_asn_renew(args);
    } else if (args[0] == "transfer") {
      args.erase(args.begin());
      return cmd_asn_transfer(args);
    } else if (args[0] == "update") {
      args.erase(args.begin());
      return cmd_asn_update(args);
    } else {
      return cmd_asn_help("invalid command: asn " + args[0]);
    }
  }
  
  return cmd_asn_help("");
}

#endif //__ASN_FUNCTIONS_H__
