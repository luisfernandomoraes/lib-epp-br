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
/* $Id: DomErrorHandler.H 1044 2009-10-19 14:39:10Z koji $ */
/** @file DomErrorHandler.H
 *  @brief EPP DomErrorHandler Class
 */

#ifndef __DOMERRORHANDLER_H__
#define __DOMERRORHANDLER_H__

#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/ErrorHandler.hpp>

#include "libepp_nicbr.H"

XERCES_CPP_NAMESPACE_USE

LIBEPP_NICBR_NS_BEGIN

/// EPP DomErrorHandler Class
class DomErrorHandler : public ErrorHandler
{
public:

  /// Default Constructor
  DomErrorHandler() {}

  /// Default Destructor
  ~DomErrorHandler() {}

  /// Handles parsing warning
  /**
     @param exception The warning info encapsulated in a SAX parse exception. 
   */
  void warning(const SAXParseException& exception);

  /// Handles parsing errors
  /**
     @param exception The error info encapsulated in a SAX parse exception. 
   */
  void error(const SAXParseException& exception);

  /// Handles parsing fatal errors
  /**
     @param exception The error info encapsulated in a SAX parse exception. 
   */
  void fatalError(const SAXParseException& exception);

  /// Reset errors
  void resetErrors();
};

LIBEPP_NICBR_NS_END
#endif //__DOMERRORHANDLER_H__
