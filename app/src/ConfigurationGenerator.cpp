/**
 * \file ConfigurationGenerator.cpp
 *
 * \brief Implementation of ConfigurationGenerator module
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

#include "ConfigurationGenerator.h"
#include "ProjectParser.h"

ConfigurationGenerator::ConfigurationGenerator()
{
}

ConfigurationGenerator::~ConfigurationGenerator()
{
}

ConfigurationGenerator& ConfigurationGenerator::GetInstance()
{
	static ConfigurationGenerator instance;

	return instance;
}

CliResult ConfigurationGenerator::GenerateConfigurationFiles(const std::string xmlPath, 
															 const std::string outputPath)
{
	CliResult cliRes;

	/** Parse the XML file */
	cliRes = ProjectParser::GetInstance().ParseXmlFile(xmlPath);
	if (!cliRes.IsSuccessful())
	{
		return CliLogger::GetInstance().HandleCliApiFailed("Parser XML File", cliRes);
	}

	/** Generate output files */
	cliRes = BuildConciseDeviceConfiguration(outputPath);
	if (!cliRes.IsSuccessful())
	{
		CliResult res = CliLogger::GetInstance().HandleCliApiFailed("Build Concise Device Configuration", cliRes);

		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
	}

	cliRes = BuildProcessImageDescriptions(outputPath);
	if (!cliRes.IsSuccessful())
	{
		CliResult res = CliLogger::GetInstance().HandleCliApiFailed("Build Process Image Descriptions", cliRes);

		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
	}

	return CliResult();
}

CliResult ConfigurationGenerator::BuildConciseDeviceConfiguration(const std::string outputPath)
{
	std::vector<std::uint8_t> binOutput;		/** binary output vector */
	std::string configurationOutput = "";		/** configuration output stream */
	Result res;
	CliResult cliRes;

	res = OpenConfiguratorCore::GetInstance().BuildConfiguration(
					OpenConfiguratorCli::GetInstance().networkName,
					configurationOutput,
					binOutput);
	if (!res.IsSuccessful())
	{
		return CliLogger::GetInstance().HandleCoreApiFailed("Build Configuration", res);
	}

	cliRes = CreateMnobdTxt(outputPath, configurationOutput);
	if (!cliRes.IsSuccessful())
	{
		CliResult funRes = CliLogger::GetInstance().HandleCliApiFailed("Create Mnobd Txt", cliRes);

		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, funRes.GetErrorMessage());
	}

	std::ostringstream buffer;
	/** Convert binary values in to a buffer stream */
	for (std::uint32_t index = 0; index < binOutput.size(); index++)
	{
		std::uint8_t value = binOutput.at(index);
		buffer << (value);
	}

	cliRes = CreateMnobdCdc(outputPath, buffer);
	if (!cliRes.IsSuccessful())
	{
		CliResult funRes = CliLogger::GetInstance().HandleCliApiFailed("Create Mnobd Cdc", cliRes);

		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, funRes.GetErrorMessage());
	}

	cliRes = CreateMnobdHexTxt(outputPath, buffer);
	if (!cliRes.IsSuccessful())
	{
		CliResult funRes = CliLogger::GetInstance().HandleCliApiFailed("Create Mnobd Hex Txt", cliRes);

		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, funRes.GetErrorMessage());
	}

	return CliResult();
}

CliResult ConfigurationGenerator::BuildProcessImageDescriptions(const std::string outputPath)
{
	CliResult cliRes;
	std::vector<std::uint8_t> nodeIdCollection;

	Result res = OpenConfiguratorCore::GetInstance().GetAvailableNodeIds(
					OpenConfiguratorCli::GetInstance().networkName,
					nodeIdCollection);
	if (!res.IsSuccessful())
	{
		return CliLogger::GetInstance().HandleCoreApiFailed("Build Process Image Descriptions", res);
	}

	/** Create process image for CN nodes */
    for (std::uint32_t index = 0; index < nodeIdCollection.size(); index++)
	{
		std::uint8_t value = nodeIdCollection.at(index);
		std::string outputPathExtended;

		if (value != MN_DEFAULT_NODE_ID)
		{
			outputPathExtended = outputPath + "/" + std::to_string(value);
			continue;
		}

		cliRes = CreateCProcessImage(value, outputPathExtended);
		if (!cliRes.IsSuccessful())
		{
			CliResult funRes = CliLogger::GetInstance().HandleCliApiFailed("Create CProcess Image", cliRes);

			CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, funRes.GetErrorMessage());
		}

		cliRes = CreateXmlProcessImage(value, outputPathExtended);
		if (!cliRes.IsSuccessful())
		{
			CliResult funRes = CliLogger::GetInstance().HandleCliApiFailed("Create XML Process Image", cliRes);

			CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, funRes.GetErrorMessage());
		}

		cliRes = CreateCSharpProcessImage(value, outputPathExtended);
		if (!cliRes.IsSuccessful())
		{
			CliResult funRes = CliLogger::GetInstance().HandleCliApiFailed("Create CSharp Process Image", cliRes);

			CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, funRes.GetErrorMessage());
		}
	}

    return CliResult();
}

