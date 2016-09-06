__kernel void
twice(__global real *y, __global real *x)
{
	int i = get_global_id(0);
	y[i] = 2 * x[i];
}
