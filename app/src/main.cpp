/**
 * \file main.cpp
 *
 * \brief Implementation to receive the command line parameters for
 *        OpenCONFIGURATOR CLI and generate the POWERLINK configuration files
 *        at the output path
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#include "OpenConfiguratorCLI.h"

using namespace IndustrialNetwork::POWERLINK::Application;

int main(int parameterCount, char* parameter[])
{
	std::vector<std::string> paramList;

	// Prepare the parameter list
	for (int index = 1; index < parameterCount; index++)
	{
		paramList.push_back(parameter[index]);
	}

	CLIResult ress = CLIResult(CLIErrorCode::LESS_NO_OF_PARAMS, kMsgLessNoOfParams);

	printf("%s",ress.GetErrorMessage().c_str());
	LOG_INFO() << ress.GetErrorMessage();

	// Generate output files
	bool result = OpenConfiguratorCLI::GetInstance().GeneratePOWERLINKConfigurationFiles(paramList);

	if (result == true)
	{
		LOG_INFO() <<"POWERLINK Configuration Files generated successfully";
	}
	else
	{
		LOG_INFO() <<"Failed to Generate POWERLINK Configuration Files";
	}

	return 0;
}
