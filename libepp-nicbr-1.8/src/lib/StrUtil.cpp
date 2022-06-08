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
/* $Id: StrUtil.cpp 926 2008-06-30 15:26:23Z eduardo $ */

#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/util/XMLString.hpp>

#include "config.h"

#include "StrUtil.H"
#include "BeautifierHandlers.H"
#include "StringEncodingException.H"
#include "XmlException.H"
#include "DomParserCommon.H"

XERCES_CPP_NAMESPACE_USE

LIBEPP_NICBR_NS_BEGIN

string StrUtil::parse(const string &text,
		      const map < string, string, less<string> > &to_parse, 
		      string tag_begin, string tag_end)
{
  string result = text;
  string buffer;
  map<string, string, less<string> >::const_iterator it;

  for (it = to_parse.begin(); it != to_parse.end(); it++) {
    buffer = tag_begin + (*it).first + tag_end;
    gsub(result, buffer.c_str(), (*it).second.c_str());
  }

  return result;
}

int StrUtil::gsub(string &buffer, const char *pat, const char *drp)
{
  int i = 0;
  int count = 0;

  do {
    i = buffer.find(pat,i);
    if (i == -1) {
      break;
    }

    count++;
    buffer.replace(i,strlen(pat),drp);
    i += strlen(drp);
  } while (i < (int)buffer.length());

  return count;
}

int StrUtil::iso88591_to_utf8(const string &iso88591, string &utf8)
{
  const char *s = iso88591.c_str();
  utf8 = "";
  int np_count = 0;
  unsigned char c;
  for (unsigned int i = 0; i < iso88591.length(); i++) {
    c = s[i];
    if ((c & 0x80) == 0x80) {
      if (c > 0x7F && c < 0xA0) {
	// non-printable characters
	utf8 += " ";
	np_count++;
      } else {
	utf8 += (0xC0 | (c >> 6));
	utf8 += (0x80 | (0x3F & c));
      }
    } else {
      // ASCII
      if (c < 0x20 || c == 0x7F) {
	// non-printable characters
	utf8 += " ";
	np_count++;
      } else {
	utf8 += c;
      }
    }
  }

 return np_count;
}

int StrUtil::utf8_to_iso88591(const string &utf8, string &iso88591)
{
  const char *s = utf8.c_str();
  iso88591 = "";
  int np_niso_count = 0;
  unsigned char c1, c2;
  for (unsigned int i = 0; i < utf8.length(); i++) {
    c1 = s[i];
    if ((c1 & 0x80) == 0) {
      // ASCII
      if (c1 < 0x20 || c1 == 0x7F) {
	// non-printable characters
	iso88591 += " ";
	np_niso_count++;
      } else {
	iso88591 += c1;
      }
    } else if ((c1 & 0xFE) == 0xC2) { // 1100 001x
      // Possibly valid ISO-8859-1
      i++;
      if (i >= utf8.length()) {
	// Invalid utf8 encoding: missing char
	throw StringEncodingException(StringEncodingException::MISSING_CHAR,
				      "Invalid UTF-8 encoding: reached end of "
				      "string while expecting byte #2 of a "
				      "2-byte sequence.");
      }
      if ((s[i] & 0xC0) != 0x80) {
	// Invalid utf8 encoding: second byte must be 10xx xxxx
	throw StringEncodingException(
				     StringEncodingException::ENCODING_SEQUENCE,
				     "Invalid UTF-8 encoding: invalid byte #2 "
				     "of a 2-byte sequence.");
      }
      c2 = (0x3F & s[i]);
      c2 |= ((c1 & 0x03) << 6);
      if (c2 > 0x7F && c2 < 0xA0) {
	// non-printable characters
	iso88591 += " ";
	np_niso_count++;
      } else {
	iso88591 += c2;
      }
    } else if ((c1 & 0xFE) == 0xC0) { // 1100 000x
      // Invalid utf8 encoding: Trying to encode ASCII char
      throw StringEncodingException(StringEncodingException::ASCII_ENCODING,
				    "Invalid UTF-8 encoding: trying to encode "
				    "an ASCII character.");
    } else {
      // Not ISO-8859-1. Skip chars if necessary and possible.
      iso88591 += " ";
      np_niso_count++;
      int bytes_to_skip = 0;
      if ((c1 & 0xE0) == 0xC0) { // 110x xxxx
	bytes_to_skip = 1;
      } else if ((c1 & 0xF0) == 0xE0) { // 1110 xxxx
	bytes_to_skip = 2;
      } else if ((c1 & 0xF8) == 0xF0) { // 1111 0xxx
	bytes_to_skip = 3;
      } else {
	throw StringEncodingException(StringEncodingException::FIRST_OCTET,
				      "Invalid UTF-8 encoding: invalid first "
				      "octet.");
      }
      for (int j = 0; j < bytes_to_skip; j++) {
	i++;
	if (i >= utf8.length()) {
	  // Invalid utf8 encoding: missing char
	  throw StringEncodingException(StringEncodingException::MISSING_CHAR,
					"Invalid UTF-8 encoding: reached end "
					"of string while expecting byte #" +
					to_string("%d", j + 2) + " of a " +
					to_string("%d", bytes_to_skip + 1) +
					"-byte sequence.");
	}
	c2 = s[i];
	if ((c2 & 0xC0) != 0x80) {
	  // Invalid utf8 encoding: subsequent bytes must be 10xx xxxx
	  throw StringEncodingException(
				     StringEncodingException::ENCODING_SEQUENCE,
				     "Invalid UTF-8 encoding: invalid byte #" +
				     to_string("%d", j + 2) + " of a " +
				     to_string("%d", bytes_to_skip + 1) +
				     "-byte sequence." + to_string("%d", i) +
				     " " + to_string("%hhX", s[i-1]) +
				     " " + to_string("%hhX", s[i]));
	}
      }
    }
  }

  return np_niso_count;
}

