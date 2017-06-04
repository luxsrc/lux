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

There are two solutions to reduce the indirectness:
*i*) to embed :code:`struct point_s` directly in :code:`struct
task_s`, or
*ii*) to always pass :code:`struct task_s` by value.
