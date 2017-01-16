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
#include "ParameterValidator.h"
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

CliResult ConfigurationGenerator::GenerateOutputFiles(const std::string& xmlPath,
        const std::string& outputPath)
{
	CliResult cliRes;

	/** Validate the parameters */
	cliRes = ParameterValidator::GetInstance().IsXmlFileValid(xmlPath);
	if (!cliRes.IsSuccessful())
	{
		return cliRes;
	}

	cliRes = ParameterValidator::GetInstance().IsPathValid(outputPath);
	if (!cliRes.IsSuccessful())
	{
		LOG_WARN() << cliRes.GetErrorMessage();

		try
		{
			/** Create the output path as it doesnt exists */
			boost::filesystem::path dir(outputPath);
			if (!boost::filesystem::create_directory(dir))
			{
				/** Failed to create the output path */
				return cliRes;
			}
		}
		catch (const std::exception& e)
		{
			return CliLogger::GetInstance().GetFailureErrorString(e);
		}
	}

	/** Parse the XML file */
	cliRes = ProjectParser::GetInstance().ParseXmlFile(xmlPath);
	if (!cliRes.IsSuccessful())
	{
		return cliRes;
	}

	/** Generate output files */
	cliRes = BuildConciseDeviceConfiguration(outputPath);
	if (!cliRes.IsSuccessful())
	{
		return cliRes;
	}

	cliRes = BuildProcessImageDescriptions(outputPath);
	if (!cliRes.IsSuccessful())
	{
		return cliRes;
	}

	return CliResult();
}

CliResult ConfigurationGenerator::BuildConciseDeviceConfiguration(const std::string& outputPath)
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
		return CliLogger::GetInstance().GetFailureErrorString(res);
	}

	cliRes = CreateMnobdTxt(outputPath, configurationOutput);
	if (!cliRes.IsSuccessful())
	{
		LOG_ERROR() << CliLogger::GetInstance().GetErrorString(cliRes);

		return CliResult(CliErrorCode::FAILURE);
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
		return cliRes;
	}

	cliRes = CreateMnobdHexTxt(outputPath, buffer);
	if (!cliRes.IsSuccessful())
	{
		return cliRes;
	}

	return CliResult();
}

CliResult ConfigurationGenerator::BuildProcessImageDescriptions(const std::string& outputPath)
{
	CliResult cliRes;
	std::vector<std::uint8_t> nodeIdCollection;

	Result res = OpenConfiguratorCore::GetInstance().GetAvailableNodeIds(
	                 OpenConfiguratorCli::GetInstance().networkName,
	                 nodeIdCollection);
	if (!res.IsSuccessful())
	{
		return CliLogger::GetInstance().GetFailureErrorString(res);
	}

	/** Create process image for CN nodes */
	for (std::uint32_t index = 0; index < nodeIdCollection.size(); index++)
	{
		std::uint8_t value = nodeIdCollection.at(index);
		std::string outputPathExtended = outputPath;

		if (value != MN_DEFAULT_NODE_ID)
		{
			outputPathExtended = outputPath + kPathSeparator + std::to_string(value);

			continue;
		}

		cliRes = CreateCProcessImage(value, outputPathExtended);
		if (!cliRes.IsSuccessful())
		{
			return cliRes;
		}

		cliRes = CreateXmlProcessImage(value, outputPathExtended);
		if (!cliRes.IsSuccessful())
		{
			return cliRes;
		}

		cliRes = CreateCSharpProcessImage(value, outputPathExtended);
		if (!cliRes.IsSuccessful())
		{
			return cliRes;
		}
	}

	return CliResult();
}

CliResult ConfigurationGenerator::CreateMnobdTxt(const std::string& outputPath,
        const std::string& configuration)
{
	try
	{
		std::string targetPath = outputPath + kPathSeparator + kMnobdTxtFileName;
		std::ofstream ofile(targetPath);

		if (ofile.is_open())
		{
			ofile << configuration << std::endl;
			ofile.close();
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}

CliResult ConfigurationGenerator::CreateMnobdCdc(const std::string& outputPath,
        const std::ostringstream& buffer)
{
	try
	{
		std::string targetPath = outputPath + kPathSeparator + kMnobdCdcFileName;
		std::ofstream ofile(targetPath);

		if (ofile.is_open())
		{
			ofile << buffer.str() << std::endl;
			ofile.close();
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}

CliResult ConfigurationGenerator::CreateMnobdHexTxt(const std::string& outputPath,
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
			std::ostringstream hexVal;

			hexVal << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << std::int16_t(buffer.str().at(cnt));
			toStream << "0x";
			if (hexVal.str().size() > 2)
			{
				toStream << hexVal.str().substr(2, 2);
			}
			else
			{
				toStream << hexVal.str();
			}

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

		std::string targetPath = outputPath + kPathSeparator + kMnobdHexTxtFileName;
		std::ofstream ofile(targetPath);

		if (ofile.is_open())
		{
			/** Conversion to be added */
			ofile << toStream.str() << std::endl;

			ofile.close();
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}


CliResult ConfigurationGenerator::CreateCProcessImage(const std::uint8_t nodeId,
        const std::string& outputPath)
{
	std::string piDataOutput = "";

	Result res = OpenConfiguratorCore::GetInstance().BuildCProcessImage(
	                 OpenConfiguratorCli::GetInstance().networkName,
	                 nodeId, piDataOutput);
	if (!res.IsSuccessful())
	{
		return CliLogger::GetInstance().GetFailureErrorString(res);
	}

	try
	{
		std::string targetPath = outputPath + kPathSeparator + kXapHFileName;
		std::ofstream ofile(targetPath);

		if (ofile.is_open())
		{
			ofile << piDataOutput << std::endl;
			ofile.close();
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}

CliResult ConfigurationGenerator::CreateXmlProcessImage(const std::uint8_t nodeId,
        const std::string& outputPath)
{
	std::string piDataOutput = "";

	Result res = OpenConfiguratorCore::GetInstance().BuildXMLProcessImage(
	                 OpenConfiguratorCli::GetInstance().networkName,
	                 nodeId, piDataOutput);
	if (!res.IsSuccessful())
	{
		return CliLogger::GetInstance().GetFailureErrorString(res);
	}

	try
	{
		std::string targetPath = outputPath + kPathSeparator + kXapXmlFileName;
		std::ofstream ofile(targetPath);

		if (ofile.is_open())
		{
			ofile << piDataOutput << std::endl;
			ofile.close();
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}

CliResult ConfigurationGenerator::CreateCSharpProcessImage(const std::uint8_t nodeId,
        const std::string& outputPath)
{
	std::string piDataOutput = "";

	Result res = OpenConfiguratorCore::GetInstance().BuildNETProcessImage(
	                 OpenConfiguratorCli::GetInstance().networkName,
	                 nodeId, piDataOutput);
	if (!res.IsSuccessful())
	{
		return CliLogger::GetInstance().GetFailureErrorString(res);
	}

	try
	{
		std::string targetPath = outputPath + kPathSeparator + kProcessImageFileName;
		std::ofstream ofile(targetPath);

		if (ofile.is_open())
		{
			ofile << piDataOutput << std::endl;
			ofile.close();
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}
