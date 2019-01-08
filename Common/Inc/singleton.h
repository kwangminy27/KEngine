#pragma once

namespace K
{
	template <typename T>
	class Singleton
	{
	public:
		static std::unique_ptr<T, std::function<void(T*)>>& singleton();

		virtual void Initialize() = 0;

	protected:
		Singleton() = default;
		Singleton(Singleton const&) = delete;
		Singleton(Singleton&&) noexcept = delete;
		Singleton& operator=(Singleton const&) = delete;
		Singleton& operator=(Singleton&&) noexcept = delete;

		virtual void _Finalize() = 0;

	private:
		static std::once_flag once_flag_;
		static std::unique_ptr<T, std::function<void(T*)>> singleton_;
	};
}

#include "singleton.inl"
