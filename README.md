TinyFSM
=======

TinyFSM is a simple finite state machine library for C++, designed for
optimal performance and low memory footprint. This makes it ideal for
real-time operating systems. The concept is very simple, allowing the
programmer to fully understand what is happening behind the scenes. It
provides a straightforward way of mapping your state machine charts
into source code.

TinyFSM basically wraps event dispatching into method calls, making
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


Official home page: <http://www.digint.ch/tinyfsm>

Current version: `0.10`


DOCUMENTATION
-------------

You can find the main documentation in the `doc/` directory of the
TinyFSM project. The latest version is also available
[online](http://www.digint.ch/tinyfsm/doc/introduction.html).


INSTALLATION
------------

TinyFSM is a header-only library, no special installation steps are
needed. Just point your compiler to the "include" directory.


DEVELOPMENT
-----------

The source code for TinyFSM is managed using Git. Check out the
source repository like this:

    git clone git://dev.tty0.ch/tinyfsm.git

If you would like to contribute, you might want to visit the [TinyFSM
project page on GitHub](http://github.com/digint/tinyfsm).


SUPPORT
-------

If you need further information or find bugs, please contact the
author.


AUTHOR
------

Axel Burri <axel@tty0.ch>


COPYRIGHT AND LICENSE
---------------------

Copyright (C) 2012 Axel Burri. All rights reserved.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
