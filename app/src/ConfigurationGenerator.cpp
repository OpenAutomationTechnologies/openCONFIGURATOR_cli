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

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "OpenConfiguratorCore.h"
#include "OpenConfiguratorCLI.h"
#include "ConfigurationGenerator.h"

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

bool ConfigurationGenerator::BuildConciseDeviceConfiguration(std::string networkId,std::string outputPath){
	LOG_INFO()<< "Entered Build configuration....";
	std::vector<std::uint8_t> binOutput;
	std::string configurationOutput="";
	Result res = OpenConfiguratorCore::GetInstance().BuildConfiguration(OpenConfiguratorCLI::GetInstance().GetNetworkName(),configurationOutput,binOutput);
	LOG_INFO()<< "Entered Build core library build function....";
	
	LOG_INFO()<< res.IsSuccessful();
	if (!res.IsSuccessful())
	{
		LOG_ERROR() << res.GetErrorMessage();
		//return SUCCESS;
	}

	try
	{
		if (!boost::filesystem::exists("C:/OCP_Projects_Xerces/Xerces_4CN/output"))
			{
				boost::filesystem::path dir("C:/OCP_Projects_Xerces/Xerces_4CN/output");
				boost::filesystem::create_directory(dir);
			}
	}
	catch (std::exception exc)
	{
		exc.what();
	}
	bool retVal = createMnobdTxt(outputPath, configurationOutput);
        if (!retVal) {
            return retVal;
        }

	retVal = createMnobdCdc(outputPath,configurationOutput);
        if (!retVal) {
            return retVal;
        }

      retVal = createMnobdHexTxt(outputPath,configurationOutput);
        if (!retVal) {
            return retVal;
        }
	return true;
}

bool ConfigurationGenerator::createMnobdTxt(std::string outputPath,std::string configuration)
{
	try
	{
		LOG_INFO()<< "Entered createMnobdtxt methos...........";
		std::string targetPath= "C:/OCP_Projects_Xerces/Xerces_4CN/output/mnobd.txt";
		std::ofstream file(targetPath);
		file << configuration << std::endl;
	}
	catch(std::exception exce)
	{ 
		exce.what();
	}
	return true;
}

bool ConfigurationGenerator::createMnobdCdc(std::string outputPath,std::string configuration)
{
	try
	{
		LOG_INFO()<< "Entered createMnobdCdc methos...........";
		std::string targetPath= "C:/OCP_Projects_Xerces/Xerces_4CN/output/mnobd.cdc";
		std::ofstream file(targetPath);
		file << configuration << std::endl;
	}
	catch(std::exception exce)
	{ 
		exce.what();
	}
	return true;
}

bool ConfigurationGenerator::createMnobdHexTxt(std::string outputPath,std::string configuration)
{
	try
	{
		LOG_INFO()<< "Entered createMnobHexTxt methos...........";
		std::string targetPath= "C:/OCP_Projects_Xerces/Xerces_4CN/output/mnobd_char.txt";
		std::ofstream file(targetPath);
		file << configuration << std::endl;
	}
	catch(std::exception exce)
	{ 
		exce.what();
	}
	return true;
}



