#include "Engine.h"
#include "assimp_loader.h"

#include "path_manager.h"

K::AssimpLoader::AssimpLoader(std::wstring const& _file_name, std::string const& _path_tag)
{
	auto path_buffer = PathManager::singleton()->FindPath(_path_tag);

	path_buffer /= _file_name;

	importer_ = MAKE_UNIQUE<Assimp::Importer>();

	scene_ = importer_->ReadFile(
		path_buffer.string().c_str(),
		aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	if (false == scene_)
		throw std::exception{ "AssimpLoader::AssimpLoader" };
}

K::AssimpLoader::~AssimpLoader()
{
}

void K::AssimpLoader::ExportAsset(std::wstring const& _file_name, std::string const& _path_tag)
{
	_ReadMesh(scene_->mRootNode);
	_WriteMesh(_file_name + L".mesh", _path_tag);

	_ReadMaterial();
	_WriteMaterial();

	_ReadAnimation();
	_WriteAnimation();
}

void K::AssimpLoader::_ReadMesh(aiNode* _ai_node)
{
	for (uint32_t i = 0; i < _ai_node->mNumMeshes; ++i)
		_ProcessMesh(scene_->mMeshes[_ai_node->mMeshes[i]]);

	for (uint32_t i = 0; i < _ai_node->mNumChildren; ++i)
		_ReadMesh(_ai_node->mChildren[i]);
}

void K::AssimpLoader::_ProcessMesh(aiMesh* _ai_mesh)
{
	auto mesh = std::make_unique<AssimpMesh>();

	for (uint32_t i = 0; i < _ai_mesh->mNumVertices; ++i)
	{
		auto vertex = std::make_unique<AssimpVertex>();

		vertex->position.x = _ai_mesh->mVertices[i].x;
		vertex->position.y = _ai_mesh->mVertices[i].y;
		vertex->position.z = _ai_mesh->mVertices[i].z;

		vertex->uv.x = _ai_mesh->mTextureCoords[0][i].x;
		vertex->uv.y = _ai_mesh->mTextureCoords[0][i].y;

		vertex->normal.x = _ai_mesh->mNormals[i].x;
		vertex->normal.y = _ai_mesh->mNormals[i].y;
		vertex->normal.z = _ai_mesh->mNormals[i].z;

		vertex->tangent.x = _ai_mesh->mTangents[i].x;
		vertex->tangent.y = _ai_mesh->mTangents[i].y;
		vertex->tangent.z = _ai_mesh->mTangents[i].z;

		vertex->bitangent.x = _ai_mesh->mBitangents[i].x;
		vertex->bitangent.y = _ai_mesh->mBitangents[i].y;
		vertex->bitangent.z = _ai_mesh->mBitangents[i].z;

		mesh->vertices.push_back(std::move(vertex));
	}

	for (uint32_t i = 0; i < _ai_mesh->mNumFaces; ++i)
	{
		auto ai_face = _ai_mesh->mFaces[i];

		for (uint32_t j = 0; j < ai_face.mNumIndices; ++j)
			mesh->indices.push_back(ai_face.mIndices[j]);
	}

	meshes_.push_back(std::move(mesh));
}

void K::AssimpLoader::_WriteMesh(std::wstring const& _file_name, std::string const& _path_tag)
{
	auto path_buffer = PathManager::singleton()->FindPath(_path_tag);

	path_buffer /= _file_name;

	std::fstream file{ path_buffer, std::ios::out | std::ios::binary };

	if (file.fail() == true)
		throw std::exception{ "AssimpLoader::_WriteMesh" };

	OutputMemoryStream omstream{};

	auto mesh_count = meshes_.size();
	omstream.Serialize(mesh_count);

	for (size_t i = 0; i < mesh_count; ++i)
	{
		auto const& mesh = meshes_.at(i);

		auto vertex_count = mesh->vertices.size();
		omstream.Serialize(vertex_count);

		for (size_t j = 0; j < vertex_count; ++j)
		{
			auto const& vertex = mesh->vertices.at(j);

			omstream.Serialize(&vertex->position, sizeof(Vector3));
			omstream.Serialize(&vertex->uv, sizeof(Vector2));
			omstream.Serialize(&vertex->normal, sizeof(Vector3));
			omstream.Serialize(&vertex->tangent, sizeof(Vector3));
			omstream.Serialize(&vertex->bitangent, sizeof(Vector3));
			omstream.Serialize(&vertex->bone_indices, sizeof(Vector4));
			omstream.Serialize(&vertex->bone_weights, sizeof(Vector4));
		}

		auto index_count = mesh->indices.size();
		omstream.Serialize(index_count);

		for (size_t j = 0; j < index_count; ++j)
			omstream.Serialize(mesh->indices.at(j));
	}

	file.write(reinterpret_cast<char const*>(omstream.buffer()->data()), omstream.head());
}

void K::AssimpLoader::_ReadMaterial()
{
}

void K::AssimpLoader::_WriteMaterial()
{
}

void K::AssimpLoader::_ReadAnimation()
{
}

void K::AssimpLoader::_WriteAnimation()
{
}
