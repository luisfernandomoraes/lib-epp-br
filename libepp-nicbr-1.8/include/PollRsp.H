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
/* $Id: PollRsp.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file PollRsp.H
 *  @brief EPP PollRsp Class
 */

#ifndef __POLL_RSP_H__
#define __POLL_RSP_H__

#include <string>
#include <map>
#include <memory>

#include "libepp_nicbr.H"

#include "Response.H"
#include "CommonData.H"

using std::string;
using std::map;
using std::auto_ptr;

LIBEPP_NICBR_NS_BEGIN

/// EPP PollRsp Class
class PollRsp : public Response
{
public:  
  struct MsgContent {
    string value;
    map< string, string, less<string> > attributes;
  }; 

  /// Default constructor
  PollRsp(bool reset = true) : Response(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Sets the message count
  /**
     @param count msg queue counter
  */
  void set_count(const string &count) { _count = count; }

  /// Sets the message id
  /**
     @param id message id
  */
  void set_id(const string &id) { _id = id; }

  /// Sets the message date
  /**

     @param qDate date when the message entered in the queue
  */
  void set_qDate(const string &qDate) { _qDate = qDate; }

  /// Sets the language of the message
  /**
     @param lang language msg
  */
  void set_lang(const string &lang) { _lang = lang; }

  /// Sets the message text
  /**
     @param text text msg
  */
  void set_text(const string &text) { _text = text; }

  /// Sets the msg content
  /**
     @param content msg
  */
  void set_content(const map< string, MsgContent, less<string> > &content) 
  { 
    _content = content; 
  }

  /// Sets the specific response inside the poll response
  /**
     @param resp response (resData element)
     @param type response type 
  */
  void set_response(Response *resp, const ActionType type) {
    _type = type;
    _response = auto_ptr<Response>(resp); 
  } 
  
  /// Returns the message count
  /**
     @return message count
  */
  string get_count() const { return _count; }

  /// Returns the message id
  /**
     @return message id
  */
  string get_id() const { return _id; }

  /// Returns the message date
  /**
     @return qDate
  */
  string get_qDate() const { return _qDate; }

  /// Returns the message language
  /**
     @return message language
  */
  string get_lang() const { return _lang; }
  
  /// Returns the message text
  /**
     @return message text
  */
  string get_text() const { return _text; }

  /// Returns the msg content
  /**
     @return msg content
  */
  map< string, MsgContent, less<string> > get_content() const 
  { 
    return _content;
  }

  /// Returns the response type of the specific response
  /**
     @return Response type
  */
  ActionType get_response_type() const { return _type; }

  /// Returns the specific response inside the poll response
  /**
     @return a pointer to the response
  */
  Response* get_response() { return _response.get(); }
  
  /// reset attributes
  void reset() { 
    Response::reset();
    _count = "";
    _id = "";
    _lang = "en";
    _text = "";
    _content.clear();
    _response = auto_ptr<Response>(new Response());
    _type = UNSET_ACTION;
  }

protected:
  /// Counter
  string _count;

  /// Contact id
  string _id;

  /// Date when the message entered in the queue
  string _qDate;

  /// Language msg
  string _lang;

  /// Text msg
  string _text;

  /// Message content
  map< string, MsgContent, less<string> > _content;
  
  /// Response type
  ActionType _type;

  /// Response (resData element)
  auto_ptr<Response> _response;
  
};

LIBEPP_NICBR_NS_END
#endif //__POLL_RSP_H__
