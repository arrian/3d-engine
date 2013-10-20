#include "InitialisationParser.h"

//Ogre
#include "Ogre.h"

//Boost
#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>

//Local
#include "NHException.h"


InitialisationParser::InitialisationParser(std::string filename)
{
  try
  {
    Ogre::FileInfoListPtr fileListPtr = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceFileInfo("Essential", filename);
    if(fileListPtr->size() < 1) throw NHException("could not find the path to the specified initialisation file");

    boost::property_tree::ini_parser::read_ini(fileListPtr->front().archive->getName() + "/" + fileListPtr->front().filename, pt);
  }
  catch(boost::property_tree::ini_parser::ini_parser_error e)//need to provide default values
  {
    throw NHException("could not parse the initialisation file");
  }
  catch(boost::property_tree::ptree_bad_path e)
  {
    throw NHException("the initialisation file is malformed");
  }
}

//-------------------------------------------------------------------------------------
InitialisationParser::~InitialisationParser(void)
{
}

//-------------------------------------------------------------------------------------
template<> std::string InitialisationParser::get<std::string>(std::string label)
{
  try
  {
    return pt.get<std::string>(label);
  }
  catch(boost::property_tree::ini_parser::ini_parser_error e)//need to provide default values
  {
    std::cerr << "failed to parse the ini file for the label " << label << std::endl;
  }
  catch(boost::property_tree::ptree_bad_path e)
  {
    std::cerr << "failed to parse the ini file for the label " << label << std::endl;
  }
  return "";
}

//-------------------------------------------------------------------------------------
template<> int InitialisationParser::get<int>(std::string label)
{
  try
  {
    return pt.get<int>(label);
  }
  catch(boost::property_tree::ini_parser::ini_parser_error e)//need to provide default values
  {
    std::cerr << "failed to parse the ini file for the label " << label << std::endl;
  }
  catch(boost::property_tree::ptree_bad_path e)
  {
    std::cerr << "failed to parse the ini file for the label " << label << std::endl;
  }
  return 0;
}

//-------------------------------------------------------------------------------------
template<> bool InitialisationParser::get<bool>(std::string label)
{
  return (get<std::string>(label) == "true");
}
