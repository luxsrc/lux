Design
======


Run Time Optimization
---------------------

According to the "README.hacks" file in the ``fftw2`` library,

   The architectural details of modern machines make performance
   *extremely* sensitive to little details such as where your code and
   data are placed in memory.

It is therefore very difficult to develop a library with portable
performance.
In ``lux``, we follow ``fftw``'s approach (and terminology) for
run time adaptive performance tuning.
For each run time optimizable algorithm, we store its specifications
in a "problem" data structure.
The problem is passed to a planner, which uses a list of "solver"
modules to solve for many "solution" algorithms.
We then measure the performance of each of these solutions at run time
and select the fastest one.
Furthermore, ``lux`` exposes this technique through a standard
interface.
This makes developing run time optimizable algorithms more accessible
to a wider range of computational scientists.

At the lowest level, a run time optimized algorithm can be executed by
the following function call in C:

.. code-block:: c

   selected_algorithm(ispec, xspec, params, ro, rw, wo);

where ``selected_algorithm`` is a pointer to a driver function or to
the actual algorithm;
``ispec`` and ``xspec`` are internal (derived) and external (input)
specifications, respectively, which hold all the information needed by
the algorithm that would affect the performance (e.g., array stride,
twiddle factors for FFT, pointers to codelets);
``params`` are input parameters that would not affect performance
(e.g., ``alpha`` in ``BLAS``);
and ``ro``, ``rw``, and ``wo`` are pointers to the read-only,
read-write, and write-only data structures in memory for input and/or
output.


Task Abstractions
-----------------

In principle, we could pack ``algo``, ``ispec``, ``xspec``,
``params``, ``ro``, ``rw``, and ``wo`` into a single abstraction.
We call this a "task" in ``lux``.
Although run time optimization using a full task can take into
account, e.g., memory alignments of data structures, it requires
creating new tasks to apply the same algorithm to different parameters
and data sets.
This often makes the code harder to manage.
Moreover, creating additional tasks can be costly.
The ``fftw3`` library solves the second disadvantage by caching the
measured performance with a hash table.
This is overkill for ``lux`` (at least for now).

Instead, ``lux`` uses a more flexible and lightweight, but parameter
dependent, abstraction "plan" that mirrors the "new-array execute"
functions in ``fftw3`` such as ``fftw_execute_dft()`` and
``fftw_execute_split_dft()``.
Packing problem specific ``ispec`` and ``xspec`` into a type-safe
generic pointer ``Lux_spec``, and problem specific ``params``, ``ro``,
``rw``, and ``wo`` into another type-safe generic pointer
``Lux_conf``, ``lux`` plans and tasks can be defined by the following
structures:

.. code-block:: c

   struct plan_s {
   	void (*algo)(Lux_spec, Lux_conf);
   	Lux_spec spec;
   };

   struct task_s {
   	struct plan_s plan;
   	Lux_conf conf;
   };

To summarize, a ``plan``, a ``task``, and other ``lux`` data
structures need to keep at least the following information:

+-------+----+------+-----+-----+----+--+--+--+------+-----+-----+
|       |problem          |solution  |data    |performance       |
+       +----+------+-----+-----+----+--+--+--+-----+-----+------+
|       |type|params|xspec|ispec|algo|ro|rw|wo|opcnt|cost |status|
+=======+====+======+=====+=====+====+==+==+==+=====+=====+======+
|case   |v   |?     |v    |     |    |  |  |  |     |     |      |
+-------+----+------+-----+-----+----+--+--+--+-----+-----+------+
|plan   |    |      |v    |v    |v   |  |  |  |     |     |      |
+-------+----+------+-----+-----+----+--+--+--+-----+-----+------+
|memory |    |      |     |     |    |? |? |? |     |     |      |
+-------+----+------+-----+-----+----+--+--+--+-----+-----+------+
|profile|    |      |&plan           |  |  |  |v    |v    |      |
+-------+----+------+-----+-----+----+--+--+--+-----+-----+------+
|task   |    |v     |&plan           |&memory |     |     |      |
+-------+----+------+-----+-----+----+--+--+--+-----+-----+------+
|event  |    |&task                           |     |v    |v     |
+-------+----+------+-----+-----+----+--+--+--+-----+-----+------+

In the above table, ``?`` and ``&`` stand for optional properties and
reference, respectively; ``opcnt`` stands for floating-point operation
count; ``cost`` stands for the estimated and/or measured computation
cost; and ``status`` stands for the status of a task in a queue.
