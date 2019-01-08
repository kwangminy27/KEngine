#pragma once

template <typename T>
std::once_flag K::Singleton<T>::once_flag_{};

template <typename T>
std::unique_ptr<T, std::function<void(T*)>> K::Singleton<T>::singleton_{};

template <typename T>
std::unique_ptr<T, std::function<void(T*)>>& K::Singleton<T>::singleton()
{
	std::call_once(once_flag_, []() {
		singleton_ = std::unique_ptr<T, std::function<void(T*)>>{ new T, [](T* _p) {
			_p->_Finalize();
			delete _p;
		} };
	});

	return singleton_;
}
