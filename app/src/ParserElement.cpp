/**
 * \file ParserElement.cpp
 *
 * \brief Implementation of ParserElement module
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 1.0
 *
 */
/*------------------------------------------------------------------------------
Copyright (c) 2016, Kalycito Infotech Private Limited, INDIA.
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

#include "ParserElement.h"

ParserElement::ParserElement()
{
	/** Initialize the Xerces usage */
	xercesc::XMLPlatformUtils::Initialize();

	domParser = new xercesc::XercesDOMParser();
}

ParserElement::~ParserElement()
{
	/** Release the DOM elements */
	delete [] domParser;

	/** Release the Xerces usage */
	xercesc::XMLPlatformUtils::Terminate();
}

CliResult ParserElement::CreateElement(const std::string& file)
{
	try
	{
		filePath = file;

		/** Input project XML file to DOM parse() function */
		domParser->parse(filePath.c_str());

		/** Store the entire project XML file in DOMDocument */
		domDocument = domParser->getDocument();
		if (domDocument == NULL)
		{
			boost::format formatter(kMsgNullPtrFound[CliLogger::GetInstance().languageIndex]);
			formatter % "create DOM parser document";

			return CliResult(CliErrorCode::NULL_POINTER_FOUND, formatter.str());
		}

		/** Store the Top node element of the document in root */
		domElement = domDocument->getDocumentElement();
		if (domElement == NULL)
		{
			boost::format formatter(kMsgNullPtrFound[CliLogger::GetInstance().languageIndex]);
			formatter % "create DOM parser element";

			return CliResult(CliErrorCode::NULL_POINTER_FOUND, formatter.str());
		}
	}
	catch (std::exception& e)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Create Element", e);
	}

	return CliResult();
}
