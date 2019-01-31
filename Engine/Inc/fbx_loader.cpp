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

	// 이 친구는 Rotation과 Animation에만 영향을 줌.
	FbxAxisSystem::DirectX.ConvertScene(fbx_scene.get());
	// 이 친구는 Scaling에 영향을 줌.
	FbxSystemUnit::cm.ConvertScene(fbx_scene.get());

	fbx_importer_->Import(fbx_scene.get());

	FbxGeometryConverter fbx_geometry_converter{ fbx_manager_.get() };
	fbx_geometry_converter.Triangulate(fbx_scene.get(), true);

	auto root_node = fbx_scene->GetRootNode();

	_LoadMesh(root_node);
}

void K::FBXLoader::End()
{
	fbx_mesh_container_vector_.clear();
}

std::unique_ptr<FbxManager, std::function<void(FbxManager*)>> const& K::FBXLoader::fbx_manager() const
{
	return fbx_manager_;
}

std::unique_ptr<FbxIOSettings, std::function<void(FbxIOSettings*)>> const& K::FBXLoader::fbx_io_settings() const
{
	return fbx_io_settings_;
}

std::unique_ptr<FbxImporter, std::function<void(FbxImporter*)>> const& K::FBXLoader::fbx_importer() const
{
	return fbx_importer_;
}

void K::FBXLoader::_Finalize()
{
}

void K::FBXLoader::_LoadMesh(FbxNode* _node)
{
	auto attribute = _node->GetNodeAttribute();

	if (false == attribute)
		return;
	
	if (FbxNodeAttribute::eMesh == attribute->GetAttributeType())
		_LoadMesh(_node->GetMesh());

	auto child_count = _node->GetChildCount();

	for (auto i = 0; i < child_count; ++i)
		_LoadMesh(_node->GetChild(i));
}

void K::FBXLoader::_LoadMesh(FbxMesh* _mesh)
{
	auto fbx_mesh_container = std::unique_ptr<FBXMeshContainer, std::function<void(FBXMeshContainer*)>>{ new FBXMeshContainer, [](FBXMeshContainer* _p) {
		delete _p;
	} };

	fbx_mesh_container->bump_flag = false;
	fbx_mesh_container->animation_flag = false;

	auto polygon_count = _mesh->GetPolygonCount();

	for (auto i = 0; i < polygon_count; ++i)
	{
		for (auto j = 0; j < 3; ++j)
		{
			FBXIndex index{};

			index.idx_for_vtx = _mesh->GetPolygonVertex(i, j);
			index.idx_for_idx = 3 * i + j;

			auto& vertex = fbx_mesh_container->vertices.at(index.idx_for_vtx);

			_LoadPosition(vertex, _mesh, index.idx_for_vtx);

			_LoadNormal(vertex, _mesh, index);
			_LoadBinormal(vertex, _mesh, index);
			_LoadTangent(vertex, _mesh, index);

			index.idx_for_idx = _mesh->GetTextureUVIndex(i, j);
			_LoadUV(vertex, _mesh, index);
		}
	}

	fbx_mesh_container_vector_.push_back(std::move(fbx_mesh_container));
}

void K::FBXLoader::_LoadPosition(FBXVertex& _vtx, FbxMesh* _mesh, uint32_t _idx)
{
	auto control_points = _mesh->GetControlPoints();

	_vtx.position.x = -static_cast<float>(control_points[_idx].mData[0]);
	_vtx.position.y = static_cast<float>(control_points[_idx].mData[1]);
	_vtx.position.z = static_cast<float>(control_points[_idx].mData[2]);
}

