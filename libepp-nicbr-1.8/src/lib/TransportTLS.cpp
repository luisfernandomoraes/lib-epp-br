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
/* $Id: TransportTLS.cpp 1040 2009-10-01 17:49:12Z fneves $ */

#include "config.h"

#include "TransportTLS.H"
#include "TransportException.H"

LIBEPP_NICBR_NS_BEGIN

#define CONNECT_TIMEOUT 5

TransportTLS::TransportTLS(const string &server, const int &port) :
  _server(server), _port(port) {}

string TransportTLS::get_server() const
{
  return _server;
}

int TransportTLS::get_port() const
{
  return _port;
}

void TransportTLS::set_server(const string &server)
{
  _server = server;
}

void TransportTLS::set_port(const int port)
{
  _port = port;
}

void TransportTLS::connect(const string &client_cert_file,
			   const string &root_ca_file,
			   const string &pem_passphrase)
{
  _ssl_ctx = SSL_CTX_new(TLSv1_client_method());

  setup_context(client_cert_file, root_ca_file, pem_passphrase);

  // BIO_new_connect
  // Return value: creates a new connect BIO
  string connect_info = _server + ":" + StrUtil::to_string("%d", _port);
  _conn = BIO_new_connect((char*) connect_info.c_str());

  // BIO_do_connect
  // Return value: 1 or x
  //               1      (if the connection was established successfully).
  //               x <= 0 (if the connection could not be established)
  if (BIO_do_connect(_conn) != 1) {
    throw TransportException(TransportException::BIO_DO_CONNECT_ERR,
			     "Connection could not be established", 
			     get_openssl_msg());
  }

  // Create a new SSL structure for a connection
  if (!(_ssl = SSL_new(_ssl_ctx))) {
    throw TransportException(TransportException::SSL_NEW_ERR,
			     "Error creating an SSL context",
			     get_openssl_msg());
  }
 
 // Connect the SSL object with BIO and initiate the TLS handshake
  // with the TLS server
  SSL_set_bio(_ssl, _conn, _conn);

  set_non_blocking();

  int connectStatus = SSL_connect(_ssl);
  while (connectStatus == -1) {
    int connectError = SSL_get_error(_ssl, connectStatus);
    struct timeval timeout;
    timeout.tv_sec = CONNECT_TIMEOUT;
    timeout.tv_usec = 0;

    int fd = SSL_get_fd(_ssl);
    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(fd, &fdset);
    int hasData = 0;
    if (connectError == SSL_ERROR_WANT_READ) {
      hasData = select(fd + 1, &fdset, NULL, NULL, &timeout);
      
    } else if (connectError == SSL_ERROR_WANT_WRITE) {
      hasData = select(fd + 1, NULL, &fdset, NULL, &timeout);
    }
    
    if (hasData <= 0) {
      throw TransportException(TransportException::SSL_CONNECT_ERR,
                               "Error connecting SSL object",
                               get_openssl_msg());
    }
    connectStatus = SSL_connect(_ssl);
  }

  if (connectStatus <= 0) {
    throw TransportException(TransportException::SSL_CONNECT_ERR,
			     "Error connecting SSL object",
			     get_openssl_msg());
  }

  long x509_err = cert_common_name_check(_server);
  if (x509_err != X509_V_OK) {
    disconnect();
    throw TransportException(TransportException::PEER_CERTIFICATE_CN_ERR,
			     "Peer's certificate common name mismatch",
			     X509_verify_cert_error_string(x509_err));
  }
}

LIBEPP_NICBR_NS_END
