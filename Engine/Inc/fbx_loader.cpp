#include "Engine.h"
#include "fbx_loader.h"

#include "path_manager.h"

void K::FBXLoader::Initialize()
{
	try
	{
		fbx_manager_ = std::unique_ptr<FbxManager, std::function<void(FbxManager*)>>{ FbxManager::Create(), [](FbxManager* _p) {
			_p->Destroy();
		} };

		fbx_io_settings_ = std::unique_ptr<FbxIOSettings, std::function<void(FbxIOSettings*)>>{ FbxIOSettings::Create(fbx_manager_.get(), IOSROOT), [](FbxIOSettings* _p) {
			_p->Destroy();
		} };

		fbx_manager_->SetIOSettings(fbx_io_settings_.get());

		fbx_importer_ = std::unique_ptr<FbxImporter, std::function<void(FbxImporter*)>>{ FbxImporter::Create(fbx_manager_.get(), ""), [](FbxImporter* _p) {
			_p->Destroy();
		} };
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "FBXLoader::Initialize" << std::endl;
	}
}

void K::FBXLoader::Begin(std::wstring const& _file_name, std::string const& _path_tag)
{
	auto fbx_scene = std::unique_ptr<FbxScene, std::function<void(FbxScene*)>>{ FbxScene::Create(fbx_manager_.get(), ""), [](FbxScene* _p) {
		_p->Destroy();
	} };

	auto path_buffer = PathManager::singleton()->FindPath(_path_tag);

	path_buffer /= _file_name;

	fbx_importer_->Initialize(path_buffer.string().c_str(), -1, fbx_io_settings_.get());

	fbx_importer_->Import(fbx_scene.get());

	FbxGeometryConverter fbx_geometry_converter{ fbx_manager_.get() };
	fbx_geometry_converter.Triangulate(fbx_scene.get(), true);

	auto root_node = fbx_scene->GetRootNode();

	_LoadMesh(root_node);
	_LoadMaterial(root_node);
}

void K::FBXLoader::End()
{
	fbx_mesh_container_vector_.clear();
}

std::vector<std::unique_ptr<K::FBXMeshContainer, std::function<void(K::FBXMeshContainer*)>>> const& K::FBXLoader::fbx_mesh_container_vector()
{
	return fbx_mesh_container_vector_;
}

std::vector<std::vector<std::unique_ptr<K::FBXMaterial, std::function<void(K::FBXMaterial*)>>>> const& K::FBXLoader::fbx_material_2d_vector()
{
	return fbx_material_2d_vector_;
}

void K::FBXLoader::_Finalize()
{
}

void K::FBXLoader::_LoadMesh(FbxNode* _fbx_node)
{
	if (auto fbx_node_attribute = _fbx_node->GetNodeAttribute())
	{
		if (FbxNodeAttribute::eMesh == fbx_node_attribute->GetAttributeType())
			_LoadMesh(_fbx_node->GetMesh());
	}

	for (auto i = 0; i < _fbx_node->GetChildCount(); ++i)
		_LoadMesh(_fbx_node->GetChild(i));
}

void K::FBXLoader::_LoadMesh(FbxMesh* _fbx_mesh)
{
	auto fbx_mesh_container = std::unique_ptr<FBXMeshContainer, std::function<void(FBXMeshContainer*)>>{ new FBXMeshContainer, [](FBXMeshContainer* _p) {
		delete _p;
	} };

	auto triangle_count = _fbx_mesh->GetPolygonCount();
	for (auto i = 0; i < triangle_count; ++i)
	{
		std::array<uint32_t, 3> index{};

		for (auto j = 0; j < 3; ++j)
		{
			auto control_point_idx = _fbx_mesh->GetPolygonVertex(i, j);
			auto polygon_vertex_idx = 3 * i + j;

			FBXVertex vertex{};

			_LoadPosition(vertex, _fbx_mesh, control_point_idx);
			_LoadUV(vertex, _fbx_mesh, _fbx_mesh->GetTextureUVIndex(i, j), polygon_vertex_idx);
			_LoadNormal(vertex, _fbx_mesh, control_point_idx, polygon_vertex_idx);
			_LoadBinormal(vertex, _fbx_mesh, control_point_idx, polygon_vertex_idx);
			_LoadTangent(vertex, _fbx_mesh, control_point_idx, polygon_vertex_idx);

			fbx_mesh_container->vertices.push_back(std::move(vertex));

			index.at(j) = static_cast<uint32_t>(polygon_vertex_idx);
		}

		// 애초에 모델이 생성된 좌표계가 OpenGL 좌표계(오른손 좌표계)인데 여기서는 CCW를 FrontFace로 친다. 그래서 인덱스를 바꿔줘야 한다.
		fbx_mesh_container->indices.push_back(std::move(index.at(0)));
		fbx_mesh_container->indices.push_back(std::move(index.at(2)));
		fbx_mesh_container->indices.push_back(std::move(index.at(1)));
	}

	fbx_mesh_container_vector_.push_back(std::move(fbx_mesh_container));
}

