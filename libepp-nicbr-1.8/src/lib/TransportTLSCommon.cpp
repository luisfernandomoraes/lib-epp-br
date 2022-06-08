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
/* $Id: TransportTLSCommon.cpp 1040 2009-10-01 17:49:12Z fneves $ */

#include "config.h"

#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "TransportTLSCommon.H"
#include "TransportException.H"

LIBEPP_NICBR_NS_BEGIN

TransportTLSCommon::TransportTLSCommon() :
  _conn(0), _ssl_ctx(0), _ssl(0)
{
  _cert_common_name_check_enabled = false;

  // SSL_library_init
  // Return Value: Always 1 (nothing to be tested)
  SSL_library_init();

  // SSL_load_error_strings
  // Return Value: Void (nothing to be tested)
  SSL_load_error_strings();

  seed_prng();
}

TransportTLSCommon::~TransportTLSCommon()
{
  if (_ssl) {
    SSL_free(_ssl);
  }
  if (_ssl_ctx) {
    SSL_CTX_free(_ssl_ctx);
  }
}

void TransportTLSCommon::read(string &xml_payload, const int &timeout)
{
  struct timeval to;
  to.tv_sec = timeout;
  to.tv_usec = 0;
  int has_data;
  fd_set readfds, writefds;
  bool read_blocked = false;
  bool read_blocked_on_write = false;
  bool done = false;
  uint32_t to_read;

  const uint32_t BUF_SZ = 2049;
  const uint32_t TOTAL_LENGTH_SZ = 4;
  unsigned char buf[BUF_SZ];
  uint32_t total_length = 0;
  uint32_t total_read_bytes = 0;

  // checks connection
  if (_conn == NULL) {
    throw TransportException(TransportException::LOST_CONNECTION,
			     "Lost connection to peer",
			     get_openssl_msg());
  }

  int fd = SSL_get_fd(_ssl);
  while (!done) {
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_SET(fd, &readfds);
    if (read_blocked_on_write) {
      // Read the comments on SSL_ERROR_WANT_WRITE below
      FD_SET(fd, &writefds);
      has_data = select(fd+1, &readfds, &writefds, NULL, &to);
    } else {
      // Wait for data on the network read buffer
      has_data = select(fd+1, &readfds, NULL, NULL, &to);
    }
    if (has_data == 0) {
      if (total_length == 0 || (total_length > 0 && xml_payload == "")) {
	throw TransportException(TransportException::READ_TIMEOUT,
				 "Read Operation Timeout");
      } else {
	throw TransportException(TransportException::PAYLOAD_INCOMPLETE,
				 "Incomplete XML payload");
      }
    } else if (has_data < 0) {
      throw TransportException(TransportException::SSL_READ_ERR,
			       "Could not read the data unit");
    }

    if (FD_ISSET(fd, &readfds) ||
	(read_blocked_on_write && FD_ISSET(fd, &writefds))) {
      do {
	read_blocked_on_write = false;
	read_blocked = false;
	
	memset(buf, 0, BUF_SZ);
	if (total_length == 0) {
	  to_read = TOTAL_LENGTH_SZ;
	} else {
	  to_read = total_length - total_read_bytes;
	  if (to_read > (BUF_SZ - 1)) {
	    to_read = BUF_SZ-1;
	  }
	}

	int r = SSL_read(_ssl, buf, to_read);
	switch (SSL_get_error(_ssl, r)) {
	case SSL_ERROR_NONE:
	  if (total_length == 0) {
	    if (r < (int)TOTAL_LENGTH_SZ) { // Could not read total length
	      throw TransportException(TransportException::MISSING_TOTAL_LENGTH,
				       "Missing total length of the data unit");
	    }
	    memcpy(&total_length, buf, TOTAL_LENGTH_SZ);
	    total_length = ntohl(total_length);
	    if (total_length <= TOTAL_LENGTH_SZ) { // No payload to be read
	      throw TransportException(TransportException::INVALID_TOTAL_LENGTH,
				       "Invalid total length of the data unit");
	    }
	    total_read_bytes += TOTAL_LENGTH_SZ;
	    xml_payload = "";
	  } else {
	    total_read_bytes += r;
	    xml_payload += (char *)buf;
	  }
	  if (total_read_bytes == total_length) {
	    done = true;
	  }
	  break;
	case SSL_ERROR_WANT_READ:
	  // This happens when the SSL record arrives in two (or more)
	  // pieces. The first piece makes the select() call signal
	  // there is data in the network buffer, but the SSL buffer
	  // is still empty. Another select() call is needed.
	  read_blocked = true;
	  break;
	case SSL_ERROR_WANT_WRITE:
	  // This happens when we're trying to rehandshake and we
	  // block on a write during that rehandshake. Another
	  // select() call for waiting the socket to be writable is
	  // needed. As soon as it is, reinitiate the read.
	  read_blocked_on_write = true;
	  break;
	case SSL_ERROR_ZERO_RETURN:
	  // shutdown and exception 
	  SSL_shutdown(_ssl);
	default:
	  throw TransportException(TransportException::SSL_READ_ERR,
				   "Could not read the data unit",
				   get_openssl_msg());
	}
      } while (SSL_pending(_ssl) && !read_blocked && !read_blocked_on_write &&
	       !done);
    }
  }
}

