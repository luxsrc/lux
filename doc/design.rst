Design
======


Resource Abstraction
--------------------

Modern heterogeneous distributed systems can be extremely complex.
To control the complexity and let developers focus on a manageable
amount of details, ``lux`` uses a hierarchical approach for resource
management.

Physically related hardware, e.g., different computing devices in a
node, are grouped together into a resource abstraction ``Lux_libra``.
Multiple instances of ``Lux_libra``, or ``lra``s, are then arranged in
a hierarchical tree to hide the complexity of the whole system.

Therefore, a heterogeneous distributed system may be represented in
the following tree::

   node0: lra-+->lra_node0-+->lra_node0_device0
              |            +->lra_node0_device1
              |            +->...
              |
   node1:     +->lra_node1-+->lra_node1_device0
              |            +->lra_node1_device1
              |            +->...
              |
              +->...-------+->...
                           +->...
                           +->...

By combining this resource abstraction with dynamic task-based
parallelism, developers simply need to break down a complex parent
task into multiple child tasks, and enqueue them to the right ``lra``
object with proper dependency.
This greatly simplify the development of parallel algorithms.

Each ``lra`` may have different properties.
For example, MPI requires only the first node to have the capability
to perform I/O.
It is therefore possible to flag all other nodes as "compute-only",
and let ``lux`` manage data transfer through network before writing
them to storage through the first node.
In fact, it is possible that different nodes use different CPU/GPU
architectures.
Each ``lra`` can then be configured to load modules compiled for the
appropriate CPU architectures.

With this resource abstraction, ``lra`` has to implement the following
interface:

.. code-block:: c

   typedef struct Lux_libra_s *Lux_libra;

   struct Lux_libra_s {
   	void  (*vlog)(Lux_libra, unsigned, const char *, va_list);
   	void *(*load)(Lux_libra, const char *, const void *);
	int   (*exec)(Lux_libra, Lux_task);
   };

which simply enables logging message and running tasks.
The actual hardware dependent codes are left as implementation
details.

Because different nodes may have different hardware architectures, it
does not make sense to create tasks on the parent ``lra`` instance and
distribute it to the children ``lra`` instances.
Instead, ``lra`` would hold a portable description of the algorithm
requirement and send it to the children ``lra`` instances.
Each child ``lra`` can then construct their own algorithms to optimize
for the performance.
This suggests that each child ``lra`` may need to dynamically load its
own ``lux`` modules.
