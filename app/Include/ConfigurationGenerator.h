/**
 * \class ConfigurationGenerator
 *
 * \brief Generates the configuration file by getting byte stream from Core library
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

#ifndef _CONFIGURATION_GENERATOR_CLI_H_
#define _CONFIGURATION_GENERATOR_CLI_H_

#include "OpenConfiguratorCli.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			class ConfigurationGenerator
			{
				public:
					/** \brief Default constructor of the class
					  */
					ConfigurationGenerator();

					/** \brief Destructor of the class
					  */
					~ConfigurationGenerator();

					/** \brief Creates single instance
					  * \return Static instance of the class
					  */
					static ConfigurationGenerator& GetInstance();

					/** \brief Parses and builds configuration output files
					  * \param xmlPath			XML file name with path to be parsed
					  * \param outputPath		Output path of the configuraiton files
					  * \return CliResult
					  */
					CliResult GenerateOutputFiles(const std::string& xmlPath,
									const std::string& outputPath);
				private:
					/** \brief Creates Mnobd configuration files
					  * \param outputPath		Output path of the configuraiton files
					  * \return CliResult
					  */
					CliResult BuildConciseDeviceConfiguration(const std::string& outputPath);

					/** \brief Creates process images
					  * \param outputPath		Output path of the image files
					  * \return CliResult
					  */
					CliResult BuildProcessImageDescriptions(const std::string& outputPath);

					/** \brief Creates mnobd.txt
					  * \param outputPath		Output path of the image files
					  * \param configuration	Configuration output to be copied
					  * \return CliResult
					  */
					CliResult CreateMnobdTxt(const std::string& outputPath,
									const std::string& configuration);

					/** \brief Creates mnobd.cdc
					  * \param outputPath		Output path of the image files
					  * \param buffer			Buffer output to be coiped
					  * \return CliResult
					  */
					CliResult CreateMnobdCdc(const std::string& outputPath,
									const std::ostringstream& buffer);

					/** \brief Creates mnobdhex.txt
					  * \param outputPath		Output path of the image files
					  * \param buffer			Buffer output to be coiped
					  * \return CliResult
					  */
					CliResult CreateMnobdHexTxt(const std::string& outputPath,
									const std::ostringstream& buffer);

					/** \brief Creates CProcess image file
					  * \param nodeId			Node for which the process to be created
					  * \param outputPath		Output path of the image files
					  * \return CliResult
					  */
					CliResult CreateCProcessImage(const std::uint8_t nodeId,
									const std::string& outputPath);

					/** \brief Creates XML process image file
					  * \param nodeId			Node for which the process to be created
					  * \param outputPath		Output path of the image files
					  * \return CliResult
					  */
					CliResult CreateXmlProcessImage(const std::uint8_t nodeId,
									const std::string& outputPath);

					/** \brief Creates CSharp process image file
					  * \param nodeId			Node for which the process to be created
					  * \param outputPath		Output path of the image files
					  * \return CliResult
					  */
					CliResult CreateCSharpProcessImage(const std::uint8_t nodeId,
									const std::string& outputPath);

			}; // end of class ConfigurationGenerator
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // _CONFIGURATION_GENERATOR_CLI_H_
