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
/* $Id: SessionTestServer.cpp 596 2006-03-16 13:57:09Z eduardo $ */
/**
   @file  SessionTestServer.c
   @brief Simple server to test Session class
 */

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>

#include "libepp_nicbr.H"
#include "StrUtil.H"
#include <string>

using std::string;

LIBEPP_NICBR_NS_USE

#define   PORT_FILE    "SessionTestServer.port"
#define   DATA_FILE    "SessionTestServer.data"

const int READ_LIMIT = 2048; /* bytes */
const int TIMEOUT    = 60;   /* seconds */

static void sig_handler(int signo);
int readln(FILE *stream, unsigned char *buf, int size);

int main(int argc, char **argv)
{
  signal(SIGINT,  sig_handler);
  signal(SIGTERM, sig_handler);
  signal(SIGALRM, sig_handler);

  pid_t pid;
  if ((pid = fork()) < 0) {
    /* fork error */
    fprintf(stderr, "fork error.\n");
    exit(1);
  } else if (pid != 0) {
    /* parent quits */
    exit(0);
  }

  /* child continues */

  SSL_library_init();

  SSL *ssl;
  SSL_CTX *ssl_ctx;

  ssl_ctx = SSL_CTX_new(TLSv1_server_method());
  if (SSL_CTX_use_certificate_chain_file(ssl_ctx, "server.pem") != 1) {
    fprintf(stderr, "Error loading certificate from file.\n");
    exit(1);
  }
  if (SSL_CTX_use_PrivateKey_file(ssl_ctx, "server.pem",
				  SSL_FILETYPE_PEM) != 1) {
    fprintf(stderr, "Error loading private key from file.\n");
    exit(1);
  }

  /* finds an available port >= 1024 */
  BIO *acc = 0;

  char port[6];
  int i;
  for (i = 1024; i <= 65535; ++i) {
    BIO_free(acc);

    sprintf(port, "%d", i);

    if ((acc = BIO_new_accept(port)) <= 0) {
      /* error creating server socket */
      if (i == 65535) {
	exit(1);
      }
      continue;
    }
  
    if (BIO_do_accept(acc) <= 0) {
      /* error binding server socket */
      if (i == 65535) {
	exit(1);
      }
      continue;
    }

    break;
  }

  /* write port number to tmp file */
  FILE *port_fd = fopen(PORT_FILE, "w");
  if (!port_fd) {
    exit(1);
  }

  int written = fprintf(port_fd, "%s", port);
  if (written < 0 || (size_t) written < strlen(port)) {
    unlink(PORT_FILE);
    exit(1);
  }
  fclose(port_fd);

  /* timeout */
  alarm(TIMEOUT);

  /* awaits connection */
  if (BIO_do_accept(acc) <= 0) {
    /* error accepting connection */
    unlink(PORT_FILE);
    exit(1);
  }

  int err;
  char send_buf[READ_LIMIT];
  memset(send_buf, 0, READ_LIMIT);
  unsigned char recv_buf[READ_LIMIT];
  memset(recv_buf, 0, READ_LIMIT);
  FILE *data_fd = fopen(DATA_FILE, "r");
  if (!data_fd) {
    /* error opening data file */
    unlink(PORT_FILE);
    exit(1);
  }

  BIO *client = 0;
  client = BIO_pop(acc);

  if (!(ssl = SSL_new(ssl_ctx))) {
    /* error creating an SSL context */
    unlink(PORT_FILE);
    exit(1);
  }
  SSL_set_bio(ssl, client, client);

  if (SSL_accept(ssl) <= 0) {
    /* error accepting SSL connection */
    unlink(PORT_FILE);
    exit(1);
  }

  const uint32_t TOTAL_LENGTH_SZ = 4;
  unsigned char send_buffer[2048];
  string utf8_buffer;
  while (readln(data_fd, (unsigned char *) send_buf, READ_LIMIT)) {
    /* writes next line from data file to client */

    /* data to be written must be encapsulated into a data unit */
    StrUtil::iso88591_to_utf8(send_buf, utf8_buffer);

    bzero(send_buffer, 2048);   
    uint32_t PAYLOAD_SZ = utf8_buffer.length();
    uint32_t TOTAL_LENGTH = TOTAL_LENGTH_SZ + PAYLOAD_SZ;
    uint32_t WF_TOTAL_LENGTH = htonl(TOTAL_LENGTH);

    memcpy(send_buffer, &WF_TOTAL_LENGTH, TOTAL_LENGTH_SZ);
    memcpy(send_buffer + TOTAL_LENGTH_SZ, utf8_buffer.c_str(), PAYLOAD_SZ);

    /* write loop */
    for (uint32_t nwritten = 0; nwritten < TOTAL_LENGTH; nwritten += err) {
      err = SSL_write(ssl, send_buffer + nwritten,
		      TOTAL_LENGTH - nwritten);
      if (err <= 0) {
	/* error writing to socket */
	unlink(PORT_FILE);
	exit(1);
      }
    }

    /* read loop: blocked waiting for the client input */

    /* read total length */
    int read_bytes = SSL_read(ssl, recv_buf, sizeof(recv_buf)); 
    const uint32_t TOTAL_LENGTH_SZ = 4;
    uint32_t total_length;
    memcpy(&total_length, recv_buf, TOTAL_LENGTH_SZ);
    total_length = ntohl(total_length);

    /* "read" the XML payload: 
       not really, just ignore the received buffer!! */
    memset(recv_buf, 0, READ_LIMIT);
    uint32_t total_bytes = read_bytes;
    int buf_sz = 0;
    while (total_bytes < total_length) {
      buf_sz = total_length - total_bytes;
      if (buf_sz > READ_LIMIT) {
	buf_sz = READ_LIMIT;
      } 
      memset(recv_buf, 0, READ_LIMIT);
      if ((read_bytes = SSL_read(ssl, recv_buf, buf_sz)) <= 0)
	break;
      total_bytes += read_bytes;
    }

    BIO_flush(client);
    bzero(recv_buf, READ_LIMIT);
    bzero(send_buf, READ_LIMIT);
  }
  
  /* clean up */
  SSL_free(ssl);
  SSL_CTX_free(ssl_ctx);
  BIO_free(acc);
  fclose(data_fd);
  unlink(PORT_FILE);

  return 0;
}

/// signal handler */
static void sig_handler(int signo)
{
  unlink(PORT_FILE);
  exit(1);
}

/// read a line from a file and stores it in a buffer
/**
   @return bytes read
   @param  descriptor of input file
   @param  destination buffer
   @param  buffer size
 */
int readln(FILE *stream, unsigned char *buf, int size)
{
  char c;
  int count = 0;

  fread(&c, 1, 1, stream);
  while (c != '\n' && count < size - 1) {
    buf[count] = c;
    count++;
    fread(&c, 1, 1, stream);
  }
  buf[count] = '\0';

  return count;
}
