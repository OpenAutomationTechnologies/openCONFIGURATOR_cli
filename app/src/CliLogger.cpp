/**
 * \file CliLogger.cpp
 *
 * \brief Implementation of wrapper module
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
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

CliLogger::CliLogger() :
	languageIndex((std::uint32_t)Language::EN)
{
}

CliLogger::~CliLogger()
{
}

CliLogger& CliLogger::GetInstance()
{
	static CliLogger instance;

	return instance;
}

CliResult CliLogger::HandleCliApiFailed(const std::string& apiDescription, 
										const CliResult& result)
{
	/** Log the reason of CLI API failure details */
	LOG_ERROR() << result.GetErrorMessage();

	/** Prepare the failure message with caller API name */
	boost::format formatter(kMsgCliApiFailed[languageIndex]);
	formatter % apiDescription;

	return CliResult(CliErrorCode::CLI_API_FAILED, formatter.str());
}

CliResult CliLogger::HandleCoreApiFailed(const std::string& apiDescription,
										 const Result& result)
{
	/** Log the reason of CLI API failure details */
	LOG_ERROR() << result.GetErrorMessage();

	/** Prepare the failure message with caller API name */
	boost::format formatter(kMsgCoreApiFailed[languageIndex]);
	formatter % apiDescription;

	return CliResult(CliErrorCode::CORE_API_FAILED, formatter.str());
}

CliResult CliLogger::HandleExceptionCaught(const std::string& apiDescription,
										   const std::exception& e)
{
	/** Log the reason of CLI API failure details */
	LOG_ERROR() << e.what();

	/** Prepare the failure message with caller API name */
	boost::format formatter(kMsgExceptionCaught[languageIndex]);
	formatter % apiDescription;

	return CliResult(CliErrorCode::EXCEPTION_CAUGHT, formatter.str());
}
