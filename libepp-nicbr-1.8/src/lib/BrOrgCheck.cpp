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
/* $Id: BrOrgCheck.cpp 961 2008-12-11 15:11:25Z eduardo $ */
#if USE_BR_ORG
#include <map>

#include "config.h"

#include "BrOrgCheck.H"
#include "StrUtil.H"

LIBEPP_NICBR_NS_BEGIN

void BrOrgCheck::set_xml_template(const string &xml_template)
{
  StrUtil su;
  ContactCheck::set_xml_template(xml_template);

  map < string, string, less<string> > to_parse;

  map < string, string, less<string> > organization_list = 
    get_command()->get_organization_list();
  map < string, string, less<string> >::const_iterator it;
  
  for (it = organization_list.begin(); it != organization_list.end(); it++) {
    to_parse["organization_list"] += "<brorg:cd>";
    to_parse["organization_list"] += 
      "<brorg:id>" + su.esc_xml_markup((*it).first) + "</brorg:id>";
    to_parse["organization_list"] += 
      "<brorg:organization>" + su.esc_xml_markup((*it).second) +
      "</brorg:organization>";
    to_parse["organization_list"] += "</brorg:cd>";      
  }
  
  _xml = StrUtil::parse(_xml, to_parse, "$(", ")$");
}

LIBEPP_NICBR_NS_END
#endif //USE_BR_ORG
