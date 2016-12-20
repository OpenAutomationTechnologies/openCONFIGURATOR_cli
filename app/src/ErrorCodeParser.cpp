/**
 * \class ErrorCodeParser
 *
 * \brief Implementation of ErrorCodeParser module
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

#include "ErrorCodeParser.h"

ErrorCodeParser::ErrorCodeParser() :
	errorCodeObject(),
	isErrorTableLoaded(false)
{
}

ErrorCodeParser::~ErrorCodeParser()
{
}

ErrorCodeParser& ErrorCodeParser::GetInstance()
{
	static ErrorCodeParser instance;

	return instance;
}

CliResult ErrorCodeParser::LoadErrorCodeFile()
{
	/** Create results for error codes */
	CliResult clires;
	CliResult ceres;

	/** Validate the parameters */
	ceres = ParameterValidator::GetInstance().IsXmlFileValid(kErrorCodeXmlFile);
	if (!ceres.IsSuccessful())
	{
		return ceres;
	}

	ParserElement xmlElement(kErrorCodeXmlFile);

	/* Validate the XML file schema */
	ceres = ParameterValidator::GetInstance().IsErrCodeXmlSchemaValid(xmlElement.domParser);
	if (!ceres.IsSuccessful())
	{
		/** XML file schema is not valid */
		return ceres;
	}

	ceres = xmlElement.CreateElement();
	if (!ceres.IsSuccessful())
	{
		return ceres;
	}

	clires = CreateErrorTable(xmlElement);
	if (!clires.IsSuccessful())
	{
		return clires;
	}

	return CliResult();
}

CliResult ErrorCodeParser::CreateErrorTable(const ParserElement& element)
{
	ParserResult pResult;
	CliResult crres;

	crres = pResult.CreateResult(element, kErrorCodeComponent,
									kFormatStrErrorCodeComponent);
	if (!crres.IsSuccessful())
	{
		return crres;
	}

	errorCodeObject.clear();
	
	for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
	{
		ParserResult pSubResult;
		CliResult subres;
		ErrorCodeComponent ecComponent;

		ecComponent.component = pResult.parameters[row].at(0);
		ecComponent.componentVersion = pResult.parameters[row].at(1);

		subres = pSubResult.CreateResult(element, kErrorCode,
											kFormatStrErrorCode,
											pResult.node.at(row));
		if (!subres.IsSuccessful())
		{
			return subres;
		}

		for (std::uint32_t subrow = 0; subrow < pSubResult.parameters.size(); subrow++)
		{
			ParserResult pDescResult;
			CliResult descres;
			ErrorCode errCode;

			errCode.value = pSubResult.parameters[subrow].at(0);
			errCode.originalCode = 0xFFFF;
			if (!pSubResult.parameters[subrow].at(1).empty())
			{
				errCode.originalCode = stoi(pSubResult.parameters[subrow].at(1), NULL, 10);
			}
			errCode.toolCode = 0xFFFF;
			if (!pSubResult.parameters[subrow].at(2).empty())
			{
				errCode.toolCode = stoi(pSubResult.parameters[subrow].at(2), NULL, 10);
			}

			descres = pDescResult.CreateResult(element, kErrorDescription,
												kFormatStrErrorDescription,
												pSubResult.node.at(subrow));
			if (!descres.IsSuccessful())
			{
				return descres;
			}

			for (std::uint32_t descrow = 0; descrow < pDescResult.parameters.size(); descrow++)
			{
				ErrCoderDesc errDesc;

				errDesc.lang = Language::EN;
				if (pDescResult.parameters[descrow].at(0).compare("de") == 0)
				{
					errDesc.lang = Language::DE;
				}

				errDesc.value = pDescResult.parameters[descrow].at(1);
				errCode.descriptions.push_back(errDesc);
			}

			ecComponent.errorCodes.push_back(errCode);
		}

		errorCodeObject.push_back(ecComponent);
	}

	isErrorTableLoaded = true;

	return CliResult();
}

CliResult ErrorCodeParser::GetToolCode(const std::string& compType, 
									   const std::uint32_t& originalCode, 
									   std::uint32_t& toolCode)
{
	if (isErrorTableLoaded)
	{
		for (std::uint32_t index = 0; index < errorCodeObject.size(); index++)
		{
			std::uint32_t ecComponentIndex = 0;

			if (compType.compare(ComponentType::kComponentLibrary) == 0)
			{
				ecComponentIndex = 0;
			}
			else if (compType.compare(ComponentType::kComponentCli) == 0)
			{
				ecComponentIndex = 1;
			}
			else
			{
				return CliResult(CliErrorCode::ERROR_INFO_NOT_FOUND, 
						kMsgErrorInfoNotFound[CliLogger::GetInstance().languageIndex]);
			}

			for (ErrorCode errCode : errorCodeObject.at(ecComponentIndex).errorCodes)
			{
				if (errCode.originalCode == originalCode)
				{
					toolCode = errCode.toolCode;
					return CliResult();
				}
			}

			return CliResult(CliErrorCode::ERROR_INFO_NOT_FOUND, 
						kMsgErrorInfoNotFound[CliLogger::GetInstance().languageIndex]);
		}
	}
	else
	{
		return CliResult(CliErrorCode::ERROR_TABLE_NOT_LOADED, 
				kMsgErrorTableNotLoaded[CliLogger::GetInstance().languageIndex]);
	}

	return CliResult(CliErrorCode::ERROR_INFO_NOT_FOUND, 
						kMsgErrorInfoNotFound[CliLogger::GetInstance().languageIndex]);
}