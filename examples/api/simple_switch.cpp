#include "../../include/tinyfsm.hpp"
#include <iostream>

class Off; // forward declaration


// ----------------------------------------------------------------------------
// 1. Event Declarations
//
struct Toggle : tinyfsm::Event { };


// ----------------------------------------------------------------------------
// 2. State Machine Base Class Declaration
//
class Switch
: public tinyfsm::Fsm<Switch>
{
public:
  /* set initial state (option 1)*/
  static void reset(void) {
    std::cout << "* Initializing Switch to Off state" << std::endl;
    set_current_state<Off>();
  }

  /* default reaction for unhandled events */
  void react(tinyfsm::Event const &) { };

  virtual void react(Toggle const &) { };
  virtual void entry(void) { };  /* entry actions in some states */
  void         exit(void)  { };  /* no exit actions */
};


// ----------------------------------------------------------------------------
// 3. State Declarations
//
class On
: public Switch
{
  void entry() override { std::cout << "* Switch is ON" << std::endl; };
  void react(Toggle const &) override { transit<Off>(); };
};

class Off
: public Switch
{
  void entry() override { std::cout << "* Switch is OFF" << std::endl; };
  void react(Toggle const &) override { transit<On>(); };
};

/* set initial state (option 2)*/
//FSM_INITIAL_STATE(Switch, Off)


// ----------------------------------------------------------------------------
// 4. State Machine List Declaration (dispatches events to multiple FSM's)
//
// In this example, we only have a single state machine, no need to use FsmList<>:
//using fsm_handle = tinyfsm::FsmList< Switch >;
using fsm_handle = Switch;


// ----------------------------------------------------------------------------
// Main
//
int main()
{
  // instantiate events
  Toggle toggle;

  fsm_handle::start();
  // alternative: start single state machine:
  //Switch::start();

  while(1)
  {
    char c;
    std::cout << std::endl << "t=Toggle, q=Quit ? ";
    std::cin >> c;
    switch(c) {
    case 't':
      std::cout << "> Toggling switch..." << std::endl;
      fsm_handle::dispatch(toggle);
      // alternative: dispatch event to single state machine:
      //Switch::dispatch(toggle);
      break;
    case 'q':
      return 0;
    default:
      std::cout << "> Invalid input" << std::endl;
    };
  }
}
