/**
 * \class CLILogger
 *
 * \brief Functionalities of message logging
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#ifndef CLI_LOGGER_H
#define CLI_LOGGER_H

#include "Result.h"
#include "CLIErrorCode.h"
#include "CLIResult.h"

using namespace IndustrialNetwork::POWERLINK::Application::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			/// Log file name if file logging enabled
			const std::string kOpenConfiguratorCLILogFileName = "OpenCONFIGURATOR_CLI.log";

			/**
			  *\brief Types of messages LogMessage can accept
			  */
			enum class CLIMessageType : std::uint8_t
			{
				CLI_INFO = 0,
				CLI_WARN,
				CLI_ERROR,
				CLI_DEBUG
			};

			/**
			  *\brief Types of languages LogMessage can accept
			  */
			enum class CLILanguageType : std::uint8_t
			{
				EN = 0,				/**< English */
				DE = 1				/**< German */
			};

			class CLILogger
			{
				public:
					/// Default constructor of the class
					CLILogger();

					/// Descriptor of the class
					~CLILogger();

					/** \brief Creates single instance
					  * \return Static instance of the class
					  */
					static CLILogger& GetInstance();

					/** \brief Toggles the logging between console and file
					  * \param set true or false; true for file log; false for console log
					  * \path path logging file path
					  * \return Nothing
					  */
					void SetFileLog(bool set, std::string path);

					/** \brief Sets the language to be used for logging
					  * \param set true or false; true for German; false for English
					  * \return Nothing
					  */
					void SetLanguageToGerman(bool set);

					/** \brief Log message with given message type description
					  * \param msgType Log message type
					  * \param logDescription Message string to be logged
					  * \return Nothing
					  */
					void LogMessage(CLIMessageType msgType, std::string logDescription);

					/** \brief Log message with given message type description
					  * \param msgType Log message type
					  * \param result CLIResult returned by member functions
					  * \return Nothing
					  */
					void LogMessage(CLIMessageType msgType, CLIResult& result);

					/** \brief Logs message for CLI API failure
					  * \param apiDescription Description of the CLI API failure
					  * \param result CLIResult returned by the failed CLI API
					  * \return CLIResult Failure result with caller API name
					  */
					CLIResult HandleCliApiFailed(std::string apiDescription, CLIResult& result);

					/** \brief Logs message for Core API failure
					  * \param apiDescription Description of the CLI API failure
					  * \param result Result returned by the failed Core API
					  * \return CLIResult Failure result with caller API name
					  */
					CLIResult HandleCoreApiFailed(std::string apiDescription, Result& result);

					/** \brief Logs message when exception caught
					  * \param apiDescription Description of the CLI API failure
					  * \param e Exception ojbect
					  * \return CLIResult Failure result with caller API name
					  */
					CLIResult HandleExceptionCaught(std::string apiDescription, std::exception& e);

					std::uint8_t languageIndex;						///< Index of current language
				private:
					bool fileLog;									///< file Log enabled status

					bool languageGerman;							///< German language enabled status

					std::ofstream ofs;								///< Log file stream handle

					std::vector<std::string> CLIMessageTypeString;	///< Message type strings

			}; // end of class CLILogger
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // CLI_LOGGER_H
