/*
 * TinyFSM - Tiny Finite State Machine Processor
 *
 * Copyright (c) 2012-2017 Axel Burri
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* ---------------------------------------------------------------------
 * Version: 0.2.0
 *
 * The official TinyFSM website is located at:
 * https://digint.ch/tinyfsm/
 *
 * Author:
 * Axel Burri <axel@tty0.ch>
 * ---------------------------------------------------------------------
 */

#ifndef TINYFSM_HPP_INCLUDED
#define TINYFSM_HPP_INCLUDED

#include <type_traits>

// #include <iostream>
// #define DBG(str) do { std::cerr << str << std::endl; } while( false )
// DBG("*** dbg_example *** " << __PRETTY_FUNCTION__);

namespace tinyfsm
{

  // --------------------------------------------------------------------------

  struct Event { };

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

  template<typename F>
  class Fsm
  {
    typedef F *       state_ptr_t;
    typedef F const * state_ptr_const_t;

    static state_ptr_t current_state_ptr;

  protected:

    template<typename S>
    static constexpr S const & state(void) {
      return _state_instance<F, S>::value;
    }

    template<typename S>
    static constexpr state_ptr_t state_ptr(void) {
      return &_state_instance<F, S>::value;
    }


  /// state machine functions (should not be used in state class)
  public:

    static void reset(void);

    static void start(void) {
      F::reset();
      current_state_ptr->entry();
    }

    template<typename E>
    static void dispatch(E const & event) {
      current_state_ptr->react(event);
    }

    static state_ptr_const_t get_current_state(void) {
      return current_state_ptr;
    }

    template<typename S>
    static void set_current_state(void) {
      current_state_ptr = state_ptr<S>();
    }

    static void enter(void) {
      current_state_ptr->entry();
    }

    template<typename S>
    static void enter(void) {
      current_state_ptr = state_ptr<S>();
      current_state_ptr->entry();
    }


  /// state transition functions (for use in state class)
  protected:

    template<typename S>
    void transit(void) {
      current_state_ptr->exit();
      current_state_ptr = state_ptr<S>();
      current_state_ptr->entry();
    }

    template<typename S, typename ActionFunction>
    void transit(ActionFunction action_function) {
      static_assert(std::is_void<typename std::result_of<ActionFunction()>::type >::value,
                    "result type of 'action_function()' is not 'void'");

      current_state_ptr->exit();
      // NOTE: we get into deep trouble if the action_function sends a new event.
      // TODO: implement a mechanism to check for reentrancy
      action_function();
      current_state_ptr = state_ptr<S>();
      current_state_ptr->entry();
    }

    template<typename S, typename ActionFunction, typename ConditionFunction>
    void transit(ActionFunction action_function, ConditionFunction condition_function) {
      static_assert(std::is_same<typename std::result_of<ConditionFunction()>::type, bool>::value,
                    "result type of 'condition_function()' is not 'bool'");

      if(condition_function()) {
        transit<S>(action_function);
      }
    }
  };

  template<typename F>
  typename Fsm<F>::state_ptr_t Fsm<F>::current_state_ptr;

  // --------------------------------------------------------------------------

  template<typename... FF>
  struct FsmList;

  template<>
  struct FsmList<>
  {
    static void reset() { }
    static void enter() { }

    template<typename E>
    static void dispatch(E const &) { }
  };

  template<typename F, typename... FF>
  struct FsmList<F, FF...>
  {
    static void reset() {
      F::reset();
      FsmList<FF...>::reset();
    }

    static void enter() {
      F::enter();
      FsmList<FF...>::enter();
    }

    static void start() {
      reset();
      enter();
    }

    template<typename E>
    static void dispatch(E const & event) {
      F::template dispatch<E>(event);
      FsmList<FF...>::template dispatch<E>(event);
    }
  };
} /* namespace tinyfsm */


#define FSM_INITIAL_STATE(_FSM, _STATE)          \
namespace tinyfsm {                              \
  template<> void Fsm< _FSM >::reset(void) {     \
    set_current_state< _STATE >();               \
  }                                              \
}


#endif /* TINYFSM_HPP_INCLUDED */
