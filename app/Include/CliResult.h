/**
 * \class CliResult
 *
 * \brief Error code messages and handling functionalities for CLI
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

#ifndef _CLI_RESULT_H_
#define _CLI_RESULT_H_

#include "IResult.h"
#include "CliErrorCode.h"

using namespace IndustrialNetwork::Fieldbus;
using namespace IndustrialNetwork::POWERLINK::Application::ErrorHandling;

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			namespace ErrorHandling
			{
				const std::string kMsgAppDescription[] = {"OpenCONFIGURATOR CLI Application", ""};
				const std::string kMsgCoreApiFailed[] = {"Core library API: \"%s\" failed", "\"%s\""};
				const std::string kMsgCliApiFailed[] = {"CLI library API: \"%s\" failed", "\"%s\""};
				const std::string kMsgExceptionCaught[] = {"Exception caught at \"%s\"", "\"%s\""};
				const std::string kMsgLessNoOfParams[] = {"Project XML file name and output path are mandatory", ""};
				const std::string kMsgOutputPathNotFound[] = {"Project output path not found from command line parameters", ""};
				const std::string kMsgXmlFileNotFound[] = {"Project XML file name is not found from command line parameters", ""};
				const std::string kMsgFileNotExists[] = {"File \"%s\" does not exists", "\"%s\""};
				const std::string kMsgFileExtnIncorrect[] = {"Extension of file \"%s\" is incorrect", "\"%s\""};
				const std::string kMsgSchemaFileNotExists[] = {"Schema file \"%s\" does not exists", "\"%s\""};
				const std::string kMsgOutputPathNotExiists[] = {"Output path \"%s\" not exists", "\"%s\""};
				const std::string kMsgErrorLoadingGrammer[] = {"Error loading grammer to DOM Parser", ""};
				const std::string kMsgFileSchemeNotValid[] = {"Schema of file is not valid", ""};
				const std::string kMsgNullPtrFound[] = {"NULL pointer found on \"%s\" ", "\"%s\""};
				const std::string kMsgNoResultForXPath[] = {"There is no result for the XPath \"%s\"", "\"%s\""};
				const std::string kMsgUnableToOpenLogFile[] = {"Unable to open the log file \"%s\"", "\"%s\""};
				const std::string kMsgConfGenerationSuccess[] = {"POWERLINK configuration files generated successfully", ""};
				const std::string kMsgErrorTableNotLoaded[] = {"Error table not loaded", ""};
				const std::string kMsgErrorInfoNotFound[] = {"Error information not found", ""};

				/** \brief Functionalities to handle the error messages
				  */
				class CliResult : public IndustrialNetwork::Fieldbus::IResult<IndustrialNetwork::POWERLINK::Application::ErrorHandling::CliErrorCode>
				{

					public:
						/** \brief Default constructor of the class 
						  */
						CliResult();

						/** \brief Destructor of the class 
						  */
						virtual ~CliResult();

						/** \brief Constructor of the class with error code 
						  */
						explicit CliResult(const CliErrorCode& errorCode);

						/** \brief Constructor of the class with error code and message
						  */
						CliResult(const CliErrorCode& errorCode, const std::string& errorMessage);

						/** \brief Status of the function
						  * \return true if function is successful; false otherwise
						  */
						bool IsSuccessful();
				};
			} // end of namespace ErrorHandling
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // _CLI_RESULT_H_
