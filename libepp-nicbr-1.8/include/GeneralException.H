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
/* $Id: GeneralException.H 298 2006-02-07 19:19:57Z milena $ */
/** @file GeneralException.H
 *  @brief General Exception Class
 */

#ifndef __GENERAL_EXCEPTION_H__
#define __GENERAL_EXCEPTION_H__

#include <string>

#include "libepp_nicbr.H"

using std::string;

LIBEPP_NICBR_NS_BEGIN

/// General Exception Class
class GeneralException 
{
public:
  
  /// Constructor 
  /**
     @param code Exception Code
     @param msg Exception Message
     @param low_level_msg Optional message returned by inner libraries
  */
  GeneralException(const int code, const string &msg,
		   const string &low_level_msg = "") : 
    _code(code), _msg(msg), _low_level_msg(low_level_msg) {}; 

  /// Returns the Exception Code
  /**
     @return Exception Code
  */
  int get_code() const { return _code; }; 

  /// Returns the Exception Message
  /**
     @return Exception Message
  */
  string get_msg() const { return _msg; };

  /// Return Low Level Error Message
  /**
     @return Low Level Error Message when available, "" otherwise
  */
  string get_low_level_msg() const { return _low_level_msg; };
  
protected:  
  /// Exception Code
  int _code;
  
  /// Exception Message
  string _msg;

  /// Low Level Message
  string _low_level_msg;
};

LIBEPP_NICBR_NS_END
#endif //__GENERAL_EXCEPTION_H__
