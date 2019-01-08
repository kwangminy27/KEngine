#pragma once

namespace K
{
	class ENGINE_DLL TextManager final : public Singleton<TextManager>
	{
		friend class Singleton<TextManager>;
	public:
		virtual void Initialize() override;

		Microsoft::WRL::ComPtr<IDWriteTextFormat> const& FindTextFormat(std::string const& _tag) const;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> const& FindSolidColorBrush(Vector4 const& _color) const;

		static Microsoft::WRL::ComPtr<IDWriteTextFormat> text_format_dummy_;
		static Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> solid_color_brush_dummy_;

	private:
		TextManager() = default;
		TextManager(TextManager const&) = delete;
		TextManager(TextManager&&) noexcept = delete;
		TextManager& operator=(TextManager const&) = delete;
		TextManager& operator=(TextManager&&) noexcept = delete;

		virtual void _Finalize() override;

		void _CreateTextFormat(
			std::string const& _tag,
			std::wstring const& _font_family_name,
			DWRITE_FONT_WEIGHT _weight,
			DWRITE_FONT_STYLE _style,
			DWRITE_FONT_STRETCH _stretch,
			float _size,
			std::wstring const& _locale_name);
		void _CreateSolidColorBrush(Vector4 const& _color);
		uint32_t _CreateColorKey(Vector4 const& _color) const;

		Microsoft::WRL::ComPtr<IDWriteFactory> dwrite_factory_{};
		std::unordered_map<std::string, Microsoft::WRL::ComPtr<IDWriteTextFormat>> text_format_map_{};
		std::unordered_map<uint32_t, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>> solid_color_brush_map_{};
	};
}
