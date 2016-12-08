/**
 * \file ConfigurationGenerator.cpp
 *
 * \brief Implementation of ConfigurationGenerator module
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

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

CLIResult ConfigurationGenerator::GenerateConfigurationFiles(std::string xmlPath, std::string outputPath)
{
	CLIResult cliRes;

	/**< Parse the XML file */
	cliRes = ProjectParser::GetInstance().ParserXMLFile(xmlPath);
	if (!cliRes.IsSuccessful())
	{
		return CLILogger::GetInstance().HandleCliApiFailed("Parser XML File", cliRes);
	}

	/**< Generate output files */
	cliRes = BuildConciseDeviceConfiguration(outputPath);
	if (!cliRes.IsSuccessful())
	{
		CLIResult res = CLILogger::GetInstance().HandleCliApiFailed("Build Concise Device Configuration", cliRes);

		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
	}

	cliRes = BuildProcessImageDescriptions(outputPath);
	if (!cliRes.IsSuccessful())
	{
		CLIResult res = CLILogger::GetInstance().HandleCliApiFailed("Build Process Image Descriptions", cliRes);

		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
	}

	return CLIResult();
}

CLIResult ConfigurationGenerator::BuildConciseDeviceConfiguration(std::string outputPath)
{
	std::vector<std::uint8_t> binOutput;		/**< binary output vector */
	std::string configurationOutput = "";		/**< configuration output stream */

	Result res;
	CLIResult cliRes;

	res = OpenConfiguratorCore::GetInstance().BuildConfiguration(
					OpenConfiguratorCLI::GetInstance().networkName,
					configurationOutput,
					binOutput);
	if (!res.IsSuccessful())
	{
		return CLILogger::GetInstance().HandleCoreApiFailed("Build Configuration", res);
	}

	cliRes = CreateMnobdTxt(outputPath, configurationOutput);
	if (!cliRes.IsSuccessful())
	{
		CLIResult funRes = CLILogger::GetInstance().HandleCliApiFailed("Create Mnobd Txt", cliRes);

		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, funRes.GetErrorMessage());
	}

	std::ostringstream buffer;
	/**< Convert binary values in to a buffer stream */
	for (std::uint32_t index = 0; index < binOutput.size(); index++)
	{
		std::uint8_t value = binOutput.at(index);
		buffer << (value);
	}

	cliRes = CreateMnobdCdc(outputPath, buffer);
	if (!cliRes.IsSuccessful())
	{
		CLIResult funRes = CLILogger::GetInstance().HandleCliApiFailed("Create Mnobd Cdc", cliRes);

		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, funRes.GetErrorMessage());
	}

	cliRes = CreateMnobdHexTxt(outputPath, buffer);
	if (!cliRes.IsSuccessful())
	{
		CLIResult funRes = CLILogger::GetInstance().HandleCliApiFailed("Create Mnobd Hex Txt", cliRes);

		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, funRes.GetErrorMessage());
	}

	return CLIResult();
}

CLIResult ConfigurationGenerator::BuildProcessImageDescriptions(std::string outputPath)
{
	CLIResult cliRes;

	std::vector<std::uint8_t> nodeIdCollection;

	Result res = OpenConfiguratorCore::GetInstance().GetAvailableNodeIds(
					OpenConfiguratorCLI::GetInstance().networkName,
					nodeIdCollection);
	if (!res.IsSuccessful())
	{
		return CLILogger::GetInstance().HandleCoreApiFailed("Build Process Image Descriptions", res);
	}

	/**< Create process image for CN nodes */
    for (std::uint32_t index = 0; index < nodeIdCollection.size(); index++)
	{
		std::uint8_t value = nodeIdCollection.at(index);

		if (value != MN_DEFAULT_NODE_ID)
		{
			// resolve to be done
			continue;
		}

		cliRes = CreateCProcessImage(value, outputPath);
		if (!cliRes.IsSuccessful())
		{
			CLIResult funRes = CLILogger::GetInstance().HandleCliApiFailed("Create CProcess Image", cliRes);

			CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, funRes.GetErrorMessage());
		}

		cliRes = CreateXMLProcessImage(value, outputPath);
		if (!cliRes.IsSuccessful())
		{
			CLIResult funRes = CLILogger::GetInstance().HandleCliApiFailed("Create XML Process Image", cliRes);

			CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, funRes.GetErrorMessage());
		}

		cliRes = CreateCSharpProcessImage(value, outputPath);
		if (!cliRes.IsSuccessful())
		{
			CLIResult funRes = CLILogger::GetInstance().HandleCliApiFailed("Create CSharp Process Image", cliRes);

			CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, funRes.GetErrorMessage());
		}
	}

    return CLIResult();
}

