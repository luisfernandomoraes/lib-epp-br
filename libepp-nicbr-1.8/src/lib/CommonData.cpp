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
/* $Id: CommonData.cpp 1040 2009-10-01 17:49:12Z fneves $ */

#include "config.h"

#include "CommonData.H"

LIBEPP_NICBR_NS_BEGIN

void CommonData::set_id(const string &id) 
{ 
  _id = id;
}

void CommonData::insert_postal_info(const PostalInfo &postal_info)
{ 
  _postal_info.push_back(postal_info);     
}

void CommonData::set_voice(const Phone &voice)
{
  _voice = voice;
  _voice_f = true;
}
  
void CommonData::set_fax(const Phone &fax)
{
  _fax = fax;
  _fax_f = true;
}
  
void CommonData::set_email(const string &email)
{
  _email = email;
  _email_f = true;
}
  
void CommonData::set_disclose(const Disclose &disclose)
{
  _disclose = disclose;
}

string CommonData::get_id() const
{ 
  return _id; 
}

vector<PostalInfo> CommonData::get_postal_info() const
{ 
  return _postal_info; 
}

CommonData::Phone CommonData::get_voice() const
{
  return _voice;
}
  
CommonData::Phone CommonData::get_fax() const
{
  return _fax;
}
  
string CommonData::get_email() const
{
  return _email;
}
  
CommonData::Disclose CommonData::get_disclose() const
{
  return _disclose;
}

void CommonData::reset()
{ 
  _id = "";

  _postal_info.clear();

  _voice.ext = "";
  _voice.number = "";
  _voice_f = false;

  _fax.ext = "";
  _fax.number = "";
  _fax_f = false;

  _email = "";
  _email_f = false;

  _disclose.flag = -1;
  _disclose.name_int = false;
  _disclose.name_loc = false;
  _disclose.org_int = false;
  _disclose.org_loc = false;
  _disclose.addr_int = false;
  _disclose.addr_loc = false;
  _disclose.voice = false;
  _disclose.fax = false;
  _disclose. email = false;
}

LIBEPP_NICBR_NS_END
