Design
======


Error/Exception Handling
------------------------

The terms "error" here refers to a recoverable and expected runtime
error.
It is the type of errors typically handled by "exceptions" in object
oriented languages such as C++ and Java.
Examples include "out of memory" by allocating memory and "permission
denied" by opening a file.
It is not a programming bug that the developer can eliminate.
However, ignoring such a runtime error in ``lux`` is considered a bug.
We use the C++/Java term "exception" in ``lux`` for recoverable
runtime errors to avoid confusions.

In C, ad-hoc special values are used to indicate exceptions in the
returned variable of functions.
Once an exception is identified, the special global variable ``errno``
is checked to obtain the detail of the exception.
This overcomes the limitation that a C function can only return a
single variable, and it ensures performance because only a single
integer is set in order to indicating the exception.
Usually, a message is then logged before other action is taken to
resolve the exception:

.. code-block:: c

   int fid = open(path, flags, mode);
   if(fid < 0) { /* handle exception */
   	int err = errno;
   	(void)fprintf(stderr,
   	              "Failed to open \"%s\" [%d: %s]\n",
   	              path, err, strerror(err));
   	switch(err) {
   	... /* other error handling actions */
   	}
   }

In ``lux``, the static module ``<lux/exception.h>`` enhances the
standard ``<errno.h>`` mechanism.
This module defines the macro

.. code-block:: c

   	#define exception errno

which, expands to ``errno``, is an lvalue and has type ``int``.
The maximum defined system exception code (unified as ``LUX_ELAST`` in
``lux``, which is equal to
``ELAST`` for BSD and Darwin,
``EHWPOISON`` for current Linux on most architectures,
``ERFKILL`` for old Linux, and
``EDQUOT`` for Linux/MIPS)
is usually only a few hundreds.
Hence, in addition to the system exception codes, ``exception`` can
encode additional information.

How many bits can we use to encode the additional information?
It is architecture and platform dependent.
As far as we can tell, BSD-based systems have ``ELAST`` up to 106
(Darwin).
For Linux, ``EHWPOISON`` is 133 on generic architectures.
It can take larger values on SPARC (135), Alpha (139), MIPS (168), and
PA-RISC (257).
For Linux/MIPS, ``EDQUOT`` is 1133.
Nevertheless, all these architectures (except generic) are 32-bit or
64-bit so there are at least 21 remaining bits.
If we want ``lux`` to support all the platforms that have C compilers,
our constraint really comes from the size of ``int``, which is only
guaranteed to be 16-bit.
As long as we only allow 122 = 255 - 133 customized exception codes,
we have at least 8 bits to encode additional information even for the
ancient 16-bit systems.

For most applications in high-performance computing, we need a
detailed content dependent message to help debugging the algorithm.
We suggest using a unique string id (e.g., message count, hash key) to
identify a cached message, and bitwise-OR-ing the id with an optional
facility key and the exception code described in this section.
This string id can be architecture, platform, and implementation
dependent.


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
In fact, it is possible that different nodes use different CPU/GPU
architectures.
Each ``ins`` can then be configured to load modules compiled for the
appropriate CPU architectures.

With this resource abstraction, ``ins`` has to implement the following
interface:

.. code-block:: c

   typedef struct LuxSinstance *Lux_instance;

   struct LuxSinstance {
   	void     (*log)(Lux_instance, const char *, ...);
   	Lux_task (*que)(Lux_instance, Lux_task [], Lux_task);
   };

which simply enables logging message and running tasks.
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