void K::FBXLoader::_LoadPosition(FBXVertex& _vertex, FbxMesh* _fbx_mesh, uint32_t _control_point_idx)
{
	_vertex.position.x = static_cast<float>(_fbx_mesh->GetControlPointAt(_control_point_idx).mData[0]);
	_vertex.position.y = static_cast<float>(_fbx_mesh->GetControlPointAt(_control_point_idx).mData[2]);
	_vertex.position.z = static_cast<float>(_fbx_mesh->GetControlPointAt(_control_point_idx).mData[1]);
}

void K::FBXLoader::_LoadUV(FBXVertex& _vertex, FbxMesh* _fbx_mesh, uint32_t _control_point_idx, uint32_t _polygon_vertex_idx)
{
	auto uv = _fbx_mesh->GetElementUV();

	uint32_t index{};

	switch (uv->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		index = _control_point_idx;
		break;

	case FbxGeometryElement::eByPolygonVertex:
		index = _polygon_vertex_idx;
		break;
	}

	if (FbxGeometryElement::eIndexToDirect == uv->GetReferenceMode())
		index = uv->GetIndexArray().GetAt(index);

	auto fbx_uv = uv->GetDirectArray().GetAt(index);

	_vertex.uv.x = static_cast<float>(fbx_uv.mData[0] - static_cast<int>(fbx_uv.mData[0]));
	_vertex.uv.y = 1.f - static_cast<float>(fbx_uv.mData[1] - static_cast<int>(fbx_uv.mData[1]));
}

void K::FBXLoader::_LoadNormal(FBXVertex& _vertex, FbxMesh* _fbx_mesh, uint32_t _control_point_idx, uint32_t _polygon_vertex_idx)
{
	auto normal = _fbx_mesh->GetElementNormal();

	uint32_t index{};

	switch (normal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		index = _control_point_idx;
		break;

	case FbxGeometryElement::eByPolygonVertex:
		index = _polygon_vertex_idx;
		break;
	}

	if (FbxGeometryElement::eIndexToDirect == normal->GetReferenceMode())
		index = normal->GetIndexArray().GetAt(index);

	auto fbx_normal = normal->GetDirectArray().GetAt(index);

	_vertex.normal.x = static_cast<float>(fbx_normal.mData[0]);
	_vertex.normal.y = static_cast<float>(fbx_normal.mData[2]);
	_vertex.normal.z = static_cast<float>(fbx_normal.mData[1]);
}

void K::FBXLoader::_LoadBinormal(FBXVertex& _vertex, FbxMesh* _fbx_mesh, uint32_t _control_point_idx, uint32_t _polygon_vertex_idx)
{
	auto binormal = _fbx_mesh->GetElementBinormal();

	uint32_t index{};

	switch (binormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		index = _control_point_idx;
		break;

	case FbxGeometryElement::eByPolygonVertex:
		index = _polygon_vertex_idx;
		break;
	}

	if (FbxGeometryElement::eIndexToDirect == binormal->GetReferenceMode())
		index = binormal->GetIndexArray().GetAt(index);

	auto fbx_binormal = binormal->GetDirectArray().GetAt(index);

	_vertex.binormal.x = static_cast<float>(fbx_binormal.mData[0]);
	_vertex.binormal.y = static_cast<float>(fbx_binormal.mData[2]);
	_vertex.binormal.z = static_cast<float>(fbx_binormal.mData[1]);
}

