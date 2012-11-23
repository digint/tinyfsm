#ifndef FSMLIST_HPP_INCLUDED
#define FSMLIST_HPP_INCLUDED

#include "debug.hpp"
#include <tinyfsm.hpp>

#include "elevator.hpp"
#include "motor.hpp"

typedef tinyfsm::FsmList<Motor, Elevator> fsm_list;


/* wrapper to fsm_list::dispatch() */
template<typename E>
void send_event(E const & event)
{
  fsm_list::template dispatch<E>(event);
}


#endif
