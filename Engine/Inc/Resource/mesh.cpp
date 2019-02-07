#include "Engine.h"
#include "mesh.h"

#include "device_manager.h"
#include "path_manager.h"
#include "resource_manager.h"
#include "fbx_loader.h"

void K::Mesh::Render()
{
	auto const& context = DeviceManager::singleton()->context();

	for (auto const& mesh_container : mesh_container_vector_)
	{
		context->IASetPrimitiveTopology(mesh_container->topology);

		std::vector<ID3D11Buffer*> buffer_vector{};
		std::vector<UINT> stride_vector{};
		std::vector<UINT> offset_vector{};

		for (auto const& VB : mesh_container->VB_vector)
		{
			buffer_vector.push_back(VB.buffer.Get());
			stride_vector.push_back(VB.stride);
			offset_vector.push_back(0);
		}

		context->IASetVertexBuffers(0, static_cast<UINT>(mesh_container->VB_vector.size()), buffer_vector.data(), stride_vector.data(), offset_vector.data());

		if (mesh_container->IB_vector.empty())
		{
			if (mesh_container->VB_vector.size() == 1)
				context->Draw(mesh_container->VB_vector.at(static_cast<int>(VERTEX_BUFFER_TYPE::VERTEX)).count, 0);
			else
				context->DrawInstanced(mesh_container->VB_vector.at(static_cast<int>(VERTEX_BUFFER_TYPE::VERTEX)).count, mesh_container->VB_vector.at(static_cast<int>(VERTEX_BUFFER_TYPE::INSTANCE)).count, 0, 0);
		}
		else
		{
			for (auto const& IB : mesh_container->IB_vector)
			{
				context->IASetIndexBuffer(IB.buffer.Get(), IB.format, 0);

				if (mesh_container->VB_vector.size() == 1)
					context->DrawIndexed(IB.count, 0, 0);
				else
					context->DrawIndexedInstanced(IB.count, mesh_container->VB_vector.at(static_cast<int>(VERTEX_BUFFER_TYPE::INSTANCE)).count, 0, 0, 0);
			}
		}
	}
}

void K::Mesh::Render(int _container_idx, int _subset_idx)
{
	auto const& context = DeviceManager::singleton()->context();
	auto const& mesh_container = mesh_container_vector_.at(_container_idx);

	context->IASetPrimitiveTopology(mesh_container->topology);

	std::vector<ID3D11Buffer*> buffer_vector{};
	std::vector<UINT> stride_vector{};
	std::vector<UINT> offset_vector{};

	for (auto const& VB : mesh_container->VB_vector)
	{
		buffer_vector.push_back(VB.buffer.Get());
		stride_vector.push_back(VB.stride);
		offset_vector.push_back(0);
	}

	context->IASetVertexBuffers(0, static_cast<UINT>(mesh_container->VB_vector.size()), buffer_vector.data(), stride_vector.data(), offset_vector.data());

	auto const& IB = mesh_container->IB_vector.at(_subset_idx);

	context->IASetIndexBuffer(IB.buffer.Get(), IB.format, 0);

	if (mesh_container->VB_vector.size() == 1)
		context->DrawIndexed(IB.count, 0, 0);
	else
		context->DrawIndexedInstanced(IB.count, mesh_container->VB_vector.at(static_cast<int>(VERTEX_BUFFER_TYPE::INSTANCE)).count, 0, 0, 0);
}

size_t K::Mesh::GetContainerSize() const
{
	return mesh_container_vector_.size();
}

size_t K::Mesh::GetSubsetSize(int _container_idx) const
{
	return mesh_container_vector_.at(_container_idx)->IB_vector.size();
}

Microsoft::WRL::ComPtr<ID3D11Buffer> const& K::Mesh::GetInstanceBuffer(int _container_idx) const
{
	return mesh_container_vector_.at(_container_idx)->VB_vector.at(static_cast<int>(VERTEX_BUFFER_TYPE::INSTANCE)).buffer;
}

void K::Mesh::SetInstanceCount(int _container_idx, int _count)
{
	mesh_container_vector_.at(_container_idx)->VB_vector.at(static_cast<int>(VERTEX_BUFFER_TYPE::INSTANCE)).count = _count;
}

K::Vector3 K::Mesh::min() const
{
	return min_;
}

K::Vector3 K::Mesh::max() const
{
	return max_;
}

K::Vector3 K::Mesh::center() const
{
	return center_;
}

K::Vector3 K::Mesh::extent() const
{
	return extent_;
}

