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
	
	static thread_pool_t& instance();

	void init(size_t n = std::thread::hardware_concurrency());
	void end();

	void worker();

	template <typename F, typename... Args>
	inline auto enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))>;
};

#endif // R3D_THREAD_POOL_HPP