CLIResult ConfigurationGenerator::CreateMnobdTxt(std::string outputPath, std::string configuration)
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
	catch(std::exception ex)
	{
		return CLILogger::GetInstance().HandleExceptionCaught("Create Mnobd Txt", ex);
	}

	return CLIResult();
}

CLIResult ConfigurationGenerator::CreateMnobdCdc(std::string outputPath, std::ostringstream& buffer)
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
		return CLILogger::GetInstance().HandleExceptionCaught("Create Mnobd Cdc", ex);
	}

	return CLIResult();
}

CLIResult ConfigurationGenerator::CreateMnobdHexTxt(std::string outputPath, std::ostringstream& buffer)
{
	try
	{
		const std::uint8_t kWordWrapLength = 16;

		std::ostringstream toStream;
		std::uint8_t lineBreakCount = 0;

		/**< Formatting the mnob hex text stream */
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
			// Conversion to be done
			ofile << toStream.str() << std::endl;

			ofile.close();
		}
	}
	catch(std::exception ex)
	{
		return CLILogger::GetInstance().HandleExceptionCaught("Create Mnobd Hex Txt", ex);
	}

	return CLIResult();
}


CLIResult ConfigurationGenerator::CreateCProcessImage(std::uint8_t nodeId, std::string outputPath)
{
	std::string piDataOutput = "";

	Result res = OpenConfiguratorCore::GetInstance().BuildCProcessImage(
					OpenConfiguratorCLI::GetInstance().networkName,
					nodeId, piDataOutput);
	if (!res.IsSuccessful())
	{
		return CLILogger::GetInstance().HandleCoreApiFailed("BuildC Process Image", res);
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
	catch(std::exception ex)
	{
		return CLILogger::GetInstance().HandleExceptionCaught("Create CProcess Image", ex);
	}

	return CLIResult();
}

CLIResult ConfigurationGenerator::CreateXMLProcessImage(std::uint8_t nodeId, std::string outputPath)
{
	std::string piDataOutput = "";

	Result res = OpenConfiguratorCore::GetInstance().BuildXMLProcessImage(
					OpenConfiguratorCLI::GetInstance().networkName,
					nodeId, piDataOutput);
	if (!res.IsSuccessful())
	{
		return CLILogger::GetInstance().HandleCoreApiFailed("Create XML Process Image", res);
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
	catch(std::exception ex)
	{
		return CLILogger::GetInstance().HandleExceptionCaught("Create XML Process Image", ex);
	}

	return CLIResult();
}

CLIResult ConfigurationGenerator::CreateCSharpProcessImage(std::uint8_t nodeId, std::string outputPath)
{
	std::string piDataOutput = "";

	Result res = OpenConfiguratorCore::GetInstance().BuildNETProcessImage(
					OpenConfiguratorCLI::GetInstance().networkName,
					nodeId, piDataOutput);
	if (!res.IsSuccessful())
	{
		return CLILogger::GetInstance().HandleCoreApiFailed("Create CSharp Process Image", res);
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
	catch(std::exception ex)
	{
		return CLILogger::GetInstance().HandleExceptionCaught("Create CSharp Process Image", ex);
	}

	return CLIResult();
}
