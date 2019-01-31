#pragma once

namespace K
{
	class ENGINE_DLL RenderTarget
	{
		friend class RenderingManager;
		friend class MRT;
	public:
		void Render(float _time);
		void RenderFullScreen(float _time);

		void Clear();

		void SetTarget();
		void ResetTarget();

		void Attach(int _slot);
		void Detach(int _slot);

		void set_mesh_tag(std::string const& _tag);
		void set_shader_tag(std::string const& _tag);
		void set_sampler_tag(std::string const& _tag);

	private:
		RenderTarget() = default;
		RenderTarget(RenderTarget const& _other);
		RenderTarget(RenderTarget&& _other) noexcept;
		RenderTarget& operator=(RenderTarget const&) = default;
		RenderTarget& operator=(RenderTarget&&) noexcept = default;

		void _CreateRenderTarget(Vector3 const& _scaling, Vector3 const& _translation, DXGI_FORMAT _format);

		Vector3 scaling_{};
		Vector3 translation_{};
		std::string mesh_tag_{};
		std::string shader_tag_{};
		std::string sampler_tag_{};
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RTV_{};
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DSV_{};
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV_{};
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> old_RTV_{};
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> old_DSV_{};
	};
}
