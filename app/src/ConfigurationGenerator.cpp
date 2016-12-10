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
// REVIEW_COMMENT:
}

ConfigurationGenerator::~ConfigurationGenerator()
{
// REVIEW_COMMENT:
}

ConfigurationGenerator& ConfigurationGenerator::GetInstance()
{
	static ConfigurationGenerator instance;// REVIEW_COMMENT:
	return instance;
}

CliResult ConfigurationGenerator::GenerateConfigurationFiles(std::string xmlPath, std::string outputPath)
{
	CliResult cliRes;

	/** Parse the XML file */
	cliRes = ProjectParser::GetInstance().ParserXMLFile(xmlPath);
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

CliResult ConfigurationGenerator::BuildConciseDeviceConfiguration(std::string outputPath)
{
	std::vector<std::uint8_t> binOutput;		/** binary output vector */
	std::string configurationOutput = "";		/** configuration output stream */
// REVIEW_COMMENT:
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

CliResult ConfigurationGenerator::BuildProcessImageDescriptions(std::string outputPath)
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

		if (value != MN_DEFAULT_NODE_ID)
		{
			// resolve to be done
			continue;
		}

		cliRes = CreateCProcessImage(value, outputPath);
		if (!cliRes.IsSuccessful())
		{
			CliResult funRes = CliLogger::GetInstance().HandleCliApiFailed("Create CProcess Image", cliRes);

			CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, funRes.GetErrorMessage());
		}

		cliRes = CreateXMLProcessImage(value, outputPath);
		if (!cliRes.IsSuccessful())
		{
			CliResult funRes = CliLogger::GetInstance().HandleCliApiFailed("Create XML Process Image", cliRes);

			CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, funRes.GetErrorMessage());
		}

		cliRes = CreateCSharpProcessImage(value, outputPath);
		if (!cliRes.IsSuccessful())
		{
			CliResult funRes = CliLogger::GetInstance().HandleCliApiFailed("Create CSharp Process Image", cliRes);

			CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, funRes.GetErrorMessage());
		}
	}

    return CliResult();
}

CliResult ConfigurationGenerator::CreateMnobdTxt(std::string outputPath, std::string configuration)
{
	try
	{
		std::string targetPath = outputPath + "/" + kMnobdTxtFileName;
		std::ofstream ofile(targetPath);

		if (ofile.is_open())
		{
			ofile << configuration << std::endl;
// REVIEW_COMMENT:
			ofile.close();
		}
	}
	catch(std::exception ex)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Create Mnobd Txt", ex);
	}

	return CliResult();
}

CliResult ConfigurationGenerator::CreateMnobdCdc(std::string outputPath, std::ostringstream& buffer)
{
	try
	{
		std::string targetPath = outputPath + "/" + kMnobdCdcFileName;
		std::ofstream ofile(targetPath);

		if (ofile.is_open())
		{
			ofile << buffer.str() << std::endl;
// REVIEW_COMMENT:
			ofile.close();
		}
	}
	catch(std::exception ex)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Create Mnobd Cdc", ex);
	}

	return CliResult();
}

CliResult ConfigurationGenerator::CreateMnobdHexTxt(std::string outputPath, std::ostringstream& buffer)
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
// REVIEW_COMMENT:
			toStream << (std::uint8_t) ("%02X" , (buffer.str().at(cnt)));
            if (cnt != (buffer.str().size() - 1))
			{
                toStream << ",";
            }// REVIEW_COMMENT:
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
		return CliLogger::GetInstance().HandleExceptionCaught("Create Mnobd Hex Txt", ex);
	}

	return CliResult();
}


CliResult ConfigurationGenerator::CreateCProcessImage(std::uint8_t nodeId, std::string outputPath)
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
// REVIEW_COMMENT:
			ofile.close();
		}
	}
	catch(std::exception ex)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Create CProcess Image", ex);
	}

	return CliResult();
}

CliResult ConfigurationGenerator::CreateXMLProcessImage(std::uint8_t nodeId, std::string outputPath)
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
// REVIEW_COMMENT:
			ofile.close();
		}
	}
	catch(std::exception ex)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Create XML Process Image", ex);
	}

	return CliResult();
}

CliResult ConfigurationGenerator::CreateCSharpProcessImage(std::uint8_t nodeId, std::string outputPath)
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
// REVIEW_COMMENT:
			ofile.close();
		}
	}
	catch(std::exception ex)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Create CSharp Process Image", ex);
	}

	return CliResult();
}
