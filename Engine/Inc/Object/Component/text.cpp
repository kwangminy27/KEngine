#include "Engine.h"
#include "text.h"

#include "device_manager.h"
#include "text_manager.h"
#include "World/world_manager.h"
#include "Object/Actor/camera_actor.h"
#include "Object/Component/transform.h"

void K::Text::Initialize()
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
		std::cout << "Text::Initialize" << std::endl;
	}
}

void K::Text::Render(float _time)
{
	auto const& device_manager = DeviceManager::singleton();
	auto const& text_manager = TextManager::singleton();

	auto const& d2d_render_target = device_manager->d2d_render_target();

	auto const& format = text_manager->FindTextFormat(format_tag_);
	auto const& brush = text_manager->FindSolidColorBrush(color_key_);

	auto caching_owner = owner();
	auto const& transform = static_cast<Transform*>(caching_owner->FindComponent(TAG{ TRANSFORM, 0 }).get());
	auto position = transform->world().Translation();

	if (false == caching_owner->ui_flag())
	{
		auto const& default_camera = WorldManager::singleton()->FindCamera({ DEFAULT_CAMERA, 0 });
		auto const& default_camera_transform = static_cast<Transform*>(default_camera->FindComponent(TAG{ TRANSFORM, 0 }).get());
		auto default_camera_position = default_camera_transform->world().Translation();

		position -= default_camera_position;
	}

	auto render_area = text_area_;

	render_area.left += position.x;
	render_area.right += position.x;
	render_area.top += static_cast<float>(RESOLUTION::HEIGHT) - position.y;
	render_area.bottom += static_cast<float>(RESOLUTION::HEIGHT) - position.y;

	d2d_render_target->BeginDraw();

	format->SetTextAlignment(text_alignment_);
	format->SetParagraphAlignment(paragraph_alignment_);

	d2d_render_target->DrawTextW(text_.c_str(), static_cast<uint32_t>(text_.length()), format.Get(), render_area, brush.Get());

	d2d_render_target->EndDraw();
}

K::CPTR K::Text::Clone() const
{
	return CPTR{ new Text{ *this }, [](Component* _p) {
		static_cast<Text*>(_p)->_Finalize();
		delete static_cast<Text*>(_p);
	} };
}

void K::Text::Serialize(InputMemoryStream& _imstream)
{
}

void K::Text::Serialize(OutputMemoryStream& _omstream)
{
}

void K::Text::set_text_alignment(DWRITE_TEXT_ALIGNMENT _alignment)
{
	text_alignment_ = _alignment;
}

void K::Text::set_paragraph_alignment(DWRITE_PARAGRAPH_ALIGNMENT _alignment)
{
	paragraph_alignment_ = _alignment;
}

void K::Text::set_text(std::wstring const& _text)
{
	text_ = _text;
}

void K::Text::set_format_tag(std::string const& _tag)
{
	format_tag_ = _tag;
}

void K::Text::set_text_area(D2D1_RECT_F const& _area)
{
	text_area_ = _area;
}

void K::Text::set_color_key(Vector4 const& _key)
{
	color_key_ = _key;
}

K::Text::Text(Text const& _other) : Component(_other)
{
	text_alignment_ = _other.text_alignment_;
	paragraph_alignment_ = _other.paragraph_alignment_;

	text_ = _other.text_;
	format_tag_ = _other.format_tag_;
	text_area_ = _other.text_area_;
	color_key_ = _other.color_key_;
}

K::Text::Text(Text&& _other) noexcept : Component(std::move(_other))
{
	text_alignment_ = std::move(_other.text_alignment_);
	paragraph_alignment_ = std::move(_other.paragraph_alignment_);

	text_ = std::move(_other.text_);
	format_tag_ = std::move(_other.format_tag_);
	text_area_ = std::move(_other.text_area_);
	color_key_ = std::move(_other.color_key_);
}

void K::Text::_Finalize()
{
}
