#include "SceneLoader.h"

#include "Scene.h"
#include "Portal.h"

#include "SceneGraphicsManager.h"

#include "boost/lexical_cast.hpp"


//XML scene file string constants
static const char SCENE_STRING[] = "scene";
static const char AMBIENT_RED_STRING[] = "ambient_r";
static const char AMBIENT_GREEN_STRING[] = "ambient_g";
static const char AMBIENT_BLUE_STRING[] = "ambient_b";
static const char NORTH_STRING[] = "north";
static const char ARCHITECTURE_STRING[] = "architecture";
static const char ID_STRING[] = "id";
static const char LIGHT_STRING[] = "light";
static const char CAST_SHADOWS_STRING[] = "cast_shadows";
static const char RANGE_STRING[] = "range";
static const char ITEM_STRING[] = "item";
static const char CREATURE_STRING[] = "creature";
static const char PORTAL_STRING[] = "portal";
static const char TARGET_SCENE_ID_STRING[] = "target_scene_id";
static const char TARGET_PORTAL_ID_STRING[] = "target_portal_id";
static const char LOOK_AT_X_STRING[] = "ltx";
static const char LOOK_AT_Y_STRING[] = "lty";
static const char LOOK_AT_Z_STRING[] = "ltz";
static const char PARTICLE_STRING[] = "particle";
static const char NAME_STRING[] = "name";
static const char TEMPLATE_NAME_STRING[] = "template_name";

static const char TRANSLATION_X_STRING[] = "tx";
static const char TRANSLATION_Y_STRING[] = "ty";
static const char TRANSLATION_Z_STRING[] = "tz";
static const char ROTATION_X_STRING[] = "rx";
static const char ROTATION_Y_STRING[] = "ry";
static const char ROTATION_Z_STRING[] = "rz";
static const char SCALE_X_STRING[] = "sx";
static const char SCALE_Y_STRING[] = "sy";
static const char SCALE_Z_STRING[] = "sz";

//-------------------------------------------------------------------------------------
void SceneLoader::load(std::string filename, Scene* scene)
{
  try
  {
    Ogre::FileInfoListPtr files = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceFileInfo("Essential", filename);
    if(files->size() < 1) throw NHException("could not find the path to the specified scene");

    std::string filePath = files->front().archive->getName() + "/" + files->front().filename;

    std::ifstream streamfile(filePath);
    rapidxml::xml_document<> doc;

    std::vector<char> buffer((std::istreambuf_iterator<char>(streamfile)), std::istreambuf_iterator<char>());//streamfile)), std::istreambuf_iterator<char>());

    buffer.push_back('\0');//null terminating the buffer

    //std::cout << &buffer[0] << std::endl; //test the buffer

    doc.parse<0>(&buffer[0]);
    rapidxml::xml_node<>* root = doc.first_node(SCENE_STRING);//"scene");

    scene->getSceneGraphicsManager()->setAmbientLight(getXMLColour(root, AMBIENT_RED_STRING, AMBIENT_GREEN_STRING, AMBIENT_BLUE_STRING));

    //description attributes
    //north = boost::lexical_cast<float>(root->first_attribute(NORTH_STRING)->value());

    //Architecture
    rapidxml::xml_node<>* architectureNode = root->first_node(ARCHITECTURE_STRING);//"architecture");
    while(architectureNode != NULL)
    {
      int id = boost::lexical_cast<int>(architectureNode->first_attribute(ID_STRING)->value());
      try
      {
        scene->addArchitecture(id, getXMLPosition(architectureNode), getXMLRotation(architectureNode), getXMLScale(architectureNode));
      }
      catch(NHException e) 
      {
#ifdef _DEBUG
        std::cout << e.what() << std::endl;
#endif
      }
      
      architectureNode = architectureNode->next_sibling(ARCHITECTURE_STRING);
    }

    scene->build();//building static geometry - must do here else we can't use the navigation mesh for creature placement

    //Lights
    rapidxml::xml_node<>* lightNode = root->first_node(LIGHT_STRING);//"light");
    while(lightNode != NULL)
    {
      bool cast_shadows = boost::lexical_cast<bool>(lightNode->first_attribute(CAST_SHADOWS_STRING)->value());
      float range = boost::lexical_cast<float>(lightNode->first_attribute(RANGE_STRING)->value());
      Ogre::ColourValue colour = getXMLColour(lightNode);
      scene->addLight(-1, getXMLPosition(lightNode),cast_shadows,range);//TODO: use actual id rather than -1
      lightNode = lightNode->next_sibling(LIGHT_STRING);
    }

    //Items
    rapidxml::xml_node<>* itemNode = root->first_node(ITEM_STRING);
    while(itemNode != NULL)
    {
      int id = boost::lexical_cast<int>(itemNode->first_attribute(ID_STRING)->value());
      scene->addItem(id, getXMLPosition(itemNode), getXMLRotation(itemNode));
      itemNode = itemNode->next_sibling(ITEM_STRING);
    }

    //Creatures
    rapidxml::xml_node<>* creatureNode = root->first_node(CREATURE_STRING);
    while(creatureNode != NULL)
    {
      int id = boost::lexical_cast<int>(creatureNode->first_attribute(ID_STRING)->value());
      scene->addCreature(id, getXMLPosition(creatureNode));
      creatureNode = creatureNode->next_sibling(CREATURE_STRING);
    }

    //Portals
    rapidxml::xml_node<>* portalNode = root->first_node(PORTAL_STRING);
    while(portalNode != NULL)
    {
      int id = boost::lexical_cast<int>(portalNode->first_attribute(ID_STRING)->value());
      int targetSceneID = boost::lexical_cast<int>(portalNode->first_attribute(TARGET_SCENE_ID_STRING)->value());
      int targetPortalID = boost::lexical_cast<int>(portalNode->first_attribute(TARGET_PORTAL_ID_STRING)->value());
      scene->addPortal(id, getXMLPosition(portalNode), getXMLVector(portalNode, LOOK_AT_X_STRING, LOOK_AT_Y_STRING, LOOK_AT_Z_STRING), Id<Scene>(targetSceneID), Id<Portal>(targetPortalID), Id<Portal>(id));
      portalNode = portalNode->next_sibling(PORTAL_STRING);//"portal");
    }

    //Particles //TODO: change to emitter
    /*
    rapidxml::xml_node<>* particleNode = root->first_node(PARTICLE_STRING);//"particle");
    while(particleNode != NULL)
    {
      scene->addParticles(particleNode->first_attribute(NAME_STRING)->value(), particleNode->first_attribute(TEMPLATE_NAME_STRING)->value(),getXMLPosition(particleNode));
      particleNode = particleNode->next_sibling(PARTICLE_STRING);
    }
    */
  }
  catch (rapidxml::parse_error e)
  {
    std::stringstream ss;
    ss << "could not load the xml scene '" << filename << "': " << e.what() << std::endl;
    throw NHException(ss.str().c_str());
  }
}

