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
/* $Id: SheppObjSet.H 915 2007-05-31 18:19:51Z eduardo $ */
/** @file SheppObjSet.H
 *  @brief shepp specific objects information setting class
 */

#ifndef __SHEPP_OBJ_SET_H__
#define __SHEPP_OBJ_SET_H__

#include "CommonData.H"
#include "DomainUpdateCmd.H"

/// shepp specific objects information setting class
class SheppObjSet {
public:

  //used by both contact and domain

  /// fills an AuthInfo based on input command line
  /**
     @param auth    reference to an AuthInfo object to be set
     @param words   vector with command-line arguments
     @return 0 if ok, -1 otherwise
  */
  static int authInfo(AuthInfo &auth, vector<string> &words)
  {
    words.erase(words.begin());
    
    if (words.size() <= 0) {
      return -1;
    }
    
    string pw;
    string roid;
    if (SheppStrUtil::split(words[0], pw, roid, ":", true) != 0) {
      return -1;
    }
    
    auth.set_pw(pw);
    auth.set_roid(roid);
    
    words.erase(words.begin()); 
    
    return 0;
  }

  //domain specific

  /// fills a NameServer based on input command line
  /**
     @param ns      reference to a NameServer object to be set
     @param words   vector with command-line arguments
     @return 0 if ok, -1 otherwise
  */
  static int nameServer(NameServer &ns, vector<string> &words)
  {
    words.erase(words.begin());
    if (words.empty()) {
      return -1;
    }

    string ips;
    SheppStrUtil::split(words[0], ns.name, ips, ",", true);

    if (ips != "") {
      string this_ip;
      do {
	if (SheppStrUtil::split(ips, this_ip, ips, ",", true) != 0) {
	  return -1;
	}
      
	NSIPAddr ip;
	if (SheppStrUtil::split(this_ip, ip.version, ip.addr, ":", false)
	    != 0) {
	  return -1;
	}

	ns.ips.insert(ip);
      } while (ips != "");
    }

    words.erase(words.begin());
  
    return 0;
  }

  /// fills a Status based on input command line
  /**
     @param status      reference to a Status object to be set
     @param words       vector with command-line arguments
     @return 0 if ok, -1 otherwise
  */
  static int status(DomainUpdateCmd::Status &st, vector<string> &words)
  {
    string status;
    string lang;
    string description;

    if (SheppStrUtil::split(words[0], status, lang, ":", false) != 0) {
      return -1;
    }
    words.erase(words.begin());

    if (SheppStrUtil::split(lang, lang, description, ":", false) != 0) {
      return -1;
    }

    SheppStrUtil::quote_gathering(words, description);

    st.s = status;
    st.lang = lang;
    st.msg = description;

    return 0;
  }

  /// fills a DSInfo based on input command line
  /**
     @param ds      reference to a DSInfo object to be set
     @param words   vector with command-line arguments
     @return 0 if ok, -1 otherwise
  */
  static int dsInfo(DSInfo &ds, vector<string> &words, string &error_msg)
  {
    words.erase(words.begin()); // -ds

    // must have at least 4 elements: keytag, algorithm, digtype and digest
    if (words.size() < 4) {
      error_msg = "not enough arguments for DS";
      return -1;
    }

    // parse the key tag: check if it has only numbers
    string c;
    for (unsigned int i = 0; i < words[0].length(); i++) {
      c = words[0][i];
      if (c != "0" && atoi(c.c_str()) == 0) {
	error_msg = "keyTag must be a number";
	return -1;
      }
    }

    int key_tag = atoi(words[0].c_str());
    if (key_tag < 0) {
      error_msg = "keyTag cannot be a negative number";
      return -1;
    }
    ds.set_key_tag(key_tag);

    int alg = atoi(words[1].c_str());
    if (alg < 0) {
      error_msg = "alg cannot be a negative number";
      return -1;
    }
    ds.set_algo(alg);

    int dig_type = atoi(words[2].c_str());
    if (dig_type < 0) {
      error_msg = "digestType cannot be a negative number";
      return -1;
    }
    ds.set_digest_type(dig_type);
    ds.set_digest(words[3]);

    words.erase(words.begin(), words.begin() + 4);

    //optional parameters
    while (!words.empty()) {
      if (words[0] == "-maxlife") {
	if (words.size() < 2) {
	  error_msg = "maxSigLife value missing";
	  return -1;
	}
	words.erase(words.begin()); // -maxlife
	int maxlife = atoi(words[0].c_str());
	if (maxlife < 0) {
	  error_msg = "maxSigLife cannot be a negative number";
	  return -1;
	}
	ds.set_max_sig_life(maxlife);
	words.erase(words.begin()); // maxlife value
      } else if (words[0] == "-keyData") {
	if (words.size() < 5) {
	  error_msg = "not enough arguments for keyData";
	  return -1;
	}
	DSInfo::KeyData kd;
	kd._flags = atoi(words[1].c_str());
	kd._protocol = atoi(words[2].c_str());
	kd._algorithm = atoi(words[3].c_str());
	kd._pub_key = words[4];

	if (kd._flags < 0 || kd._protocol < 0 || kd._algorithm < 0) {
	  error_msg =
	    "keyData flags, protocol and alg cannot be negative numbers";
	  return -1;
	}
	words.erase(words.begin(), words.begin() + 5); 
	ds.set_key_data(kd);
      } else {
	break;
      }
    }

    return 0;
  }

