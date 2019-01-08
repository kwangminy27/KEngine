#include "Engine.h"
#include "texture.h"

#include "device_manager.h"
#include "path_manager.h"

void K::Texture::SetToShader(int _slot)
{
	DeviceManager::singleton()->context()->PSSetShaderResources(_slot, 1, SRV_.GetAddressOf());
}

K::Texture::Texture(Texture const& _other)
{
	texture_ = _other.texture_;
	SRV_ = _other.SRV_;
}

K::Texture::Texture(Texture&& _other) noexcept
{
	texture_ = std::move(_other.texture_);
	SRV_ = std::move(_other.SRV_);
}

void K::Texture::_CreateTexture2D(std::wstring const& _file_name, std::string const& _path_tag)
{
	auto const& device = DeviceManager::singleton()->device();

	std::filesystem::path path_buffer = PathManager::singleton()->FindPath(_path_tag);

	if (path_buffer.empty())
		throw std::exception{ "Texture::_CreateTexture2D" };

	path_buffer /= _file_name;

	Microsoft::WRL::ComPtr<ID3D11Resource> resource{};

	if (path_buffer.extension() == ".dds")
		ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device.Get(), path_buffer.c_str(), &resource, &SRV_));
	else
		ThrowIfFailed(DirectX::CreateWICTextureFromFile(device.Get(), path_buffer.c_str(), &resource, &SRV_));

	ThrowIfFailed(resource.As(&texture_));
}