CliResult ConfigurationGenerator::CreateMnobdTxt(const std::string outputPath, 
												 const std::string configuration)
{
	try
	{
		std::string targetPath = outputPath + "/" + kMnobdTxtFileName;
		std::ofstream ofile(targetPath);

		if (ofile.is_open())
		{
			ofile << configuration << std::endl;
			ofile.close();
		}
	}
	catch(std::exception e)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Create Mnobd Txt", e);
	}

	return CliResult();
}

CliResult ConfigurationGenerator::CreateMnobdCdc(const std::string outputPath, 
												 const std::ostringstream& buffer)
{
	try
	{
		std::string targetPath = outputPath + "/" + kMnobdCdcFileName;
		std::ofstream ofile(targetPath);

		if (ofile.is_open())
		{
			ofile << buffer.str() << std::endl;
			ofile.close();
		}
	}
	catch(std::exception ex)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Create Mnobd Cdc", ex);
	}

	return CliResult();
}

CliResult ConfigurationGenerator::CreateMnobdHexTxt(const std::string outputPath, 
													const std::ostringstream& buffer)
{
	try
	{
		const std::uint8_t kWordWrapLength = 16;

		std::ostringstream toStream;
		std::uint8_t lineBreakCount = 0;

		/** Formatting the mnob hex text stream */
		for (std::uint32_t cnt = 0; cnt < buffer.str().size(); ++cnt)
		{
            toStream << "0x";
			toStream << (std::uint8_t) ("%02X" , (buffer.str().at(cnt)));
            if (cnt != (buffer.str().size() - 1))
			{
                toStream << ",";
            }

            lineBreakCount++;

            if (lineBreakCount == kWordWrapLength)
			{
                toStream << "\n";
                lineBreakCount = 0;
            } 
			else
			{
                toStream << " ";
            }
        }

		std::string targetPath = outputPath + "/" + kMnobdHexTxtFileName;
		std::ofstream ofile(targetPath);

		if (ofile.is_open())
		{
			/** Conversion to be added */
			ofile << toStream.str() << std::endl;

			ofile.close();
		}
	}
	catch(std::exception e)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Create Mnobd Hex Txt", e);
	}

	return CliResult();
}


CliResult ConfigurationGenerator::CreateCProcessImage(const std::uint8_t nodeId, 
													  const std::string outputPath)
{
	std::string piDataOutput = "";

	Result res = OpenConfiguratorCore::GetInstance().BuildCProcessImage(
					OpenConfiguratorCli::GetInstance().networkName,
					nodeId, piDataOutput);
	if (!res.IsSuccessful())
	{
		return CliLogger::GetInstance().HandleCoreApiFailed("BuildC Process Image", res);
	}

	try
	{
		std::string targetPath = outputPath + "/" + kXapHFileName;
		std::ofstream ofile(targetPath);

		if (ofile.is_open())
		{
			ofile << piDataOutput << std::endl;
			ofile.close();
		}
	}
	catch(std::exception e)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Create CProcess Image", e);
	}

	return CliResult();
}

CliResult ConfigurationGenerator::CreateXmlProcessImage(const std::uint8_t nodeId, 
														const std::string outputPath)
{
	std::string piDataOutput = "";

	Result res = OpenConfiguratorCore::GetInstance().BuildXMLProcessImage(
					OpenConfiguratorCli::GetInstance().networkName,
					nodeId, piDataOutput);
	if (!res.IsSuccessful())
	{
		return CliLogger::GetInstance().HandleCoreApiFailed("Create XML Process Image", res);
	}

	try
	{
		std::string targetPath = outputPath + "/" + kXapXmlFileName;
		std::ofstream ofile(targetPath);

		if (ofile.is_open())
		{
			ofile << piDataOutput.c_str() << std::endl;
			ofile.close();
		}
	}
	catch(std::exception e)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Create XML Process Image", e);
	}

	return CliResult();
}

CliResult ConfigurationGenerator::CreateCSharpProcessImage(const std::uint8_t nodeId, 
														   const std::string outputPath)
{
	std::string piDataOutput = "";

	Result res = OpenConfiguratorCore::GetInstance().BuildNETProcessImage(
					OpenConfiguratorCli::GetInstance().networkName,
					nodeId, piDataOutput);
	if (!res.IsSuccessful())
	{
		return CliLogger::GetInstance().HandleCoreApiFailed("Create CSharp Process Image", res);
	}

	try
	{
		std::string targetPath = outputPath + "/" + kProcessImageFileName;
		std::ofstream ofile(targetPath);

		if (ofile.is_open())
		{
			ofile << piDataOutput << std::endl;
			ofile.close();
		}
	}
	catch(std::exception e)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Create CSharp Process Image", e);
	}

	return CliResult();
}
