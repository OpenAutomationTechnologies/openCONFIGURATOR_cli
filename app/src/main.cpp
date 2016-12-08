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

int main(int parameterCount, char* parameter[])
{
	std::vector<std::string> paramList;

	/**< Prepare the parameter list */
	for (std::int32_t index = 1; index < parameterCount; index++)
	{
		paramList.push_back(parameter[index]);
	}

	/**< Generate output configuration files */
	CLIResult result = OpenConfiguratorCLI::GetInstance().GeneratePOWERLINKConfigurationFiles(paramList);

	if (!result.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_ERROR, result);
	}
	else
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_INFO, 
					"POWERLINK configuration files generated successfully");
	}

	return 0;
}
