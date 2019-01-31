#pragma once

namespace K
{
	struct FBXVertex
	{
		Vector3 position;
		Vector3 normal;
		Vector3 binormal;
		Vector3 tangent;
		Vector2 uv;
	};

	struct FBXIndex
	{
		uint32_t idx_for_vtx;
		uint32_t idx_for_idx;
	};

	struct FBXMeshContainer
	{
		bool bump_flag;
		bool animation_flag;
		std::vector<FBXVertex> vertices;
		std::vector<std::vector<uint32_t>> material_indices;
	};

	class ENGINE_DLL FBXLoader final : public Singleton<FBXLoader>
	{
		friend class Singleton<FBXLoader>;
	public:
		virtual void Initialize() override;

		void Begin(std::wstring const& _file_name, std::string const& _path_tag);
		void End();

		std::unique_ptr<FbxManager, std::function<void(FbxManager*)>> const& fbx_manager() const;
		std::unique_ptr<FbxIOSettings, std::function<void(FbxIOSettings*)>> const& fbx_io_settings() const;
		std::unique_ptr<FbxImporter, std::function<void(FbxImporter*)>> const& fbx_importer() const;

	private:
		FBXLoader() = default;
		FBXLoader(FBXLoader const&) = delete;
		FBXLoader(FBXLoader&&) noexcept = delete;
		FBXLoader& operator=(FBXLoader const&) = delete;
		FBXLoader& operator=(FBXLoader&&) noexcept = delete;

		virtual void _Finalize() override;

		void _LoadMesh(FbxNode* _node);
		void _LoadMesh(FbxMesh* _mesh);

		void _LoadPosition(FBXVertex& _vertex, FbxMesh* _mesh, uint32_t _index);

		void _LoadNormal(FBXVertex& _vtx, FbxMesh* _mesh, FBXIndex const& _idx);
		void _LoadBinormal(FBXVertex& _vtx, FbxMesh* _mesh, FBXIndex const& _idx);
		void _LoadTangent(FBXVertex& _vtx, FbxMesh* _mesh, FBXIndex const& _idx);
		
		void _LoadUV(FBXVertex& _vtx, FbxMesh* _mesh, FBXIndex const& _idx);

		std::unique_ptr<FbxManager, std::function<void(FbxManager*)>> fbx_manager_{};
		std::unique_ptr<FbxIOSettings, std::function<void(FbxIOSettings*)>> fbx_io_settings_{};
		std::unique_ptr<FbxImporter, std::function<void(FbxImporter*)>> fbx_importer_{};

		std::vector<std::unique_ptr<FBXMeshContainer, std::function<void(FBXMeshContainer*)>>> fbx_mesh_container_vector_{};
	};
}