string StrUtil::xml_beautifier(const string &input_txt)
{
  // User Data Structure
  UserData user_data;
  user_data.input_txt = input_txt;

  try {
    XMLPlatformUtils::Initialize();
  } catch (const XMLException& to_catch) {
    // throws XmlException
    string message = DomParserCommon::str_transcode(to_catch.getMessage());
    throw XmlException(XmlException::PARSER_INITIALIZE_EXCEPTION,
                      "Could not initialize the XML parser", message);
  }

  try {
    // Beautifier Handler
    BeautifierHandlers handler(&user_data);

    //  Create a SAX parser object. 
    SAXParser parser;
    parser.setDocumentHandler(&handler);
    parser.setErrorHandler(&handler);

    // Transcode the std::string to MemBufInputSource
    char *sysId = (char *)"";
    MemBufInputSource tmp_membuf((const XMLByte*)input_txt.c_str(),
				 input_txt.length(), sysId);

    // Parse
    parser.parse(tmp_membuf);
  } catch (const XMLException& toCatch) {
    throw;
  } 

  XMLPlatformUtils::Terminate();
  return user_data.output_txt;
}

string StrUtil::esc_xml_markup(const string &input_txt)
{
  string escaped = "";
  try {
    XMLPlatformUtils::Initialize();
  } catch (const XMLException& to_catch) {
    // throws XmlException
    string message = DomParserCommon::str_transcode(to_catch.getMessage());
    throw XmlException(XmlException::PARSER_INITIALIZE_EXCEPTION,
                      "Could not initialize the XML parser", message);
  }

  try {
    MemBufFormatTarget mbft;
    string encoding = "ISO-8859-1";
    XMLFormatter formatter(encoding.c_str(), &mbft, XMLFormatter::StdEscapes,
			   XMLFormatter::UnRep_Replace);
    XMLCh *ret = XMLString::transcode(input_txt.c_str());
    formatter.formatBuf(ret, XMLString::stringLen(ret),
			XMLFormatter::StdEscapes,
			XMLFormatter::UnRep_Replace);
    if (mbft.getLen() > 0) {
      escaped = (char*)mbft.getRawBuffer();
    }
    XMLString::release(&ret);
  } catch (const XMLException &toCatch) {
    throw;
  }

  XMLPlatformUtils::Terminate();
  return escaped;
}

LIBEPP_NICBR_NS_END
