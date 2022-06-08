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
/* $Id: DomErrorHandler.cpp 1044 2009-10-19 14:39:10Z koji $ */

#include <config.h>

#include "DomErrorHandler.H"
#include "DomParser.H"
#include "XmlException.H"

XERCES_CPP_NAMESPACE_USE

LIBEPP_NICBR_NS_BEGIN

void DomErrorHandler::warning(const SAXParseException& exception)
{

}

void DomErrorHandler::error(const SAXParseException& exception)
{
  string message = DomParser::str_transcode(exception.getMessage());
  throw XmlException(XmlException::INVALID_XML_DOCUMENT, 
		     "The XML document is invalid", message);
}

void DomErrorHandler::fatalError(const SAXParseException& exception)
{
  string message = DomParser::str_transcode(exception.getMessage());
  throw XmlException(XmlException::INVALID_XML_DOCUMENT, 
		     "The XML document is invalid", message);
}

void DomErrorHandler::resetErrors()
{

}

LIBEPP_NICBR_NS_END
