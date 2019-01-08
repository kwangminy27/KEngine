#pragma once

template <typename T>
void K::WorldManager::CreateLevel(TAG const& _tag)
{
	auto level = std::shared_ptr<Level>{ new T, [](T* _p) {
		_p->_Finalize();
		delete _p;
	} };

	level->set_tag(_tag);

	level->Initialize();

	level_list_.push_back(std::move(level));
}
