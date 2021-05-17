/*
 * TinyFSM - Tiny Finite State Machine Processor
 *
 * Copyright (c) 2012-2018 Axel Burri
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
 * Version: 0.4.0
 *
 * API documentation: see "../doc/50-API.md"
 *
 * The official TinyFSM website is located at:
 * https://digint.ch/tinyfsm/
 *
 * Author:
 * Axel Burri <axel@tty0.ch>
 * incloon
 * ---------------------------------------------------------------------
 */

#ifndef TINYFSM_HPP_INCLUDED
#define TINYFSM_HPP_INCLUDED

#include <type_traits>
#include <utility>

//#define TINYFSM_ENABLE_MULTIDISPATCH

#ifndef TINYFSM_ENABLE_MULTIDISPATCH
#define DISPATCH_TIMES_ASSERT(__TIMES) static_assert(__TIMES == 1, "Invalid dispatch")
#else
#ifndef TINYFSM_MAX_DISPATCH_TIMES
#define TINYFSM_MAX_DISPATCH_TIMES  3
#endif
#define DISPATCH_TIMES_ASSERT(__TIMES) static_assert(__TIMES <= TINYFSM_MAX_DISPATCH_TIMES, "Invalid dispatch")
#endif

// #include <iostream>
// #define DBG(str) do { std::cerr << str << std::endl; } while( false )
// DBG("*** dbg_example *** " << __PRETTY_FUNCTION__);

namespace tinyfsm
{

  // --------------------------------------------------------------------------

  struct Event { };

  // --------------------------------------------------------------------------

  // check if both fsm and state class share same fsmtype
  template<typename F, typename S>
  using is_same_fsm = std::is_same< typename F::fsmtype, typename S::fsmtype >;
  
  template <typename T1, typename T2, typename T = void>
  using __enable_if_is_same_t = typename std::enable_if<std::is_same<T1, T2>::value, T>::type;



  template <typename S>
  struct _StateBaseCheck {
  private:
    template <typename T, typename = decltype(static_cast<typename T::state_ptr_t>(0)->exit())>
    static constexpr bool checkExitFunc(T*) { return true; };
    static constexpr bool checkExitFunc(...) { return false; };

    static_assert(checkExitFunc(static_cast<S*>(0)), "The function exit() must be in state machine base class! ");

    template <typename T, void(T::*)() = &T::initial>
    static constexpr bool checkIniFunc(T*) { return true; };
    static constexpr bool checkIniFunc(...) { return false; };

    template <typename T, typename = __enable_if_is_same_t<decltype(static_cast<typename T::state_ptr_t>(0)->exit()), void> >
    static constexpr bool checkReturn(T*) { return false; };
    static constexpr bool checkReturn(...) { return true; };

  public:
    static constexpr bool has_initial = checkIniFunc(static_cast<S*>(0));
    static constexpr bool has_return = checkReturn(static_cast<S*>(0));
  };
  
  template <typename S, typename... P>
  struct _StateEntryCheck {
  private:
    static constexpr bool assert = _StateBaseCheck<S>::has_return;

    template <typename T, typename = __enable_if_is_same_t<decltype(static_cast<T*>(0)->entry((P&&)(*static_cast<typename std::remove_reference<P>::type*>(0))...)), void> >
    static constexpr bool checkEntry(T*) { return true; };
    static constexpr bool checkEntry(...) { return false; };

  public:
    typedef _StateBaseCheck<S> SBC;

    static constexpr bool has_entry = checkEntry(static_cast<S*>(0));
  };

  template <typename F, typename E>
  struct _EventCheck {
  private:
    template <typename T, typename = __enable_if_is_same_t<decltype(static_cast<T*>(0)->react((E&&)(*static_cast<typename std::remove_reference<E>::type*>(0)))), void> >
    static constexpr bool checkEvent(T*) { return true; };
    static constexpr bool checkEvent(...) { return false; };

  public:
    static constexpr bool valid = checkEvent(static_cast<F*>(0));
    template<typename InferenceTrigger = void> static inline typename std::enable_if<valid, InferenceTrigger>::type dispatch(E&& event) { F::fsmtype::template dispatch<E>(std::forward<E>(event)); }
    template<typename InferenceTrigger = void> static inline typename std::enable_if<!valid, InferenceTrigger>::type dispatch(E&& event) {}
  };
  // --------------------------------------------------------------------------