//-------------------------------------------------------------------------------------
Vector3 SceneLoader::getXMLVector(rapidxml::xml_node<>* node, std::string first, std::string second, std::string third)
{
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;

  rapidxml::xml_attribute<>* xPosition = node->first_attribute(first.c_str());
  rapidxml::xml_attribute<>* yPosition = node->first_attribute(second.c_str());
  rapidxml::xml_attribute<>* zPosition = node->first_attribute(third.c_str());

  if(xPosition) x = boost::lexical_cast<float>(xPosition->value());
  if(yPosition) y = boost::lexical_cast<float>(yPosition->value());
  if(zPosition) z = boost::lexical_cast<float>(zPosition->value());

  return Vector3(x,y,z);
}

//-------------------------------------------------------------------------------------
Quaternion SceneLoader::getXMLRotation(rapidxml::xml_node<>* node)
{
  Quaternion rotation = Quaternion::IDENTITY;
  Vector3 components = getXMLVector(node, ROTATION_X_STRING, ROTATION_Y_STRING, ROTATION_Z_STRING);

  rotation.FromAngleAxis(Ogre::Degree(components.x), Vector3::UNIT_X);
  rotation.FromAngleAxis(Ogre::Degree(components.y), Vector3::UNIT_Y);
  rotation.FromAngleAxis(Ogre::Degree(components.z), Vector3::UNIT_Z);

  return rotation;
}

//-------------------------------------------------------------------------------------
Ogre::ColourValue SceneLoader::getXMLColour(rapidxml::xml_node<>* node, std::string first, std::string second, std::string third, std::string fourth)
{
  float a = 1.0f;
  rapidxml::xml_attribute<>* aColour = node->first_attribute(fourth.c_str());  
  if(aColour) a = boost::lexical_cast<float>(aColour->value());

  Vector3 components = getXMLVector(node, first, second, third);

  return Ogre::ColourValue(components.x,components.y,components.z,a);
}

//-------------------------------------------------------------------------------------
Vector3 SceneLoader::getXMLScale(rapidxml::xml_node<>* node)
{
  return getXMLVector(node, SCALE_X_STRING, SCALE_Y_STRING, SCALE_Z_STRING);//"sx", "sy", "sz");
}

//-------------------------------------------------------------------------------------
Vector3 SceneLoader::getXMLPosition(rapidxml::xml_node<>* node)
{
  return getXMLVector(node, TRANSLATION_X_STRING, TRANSLATION_Y_STRING, TRANSLATION_Z_STRING);//"tx", "ty", "tz");
}