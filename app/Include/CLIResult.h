
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
				const std::string kMsgToolUsage ="Usage: openCONFIGURATOR.exe <ProjectFilePath> [options]";
				const std::string kMsgOptions = " Options: ";
				const std::string kMsgProjectParameter =  " -p,--project <ProjectFile> \t Project XML file. ";
				const std::string kMsgOutputparameter = " -o,--output <OutputPath> \t Output path for generated files. ";
				const std::string kMsgLanguageparameter = " -de,--german \t German log messages. Default is English.";
				const std::string kMsgLogMessage = " -v,--verbose \t Log file. ";
				const std::string kMsgHelpParameter = " -h,--help <Help> \t Help. ";
				const std::string kMsgInvalidArguments = "Invalid number of arguments. Expected: 3 Actual: ";
				const std::string kMsgInsufficientParameters = "Insufficient command line parameters.";
				const std::string kMsgProjectFileExists = "Project file does not exists.";
				const std::string kMsgInvalidFile = "Invalid file!";
				const std::string kMsgOutputDirectoryExists ="Output directory does not exists.";
				const std::string kMsgInvalidparameter = "Invalid command line parameter. ";
					
				
												
			}
		}
	}
}
#endif
