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
		CPTR_CAST<Renderer>(renderer)->set_mesh(resource_manager->FindMesh(SPOTLIGHT_VOLUME));
		CPTR_CAST<Renderer>(renderer)->set_shader(rendering_manager->FindShader(GBUFFER_SHADER));
		AddComponent(renderer);

		auto material = object_manager->CreateComponent<Material>(TAG{ MATERIAL, 0 });
		MaterialConstantBuffer material_CB{};
		material_CB.ambient = DirectX::Colors::White.v;
		material_CB.diffuse = DirectX::Colors::White.v;
		material_CB.specular = DirectX::Colors::White.v;
		material_CB.emissive = DirectX::Colors::White.v;
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
}
