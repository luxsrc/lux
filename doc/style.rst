``lux`` Style Guides
====================

The ``lux`` code style is heavily based on ``fftw3`` and the ``linux``
kernel.

**Learn from the masters**
   Read Ken Thompson's C compiler in Plan 9, Matteo Frigo's ``fftw3``
   library, and the ``linux`` kernel.  Avoid learning from C++/Java
   programs.

**Indentation, tab, and space**
   Follow Kernighan & Ritchie except:
   *i*) use tabs for indentations and spaces for alignments (code
   looks good even if tab size changes);
   *ii*) tabs are 8 characters (see ``linux`` kernel and Plan 9 source
   codes);
   *iii*) don't put space between a keyword and an open parenthesis
   (enable consistent macros usages);
   *iv*) put the return type of a function in its own line (easy
   :code:`grep`).

**Names**
   Keep them consistent and short.
   Shorter than you think.
   The Bible was written without vowels.
   Don't outsmart the Bible.

   Leave that hungarian crap to MS.
   ``foo_t`` counts as hungarian: use ``foo`` instead.
   ``foo`` is lowercase so that it does not look like a DOS program.
   Exception: :code:`typedef struct foo_s {...} foo;` instead of
   :code:`typedef struct foo {...} foo;` for C++ compatibility.

**C++**
   The code should compile as a C++ program.
   Run the code through :code:`gcc -xc++`.
   The extra C++ restrictions are unnecessary, of course, but this
   will save us from a flood of complaints when we release the code.
