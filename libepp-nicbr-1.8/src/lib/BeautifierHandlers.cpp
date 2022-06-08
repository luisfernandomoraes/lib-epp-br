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
/* $Id: BeautifierHandlers.cpp 1041 2009-10-16 19:05:48Z koji $ */
/**
   Greedy algorithm: 
   
                 ===================================================
                 | CURRENT EVENT                                   |            
====================================================================
|  FORMER EVENT  | start       | character   |  end                |
====================================================================
|    start       | print_start | print_start | print_start + "/>"  |
--------------------------------------------------------------------
|    character   | print_char  |  bufferize  | print_char +        |
|                |             |(& in chars) | "<" + name + "/>"   |
--------------------------------------------------------------------
|    end         |     X       |     X       | "<" + name + "/>"   |
--------------------------------------------------------------------     
*/

#include <cctype>
#include <iostream>

#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/SAXException.hpp>

#include "config.h"

#include "BeautifierHandlers.H"
#include "XmlException.H"

XERCES_CPP_NAMESPACE_USE

LIBEPP_NICBR_NS_BEGIN

#define MAX_LINE_LENGTH 80

BeautifierHandlers::BeautifierHandlers(UserData *user_data) 
{
  _depth = 0;
  _user_data = user_data;
  _last_event = NONE;
  _last_non_chars_event = NONE;
  _truncated_txt = false;
}

BeautifierHandlers::~BeautifierHandlers() {}


void BeautifierHandlers::startElement(const XMLCh* const name, 
				      AttributeList&  attributes)
{
  if (_last_event == START) {
    string output_txt = _to_be_printed + ">";    
    _user_data->output_txt += truncate(output_txt);
  } else if (_last_event == CHARACTERS) {
    chars_handler();
  } 

  if (_depth != 0) {
    _to_be_printed = "\n";
  }

  for (int i = 0; i < _depth; i++) {
    _to_be_printed += " ";
  }
  _tag_margin = _to_be_printed;
  _to_be_printed += "<" + str_transcode(name);
  _element_name = str_transcode(name);

  if (attributes.getLength() != 0) {
    //    The method length() takes \n into account
    if (_depth == 0) { // No leading \n
      _left_margin = _to_be_printed.length() + 1;
    } else { // Leading \n
      _left_margin = _to_be_printed.length();
    }

    for (unsigned int i = 0; i < attributes.getLength(); i++) {
      _to_be_printed += " " + str_transcode(attributes.getName(i)) +
	"=\"" + str_transcode(attributes.getValue(i)) + "\"";
    }
  }

  _last_event = _last_non_chars_event = START;
  _depth++;  
}

void BeautifierHandlers::characters(const XMLCh* const chars, 
#if XERCES_VERSION_MAJOR == 2
				    const unsigned int length
#else
                                    const XMLSize_t length
#endif
				    )
{
  string lstr = alltrim(str_transcode(chars));

  // skip full space strings
  if (lstr.length() > 0) {
    _buffered_chars += str_transcode(chars);
    _last_event = CHARACTERS;
  }
}

void BeautifierHandlers::endElement(const XMLCh* const name) 
{
  _depth--;

  string whitespace;
  if (_last_event == END) {
    whitespace = "\n";
    for (int i = 0; i < _depth; i++) {
      whitespace += " ";
    }
  }

  string output_txt;
  switch (_last_event) {
  case START:
    output_txt = _to_be_printed + "/>";
    break;
  case CHARACTERS: {
    // Check the maximum line length    
    chars_handler();
    string end_tag = "</" + str_transcode(name) + ">";
    if (_truncated_txt == true) { // Truncated Text 
      _left_margin = _depth + 1;
      output_txt = truncate(_to_be_printed) + _tag_margin + end_tag;
    } else { // Text fit into the maximum line length
      output_txt = _to_be_printed + end_tag;
    }  
    _truncated_txt = false;
  } break;
  case END:
    output_txt = whitespace + "</" + str_transcode(name) + ">";
    break;
  default:
    break;
  }

  _user_data->output_txt += truncate(output_txt); 
  _last_event = _last_non_chars_event = END;
}

void BeautifierHandlers::error(const SAXParseException& e)
{
  throw XmlException(XmlException::INVALID_XML_DOCUMENT, 
		     "The XML document is invalid", 
		     str_transcode(e.getMessage()));
}