std::vector<std::vector<std::unique_ptr<K::MaterialInfo, std::function<void(K::MaterialInfo*)>>>> const& K::Mesh::material_info_2d_vector() const
{
	return material_info_2d_vector_;
}

K::Mesh::Mesh(Mesh&& _other) noexcept
{
	mesh_container_vector_ = std::move(_other.mesh_container_vector_);
}

void K::Mesh::_LoadMesh(std::wstring const& _file_name, std::string const& _path_tag)
{
	std::filesystem::path path_buffer = _file_name;

	auto extension = path_buffer.extension().string();

	std::for_each(extension.begin(), extension.end(), [](char& c) {
		c = toupper(c);
	});

	if (".FBX" == extension)
	{
		auto const& fbx_loader = FBXLoader::singleton();

		fbx_loader->Begin(_file_name, _path_tag);

		_ConvertFromFBX();

		fbx_loader->End();

		path_buffer.replace_extension(".msh");

		_SaveMSH(path_buffer, MESH_PATH);
	}
	else if(".MSH" == extension)
		_LoadMSH(path_buffer, MESH_PATH);

	// 텍스처 생성
	auto const& resource_manager = ResourceManager::singleton();

	for (size_t i = 0; i < material_info_2d_vector_.size(); ++i)
	{
		for (size_t j = 0; j < material_info_2d_vector_.at(i).size(); ++j)
		{
			auto const& material_info = material_info_2d_vector_.at(i).at(j);

			path_buffer = material_info->diffuse_texture;
			resource_manager->_CreateTexture2D(path_buffer.string(), path_buffer.wstring(), TEXTURE_PATH);

			path_buffer = material_info->specular_texture;
			resource_manager->_CreateTexture2D(path_buffer.string(), path_buffer.wstring(), TEXTURE_PATH);

			path_buffer = material_info->bump_texture;
			resource_manager->_CreateTexture2D(path_buffer.string(), path_buffer.wstring(), TEXTURE_PATH);
		}
	}
}

void K::Mesh::_CreateMesh(
	D3D11_PRIMITIVE_TOPOLOGY _topology,
	void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage)
{
	auto mesh_container_buffer = std::unique_ptr<MeshContainer, std::function<void(MeshContainer*)>>{ new MeshContainer, [](MeshContainer* _p) {
		for (auto& VB : _p->VB_vector)
			delete[] VB.data;
		for (auto& IB : _p->IB_vector)
			delete[] IB.data;
		delete _p;
	} };

	mesh_container_buffer->topology = _topology;

	mesh_container_vector_.push_back(std::move(mesh_container_buffer));

	_CreateVertexBuffer(_vtx_data, _vtx_stride, _vtx_count, _vtx_usage, VERTEX_BUFFER_TYPE::VERTEX);
}

void K::Mesh::_CreateMesh(
	D3D11_PRIMITIVE_TOPOLOGY _topology,
	void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage,
	void* _idx_data, int _idx_stride, int _idx_count, D3D11_USAGE _idx_usage, DXGI_FORMAT _idx_format)
{
	auto mesh_container_buffer = std::unique_ptr<MeshContainer, std::function<void(MeshContainer*)>>{ new MeshContainer, [](MeshContainer* _p) {
		for (auto& VB : _p->VB_vector)
			delete[] VB.data;
		for (auto& IB : _p->IB_vector)
			delete[] IB.data;
		delete _p;
	} };

	mesh_container_buffer->topology = _topology;

	mesh_container_vector_.push_back(std::move(mesh_container_buffer));

	_CreateVertexBuffer(_vtx_data, _vtx_stride, _vtx_count, _vtx_usage, VERTEX_BUFFER_TYPE::VERTEX);
	_CreateIndexBuffer(_idx_data, _idx_stride, _idx_count, _idx_usage, _idx_format);
}

void K::Mesh::_CreateMesh(
	D3D11_PRIMITIVE_TOPOLOGY _topology,
	void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage,
	void* _idx_data, int _idx_stride, int _idx_count, D3D11_USAGE _idx_usage, DXGI_FORMAT _idx_format,
	void* _inst_data, int _inst_stride, int _inst_count, D3D11_USAGE _inst_usage)
{
	auto mesh_container_buffer = std::unique_ptr<MeshContainer, std::function<void(MeshContainer*)>>{ new MeshContainer, [](MeshContainer* _p) {
		for (auto& VB : _p->VB_vector)
			delete[] VB.data;
		for (auto& IB : _p->IB_vector)
			delete[] IB.data;
		delete _p;
	} };

	mesh_container_buffer->topology = _topology;

	mesh_container_vector_.push_back(std::move(mesh_container_buffer));

	_CreateVertexBuffer(_vtx_data, _vtx_stride, _vtx_count, _vtx_usage, VERTEX_BUFFER_TYPE::VERTEX);
	_CreateVertexBuffer(_inst_data, _inst_stride, _inst_count, _inst_usage, VERTEX_BUFFER_TYPE::INSTANCE);
	_CreateIndexBuffer(_idx_data, _idx_stride, _idx_count, _idx_usage, _idx_format);
}

