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
/* $Id: TransportTLSCommon.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file TransportTLSCommon.H
 *  @brief TransportTLSCommon class
 */

#ifndef __TRANSPORTTLSCOMMON_H__
#define __TRANSPORTTLSCOMMON_H__

#include <string>

#include <openssl/bio.h>
#include <openssl/ssl.h>

#include "libepp_nicbr.H"

#include "StrUtil.H"

using std::string;

LIBEPP_NICBR_NS_BEGIN

/// TransportTLSCommon class
class TransportTLSCommon
{
public:
  /// Constructor
  TransportTLSCommon();

  /// Destructor
  ~TransportTLSCommon();

  /// Read payload from the open connection
  /**
     @param xml_payload buffer to be read
     @param timeout timeout in seconds
  */
  void read(string &xml_payload, const int &timeout = TIMEOUT);

  /// Write payload to the open connection
  /**
     @param xml_payload XML Payload
     @param timeout timeout in seconds
  */
  void write(const string &xml_payload, const int &timeout = TIMEOUT);

  /// Close the connection
  void disconnect();

  /// Return the OpenSSL error message
  /**
     @return OpenSSL error message
  */
  string get_openssl_msg() const;

  /// Checks if peer's certificate common name matches string common_name
  /**
     @param common_name expected common_name
     @return X509 error code
   */
  long cert_common_name_check(const string &common_name);

  /// Enables peer's certificate common name check
  void enable_cert_common_name_check();

  /// Disables peer's certificate common name check
  void disable_cert_common_name_check();

protected:
  /// Seed OpenSSL pseudo random number generator
  void seed_prng();

  /// Callback method used by OpenSSL to collect passphrases. 
  /**
     @param buf buffer that the passphrase should be copied into
     @param size size of buf in bytes, including the NULL terminating character
     @param rwflag indicates whether the callback is used for reading/decryption
                   (rwflag=0) or writing/decryption (rwflag=1)
     @param userdata application specific data
     @return the actual length of the password
   */
  static int pem_passwd_cb(char *buf, int size, int rwflag, void *userdata);

  /// Setup Context for Certificate Validation
  /**
     @param cert_file Certificate file
     @param root_ca_file Root certificate file
     @param pem_passphrase Optional passphrase for an encrypted private key
  */
  void setup_context(const string &cert_file,
		     const string &root_ca_file,
		     const string &pem_passphrase = "");

  /// Set the underlying I/O descriptor as non-blocking
  void set_non_blocking();

  /// BIO Socket
  BIO *_conn;

  /// SSL Context
  SSL_CTX *_ssl_ctx;

  /// SSL object
  SSL *_ssl;

  /// pem passphrase
  static string _pem_passphrase;

  /// Default read/write timeout in seconds
  static const int TIMEOUT;

  /// Flag for peer's certificate CN check
  bool _cert_common_name_check_enabled;
};

LIBEPP_NICBR_NS_END
#endif //__TRANSPORTTLSCOMMON_H__
