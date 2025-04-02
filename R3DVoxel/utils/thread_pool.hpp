#ifndef R3DVOXEL_THREAD_POOL_HPP
#define R3DVOXEL_THREAD_POOL_HPP

#include <mutex>
#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <future>

struct thread_pool_t
{
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> queue;
	std::mutex mutex;
	std::condition_variable cv;
	bool stop = false;
	
	thread_pool_t(size_t n = std::thread::hardware_concurrency());
	~thread_pool_t();

	thread_pool_t(const thread_pool_t&) = delete;
	thread_pool_t& operator=(const thread_pool_t&) = delete;
	thread_pool_t(thread_pool_t&&) = delete;
	thread_pool_t& operator=(thread_pool_t&&) = delete;

	static thread_pool_t& instance();
	void worker();

	template <typename F, typename... Args>
	inline auto enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
	{
		auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
		auto encapsulated_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

		std::future<decltype(f(args...))> future_object = encapsulated_ptr->get_future();
		{
			std::unique_lock<std::mutex> lock(mutex);
			queue.emplace(
			    [encapsulated_ptr]()
			    {
				    (*encapsulated_ptr)(); // execute the fx
			    });
		}
		cv.notify_one();
		return future_object;
	}
};

#endif // R3D_THREAD_POOL_HPP
