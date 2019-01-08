#pragma once

template <typename T>
K::APTR K::ObjectManager::CreateActor(TAG const& _tag)
{
	auto actor = APTR{ new T, [](T* _p) {
		_p->_Finalize();
		delete _p;
	} };

	actor->set_tag(_tag);

	actor->Initialize();

	return actor;
}

template <typename T>
K::CPTR K::ObjectManager::CreateComponent(TAG const& _tag)
{
	auto component = CPTR{ new T, [](Component* _p) {
		static_cast<T*>(_p)->_Finalize();
		delete static_cast<T*>(_p);
	} };

	component->set_tag(_tag);

	component->Initialize();

	return component;
}
