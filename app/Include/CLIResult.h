/************************************************************************
\file CLIResult.h
\author Sree Hari Vignesh, Kalycito Infotech Private Limited.
\brief Specifies the error and info messages.
************************************************************************/

/*------------------------------------------------------------------------------
Copyright (c) 2016, Kalycito Infotech Private Limited
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
#ifndef CLI_RESULT_H
#define CLI_RESULT_H

#include <string>
#include "Result.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			namespace ErrorHandling
			{
				const std::string kMsgToolGenerator =  "Kalycito Infotech Private Limited ";
				const std::string kMsgToolVendor = "B&R Internal" ;
				const std::string kMsgToolUsage = "Usage: openCONFIGURATOR.exe <ProjectFilePath> [options]";
				const std::string kMsgOptions = " Options: ";
				const std::string kMsgProjectParameter =  " -p,--project <ProjectFile> \t Project XML file. ";
				const std::string kMsgOutputParameter = " -o,--output <OutputPath> \t Output path for generated files. ";
				const std::string kMsgLanguageParameter = " -de,--german \t German log messages. Default is English.";
				const std::string kMsgLogMessage = " -v,--verbose \t Log file. ";
				const std::string kMsgHelpParameter = " -h,--help <Help> \t Help. ";
				const std::string kMsgInvalidArguments = "Invalid number of arguments. Expected: 3 Actual: ";
				const std::string kMsgInsufficientParameters = "Insufficient command line parameters.";
				const std::string kMsgProjectFileExists = "Project file does not exists.";
				const std::string kMsgInvalidFile = "Invalid file!";
				const std::string kMsgOutputDirectoryExists = "Output directory does not exists.";
				const std::string kMsgInvalidParameter = "Invalid command line parameter. ";
			}
		}
	}
}
#endif //CLI_RESULT_H


