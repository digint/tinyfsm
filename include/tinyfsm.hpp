/*
 * TinyFSM - Tiny Finite State Machine Processor
 *
 * Copyright 2012 Axel Burri <axel@tty0.ch>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */


#ifndef TINYFSM_HPP_INCLUDED
#define TINYFSM_HPP_INCLUDED

#include <type_traits>

namespace tinyfsm
{

  // --------------------------------------------------------------------------

  template<typename... FF>
  struct FsmList;

  template<>
  struct FsmList<>
  {
    static void start() {
      // DBG("*** FsmList::start() - size=0 *** " << __PRETTY_FUNCTION__);
    }

    template<typename E>
    static void dispatch(E const &) {
      // DBG("*** FsmList::dispatch() - size=0 *** " << __PRETTY_FUNCTION__);
    }
  };

  template<typename F, typename... FF>
  struct FsmList<F, FF...>
  {
    static void start() {
      // DBG("*** FsmList::start() - size=" << (sizeof...(FF) + 1) << " *** " << __PRETTY_FUNCTION__);
      F::start();
      FsmList<FF...>::start();
    }

    template<typename E>
    static void dispatch(E const & event) {
      // DBG("*** FsmList::dispatch() - size=" << (sizeof...(FF) + 1) << " *** " << __PRETTY_FUNCTION__);
      F::template dispatch<E>(event);
      FsmList<FF...>::template dispatch<E>(event);
    }
  };

  // --------------------------------------------------------------------------

  struct Event { };     /* Event base class  */

  // --------------------------------------------------------------------------

  namespace experimental
  {
    /**
     * Event which is bound to a Fsm or FsmList class. A BoundEvent
     * implements non-template dispatch() function, which will send
     * the event instance to the specified Fsm or FsmList.
     *
     * NOTE: If BoundEvent is used, forward declaration for all events
     * are needed in front of the Fsm declaration. This is because
     * BoundEvent<FsmList> introduces circular dependency to
     * the Fsm<> class (which is again part of the FsmList<> template
     * parameter).
     *
     * NOTE: BoundEvent uses static_cast<>, which can have ugly
     * side-effects if not used correctly!
     */
    template<typename E, typename L>
    struct BoundEvent
    : public Event
    {
      void dispatch() {
        L::template dispatch<E>(static_cast<E const &>(*this));
      }
    };
  } /* namespace experimental */

  // --------------------------------------------------------------------------

  template<typename F, typename S>
  struct _state_instance
  {
    static S value;
    typedef S value_type;
    typedef _state_instance<F, S> type;
  };

  /* state instance definitions */
  template<typename F, typename S>
  typename _state_instance<F, S>::value_type _state_instance<F, S>::value;

  // --------------------------------------------------------------------------

  template<typename F>     /* FSM base class  */
  class Fsm
  {
    typedef F *       state_ptr_t;
    typedef F const * state_ptr_const_t;


    /* NOTE: The 'current_state' static member variable must be defined
     *       for every FSM. This can be done by the FSM_INITIAL_STATE macro.
     */
    static state_ptr_t current_state;

    template<typename S>
    static constexpr state_ptr_t state_ptr(void) {
      return &_state_instance<F, S>::value;
    }

  protected:

    template<typename S>
    static constexpr S const & state(void) {
      return _state_instance<F, S>::value;
    }

    template<typename S>
    void transit(void) {
      // DBG("*** Fsm::transit<S>() *** " << __PRETTY_FUNCTION__);
      current_state->exit();
      current_state = state_ptr<S>();
      current_state->entry();
    }

    template<typename S, typename ActionFunction>
    void transit(ActionFunction action_function) {
      // DBG("*** Fsm::transit<S, ActionFunction>() *** " << __PRETTY_FUNCTION__);
      static_assert(std::is_void<typename std::result_of<ActionFunction()>::type >::value,
                    "result type of 'action_function()' is not 'void'");

      current_state->exit();
      // NOTE: we get into deep trouble if the action_function sends a new event.
      // TODO: implement a mechanism to check for reentrancy
      action_function();
      current_state = state_ptr<S>();
      current_state->entry();
    }

    template<typename S, typename ActionFunction, typename ConditionFunction>
    void transit(ActionFunction action_function, ConditionFunction condition_function) {
      // DBG("*** Fsm::transit<S, ActionFunction, ConditionFunction>() *** " << __PRETTY_FUNCTION__);
      static_assert(std::is_same<typename std::result_of<ConditionFunction()>::type, bool>::value,
                    "result type of 'condition_function()' is not 'bool'");

      if(condition_function()) {
        transit<S>(action_function);
      }
    }


  public:

    static void start(void) {
      // DBG("*** Fsm::start() *** " << __PRETTY_FUNCTION__);
      current_state->entry();
    }

    template<typename E>
    static void dispatch(E const & event) {
      // DBG("*** Fsm::dispatch() *** " << __PRETTY_FUNCTION__);
      current_state->react(event);
    }

    static state_ptr_const_t get_current_state(void) {
      return current_state;
    }
  };

} /* namespace tinyfsm */


/** Initialize (define) "current_state" to _STATE for _FSM (explicit template specialization) */
#define FSM_INITIAL_STATE(_FSM, _STATE)                                 \
  template<>                                                            \
  tinyfsm::Fsm<_FSM>::state_ptr_t tinyfsm::Fsm<_FSM>::current_state = tinyfsm::Fsm<_FSM>::state_ptr<_STATE>()


#endif /* TINYFSM_HPP_INCLUDED */
