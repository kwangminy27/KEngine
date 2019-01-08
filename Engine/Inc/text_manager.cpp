#include "Engine.h"
#include "text_manager.h"

#include "device_manager.h"

Microsoft::WRL::ComPtr<IDWriteTextFormat> K::TextManager::text_format_dummy_{};
Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> K::TextManager::solid_color_brush_dummy_{};

void K::TextManager::Initialize()
{
	try
	{
		ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &dwrite_factory_));

		_CreateTextFormat(
			"D2Coding",
			L"D2Coding",
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20.f,
			L"ko-KR"
		);

		_CreateSolidColorBrush(DirectX::Colors::Red.v);
		_CreateSolidColorBrush(DirectX::Colors::Green.v);
		_CreateSolidColorBrush(DirectX::Colors::Blue.v);
		_CreateSolidColorBrush(DirectX::Colors::Black.v);
		_CreateSolidColorBrush(DirectX::Colors::White.v);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "TextManager::Initialize" << std::endl;
	}
}

Microsoft::WRL::ComPtr<IDWriteTextFormat> const& K::TextManager::FindTextFormat(std::string const& _tag) const
{
	auto iter = text_format_map_.find(_tag);

	if (iter == text_format_map_.end())
		return text_format_dummy_;

	return iter->second;
}

Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> const& K::TextManager::FindSolidColorBrush(Vector4 const& _color) const
{
	auto iter = solid_color_brush_map_.find(_CreateColorKey(_color));

	if (iter == solid_color_brush_map_.end())
		return solid_color_brush_dummy_;

	return iter->second;
}

void K::TextManager::_Finalize()
{
}

void K::TextManager::_CreateTextFormat(
	std::string const& _tag,
	std::wstring const& _font_family_name,
	DWRITE_FONT_WEIGHT _weight,
	DWRITE_FONT_STYLE _style,
	DWRITE_FONT_STRETCH _stretch,
	float _size,
	std::wstring const& _locale_name)
{
	if (FindTextFormat(_tag))
		throw std::exception{ "TextManager::_CreateTextFormat" };

	Microsoft::WRL::ComPtr<IDWriteTextFormat> text_format{};
	ThrowIfFailed(dwrite_factory_->CreateTextFormat(
		_font_family_name.c_str(),
		nullptr,
		_weight,
		_style,
		_stretch,
		_size,
		_locale_name.c_str(),
		&text_format
	));

	text_format_map_.insert(std::make_pair(_tag, std::move(text_format)));
}

void K::TextManager::_CreateSolidColorBrush(Vector4 const& _color)
{
	auto const& d2d_render_target = DeviceManager::singleton()->d2d_render_target();

	if (FindSolidColorBrush(_color))
		throw std::exception{ "TextManager::_CreateSolidColorBrush" };

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> solid_color_brush{};
	ThrowIfFailed(d2d_render_target->CreateSolidColorBrush(D2D1::ColorF{ _color.x, _color.y, _color.z, _color.w }, &solid_color_brush));

	solid_color_brush_map_.insert(std::make_pair(_CreateColorKey(_color), std::move(solid_color_brush)));
}

uint32_t K::TextManager::_CreateColorKey(Vector4 const& _color) const
{
	auto color = _color;
	color *= 255.f;

	uint32_t key{};
	key |= static_cast<uint8_t>(color.x);
	key |= static_cast<uint8_t>(color.y) << 8;
	key |= static_cast<uint8_t>(color.z) << 16;
	key |= static_cast<uint8_t>(color.w) << 24;

	return key;
}
