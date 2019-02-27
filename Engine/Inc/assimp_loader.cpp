#include "Engine.h"
#include "assimp_loader.h"

#include "path_manager.h"

K::AssimpLoader::AssimpLoader(std::wstring const& _file_name, std::string const& _path_tag)
{
	auto path_buffer = PathManager::singleton()->FindPath(_path_tag);

	path_buffer /= _file_name;

	ai_importer_ = MAKE_UNIQUE<Assimp::Importer>();

	ai_scene_ = ai_importer_->ReadFile(
		path_buffer.string(),
		aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_GenUVCoords
	);

	if (false == ai_scene_)
		throw std::exception{ ai_importer_->GetErrorString() };
}

K::AssimpLoader::~AssimpLoader()
{
}

void K::AssimpLoader::ExportAsset(std::wstring const& _file_name)
{
	_ReadMesh(ai_scene_->mRootNode);
	_WriteMesh(_file_name + L".mesh", MESH_PATH);

	_ReadMaterial();
	_WriteMaterial(_file_name + L".material", MATERIAL_PATH);

	_ReadAnimation();
	_WriteAnimation();
}

void K::AssimpLoader::_ReadMesh(aiNode* _ai_node)
{
	for (uint32_t i = 0; i < _ai_node->mNumMeshes; ++i)
		_ProcessMesh(ai_scene_->mMeshes[_ai_node->mMeshes[i]]);

	for (uint32_t i = 0; i < _ai_node->mNumChildren; ++i)
		_ReadMesh(_ai_node->mChildren[i]);
}

void K::AssimpLoader::_ProcessMesh(aiMesh* _ai_mesh)
{
	auto mesh = std::make_unique<AssimpMesh>();

	mesh->name = _ai_mesh->mName.C_Str();
	mesh->material_name = ai_scene_->mMaterials[_ai_mesh->mMaterialIndex]->GetName().C_Str();

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

		omstream.Serialize(mesh->name);
		omstream.Serialize(mesh->material_name);

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
	static auto GetColor = [](aiMaterial* _ai_material, char const* _key, uint32_t _type, uint32_t _idx) -> Vector4 {
		aiColor3D color{};
		_ai_material->Get(_key, _type, _idx, color);
		return Vector4{ color.r, color.g, color.b, 1.f };
	};
	static auto GetMaps = [](aiMaterial* _ai_material, aiTextureType _ai_texture_type) -> std::vector<std::string> {
		std::vector<std::string> maps{};
		for (uint32_t i = 0; i < _ai_material->GetTextureCount(_ai_texture_type); ++i)
		{
			aiString path{};
			_ai_material->GetTexture(_ai_texture_type, i, &path);
			std::filesystem::path map = std::wstring{ CA2W(path.C_Str()) };
			map = map.filename();
			map.replace_extension(".dds");
			maps.push_back(map.string());
		}
		return maps;
	};

	for (uint32_t i = 0; i < ai_scene_->mNumMaterials; ++i)
	{
		auto material = std::make_unique<AssimpMaterial>();
		auto ai_material = ai_scene_->mMaterials[i];

		material->name = ai_material->GetName().C_Str();

		material->diffuse_maps = GetMaps(ai_material, aiTextureType_DIFFUSE);
		material->specular_maps = GetMaps(ai_material, aiTextureType_SPECULAR);
		material->normal_maps = GetMaps(ai_material, aiTextureType_NORMALS);

		material->ambient = GetColor(ai_material, AI_MATKEY_COLOR_AMBIENT);
		material->diffuse = GetColor(ai_material, AI_MATKEY_COLOR_DIFFUSE);
		material->specular = GetColor(ai_material, AI_MATKEY_COLOR_SPECULAR);
		material->emissive = GetColor(ai_material, AI_MATKEY_COLOR_EMISSIVE);

		float shininess{};
		ai_material->Get(AI_MATKEY_SHININESS, shininess);
		material->specular.w = shininess;

		float shininess_strength{};
		ai_material->Get(AI_MATKEY_SHININESS_STRENGTH, shininess_strength);
		material->specular_exp = shininess_strength;

		materials_.push_back(std::move(material));
	}
}

void K::AssimpLoader::_WriteMaterial(std::wstring const& _file_name, std::string const& _path_tag)
{
	static auto WriteMaps = [](tinyxml2::XMLDocument* _document, tinyxml2::XMLElement* _element, std::vector<std::string> const& _maps) {
		for (size_t j = 0; j < _maps.size(); ++j)
		{
			tinyxml2::XMLElement* e{};
			auto const& map = _maps.at(j);
			e = _document->NewElement("Map");
			e->SetText(map.c_str());
			_element->LinkEndChild(e);
		}
	};
	static auto WriteColor = [](tinyxml2::XMLDocument* _document, tinyxml2::XMLElement* _element, Vector4 const& _color) {
		tinyxml2::XMLElement* e{};
		e = _document->NewElement("R");
		e->SetText(_color.x);
		_element->LinkEndChild(e);
		e = _document->NewElement("G");
		e->SetText(_color.y);
		_element->LinkEndChild(e);
		e = _document->NewElement("B");
		e->SetText(_color.z);
		_element->LinkEndChild(e);
		e = _document->NewElement("A");
		e->SetText(_color.w);
		_element->LinkEndChild(e);
	};

	auto document = std::make_unique<tinyxml2::XMLDocument>();

	auto declaration = document->NewDeclaration();
	document->LinkEndChild(declaration);

	auto root_element = document->NewElement("Materials");
	document->LinkEndChild(root_element);

	for (size_t i = 0; i < materials_.size(); ++i)
	{
		auto const& material = materials_.at(i);

		tinyxml2::XMLElement* e{};

		auto mat_element = document->NewElement("Mat");
		root_element->LinkEndChild(mat_element);

		e = document->NewElement("Name");
		e->SetText(material->name.c_str());
		mat_element->LinkEndChild(e);

		tinyxml2::XMLElement* maps_element{};

		maps_element = document->NewElement("DiffuseMaps");
		mat_element->LinkEndChild(maps_element);
		WriteMaps(document.get(), maps_element, material->diffuse_maps);

		maps_element = document->NewElement("SpecularMaps");
		mat_element->LinkEndChild(maps_element);
		WriteMaps(document.get(), maps_element, material->specular_maps);

		maps_element = document->NewElement("NormalMaps");
		mat_element->LinkEndChild(maps_element);
		WriteMaps(document.get(), maps_element, material->normal_maps);

		tinyxml2::XMLElement* color_element{};

		color_element = document->NewElement("Ambient");
		mat_element->LinkEndChild(color_element);
		WriteColor(document.get(), color_element, material->ambient);

		color_element = document->NewElement("Diffuse");
		mat_element->LinkEndChild(color_element);
		WriteColor(document.get(), color_element, material->diffuse);

		color_element = document->NewElement("Specular");
		mat_element->LinkEndChild(color_element);
		WriteColor(document.get(), color_element, material->specular);

		color_element = document->NewElement("Emissive");
		mat_element->LinkEndChild(color_element);
		WriteColor(document.get(), color_element, material->emissive);

		e = document->NewElement("SpecularExp");
		e->SetText(material->specular_exp);
		mat_element->LinkEndChild(e);
	}

	auto path_buffer = PathManager::singleton()->FindPath(_path_tag);

	path_buffer /= _file_name;

	document->SaveFile(path_buffer.string().c_str());
}

void K::AssimpLoader::_ReadAnimation()
{
}

void K::AssimpLoader::_WriteAnimation()
{
}
