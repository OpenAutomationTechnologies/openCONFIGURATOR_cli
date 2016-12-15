/**
 * \file CliErrorCode
 *
 * \brief Error code enums of CLI application
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

#ifndef _CLI_ERROR_CODE_H_
#define _CLI_ERROR_CODE_H_

#include <cstdint>

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			namespace ErrorHandling
			{
				/** \brief Represents the error codes returned by the openCONFIGURATOR CLI
				  */
				enum class CliErrorCode : std::uint8_t
				{
					SUCCESS = 0,				/** Operation completed successfully */
					USAGE = 1,					/** Usage of the application */
					CORE_API_FAILED,			/** Core library API failed */
					CLI_API_FAILED,				/** CLI library API failed */
					EXCEPTION_CAUGHT,			/** Exception caught */
					LESS_NO_OF_PARAMS,			/** Project XML file name and output path are mandatory */
					OUTPUT_PATH_NOT_FOUND,		/** Project output path not found from command line parameters */
					XML_FILE_NOT_FOUND,			/** Project XML file name is not found from command line parameters */
					FILE_NOT_EXISTS,			/** File does not exists */
					FILE_EXTN_INCORRECT,		/** Extension of file is incorrect */
					SCHEMA_FILE_NOT_EXISTS,		/** Schema file does not exists */
					OUTPUT_PATH_NOT_EXISTS,		/** Output path not exists */
					ERROR_LOADING_GRAMMER,		/** Error loading grammer to DOM Parser */
					FILE_SCHEMA_NOT_VALID,		/** Schema of file is not valid */
					NULL_POINTER_FOUND,			/** NULL pointer found */
					NO_RESULT_FOR_XPATH,		/** There is no result for the Managing Node XPath */
					UNABLE_TO_OPEN_LOG			/** Unable to open the log file */
				};
			} // end of namespace ErrorHandling
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // _CLI_ERROR_CODE_H_