  template<typename S>
  struct _state_instance
  {
    using value_type = S;
    using type = _state_instance<S>;
    static S value;
  };

  template<typename S>
  typename _state_instance<S>::value_type _state_instance<S>::value;

  template<typename S>
  struct _IniState
  {
    static_assert(_StateEntryCheck<S>::has_entry || _StateBaseCheck<S>::has_initial, "No entry function! The initial state of the state machine should have entry() or initial()");
    template<typename InferenceTrigger = void> static inline typename std::enable_if<_StateBaseCheck<S>::has_initial, InferenceTrigger>::type enter() { _state_instance<S>::value.initial(); }
    template<typename InferenceTrigger = void> static inline typename std::enable_if<!_StateBaseCheck<S>::has_initial, InferenceTrigger>::type enter() { _state_instance<S>::value.entry(); }
  };

  // --------------------------------------------------------------------------

  template<typename F>
  class Fsm
  {
  public:

    using fsmtype = Fsm<F>;
    using state_ptr_t = F *;

    static state_ptr_t current_state_ptr;

    // public, leaving ability to access state instance (e.g. on reset)
    template<typename S>
    static constexpr S & state(void) {
      static_assert(is_same_fsm<F, S>::value, "accessing state of different state machine");
      return _state_instance<S>::value;
    }

    template<typename S>
    static constexpr bool is_in_state(void) {
      static_assert(is_same_fsm<F, S>::value, "accessing state of different state machine");
      return current_state_ptr == &_state_instance<S>::value;
    }

  /// state machine functions
  public:

    // explicitely specialized in FSM_INITIAL_STATE macro
    static void set_initial_state();

    static void reset() { };

    static void start() {
      set_initial_state();
    }

    template<typename E>
    static void dispatch(E&& event) {
      static_assert(_EventCheck<F, E>::valid, "The dispatched event is not in the state machine");
      current_state_ptr->react(std::forward<E>(event));
    }


  /// state transition functions
  protected:
	template <typename S>
    typename std::enable_if<!_StateBaseCheck<S>::has_return>::type transit(void) {
      static_assert(is_same_fsm<F, S>::value, "transit to different state machine");
      static_assert(_StateEntryCheck<S>::has_entry, "There is no function entry() in the current state");
      current_state_ptr->exit();
      current_state_ptr = &_state_instance<S>::value;
      _state_instance<S>::value.entry();
    }

	template <typename S>
    typename std::enable_if<_StateBaseCheck<S>::has_return>::type transit(void) {
	  static_assert(is_same_fsm<F, S>::value, "transit to different state machine");
      static_assert(_StateEntryCheck<S, decltype(static_cast<F*>(0)->exit())>::has_entry, "There is no function entry(exit()) in the current state");
	  auto&& transdata = current_state_ptr->exit();
	  current_state_ptr = &_state_instance<S>::value;
      _state_instance<S>::value.entry(std::move(transdata));
	}
    
    template <typename S, typename T>
    typename std::enable_if<_StateEntryCheck<S, T>::has_entry>::type transit(T&& data) {
      static_assert(is_same_fsm<F, S>::value, "transit to different state machine");
      current_state_ptr->exit();
      current_state_ptr = &_state_instance<S>::value;
      _state_instance<S>::value.entry(std::forward<T>(data));
    }

	template <typename S, typename ActionFunction>
    typename std::enable_if<!_StateEntryCheck<S, ActionFunction>::has_entry && !_StateBaseCheck<S>::has_return>::type transit(ActionFunction action_function) {
      static_assert(is_same_fsm<F, S>::value, "transit to different state machine");
      static_assert(_StateEntryCheck<S>::has_entry, "There is no function entry() in the current state");
      static_assert(decltype(action_function(),true)(true), "");
      current_state_ptr->exit();
      // NOTE: we get into deep trouble if the action_function sends a new event.
      // TODO: implement a mechanism to check for reentrancy
      action_function();
      current_state_ptr = &_state_instance<S>::value;
      _state_instance<S>::value.entry();
    }

