#include "Engine.h"
#include "renderer.h"

#include "Resource/mesh.h"
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
