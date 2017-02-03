/************************************************************************
\file ParserErrorHandler.cpp
\brief Implementation of the Class ParserErrorHandler
\author rueckerc, Bernecker+Rainer Industrie Elektronik Ges.m.b.H.
\date 01-February-2017 12:00:00
************************************************************************/

/*------------------------------------------------------------------------------
Copyright (c) 2017, Bernecker+Rainer Industrie-Elektronik Ges.m.b.H. (B&R)
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of the copyright holders nor the
names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
------------------------------------------------------------------------------*/

#include "ParserErrorHandler.h"

using namespace IndustrialNetwork::POWERLINK::Application;
using namespace IndustrialNetwork::POWERLINK::Application::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

std::string ParserErrorHandler::reportParseException(const xercesc::SAXParseException& ex)
{
	char* msg = xercesc::XMLString::transcode(ex.getMessage());
	boost::format formatter(kMsgXmlValidationError[CliLogger::GetInstance().languageIndex]);
	formatter % ex.getColumnNumber()
	% ex.getLineNumber()
	% msg;
	xercesc::XMLString::release(&msg);
	return formatter.str();
}

void ParserErrorHandler::warning(const xercesc::SAXParseException& ex)
{
	LOG_WARN() << reportParseException(ex);
}

void ParserErrorHandler::error(const xercesc::SAXParseException& ex)
{
	LOG_ERROR() << reportParseException(ex);
}

void ParserErrorHandler::fatalError(const xercesc::SAXParseException& ex)
{
	LOG_FATAL() << reportParseException(ex);
}

void ParserErrorHandler::resetErrors()
{
}
