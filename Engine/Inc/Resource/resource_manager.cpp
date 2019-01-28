#include "Engine.h"
#include "resource_manager.h"

#include "path_manager.h"
#include "mesh.h"
#include "texture.h"
#include "sampler.h"

std::shared_ptr<K::Mesh> K::ResourceManager::mesh_dummy_{};
std::shared_ptr<K::Texture> K::ResourceManager::texture_dummy_{};
std::shared_ptr<K::Sampler> K::ResourceManager::sampler_dummy_{};
std::shared_ptr<K::ANIMATION_2D_CLIP_DESC> K::ResourceManager::animation_2d_clip_dummy_{};

void K::ResourceManager::Initialize()
{
	try
	{
#pragma region Mesh
		VertexTex tex_rect_vertices[4]{
			{ { -0.5f, -0.5f, 0.f }, { 0.f, 1.f } },
			{ { -0.5f, 0.5f, 0.f }, { 0.f, 0.f } },
			{ { 0.5f, 0.5f, 0.f }, { 1.f, 0.f } },
			{ { 0.5f, -0.5f, 0.f }, { 1.f, 1.f } }
		};

		uint16_t tex_rect_indices[6]{ 0, 1, 2, 0, 2, 3 };

		_CreateMesh(
			TEX_RECT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			tex_rect_vertices, sizeof(VertexTex), 4, D3D11_USAGE_DEFAULT,
			tex_rect_indices, sizeof(uint16_t), 6, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R16_UINT
		);

		Vector3 collider_rect_vertices[5]{
			{ -0.5f, -0.5f, 0.f },
			{ -0.5f, 0.5f, 0.f },
			{ 0.5f, 0.5f, 0.f },
			{ 0.5f, -0.5f, 0.f },
			{ -0.5f, -0.5f, 0.f }
		};

		_CreateMesh(
			COLLIDER_RECT, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
			collider_rect_vertices, sizeof(Vector3), 5, D3D11_USAGE_DEFAULT
		);

		Vector3 collider_circle_vertices[37]{};
		for (auto i = 0; i < 37; ++i)
			collider_circle_vertices[i] = { cos(DirectX::XMConvertToRadians(i * 10.f)) * 0.5f, sin(DirectX::XMConvertToRadians(i * 10.f)) * 0.5f, 0.f };

		_CreateMesh(
			COLLIDER_CIRCLE, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			collider_circle_vertices, sizeof(Vector3), 37, D3D11_USAGE_DEFAULT
		);

		VertexTex instance_tex_rect_vertices[4]{
			{ { -0.5f, -0.5f, 0.f }, { 0.f, 1.f } },
			{ { -0.5f, 0.5f, 0.f }, { 0.f, 0.f } },
			{ { 0.5f, 0.5f, 0.f }, { 1.f, 0.f } },
			{ { 0.5f, -0.5f, 0.f }, { 1.f, 1.f } }
		};

		uint16_t instance_tex_rect_indices[6]{ 0, 1, 2, 1, 3, 2 };

		MatrixTex instance_tex_rect_instances[10000]{};

		_CreateMesh(
			INSTANCE_TEX_RECT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			instance_tex_rect_vertices, sizeof(VertexTex), 4, D3D11_USAGE_DEFAULT,
			instance_tex_rect_indices, sizeof(uint16_t), 6, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R16_UINT,
			instance_tex_rect_instances, sizeof(MatrixTex), 10000, D3D11_USAGE_DYNAMIC
		);

		// pyramid
		Vector3 pyramid_position[5]{
			Vector3{ 0.f, 0.5f, 0.f },
			Vector3{ -0.5f, -0.5f, 0.5f },
			Vector3{ 0.5f, -0.5f, 0.5f },
			Vector3{ 0.5f, -0.5f, -0.5f },
			Vector3{ -0.5f, -0.5f, -0.5f }
		};

		Vector3 side_face_normal[4]{};
		Vector3 edge1{}, edge2{};
		
		edge1 = pyramid_position[4] - pyramid_position[0];
		edge2 = pyramid_position[1] - pyramid_position[0];
		edge1.Normalize();
		edge2.Normalize();
		side_face_normal[0] = edge1.Cross(edge2);
		side_face_normal[0].Normalize();

		edge1 = pyramid_position[1] - pyramid_position[0];
		edge2 = pyramid_position[2] - pyramid_position[0];
		edge1.Normalize();
		edge2.Normalize();
		side_face_normal[1] = edge1.Cross(edge2);
		side_face_normal[1].Normalize();

		edge1 = pyramid_position[2] - pyramid_position[0];
		edge2 = pyramid_position[3] - pyramid_position[0];
		edge1.Normalize();
		edge2.Normalize();
		side_face_normal[2] = edge1.Cross(edge2);
		side_face_normal[2].Normalize();

		edge1 = pyramid_position[3] - pyramid_position[0];
		edge2 = pyramid_position[4] - pyramid_position[0];
		edge1.Normalize();
		edge2.Normalize();
		side_face_normal[3] = edge1.Cross(edge2);
		side_face_normal[3].Normalize();

		Vector3 pyramid_normal[4]{};

		pyramid_normal[0] = side_face_normal[0] + side_face_normal[1];
		pyramid_normal[0].Normalize();

		pyramid_normal[1] = side_face_normal[1] + side_face_normal[2];
		pyramid_normal[1].Normalize();

		pyramid_normal[2] = side_face_normal[2] + side_face_normal[3];
		pyramid_normal[2].Normalize();

		pyramid_normal[3] = side_face_normal[3] + side_face_normal[0];
		pyramid_normal[3].Normalize();

		VertexNormalColor pyramid_vertices[9]{
			VertexNormalColor{ pyramid_position[0], Vector3::UnitY, DirectX::Colors::Black.v },
			VertexNormalColor{ pyramid_position[1], pyramid_normal[0], DirectX::Colors::White.v },
			VertexNormalColor{ pyramid_position[2], pyramid_normal[1], DirectX::Colors::Red.v },
			VertexNormalColor{ pyramid_position[3], pyramid_normal[2], DirectX::Colors::Green.v },
			VertexNormalColor{ pyramid_position[4], pyramid_normal[3], DirectX::Colors::Blue.v },
			VertexNormalColor{ pyramid_position[1], -Vector3::UnitY, DirectX::Colors::White.v },
			VertexNormalColor{ pyramid_position[2], -Vector3::UnitY, DirectX::Colors::White.v },
			VertexNormalColor{ pyramid_position[3], -Vector3::UnitY, DirectX::Colors::White.v },
			VertexNormalColor{ pyramid_position[4], -Vector3::UnitY, DirectX::Colors::White.v }
		};

		uint16_t pyramid_indices[18]{ 1, 0, 4, 2, 0, 1, 3, 0, 2, 4, 0, 3, 5, 8, 6, 6, 8, 7 };

		_CreateMesh(
			NORMAL_PYRAMID, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			pyramid_vertices, sizeof(VertexNormalColor), 9, D3D11_USAGE_DEFAULT,
			pyramid_indices, sizeof(uint16_t), 18, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R16_UINT
		);

		VertexTex full_screen_vertices[4]{
			{ Vector3{ -1.f, -1.f, 0.f }, Vector2{ 0.f, 1.f } },
			{ Vector3{ -1.f, 1.f, 0.f }, Vector2{ 0.f, 0.f } },
			{ Vector3{ 1.f, 1.f, 0.f }, Vector2{ 1.f, 0.f } },
			{ Vector3{ 1.f, -1.f, 0.f }, Vector2{ 1.f, 1.f } }
		};

		uint16_t full_screen_indices[6]{ 0, 1, 2, 0, 2, 3 };

		_CreateMesh(
			FULL_SCREEN_RECT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			full_screen_vertices, sizeof(VertexTex), 4, D3D11_USAGE_DEFAULT,
			full_screen_indices, sizeof(uint16_t), 6, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R16_UINT
		);

		_CreateSphereMesh(SPHERE_VOLUME, 0.5f, 32, 16);
		_CreateCylinderMesh(CYLINDER_VOLUME, 1.f, 0.5f, 32);
		_CreateCapsuleMesh(CAPSULE_VOLUME, 1.f, 0.5f, 32, 16);
#pragma endregion

#pragma region Texture
#pragma endregion

#pragma region Sampler
		_CreateSampler(
			LINEAR_SAMPLER,
			D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			D3D11_TEXTURE_ADDRESS_CLAMP,
			D3D11_TEXTURE_ADDRESS_CLAMP,
			D3D11_TEXTURE_ADDRESS_CLAMP
		);

		_CreateSampler(
			POINT_SAMPLER,
			D3D11_FILTER_MIN_MAG_MIP_POINT,
			D3D11_TEXTURE_ADDRESS_CLAMP,
			D3D11_TEXTURE_ADDRESS_CLAMP,
			D3D11_TEXTURE_ADDRESS_CLAMP
		);
#pragma endregion

#pragma region Animation2DClip
#pragma endregion
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "ResourceManager::Initialize" << std::endl;
	}
}

