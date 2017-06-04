Design Patterns
===============

Memory leakage is a prevalent issue in software written in ``C/C++``,
and is likely to cause issues for ``lux``.
While ``lux`` provides tools such as reference counting to avoid
memory leakage, we also develop design patterns to avoid allocating
small memory buffers.
This design pattern improves the performance of ``lux`` and avoid
unnecessary dynamic memory management operations.

Consider the following code:

.. code-block:: c

   struct point_s {
   	double x, y, z;
   };

   struct task_s {
   	double (*algo)(struct task_s *); /* pass self */
   	struct point_s *point;
   };

   struct task_s *t = mktask();
   double r = t->algo(t);
   rmtask(t);

where :code:`mktask()` performs double dynamic memory allocation
internally and creates the memory layout:

::

   t -> | algo  | -> function
        | point | -> | x |
                     | y |
                     | z |

If :code:`mktask()`, :code:`rmtask(t)`, or :code:`t->algo(t)` are
called often, these double reference, potential incoherent memory, and
dynamic memory management may cause significant overhead.

There are two solutions to reduce memory indirectness:
*i*) to embed :code:`struct point_s` directly in :code:`struct
task_s`, or
*ii*) to treat :code:`struct task_s t` as a variable and always pass
it by value.


Structure Embedding
-------------------

Structure embedding is a standard method to reduce memory
indirectness.

.. code-block:: c

   	struct task_s {
   		double (*algo)(struct task_s *);
   		struct point_s point; /* embeded instead of a pointer */
   	};

With structure embedding, :code:`mktask()` and `rmtask()` only need to
allocate and free the heap once.
Also, :code:`struct point_s` is now guaranteed to be near the
:code:`algo` pointer, potentially improving the cache performance.

It is how ``C++`` implements inheritance under-the-hood, and hence
carries all the problem of inheritance.
Therefore, when inheritance is a good solution, structure embedding
also tents to be a good solution.


Small Structure as Variable
---------------------------

Another solution is to treat small structure such as the original
:code:`struct task_s t` as a variable and always pass it by value.

.. code-block:: c


   	struct task_s t = mktask(); /* return struct instead of pointer */
   	double r = t.algo(t);
   	rmtask(t);

This way, :code:`mktask()` only needs to allocate memory for
:code:`t.point` once and :code:`rmtask()` is only responsbile for
freeing :code:`t.point`.
There is also less indirectness in calling :code:`t.algo()` and within
the implementation of :code:`t.algo()`.
In fact, for many calling conventions, the small structure :code:`t`
will be passed to functions through registers.
