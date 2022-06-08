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
/* $Id: PollCmd.H 1040 2009-10-01 17:49:12Z fneves $ */
/** @file PollCmd.H
 *  @brief EPP PollCmd Class
 */

#ifndef __POLL_CMD_H__
#define __POLL_CMD_H__

#include <string>

#include "libepp_nicbr.H"

#include "Command.H"

using std::string;

LIBEPP_NICBR_NS_BEGIN

/// EPP PollCmd Class
class PollCmd : public Command
{
public:
  /// Default constructor
  PollCmd(bool reset = true) : Command(false)
  {
    if (reset) {
      this->reset();
    }
  }

  /// Sets the operation of the poll command ("req" or "ack")
  /**
     @param op operation ("req" or "ack")
  */
  void set_op(const string &op) { _op = op; }

  /// Sets the msgID of the message being acknowledged
  /**
     @param msgID message id (just in "ack" operation)
  */
  void set_msgID(const string &msgID) { _msgID = msgID; }

  /// Returns the operation of the poll command
  /**
     @return operation
  */
  string get_op() const { return _op; }

  /// Returns the msgID of the message
  /**
     @return message id
  */
  string get_msgID() const { return _msgID; }

  /// reset attributes
  void reset() { 
    Command::reset();
    _op = "";
    _msgID = "";
  }
  
protected:
  /// Operation
  string _op;

  /// Message id
  string _msgID;
  
};

LIBEPP_NICBR_NS_END
#endif //__POLL_CMD_H__
