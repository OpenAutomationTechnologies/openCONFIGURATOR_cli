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

bool ConfigurationGenerator::BuildConciseDeviceConfiguration(std::string outputPath){
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
		if (!boost::filesystem::exists(outputPath))
			{
				boost::filesystem::path dir(outputPath);
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

		std::ostringstream buffer;
		
		 for (int i = 0; i < binOutput.size(); i++) {
			 std::uint8_t value = binOutput.at(i);
			buffer << (value);
		 }

	retVal = createMnobdCdc(outputPath,buffer);
        if (!retVal) {
            return retVal;
        }

      retVal = createMnobdHexTxt(outputPath,buffer);
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
		std::string targetPath= outputPath+"/mnobd.txt";
		std::ofstream file(targetPath);
		file << configuration << std::endl;
	}
	catch(std::exception exce)
	{ 
		exce.what();
	}
	return true;
}

bool ConfigurationGenerator::createMnobdCdc(std::string outputPath,std::ostringstream& buffer)
{
	try
	{
		LOG_INFO()<< "Entered createMnobdCdc methos...........";
		std::string targetPath= outputPath+"/mnobd.cdc";
		std::ofstream file(targetPath);
		file << buffer.str() << std::endl;
	}
	catch(std::exception exce)
	{ 
		exce.what();
	}
	return true;
}

bool ConfigurationGenerator::createMnobdHexTxt(std::string outputPath,std::ostringstream& buffer)
{
	try
	{
		LOG_INFO()<< "Entered createMnobHexTxt methos...........";

		std::ostringstream sb;

		

		LOG_INFO() << "Created string stream";
		
		short lineBreakCount = 0;
		for (int cnt = 0; cnt < buffer.str().size(); ++cnt) {
            sb << "0x"; 
			
			sb <<(std::uint8_t) ("%02X" , (buffer.str().at(cnt))); 
            if (cnt != (buffer.str().size() - 1)) {
                sb << ",";
            }
            lineBreakCount++;

            if (lineBreakCount == 16) {
                sb << "\n";
                lineBreakCount = 0;
            } else {
                sb << " " ;
            }
        }

		std::string targetPath= outputPath+"/mnobd_char.txt";
		std::ofstream file(targetPath);
		file << sb.str() << std::endl;
	}
	catch(std::exception exce)
	{ 
		exce.what();
	}
	return true;
}



