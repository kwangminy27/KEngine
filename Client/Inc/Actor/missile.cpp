#include "Client.h"
#include "missile.h"

void K::Missile::Initialize()
{
	try
	{
		auto const& object_manager = ObjectManager::singleton();
		auto const& resource_manager = ResourceManager::singleton();
		auto const& rendering_manager = RenderingManager::singleton();

		auto transform = object_manager->CreateComponent<Transform>(TAG{ TRANSFORM, 0 });
		CPTR_CAST<Transform>(transform)->set_local_scaling(Vector3::One);
		CPTR_CAST<Transform>(transform)->set_local_translation(Vector3{ 0.0f, 0.f, 1.f });
		AddComponent(transform);

		auto renderer = object_manager->CreateComponent<Renderer>(TAG{ RENDERER, 0 });
		CPTR_CAST<Renderer>(renderer)->set_mesh(resource_manager->FindMesh(SPHERE_MESH));
		CPTR_CAST<Renderer>(renderer)->set_shader(rendering_manager->FindShader(GBUFFER_SHADER));
		AddComponent(renderer);

		auto material = object_manager->CreateComponent<Material>(TAG{ MATERIAL, 0 });
		MaterialConstantBuffer material_CB{};
		material_CB.ambient = DirectX::Colors::White.v;
		material_CB.diffuse = DirectX::Colors::White.v;
		material_CB.specular = DirectX::Colors::White.v;
		material_CB.emissive = DirectX::Colors::White.v;
		CPTR_CAST<Material>(material)->SetMaterialConstantBuffer(material_CB, 0, 0);
		AddComponent(material);

		set_render_group_type(RENDER_GROUP_TYPE::NORMAL);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Missile::Initialize" << std::endl;
	}
}

K::APTR K::Missile::Clone() const
{
	return APTR{ new Missile, [](Missile* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::Missile::Serialize(InputMemoryStream& _imstream)
{
}

void K::Missile::Serialize(OutputMemoryStream& _omstream)
{
}

K::Missile::Missile(Missile const& _other) : ActorClient(_other)
{
}

K::Missile::Missile(Missile&& _other) noexcept : ActorClient(std::move(_other))
{
}

void K::Missile::_Finalize()
{
}

void K::Missile::_Update(float _time)
{
}
