#ifndef R3DVOXEL_THREAD_POOL_HPP
#define R3DVOXEL_THREAD_POOL_HPP

#include <mutex>
#include <vector>
#include <thread>

struct thread_pool_t
{
	std::vector<std::thread> workers;
	std::mutex mutex;
	
	void init(size_t n = std::thread::hardware_concurrency());
	void worker();
};

#endif // R3D_THREAD_POOL_HPP
