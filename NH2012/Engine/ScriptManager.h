#pragma once

//Standard
#include <iostream>
#include <vector>
#include <string>
#include <map>


class World;

typedef std::vector<std::string> Options;

/*Defines an output target for all text produced by the ScriptManager.*/
class OutputCallback
{
public:
  virtual void print(std::string comment) = 0;
  virtual void print(std::string highlight, std::string comment) = 0;
};

class StandardOutput : public OutputCallback
{
public:
  virtual void print(std::string comment)
  {
    std::cout << comment << std::endl;
  }

  virtual void print(std::string highlight, std::string comment)
  {
    std::cout << highlight << " - " << comment << std::endl;
  }
};

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

  bool update(double elapsedSeconds);

  void run(std::string file);//executes the specified file
  void execute(std::string command);//executes a string of commands

  void display(std::string comment);//displays a line of text
  void display(std::string highlight, std::string comment);//displays a highlighted section of text followed by a normal section
  void addOutputTarget(OutputCallback* target);

private:
  World* world;
  bool done;
  std::vector<OutputCallback*> outputs;
  std::map<std::string, Command<ScriptManager>*> commands;//list of all possible commands

  bool stringToBool(std::string string);
  void split(const std::string &s, char delim, std::vector<std::string> &elems);//Tokenises a string by the given delimiter.
  void addCommand(std::string name, std::string arguments, std::string help, void (ScriptManager::*run) (Options));

  //Set of executable commands
  void about                     (Options);
  void help                      (Options);
  void exit                      (Options);
  void screenshot                (Options);
  void setPhysicsEnabled         (Options);
  void setCameraFree             (Options);
  void setFullscreen             (Options);
  void setWindowed               (Options);
  void setPlayerScene            (Options);
  void setSceneAmbientLight      (Options);
  void setPlayerPosition         (Options);
  void setPlayerItemGenerationID (Options);
  void getItemData               (Options);
  void getCreatureData           (Options);
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
  void addCreature               (Options);
  void setSceneLoaded            (Options);
  void setSceneDrawDebugNavMesh  (Options);
  void setSceneShadowsEnabled    (Options);
  void setSceneGravity           (Options);
  void reset                     (Options);
  
};

