#include "debug.hpp"
#include <tinyfsm.hpp>

#include "elevator.hpp"
#include "fsmlist.hpp"

#include <iostream>

using namespace std;

class Idle; // forward declaration


// ----------------------------------------------------------------------------
// Transition functions
//

static void CallMaintenance() {
  cout << "*** calling maintenance ***" << endl;
}

static void CallFirefighters() {
  cout << "*** calling firefighters ***" << endl;
}


// ----------------------------------------------------------------------------
// State: Panic
//

class Panic
: public Elevator
{
  void entry() override {
    send_event(MotorStop());
  }
};


// ----------------------------------------------------------------------------
// State: Moving
//

class Moving
: public Elevator
{
  void react(FloorSensor const & e) override {
    cout << "Reached floor " << e.floor << endl;

    int floor_expected = current_floor + Motor::getDirection();
    if(floor_expected != e.floor)
    {
      cout << "Floor sensor defect (expected " << floor_expected << ", got " << e.floor << ")" << endl;
      transit<Panic>(CallMaintenance);
    }

    current_floor = e.floor;
    if(e.floor == dest_floor)
      transit<Idle>();
  };
};


// ----------------------------------------------------------------------------
// State: Idle
//

class Idle
: public Elevator
{
  void entry() override {
    send_event(MotorStop());
  }

  void react(Call  const & e) override {
    dest_floor = e.floor;

    if(dest_floor == current_floor)
      return;

    /* lambda function used for transition action */
    auto action = [] { 
      if(dest_floor > current_floor)
        send_event(MotorUp());
      else if(dest_floor < current_floor)
        send_event(MotorDown());
    };

    transit<Moving>(action);
  };

#if 0
  /* NOTE: Alarm reaction is marked final, so this results in an compile error: */
  void react(Alarm  const & e) override {  };
#endif
};


// ----------------------------------------------------------------------------
// Base state: default implementations
//

void Elevator::react(Call const &) {
  cout << "Call event ignored" << endl;
}

void Elevator::react(FloorSensor const &) {
  cout << "FloorSensor event ignored" << endl;
}

void Elevator::react(Alarm const &) {
  transit<Panic>(CallFirefighters);
}

int Elevator::current_floor = 0;
int Elevator::dest_floor = 0;


// ----------------------------------------------------------------------------
// Initial state definition
//
FSM_INITIAL_STATE(Elevator, Idle);
