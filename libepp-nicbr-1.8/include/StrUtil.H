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
/* $Id: StrUtil.H 1033 2009-09-28 18:59:07Z fneves $ */
/** @file StrUtil.H
 *  @brief String Manipulation Utilities
 */

#ifndef __STRUTIL_H__
#define __STRUTIL_H__

#include <string>
#include <cstdio>
#include <map>

#include "libepp_nicbr.H"

#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xercesc/framework/XMLFormatter.hpp>

using std::string;
using std::map;
using std::less;

LIBEPP_NICBR_NS_BEGIN

/// StrUtil Class: String Manipulation Utilities
class StrUtil
{
public:
  /// Used for parsing XML Templates 
  /**
     @param text      XML template to be parsed
     @param to_parse  Mapping tags into values
     @param tag_begin Symbol for begin tag
     @param tag_end   Symbol for end tag
     @return          Parsed string
  */
  static string parse(const string &text,
		      const map < string, string, less<string> > &to_parse,
		      string tag_begin, string tag_end);

  /// Used for substitution of pat for drp within buffer
  /**
     @param buffer Text to be scanned   
     @param pat    Pattern to be substituted
     @param drp    Substitute for the pattern indicated by pat
     @return       Number of matches       
  */
  static int gsub(string &buffer, const char *pat, const char *drp);

  /// Convert number to string where the format string looks like printf format.
  /** 
      @param format the format string in the printf format
      @param number the number to be converted
      @return the string containing the number
  */
  template<class kind>
  static inline string to_string(const char* format, const kind &number)
  {
    // Max size of numeric buffer
    const int _NUMBER_SIZE = 128;
    char *p;

    p = new char[_NUMBER_SIZE];
    snprintf(p, (_NUMBER_SIZE - 1), format, number);
    string s(p);

    delete [] p;
    return s;
  }

  /**
     Encodes an ISO-8859-1 string to UTF-8. ISO-8859-1 non-printable
     characters are substituted by spaces (ASCII 0x20).
     @param iso88591 ISO-8859-1 string to be encoded to UTF-8
     @param utf8 UTF-8 encodeded string
     @return number of characters substituted by spaces
   */
  static int iso88591_to_utf8(const string &iso88591, string &utf8);

  /**
     Decodes a UTF-8 string to ISO-8859-1. ISO-8859-1 non-printable
     characters and not ISO-8859-1 characters are substituted by
     spaces (ASCII 0x20).
     @param utf8 UTF-8 encoded string to be decoded
     @param iso88591 decoded ISO-8859-1 string
     @return number of characters substituted by spaces
   */
  static int utf8_to_iso88591(const string &utf8, string &iso88591);

  /// XML Beautifier
  /**
     @param input_txt The text to be formatted (in UTF-8)
     @return The indented XML document 
   */
  string xml_beautifier(const string &input_txt);

  /// Escape &'><" characters 
  /**
     @param input_txt The text that will possibly have some characters
     escaped
     @return strings with &'><" characters escaped
   */
  static string esc_xml_markup(const string &input_txt);
};

LIBEPP_NICBR_NS_END
#endif //__STRUTIL_H__
