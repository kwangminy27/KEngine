#pragma once

namespace K
{
	enum class VERTEX_BUFFER_TYPE
	{
		VERTEX,
		INSTANCE
	};

	struct VertexBuffer
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		void* data;
		int stride;
		int count;
		D3D11_USAGE usage;
	};

	struct IndexBuffer
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		void* data;
		int stride;
		int count;
		D3D11_USAGE usage;
		DXGI_FORMAT format;
	};

	struct MeshContainer
	{
		std::string name;
		std::string material_name;
		D3D11_PRIMITIVE_TOPOLOGY topology;
		std::vector<VertexBuffer> VB_vector;
		std::vector<IndexBuffer> IB_vector;
	};

	class ENGINE_DLL Mesh
	{
		friend class ResourceManager;
	public:
		void Render();
		void Render(int _container_idx, int _subset_idx);

		size_t GetContainerSize() const;
		size_t GetSubsetSize(int _container_idx) const;
		Microsoft::WRL::ComPtr<ID3D11Buffer> const& GetInstanceBuffer(int _container_idx) const;

		void SetInstanceCount(int _container_idx, int _count);

		std::string name() const;
		Vector3 min() const;
		Vector3 max() const;
		Vector3 center() const;
		Vector3 extent() const;
		std::vector<std::unique_ptr<MeshContainer, std::function<void(MeshContainer*)>>> const& mesh_container_vector() const;

	private:
		Mesh() = default;
		Mesh(Mesh const&) = delete;
		Mesh(Mesh&& _other) noexcept;
		Mesh& operator=(Mesh const&) = delete;
		Mesh& operator=(Mesh&&) noexcept = default;

		void _LoadMesh(std::string const& _name, std::wstring const& _file_name, std::string const& _path_tag);
		void _CreateMesh(
			D3D11_PRIMITIVE_TOPOLOGY _topology,
			void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage);
		void _CreateMesh(
			D3D11_PRIMITIVE_TOPOLOGY _topology,
			void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage,
			void* _idx_data, int _idx_stride, int _idx_count, D3D11_USAGE _idx_usage, DXGI_FORMAT _idx_format);
		void _CreateMesh(
			D3D11_PRIMITIVE_TOPOLOGY _topology,
			void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage,
			void* _idx_data, int _idx_stride, int _idx_count, D3D11_USAGE _idx_usage, DXGI_FORMAT _idx_format,
			void* _inst_data, int _inst_stride, int _inst_count, D3D11_USAGE _inst_usage);
		void _CreateVertexBuffer(void* _data, int _stride, int _count, D3D11_USAGE _usage, VERTEX_BUFFER_TYPE _type);
		void _CreateIndexBuffer(void* _data, int _stride, int _count, D3D11_USAGE _usage, DXGI_FORMAT _format);

		std::string name_{};
		Vector3 min_{};
		Vector3 max_{};
		Vector3 center_{};
		Vector3 extent_{};
		std::vector<std::unique_ptr<MeshContainer, std::function<void(MeshContainer*)>>> mesh_container_vector_{};
	};
}
