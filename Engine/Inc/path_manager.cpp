#include "Engine.h"
#include "path_manager.h"

std::filesystem::path K::PathManager::path_dummy_{};

void K::PathManager::Initialize()
{
	try
	{
		_CreateRootPath();
		_CreatePath(AUDIO_PATH, "Audio\\");
		_CreatePath(VIDEO_PATH, "Video\\");
		_CreatePath(TEXTURE_PATH, "Texture\\");
		_CreatePath(SHADER_PATH, "Shader\\");
		_CreatePath(FBX_PATH, "FBX\\");
		_CreatePath(MESH_PATH, "MESH\\");
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "PathManager::Initialize" << std::endl;
	}
}

std::filesystem::path const& K::PathManager::FindPath(std::string const& _tag) const
{
	auto iter = path_map_.find(_tag);

	if (iter == path_map_.end())
		return path_dummy_;

	return iter->second;
}

void K::PathManager::_Finalize()
{
}

void K::PathManager::_CreateRootPath()
{
	std::filesystem::path path_buffer = std::filesystem::current_path();

	bool is_resource_directory{};
	while (false == is_resource_directory)
	{
		path_buffer = path_buffer.parent_path();

		for (auto iter = std::filesystem::begin(std::filesystem::directory_iterator{ path_buffer });
			iter != std::filesystem::end(std::filesystem::directory_iterator{ path_buffer }); ++iter)
		{
			if (iter->path().filename() == "Resource")
			{
				is_resource_directory = true;
				break;
			}
		}

		if (is_resource_directory)
			break;
	}

	path_buffer /= "Resource\\";

	_CreatePath(ROOT_PATH, path_buffer, "");
}

void K::PathManager::_CreatePath(std::string const& _tag, std::filesystem::path const& _path, std::string const& _base_path_tag)
{
	if (!FindPath(_tag).empty())
		throw std::exception{ "PathManager::_CreatePath" };

	path_map_.insert(std::make_pair(_tag, FindPath(_base_path_tag) / _path));
}
