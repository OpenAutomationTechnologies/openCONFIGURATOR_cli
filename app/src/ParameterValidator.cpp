/**
 * \file ParameterValidator.cpp
 *
 * \brief Implementation of validation of XML file and output path
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

#include "ParameterValidator.h"

ParameterValidator::ParameterValidator()
{
}

ParameterValidator::~ParameterValidator()
{
}

ParameterValidator& ParameterValidator::GetInstance()
{
	static ParameterValidator instance;
	
	return instance;
}

CliResult ParameterValidator::IsFileValid(const std::string& fileName, 
											const std::string& fileExtn)
{
	try
	{
		if (!boost::filesystem::exists(fileName))
		{
			boost::format formatter(kMsgFileNotExists[CliLogger::GetInstance().languageIndex]);
			formatter % fileName.c_str();

			return CliResult(CliErrorCode::FILE_NOT_EXISTS, formatter.str());
		}

		if (boost::filesystem::extension(fileName).compare(fileExtn) != 0)
		{
			boost::format formatter(kMsgFileExtnIncorrect[CliLogger::GetInstance().languageIndex]);
			formatter % fileName.c_str();

			return CliResult(CliErrorCode::FILE_EXTN_INCORRECT, formatter.str());
		}
	}
	catch(std::exception& e)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Is File Valid", e);
	}

	return CliResult();
}

CliResult ParameterValidator::IsXmlFileValid(const std::string& xmlFileName)
{
	return IsFileValid(xmlFileName, ".xml");
}

CliResult ParameterValidator::IsXdcFileValid(const std::string& xdcFileName)
{
	return IsFileValid(xdcFileName, ".xdc");
}

CliResult ParameterValidator::IsPathValid(const std::string& path)
{
	try
	{
		if (!boost::filesystem::exists(path))
		{
			boost::format formatter(kMsgOutputPathNotExiists[CliLogger::GetInstance().languageIndex]);
			formatter % path.c_str();

			return CliResult(CliErrorCode::OUTPUT_PATH_NOT_EXISTS, formatter.str());
		}
	}
	catch(std::exception& e)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Is Path Valid", e);
	}

	return CliResult();
}

CliResult ParameterValidator::IsXmlSchemaValid(const std::string& xmlFileName)
{
	return IsSchemaValid(xmlFileName, kXmlSchemaDefinitionFileName);
}

CliResult ParameterValidator::IsXdcSchemaValid(const std::string& xdcFileName)
{
	return IsSchemaValid(xdcFileName, kXdcSchemaDefinitionFileName);
}

CliResult ParameterValidator::IsSchemaValid(const std::string& fileName, 
											const std::string& schemaDefFile)
{
	try
	{
		/** Initialize the Xerces usage */
		xercesc::XMLPlatformUtils::Initialize();
		xercesc::XercesDOMParser* domParserXdc = new xercesc::XercesDOMParser();

		/** Validate for the schema file existance */
		if (!boost::filesystem::exists(schemaDefFile))
		{
			boost::format formatter(kMsgSchemaFileNotExists[CliLogger::GetInstance().languageIndex]);
			formatter % schemaDefFile.c_str();

			return CliResult(CliErrorCode::SCHEMA_FILE_NOT_EXISTS, formatter.str());
		}

		/** Input project file to DOM parse() function */
		domParserXdc->parse(fileName.c_str()); 

		if (domParserXdc->loadGrammar(schemaDefFile.c_str(), 
									xercesc::Grammar::SchemaGrammarType) == NULL)
		{
			return CliResult(CliErrorCode::ERROR_LOADING_GRAMMER, 
								kMsgErrorLoadingGrammer[CliLogger::GetInstance().languageIndex]);
		}

		/** Set validation checks required for the file */
		domParserXdc->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
		domParserXdc->setDoNamespaces(true);
		domParserXdc->setDoXInclude(true);
		domParserXdc->setDoSchema(true);
		domParserXdc->setValidationConstraintFatal(true);

		if (domParserXdc->getErrorCount() != 0)
		{
			CliLogger::GetInstance().LogMessage(CliMessageType::CLI_INFO, 
												"DOM Parser error count" + domParserXdc->getErrorCount());

			boost::format formatter(kMsgFileSchemeNotValid[CliLogger::GetInstance().languageIndex]);
			formatter % fileName.c_str();

			return CliResult(CliErrorCode::FILE_SCHEMA_NOT_VALID, formatter.str());
		}

		delete [] domParserXdc;

		/** Release the Xerces usage */
		xercesc::XMLPlatformUtils::Terminate();
	}
	catch(std::exception& e)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Is Schema Valid", e);
	}

	return CliResult();
}
