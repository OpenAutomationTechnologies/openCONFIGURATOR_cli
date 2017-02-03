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

#ifndef CLI_LOGGER_H_
#define CLI_LOGGER_H_

#include "Language.h"
#include "Result.h"
#include "CliErrorCode.h"
#include "CliResult.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
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

					/** \brief Generates the error string with tool code for CliResult
					  * \param result	CliResult for which error string to be created
					  * \return Error string with tool code
					  */
					std::string GetErrorString(const IndustrialNetwork::POWERLINK::Application::ErrorHandling::CliResult& result);

					/** \brief Generates the error string with tool code for Result
					  * \param result	Result for which error string to be created
					  * \return Error string with tool code
					  */
					std::string GetErrorString(const IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result& result);

					/** \brief Generates the failure error string for exception
					  * \param e		Exception ojbect
					  * \return CliResult
					  */
					IndustrialNetwork::POWERLINK::Application::ErrorHandling::CliResult GetFailureErrorString(const std::exception& e);

					/** \brief Generates the failure error string for Result with tool code
					  * \param result	Result for which error string to be created
					  * \return CliResult
					  */
					IndustrialNetwork::POWERLINK::Application::ErrorHandling::CliResult GetFailureErrorString(const IndustrialNetwork::POWERLINK::Core::ErrorHandling::Result& result);

					/** Index of the current language */
					std::uint32_t languageIndex;

			}; // end of class CliLogger
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // _CLI_LOGGER_H_