	template <typename S, typename ActionFunction>
    typename std::enable_if<!_StateEntryCheck<S, ActionFunction>::has_entry && _StateBaseCheck<S>::has_return>::type transit(ActionFunction action_function) {
      static_assert(is_same_fsm<F, S>::value, "transit to different state machine");
      static_assert(_StateEntryCheck<S, decltype(static_cast<F*>(0)->exit())>::has_entry, "There is no function entry(exit()) in the current state");
      static_assert(decltype(action_function(), true)(true), "");
	  auto &&transdata = current_state_ptr->exit();
      // NOTE: we get into deep trouble if the action_function sends a new event.
      // TODO: implement a mechanism to check for reentrancy
      action_function();
      current_state_ptr = &_state_instance<S>::value;
      _state_instance<S>::value.entry(std::move(transdata));
    }
	
    template <typename S, typename T, typename ActionFunction>
    void transit(T&& data, ActionFunction action_function) {
      static_assert(is_same_fsm<F, S>::value, "transit to different state machine");
      static_assert(_StateEntryCheck<S, T>::has_entry, "There is no function entry(data) in the current state");
      static_assert(decltype(action_function(), true)(true), "");
      current_state_ptr->exit();
      // NOTE: we get into deep trouble if the action_function sends a new event.
      // TODO: implement a mechanism to check for reentrancy
      action_function();
      current_state_ptr = &_state_instance<S>::value;
      _state_instance<S>::value.entry(std::forward<T>(data));
    }

  //private:
  //  static bool transit_lock;
  };

  template<typename F>
  typename Fsm<F>::state_ptr_t Fsm<F>::current_state_ptr;

  //template<typename F>
  //bool Fsm<F>::transit_lock = false;

  // --------------------------------------------------------------------------

  template<typename... FF>
  struct FsmList;

  template<> struct FsmList<> {
    static void set_initial_state() { }
    static void reset() { }
    template<typename E, unsigned int dispatch_times>
    static void dispatch(E&&) {
      static_assert(dispatch_times, "Invalid dispatch"); 
      DISPATCH_TIMES_ASSERT(dispatch_times);
    }
  };

  template<typename F, typename... FF>
  struct FsmList<F, FF...>
  {
    static void set_initial_state() {
      F::fsmtype::set_initial_state();
      FsmList<FF...>::set_initial_state();
    }

    static void reset() {
      F::fsmtype::reset();
      FsmList<FF...>::reset();
    }

    static void start() {
      set_initial_state();
    }

    template<typename E, unsigned int dispatch_times = 0>
    static void dispatch(E&& event) {
      _EventCheck<F, E>::dispatch(std::forward<E>(event));
      FsmList<FF...>::template dispatch<E, dispatch_times + (_EventCheck<F, E>::valid ? 1 : 0)>(std::forward<E>(event));
    }
  };

  // --------------------------------------------------------------------------

  template<typename... SS> struct StateList;
  template<> struct StateList<> {
    static void reset() { }
  };
  template<typename S, typename... SS>
  struct StateList<S, SS...>
  {
    static void reset() {
      _state_instance<S>::value = S();
      StateList<SS...>::reset();
    }
  };

  // --------------------------------------------------------------------------

  template<typename F>
  struct MooreMachine : tinyfsm::Fsm<F>
  {
    virtual void entry(void) { };  /* entry actions in some states */
    void exit(void) { };           /* no exit actions */
  };

  template<typename F>
  struct MealyMachine : tinyfsm::Fsm<F>
  {
    // input actions are modeled in react():
    // - conditional dependent of event type or payload
    // - transit<>(ActionFunction)
    void entry(void) { };  /* no entry actions */
    void exit(void) { };   /* no exit actions */
  };

} /* namespace tinyfsm */


#define FSM_INITIAL_STATE(_FSM, _STATE)                               \
namespace tinyfsm {                                                   \
  template<> void Fsm< _FSM >::set_initial_state(void) {              \
    current_state_ptr = &_state_instance< _STATE >::value;            \
    _IniState< _STATE >::enter();                                     \
  }                                                                   \
}

#endif /* TINYFSM_HPP_INCLUDED */
