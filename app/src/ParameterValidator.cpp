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

#include "OpenConfiguratorCLI.h"
#include "ParameterValidator.h"

ParameterValidator::ParameterValidator()
{

}

ParameterValidator::~ParameterValidator()
{

}

bool ParameterValidator::IsXMLFileValid(std::string xmlFileName)
{
	if (!boost::filesystem::exists(xmlFileName))
	{
		// Show error

		return false;
	}

	if (boost::filesystem::extension(xmlFileName) != ".xml")
	{
		// Show error

		return false;
	}
}

bool ParameterValidator::IsPathValid(std::string path)
{
	if (!boost::filesystem::exists(path))
	{
		// Show error

		return false;
	}
}

bool ParameterValidator::IsXMLSchemaValid(std::string xmlFileName)
{
	return false;
}

bool ParameterValidator::IsXDCSchemaValid(std::string xdcFileName)
{
	return false;
}
