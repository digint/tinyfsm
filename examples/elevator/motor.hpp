#ifndef MOTOR_HPP_INCLUDED
#define MOTOR_HPP_INCLUDED

#include <tinyfsm.hpp>


// ----------------------------------------------------------------------------
// Event declarations
//

struct MotorUp   : tinyfsm::Event { };
struct MotorDown : tinyfsm::Event { };
struct MotorStop : tinyfsm::Event { };


// ----------------------------------------------------------------------------
// Motor (FSM base class) declaration
//
class Motor
: public tinyfsm::Fsm<Motor>
{
  friend class Fsm;

  void react(tinyfsm::Event const &) { };    /* default reaction (unwanted events) */

  void react(MotorUp   const &);
  void react(MotorDown const &);
  void react(MotorStop const &);

  virtual void entry(void) = 0;  /* pure virtual: enforce implementation in all states */
  void exit(void)  { };

protected:

  static int direction;

public:
  static int getDirection() { return direction; }
};


#endif
