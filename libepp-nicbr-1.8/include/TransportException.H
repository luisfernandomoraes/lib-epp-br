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
/* $Id: TransportException.H 767 2006-06-19 21:31:58Z koji $ */
/** @file TransportException.H
 *  @brief Transport Exception Class
 */

#ifndef __TRANSPORT_EXCEPTION_H__
#define __TRANSPORT_EXCEPTION_H__

#include "libepp_nicbr.H"

#include "GeneralException.H"

LIBEPP_NICBR_NS_BEGIN

/// Transport Exception Class
class TransportException : public GeneralException 
{
public:
  /// Transport Exception Code
  enum ExceptionCode {
    BIO_DO_CONNECT_ERR = 1000,
    LOST_CONNECTION = 1001,
    SSL_NEW_ERR = 1002,
    SSL_CONNECT_ERR = 1003,
    SSL_READ_ERR = 1004,
    SSL_WRITE_ERR = 1005,
    READ_TIMEOUT = 1006,
    WRITE_TIMEOUT = 1007,
    MISSING_TOTAL_LENGTH = 1008,
    INVALID_TOTAL_LENGTH = 1009,
    PAYLOAD_INCOMPLETE = 1010,
    LOAD_CERT_FILE_ERR = 1011,
    LOAD_KEY_FILE_ERR = 1012,
    GET_FNCTL_ERR = 1013,
    SET_NONBLOCKING_ERR = 1014,
    LOAD_ROOTCA_FILE_ERR = 1015,
    PEER_CERTIFICATE_CN_ERR = 1016
  };

  /// Construtor 
  /**
     @param code Exception Code
     @param msg Exception Message
     @param low_level_msg Optional message returned by inner libraries (openssl)
  */  
  TransportException(const ExceptionCode &code, const string &msg, 
		     const string &low_level_msg = "") : 
    GeneralException(code, msg, low_level_msg) {};
  
private:
  
};

LIBEPP_NICBR_NS_END
#endif //__TRANSPORT_EXCEPTION_H__
