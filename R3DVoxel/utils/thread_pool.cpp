#include "thread_pool.hpp"

void thread_pool_t::init(size_t n)
{
	workers.reserve(n);
	for (size_t i = 0; i < n; ++i)
	{
		workers.emplace_back(&thread_pool_t::worker, this);
	}
}

void thread_pool_t::worker()
{
}
