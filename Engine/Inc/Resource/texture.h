#pragma once

namespace K
{
	class ENGINE_DLL Texture
	{
		friend class ResourceManager;
	public:
		void SetToShader(int _slot);

	private:
		Texture() = default;
		Texture(Texture const& _other);
		Texture(Texture&& _other) noexcept;
		Texture& operator=(Texture const&) = default;
		Texture& operator=(Texture&&) noexcept = default;

		void _CreateTexture2D(std::wstring const& _file_name, std::string const& _path_tag);

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture_{};
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV_{};
	};
}
