Design
======


Resource Abstraction
--------------------

Modern heterogeneous distributed systems can be extremely complex.
To control the complexity and let developers focus on a manageable
amount of details, ``lux`` uses a hierarchical approach for resource
management.

Physically related hardware, e.g., different computing devices in a
node, are grouped together into an abstract resource "instance"
``Lux_instance``.
Multiple instances of ``Lux_instance`` are then arranged in a
hierarchical tree to hide the complexity of the whole system.

Therefore, a heterogeneous distributed system may be represented in
the following tree::

   node0: ins-+->ins_node0-+->ins_node0_device0
              |            +->ins_node0_device1
              |            +->...
              |
   node1:     +->ins_node1-+->ins_node1_device0
              |            +->ins_node1_device1
              |            +->...
              |
              +->...-------+->...
	                   +->...
			   +->...

By combining this resource abstraction with dynamic task-based
parallelism, developers simply need to break down a complex parent
task into multiple child tasks, and enqueue them to the right ``ins``
object with proper dependency.
This greatly simplify the development of parallel algorithms.

Each ``ins`` may have different properties.
For example, MPI requires only the first node to have the capability
to perform I/O.
It is therefore possible to flag all other nodes as "compute-only",
and let ``lux`` manage data transfer through network before writing
them to storage through the first node.
In fact, it is possible that different nodes use different CPU
architectures.
Each ``ins`` can then be configured to load modules compiled for the
appropriate CPU architectures.

With this resource abstraction, ``ins`` has to implement the following
interface:

.. code-block:: c

   typedef struct LuxSinstance *Lux_instance;

   struct LuxSinstance {
   	unsigned    capability; /* compute, module loading, I/O, etc */
   	void      (*log   )(Lux_instance, const char *, ...);
   	...
   	Lux_event (*submit)(Lux_instance, Lux_event [], Lux_task);
   	void      (*wait  )(Lux_instance, Lux_event []);
   	...
   };

which simply enables logging message, submitting tasks, and waiting
for events.
The actual hardware dependent codes are left as implementation
details.

Because different nodes may have different hardware architectures, it
does not make sense to create tasks on the parent ``ins`` instance and
distribute it to the children ``ins`` instances.
Instead, ``ins`` would hold a portable description of the algorithm
requirement and send it to the children ``ins`` instances.
Each child ``ins`` can then construct their own algorithms to optimize
for the performance.
This suggests that each child ``ins`` may need to dynamically load its
own ``lux`` modules.