void TransportTLSCommon::write(const string &xml_payload, const int &timeout) 
{ 
  struct timeval to;
  to.tv_sec = timeout;
  to.tv_usec = 0;
  int has_data;
  fd_set readfds, writefds;
  bool done = false;
  bool write_blocked_on_read = false;

  string utf8_xml_payload;
  StrUtil::iso88591_to_utf8(xml_payload, utf8_xml_payload);

  const uint32_t TOTAL_LENGTH_SZ = 4;
  const uint32_t PAYLOAD_SZ = (uint32_t) utf8_xml_payload.length();
  const uint32_t TOTAL_LENGTH = TOTAL_LENGTH_SZ + PAYLOAD_SZ;

  unsigned char *data_unit = new unsigned char[TOTAL_LENGTH + 1];

  memset(data_unit, 0, TOTAL_LENGTH + 1);

  // TOTAL LENGTH
  uint32_t wf_total_length = htonl(TOTAL_LENGTH);
  memcpy(data_unit, &wf_total_length, TOTAL_LENGTH_SZ);

  // EPP XML INSTANCE
  memcpy(data_unit+4, (char *) utf8_xml_payload.c_str(), PAYLOAD_SZ);

  // WRITE OPERATION

  // checks connection
  if (_conn == NULL) {
    delete[] data_unit;
    throw TransportException(TransportException::LOST_CONNECTION,
			     "Lost connection to peer",
			     get_openssl_msg());
  }

  int fd = SSL_get_fd(_ssl);
  while (!done) {
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_SET(fd, &writefds);
    if (write_blocked_on_read) {
      // Read the comments on SSL_ERROR_WANT_READ below
      FD_SET(fd, &readfds);
      has_data = select(fd+1, &readfds, &writefds, NULL, &to);
    } else {
      // Wait for some room on the network write buffer
      has_data = select(fd+1, NULL, &writefds, NULL, &to);
    }
    if (has_data == 0) {
      delete[] data_unit;
      throw TransportException(TransportException::WRITE_TIMEOUT,
			       "Write Operation Timeout");
    } else if (has_data < 0) {
      delete[] data_unit;
      throw TransportException(TransportException::SSL_WRITE_ERR,
                               "Could not write the data unit");
    }

    if (FD_ISSET(fd, &writefds) ||
	(write_blocked_on_read && FD_ISSET(fd, &readfds))) {
      write_blocked_on_read = false;

      int w = SSL_write(_ssl, data_unit, TOTAL_LENGTH);
      switch (SSL_get_error(_ssl, w)) {
      case SSL_ERROR_NONE:
	done = true;
	break;
      case SSL_ERROR_WANT_WRITE:
	// We've got unflushed data in the SSL buffer as there was no
	// more room on the network write buffer. As we're using
	// non-blocking I/O, we just need to wait on select() and try
	// SSL_write() again.
	break;
      case SSL_ERROR_WANT_READ:
	// This happens when we're trying to rehandshake and we
	// block on a read during that rehandshake. Another
	// select() call for waiting the socket to be readable is
	// needed. As soon as it is, reinitiate the write.
	write_blocked_on_read = true;
	break;
      case SSL_ERROR_ZERO_RETURN:
	// shutdown and exception 
	SSL_shutdown(_ssl);
      default:
	delete[] data_unit;
	throw TransportException(TransportException::SSL_WRITE_ERR,
				 "Could not write the data unit");
      }
    }
  }

  delete[] data_unit;
  BIO_flush(_conn);
}

void TransportTLSCommon::disconnect()
{
  int fd = -1;
  if (_ssl != NULL) {
    fd = SSL_get_fd(_ssl);
  }
  
  if (fd >= 0) {
    int r = SSL_shutdown(_ssl);
    // If we called SSL_shutdown() first then we always get return
    // value of '0'. In this case, try again, but first send a TCP FIN
    // to trigger the other side's close_notify
    if (r == 0) {
      shutdown(fd, 1);
      r = SSL_shutdown(_ssl);
    }
    SSL_free(_ssl);
    if (_ssl_ctx) {
      SSL_CTX_free(_ssl_ctx);
    }
    close(fd);
    _ssl = NULL;
    _ssl_ctx = NULL;
    _conn = NULL;
  }
}

