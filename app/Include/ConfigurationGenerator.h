/**
 * \class ConfigurationGenerator
 *
 * \brief Generates the configuration file by getting byte stream from Core library
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#ifndef CONFIGURATION_GENERATOR_CLI_H
#define CONFIGURATION_GENERATOR1_CLI_H

#include <string>

#include "OpenConfiguratorCLI.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
				class ConfigurationGenerator
				{
					public:
						/// Default constructor of the class
						ConfigurationGenerator();

						/// Descriptor of the class
						~ConfigurationGenerator();

						/** \brief Creates single instance
						  * \return Static instance of the class
						  */
						static ConfigurationGenerator& GetInstance();


						bool BuildConciseDeviceConfiguration(std::string networkId,std::string outputPath);
						bool createMnobdTxt(std::string outputPath,std::string configuration);
						bool createMnobdCdc(std::string outputPath,std::string configuration);
						bool createMnobdHexTxt(std::string outputPath,std::string configuration);
					
				};
			}

		

	}
		
}


#endif // CONFIGURATION_GENERATOR_H

