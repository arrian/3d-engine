#pragma once

#include <string>
#include <sstream>
#include <vector>

#include <OISKeyboard.h>
#include <OgreString.h>

#include "World.h"
#include "SceneChangeListener.h"
#include "Gorilla.h"

typedef std::vector<std::string> Options;//shorter vector string typedef

/**
* Defines a command that can be run.
*/
template<typename Owner>
class Command
{
public:
  Command(std::string name, std::string arguments, std::string help, void (Owner::*Run) (Options), Owner* owner)
    : name(name),
      args(arguments),
      help(help),
      Run(Run),
      owner(owner)
  {
  }

  ~Command() {}

  std::string name;//command name
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

/**
 * In-game command console singleton.
 */
class Console : public SceneChangeListener
{
public:
  static Console& getInstance()
  {
      static Console instance;
      return instance;
  }

  void setWorld(World* world);
  void setKeyboard(OIS::Keyboard* keyboard);


  void hookWindow(Ogre::RenderWindow* window);//hooks the console to a render window
  bool isVisible();//returns the visibility of the console
  void setVisible(bool visible);//sets the visibility of the console
  bool update(double elapsedSeconds);
  void injectKeyDown(const OIS::KeyEvent &arg);//key down
  void injectKeyUp(const OIS::KeyEvent &arg);//key up
  void injectKey(const OIS::KeyEvent &arg, bool isDown);//key down or up
  void keyPressed(const OIS::KeyEvent &arg);

  void print(std::string comment);

  void sceneChanged();
  
private:
  Console();//World* world, OIS::Keyboard* keyboard);
  ~Console(void);
  Console(Console const&);              // Don't Implement
  void operator=(Console const&); // Don't implement

  bool done;//exit called

  bool isShift;//True if shift is pressed.
  bool isControl;//True if control is pressed.
  bool showCursor;
  Ogre::Root* root;//Game root.
  Ogre::RenderWindow* window;//Game render window.
  OIS::Keyboard* keyboard;//Keyboard manager.
  World* world;//Game world.
  Gorilla::Silverback* overlay;//Console renderer.
  Gorilla::Screen* screen;//Console screen.
  Gorilla::Layer* layer;//Console layer.
  Gorilla::MarkupText* view;//Console display text.
  std::vector<std::string> lines;
  int displayLines;//number of lines to display at a time
  int displayOffset;
  std::string command;//Current console command.
  std::vector<std::string> history;//The command execution history.
  int historyIndex;
  std::vector<Command<Console>*> commands;//list of all possible commands

  bool keyIsDown;
  bool hasDoneFirstHold;//has only done the initial key press and no repeats
  double firstKeyPressHoldWait;//how long to wait after the first key press to repeat the key
  double consecutiveKeyPressHoldWait;//how long to wait before a key press is repeated
  double holdAccumulator;
  double cursorFlashRate;
  double cursorAccumulator;
  OIS::KeyCode previousKey;

  void enter();//Submit the command for processing.
  void backspace();//Backspace pressed.
  void update();//Updates the console display.
  void noCommand(std::string command);//Displays the no command error.
  void split(const std::string &s, char delim, std::vector<std::string> &elems);//Tokenises a string by the given delimiter.
  void up();//up pressed
  void down();//down pressed
  void display(std::string comment);//displays a line of text
  void display(std::string highlight, std::string comment);//displays a highlighted section of text followed by a normal section
  void error(std::string comment);//displays an error on the console

  bool stringToBool(std::string string);

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

