#pragma once

namespace K
{
	struct FBXVertex
	{
		Vector3 position;
		Vector2 uv;
		Vector3 normal;
		Vector3 binormal;
		Vector3 tangent;
		Vector4 joint_weights;
		Vector4 joint_indices;
	};

	struct FBXMaterial
	{
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
		Vector4 emissive;
		std::filesystem::path diffuse_texture;
		std::filesystem::path specular_texture;
		std::filesystem::path bump_texture;
	};

	struct FBXMeshContainer
	{
		std::vector<FBXVertex> vertices;
		std::vector<uint32_t> indices;
	};

	class ENGINE_DLL FBXLoader final : public Singleton<FBXLoader>
	{
		friend class Singleton<FBXLoader>;
	public:
		virtual void Initialize() override;

		void Begin(std::wstring const& _file_name, std::string const& _path_tag);
		void End();

		std::vector<std::unique_ptr<FBXMeshContainer, std::function<void(FBXMeshContainer*)>>> const& fbx_mesh_container_vector();
		std::vector<std::vector<std::unique_ptr<FBXMaterial, std::function<void(FBXMaterial*)>>>> const& fbx_material_2d_vector();

	private:
		FBXLoader() = default;
		FBXLoader(FBXLoader const&) = delete;
		FBXLoader(FBXLoader&&) noexcept = delete;
		FBXLoader& operator=(FBXLoader const&) = delete;
		FBXLoader& operator=(FBXLoader&&) noexcept = delete;

		virtual void _Finalize() override;

		void _LoadMesh(FbxNode* _fbx_node);
		void _LoadMesh(FbxMesh* _fbx_mesh);

		void _LoadPosition(FBXVertex& _vertex, FbxMesh* _fbx_mesh, uint32_t _control_point_idx);
		void _LoadUV(FBXVertex& _vertex, FbxMesh* _fbx_mesh, uint32_t _control_point_idx, uint32_t _polygon_vertex_idx);
		void _LoadNormal(FBXVertex& _vertex, FbxMesh* _fbx_mesh, uint32_t _control_point_idx, uint32_t _polygon_vertex_idx);
		void _LoadBinormal(FBXVertex& _vertex, FbxMesh* _fbx_mesh, uint32_t _control_point_idx, uint32_t _polygon_vertex_idx);
		void _LoadTangent(FBXVertex& _vertex, FbxMesh* _fbx_mesh, uint32_t _control_point_idx, uint32_t _polygon_vertex_idx);

		void _LoadMaterial(FbxNode* _fbx_node);
		void _LoadMaterial(FbxSurfaceMaterial* _fbx_surface_material);

		Vector4 _LoadColor(FbxSurfaceMaterial* _fbx_surface_material, std::string const& _color_property_name, std::string const& _color_factor_property_name);
		float _LoadFactor(FbxSurfaceMaterial* _fbx_surface_material, std::string const& _factor_property_name);
		std::filesystem::path _LoadTexture(FbxSurfaceMaterial* _fbx_surface_material, std::string const& _texture_property_name);

		std::unique_ptr<FbxManager, std::function<void(FbxManager*)>> fbx_manager_{};
		std::unique_ptr<FbxIOSettings, std::function<void(FbxIOSettings*)>> fbx_io_settings_{};
		std::unique_ptr<FbxImporter, std::function<void(FbxImporter*)>> fbx_importer_{};

		std::vector<std::unique_ptr<FBXMeshContainer, std::function<void(FBXMeshContainer*)>>> fbx_mesh_container_vector_{};
		std::vector<std::vector<std::unique_ptr<FBXMaterial, std::function<void(FBXMaterial*)>>>> fbx_material_2d_vector_{};
	};
}