void BeautifierHandlers::fatalError(const SAXParseException& e)
{
  throw XmlException(XmlException::INVALID_XML_DOCUMENT, 
		     "The XML document is invalid", 
		     str_transcode(e.getMessage()));
}

void BeautifierHandlers::warning(const SAXParseException& e)
{
  throw XmlException(XmlException::INVALID_XML_DOCUMENT, 
		     "The XML document is invalid", 
		     str_transcode(e.getMessage()));
}

string BeautifierHandlers::str_transcode(const XMLCh *const to_transcode)
{
  char *str = XMLString::transcode(to_transcode);
  string ret_str;
  if (str == 0) {
    ret_str = "";
  } else {
    ret_str = str;
  }
  XMLString::release(&str);
  return ret_str;
}

string BeautifierHandlers::truncate(const string &line)
{
  if (line.length() <= MAX_LINE_LENGTH) {
    return line;
  }

  string trimmed_line = alltrim(line);
  vector<string> words;
  int word_count = split(trimmed_line, words);

  if (word_count == 1) {
    return trimmed_line;
  }

  // Margin to the words. Except the first one.
  string local_margin = "\n";
  for (int i = 0; i < _left_margin; i++) {
    local_margin += " ";
  }

  // First word has a different margin.
  string first_margin;
  if (words[0][0] == '<') {    
    first_margin = _tag_margin;
  } else {
    first_margin = local_margin;
  }
  words[0] = first_margin + words[0];

  string local_line;
  string output_line;
  for (int i = 0; i < word_count; i++) {
    // Again first word has a different margin.
    int left_margin = _left_margin;
    if (output_line.length() == 0) {
      left_margin = 0;      
    }
    // Check the maximum length size
    int line_len = local_line.length() + words[i].length() + left_margin;
    if (line_len < MAX_LINE_LENGTH) {
      local_line += words[i] + " ";
    } else { 
      output_line += local_line + local_margin;
      local_line = words[i] + " ";
    }
  }

  output_line += local_line;
  _left_margin = 0;

  return output_line;
}

int BeautifierHandlers::split(const string& buffer, vector<string>& words)
{
  int i = 0;
  int ult = 0;
  int count = 0;

  while (1) {
    bool keep_trying = false;
    for (unsigned int j = i; j < buffer.length(); j++) {
      if (isspace(buffer.c_str()[j]) != 0) {
	i = j;
	keep_trying = true;
	break;
      }
    }

    if (keep_trying == false) {
      words.push_back(buffer.substr(ult, buffer.length()-ult));
      count++;
      break;
    }

    words.push_back(buffer.substr(ult,i-ult));
    count++;
    i++;
    ult = i;
    if (i >= (int)buffer.length())
      break;
  }
  return count;
}

string BeautifierHandlers::alltrim(const string& buffer)
{
  string dummy;

  // Remove multiple zeros
  for (unsigned int i = 0; i <= (buffer.length() - 1); i++) {
    if (i < (buffer.length() - 1) && 
	isspace(buffer.c_str()[i]) && isspace(buffer.c_str()[i + 1])) {
      continue;
    }
    dummy += buffer[i];
  }
  
  // Remove Leading zero
  if (isspace(dummy.c_str()[0])) {
    if (dummy.length() > 1) {
      dummy = dummy.substr(1, dummy.length());
    } else {
      dummy = "";
    }
  }
  
  // Remove Trailing zero
  if (isspace(dummy.c_str()[dummy.length() - 1])) {
    dummy = dummy.substr(0, dummy.length() - 1);
  }

  return dummy;
}

void BeautifierHandlers::chars_handler()
{
  string lstr = alltrim(_buffered_chars);

  // skip full space strings
  if (lstr.length() > 0) {
    if (_last_non_chars_event == START) {   
      _to_be_printed += ">";   
      // Check the maximum line length
      string end_tag = "</" + _element_name + ">";
      int line_len = _to_be_printed.length() + lstr.length() + end_tag.length();
      if (line_len > MAX_LINE_LENGTH) {
	_truncated_txt = true;
	_user_data->output_txt += truncate(_to_be_printed);
	_to_be_printed = "";
      } 
    }
    _to_be_printed += lstr;
  }

  _buffered_chars = "";
}

LIBEPP_NICBR_NS_END
