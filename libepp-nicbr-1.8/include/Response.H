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
/* $Id: Response.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file Response.H
 *  @brief EPP Response Class
 */

#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include <string>
#include <list>
#include <map>

#include "libepp_nicbr.H"
#include "StrUtil.H"

using std::string;
using std::list;
using std::map;

LIBEPP_NICBR_NS_BEGIN

/// EPP Response Class
class Response
{
public:
  /// Result Codes - See RFC 3730 - Session 3
  enum ResultCode {
    UNSET = -1,
    OK = 1000,
    OK_ACTION_PENDING = 1001,
    OK_NO_MESSAGES = 1300,
    OK_ACK_DEQUEUE = 1301,
    OK_ENDING_SESSION = 1500,
    UNKNOWN_COMMAND = 2000,
    COMMAND_SYNTAX_ERROR = 2001,
    COMMAND_USE_ERROR = 2002,
    PARAMETER_MISSING = 2003,
    PARAMETER_RANGE_ERROR = 2004,
    PARAMETER_SYNTAX_ERROR = 2005,
    UNIMPLEMENTED_VERSION = 2100,
    UNIMPLEMENTED_COMMAND = 2101,
    UNIMPLEMENTED_OPTION = 2102,
    UNIMPLEMENTED_EXTENSION = 2103,
    BILLING_FAILURE = 2104,
    NOT_RENEWABLE = 2105,
    NOT_TRANSFERABLE = 2106,
    AUTHENTICATION_ERROR = 2200,
    AUTHORIZATION_ERROR = 2201,
    INVALID_AUTHORIZATION_INFO = 2202,
    PENDING_TRANSFER = 2300,
    NOT_PENDING_TRANSFER = 2301,
    OBJECT_EXISTS = 2302,
    OBJECT_DOESNT_EXIST = 2303,
    STATUS_PROHIBITS_OPERATION = 2304,
    ASSOCIATION_PROHIBITS_OPERATION = 2305,
    PARAMETER_POLICY_ERROR = 2306,
    UNIMPLEMENTED_SERVICE = 2307,
    DATA_MGMT_POLICY_VIOLATION = 2308,
    COMMAND_FAILED = 2400,
    COMMAND_FAILED_CLOSING_CONN = 2500,
    AUTHENTICATION_ERROR_CLOSING_CONN = 2501,
    SESSION_LIMIT_EXCEEDED = 2502
  };

  struct ResultExtValue {
    string value;
    string xmlns;
    string reason;
  };

  struct ResultInfo {
    string msg;
    list<ResultExtValue> ext_values;
  };

  /// Default constructor
  Response(bool reset = true)
  {
    if (reset) {
      this->reset();
    }
  }
    
  /// Sets the client transaction ID
  /**
     @param clTRID Client transacion ID
  */
  void set_clTRID(const string &clTRID) { _clTRID = clTRID; }
    
  /// Sets the server transaction ID
  /**
     @param svTRID Server transacion ID
  */
  void set_svTRID(const string &svTRID) { _svTRID = svTRID; }

  /// Sets the result message language
  /**
     @param result_lang Result message language
  */
  void set_result_lang(const string &result_lang) 
  { 
    _result_lang = result_lang; 
  }

  /// Sets a result element
  /**
     @param code Result code
     @param msg Response text
     @param value value sub-element of extValue
     @param xmlns value sub-element's attribute
     @param reason reason sub-element of extValue
   */
  void insert_result(const ResultCode &code, const string &msg,
		     const string &value = "", const string &xmlns = "",
		     const string &reason = "")
  {
    _result_list[code].msg = msg;

    if (value != "" || xmlns != "" || reason != "") {
      struct ResultExtValue extValue;
      extValue.value = value;
      extValue.xmlns = xmlns;
      extValue.reason = reason;
      _result_list[code].ext_values.push_back(extValue);
    }
  }

  /// Returns the client transaction ID
  /**
     @return Client transacion ID
  */
  string get_clTRID() const { return _clTRID; }
    
  /// Returns the servers transaction ID
  /**
     @return Server transacion ID
  */
  string get_svTRID() const { return _svTRID; }

  /// Returns the result message language
  /**
     @return Result message language
  */
  string get_result_lang() { return _result_lang; }

  /// Returns the list of result elements
  /**
     @return List of result elements
   */
  map<ResultCode, ResultInfo> get_result_list() const
  { 
    return _result_list;
  }

  /// reset attributes
  void reset() 
  {
    _clTRID = "";
    _svTRID = "";
    _result_lang = "en";
    _result_list.clear();
  }
    
  /// virtual destructor
  virtual ~Response() {}

protected:
  /// Client transaction ID
  string _clTRID;

  /// Server transaction ID
  string _svTRID;

  /// Result language
  string _result_lang;

  /// Result elements list
  map<ResultCode, ResultInfo> _result_list;
};

LIBEPP_NICBR_NS_END
#endif //__RESPONSE_H__
