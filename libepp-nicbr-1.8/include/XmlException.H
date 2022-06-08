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
/* $Id: XmlException.H 280 2006-02-07 12:22:07Z cesar $ */
/** @file XmlException.H
 *  @brief XML Exception Class
 */

#ifndef __XML_EXCEPTION_H__
#define __XML_EXCEPTION_H__

#include "libepp_nicbr.H"

#include "GeneralException.H"

LIBEPP_NICBR_NS_BEGIN

/// XML Exception Class
class XmlException : public GeneralException
{
public:
  /// XML Exception Code
  enum ExceptionCode {
    INVALID_XML_DOCUMENT = 3000,
    PARSER_INITIALIZE_EXCEPTION,  // 3001
    UNEXPECTED_PARSER_EXCEPTION, // 3002
    NULL_XML_DOCUMENT // 3003
  };
  
  /// Constructor 
  /**
     @param code Exception Code
     @param msg Exception Message
     @param low_level_msg Optional message returned by inner libraries (xerces)
  */
  XmlException(const int code, const string &msg, 
	       const string &low_level_msg = "") : 
    GeneralException(code, msg, low_level_msg) {}; 

  
};

LIBEPP_NICBR_NS_END
#endif //__XML_EXCEPTION_H__
