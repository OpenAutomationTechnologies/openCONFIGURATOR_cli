## <center>openCONFIGURATOR Command Line Application
## <center>User Manual 

#### About:
```
openCONFIGURATOR command line application is a stand-alone console application of Automation Studio to generate the POWERLINK configuration files.
```
#### Usage:
```
openCONFIGURATOR.exe [options]
options:
 -p,--project <ProjectFile>      Project XML file.
 -o,--output <OutputPath>        Output path for generated files.
 -de,--german                    German log messages. Default is English.
 -d,--debug                      Log on file. Default logging is on console.
 -h,--help                       Help.
 
 ProjectFile and OutputPath and mandatory parameters.
```
#### Example:
```
openCONFIGURATOR.exe -p C:\FolderName\openCONFIGURATOR.xml -o C:\FolderName\output -d
```
#### Pre-requisites:
```
1. openCONFIGURATOR project input files
2. Output path with write permissions
3. Application files
	│   boost_chrono-vc110-mt-gd-1_58.dll
	│   boost_date_time-vc110-mt-gd-1_58.dll
	│   boost_filesystem-vc110-mt-gd-1_58.dll
	│   boost_log-vc110-mt-gd-1_58.dll
	│   boost_log_settings.ini
	│   boost_log_setup-vc110-mt-gd-1_58.dll
	│   boost_regex-vc110-mt-gd-1_58.dll
	│   boost_system-vc110-mt-gd-1_58.dll
	│   boost_thread-vc110-mt-gd-1_58.dll
	│   openCONFIGURATOR.exe
	│   openconfigurator_core_lib.dll
	│   xerces-c_3_1D.dll
	│
	├───logs
	└───resources
		│   error_codes.xml
		│
		├───ErrorCodesXMLSchema
		│       ocErrorCodes.xsd
		│
		├───OC_ProjectFile
		│       openCONFIGURATOR.xsd
		│
		├───xddschema
		│       CommonElements.xsd
		│       CommonElements_Modular.xsd
		│       ISO15745ProfileContainer.xsd
		│       Powerlink_Main.xsd
		│       ProfileBody_CommunicationNetwork_Powerlink.xsd
		│       ProfileBody_CommunicationNetwork_Powerlink_Modular_Child.xsd
		│       ProfileBody_CommunicationNetwork_Powerlink_Modular_Head.xsd
		│       ProfileBody_Device_Powerlink.xsd
		│       ProfileBody_Device_Powerlink_Modular_Child.xsd
		│       ProfileBody_Device_Powerlink_Modular_Head.xsd
		│       TextResource.xsd
		│
		└───XDD_ImportExport_Config
				POWERLINK_Types.xsd
```
#### General Guidelines:
```
1. The application folder and subfolders shall have write permissions
2. In absents of option '-p' and '--project', the first parameter will be considered as project file
3. '-p <ProjectFile>' and '-o <OutputPath>' are mandatory parameters
4. '-d' and '-de' are optional parameters
5. It is prefered to not to edit any files manually
```
