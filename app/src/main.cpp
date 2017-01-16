/**
 * \file main.cpp
 *
 * \brief Implementation to receive the command line parameters for
 *        OpenCONFIGURATOR CLI and generate the POWERLINK configuration files
 *        at the output path
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


#include "OpenConfiguratorCli.h"

int main(int parameterCount, char* parameter[])
{
	std::vector<std::string> paramList;

	/** Prepare the parameter list */
	for (std::int32_t index = 1; index < parameterCount; index++)
	{
		paramList.push_back(parameter[index]);
	}

	/** Generate output configuration files */
	CliResult result = OpenConfiguratorCli::GetInstance().GenerateConfigurationFiles(paramList);
	if (!result.IsSuccessful())
	{
		switch (result.GetErrorType())
		{
			case CliErrorCode::USAGE:
				break;
			case CliErrorCode::FAILURE:
				{
					LOG_ERROR() << result.GetErrorMessage();
					std::cout << kApplicationName << ": ERROR " << result.GetErrorMessage();
				}
				break;
			default:
				{
					LOG_ERROR() << CliLogger::GetInstance().GetErrorString(result);
					std::cout << kApplicationName << ": ERROR " << CliLogger::GetInstance().GetErrorString(result);
				}
				break;
		}
	}
	else
	{
		boost::format formatter(kMsgConfGenerationSuccess[CliLogger::GetInstance().languageIndex]);
		formatter % OpenConfiguratorCli::GetInstance().outputPath;

		CliResult res(CliErrorCode::CONF_GENERATION_SUCCESS, formatter.str());
		LOG_INFO() << CliLogger::GetInstance().GetErrorString(res);

		std::cout << kApplicationName << ": INFO " << CliLogger::GetInstance().GetErrorString(res);
	}

	return 0;
}