void K::Mesh::_CreateVertexBuffer(void* _data, int _stride, int _count, D3D11_USAGE _usage, VERTEX_BUFFER_TYPE _type)
{
	auto const& device = DeviceManager::singleton()->device();

	auto& mesh_container = mesh_container_vector_.at(mesh_container_vector_.size() - 1);

	VertexBuffer VB{};
	VB.stride = _stride;
	VB.count = _count;
	VB.usage = _usage;

	int byte_width = _stride * _count;
	VB.data = new char[byte_width];
	memcpy_s(VB.data, byte_width, _data, byte_width);

	D3D11_BUFFER_DESC dbd{};
	dbd.ByteWidth = byte_width;
	dbd.Usage = _usage;
	dbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	if (dbd.Usage == D3D11_USAGE::D3D11_USAGE_DYNAMIC)
		dbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA dsd{};
	dsd.pSysMem = VB.data;

	ThrowIfFailed(device->CreateBuffer(&dbd, &dsd, &VB.buffer));

	mesh_container->VB_vector.push_back(std::move(VB));

	if (VERTEX_BUFFER_TYPE::VERTEX == _type)
	{
		char* vertices = static_cast<char*>(_data);

		Vector3 position{};
		for (int i = 0; i < _count; ++i)
		{
			memcpy_s(&position, sizeof(Vector3), vertices + _stride * i, sizeof(Vector3));

			min_ = Vector3::Min(min_, position);
			max_ = Vector3::Max(max_, position);

			center_ = (max_ + min_) / 2.f;
			extent_ = (max_ - min_) / 2.f;
		}
	}
}

void K::Mesh::_CreateIndexBuffer(void* _data, int _stride, int _count, D3D11_USAGE _usage, DXGI_FORMAT _format)
{
	auto const& device = DeviceManager::singleton()->device();

	auto& mesh_container = mesh_container_vector_.at(mesh_container_vector_.size() - 1);

	IndexBuffer IB{};
	IB.stride = _stride;
	IB.count = _count;
	IB.usage = _usage;
	IB.format = _format;

	int byte_width = _stride * _count;
	IB.data = new char[byte_width];
	memcpy_s(IB.data, byte_width, _data, byte_width);

	D3D11_BUFFER_DESC dbd{};
	dbd.ByteWidth = byte_width;
	dbd.Usage = _usage;
	dbd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	if (_usage == D3D11_USAGE_DYNAMIC)
		dbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA dsd{};
	dsd.pSysMem = IB.data;

	ThrowIfFailed(device->CreateBuffer(&dbd, &dsd, &IB.buffer));

	mesh_container->IB_vector.push_back(std::move(IB));
}

void K::Mesh::_ConvertFromFBX()
{
	auto const& fbx_loader = FBXLoader::singleton();

	auto const& fbx_mesh_container_vector = fbx_loader->fbx_mesh_container_vector();
	auto const& fbx_material_2d_vector = fbx_loader->fbx_material_2d_vector();

	// Process Mesh
	for (auto iter = fbx_mesh_container_vector.begin(); iter != fbx_mesh_container_vector.end(); ++iter)
	{
		std::vector<Vertex3D> vertices{};
		for (auto i = 0; i < (*iter)->vertices.size(); ++i)
		{
			auto const& fbx_vertex = (*iter)->vertices.at(i);
			
			Vertex3D vertex{};

			vertex.position = fbx_vertex.position;
			vertex.uv = fbx_vertex.uv;
			vertex.normal = fbx_vertex.normal;
			vertex.binormal = fbx_vertex.binormal;
			vertex.tangent = fbx_vertex.tangent;
			vertex.joint_weights = fbx_vertex.joint_weights;
			vertex.joint_indices = fbx_vertex.joint_indices;

			vertices.push_back(std::move(vertex));
		}

		std::vector<uint32_t> indices = std::move((*iter)->indices);

		_CreateMesh(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			vertices.data(), sizeof(Vertex3D), static_cast<int>(vertices.size()), D3D11_USAGE_DEFAULT,
			indices.data(), sizeof(uint32_t), static_cast<int>(indices.size()), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT
		);
	}

	// Process Material
	for (auto i = 0; i < fbx_material_2d_vector.size(); ++i)
	{
		std::vector<std::unique_ptr<MaterialInfo, std::function<void(MaterialInfo*)>>> material_info_vector{};
		for (auto j = 0; j < fbx_material_2d_vector.at(i).size(); ++j)
		{
			auto const& fbx_material = fbx_material_2d_vector.at(i).at(j);

			auto material_info = std::unique_ptr<MaterialInfo, std::function<void(MaterialInfo*)>>{ new MaterialInfo, [](MaterialInfo* _p) {
				delete _p;
			} };

			material_info->ambient = fbx_material->ambient;
			material_info->diffuse = fbx_material->diffuse;
			material_info->specular = fbx_material->specular;
			material_info->emissive = fbx_material->emissive;
			material_info->diffuse_texture = fbx_material->diffuse_texture.string();
			material_info->specular_texture = fbx_material->specular_texture.string();
			material_info->bump_texture = fbx_material->bump_texture.string();

			material_info_vector.push_back(std::move(material_info));
		}
		material_info_2d_vector_.push_back(std::move(material_info_vector));
	}
}

