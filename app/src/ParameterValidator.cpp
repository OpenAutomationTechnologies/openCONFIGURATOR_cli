/**
 * \file ParameterValidator.cpp
 *
 * \brief Implementation of validation of XML file and output path
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

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

CLIResult ParameterValidator::IsXMLFileValid(std::string xmlFileName)
{
	if (!boost::filesystem::exists(xmlFileName))
	{
		boost::format formatter(kMsgXMLFileNotExists[CLILogger::GetInstance().languageIndex]);
		formatter
		% xmlFileName.c_str();

		return CLIResult(CLIErrorCode::XML_FILE_NOT_EXISTS, formatter.str());
	}

	if (boost::filesystem::extension(xmlFileName) != ".xml")
	{
		return CLIResult(CLIErrorCode::XML_FILE_EXTN_INCORRECT, 
							kMsgXMLFileExtnIncorrect[CLILogger::GetInstance().languageIndex]);
	}

	return CLIResult();
}

CLIResult ParameterValidator::IsPathValid(std::string path)
{
	if (!boost::filesystem::exists(path))
	{
		boost::format formatter(kMsgOutputPathNotExiists[CLILogger::GetInstance().languageIndex]);
		formatter
		% path.c_str();

		return CLIResult(CLIErrorCode::OUTPUT_PATH_NOT_EXISTS, formatter.str());
	}

	return CLIResult();
}

CLIResult ParameterValidator::IsXMLSchemaValid(std::string xmlFileName)
{
	return IsSchemaValid(xmlFileName, kXmlSchemaDefinitionFileName);
}

CLIResult ParameterValidator::IsXDCSchemaValid(std::string xdcFileName)
{
	return IsSchemaValid(xdcFileName, kXdcSchemaDefinitionFileName);
}

CLIResult ParameterValidator::IsSchemaValid(std::string fileName, const std::string schemaDefFile)
{
	xercesc::XercesDOMParser domParser;

	if (!boost::filesystem::exists(schemaDefFile))
	{
		boost::format formatter(kMsgSchemaFileNotExists[CLILogger::GetInstance().languageIndex]);
		formatter
		% schemaDefFile.c_str();

		return CLIResult(CLIErrorCode::SCHEMA_FILE_NOT_EXISTS, formatter.str());
	}

	/**< Input project file to DOM parse() function */
	domParser.parse(fileName.c_str()); 

	if (domParser.loadGrammar(schemaDefFile.c_str(), 
								xercesc::Grammar::SchemaGrammarType) == NULL)
	{
		return CLIResult(CLIErrorCode::ERROR_LOADING_GRAMMER, 
							kMsgErrorLoadingGrammer[CLILogger::GetInstance().languageIndex]);
	}

	/**< Set validation checks required for the file */
	domParser.setValidationScheme(xercesc::XercesDOMParser::Val_Always);
	domParser.setDoNamespaces(true);
	domParser.setDoXInclude(true);
	domParser.setDoSchema(true);
	domParser.setValidationConstraintFatal(true);

	if (domParser.getErrorCount() != 0)
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_INFO, "DOM Parser error count" + domParser.getErrorCount());

		boost::format formatter(kMsgFileSchemeNotValid[CLILogger::GetInstance().languageIndex]);
		formatter
		% fileName.c_str();

		return CLIResult(CLIErrorCode::FILE_SCHEMA_NOT_VALID, formatter.str());
	}

	return CLIResult();
}
