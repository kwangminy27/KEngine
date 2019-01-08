#include "Engine.h"
#include "audio_manager.h"

#include "path_manager.h"

std::unique_ptr<DirectX::SoundEffect> K::AudioManager::sound_effect_dummy_{};
std::unique_ptr<DirectX::SoundEffectInstance> K::AudioManager::sound_effect_instance_dummy_{};

void K::AudioManager::Initialize()
{
	try
	{
		DirectX::AUDIO_ENGINE_FLAGS audio_engine_flags = DirectX::AudioEngine_Default;
#ifdef _DEBUG
		audio_engine_flags = audio_engine_flags | DirectX::AudioEngine_Debug;
#endif
		audio_engine_ = std::make_unique<DirectX::AudioEngine>(audio_engine_flags);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "AudioManager::Initialize" << std::endl;
	}
}

void K::AudioManager::Update()
{
	audio_engine_->Update();
}

void K::AudioManager::Suspend()
{
	audio_engine_->Suspend();
}

void K::AudioManager::Resume()
{
	audio_engine_->Resume();
}

std::unique_ptr<DirectX::SoundEffect> const& K::AudioManager::FindSoundEffect(std::string const& _tag) const
{
	auto iter = sound_effect_map_.find(_tag);

	if (iter == sound_effect_map_.end())
		return sound_effect_dummy_;

	return iter->second;
}

std::unique_ptr<DirectX::SoundEffectInstance> const& K::AudioManager::FindSoundEffectInstance(std::string const& _tag) const
{
	auto iter = sound_effect_instance_map_.find(_tag);

	if (iter == sound_effect_instance_map_.end())
		return sound_effect_instance_dummy_;

	return iter->second;
}

void K::AudioManager::AddSoundEffectInstance(std::string const& _tag, std::unique_ptr<DirectX::SoundEffectInstance> _sound_effect_instance)
{
	sound_effect_instance_map_.insert(std::make_pair(_tag, std::move(_sound_effect_instance)));
}

void K::AudioManager::RemoveSoundEffectInstance(std::string const& _tag)
{
	auto iter = sound_effect_instance_map_.find(_tag);

	if (iter == sound_effect_instance_map_.end())
		return;

	sound_effect_instance_map_.erase(iter);
}

void K::AudioManager::_Finalize()
{
}

void K::AudioManager::_CreateSoundEffect(std::string const& _tag, std::wstring const& _file_name, std::string const& _path_tag)
{
	if (FindSoundEffect(_tag))
		throw std::exception{ "AudioManager::_CrateSoundEffect" };

	std::filesystem::path path_buffer = PathManager::singleton()->FindPath(_path_tag);

	if (path_buffer.empty())
		throw std::exception{ "AudioManager::_CrateSoundEffect" };

	path_buffer /= _file_name;

	auto sound_effect = std::make_unique<DirectX::SoundEffect>(audio_engine_.get(), path_buffer.c_str());

	sound_effect_map_.insert(make_pair(_tag, std::move(sound_effect)));
}
