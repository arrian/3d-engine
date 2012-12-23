#pragma once

#include <vector>
#include <string>

#include <OgreRenderWindow.h>

class World;

typedef std::vector<std::string> Options;

/**
* Defines a command that can be run.
*/
template<typename Owner>
class Command
{
public:
  Command(std::string arguments, std::string help, void (Owner::*Run) (Options), Owner* owner)
    : args(arguments),
      help(help),
      Run(Run),
      owner(owner)
  {
  }

  ~Command() {}

  std::string args;//arguments
  std::string help;//help information about the command

  /**
   * Runs the command on the given owner.
   */
  void run(Options options)
  {
    if(Run != 0)
    {
      (owner->*Run)(options);
    }
  }
private:
  void (Owner::*Run) (Options);//function pointer to the command that needs to be run
  Owner* owner;//the owner of the bound run method
};

class ScriptManager
{
public:
  ScriptManager(void);
  virtual ~ScriptManager(void);

  void setWorld(World* world);

  void execute(std::string command);//executes a string of commands

  bool update(double elapsedSeconds);

  void run(std::string file);//executes the specified file

private:
  World* world;
  bool done;

  bool outputStandardOut;
  bool outputGameConsole;

  std::map<std::string, Command<ScriptManager>*> commands;//list of all possible commands

  bool stringToBool(std::string string);
  void split(const std::string &s, char delim, std::vector<std::string> &elems);//Tokenises a string by the given delimiter.
  void display(std::string comment);//displays a line of text
  void display(std::string highlight, std::string comment);//displays a highlighted section of text followed by a normal section
  void addCommand(std::string name, std::string arguments, std::string help, void (ScriptManager::*run) (Options));

  //Set of executable commands
  void clear                     (Options);
  void refresh                   (Options);
  void about                     (Options);
  void help                      (Options);
  void exit                      (Options);
  void screenshot                (Options);
  void setPhysicsEnabled         (Options);
  void setCameraFree             (Options);
  void setConsoleVisible         (Options);
  void setFullscreen             (Options);
  void setWindowed               (Options);
  void setPlayerScene            (Options);
  void setSceneAmbientLight      (Options);
  void setPlayerPosition         (Options);
  void setPlayerItemGenerationID (Options);
  void getItemData               (Options);
  void getMonsterData            (Options);
  void getArchitectureData       (Options);
  void getSoundData              (Options);
  void getSceneData              (Options);
  void getDataFiles              (Options);
  void getPhysicsInfo            (Options);
  void getGameInfo               (Options);
  void getSceneInfo              (Options);
  void getWorldInfo              (Options);
  void getPlayerPosition         (Options);
  void addItem                   (Options);
  void addMonster                (Options);
  void setSceneLoaded            (Options);
  void setSceneDrawDebugNavMesh  (Options);
  void setSceneShadowsEnabled    (Options);
  void setSceneGravity           (Options);
  void reset                     (Options);
};

