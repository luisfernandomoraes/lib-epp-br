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
/* $Id: Hello.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file Hello.H
 *  @brief EPP Hello Class
 */

#ifndef __HELLO_H__
#define __HELLO_H__

#include <string>

#include "libepp_nicbr.H"

LIBEPP_NICBR_NS_BEGIN

using std::string;

/// EPP Hello Class
class Hello
{
public:  
  /// Constructor
  Hello() 
  {
   _xml = 
     "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
     "<epp xmlns=\"urn:ietf:params:xml:ns:epp-1.0\" "
     "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
     "xsi:schemaLocation=\"urn:ietf:params:xml:ns:epp-1.0 "
     "epp-1.0.xsd\">"
     "<hello/>"
     "</epp>";
  };
  
  /// Returns Hello in XML format
  /**
     @return Hello in XML format
  */
  string get_xml_format() const { return _xml; }

protected:
  /// Hello in XML format
  string _xml;
 
};

LIBEPP_NICBR_NS_END
#endif //__HELLO_H__
