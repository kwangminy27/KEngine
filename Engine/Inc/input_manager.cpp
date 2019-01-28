#include "Engine.h"
#include "input_manager.h"

#include "core.h"

std::unique_ptr<K::KEY_STATE, std::function<void(K::KEY_STATE*)>> K::InputManager::key_state_dummy_{};

void K::InputManager::Initialize()
{
}

void K::InputManager::Initialize(HINSTANCE _instance, HWND _window)
{
	try
	{
		Initialize();

		ThrowIfFailed(DirectInput8Create(_instance, DIRECTINPUT_VERSION, IID_IDirectInput8, &input_, nullptr));
		ThrowIfFailed(input_->CreateDevice(GUID_SysKeyboard, &keyboard_, nullptr));
		ThrowIfFailed(keyboard_->SetCooperativeLevel(_window, DISCL_FOREGROUND | DISCL_EXCLUSIVE));
		ThrowIfFailed(keyboard_->SetDataFormat(&c_dfDIKeyboard));
		ThrowIfFailed(keyboard_->Acquire());

		std::vector<uint8_t> combination_key{};
		combination_key.push_back(DIK_LSHIFT);
		_CreateKeyState(std::string{ "ESC" }, DIK_ESCAPE, combination_key);
		combination_key.clear();
		_CreateKeyState(std::string{ "F1" }, DIK_F1, combination_key);
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

void K::InputManager::Update(float _time)
{
	_UpdateKeyboard(_time);
}

bool K::InputManager::KeyDown(std::string const& _tag) const
{
	return _FindKeyState(_tag)->down;
}

bool K::InputManager::KeyPress(std::string const& _tag) const
{
	return _FindKeyState(_tag)->press;
}

bool K::InputManager::KeyUp(std::string const& _tag) const
{
	return _FindKeyState(_tag)->up;
}

void K::InputManager::_Finalize()
{
}

void K::InputManager::_UpdateKeyboard(float _time)
{
	auto result = keyboard_->GetDeviceState(256, &keyboard_state_.key);

	if (FAILED(result))
	{
		if (DIERR_INPUTLOST == result || DIERR_NOTACQUIRED)
			keyboard_->Acquire();
		else
			throw std::exception{ "InputManager::_UpdateKeyboard" };
	}

	for (auto i = 0; i < MAX_KEY_SIZE; ++i)
	{
		if (keyboard_state_.key[i] & 0x80)
		{
			if (false == keyboard_state_.press[i])
			{
				keyboard_state_.down[i] = true;
				keyboard_state_.press[i] = true;
			}
			else
				keyboard_state_.down[i] = false;
		}
		else if (keyboard_state_.press[i])
		{
			keyboard_state_.down[i] = false;
			keyboard_state_.press[i] = false;
			keyboard_state_.up[i] = true;
		}
		else if (keyboard_state_.up[i])
			keyboard_state_.up[i] = false;
	}

	for (auto& key_state : key_state_map_)
	{
		int count{};

		for (auto const& key : key_state.second->combination_key)
		{
			if (keyboard_state_.press[key])
				++count;
		}

		if (keyboard_state_.press[key_state.second->key] && count == key_state.second->combination_key.size())
		{
			if (false == key_state.second->press)
			{
				key_state.second->down = true;
				key_state.second->press = true;
			}
			else
				key_state.second->down = false;
		}
		else if (key_state.second->press)
		{
			key_state.second->down = false;
			key_state.second->press = false;
			key_state.second->up = true;
		}
		else if (key_state.second->up)
			key_state.second->up = false;
	}
}

std::unique_ptr<K::KEY_STATE, std::function<void(K::KEY_STATE*)>> const& K::InputManager::_FindKeyState(std::string const& _tag) const
{
	auto iter = key_state_map_.find(_tag);

	if (iter == key_state_map_.end())
		return key_state_dummy_;

	return iter->second;
}

void K::InputManager::_CreateKeyState(std::string const& _tag, uint8_t _key, std::vector<uint8_t> const& _combination_key)
{
	if (_FindKeyState(_tag))
		throw std::exception{ "InputManager::_CreateKeyState" };

	auto key_state = std::unique_ptr<KEY_STATE, std::function<void(KEY_STATE*)>>{ new KEY_STATE, [](KEY_STATE* _p) {
		delete _p;
	} };

	key_state->down = false;
	key_state->press = false;
	key_state->up = false;
	key_state->key = _key;
	key_state->combination_key = _combination_key;

	key_state_map_.insert(std::make_pair(_tag, std::move(key_state)));
}
