#pragma once

namespace K
{
	class ENGINE_DLL Shader
	{
		friend class RenderingManager;
	public:
		void SetToShader();

	private:
		Shader() = default;
		Shader(Shader const& _other);
		Shader(Shader&& _other) noexcept;
		Shader& operator=(Shader const&) = default;
		Shader& operator=(Shader&&) noexcept = default;

		void _CreateShader(
			std::vector<CSO_DESC> const& _cso_desc_vector,
			std::vector<D3D11_INPUT_ELEMENT_DESC> const& _input_element_desc_vector,
			std::string const& _path_tag);
		void _CreateVertexShader(
			std::wstring const& _file_name,
			std::vector<D3D11_INPUT_ELEMENT_DESC> const& _input_element_desc_vector,
			std::string const& _path_tag);
		void _CreatePixelShader(std::wstring const& _file_name, std::string const& _path_tag);

		Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout_{};
		Microsoft::WRL::ComPtr<ID3D11VertexShader> VS_{};
		Microsoft::WRL::ComPtr<ID3D11PixelShader> PS_{};
	};
}
