TinyFSM
=======

TinyFSM is a simple finite state machine library for C++, designed for
optimal performance and low memory footprint. This makes it ideal for
real-time operating systems. The concept is very simple, allowing the
programmer to fully understand what is happening behind the scenes. It
provides a straightforward way of mapping your state machine charts
into source code.

TinyFSM basically wraps event dispatching into function calls, making
event dispatching equally fast to calling (or even inlining) a
function. Even in the worst case, dispatching leads to nothing more
than a single vtable lookup and function call!

Key Features:

 - Entry/exit actions
 - Event actions
 - Transition functions
 - Transition conditions
 - Event payload (classes)
 - Inheritance of states and action functions

TinyFSM benefits from the C++11 template metaprogramming features like
variadic templates, and does not depend on RTTI, exceptions or any
external library.


Official home page: <https://digint.ch/tinyfsm>

Current version: `0.3.3`


Installation
------------

TinyFSM is a header-only library, no special installation steps are
needed. Just point your compiler to the "include" directory.


Documentation
-------------

You can find the full documentation in the `doc/` directory:

 - [Introduction](/doc/10-Introduction.md)
 - [Installation](/doc/20-Installation.md)
 - [Concepts](/doc/30-Concepts.md)
 - [Usage](/doc/40-Usage.md)
 - [API](/doc/50-API.md)

The docmentation is also available on the [official home
page](https://digint.ch/tinyfsm/doc/introduction.html).


### Code examples

 - [Elevator Project]: Documented example, two state machines with
   shiny buttons, floor sensors and actors.
 - [Simple Switch]: A generic switch with two states (on/off).
 - [API examples]

  [Elevator Project]: /examples/elevator/
  [Simple Switch]:    /examples/api/simple_switch.cpp
  [API Examples]:     /examples/api/


Donate
------

So TinyFSM has proven useful for you?

I will definitively continue to develop TinyFSM for free. If you want
to support me with a donation, you are welcome to do so!

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=QZQE9HY6QHDHS)


Development
-----------

The source code for TinyFSM is managed using Git:

    git clone https://dev.tty0.ch/tinyfsm.git

Mirror on GitHub:

    git clone https://github.com/digint/tinyfsm.git

If you would like to contribute or have found bugs, visit the [TinyFSM
project page on GitHub] and use the [issues tracker] there.

  [TinyFSM project page on GitHub]: http://github.com/digint/tinyfsm
  [issues tracker]: http://github.com/digint/tinyfsm/issues


Contact
-------

For questions and suggestions regarding TinyFSM, success or failure
stories, and any other kind of feedback, please feel free to contact
the author (the email address can be found in the sources).


License
-------

TinyFSM is [Open Source] software. It may be used for any purpose,
including commercial purposes, at absolutely no cost. It is
distributed under the terms of the [MIT license].

  [Open Source]: http://www.opensource.org/docs/definition.html
  [MIT license]: http://www.opensource.org/licenses/mit-license.html
