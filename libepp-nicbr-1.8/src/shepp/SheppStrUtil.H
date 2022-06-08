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
/* $Id: SheppStrUtil.H 910 2007-03-19 21:29:33Z eduardo $ */
/** @file SheppStrUtil.H
 *  @brief EPP command-line shell client string manipulation routines class
 */

#ifndef __SHEPP_STR_UTIL_H__
#define __SHEPP_STR_UTIL_H__

#include <string>
#include <vector>

#ifndef whitespace
#define whitespace(c) (((c) == ' ') || ((c) == '\t'))
#define LIMITED_READLINE
#endif //whitespace

using std::string;
using std::vector;

/// Useful string manipulation routines used by shepp
class SheppStrUtil
{
public:
  /// Removes leading and ending white spaces from line (shepp)
  /**
     @param line   line to be trimmed
     @return trimmed line
  */
  static char *trim(char *line)
  {
    
    // remove leading white spaces by incrementing pointer to beginning
    // of line
    char *head;
    for (head = line; whitespace(*head); head++)
      ;
    
    // returns if head points to the end of line
    if (*head == '\0') {
      return head;
    }
    
    // remove ending white spaces
    char *tail;
    tail = head + strlen(head) - 1;
    while (tail > head && whitespace(*tail)) {
      tail--;
    }
    
    // ends the string at position where a white space was last seen
    *++tail = '\0';
    
    return head;
  }
  
  /// Given a line, split its words by white space into a string vector (shepp)
  /**
     @param line   line to be parsed
     @param loop   boolean, default true, set to false to get only first word
     @return string vector with all words found in line
  */
  static vector<string> parse_line(char *line)
  {
    vector<string> words;
    int from;
    int to;
    char word[MAX_WORD_LENGTH + 1];
    bool loop = true;
    
    do {
      from = 0;
      to = 1;
      
      // ignore leading white spaces
      while (line[from] != '\0' && whitespace(line[from])) {
	line++;
      }
      
      // find end of this word
      while (line[to] != '\0' && !whitespace(line[to])) {
	to++;
      }
      
      // boundary check
      if (to - from > MAX_WORD_LENGTH) {
	printf("Error: MAX_WORD_LENGTH is %d.\n", MAX_WORD_LENGTH);
	words.clear();
	break;
      }

      // insert found word in vector
      strncpy(word, line, to - from);
      word[to - from] = '\0';
      if (strlen(word) > 0) {
	words.push_back((string) word);
      }

      if (line[to] == '\0') {
	// this is the last word
	loop = false;
      } else {
	// go to beginning of next word
	line += to + 1;
      }
    } while (loop);
    
    return words;
  }

  /**
     Splits input into first and second at first occurrence of splitter.
     If relaxed is true, it is acceptable for second to be empty.
     @param input     string to be divided
     @param first     left side goes here
     @param second    right side goes here
     @param splitter  division point
     @param relaxed   true means that second can be empty
  */
  static int split(string input, string &first, string &second,
		   string splitter, bool relaxed = false)
  {
    int split_pos = input.find(splitter, 0);
    
    // first can never be empty
    if (split_pos == 0) {
      return -1;
    }
    first = input.substr(0, split_pos);
    
    // second can be empty if relaxed is true
    if (split_pos == -1 ||
	split_pos == (int) (input.length() - 1)) {
      if (relaxed) {
	second = "";
      } else {
	return -1;
      }
    } else {
      second = input.substr(split_pos + 1);
    }
    
    return 0;  
  }

  /// Gathers command-line arguments bounded by quotes in a string
  /**
     @param words    reference to vector with arguments
     @param gather   reference to return string
     @return 0 if OK, -1 if ERROR
   */
  static int quote_gathering(vector<string> &words, string &gather)
  {
    string tmp_str = gather.substr(0, 1);

    if (words.empty() && tmp_str == "\"") {
      return -1;
    }

    if (tmp_str == "\"") {
      gather = gather.substr(1, gather.length() - 1);

      tmp_str = gather.substr(gather.length() - 1);
      while (tmp_str != "\"") {
	if (words.empty()) {
	  return -1;
	}

	gather += " " + words[0];

	words.erase(words.begin());
	tmp_str = gather.substr(gather.length() - 1);
      }

      //remove starting and ending quotes
      gather = gather.substr(0, gather.length() - 1);
    }

    return 0;
  }

  /// Removes chars [./-] from document strings
  /**
     @param doc   a CPF or CNPJ document
     @return input doc with no [./-] characters
   */
  static string doc2id(const string &doc)
  {
    string numbers = doc;

    StrUtil::gsub(numbers, ".", "");
    StrUtil::gsub(numbers, "/", "");
    StrUtil::gsub(numbers, "-", "");

    return numbers;
  }
};

#endif //__SHEPP_STR_UTIL_H__