std::shared_ptr<K::Mesh> const& K::ResourceManager::FindMesh(std::string const& _tag) const
{
	auto iter = mesh_map_.find(_tag);

	if (iter == mesh_map_.end())
		return mesh_dummy_;

	return iter->second;
}

std::shared_ptr<K::Texture> const& K::ResourceManager::FindTexture(std::string const& _tag) const
{
	auto iter = texture_map_.find(_tag);

	if (iter == texture_map_.end())
		return texture_dummy_;

	return iter->second;
}

std::shared_ptr<K::Sampler> const& K::ResourceManager::FindSampler(std::string const& _tag) const
{
	auto iter = sampler_map_.find(_tag);

	if (iter == sampler_map_.end())
		return sampler_dummy_;

	return iter->second;
}

std::shared_ptr<K::ANIMATION_2D_CLIP_DESC> const& K::ResourceManager::FindAnimation2DClip(std::string const& _tag) const
{
	auto iter = animation_2d_clip_map_.find(_tag);

	if (iter == animation_2d_clip_map_.end())
		return animation_2d_clip_dummy_;

	return iter->second;
}

void K::ResourceManager::_Finalize()
{
}

void K::ResourceManager::_CreateMesh(
	std::string const& _tag, D3D11_PRIMITIVE_TOPOLOGY _topology,
	void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage)
{
	if (FindMesh(_tag))
		throw std::exception{ "ResourceManager::_CreateMesh" };

	auto mesh = std::shared_ptr<Mesh>{ new Mesh, [](Mesh* _p) {
		delete _p;
	} };

	mesh->_CreateMesh(_topology, _vtx_data, _vtx_stride, _vtx_count, _vtx_usage);

	mesh_map_.insert(std::make_pair(_tag, std::move(mesh)));
}

