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
 // REVIEW_COMMENT: Copyright and license to be added 

#include "OpenConfiguratorCli.h"

int main(int parameterCount, char* parameter[])
{
	std::vector<std::string> paramList;

	/** Prepare the parameter list */
	for (std::int32_t index = 1; index < parameterCount; index++)
	{
		paramList.push_back(parameter[index]);
	}

	/** Generate output configuration files // REVIEW_COMMENT: 80 characters limit per line*/
	CliResult result = OpenConfiguratorCli::GetInstance().GeneratePOWERLINKConfigurationFiles(paramList); // REVIEW_COMMENT: Powerlink; Reduce length of the function
// REVIEW_COMMENT: Remove line space
	if (!result.IsSuccessful())
	{
		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_ERROR, result);
	}
	else
	{
		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_INFO, 
					"POWERLINK configuration files generated successfully");// REVIEW_COMMENT: Full stop in the message
	}

	return 0;
}