void K::FBXLoader::_LoadTangent(FBXVertex& _vertex, FbxMesh* _fbx_mesh, uint32_t _control_point_idx, uint32_t _polygon_vertex_idx)
{
	auto tangent = _fbx_mesh->GetElementTangent();

	uint32_t index{};

	switch (tangent->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		index = _control_point_idx;
		break;

	case FbxGeometryElement::eByPolygonVertex:
		index = _polygon_vertex_idx;
		break;
	}

	if (FbxGeometryElement::eIndexToDirect == tangent->GetReferenceMode())
		index = tangent->GetIndexArray().GetAt(index);

	auto fbx_tangent = tangent->GetDirectArray().GetAt(index);

	_vertex.tangent.x = static_cast<float>(fbx_tangent.mData[0]);
	_vertex.tangent.y = static_cast<float>(fbx_tangent.mData[2]);
	_vertex.tangent.z = static_cast<float>(fbx_tangent.mData[1]);
}

void K::FBXLoader::_LoadMaterial(FbxNode* _fbx_node)
{
	if (auto material_count = _fbx_node->GetMaterialCount())
	{
		fbx_material_2d_vector_.push_back(std::vector<std::unique_ptr<FBXMaterial, std::function<void(FBXMaterial*)>>>{});

		for (auto i = 0; i < material_count; ++i)
			_LoadMaterial(_fbx_node->GetMaterial(i));
	}

	for (auto i = 0; i < _fbx_node->GetChildCount(); ++i)
		_LoadMaterial(_fbx_node->GetChild(i));
}

void K::FBXLoader::_LoadMaterial(FbxSurfaceMaterial* _fbx_surface_material)
{
	auto fbx_material = std::unique_ptr<FBXMaterial, std::function<void(FBXMaterial*)>>{ new FBXMaterial, [](FBXMaterial* _p) {
		delete _p;
	} };

	fbx_material->ambient = _LoadColor(_fbx_surface_material, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	fbx_material->diffuse = _LoadColor(_fbx_surface_material, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	fbx_material->specular = _LoadColor(_fbx_surface_material, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
	fbx_material->emissive = _LoadColor(_fbx_surface_material, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);

	fbx_material->diffuse_texture = _LoadTexture(_fbx_surface_material, FbxSurfaceMaterial::sDiffuse).filename();
	fbx_material->specular_texture = _LoadTexture(_fbx_surface_material, FbxSurfaceMaterial::sSpecular).filename();
	fbx_material->bump_texture = _LoadTexture(_fbx_surface_material, FbxSurfaceMaterial::sBump).filename();

	if (fbx_material->bump_texture.empty())
		fbx_material->bump_texture = _LoadTexture(_fbx_surface_material, FbxSurfaceMaterial::sNormalMap).filename();

	fbx_material->diffuse_texture.replace_extension(".dds");
	fbx_material->specular_texture.replace_extension(".dds");
	fbx_material->bump_texture.replace_extension(".dds");

	fbx_material_2d_vector_.at(fbx_material_2d_vector_.size() - 1).push_back(std::move(fbx_material));
}

K::Vector4 K::FBXLoader::_LoadColor(FbxSurfaceMaterial* _fbx_surface_material, std::string const& _color_property_name, std::string const& _color_factor_property_name)
{
	auto color_property = _fbx_surface_material->FindProperty(_color_property_name.c_str());

	FbxDouble3 color{};
	if (color_property.IsValid())
	{
		color = color_property.Get<FbxDouble3>();
		auto color_factor = _LoadFactor(_fbx_surface_material, _color_factor_property_name.c_str());

		color[0] *= color_factor;
		color[1] *= color_factor;
		color[2] *= color_factor;
	}

	return Vector4{ static_cast<float>(color[0]), static_cast<float>(color[1]), static_cast<float>(color[2]), 1.f };
}

float K::FBXLoader::_LoadFactor(FbxSurfaceMaterial* _fbx_surface_material, std::string const& _factor_property_name)
{
	auto factor_property = _fbx_surface_material->FindProperty(_factor_property_name.c_str());

	float factor{};
	if (factor_property.IsValid())
		factor = static_cast<float>(factor_property.Get<FbxDouble>());

	return factor;
}

std::filesystem::path K::FBXLoader::_LoadTexture(FbxSurfaceMaterial* _fbx_surface_material, std::string const& _texture_property_name)
{
	auto texture_property = _fbx_surface_material->FindProperty(_texture_property_name.c_str());

	std::filesystem::path file_name{};
	if (texture_property.IsValid())
	{
		if (texture_property.GetSrcObjectCount<FbxFileTexture>())
			file_name = texture_property.GetSrcObject<FbxFileTexture>()->GetFileName();
	}

	return file_name;
}
