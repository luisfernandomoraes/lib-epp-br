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
/* $Id: TransportExceptionTestServer.cpp 766 2006-06-19 21:31:03Z koji $ */
/**
   @file  TransportExceptionTestServer.cpp
   @brief Small server for Transport Exception testing's sake
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
#include <sys/types.h>

#define   PORT_FILE    "TransportExceptionTestServer.port"

const int BUF_SIZE = 15; /* bytes */
const int TIMEOUT  = 60; /* seconds */

static void sig_handler(int signo);

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
  if (SSL_CTX_load_verify_locations(ssl_ctx, "root.pem", NULL) != 1) {
    fprintf(stderr, "Error loading CA Certificate file.\n");
    exit(1);
  }
  if (SSL_CTX_use_certificate_chain_file(ssl_ctx, "server.pem") != 1) {
    fprintf(stderr, "Error loading certificate from file.\n");
    exit(1);
  }
  if (SSL_CTX_use_PrivateKey_file(ssl_ctx, "server.pem",
				  SSL_FILETYPE_PEM) != 1) {
    fprintf(stderr, "Error loading private key from file.\n");
    exit(1);
  }
  SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_PEER|
		     SSL_VERIFY_FAIL_IF_NO_PEER_CERT|SSL_VERIFY_CLIENT_ONCE,
		     NULL);
  SSL_CTX_set_verify_depth(ssl_ctx, 3);

  /* finds an available port >= 1024 */
  BIO *acc = 0;
  char port[6];
  int i;
  for (i = 2000; i <= 65535; ++i) {
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

  while (1) {
    /* timeout */
    alarm(TIMEOUT);

    /* awaits connection */
    if (BIO_do_accept(acc) <= 0) {
      /* error accepting connection */
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

    /* timeout off */
    alarm(0);

    /* reads something from client */
    int err;
    char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);
    err = SSL_read(ssl, buf, sizeof(buf));

    if (err == 5) {
      int client_num = atoi(buf + 4);
      if (client_num == 1)  { // MISSING_TOTAL_LENGTH
	err = SSL_write(ssl, buf + 4, 1);	
      } else if (client_num == 2) { // INVALID_TOTAL_LENGTH
	uint32_t total_length = 3;
	total_length = htonl(total_length);
	err = SSL_write(ssl, &total_length, 4);
      } else if (client_num == 3) { // PAYLOAD INCOMPLETE
	memset(buf, 1, BUF_SIZE);
	uint32_t total_length = BUF_SIZE + 5;
	total_length = htonl(total_length);
	memcpy(buf, &total_length, 4);
	err = SSL_write(ssl, buf, BUF_SIZE);
	sleep(10);
      }
    }

    if (err <= 0) {
      /* error writing to socket */
      unlink(PORT_FILE);
      exit(1);
    }

  }
  
  /* clean up */
  SSL_free(ssl);
  SSL_CTX_free(ssl_ctx);
  BIO_free(acc);
  unlink(PORT_FILE);

  return 0;
}

static void sig_handler(int signo)
{
  unlink(PORT_FILE);
  exit(1);
}
