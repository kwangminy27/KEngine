#include "Engine.h"
#include "shader.h"

#include "device_manager.h"
#include "path_manager.h"

void K::Shader::SetToShader()
{
	auto const& context = DeviceManager::singleton()->context();

	context->IASetInputLayout(input_layout_.Get());
	context->VSSetShader(VS_.Get(), nullptr, 0);
	context->PSSetShader(PS_.Get(), nullptr, 0);
}

K::Shader::Shader(Shader const& _other)
{
	*this = _other;
}

K::Shader::Shader(Shader&& _other) noexcept
{
	*this = std::move(_other);
}

void K::Shader::_CreateShader(
	std::vector<CSO_DESC> const& _cso_desc_vector,
	std::vector<D3D11_INPUT_ELEMENT_DESC> const& _input_element_desc_vector,
	std::string const& _path_tag)
{
	for (auto const& cso_desc : _cso_desc_vector)
	{
		switch (cso_desc.type)
		{
		case SHADER_TYPE::VERTEX:
			_CreateVertexShader(cso_desc.file_name, _input_element_desc_vector, _path_tag);
			break;

		case SHADER_TYPE::PIXEL:
			_CreatePixelShader(cso_desc.file_name, _path_tag);
			break;
		}
	}
}

void K::Shader::_CreateVertexShader(
	std::wstring const& _file_name,
	std::vector<D3D11_INPUT_ELEMENT_DESC> const& _input_element_desc_vector,
	std::string const& _path_tag)
{
	auto const& device = DeviceManager::singleton()->device();

	std::filesystem::path path_buffer = PathManager::singleton()->FindPath(_path_tag);

	if (path_buffer.empty())
		throw std::exception{ "Shader::_CreateVertexShader" };

	path_buffer /= _file_name;

	std::ifstream cso{ path_buffer, std::ios::ate | std::ios::binary };

	if(cso.fail())
		throw std::exception{ "Shader::_CreateVertexShader" };

	auto size = cso.tellg();

	cso.seekg(0, std::ios::beg);

	auto buffer = std::unique_ptr<uint8_t[], std::function<void(uint8_t*)>>{ new uint8_t[size], [](uint8_t* _p) {
		delete[] _p;
	} };

	cso.read(reinterpret_cast<char*>(buffer.get()), size);

	ThrowIfFailed(device->CreateInputLayout(
		_input_element_desc_vector.data(),
		static_cast<UINT>(_input_element_desc_vector.size()),
		reinterpret_cast<void const*>(buffer.get()),
		static_cast<SIZE_T>(size),
		&input_layout_
	));

	ThrowIfFailed(device->CreateVertexShader(
		buffer.get(),
		static_cast<SIZE_T>(size),
		nullptr,
		&VS_
	));
}

void K::Shader::_CreatePixelShader(std::wstring const& _file_name, std::string const& _path_tag)
{
	auto const& device = DeviceManager::singleton()->device();

	std::filesystem::path path_buffer = PathManager::singleton()->FindPath(_path_tag);

	if (path_buffer.empty())
		throw std::exception{ "Shader::_CreatePixelShader" };

	path_buffer /= _file_name;

	std::ifstream cso{ path_buffer, std::ios::ate | std::ios::binary };

	if (cso.fail())
		throw std::exception{ "Shader::_CreatePixelShader" };

	auto size = cso.tellg();

	cso.seekg(0, std::ios::beg);

	auto buffer = std::unique_ptr<uint8_t[], std::function<void(uint8_t*)>>{ new uint8_t[size], [](uint8_t* _p) {
		delete[] _p;
	} };

	cso.read(reinterpret_cast<char*>(buffer.get()), size);

	ThrowIfFailed(device->CreatePixelShader(
		buffer.get(),
		static_cast<SIZE_T>(size),
		nullptr,
		&PS_
	));
}
