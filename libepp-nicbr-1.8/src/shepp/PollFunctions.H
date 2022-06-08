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
/* $Id: PollFunctions.H 804 2006-08-03 12:56:01Z eduardo $ */
/** @file PollFunctions.H
 *  @brief EPP Poll related functions
 */
#ifndef __POLL_FUNCTIONS_H__
#define __POLL_FUNCTIONS_H__
#include "SheppCommandFunctions.H"

#include "Poll.H"

/// print poll command usage info
/**
   @param error_msg   error message to be printed
   @param specific    optional, choses specific sub-command
   @return 0 if ok, -1 otherwise
*/
int cmd_poll_help(string error_msg)
{
  if (error_msg != "") {
    printf("error: %s\n", error_msg.c_str());
  }

  printf("poll command syntax help:\n");
  printf("\n");

  printf("  req\n");
  printf("\n");
  printf("  ack <msgID>\n");
  printf("\n");

  if (error_msg != "") {
    return -1;
  }

  return 0;
}

/// main poll command
/**
   @param arg   command-line input arguments
   @return 0 if ok, -1 otherwise
*/
int cmd_poll(vector<string> &args)
{
  Poll act;
  PollCmd *cmd = act.get_command();

  // poll command processing
  if (!args.empty() && !(args[0] == "help")) {
    if (args[0] == "req" || args[0] == "ack") {
      cmd->set_op(args[0]);
      args.erase(args.begin());
      
      if (cmd->get_op() == "req") {
	if (!args.empty()) {
	  return cmd_poll_help("too many arguments");
	}
      } else { //cmd->get_op() == "ack"
	if (args.empty()) {
	  return cmd_poll_help("unspecified msgID");
	} else if (args.size() > 1) {
	  return cmd_poll_help("too many arguments");
	}
	cmd->set_msgID(args[0]);
      }
      
      if (_debug) {
	printf("poll op: [%s]\n", cmd->get_op().c_str());
	
	if (cmd->get_msgID() != "") {
	  printf("  msgID: [%s]\n", cmd->get_msgID().c_str());
	}
      } //_debug
      
      if (process_action(act) != 0) {
	return -1;
      }
      
      return 0;
    } else {
      return cmd_poll_help("invalid command: poll " + args[0]);
    }
  }
  
  return cmd_poll_help("");
}

#endif //__POLL_FUNCTIONS_H__
