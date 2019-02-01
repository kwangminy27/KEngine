#include "Engine.h"
#include "mesh.h"

#include "device_manager.h"
#include "path_manager.h"
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

K::Mesh::Mesh(Mesh&& _other) noexcept
{
	mesh_container_vector_ = std::move(_other.mesh_container_vector_);
}

void K::Mesh::_LoadMesh(std::wstring const& _file_name, std::string const& _path_tag)
{
	auto const& fbx_loader = FBXLoader::singleton();

	fbx_loader->Begin(_file_name, _path_tag);

	// 메쉬 생성

	fbx_loader->End();
}

void K::Mesh::_CreateMesh(
	D3D11_PRIMITIVE_TOPOLOGY _topology,
	void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage,
	Vector3 _scale, Quaternion _rotation)
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

	_CreateVertexBuffer(_vtx_data, _vtx_stride, _vtx_count, _vtx_usage, VERTEX_BUFFER_TYPE::VERTEX, _scale, _rotation);
}

void K::Mesh::_CreateMesh(
	D3D11_PRIMITIVE_TOPOLOGY _topology,
	void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage,
	void* _idx_data, int _idx_stride, int _idx_count, D3D11_USAGE _idx_usage, DXGI_FORMAT _idx_format,
	Vector3 _scale, Quaternion _rotation)
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

	_CreateVertexBuffer(_vtx_data, _vtx_stride, _vtx_count, _vtx_usage, VERTEX_BUFFER_TYPE::VERTEX, _scale, _rotation);
	_CreateIndexBuffer(_idx_data, _idx_stride, _idx_count, _idx_usage, _idx_format);
}

void K::Mesh::_CreateMesh(
	D3D11_PRIMITIVE_TOPOLOGY _topology,
	void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage,
	void* _idx_data, int _idx_stride, int _idx_count, D3D11_USAGE _idx_usage, DXGI_FORMAT _idx_format,
	void* _inst_data, int _inst_stride, int _inst_count, D3D11_USAGE _inst_usage,
	Vector3 _scale, Quaternion _rotation)
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

	_CreateVertexBuffer(_vtx_data, _vtx_stride, _vtx_count, _vtx_usage, VERTEX_BUFFER_TYPE::VERTEX, _scale, _rotation);
	_CreateVertexBuffer(_inst_data, _inst_stride, _inst_count, _inst_usage, VERTEX_BUFFER_TYPE::INSTANCE, _scale, _rotation);
	_CreateIndexBuffer(_idx_data, _idx_stride, _idx_count, _idx_usage, _idx_format);
}

void K::Mesh::_CreateVertexBuffer(void* _data, int _stride, int _count, D3D11_USAGE _usage, VERTEX_BUFFER_TYPE _type, Vector3 _scale, Quaternion _rotation)
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

			position = Vector3::Transform(position, Matrix::CreateScaling(_scale) * Matrix::CreateFromQuaternion(_rotation));

			min_ = Vector3::Min(min_, position);
			max_ = Vector3::Max(max_, position);
		}

		center_ = (max_ + min_) / 2.f;
		extent_ = (max_ - min_) / 2.f;
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