string TransportTLSCommon::get_openssl_msg() const
{
  const int MSG_SIZE = 200;
  char error_msg[MSG_SIZE];
  unsigned long e = ERR_get_error();
  ERR_error_string(e, error_msg);
  string result =  error_msg;

  return result;
} 

long TransportTLSCommon::cert_common_name_check(const string &common_name)
{
  if (!_cert_common_name_check_enabled) {
    return X509_V_OK;
  }

  X509 *cert = NULL;
  X509_NAME *subject = NULL;
  
  cert = SSL_get_peer_certificate(_ssl);
  bool ok = false;
  if (cert != NULL && common_name != "") {
    char data[256];
    if ((subject = X509_get_subject_name(cert)) != NULL &&
	X509_NAME_get_text_by_NID(subject, NID_commonName, data, 256) > 0) {
      data[255] = 0;
      if (strncasecmp(data, common_name.c_str(), 255) == 0) {
	ok = true;
      }
    }
  }
  
  if (cert) {
    X509_free(cert);
  }

  if (ok) {
    return SSL_get_verify_result(_ssl);
  }
  
  return X509_V_ERR_APPLICATION_VERIFICATION;
}

void TransportTLSCommon::enable_cert_common_name_check()
{
  _cert_common_name_check_enabled = true;
}

void TransportTLSCommon::disable_cert_common_name_check()
{
  _cert_common_name_check_enabled = false;
}

void TransportTLSCommon::seed_prng()
{
  // "What happens is that when too much randomness is drawn from the
  // operating system's randomness pool then randomness can
  // temporarily be unavailable./dev/random solves this problem by
  // waiting until enough randomness can be gathered - and this can
  // take a long time since blocking reduces activity in the machine
  // and less activity provides less random events: a vicious
  // circle. /dev/urandom solves this dilemma more pragmatically by
  // simply returning predictable ``random'' numbers."
  //
  // lists.fifi.org/cgi-bin/man2html/usr/share/man/man3/Net::SSLeay.3pm.gz

#ifdef HAVE_SRANDOMDEV
  RAND_load_file("/dev/random", 2048);
#else
  RAND_load_file("/dev/urandom", 2048);
#endif //HAVE_SRANDOMDEV
}

int TransportTLSCommon::pem_passwd_cb(char *buf, int size, int rwflag,
				      void *userdata)
{
  strncpy(buf, (char*)_pem_passphrase.c_str(), size);
  buf[size-1] = '\0';
  return strlen(buf);
}

void TransportTLSCommon::setup_context(const string &cert_file,
				       const string &root_ca_file,
				       const string &pem_passphrase)
{
  if (SSL_CTX_load_verify_locations(_ssl_ctx,
				    root_ca_file.c_str(), NULL) != 1) {
    throw TransportException(TransportException::LOAD_ROOTCA_FILE_ERR,
			     "Error loading CA Certificate file",
			     get_openssl_msg());
  }

  if (SSL_CTX_use_certificate_chain_file(_ssl_ctx,
					 cert_file.c_str()) != 1) {
    throw TransportException(TransportException::LOAD_CERT_FILE_ERR,
			     "Error loading certificate from file",
			     get_openssl_msg());
  }

  if (pem_passphrase != "") {
    _pem_passphrase = pem_passphrase;
    SSL_CTX_set_default_passwd_cb(_ssl_ctx, pem_passwd_cb);
  }

  if (SSL_CTX_use_PrivateKey_file(_ssl_ctx, cert_file.c_str(),
				  SSL_FILETYPE_PEM) != 1) {
    throw TransportException(TransportException::LOAD_KEY_FILE_ERR,
			     "Error loading private key from file",
			     get_openssl_msg());
  }

  SSL_CTX_set_verify(_ssl_ctx, SSL_VERIFY_PEER|
		     SSL_VERIFY_FAIL_IF_NO_PEER_CERT|SSL_VERIFY_CLIENT_ONCE,
		     NULL);
  SSL_CTX_set_verify_depth(_ssl_ctx, 3);
}

void TransportTLSCommon::set_non_blocking()
{
  if (_ssl == NULL) {
    return;
  }

  int fd = SSL_get_fd(_ssl);
  int flags;
  if ((flags = fcntl(fd, F_GETFL, 0)) < 0) {
    throw TransportException(TransportException::GET_FNCTL_ERR,
			     "Unable to get descriptor status flags");
  }
  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
    throw TransportException(TransportException::SET_NONBLOCKING_ERR,
			     "Unable to set SSL underlying I/O descriptor "
			     "as non-blocking");
  }
}

string TransportTLSCommon::_pem_passphrase = "";

const int TransportTLSCommon::TIMEOUT = 60;

LIBEPP_NICBR_NS_END
