#include "Engine.h"
#include "mesh.h"

#include "device_manager.h"
#include "path_manager.h"
#include "resource_manager.h"
#include "fbx_loader.h"
#include "assimp_loader.h"

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

std::string K::Mesh::name() const
{
	return name_;
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

std::vector<std::unique_ptr<K::MeshContainer, std::function<void(K::MeshContainer*)>>> const & K::Mesh::mesh_container_vector() const
{
	return mesh_container_vector_;
}

K::Mesh::Mesh(Mesh&& _other) noexcept
{
	mesh_container_vector_ = std::move(_other.mesh_container_vector_);
}

void K::Mesh::_LoadMesh(std::string const& _name, std::wstring const& _file_name, std::string const& _path_tag)
{
	name_ = _name;

	auto path_buffer = PathManager::singleton()->FindPath(_path_tag);

	path_buffer /= _file_name;

	std::fstream file{ path_buffer, std::ios::in | std::ios::binary | std::ios::ate };

	if (file.fail())
		throw std::exception{ "Mesh::_LoadMesh" };

	auto file_size = file.tellg();
	file.seekg(std::ios::beg);

	InputMemoryStream imstream{};
	imstream.Resize(file_size);

	file.read(reinterpret_cast<char*>(imstream.buffer()->data()), file_size);

	std::vector<std::unique_ptr<AssimpMesh>> meshes{};

	size_t mesh_count{};
	imstream.Serialize(mesh_count);

	for (size_t i = 0; i < mesh_count; ++i)
	{
		auto mesh = std::make_unique<AssimpMesh>();

		imstream.Serialize(mesh->name);
		imstream.Serialize(mesh->material_name);

		size_t vertex_count{};
		imstream.Serialize(vertex_count);

		for (size_t j = 0; j < vertex_count; ++j)
		{
			auto vertex = std::make_unique<AssimpVertex>();

			imstream.Serialize(&vertex->position, sizeof(Vector3));
			imstream.Serialize(&vertex->uv, sizeof(Vector2));
			imstream.Serialize(&vertex->normal, sizeof(Vector3));
			imstream.Serialize(&vertex->tangent, sizeof(Vector3));
			imstream.Serialize(&vertex->bitangent, sizeof(Vector3));
			imstream.Serialize(&vertex->bone_indices, sizeof(Vector4));
			imstream.Serialize(&vertex->bone_weights, sizeof(Vector4));

			mesh->vertices.push_back(std::move(vertex));
		}

		size_t index_count{};
		imstream.Serialize(index_count);

		for (size_t j = 0; j < index_count; ++j)
		{
			uint32_t index{};
			imstream.Serialize(index);
			
			mesh->indices.push_back(index);
		}

		meshes.push_back(std::move(mesh));
	}

	for (auto const& mesh : meshes)
	{
		std::vector<Vertex3D> vertices{};
		for (size_t i = 0; i < mesh->vertices.size(); ++i)
		{
			auto const& temp_vertex = mesh->vertices.at(i);

			Vertex3D vertex{};
			vertex.position = temp_vertex->position;
			vertex.uv = temp_vertex->uv;
			vertex.normal = temp_vertex->normal;
			vertex.tangent = temp_vertex->tangent;
			vertex.binormal = temp_vertex->bitangent;
			vertex.joint_indices = temp_vertex->bone_indices;
			vertex.joint_weights = temp_vertex->bone_weights;

			vertices.push_back(std::move(vertex));
		}

		std::vector<uint32_t> indices = std::move(mesh->indices);

		_CreateMesh(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			vertices.data(), sizeof(Vertex3D), static_cast<int>(vertices.size()), D3D11_USAGE_DEFAULT,
			indices.data(), sizeof(uint32_t), static_cast<int>(indices.size()), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT
		);

		auto& mesh_container = mesh_container_vector_.back();
		mesh_container->name = mesh->name;
		mesh_container->material_name = mesh->material_name;
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