void K::FBXLoader::_LoadNormal(FBXVertex& _vtx, FbxMesh* _mesh, FBXIndex const& _idx)
{
	if (auto normal = _mesh->GetElementNormal())
	{
		uint32_t idx{};

		switch (normal->GetMappingMode())
		{
		case FbxGeometryElementNormal::eByControlPoint:
			idx = _idx.idx_for_vtx;
			break;

		case FbxGeometryElementNormal::eByPolygonVertex:
			idx = _idx.idx_for_idx;
			break;
		}

		if(FbxGeometryElementNormal::eIndexToDirect == normal->GetReferenceMode())
			idx = normal->GetIndexArray().GetAt(idx);

		_vtx.normal.x = -static_cast<float>(normal->GetDirectArray().GetAt(idx).mData[0]);
		_vtx.normal.y = static_cast<float>(normal->GetDirectArray().GetAt(idx).mData[1]);
		_vtx.normal.z = static_cast<float>(normal->GetDirectArray().GetAt(idx).mData[2]);
	}
}

void K::FBXLoader::_LoadBinormal(FBXVertex& _vtx, FbxMesh* _mesh, FBXIndex const& _idx)
{
	if (auto binormal = _mesh->GetElementNormal())
	{
		uint32_t idx{};

		switch (binormal->GetMappingMode())
		{
		case FbxGeometryElementNormal::eByControlPoint:
			idx = _idx.idx_for_vtx;
			break;

		case FbxGeometryElementNormal::eByPolygonVertex:
			idx = _idx.idx_for_idx;
			break;
		}

		if (FbxGeometryElementNormal::eIndexToDirect == binormal->GetReferenceMode())
			idx = binormal->GetIndexArray().GetAt(idx);

		_vtx.binormal.x = -static_cast<float>(binormal->GetDirectArray().GetAt(idx).mData[0]);
		_vtx.binormal.y = static_cast<float>(binormal->GetDirectArray().GetAt(idx).mData[1]);
		_vtx.binormal.z = static_cast<float>(binormal->GetDirectArray().GetAt(idx).mData[2]);
	}
}

void K::FBXLoader::_LoadTangent(FBXVertex& _vtx, FbxMesh* _mesh, FBXIndex const& _idx)
{
	if (auto tangent = _mesh->GetElementNormal())
	{
		uint32_t idx{};

		switch (tangent->GetMappingMode())
		{
		case FbxGeometryElementNormal::eByControlPoint:
			idx = _idx.idx_for_vtx;
			break;

		case FbxGeometryElementNormal::eByPolygonVertex:
			idx = _idx.idx_for_idx;
			break;
		}

		if (FbxGeometryElementNormal::eIndexToDirect == tangent->GetReferenceMode())
			idx = tangent->GetIndexArray().GetAt(idx);

		_vtx.tangent.x = -static_cast<float>(tangent->GetDirectArray().GetAt(idx).mData[0]);
		_vtx.tangent.y = static_cast<float>(tangent->GetDirectArray().GetAt(idx).mData[1]);
		_vtx.tangent.z = static_cast<float>(tangent->GetDirectArray().GetAt(idx).mData[2]);
	}
}

void K::FBXLoader::_LoadUV(FBXVertex& _vtx, FbxMesh* _mesh, FBXIndex const& _idx)
{
	if (auto uv = _mesh->GetElementNormal())
	{
		uint32_t idx{};

		switch (uv->GetMappingMode())
		{
		case FbxGeometryElementNormal::eByControlPoint:
			idx = _idx.idx_for_vtx;
			break;

		case FbxGeometryElementNormal::eByPolygonVertex:
			idx = _idx.idx_for_idx;
			break;
		}

		if (FbxGeometryElementNormal::eIndexToDirect == uv->GetReferenceMode())
			idx = uv->GetIndexArray().GetAt(idx);

		// TODO: 소수점 잘라내야 하는지 여부 체크해야 함, 주소지정모드 Wrap 모드를 사용한다고 함, 그리고 y도 뒤집어야 하는지 체크해야 함
		_vtx.uv.x = -static_cast<float>(uv->GetDirectArray().GetAt(idx).mData[0]);
		_vtx.uv.y = static_cast<float>(uv->GetDirectArray().GetAt(idx).mData[1]);
	}
}
