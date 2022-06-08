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
/* $Id: TransportTLS.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file TransportTLS.H
 *  @brief TransportTLS class
 */

#ifndef __TRANSPORTTLS_H__
#define __TRANSPORTTLS_H__

#include "TransportTLSCommon.H"

LIBEPP_NICBR_NS_BEGIN

/// TransportTLS class
class TransportTLS : public TransportTLSCommon
{
public:
  /// Default constructor
  TransportTLS(const string &server = "localhost", const int &port = 700);

  /// Return the server address
  /**
     @return server name/address 
  */
  string get_server() const;

  /// Return the server port number
  /**
     @return server port number
  */
  int get_port() const;

  /// Set the server name/address
  /**
     @param server name/address 
  */
  void set_server(const string &server);

  /// Set connection port
  /**
     @param port server port number
  */
  void set_port(const int port);

  /// Open the Connection
  /**
     @param client_cert_file Client certificate file
     @param root_ca_file Root certificate file
     @param pem_passphrase Optional passphrase for an encrypted private key
  */
  void connect(const string &client_cert_file, const string &root_ca_file,
	       const string &pem_passphrase = "");

private:
  /// Server address
  string _server;

  /// Connection port
  int _port;
};

LIBEPP_NICBR_NS_END
#endif //__TRANSPORTTLS_H__
