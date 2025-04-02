#include "thread_pool.hpp"

#include <type_traits>

thread_pool_t& thread_pool_t::instance()
{
	static thread_pool_t instance;
	return instance;
}

void thread_pool_t::init(size_t n)
{
	workers.reserve(n);
	for (size_t i = 0; i < n; ++i)
	{
		workers.emplace_back(&thread_pool_t::worker, this);
	}
}

void thread_pool_t::end()
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

template <typename F, typename... Args>
inline auto thread_pool_t::enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
{
	auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
	auto encapsulated_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

	std::future<std::invoke_result<F, Args...>> future_object = encapsulated_ptr->get_future();
	{
		std::unique_lock<std::mutex> lock(mutex);
		queue.emplace([encapsulated_ptr]() {
			(*encapsulated_ptr)(); // execute the fx
		});
	}
	cv.notify_one();
	return future_object;
}
