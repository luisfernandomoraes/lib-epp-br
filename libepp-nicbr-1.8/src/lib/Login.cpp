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
/* $Id: Login.cpp 756 2006-06-09 17:42:34Z koji $ */

#include "config.h"

#include "Login.H"
#include "StrUtil.H"

LIBEPP_NICBR_NS_BEGIN
  
void Login::set_xml_template(const string &xml_template)
{
  StrUtil su;
  Action::set_xml_template_common(xml_template);
  map < string, string, less<string> > to_parse;
  to_parse["clID"] = su.esc_xml_markup(get_command()->get_clID());
  to_parse["pw"] = su.esc_xml_markup(get_command()->get_pw());
  if (strcmp(get_command()->get_new_pw().c_str(), "") == 0) {
    to_parse["newPW"] = "";
  } else {
    to_parse["newPW"] =  "<newPW>" +
      su.esc_xml_markup(get_command()->get_new_pw()) + "</newPW>";
  }

  if (get_command()->get_lang() == "") {
    to_parse["lang"] = "en";
  } else {
    to_parse["lang"] = su.esc_xml_markup(get_command()->get_lang());
  }

  _xml = StrUtil::parse(_xml, to_parse, "$(", ")$");
}

LIBEPP_NICBR_NS_END
