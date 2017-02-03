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
#include "ParameterValidator.h"

using namespace IndustrialNetwork::POWERLINK::Application::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Application;

ParserElement::ParserElement(const std::string& file, const std::string& schemaFile, const std::string& xmlNamespace) :
	domDocument(NULL),
	domElement(NULL),
	domParser(NULL),
	filePath(file),
	schemaFilePath(schemaFile),
	xmlNamespace(xmlNamespace)
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
		xercesc::XMLPlatformUtils::Initialize();
		/** Validate for the schema file existance */
		CliResult res = ParameterValidator::GetInstance().IsFileExists(schemaFilePath);
		if (!res.IsSuccessful())
		{
			return res;
		}

		/** Load schema file constraints */
		if (domParser->loadGrammar(schemaFilePath.data(),
		                           xercesc::Grammar::SchemaGrammarType) == NULL)
		{
			return CliResult(CliErrorCode::ERROR_LOADING_GRAMMER,
			                 kMsgErrorLoadingGrammer[CliLogger::GetInstance().languageIndex]);
		}

		/** Set validation checks required for the file */
		domParser->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
		domParser->setDoNamespaces(true);
		domParser->setDoXInclude(true);
		domParser->setDoSchema(true);
		domParser->setValidationConstraintFatal(true);
		domParser->setValidationSchemaFullChecking(true);

		ParserErrorHandler parserErrorHandler;
		domParser->setErrorHandler(&parserErrorHandler);

		boost::filesystem::path schemaPath = boost::filesystem::absolute(schemaFilePath);
		std::string externalSchemaLocation = this->xmlNamespace;
		externalSchemaLocation.append(" ");
		externalSchemaLocation.append(schemaPath.string());
		domParser->setExternalSchemaLocation(externalSchemaLocation.c_str());

		/** Input file to DOM parse function */
		domParser->parse(filePath.data());
		if (domParser->getErrorCount() != 0)
		{
			boost::format formatter(kMsgFileSchemaNotValid[CliLogger::GetInstance().languageIndex]);
			formatter % filePath;

			return CliResult(CliErrorCode::FILE_SCHEMA_NOT_VALID, formatter.str());
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}
