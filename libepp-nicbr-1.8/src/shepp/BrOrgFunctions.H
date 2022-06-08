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
/* $Id: BrOrgFunctions.H 1002 2009-01-23 09:54:19Z rafael $ */
/** @file BrOrgFunctions.H
 *  @brief EPP BrOrg related functions
 */
#ifndef __BRORG_FUNCTIONS_H__
#define __BRORG_FUNCTIONS_H__
#include "SheppCommandFunctions.H"

#include "BrOrgCheck.H"
#include "BrOrgInfo.H"
#include "BrOrgCreate.H"
#include "BrOrgUpdate.H"

/// print brorg command usage info
/**
   @param error_msg   error message to be printed
   @param specific    optional, choses specific sub-command
   @return 0 if ok, -1 otherwise
*/
int cmd_brorg_help(string error_msg, string specific = "")
{
  if (error_msg != "") {
    printf("error: %s\n", error_msg.c_str());
  }

  printf("brorg command syntax help:\n");
  printf("\n");

  if (specific == "" || specific == "check") {
    printf("  check <brorg1> [brorg2 ... brorgN]\n");
    printf("\n");
  }
  if (specific == "" || specific == "info") {
    printf("  info <brorg> [-auth authInfoPw[:authInfoRoid]]\n");
    printf("\n");
  }
  if (specific == "" || specific == "transfer") {
    printf("  transfer: not implemented\n");
    printf("\n");
  }
  if (specific == "" || specific == "create") {
    printf("  create <brorg>\n");
    printf("    [-postalInfo <type> <-name \"BrOrg Name\"> "
	   "[-org \"Org Name\"]\n");
    printf("    <-street1 \"address street/line 1\"> "
	   "[-street2 \"address number/line 2\"]\n");
    printf("    [-street3 \"address line 3\"] <-city \"City Name\"> "
	   "[-state \"State or Province\"]\n");
    printf("    [-pc \"postal code\"] <-cc country-code> ...]\n");
    printf("    <-voice number:ext> [-fax number:ext]\n");
    printf("    <-email emailAddr> [-auth authInfoPw[:authInfoRoid]]\n");
    printf("    [-disclose opt,opt,...] "
	   " [-responsible \"Responsible Name\"]\n"
	   "     -contact key=value[,key=value,...]\n");
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
    printf("  update <brorg> [-add-status st1,...,stN] "
	   "[-rem-status st1,...,stN]\n");
    printf("    [-postalInfo <type> [-name \"BrOrg Name\"] "
	   "[-org \"Org Name\"]\n");
    printf("    [-street1 \"address street/line 1\"] "
	   "[-street2 \"address number/line 2\"]\n");
    printf("    [-street3 \"address line 3\"] [-city \"City Name\"] "
	   "[-state \"State or Province\"]\n");
    printf("    [-pc \"postal code\"] [-cc country-code] ...]\n");
    printf("    [-voice number:ext] [-fax number:ext]\n");
    printf("    [-email emailAddr] [-auth authInfoPw[:authInfoRoid]] "
	   "[-disclose opt,opt,...]\n");
    printf("    [-responsible \"Responsible Name\"]\n");
    printf("    [-exdate expirationDate]\n");
    printf("    [-add-contact key=value[,key=value,...]\n");
    printf("    [-rem-contact key=value[,key=value,...]\n");
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

/// brorg check command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_brorg_check(vector<string> &args)
{
  BrOrgCheck act;
  BrOrgCheckCmd *cmd = act.get_command();
  string cmd_name = "check";

  string id;
  int counter = 0;
  while (!args.empty()) {
    string numbers = SheppStrUtil::doc2id(args[0]);
    cmd->insert_id(numbers);
    cmd->insert_organization(numbers, args[0]);
    args.erase(args.begin());
    counter++;
  }

  if (cmd->get_id_list().empty() || cmd->get_organization_list().empty()) {
    return cmd_brorg_help("no brorg specified", cmd_name);
  }

  if (_debug) {
    map<string, string, less<string> > brorgs = cmd->get_organization_list();
    map<string, string, less<string> >::const_iterator it;
    printf("brorgs to be checked:\n");
    for (it = brorgs.begin(); it != brorgs.end(); it++) {
      printf("  id: [%s] org: [%s]\n", (*it).first.c_str(),
	     (*it).second.c_str());
    }
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// brorg info command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_brorg_info(vector<string> &args)
{
  BrOrgInfo act;
  BrOrgInfoCmd *cmd = act.get_command();
  string cmd_name = "info";

  if (args.empty()) {
    return cmd_brorg_help("no brorg specified", cmd_name);
  }

  cmd->set_id(SheppStrUtil::doc2id(args[0]));
  cmd->set_organization(args[0]);
  args.erase(args.begin());

  while (!args.empty()) {
    if (args[0] == "-auth") {
      //set authInfo
      AuthInfo auth;
      if (SheppObjSet::authInfo(auth, args) != 0) {
	return cmd_brorg_help("invalid auth", cmd_name);
      }
      cmd->set_authInfo(auth);
    } else {
      return cmd_brorg_help("invalid syntax near \"" + args[0] + "\"",
			    cmd_name);
    }

  }

  if (_debug) {
    printf("id  to get info: [%s]\n", cmd->get_id().c_str());
    printf("org to get info: [%s]\n", cmd->get_organization().c_str());
    SheppPrint::authInfo(cmd->get_authInfo());
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// brorg transfer command function
/**
   @param args   vector with command-line arguments
   @return 0
*/
int cmd_brorg_transfer(vector<string> &args)
{
  string cmd_name = "transfer";
  return cmd_brorg_help("", cmd_name);
}

/// brorg create command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_brorg_create(vector<string> &args)
{
  BrOrgCreate act;
  BrOrgCreateCmd *cmd = act.get_command();
  string cmd_name = "create";

  if (args.empty()) {
    return cmd_brorg_help("no brorg specified", cmd_name);
  }

  CommonData common_data;

  common_data.set_id(SheppStrUtil::doc2id(args[0]));
  cmd->set_organization(args[0]);

  args.erase(args.begin());

  //authInfo defaults to empty
  AuthInfo auth;
  cmd->set_authInfo(auth);

  while (!args.empty()) {
    if (args[0] == "-postalInfo") {
      //postalInfo
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_brorg_help("missing postal info arguments", cmd_name);
      }
      PostalInfo postal;
      string error_msg;
      if (SheppObjSet::postalInfo(postal, args, error_msg, false) != 0) {
	return cmd_brorg_help(error_msg, cmd_name);
      }
      vector<PostalInfo> postal_list = common_data.get_postal_info();
      for (int i = 0; i < (int) postal_list.size(); i++) {
	if (postal_list[i].get_type() == postal.get_type()) {
	  return cmd_brorg_help("duplicated postal info type", cmd_name);
	}
      }
      common_data.insert_postal_info(postal);
    } else if (args[0] == "-voice") {
      //voice
      args.erase(args.begin());
      CommonData::Phone phone;
      if (SheppObjSet::phone(phone, args) != 0) {
	return cmd_brorg_help("error setting voice telephone number",
			      cmd_name);
      }
      common_data.set_voice(phone);
    } else if (args[0] == "-fax") {
      //fax
      args.erase(args.begin());
      CommonData::Phone phone;
      if (SheppObjSet::phone(phone, args) != 0) {
	return cmd_brorg_help("error setting fax telephone number",
			      cmd_name);
      }
      common_data.set_fax(phone);
    } else if (args[0] == "-email") {
      //email
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_brorg_help("no e-mail address specified", cmd_name);
      }
      common_data.set_email(args[0]);
      args.erase(args.begin());
    } else if (args[0] == "-auth") {
      //auth
      AuthInfo auth;
      if (SheppObjSet::authInfo(auth, args) != 0) {
	return cmd_brorg_help("invalid auth", cmd_name);
      }
      cmd->set_authInfo(auth);
    } else if (args[0] == "-disclose") {
      //disclose
      args.erase(args.begin());
      CommonData::Disclose disclose;
      if (SheppObjSet::disclose(disclose, args[0]) != 0) {
	return cmd_brorg_help("invalid diclose information", cmd_name);
      }
      args.erase(args.begin());
      common_data.set_disclose(disclose);
    } else if (args[0] == "-contact") {
      //contacts
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_brorg_help("-contact requires an argument", cmd_name);
      }
      map<string, string, less<string> > my_contacts;
      string error_msg;
      if (SheppObjSet::contacts(my_contacts, args[0], error_msg) != 0) {
	return cmd_brorg_help(error_msg, cmd_name);
      }
      map<string, string, less<string> >::const_iterator it;
      for (it = my_contacts.begin(); it != my_contacts.end(); it++) {
	cmd->insert_contact((*it).first, (*it).second);
      }
      args.erase(args.begin());
    } else if (args[0] == "-responsible") {
      //responsible
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_brorg_help("unspecified responsible name", cmd_name);
      }
      string responsible_name = args[0];
      args.erase(args.begin());
      if (SheppStrUtil::quote_gathering(args, responsible_name) != 0) {
	return cmd_brorg_help("error setting responsible name", cmd_name);
      }
      cmd->set_responsible(responsible_name);
    } else {
      return cmd_brorg_help("invalid syntax near \"" + args[0] + "\"",
			    cmd_name);
    }
  }

  if (common_data.get_postal_info().empty()) {
    return cmd_brorg_help("at least one postal info must be entered",
			  cmd_name);
  }

  if (common_data.get_email() == "") {
    return cmd_brorg_help("e-mail is mandatory", cmd_name);
  }

  if (common_data.get_voice().number == "") {
    return cmd_brorg_help("voice telephone number is mandatory", cmd_name);
  }

  if (cmd->get_organization() == "") {
    return cmd_brorg_help("orgid unset", cmd_name);
  }

  if (cmd->get_contact_list().empty()) {
    return cmd_brorg_help("at least one contact must be entered", cmd_name);
  }

  cmd->set_common_data(common_data);

  if (_debug) {
    printf("  brorg: [%s]\n", cmd->get_common_data().get_id().c_str());
    
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
    
    //brorg
    printf("  orgid: [%s]\n", cmd->get_organization().c_str());
    
    map<string, string, less<string> > contacts = cmd->get_contact_list();
    map<string, string, less<string> >::const_iterator c_it;
    for (c_it = contacts.begin(); c_it != contacts.end(); c_it++) {
      printf("  contact[%s] = [%s]\n", (*c_it).first.c_str(),
	     (*c_it).second.c_str());
    }
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// brorg delete command function
/**
   @param args   vector with command-line arguments
   @return 0
*/
int cmd_brorg_delete(vector<string> &args)
{
  string cmd_name = "delete";
  return cmd_brorg_help("", cmd_name);
}

/// brorg renew command function
/**
   @param args   vector with command-line arguments
   @return 0
*/
int cmd_brorg_renew(vector<string> &args)
{
  string cmd_name = "renew";
  return cmd_brorg_help("", cmd_name);
}

/// brorg update command function
/**
   @param args   vector with command-line arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_brorg_update(vector<string> &args)
{
  BrOrgUpdate act;
  BrOrgUpdateCmd *cmd = act.get_command();
  string cmd_name = "update";

  if (args.empty()) {
    return cmd_brorg_help("no brorg specified", cmd_name);
  }

  CommonData common_data;
  common_data.set_id(SheppStrUtil::doc2id(args[0]));

  cmd->set_organization(args[0]);
  args.erase(args.begin());

  while (!args.empty()) {
    if (args[0] == "-add-status") {
      //add-status
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_brorg_help("error setting add-status", cmd_name);
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
	return cmd_brorg_help("error setting rem-status", cmd_name);
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
	return cmd_brorg_help("missing postal info arguments", cmd_name);
      }
      PostalInfo postal;
      string error_msg;
      if (SheppObjSet::postalInfo(postal, args, error_msg, true) != 0) {
	return cmd_brorg_help(error_msg, cmd_name);
      }
      vector<PostalInfo> postal_list = common_data.get_postal_info();
      for (int i = 0; i < (int) postal_list.size(); i++) {
	if (postal_list[i].get_type() == postal.get_type()) {
	  return cmd_brorg_help("duplicated postal info type", cmd_name);
	}
      }
      common_data.insert_postal_info(postal);
    } else if (args[0] == "-voice") {
      //voice
      args.erase(args.begin());
      CommonData::Phone phone;
      if (SheppObjSet::phone(phone, args) != 0) {
	return cmd_brorg_help("error setting voice telephone number",
			      cmd_name);
      }
      common_data.set_voice(phone);
    } else if (args[0] == "-fax") {
      //fax
      args.erase(args.begin());
      CommonData::Phone phone;
      if (SheppObjSet::phone(phone, args) != 0) {
	return cmd_brorg_help("error setting fax telephone number",
			      cmd_name);
      }
      common_data.set_fax(phone);
    } else if (args[0] == "-email") {
      //email
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_brorg_help("no e-mail address specified", cmd_name);
      }
      common_data.set_email(args[0]);
      args.erase(args.begin());
    } else if (args[0] == "-auth") {
      //auth
      AuthInfo auth;
      if (SheppObjSet::authInfo(auth, args) != 0) {
	return cmd_brorg_help("invalid auth", cmd_name);
      }
      cmd->set_authInfo(auth);
    } else if (args[0] == "-disclose") {
      //disclose
      args.erase(args.begin());
      CommonData::Disclose disclose;
      if (SheppObjSet::disclose(disclose, args[0]) != 0) {
	return cmd_brorg_help("invalid diclose information", cmd_name);
      }
      args.erase(args.begin());
      common_data.set_disclose(disclose);
    } else if (args[0] == "-add-contact") {
      //add contacts
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_brorg_help("-add-contact requires an argument",
			      cmd_name);
      }
      map<string, string, less<string> > my_contacts;
      string error_msg;
      if (SheppObjSet::contacts(my_contacts, args[0], error_msg) != 0) {
	return cmd_brorg_help(error_msg, cmd_name);
      }
      map<string, string, less<string> >::const_iterator it;
      for (it = my_contacts.begin(); it != my_contacts.end(); it++) {
	cmd->insert_contact_add((*it).first, (*it).second);
      }
      args.erase(args.begin());
    } else if (args[0] == "-rem-contact") {
      //remove contacts
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_brorg_help("-rem-contact requires an argument",
			      cmd_name);
      }
      map<string, string, less<string> > my_contacts;
      string error_msg;
      if (SheppObjSet::contacts(my_contacts, args[0], error_msg) != 0) {
	return cmd_brorg_help(error_msg, cmd_name);
      }
      map<string, string, less<string> >::const_iterator it;
      for (it = my_contacts.begin(); it != my_contacts.end(); it++) {
	cmd->insert_contact_rem((*it).first, (*it).second);
      }
      args.erase(args.begin());

    } else if (args[0] == "-responsible") {
      //responsible
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_brorg_help("unspecified responsible name", cmd_name);
      }
      string responsible_name = args[0];
      args.erase(args.begin());
      if (SheppStrUtil::quote_gathering(args, responsible_name) != 0) {
	return cmd_brorg_help("error setting responsible name", cmd_name);
      }
      cmd->set_responsible(responsible_name);
    } else if (args[0] == "-exdate") {
      //expiration date
      args.erase(args.begin());
      if (args.empty()) {
	return cmd_brorg_help("unspecified expiration date", cmd_name);
      }
      cmd->set_exDate(args[0]);
      args.erase(args.begin());
    } else {
      return cmd_brorg_help("invalid syntax near \"" + args[0] + "\"",
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
      !common_data.get_disclose().is_set() &&
      cmd->get_organization() == "") {
    return cmd_brorg_help("not enough information for update", cmd_name);
  }

  cmd->set_common_data(common_data);

  if (_debug) {
    printf("  brorg: [%s]\n", cmd->get_common_data().get_id().c_str());
    
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
    
    vector<PostalInfo> postal_list;
    postal_list = cmd->get_common_data().get_postal_info();
    for (int i = 0; i < (int) postal_list.size(); i++) {
      SheppPrint::postal_info(postal_list[i]);
    }
    
    if (cmd->get_common_data().get_voice().number != "") {
      printf("  voice:");
      SheppPrint::phone(cmd->get_common_data().get_voice());
    }
    
    if (cmd->get_common_data().get_fax().number != "") {
      printf("  fax  :");
      SheppPrint::phone(cmd->get_common_data().get_fax());
    }
    
    if (cmd->get_common_data().get_email() != "") {
      printf("  email: [%s]\n", cmd->get_common_data().get_email().c_str());
    }
    
    if (cmd->get_authInfo().get_pw() != "") {
      SheppPrint::authInfo(cmd->get_authInfo());
    }
    
    if (cmd->get_common_data().get_disclose().is_set()) {
      SheppPrint::disclose(cmd->get_common_data().get_disclose());
    }
    
    //brorg
    if (cmd->get_organization() != "") {
      printf("  orgid: [%s]\n", cmd->get_organization().c_str());
      
      map<string, string, less<string> > contacts =
	cmd->get_contact_list_add();
      map<string, string, less<string> >::const_iterator c_it;
      for (c_it = contacts.begin(); c_it != contacts.end(); c_it++) {
	printf("  contact to add [%s] = [%s]\n", (*c_it).first.c_str(),
	       (*c_it).second.c_str());
      }
      
      contacts = cmd->get_contact_list_rem();
      for (c_it = contacts.begin(); c_it != contacts.end(); c_it++) {
	printf("  contact to rem [%s] = [%s]\n", (*c_it).first.c_str(),
	       (*c_it).second.c_str());
      }
    }
  } // _debug

  if (process_action(act) != 0) {
    return -1;
  }

  return 0;
}

/// main brorg command
/**
   @param arg   command-line input arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_brorg(vector<string> &args)
{
  // brorg command processing
  if (!args.empty() && !(args[0] == "help")) {
    if (args[0] == "check") {
      args.erase(args.begin());
      return cmd_brorg_check(args);
    } else if (args[0] == "info") {
      args.erase(args.begin());
      return cmd_brorg_info(args);
    } else if (args[0] == "transfer") {
      args.erase(args.begin());
      return cmd_brorg_transfer(args);
    } else if (args[0] == "create") {
      args.erase(args.begin());
      return cmd_brorg_create(args);
    } else if (args[0] == "delete") {
      args.erase(args.begin());
      return cmd_brorg_delete(args);
    } else if (args[0] == "renew") {
      args.erase(args.begin());
      return cmd_brorg_renew(args);
    } else if (args[0] == "update") {
      args.erase(args.begin());
      return cmd_brorg_update(args);
    } else {
      return cmd_brorg_help("invalid command: brorg " + args[0]);
    }
  }
  
  return cmd_brorg_help("");
}

#endif //__BRORG_FUNCTIONS_H__
