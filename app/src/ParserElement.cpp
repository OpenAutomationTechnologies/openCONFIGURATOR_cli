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

ParserElement::ParserElement() :
	domDocument(NULL),
	domElement(NULL),
	domParser(NULL),
	filePath(""),
	schemaFilePath("")
{
}

ParserElement::ParserElement(std::string file, std::string schemaFile) :
	domDocument(NULL),
	domElement(NULL),
	domParser(NULL),
	filePath(file),
	schemaFilePath(schemaFile)
{
	/** Initialize the Xerces usage */
	xercesc::XMLPlatformUtils::Initialize();

	domParser = new xercesc::XercesDOMParser();
}

ParserElement::~ParserElement()
{
	/** Release the DOM elements */
	delete domParser;

	/** Release the Xerces usage */
	xercesc::XMLPlatformUtils::Terminate();
}

CliResult ParserElement::CreateElement()
{
	try
	{
		if (!domParser)
		{
			return CliResult(CliErrorCode::NULL_POINTER_FOUND,
					kMsgNullPtrFound[CliLogger::GetInstance().languageIndex]);
		}

		/** Create parse element by validating against the schema file */
		CliResult res = parseFile();
		if (!res.IsSuccessful())
		{
			return res;
		}

		/** Store the entire project XML file in DOMDocument */
		domDocument = domParser->getDocument();
		if (domDocument == NULL)
		{
			return CliResult(CliErrorCode::NULL_POINTER_FOUND, 
					kMsgNullPtrFound[CliLogger::GetInstance().languageIndex]);
		}

		/** Store the Top node element of the document in root */
		domElement = domDocument->getDocumentElement();
		if (domElement == NULL)
		{
			return CliResult(CliErrorCode::NULL_POINTER_FOUND,
					kMsgNullPtrFound[CliLogger::GetInstance().languageIndex]);
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}

CliResult ParserElement::parseFile()
{
	try
	{
		/** Validate for the schema file existance */
		if (!boost::filesystem::exists(schemaFilePath))
		{
			boost::format formatter(kMsgSchemaFileNotExists[CliLogger::GetInstance().languageIndex]);
			formatter % schemaFilePath;

			return CliResult(CliErrorCode::SCHEMA_FILE_NOT_EXISTS, formatter.str());
		}

		/** Load schema file constraints */
		if (domParser->loadGrammar(schemaFilePath.data(),
									xercesc::Grammar::SchemaGrammarType) == NULL)
		{
			return CliResult(CliErrorCode::ERROR_LOADING_GRAMMER,
					kMsgErrorLoadingGrammer[CliLogger::GetInstance().languageIndex]);
		}

		/** Set validation checks required for the file */
		domParser->setValidationScheme(xercesc::XercesDOMParser::Val_Auto);
		domParser->setDoNamespaces(true);
		domParser->setDoXInclude(true);
		domParser->setDoSchema(true);
		domParser->setValidationConstraintFatal(true);
		domParser->setValidationSchemaFullChecking(true);

		/** Input file to DOM parse function */
		domParser->parse(filePath.data());
		if (domParser->getErrorCount() != 0)
		{
			return CliResult(CliErrorCode::FILE_SCHEMA_NOT_VALID,
					kMsgFileSchemeNotValid[CliLogger::GetInstance().languageIndex]);
		}
	}
	catch(const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}
