#include "Engine.h"
#include "input_manager.h"

#include "core.h"

std::unique_ptr<K::KEY_AXIS, std::function<void(K::KEY_AXIS*)>> K::InputManager::key_axis_dummy_{};
std::unique_ptr<K::KEY_ACTION, std::function<void(K::KEY_ACTION*)>> K::InputManager::key_action_dummy_{};

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

		_CreateKeyAction(std::string{ "ESC" }, DIK_ESCAPE);


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

	for (auto const& key_axis : key_axis_map_)
	{
		if (keyboard_state_.press[key_axis.second->info.key])
			key_axis.second->callback(key_axis.second->info.scale, _time);
	}

	for (auto& key_action : key_action_map_)
	{
		int count{};

		for (auto const& key : key_action.second->info.combination_key)
		{
			if (keyboard_state_.key[key] & 0x80)
				++count;
		}

		if (keyboard_state_.press[key_action.second->info.key] && count == key_action.second->info.combination_key.size())
		{
			if (false == key_action.second->info.press)
			{
				key_action.second->info.down = true;
				key_action.second->info.press = true;
			}
			else
				key_action.second->info.down = false;
		}
		else if (key_action.second->info.press)
		{
			key_action.second->info.down = false;
			key_action.second->info.press = false;
			key_action.second->info.up = true;
		}
		else if (key_action.second->info.up)
			key_action.second->info.up = false;

		// 무조건 처리하는 중인데 이러면 안됨
		for (auto const& callback : key_action.second->callback_array)
		{
			if (callback)
				callback(_time);
		}
	}
}

std::unique_ptr<K::KEY_AXIS, std::function<void(K::KEY_AXIS*)>> const& K::InputManager::_FindKeyAxis(std::string const& _tag) const
{
	auto iter = key_axis_map_.find(_tag);

	if (iter == key_axis_map_.end())
		return key_axis_dummy_;

	return iter->second;
}

std::unique_ptr<K::KEY_ACTION, std::function<void(K::KEY_ACTION*)>> const& K::InputManager::_FindKeyAction(std::string const& _tag) const
{
	auto iter = key_action_map_.find(_tag);

	if (iter == key_action_map_.end())
		return key_action_dummy_;

	return iter->second;
}

void K::InputManager::_CreateKeyAxis(std::string const& _tag, uint8_t _key, float _scale)
{
	if (_FindKeyAxis(_tag))
		throw std::exception{ "InputManager::_CreateKeyAxis" };

	auto key_axis = std::unique_ptr<KEY_AXIS, std::function<void(KEY_AXIS*)>>{ new KEY_AXIS, [](KEY_AXIS* _p) {
		delete _p;
	} };

	key_axis->info.key = _key;
	key_axis->info.scale = _scale;

	key_axis_map_.insert(std::make_pair(_tag, std::move(key_axis)));
}

// 콤비네이션 키 추가 해야 함
void K::InputManager::_CreateKeyAction(std::string const& _tag, uint8_t _key)
{
	if (_FindKeyAction(_tag))
		throw std::exception{ "InputManager::_CreateKeyAction" };

	auto key_action = std::unique_ptr<KEY_ACTION, std::function<void(KEY_ACTION*)>>{ new KEY_ACTION, [](KEY_ACTION* _p) {
		delete _p;
	} };

	key_action->info.key = _key;

	key_action_map_.insert(std::make_pair(_tag, std::move(key_action)));
}
