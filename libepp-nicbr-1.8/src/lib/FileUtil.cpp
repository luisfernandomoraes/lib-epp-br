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
/* $Id: */

#include "config.h"

#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>

#include "FileUtil.H"
#include "StrUtil.H"
#include "IoException.H"

LIBEPP_NICBR_NS_BEGIN

string FileUtil::read_file(const string &filename)
{
  const int buf_sz = 16383;
  FILE* input_file;
  int read_bytes = 0;
  int flag = 0;
  string file_content = "";
  char buffer[buf_sz+1];

  struct stat buf;
  if (stat(filename.c_str(), &buf) < 0 || !S_ISREG(buf.st_mode))
    throw IoException(IoException::OPEN_ERR, 
		      StrUtil::to_string("Error opening file: [%s]",  
					 filename.c_str()));
		      
  input_file = fopen(filename.c_str(), "r");
  if (input_file == NULL)
    throw IoException(IoException::OPEN_ERR, 
		      StrUtil::to_string("Error opening file: [%s]",
					 filename.c_str()));
  
  while (1) {
    read_bytes = fread(buffer,1,buf_sz,input_file);
    flag = ferror(input_file);
    if (flag)
      throw IoException(IoException::READ_ERR, 
			StrUtil::to_string("Error reading file: [%s]", 
					   filename.c_str()));

    if (!read_bytes)
      break;
    buffer[read_bytes] = '\0';
    file_content += buffer;
    if (read_bytes != buf_sz)
      break;
  }

  flag = fclose(input_file);
  if (flag)
    throw IoException(IoException::CLOSE_ERR, 
		      StrUtil::to_string("Error closing file: [%s]",
					 filename.c_str()));

  return file_content;
}

LIBEPP_NICBR_NS_END