  //contact specific

  /// fills a PostalInfo based on input command line
  /**
     @param postal      reference to a PostalInfo object to be set
     @param words       vector with command-line arguments
     @param error_msg   string to hold error msg on return -1
     @param relaxed     true for update, false for create
     @return 0 if ok, -1 otherwise
  */
  static int postalInfo(PostalInfo &postal, vector<string> &words,
			string &error_msg, bool relaxed)
  {
    //type
    string postal_type = words[0];
    words.erase(words.begin());
    if (SheppStrUtil::quote_gathering(words, postal_type) != 0) {
      error_msg = "error setting postalInfo type";
      return -1;
    }
    postal.set_type(postal_type);

    while (!words.empty()) {
      if (words[0] == "-name") {
	//name
	words.erase(words.begin());
	if (words.empty()) {
	  error_msg = "unspecified contact name";
	  return -1;
	}
	string postal_name = words[0];
	words.erase(words.begin());
	if (SheppStrUtil::quote_gathering(words, postal_name) != 0) {
	  error_msg = "error setting contact name";
	  return -1;
	}
	postal.set_name(postal_name);
      } else if (words[0] == "-org") {
	//org
	words.erase(words.begin());
	if (words.empty()) {
	  error_msg = "unspecified organization";
	  return -1;
	}
	string postal_org = words[0];
	words.erase(words.begin());
	if (SheppStrUtil::quote_gathering(words, postal_org) != 0) {
	  error_msg = "error setting organization";
	  return -1;
	}
	postal.set_org(postal_org);
      } else if (words[0] == "-street1") {
	//street1
	words.erase(words.begin());
	if (words.empty()) {
	  error_msg = "unspecified address street/line 1";
	  return -1;
	}
	string postal_str1 = words[0];
	words.erase(words.begin());
	if (SheppStrUtil::quote_gathering(words, postal_str1) != 0) {
	  error_msg = "error setting address street/line 1";
	  return -1;
	}
	postal.set_str1(postal_str1);
      } else if (words[0] == "-street2") {
	//street2
	words.erase(words.begin());
	if (words.empty()) {
	  error_msg = "unspecified address number/line 2";
	  return -1;
	}
	string postal_str2 = words[0];
	words.erase(words.begin());
	if (SheppStrUtil::quote_gathering(words, postal_str2) != 0) {
	  error_msg = "error setting address number/line 2";
	  return -1;
	}
	postal.set_str2(postal_str2);
      } else if (words[0] == "-street3") {
	//street3
	words.erase(words.begin());
	if (words.empty()) {
	  error_msg = "unspecified street3";
	  return -1;
	}
	string postal_str3 = words[0];
	words.erase(words.begin());
	if (SheppStrUtil::quote_gathering(words, postal_str3) != 0) {
	  error_msg = "error setting address line 3";
	  return -1;
	}
	postal.set_str3(postal_str3);
      } else if (words[0] == "-city") {
	//city
	words.erase(words.begin());
	if (words.empty()) {
	  error_msg = "unspecified city";
	  return -1;
	}
	string postal_city = words[0];
	words.erase(words.begin());
	if (SheppStrUtil::quote_gathering(words, postal_city) != 0) {
	  error_msg = "error setting city";
	  return -1;
	}
	postal.set_city(postal_city);
      } else if (words[0] == "-state") {
	//state
	words.erase(words.begin());
	if (words.empty()) {
	  error_msg = "unspecified state";
	  return -1;
	}
	string postal_sp = words[0];
	words.erase(words.begin());
	if (SheppStrUtil::quote_gathering(words, postal_sp) != 0) {
	  error_msg = "error setting state/province";
	  return -1;
	}
	postal.set_sp(postal_sp);
      } else if (words[0] == "-pc") {
	//pc
	words.erase(words.begin());
	if (words.empty()) {
	  error_msg = "unspecified postal code";
	  return -1;
	}
	string postal_pc = words[0];
	words.erase(words.begin());
	if (SheppStrUtil::quote_gathering(words, postal_pc) != 0) {
	  error_msg = "error setting postal code";
	  return -1;
	}
	postal.set_pc(postal_pc);
      } else if (words[0] == "-cc") {
	//cc
	words.erase(words.begin());
	if (words.empty()) {
	  error_msg = "unspecified country code";
	  return -1;
	}
	string postal_cc = words[0];
	words.erase(words.begin());
	if (SheppStrUtil::quote_gathering(words, postal_cc) != 0) {
	  error_msg = "error setting country code";
	  return -1;
	}
	postal.set_cc(postal_cc);
      } else {
	break;
      }
    }

    //relaxed: update; !relaxed: create
    if (!relaxed) {
      if (postal.get_name() == "") {
	error_msg = "name is mandatory";
	return -1;
      }

      if (postal.get_str1() == "") {
	error_msg = "address street/line 1 is mandatory";
	return -1;
      }
    
      if (postal.get_city() == "") {
	error_msg = "city is mandatory";
	return -1;
      }
    
      if (postal.get_cc() == "") {
	error_msg = "country code is mandatory";
	return -1;
      }
    } else {
      if (postal.get_name() == "" &&
	  postal.get_org() == "" &&
	  postal.get_str1() == "" &&
	  postal.get_str2() == "" &&
	  postal.get_str3() == "" &&
	  postal.get_city() == "" &&
	  postal.get_sp() == "" &&
	  postal.get_pc() == "" &&
	  postal.get_cc() == "") {
	error_msg = "empty postal info";
	return -1;
      }
    }   

    return 0;
  }