void K::ResourceManager::_CreateMesh(
	std::string const& _tag, D3D11_PRIMITIVE_TOPOLOGY _topology,
	void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage,
	void* _idx_data, int _idx_stride, int _idx_count, D3D11_USAGE _idx_usage, DXGI_FORMAT _idx_format)
{
	if (FindMesh(_tag))
		throw std::exception{ "ResourceManager::_CreateMesh" };

	auto mesh = std::shared_ptr<Mesh>{ new Mesh, [](Mesh* _p) {
		delete _p;
	} };

	mesh->_CreateMesh(_topology, _vtx_data, _vtx_stride, _vtx_count, _vtx_usage, _idx_data, _idx_stride, _idx_count, _idx_usage, _idx_format);

	mesh_map_.insert(std::make_pair(_tag, std::move(mesh)));
}

void K::ResourceManager::_CreateMesh(
	std::string const& _tag, D3D11_PRIMITIVE_TOPOLOGY _topology,
	void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage,
	void* _idx_data, int _idx_stride, int _idx_count, D3D11_USAGE _idx_usage, DXGI_FORMAT _idx_format,
	void* _inst_data, int _inst_stride, int _inst_count, D3D11_USAGE _inst_usage)
{
	if(FindMesh(_tag))
		throw std::exception{ "ResourceManager::_CreateMesh" };

	auto mesh = std::shared_ptr<Mesh>{ new Mesh, [](Mesh* _p) {
		delete _p;
	} };

	mesh->_CreateMesh(_topology, _vtx_data, _vtx_stride, _vtx_count, _vtx_usage, _idx_data, _idx_stride, _idx_count, _idx_usage, _idx_format, _inst_data, _inst_stride, _inst_count, _inst_usage);

	mesh_map_.insert(std::make_pair(_tag, std::move(mesh)));
}

