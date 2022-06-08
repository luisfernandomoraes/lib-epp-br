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
/* $Id: StringEncodingException.H 526 2006-03-01 22:10:46Z koji $ */
/** @file StringEncodingException.H
 *  @brief String Encoding Exception Class
 */

#ifndef __STRING_ENCODING_EXCEPTION_H__
#define __STRING_ENCODING_EXCEPTION_H__

#include "libepp_nicbr.H"

#include "GeneralException.H"

LIBEPP_NICBR_NS_BEGIN

/// IO Exception Class
class StringEncodingException : public GeneralException
{
public:
  /// IO Exception Code
  enum ExceptionCode {
    MISSING_CHAR = 5000,
    ASCII_ENCODING = 5001,
    ENCODING_SEQUENCE = 5002,
    FIRST_OCTET = 5003
  };
  
  /// Constructor 
  /**
     @param code Exception Code
     @param msg Exception Message
  */
  StringEncodingException(const int code, const string &msg) : 
    GeneralException(code, msg) {}; 

  
};

LIBEPP_NICBR_NS_END
#endif //__STRING_ENCODING_EXCEPTION_H__
