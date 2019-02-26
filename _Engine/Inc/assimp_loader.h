#pragma once

class Assimp::Importer;
struct aiScene;

namespace K
{
	using Importer = std::unique_ptr<Assimp::Importer, std::function<void(Assimp::Importer*)>>;

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
		std::vector<std::unique_ptr<AssimpVertex>> vertices;
		std::vector<uint32_t> indices;
	};

	class AssimpLoader
	{
	public:
		AssimpLoader(std::wstring const& _file_name, std::string const& _path_tag);
		~AssimpLoader();

		void ExportAsset(std::wstring const& _file_name, std::string const& _path_tag);

	private:
		void _ReadMesh(aiNode* _ai_node);
		void _ProcessMesh(aiMesh* _ai_mesh);
		void _WriteMesh(std::wstring const& _file_name, std::string const& _path_tag);

		void _ReadMaterial();
		void _WriteMaterial();

		void _ReadAnimation();
		void _WriteAnimation();

		Importer importer_{};
		aiScene const* scene_{};

		std::vector<std::unique_ptr<AssimpMesh>> meshes_{};
	};
}
