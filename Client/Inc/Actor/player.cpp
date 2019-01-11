#include "Client.h"
#include "player.h"

void K::Player::Initialize()
{
	try
	{
		auto const& object_manager = ObjectManager::singleton();
		auto const& resource_manager = ResourceManager::singleton();
		auto const& rendering_manager = RenderingManager::singleton();

		auto transform = object_manager->CreateComponent<Transform>(TAG{ TRANSFORM, 0 });
		CPTR_CAST<Transform>(transform)->set_local_scaling(Vector3::One);
		AddComponent(transform);

		auto renderer = object_manager->CreateComponent<Renderer>(TAG{ RENDERER, 0 });
		CPTR_CAST<Renderer>(renderer)->set_mesh(resource_manager->FindMesh(NORMAL_PYRAMID));
		CPTR_CAST<Renderer>(renderer)->set_shader(rendering_manager->FindShader(BASIC_NORMAL_SHADER));
		CPTR_CAST<Renderer>(renderer)->set_render_state(rendering_manager->FindRenderState(DEPTH_DISABLE));
		AddComponent(renderer);

		auto material = object_manager->CreateComponent<Material>(TAG{ MATERIAL, 0 });
		MaterialConstantBuffer material_CB{};
		material_CB.ambient = DirectX::Colors::White.v;
		material_CB.diffuse = DirectX::Colors::White.v;
		material_CB.specular = DirectX::Colors::White.v;
		CPTR_CAST<Material>(material)->SetMaterialConstantBuffer(material_CB , 0, 0);
		AddComponent(material);

		set_render_group_type(RENDER_GROUP_TYPE::NORMAL);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Player::Initialize" << std::endl;
	}
}

K::APTR K::Player::Clone() const
{
	return APTR{ new Player, [](Player* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::Player::Serialize(InputMemoryStream& _imstream)
{
}

void K::Player::Serialize(OutputMemoryStream& _omstream)
{
}

K::Player::Player(Player const& _other) : ActorClient(_other)
{
}

K::Player::Player(Player&& _other) noexcept : ActorClient(std::move(_other))
{
}

void K::Player::_Finalize()
{
}

void K::Player::_Update(float _time)
{
	auto const& camera = WorldManager::singleton()->FindCamera(TAG{ DEFAULT_CAMERA, 0 });
	camera->CreateView(Vector3{ 0.f, 0.f, -3.f }, Vector3::Zero, Vector3::UnitY);
}