void K::ResourceManager::_CreateTexture2D(std::string const& _tag, std::wstring const& _file_name, std::string const& _path_tag)
{
	if (FindTexture(_tag))
		throw std::exception{ "ResourceManager::_CreateTexture2D" };

	auto texture = std::shared_ptr<Texture>{ new Texture, [](Texture* _p) {
		delete _p;
	} };

	texture->_CreateTexture2D(_file_name, _path_tag);

	texture_map_.insert(std::make_pair(_tag, std::move(texture)));
}

void K::ResourceManager::_CreateSampler(
	std::string const& _tag,
	D3D11_FILTER _filter,
	D3D11_TEXTURE_ADDRESS_MODE _address_u,
	D3D11_TEXTURE_ADDRESS_MODE _address_v,
	D3D11_TEXTURE_ADDRESS_MODE _address_w)
{
	if (FindSampler(_tag))
		throw std::exception{ "ResourceManager::_CreateSampler" };

	auto sampler = std::shared_ptr<Sampler>{ new Sampler, [](Sampler* _p) {
		delete _p;
	} };

	sampler->_CreateSampler(_filter, _address_u, _address_v, _address_w);

	sampler_map_.insert(std::make_pair(_tag, std::move(sampler)));
}

void K::ResourceManager::_CreateAnimation2DClip(std::string const& _tag, std::wstring const& _file_name, std::string const& _path_tag)
{
	std::filesystem::path path_buffer = PathManager::singleton()->FindPath(_path_tag);

	if (path_buffer.empty())
		throw std::exception{ "ResourceManager::_CreateAnimation2DClip" };

	path_buffer /= _file_name;

	std::ifstream file{ path_buffer };

	if(file.fail())
		throw std::exception{ "ResourceManager::_CreateAnimation2DClip" };

	std::string line{};
	std::stringstream line_stream{};

	std::getline(file, line);
	std::getline(file, line);
	line_stream.str(line);

	std::string clip_tag{};
	std::string texture_tag{};
	std::string type{};
	std::string option{};
	std::string width{};
	std::string height{};
	std::string completion_time{};

	std::getline(line_stream, clip_tag, ',');
	std::getline(line_stream, texture_tag, ',');
	std::getline(line_stream, type, ',');
	std::getline(line_stream, option, ',');
	std::getline(line_stream, width, ',');
	std::getline(line_stream, height, ',');
	std::getline(line_stream, completion_time, ',');

	if(nullptr == FindTexture(clip_tag))
		throw std::exception{ "ResourceManager::_CreateAnimation2DClip" };

	auto animation_2d_clip = std::make_shared<ANIMATION_2D_CLIP_DESC>();
	animation_2d_clip->clip_tag = clip_tag;
	animation_2d_clip->texture_tag = texture_tag;
	animation_2d_clip->type = static_cast<ANIMATION_2D_TYPE>(std::stoi(type));
	animation_2d_clip->option = static_cast<ANIMATION_OPTION>(std::stoi(option));
	animation_2d_clip->width = std::stof(width);
	animation_2d_clip->height = std::stof(height);
	animation_2d_clip->completion_time = std::stof(completion_time);

	std::getline(file, line);
	std::getline(file, line);

	while (true)
	{
		line.clear();
		line_stream.clear();

		std::string left{};
		std::string top{};
		std::string right{};
		std::string bottom{};

		getline(file, line);
		line_stream.str(line);

		if (line.empty())
			break;

		getline(line_stream, left, ',');
		getline(line_stream, top, ',');
		getline(line_stream, right, ',');
		getline(line_stream, bottom, ',');

		ANIMATION_2D_FRAME_DESC frame{};
		frame.LT = Vector2{ std::stof(left), std::stof(top) };
		frame.RB = Vector2{ std::stof(right), std::stof(bottom) };

		animation_2d_clip->frame_vector.push_back(std::move(frame));
	}
}

