#include "Engine.h"
#include "time_manager.h"

void K::TimeManager::Initialize()
{
	try
	{
		last_time_point_ = std::chrono::high_resolution_clock::now();
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "TimeManager::Initialize" << std::endl;
	}
}

void K::TimeManager::Update()
{
	static uint32_t FPS{};
	static float accumulated_time{};

	auto duration = std::chrono::high_resolution_clock::now() - last_time_point_;
	
	time_delta_ = duration.count() * 0.000'000'001f;

	last_time_point_ = std::chrono::high_resolution_clock::now();

	++FPS;
	accumulated_time += time_delta_;

	if (accumulated_time >= 1.f)
	{
		accumulated_time -= 1.f;
		FPS_ = FPS;
		FPS = 0;
	}
}

float K::TimeManager::time_delta() const
{
	return time_delta_;
}

uint32_t K::TimeManager::FPS() const
{
	return FPS_;
}

void K::TimeManager::_Finalize()
{
}
