#pragma once

//Standard
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class World;
class ConsoleScreen;

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

  void setWorld(boost::shared_ptr<World> world);

  bool update(double elapsedSeconds);

  void run(std::string file);//executes the specified file
  void execute(std::string command);//executes a string of commands

  void display(std::string comment);//displays a line of text
  void display(std::string highlight, std::string comment);//displays a highlighted section of text followed by a normal section
  int display(lua_State* state);//lua print
  void addOutputTarget(OutputCallback* target);

  void setConsole(ConsoleScreen* console);
  
  boost::shared_ptr<World> getWorld() 
  {
    try
    {
      return world.lock();
    }
    catch(boost::bad_weak_ptr b)
    {
#ifdef _DEBUG
      std::cout << "Could not get world from script manager. World has expired." << std::endl;
#endif
    }
    return boost::shared_ptr<World>();
  }

private:
  boost::weak_ptr<World> world;
  bool done;
  std::vector<OutputCallback*> outputs;

  lua_State* state;
  void initialiseLua();

  bool stringToBool(std::string string);
  void split(const std::string &s, char delim, std::vector<std::string> &elems);//Tokenises a string by the given delimiter.

  //Set of executable commands
  void about();
  void help();
  void exit();
  void screenshot();
  void setCameraFree(bool free);
  void setFullscreen(int width, int height);
  void setWindowed(int width, int height);
  void setPlayerItemGenerationID(int id);
  void getPhysicsInfo();
  void getGameInfo();
  void getSceneInfo();
  void getWorldInfo();
  void reset();
  
};

