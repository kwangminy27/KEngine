#pragma once

#include "component.h"

namespace K
{
	class Mesh;
	class Shader;
	class RenderState;

	class ENGINE_DLL Renderer final : public Component
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual void Render(float _time);

		virtual CPTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		void UpdateConstantBuffer(float _time);

		void set_mesh(std::shared_ptr<Mesh> const& _mesh);
		void set_shader(std::shared_ptr<Shader> const& _shader);
		void set_render_state(std::shared_ptr<RenderState> const& _render_state);

	private:
		Renderer() = default;
		Renderer(Renderer const& _other);
		Renderer(Renderer&& _other) noexcept;
		Renderer& operator=(Renderer const&) = delete;
		Renderer& operator=(Renderer&&) noexcept = default;

		virtual void _Finalize() override;

		std::shared_ptr<Mesh> mesh_{};
		std::shared_ptr<Shader> shader_{};
		std::vector<std::shared_ptr<RenderState>> render_state_vector_{};
	};
}
