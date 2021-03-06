#include <tinyfsm.hpp>
#include <iostream>

// ----------------------------------------------------------------------------
// 1. Event Declarations
//
struct Toggle : tinyfsm::Event { };


// ----------------------------------------------------------------------------
// 2. State Machine Base Class Declaration
//
struct Switch : tinyfsm::MealyMachine<Switch>
{
  /* pure virtual reaction (override required in all states) */
  virtual void react(Toggle const &) = 0;

  /* transition actions */
  static void OpenCircuit() {
    std::cout << "* Opening ciruit (light goes OFF)" << std::endl;
  }
  static void CloseCircuit()  {
    std::cout << "* Closing ciruit (light goes ON)"  << std::endl;
  }
};


// ----------------------------------------------------------------------------
// 3. State Declarations
//
struct Off; // forward declaration

struct On : Switch
{
  void react(Toggle const &) override { transit<Off>(OpenCircuit); };
};

struct Off : Switch
{
  void react(Toggle const &) override { transit<On>(CloseCircuit); };
};

FSM_INITIAL_STATE(Switch, Off)


// ----------------------------------------------------------------------------
// Main
//
int main()
{
  Switch::start();

  std::cout << "> You are facing a light switch..." << std::endl;
  while(1)
  {
    char c;
    std::cout << std::endl << "t=Toggle, q=Quit ? ";
    std::cin >> c;
    switch(c) {
    case 't':
      std::cout << "> Toggling switch..." << std::endl;
      Switch::dispatch(Toggle());
      break;
    case 'q':
      return 0;
    default:
      std::cout << "> Invalid input" << std::endl;
    };
  }
}
