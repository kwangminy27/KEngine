#pragma once

class Assimp::Importer;
struct aiScene;

namespace K
{
	struct AssimpVertex
	{
		Vector3 position;
		Vector2 uv;
		Vector3 normal;
		Vector3 tangent;
		Vector3 bitangent;
		Vector4 bone_indices;
		Vector4 bone_weights;
	};

	struct AssimpMesh
	{
		std::string name;
		std::string material_name;
		std::vector<std::unique_ptr<AssimpVertex>> vertices;
		std::vector<uint32_t> indices;
	};

	struct AssimpMaterial
	{
		std::string name;
		std::vector<std::string> diffuse_maps;
		std::vector<std::string> specular_maps;
		std::vector<std::string> normal_maps;
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
		Vector4 emissive;
		float specular_exp;
	};

	class AssimpLoader
	{
	public:
		AssimpLoader(std::wstring const& _file_name, std::string const& _path_tag);
		~AssimpLoader();

		void ExportAsset(std::wstring const& _file_name);

	private:
		void _ReadMesh(aiNode* _ai_node);
		void _ProcessMesh(aiMesh* _ai_mesh);
		void _WriteMesh(std::wstring const& _file_name, std::string const& _path_tag);

		void _ReadMaterial();
		void _WriteMaterial(std::wstring const& _file_name, std::string const& _path_tag);

		void _ReadAnimation();
		void _WriteAnimation();

		std::unique_ptr<Assimp::Importer, std::function<void(Assimp::Importer*)>> ai_importer_{};
		aiScene const* ai_scene_{};

		std::vector<std::unique_ptr<AssimpMesh>> meshes_{};
		std::vector<std::unique_ptr<AssimpMaterial>> materials_{};
	};
}
