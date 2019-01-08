#include "Engine.h"
#include "input_manager.h"

std::pair<std::string, K::KEY_DESC> K::InputManager::key_desc_dummy_{};

void K::InputManager::Initialize()
{
	try
	{
		_CreateKeyDesc(std::string{ "ESC" }, VK_ESCAPE);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "InputManager::Initialize" << std::endl;
	}
}

void K::InputManager::Update()
{
	for (auto iter = key_desc_map_.begin(); iter != key_desc_map_.end(); ++iter)
	{
		auto& key_desc = iter->second;

		int count{};
		for (auto const& key : key_desc.key_vector)
		{
			if (GetAsyncKeyState(key) & 0x8000)
				++count;
		}

		if (count == key_desc.key_vector.size())
		{
			if (!key_desc.pressed)
			{
				key_desc.down = true;
				key_desc.pressed = true;
			}
			else
				key_desc.down = false;
		}
		else if (key_desc.pressed)
		{
			key_desc.down = false;
			key_desc.pressed = false;
			key_desc.up = true;
		}
		else if (key_desc.up)
			key_desc.up = false;
	}
}

bool K::InputManager::KeyDown(std::string const& _tag) const
{
	return _FindKeyDesc(_tag).down;
}

bool K::InputManager::KeyPressed(std::string const& _tag) const
{
	return _FindKeyDesc(_tag).pressed;
}

bool K::InputManager::KeyUp(std::string const& _tag) const
{
	return _FindKeyDesc(_tag).up;
}

void K::InputManager::_Finalize()
{
}

K::KEY_DESC const& K::InputManager::_FindKeyDesc(std::string const& _tag) const
{
	auto iter = key_desc_map_.find(_tag);

	if (iter == key_desc_map_.end())
		return key_desc_dummy_.second;

	return iter->second;
}

void K::InputManager::_CreateKeyDesc()
{
	key_desc_map_.insert(std::move(key_desc_dummy_));
}
