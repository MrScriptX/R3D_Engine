#include "thread_pool.hpp"

#include <type_traits>

thread_pool_t::thread_pool_t(size_t n)
{
	workers.reserve(n);
	for (size_t i = 0; i < n; ++i)
	{
		workers.emplace_back(&thread_pool_t::worker, this);
	}
}

thread_pool_t::~thread_pool_t()
{
	{
		std::unique_lock<std::mutex> lock(mutex);
		stop = true;
	}

	cv.notify_all();
	for (auto& worker : workers)
	{
		worker.join();
	}
}

thread_pool_t& thread_pool_t::instance()
{
	static thread_pool_t instance;
	return instance;
}

void thread_pool_t::worker()
{
	for (;;)
	{
		std::function<void()> cur_task;
		{
			std::unique_lock<std::mutex> lock(mutex);
			cv.wait(lock, [this]() { return stop || !queue.empty(); });

			if (stop && queue.empty())
				break;

			if (queue.empty())
				continue;

			cur_task = queue.front();
			queue.pop();
			// grab the fx from queue
		}
		cur_task();
	}
}