void K::Mesh::_SaveMSH(std::wstring const& _file_name, std::string const& _path_tag)
{
	auto path_buffer = PathManager::singleton()->FindPath(_path_tag);

	path_buffer /= _file_name;

	std::fstream file{ path_buffer, std::ios::out | std::ios::binary };

	if (file.fail())
		std::exception{ "Mesh::_SaveMSH" };

	OutputMemoryStream omstream{};

	auto mesh_container_size = mesh_container_vector_.size();
	omstream.Serialize(mesh_container_size);

	for (auto i = 0; i < mesh_container_size; ++i)
	{
		auto& mesh_container = mesh_container_vector_.at(i);

		omstream.Serialize(mesh_container->topology);
		
		auto VB_size = mesh_container->VB_vector.size();
		omstream.Serialize(VB_size);

		for (auto j = 0; j < VB_size; ++j)
		{
			auto& VB = mesh_container->VB_vector.at(j);

			omstream.Serialize(VB.stride);
			omstream.Serialize(VB.count);
			omstream.Serialize(VB.usage);
			omstream.Serialize(VB.data, VB.stride * VB.count);
		}

		auto IB_size = mesh_container->IB_vector.size();
		omstream.Serialize(IB_size);

		for (auto j = 0; j < IB_size; ++j)
		{
			auto& IB = mesh_container->IB_vector.at(j);

			omstream.Serialize(IB.stride);
			omstream.Serialize(IB.count);
			omstream.Serialize(IB.usage);
			omstream.Serialize(IB.data, IB.stride * IB.count);
			omstream.Serialize(IB.format);
		}
	}

	auto material_info_2d_vector_size = material_info_2d_vector_.size();
	omstream.Serialize(material_info_2d_vector_size);

	for (auto i = 0; i < material_info_2d_vector_size; ++i)
	{
		auto material_info_vector_size = material_info_2d_vector_.at(i).size();
		omstream.Serialize(material_info_vector_size);

		for (auto j = 0; j < material_info_vector_size; ++j)
		{
			auto& material_info = material_info_2d_vector_.at(i).at(j);

			omstream.Serialize(material_info->ambient.x);
			omstream.Serialize(material_info->ambient.y);
			omstream.Serialize(material_info->ambient.z);
			omstream.Serialize(material_info->ambient.w);

			omstream.Serialize(material_info->diffuse.x);
			omstream.Serialize(material_info->diffuse.y);
			omstream.Serialize(material_info->diffuse.z);
			omstream.Serialize(material_info->diffuse.w);

			omstream.Serialize(material_info->specular.x);
			omstream.Serialize(material_info->specular.y);
			omstream.Serialize(material_info->specular.z);
			omstream.Serialize(material_info->specular.w);

			omstream.Serialize(material_info->emissive.x);
			omstream.Serialize(material_info->emissive.y);
			omstream.Serialize(material_info->emissive.z);
			omstream.Serialize(material_info->emissive.w);

			omstream.Serialize(material_info->diffuse_texture);
			omstream.Serialize(material_info->specular_texture);
			omstream.Serialize(material_info->bump_texture);
		}
	}

	file.write(reinterpret_cast<char const*>(omstream.buffer()->data()), omstream.head());
}

