#include "Engine.h"
#include "renderer.h"

#include "Resource/resource_manager.h"
#include "Resource/mesh.h"
#include "Rendering/rendering_manager.h"
#include "Rendering/shader.h"
#include "Rendering/render_state.h"
#include "Object/Actor/actor.h"
#include "Object/Component/transform.h"
#include "Object/Component/material.h"
#include "Object/Component/animation_2d.h"

void K::Renderer::Initialize()
{
	try
	{
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Renderer::Initialize" << std::endl;
	}
}

void K::Renderer::Render(float _time)
{
	auto const& material = owner()->FindComponent(TAG{ MATERIAL, 0 });

	UpdateConstantBuffer(_time);

	for (auto const& render_state : render_state_vector_)
		render_state->SetState();

	shader_->SetToShader();

	for (auto i = 0; i < mesh_->GetContainerSize(); ++i)
	{
		for (auto j = 0; j < mesh_->GetSubsetSize(i); ++j)
		{
			CPTR_CAST<Material>(material)->SetToShader(i, j);

			mesh_->Render(i, j);
		}
	}

	for (auto const& render_state : render_state_vector_)
		render_state->ResetState();
}

K::CPTR K::Renderer::Clone() const
{
	return CPTR{ new Renderer{ *this }, [](Component* _p) {
		static_cast<Renderer*>(_p)->_Finalize();
		delete static_cast<Renderer*>(_p);
	} };
}

void K::Renderer::Serialize(InputMemoryStream& _imstream)
{
}

void K::Renderer::Serialize(OutputMemoryStream& _omstream)
{
}

void K::Renderer::UpdateConstantBuffer(float _time)
{
	if(auto const& transform = owner()->FindComponent(TAG{ TRANSFORM, 0 }))
		CPTR_CAST<Transform>(transform)->UpdateConstantBuffer();

	if (auto const& animation_2d = owner()->FindComponent(TAG{ ANIMATION_2D, 0 }))
		CPTR_CAST<Animation2D>(animation_2d)->UpdateConstantBuffer(_time);
}

void K::Renderer::set_mesh(std::shared_ptr<Mesh> const& _mesh)
{
	mesh_ = _mesh;

	auto const& mesh_container_vector = mesh_->mesh_container_vector();
	auto const& materials = ResourceManager::singleton()->FindMaterials(mesh_->name());

	auto const& material_component = CPTR_CAST<Material>(owner()->FindComponent(TAG{ MATERIAL, 0 }));

	for (size_t i = 0; i < mesh_container_vector.size(); ++i)
	{
		for (size_t j = 0; j < materials.size(); ++j)
		{
			auto const& material = materials.at(j);

			if (mesh_container_vector.at(i)->material_name == material->name)
			{
				MaterialConstantBuffer material_CB{};
				material_CB.ambient = material->ambient;
				material_CB.diffuse = material->diffuse;
				material_CB.specular = material->specular;
				material_CB.emissive = material->emissive;
				material_CB.specular_exp = material->specular_exp;

				material_component->SetMaterialConstantBuffer(material_CB, static_cast<int>(i), 0);

				if (material->diffuse_maps.empty() == false)
					material_component->SetTexture(material->diffuse_maps.at(0), 0, static_cast<int>(i), 0);
				else
					material_component->SetTexture(mesh_->name() + "_diffuse.dds", 0, static_cast<int>(i), 0);

				if (material->specular_maps.empty() == false)
					material_component->SetTexture(material->specular_maps.at(0), 1, static_cast<int>(i), 0);
				else
					material_component->SetTexture(mesh_->name() + "_specular.dds", 1, static_cast<int>(i), 0);

				if (material->normal_maps.empty() == false)
					material_component->SetTexture(material->normal_maps.at(0), 2, static_cast<int>(i), 0);
				else
					material_component->SetTexture(mesh_->name() + "_normal.dds", 2, static_cast<int>(i), 0);
			}
		}

		material_component->SetSampler(LINEAR_SAMPLER, 0, static_cast<int>(i), 0);
	}
}

void K::Renderer::set_shader(std::shared_ptr<Shader> const& _shader)
{
	shader_ = _shader;
}

void K::Renderer::set_render_state(std::shared_ptr<RenderState> const& _render_state)
{
	render_state_vector_.push_back(_render_state);
}

K::Renderer::Renderer(Renderer const& _other) : Component(_other)
{
	mesh_ = _other.mesh_;
	shader_ = _other.shader_;
	render_state_vector_ = _other.render_state_vector_;
}

K::Renderer::Renderer(Renderer&& _other) noexcept : Component(std::move(_other))
{
	mesh_ = std::move(_other.mesh_);
	shader_ = std::move(_other.shader_);
	render_state_vector_ = std::move(_other.render_state_vector_);
}

void K::Renderer::_Finalize()
{
}
