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
/* $Id: ContactFunctions.H 804 2006-08-03 12:56:01Z eduardo $ */
/** @file ContactFunctions.H
 *  @brief EPP Contact related functions
 */
#ifndef __CONTACT_FUNCTIONS_H__
#define __CONTACT_FUNCTIONS_H__
#include "SheppCommandFunctions.H"

#include "ContactCheck.H"
#include "ContactInfo.H"
#include "ContactCreate.H"
#include "ContactUpdate.H"

/// print contact command usage info
/**
   @param error_msg   error message to be printed
   @param specific    optional, choses specific sub-command
   @return 0 if ok, -1 otherwise
*/
int cmd_contact_help(string error_msg, string specific = "")
{
  if (error_msg != "") {
    printf("error: %s\n", error_msg.c_str());
  }

  printf("contact command syntax help:\n");
  printf("\n");

  if (specific == "" || specific == "check") {
    printf("  check <contact1> [contact2 ... contactN]\n");
    printf("\n");
  }
  if (specific == "" || specific == "info") {
    printf("  info <contact> [-auth authInfoPw[:authInfoRoid]]\n");
    printf("\n");
  }
  if (specific == "" || specific == "transfer") {
    printf("  transfer: not implemented\n");
    printf("\n");
  }
  if (specific == "" || specific == "create") {
    printf("  create <contact>\n");
    printf("    [-postalInfo <type> <-name \"Contact Name\"> "
	   "[-org \"Org Name\"]\n");
    printf("    <-street1 \"address street/line 1\"> "
	   "[-street2 \"address number/line 2\"]\n");
    printf("    [-street3 \"address line 3\"] <-city \"City Name\"> "
	   "[-state \"State or Province\"]\n");
    printf("    [-pc \"postal code\"] <-cc country-code> ...]\n");
    printf("    [-voice number:ext] [-fax number:ext]\n");
    printf("    <-email emailAddr> [-auth authInfoPw[:authInfoRoid]]\n");
    printf("    [-disclose <flag> opt,opt,...]\n");
    printf("\n");
    printf("    available values for opt:\n");
    printf("    {name_int name_loc org_int org_loc addr_int addr_loc "
	   "voice fax email}\n");
    printf("\n");
  }
  if (specific == "" || specific == "delete") {
    printf("  delete: not implemented\n");
    printf("\n");
  }
  if (specific == "" || specific == "renew") {
    printf("  renew: not implemented\n");
    printf("\n");
  }
  if (specific == "" || specific == "update") {
    printf("  update <contact> [-add-status st1,...,stN] "
	   "[-rem-status st1,...,stN]\n");
    printf("    [-postalInfo <type> [-name \"Contact Name\"] "
	   "[-org \"Org Name\"]\n");
    printf("    [-street1 \"address street/line 1\"] "
	   "[-street2 \"address number/line 2\"]\n");
    printf("    [-street3 \"address line 3\"] [-city \"City Name\"] "
	   "[-state \"State or Province\"]\n");
    printf("    [-pc \"postal code\"] [-cc country-code] ...]\n");
    printf("    [-voice number:ext] [-fax number:ext]\n");
    printf("    [-email emailAddr] [-auth authInfoPw[:authInfoRoid]]\n");
    printf("    [-disclose <flag> opt,opt,...]\n");
    printf("\n");
    printf("    available values for opt:\n");
    printf("    {name_int name_loc org_int org_loc addr_int addr_loc "
	   "voice fax email}\n");
    printf("\n");
  }

  if (error_msg != "") {
    return -1;
  }

  return 0;
}