void K::Mesh::_LoadMSH(std::wstring const& _file_name, std::string const& _path_tag)
{
	auto path_buffer = PathManager::singleton()->FindPath(_path_tag);

	path_buffer /= _file_name;

	std::fstream file{ path_buffer, std::ios::in | std::ios::binary | std::ios::ate };

	if (file.fail())
		std::exception{ "Mesh::_LoadMSH" };

	auto size = file.tellg();
	file.seekg(std::ios::beg);

	InputMemoryStream imstream{};
	imstream.Resize(size);

	file.read(reinterpret_cast<char*>(imstream.buffer()->data()), size);

	size_t mesh_container_size{};
	imstream.Serialize(mesh_container_size);
	
	for (size_t i = 0; i < mesh_container_size; ++i)
	{
		auto mesh_container = std::unique_ptr<MeshContainer, std::function<void(MeshContainer*)>>{ new MeshContainer, [](MeshContainer* _p) {
			for (auto& VB : _p->VB_vector)
				delete[] VB.data;
			for (auto& IB : _p->IB_vector)
				delete[] IB.data;
			delete _p;
		} };

		imstream.Serialize(mesh_container->topology);

		mesh_container_vector_.push_back(std::move(mesh_container));

		size_t VB_size{};
		imstream.Serialize(VB_size);

		for (size_t j = 0; j < VB_size; ++j)
		{
			int stride{};
			imstream.Serialize(stride);

			int count{};
			imstream.Serialize(count);

			D3D11_USAGE usage{};
			imstream.Serialize(usage);

			auto byte_width = stride * count;
			auto data = std::unique_ptr<char, std::function<void(char*)>>{ new char[byte_width], [](char* _p) {
				delete[] _p;
			} };
			imstream.Serialize(data.get(), byte_width);

			switch (j)
			{
			case 0:
				_CreateVertexBuffer(data.get(), stride, count, usage, VERTEX_BUFFER_TYPE::VERTEX);
				break;

			case 1:
				_CreateVertexBuffer(data.get(), stride, count, usage, VERTEX_BUFFER_TYPE::INSTANCE);
				break;
			}
		}

		size_t IB_size{};
		imstream.Serialize(IB_size);

		for (size_t j = 0; j < IB_size; ++j)
		{
			int stride{};
			imstream.Serialize(stride);

			int count{};
			imstream.Serialize(count);

			D3D11_USAGE usage{};
			imstream.Serialize(usage);

			auto byte_width = stride * count;
			auto data = std::unique_ptr<char, std::function<void(char*)>>{ new char[byte_width], [](char* _p) {
				delete[] _p;
			} };
			imstream.Serialize(data.get(), byte_width);

			DXGI_FORMAT format{};
			imstream.Serialize(format);

			_CreateIndexBuffer(data.get(), stride, count, usage, format);
		}
	}

	size_t material_info_vector_size{};
	imstream.Serialize(material_info_vector_size);

	for (size_t i = 0; i < material_info_vector_size; ++i)
	{
		material_info_2d_vector_.push_back(std::vector<std::unique_ptr<MaterialInfo, std::function<void(MaterialInfo*)>>>{});

		size_t material_info_size{};
		imstream.Serialize(material_info_size);

		for (size_t j = 0; j < material_info_size; ++j)
		{
			material_info_2d_vector_.at(i).push_back(std::unique_ptr<MaterialInfo, std::function<void(MaterialInfo*)>>{ new MaterialInfo, [](MaterialInfo* _p) {
				delete _p;
			} });

			auto& material_info = material_info_2d_vector_.at(i).at(j);

			imstream.Serialize(material_info->ambient.x);
			imstream.Serialize(material_info->ambient.y);
			imstream.Serialize(material_info->ambient.z);
			imstream.Serialize(material_info->ambient.w);

			imstream.Serialize(material_info->diffuse.x);
			imstream.Serialize(material_info->diffuse.y);
			imstream.Serialize(material_info->diffuse.z);
			imstream.Serialize(material_info->diffuse.w);

			imstream.Serialize(material_info->specular.x);
			imstream.Serialize(material_info->specular.y);
			imstream.Serialize(material_info->specular.z);
			imstream.Serialize(material_info->specular.w);

			imstream.Serialize(material_info->emissive.x);
			imstream.Serialize(material_info->emissive.y);
			imstream.Serialize(material_info->emissive.z);
			imstream.Serialize(material_info->emissive.w);

			imstream.Serialize(material_info->diffuse_texture);
			imstream.Serialize(material_info->specular_texture);
			imstream.Serialize(material_info->bump_texture);
		}
	}
}
