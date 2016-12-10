/**
 * \class CliLogger
 *
 * \brief Functionalities of message logging
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

#ifndef _CLI_LOGGER_H_
#define _CLI_LOGGER_H_

#include "Result.h"
#include "CliErrorCode.h"
#include "CliResult.h"

using namespace IndustrialNetwork::POWERLINK::Application::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			/** Log file name if file logging enabled */
			const std::string kOpenConfiguratorCliLogFileName = "OpenCONFIGURATOR_CLI.log";

			/** Types of messages LogMessage can accept */
			enum class CliMessageType : std::uint8_t
			{
				CLI_INFO = 0,
				CLI_WARN,
				CLI_ERROR,
				CLI_DEBUG
			};

			/** Types of languages LogMessage can accept */
			enum class CLILanguageType : std::uint8_t
			{
				EN = 0,				/** English */
				DE = 1				/** German */
			};

			class CliLogger
			{
				public:
					/** \brief Default constructor of the class
					  */
					CliLogger();

					/** \brief Destructor of the class 
					  */
					~CliLogger();

					/** \brief Creates single instance
					  * \return Static instance of the class
					  */
					static CliLogger& GetInstance();

					/** \brief Toggles the logging between console and file
					  * \param set		true for file log; false for console log
					  * \path path		Logging file path
					  * \return Nothing
					  */
					void SetFileLog(const bool set, const std::string path);

					/** \brief Sets the language to be used for logging
					  * \param set		true for German; false for English
					  * \return Nothing
					  */
					void SetLanguageToGerman(const bool set);

					/** \brief Log message with given message type description
					  * \param msgType			Log message type
					  * \param logDescription	Message string to be logged
					  * \return Nothing
					  */
					void LogMessage(const CliMessageType msgType, 
									const std::string logDescription);

					/** \brief Log message with given message type description
					  * \param msgType			Log message type
					  * \param result			CliResult returned by member functions
					  * \return Nothing
					  */
					void LogMessage(const CliMessageType msgType, 
									const CliResult& result);

					/** \brief Logs message for CLI API failure
					  * \param apiDescription	Description of the CLI API failure
					  * \param result			CliResult returned by the failed CLI API
					  * \return CliResult failure result with caller API name
					  */
					CliResult HandleCliApiFailed(const std::string apiDescription, 
									const CliResult& result);

					/** \brief Logs message for Core API failure
					  * \param apiDescription	Description of the CLI API failure
					  * \param result			Result returned by the failed Core API
					  * \return CliResult failure result with caller API name
					  */
					CliResult HandleCoreApiFailed(const std::string apiDescription, 
									const Result& result);

					/** \brief Logs message when exception caught
					  * \param apiDescription	Description of the CLI API failure
					  * \param e				Exception ojbect
					  * \return CliResult failure result with caller API name
					  */
					CliResult HandleExceptionCaught(const std::string apiDescription, 
									const std::exception& e);

					/** Index of current language */
					std::uint8_t languageIndex;
				private:
					/** File Log enabled status */
					bool fileLog;

					/** German language enabled status */
					bool languageGerman;

					/** Log file stream handle */
					std::ofstream ofs;

					/** Message type strings */
					std::vector<std::string> CliMessageTypeString;

			}; // end of class CliLogger
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // _CLI_LOGGER_H_