/// contact check command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_contact_check(vector<string> &args)
{
  ContactCheck act;
  ContactCheckCmd *cmd = act.get_command();

  string cmd_name = "check";

  while (!args.empty()) {
    cmd->insert_id(args[0]);
    args.erase(args.begin());
  }

  if (cmd->get_id_list().empty()) {
    return cmd_contact_help("no contact id specified", cmd_name);
  }

  if (_debug) {
    set<string> contacts = cmd->get_id_list();
    set<string>::const_iterator it;
    printf("contacts to be checked:\n");
    for (it = contacts.begin(); it != contacts.end(); it++) {
      printf("  [%s]\n", (*it).c_str());
    }
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// contact info command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_contact_info(vector<string> &args)
{
  ContactInfo act;
  ContactInfoCmd *cmd = act.get_command();
  string cmd_name = "info";

  if (args.empty()) {
    return cmd_contact_help("no contact id specified", cmd_name);
  }

  cmd->set_id(args[0]);
  args.erase(args.begin());

  while (!args.empty()) {
    if (args[0] == "-auth") {
      //set authInfo
      AuthInfo auth;
      if (SheppObjSet::authInfo(auth, args) != 0) {
	return cmd_contact_help("invalid auth", cmd_name);
      }
      cmd->set_authInfo(auth);
    } else {
      return cmd_contact_help("invalid syntax near \"" + args[0] + "\"",
			      cmd_name);
    }
  }

  if (_debug) {
    printf("id to get info: [%s]\n", cmd->get_id().c_str());
    SheppPrint::authInfo(cmd->get_authInfo());
  } // _debug
    
  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// contact transfer command function
/**
   @param args   vector with command-line arguments
   @return 0
*/
int cmd_contact_transfer(vector<string> &args)
{
  string cmd_name = "transfer";
  return cmd_contact_help("", cmd_name);
}

/// contact create command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_contact_create(vector<string> &args)
{
  ContactCreate act;
  ContactCreateCmd *cmd = act.get_command();
  string cmd_name = "create";

  if (args.empty()) {
    return cmd_contact_help("no contact id specified", cmd_name);
  }

  CommonData common_data;

  common_data.set_id(args[0]);
  args.erase(args.begin());

  //authInfo defaults to empty
  AuthInfo auth;
  cmd->set_authInfo(auth);

  while (!args.empty()) {
    if (args[0] == "-postalInfo") {
      //postalInfo
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_contact_help("missing postal info arguments", cmd_name);
      }
      PostalInfo postal;
      string error_msg;
      if (SheppObjSet::postalInfo(postal, args, error_msg, false) != 0) {
	return cmd_contact_help(error_msg, cmd_name);
      }
      vector<PostalInfo> postal_list = common_data.get_postal_info();
      for (int i = 0; i < (int) postal_list.size(); i++) {
	if (postal_list[i].get_type() == postal.get_type()) {
	  return cmd_contact_help("duplicated postal info type", cmd_name);
	}
      }
      common_data.insert_postal_info(postal);
    } else if (args[0] == "-voice") {
      //voice
      args.erase(args.begin());
      CommonData::Phone phone;
      if (SheppObjSet::phone(phone, args) != 0) {
	return cmd_contact_help("error setting voice telephone number",
				cmd_name);
      }
      common_data.set_voice(phone);
    } else if (args[0] == "-fax") {
      //fax
      args.erase(args.begin());
      CommonData::Phone phone;
      if (SheppObjSet::phone(phone, args) != 0) {
	return cmd_contact_help("error setting fax telephone number",
				cmd_name);
      }
      common_data.set_fax(phone);
    } else if (args[0] == "-email") {
      //email
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_contact_help("no e-mail address specified", cmd_name);
      }
      common_data.set_email(args[0]);
      args.erase(args.begin());
    } else if (args[0] == "-auth") {
      //auth
      AuthInfo auth;
      if (SheppObjSet::authInfo(auth, args) != 0) {
	return cmd_contact_help("invalid auth", cmd_name);
      }
      cmd->set_authInfo(auth);
    } else if (args[0] == "-disclose") {
      //disclose
      args.erase(args.begin());
      CommonData::Disclose disclose;

      if (args[0] != "0" && args[0] != "1") {
	return cmd_contact_help("disclose flag must be '0' or '1'", cmd_name);
      }
      disclose.flag = atoi(args[0].c_str());
      args.erase(args.begin());

      if (SheppObjSet::disclose(disclose, args[0]) != 0) {
	return cmd_contact_help("invalid diclose information", cmd_name);
      }
      args.erase(args.begin());
      common_data.set_disclose(disclose);
    } else {
      return cmd_contact_help("invalid syntax near \"" + args[0] + "\"",
			      cmd_name);
    }
  }

  if (common_data.get_postal_info().empty()) {
    return cmd_contact_help("at least one postal info must be entered",
			    cmd_name);
  }

  if (common_data.get_email() == "") {
    return cmd_contact_help("e-mail is mandatory", cmd_name);
  }

  cmd->set_common_data(common_data);

  if (_debug) {
    printf("  contact id: [%s]\n", cmd->get_common_data().get_id().c_str());
    
    vector<PostalInfo>::const_iterator it;
    vector<PostalInfo> postal_list;
    postal_list = cmd->get_common_data().get_postal_info();
    for (it = postal_list.begin(); it != postal_list.end(); it++) {
      SheppPrint::postal_info((*it));
    }
    
    if (cmd->get_common_data().get_voice().number != "") {
      printf("  voice:");
      SheppPrint::phone(cmd->get_common_data().get_voice());
    }
    
    if (cmd->get_common_data().get_fax().number != "") {
      printf("  fax  :");
      SheppPrint::phone(cmd->get_common_data().get_fax());
    }
    
    printf("  email: [%s]\n", cmd->get_common_data().get_email().c_str());
    
    SheppPrint::authInfo(cmd->get_authInfo());
    
    if (cmd->get_common_data().get_disclose().is_set()) {
      SheppPrint::disclose(cmd->get_common_data().get_disclose());
    }
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// contact delete command function
/**
   @param args   vector with command-line arguments
   @return 0
*/
int cmd_contact_delete(vector<string> &args)
{
  string cmd_name = "delete";
  return cmd_contact_help("", cmd_name);
}

/// contact renew command function
/**
   @param args   vector with command-line arguments
   @return 0
*/
int cmd_contact_renew(vector<string> &args)
{
  string cmd_name = "renew";
  return cmd_contact_help("", cmd_name);
}

/// contact update command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_contact_update(vector<string> &args)
{
  ContactUpdate act;
  ContactUpdateCmd *cmd = act.get_command();
  string cmd_name = "update";

  if (args.empty()) {
    return cmd_contact_help("no contact id specified", cmd_name);
  }

  CommonData common_data;

  common_data.set_id(args[0]);
  args.erase(args.begin());

  while (!args.empty()) {
    if (args[0] == "-add-status") {
      //add-status
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_contact_help("error setting add-status", cmd_name);
      }
      string tmp1 = args[0];
      string tmp2;
      while (SheppStrUtil::split(tmp1, tmp1, tmp2, ",", true) == 0) {
	cmd->insert_status_list_add(tmp1);
	if (tmp2 == "") {
	  break;
	}
	tmp1 = tmp2;
      }
      args.erase(args.begin());
    } else if (args[0] == "-rem-status") {
      //rem-status
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_contact_help("error setting rem-status", cmd_name);
      }
      string tmp1 = args[0];
      string tmp2;
      while (SheppStrUtil::split(tmp1, tmp1, tmp2, ",", true) == 0) {
	cmd->insert_status_list_rem(tmp1);
	if (tmp2 == "") {
	  break;
	}
	tmp1 = tmp2;
      }
      args.erase(args.begin());
    } else if (args[0] == "-postalInfo") {
      //postalInfo
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_contact_help("missing postal info arguments", cmd_name);
      }
      PostalInfo postal;
      string error_msg;
      if (SheppObjSet::postalInfo(postal, args, error_msg, true) != 0) {
	return cmd_contact_help(error_msg, cmd_name);
      }
      vector<PostalInfo> postal_list = common_data.get_postal_info();
      for (int i = 0; i < (int) postal_list.size(); i++) {
	if (postal_list[i].get_type() == postal.get_type()) {
	  return cmd_contact_help("duplicated postal info type", cmd_name);
	}
      }
      common_data.insert_postal_info(postal);
    } else if (args[0] == "-voice") {
      //voice
      args.erase(args.begin());
      CommonData::Phone phone;
      if (SheppObjSet::phone(phone, args) != 0) {
	return cmd_contact_help("error setting voice telephone number",
				cmd_name);
      }
      common_data.set_voice(phone);
    } else if (args[0] == "-fax") {
      //fax
      args.erase(args.begin());
      CommonData::Phone phone;
      if (SheppObjSet::phone(phone, args) != 0) {
	return cmd_contact_help("error setting fax telephone number",
				cmd_name);
      }
      common_data.set_fax(phone);
    } else if (args[0] == "-email") {
      //email
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_contact_help("no e-mail address specified", cmd_name);
      }
      common_data.set_email(args[0]);
      args.erase(args.begin());
    } else if (args[0] == "-auth") {
      //auth
      AuthInfo auth;
      if (SheppObjSet::authInfo(auth, args) != 0) {
	return cmd_contact_help("invalid auth", cmd_name);
      }
      cmd->set_authInfo(auth);
    } else if (args[0] == "-disclose") {
      //disclose
      args.erase(args.begin());
      CommonData::Disclose disclose;

      if (args[0] != "0" && args[0] != "1") {
	return cmd_contact_help("disclose flag must be '0' or '1'", cmd_name);
      }
      disclose.flag = atoi(args[0].c_str());
      args.erase(args.begin());

      if (SheppObjSet::disclose(disclose, args[0]) != 0) {
	return cmd_contact_help("invalid diclose information", cmd_name);
      }
      args.erase(args.begin());
      common_data.set_disclose(disclose);
    } else {
      return cmd_contact_help("invalid syntax near \"" + args[0] + "\"",
			      cmd_name);
    }
  }

  if (cmd->get_status_list_add().empty() &&
      cmd->get_status_list_rem().empty() &&
      common_data.get_postal_info().empty() &&
      common_data.get_voice().number == "" &&
      common_data.get_fax().number == "" &&
      common_data.get_email() == "" &&
      cmd->get_authInfo().get_pw() == "" &&
      !common_data.get_disclose().is_set()) {
    return cmd_contact_help("you didn't set a thing", cmd_name);
  }

  cmd->set_common_data(common_data);

  if (_debug) {
    printf("  contact id: [%s]\n", cmd->get_common_data().get_id().c_str());
    
    set<string> status = cmd->get_status_list_add();
    set<string>::const_iterator st_it;
    if (!status.empty()) {
      printf("  status to add: [ ");
      for (st_it = status.begin(); st_it != status.end(); st_it++) {
	printf("%s ", (*st_it).c_str());
      }
      printf("]\n");
    }
    
    status = cmd->get_status_list_rem();
    if (!status.empty()) {
      printf("  status to rem: [ ");
      for (st_it = status.begin(); st_it != status.end(); st_it++) {
	printf("%s ", (*st_it).c_str());
      }
      printf("]\n");
    }
    
    vector<PostalInfo>::const_iterator it;
    vector<PostalInfo> postal_list;
    postal_list = cmd->get_common_data().get_postal_info();
    for (it = postal_list.begin(); it != postal_list.end(); it++) {
      SheppPrint::postal_info((*it));
    }
    
    if (cmd->get_common_data().get_voice_f()) {
      printf("  voice:");
      SheppPrint::phone(cmd->get_common_data().get_voice());
    }
    
    if (cmd->get_common_data().get_fax_f()) {
      printf("  fax  :");
      SheppPrint::phone(cmd->get_common_data().get_fax());
    }

    if (cmd->get_common_data().get_email_f()) {
      printf("  email: [%s]\n", cmd->get_common_data().get_email().c_str());
    }
    
    if (cmd->get_authInfo().get_pw() != "") {
      SheppPrint::authInfo(cmd->get_authInfo());
    }
    
    if (cmd->get_common_data().get_disclose().is_set()) {
      SheppPrint::disclose(cmd->get_common_data().get_disclose());
    }
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// main contact command
/**
   @param arg   command-line input arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_contact(vector<string> &args)
{
  // contact command processing
  if (!args.empty() && !(args[0] == "help")) {
    if (args[0] == "check") {
      args.erase(args.begin());
      return cmd_contact_check(args);
    } else if (args[0] == "info") {
      args.erase(args.begin());
      return cmd_contact_info(args);
    } else if (args[0] == "transfer") {
      args.erase(args.begin());
      return cmd_contact_transfer(args);
    } else if (args[0] == "create") {
      args.erase(args.begin());
      return cmd_contact_create(args);
    } else if (args[0] == "delete") {
      args.erase(args.begin());
      return cmd_contact_delete(args);
    } else if (args[0] == "renew") {
      args.erase(args.begin());
      return cmd_contact_renew(args);
    } else if (args[0] == "update") {
      args.erase(args.begin());
      return cmd_contact_update(args);
    } else {
      return cmd_contact_help("invalid command: contact " + args[0]);
    }
  }
  
  return cmd_contact_help("");
}

#endif //__CONTACT_FUNCTIONS_H__
