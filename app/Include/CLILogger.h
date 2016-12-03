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

#include "OpenConfiguratorCLI.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			enum class CLIMessageType : std::uint8_t
			{
				MT_INFO = 0,
				MT_WARN,
				MT_ERROR,
				MT_DEBUG
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
					  * \return Nothing
					  */
					void SetFileLog(bool set);

					/** \brief Sets the language to be used for logging
					  * \param set true or false; true for German; false for English
					  * \return Nothing
					  */
					void SetLanguageToGerman(bool set);

					/** \brief Log messages in to file or console
					  * \param msgType Log message type
					  * \param result result of type CLIResult
					  * \return Nothing
					  */
					//void LogMessage(CLIMessageType msgType, CLIResult& result);

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
