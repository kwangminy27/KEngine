#pragma once

template <typename T, typename... Types>
void K::InputManager::_CreateKeyDesc(T const& _key, Types... _Args)
{
	key_desc_dummy_.second.key_vector.push_back(_key);

	_CreateKeyDesc(_Args...);
}

template <typename... Types>
void K::InputManager::_CreateKeyDesc(std::string const& _tag, Types... _Args)
{
	key_desc_dummy_.first = _tag;

	_CreateKeyDesc(_Args...);
}