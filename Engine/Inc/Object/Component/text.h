#pragma once

#include "component.h"

namespace K
{
	class ENGINE_DLL Text final : public Component
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual void Render(float _time);

		virtual CPTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		void set_text_alignment(DWRITE_TEXT_ALIGNMENT _alignment);
		void set_paragraph_alignment(DWRITE_PARAGRAPH_ALIGNMENT _alignment);

		void set_text(std::wstring const& _text);
		void set_format_tag(std::string const& _tag);
		void set_text_area(D2D1_RECT_F const& _area);
		void set_color_key(Vector4 const& _key);

	private:
		Text() = default;
		Text(Text const& _other);
		Text(Text&& _other) noexcept;
		Text& operator=(Text const&) = delete;
		Text& operator=(Text&&) noexcept = default;

		virtual void _Finalize() override;

		DWRITE_TEXT_ALIGNMENT text_alignment_{};
		DWRITE_PARAGRAPH_ALIGNMENT paragraph_alignment_{};

		std::wstring text_{};
		std::string format_tag_{};
		D2D1_RECT_F text_area_{};
		Vector4 color_key_{};
	};
}