void K::ResourceManager::_CreateSphereMesh(std::string const& _tag, float _radius, int _slice_count, int _stack_count)
{
	std::vector<VertexNormalColor> sphere_vertices{};

	float phi = DirectX::XM_PI / _stack_count;
	float theta = DirectX::XM_2PI / _slice_count;

	for (auto i = 0; i <= _stack_count; ++i)
	{
		for (auto j = 0; j < _slice_count; ++j)
		{
			VertexNormalColor vertex{};

			if (j < _slice_count / 2)
				vertex.position = Vector3{ _radius * sin(phi * i) * cos(theta * j), _radius * cos(phi * i), _radius * sin(phi * i) * sin(theta * j) };
			else
				vertex.position = Vector3{ _radius * sin(phi * i) * -cos(theta * j - DirectX::XM_PI), _radius * cos(phi * i), _radius * sin(phi * i) * -sin(theta * j - DirectX::XM_PI) };

			vertex.normal = vertex.position;
			vertex.normal.Normalize();
			vertex.color = Vector4{ (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f };

			sphere_vertices.push_back(std::move(vertex));
		}
	}

	std::vector<uint16_t> sphere_indices{};

	for (auto i = 0; i <= _stack_count; ++i)
	{
		for (auto j = 0; j < _slice_count; ++j)
		{
			uint16_t index[6]{};
			index[0] = _slice_count * i + j;
			index[1] = _slice_count * (i + 1) + j + 1;
			index[2] = _slice_count * (i + 1) + j;
			index[3] = _slice_count * i + j;
			index[4] = _slice_count * i + j + 1;
			index[5] = _slice_count * (i + 1) + j + 1;

			if (j == _slice_count - 1)
			{
				index[1] = _slice_count * i + j + 1;
				index[4] = _slice_count * (i - 1) + j + 1;
				index[5] = _slice_count * i + j + 1;
			}

			sphere_indices.push_back(std::move(index[0]));
			sphere_indices.push_back(std::move(index[1]));
			sphere_indices.push_back(std::move(index[2]));
			sphere_indices.push_back(std::move(index[3]));
			sphere_indices.push_back(std::move(index[4]));
			sphere_indices.push_back(std::move(index[5]));
		}
	}

	_CreateMesh(_tag, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		sphere_vertices.data(), sizeof(VertexNormalColor), static_cast<int>(sphere_vertices.size()), D3D11_USAGE_DEFAULT,
		sphere_indices.data(), sizeof(uint16_t), static_cast<int>(sphere_indices.size()), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R16_UINT
	);
}

void K::ResourceManager::_CreateCylinderMesh(std::string const& _tag, float _height, float _radius, int _slice_count)
{
	std::vector<VertexNormalColor> cylinder_vertices{};

	auto theta = DirectX::XM_2PI / _slice_count;

	for (auto i = 0; i < 2; ++i)
	{
		for (auto j = 0; j < _slice_count; ++j)
		{
			VertexNormalColor vertex{};

			if (j < _slice_count / 2)
				vertex.position = Vector3{ _radius * cos(theta * j), (0.5f - i) * _height, _radius * sin(theta * j) };
			else
				vertex.position = Vector3{ _radius * -cos(theta * j - DirectX::XM_PI), (0.5f - i) * _height, _radius * -sin(theta * j - DirectX::XM_PI) };

			vertex.normal = vertex.position;
			vertex.normal.y = 0.f;
			vertex.normal.Normalize();
			vertex.color = Vector4{ (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f };

			cylinder_vertices.push_back(vertex);
			
			// À­, ¾Æ·§¸éÀ» À§ÇÑ normalÀ» µé°í ÀÖ´Â Á¤Á¡
			if(0 == i)
				vertex.normal = Vector3::UnitY;
			else if(1 == i)
				vertex.normal = -Vector3::UnitY;

			cylinder_vertices.push_back(std::move(vertex));
		}
	}

	std::vector<uint16_t> cylinder_indices{};

	// À­¸é
	for (auto i = 0; i < _slice_count - 2; ++i)
	{
		uint16_t index[3]{};
		index[0] = 1;
		index[1] = 1 + (i + 2) * 2;
		index[2] = 1 + (i + 1) * 2;

		cylinder_indices.push_back(std::move(index[0]));
		cylinder_indices.push_back(std::move(index[1]));
		cylinder_indices.push_back(std::move(index[2]));
	}

	// ¿·¸é
	for (auto i = 0; i < _slice_count; ++i)
	{
		uint16_t index[6]{};
		index[0] = i * 2;
		index[1] = (i + _slice_count + 1) * 2;
		index[2] = (i + _slice_count) * 2;
		index[3] = i * 2;
		index[4] = (i + 1) * 2;
		index[5] = (i + _slice_count + 1) * 2;

		if (i == _slice_count - 1)
		{
			index[1] = (i + 1) * 2;
			index[4] = (i - _slice_count + 1) * 2;
			index[5] = (i + 1) * 2;
		}

		cylinder_indices.push_back(std::move(index[0]));
		cylinder_indices.push_back(std::move(index[1]));
		cylinder_indices.push_back(std::move(index[2]));
		cylinder_indices.push_back(std::move(index[3]));
		cylinder_indices.push_back(std::move(index[4]));
		cylinder_indices.push_back(std::move(index[5]));
	}

	// ¾Æ·§¸é
	for (auto i = 0; i < _slice_count - 2; ++i)
	{
		uint16_t index[3]{};
		index[0] = 1 + _slice_count * 2;
		index[1] = 1 + _slice_count * 2 + (i + 1) * 2;
		index[2] = 1 + _slice_count * 2 + (i + 2) * 2;

		cylinder_indices.push_back(std::move(index[0]));
		cylinder_indices.push_back(std::move(index[1]));
		cylinder_indices.push_back(std::move(index[2]));
	}

	_CreateMesh(_tag, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		cylinder_vertices.data(), sizeof(VertexNormalColor), static_cast<int>(cylinder_vertices.size()), D3D11_USAGE_DEFAULT,
		cylinder_indices.data(), sizeof(uint16_t), static_cast<int>(cylinder_indices.size()), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R16_UINT
	);
}

void K::ResourceManager::_CreateCapsuleMesh(std::string const& _tag, float _height, float _radius, int _slice_count, int _stack_count)
{
	std::vector<VertexNormalColor> capsule_vertices{};

	auto stack_count = _stack_count;

	if (stack_count % 2 == 0)
		stack_count -= 1;

	float phi = DirectX::XM_PI / stack_count;
	float theta = DirectX::XM_2PI / _slice_count;

	for (auto i = 0; i <= stack_count; ++i)
	{
		for (auto j = 0; j < _slice_count; ++j)
		{
			VertexNormalColor vertex{};

			auto y = _radius * cos(phi * i);

			if (i <= stack_count / 2)
				y += _height * 0.5f;
			else
				y += -_height * 0.5f;

			if (j < _slice_count / 2)
				vertex.position = Vector3{ _radius * sin(phi * i) * cos(theta * j), y, _radius * sin(phi * i) * sin(theta * j) };
			else
				vertex.position = Vector3{ _radius * sin(phi * i) * -cos(theta * j - DirectX::XM_PI), y, _radius * sin(phi * i) * -sin(theta * j - DirectX::XM_PI) };

			vertex.normal = vertex.position;

			if (i == stack_count / 2)
				vertex.normal.y = 0.f;
			else if (i == stack_count / 2 + 1)
				vertex.normal.y = 0.f;

			vertex.normal.Normalize();
			vertex.color = Vector4{ (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f };

			capsule_vertices.push_back(std::move(vertex));
		}
	}

	std::vector<uint16_t> capsule_indices{};

	for (auto i = 0; i <= stack_count; ++i)
	{
		for (auto j = 0; j < _slice_count; ++j)
		{
			uint16_t index[6]{};
			index[0] = _slice_count * i + j;
			index[1] = _slice_count * (i + 1) + j + 1;
			index[2] = _slice_count * (i + 1) + j;
			index[3] = _slice_count * i + j;
			index[4] = _slice_count * i + j + 1;
			index[5] = _slice_count * (i + 1) + j + 1;

			if (j == _slice_count - 1)
			{
				index[1] = _slice_count * i + j + 1;
				index[4] = _slice_count * (i - 1) + j + 1;
				index[5] = _slice_count * i + j + 1;
			}

			capsule_indices.push_back(std::move(index[0]));
			capsule_indices.push_back(std::move(index[1]));
			capsule_indices.push_back(std::move(index[2]));
			capsule_indices.push_back(std::move(index[3]));
			capsule_indices.push_back(std::move(index[4]));
			capsule_indices.push_back(std::move(index[5]));
		}
	}

	_CreateMesh(_tag, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		capsule_vertices.data(), sizeof(VertexNormalColor), static_cast<int>(capsule_vertices.size()), D3D11_USAGE_DEFAULT,
		capsule_indices.data(), sizeof(uint16_t), static_cast<int>(capsule_indices.size()), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R16_UINT
	);
}