  /// fills a Phone based on input command line
  /**
     @param phone   reference to a Phone object to be set
     @param words   vector with command-line arguments
     @return 0 if ok, -1 otherwise
  */
  static int phone(CommonData::Phone &phone, vector<string> &words)
  {
    if (words.empty() ||
	SheppStrUtil::split(words[0], phone.number, phone.ext, ":", true)
	!= 0) {
      return -1;
    }
    words.erase(words.begin());
    return 0;
  }

  /// fills a Disclose based on input command line
  /**
     @param disclose   reference to a Disclose object to be set
     @param word       input string with disclose info
     @return 0 if ok, -1 otherwise
  */
  static int disclose(CommonData::Disclose &disclose, string word)
  {
    string opt1 = word;
    string opt2;
    while (opt1 != "" &&
	   SheppStrUtil::split(opt1, opt1, opt2, ",", true) == 0) {
      if (opt1 == "name_int") {
	disclose.name_int = true;
      } else if (opt1 == "name_loc") {
	disclose.name_loc = true;
      } else if (opt1 == "org_int") {
	disclose.org_int = true;
      } else if (opt1 == "org_loc") {
	disclose.org_loc = true;
      } else if (opt1 == "addr_int") {
	disclose.addr_int = true;
      } else if (opt1 == "addr_loc") {
	disclose.addr_loc = true;
      } else if (opt1 == "voice") {
	disclose.voice = true;
      } else if (opt1 == "fax") {
	disclose.fax = true;
      } else if (opt1 == "email") {
	disclose.email = true;
      } else {
	return -1;
      }
      opt1 = opt2;
    }
    return 0;
  }

  // brorg specific

  /// fills a map of contacts based on input command line
  /**
     @param contacts    reference to the map of contacts to be filled
     @param args        input string with contacts info
     @param error_msg   string to hold error msg on return -1
     @return 0 if ok, -1 otherwise
   */
  static int contacts(map<string, string, less<string> > &contacts,
		      string args, string &error_msg)
  {
    string one = args;
    string rest;

    do {
      if (SheppStrUtil::split(one, one, rest, ",", true) != 0) {
	error_msg = "invalid contact";
	return -1;
      }

      string key;
      string value;
      if (SheppStrUtil::split(one, key, value, "=", false) != 0) {
	error_msg = "invalid contact";
	return -1;
      }

      contacts[key] = value;

      one = rest;
    } while (one != "");

    return 0;
  }
};

#endif //__SHEPP_OBJ_SET_H__
