Design
======


Resource Abstraction
--------------------

Modern heterogeneous distributed systems can be extremely complex.
To control the complexity and let developers focus on a manageable
amount of details, ``lux`` uses a hierarchical approach for resource
management.

Physically related hardware, e.g., different computing devices in a
node, are grouped together into a resource abstraction ``Lux_libux``.
Multiple instances of ``Lux_libux`` are then arranged in a
hierarchical tree to hide the complexity of the whole system.

Therefore, a heterogeneous distributed system may be represented in
the following tree::

   node0: lux-+->lux_node0-+->lux_node0_device0
              |            +->lux_node0_device1
              |            +->...
              |
   node1:     +->lux_node1-+->lux_node1_device0
              |            +->lux_node1_device1
              |            +->...
              |
              +->...-------+->...
                           +->...
                           +->...

By combining this resource abstraction with dynamic task-based
parallelism, developers simply need to break down a complex parent
task into multiple child tasks, and enqueue them to the right ``lux``
object with proper dependency.
This greatly simplify the development of parallel algorithms.

Each ``lux`` may have different properties.
For example, MPI requires only the first node to have the capability
to perform I/O.
It is therefore possible to flag all other nodes as "compute-only",
and let ``lux`` manage data transfer through network before writing
them to storage through the first node.
In fact, it is possible that different nodes use different CPU/GPU
architectures.
Each ``lux`` can then be configured to load modules compiled for the
appropriate CPU architectures.

With this resource abstraction, ``lux`` has to implement the following
interface:

.. code-block:: c

   typedef struct Lux_libux_s *Lux_libux;

   struct Lux_libux_s {
   	void  (*vlog)(Lux_libux, unsigned, const char *, va_list);
   	void *(*load)(Lux_libux, const char *, const void *);
	int   (*exec)(Lux_libux, Lux_task);
   };

which simply enables logging message, loading modules, and running
tasks.
The actual hardware dependent codes are left as implementation
details.

Because different nodes may have different hardware architectures, it
does not make sense to create tasks on the parent ``lux`` instance and
distribute it to the children ``lux`` instances.
Instead, ``lux`` would hold a portable description of the algorithm
requirement and send it to the children ``lux`` instances.
Each child ``lux`` can then recursive pass the description or
construct their own algorithms to optimize for the performance.
This suggests that each child ``lux`` may need to dynamically load its
own modules.
