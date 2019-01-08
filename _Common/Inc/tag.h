#pragma once

namespace K
{
	using TAG = std::pair<std::string, uint32_t>;

	enum class TAG_STATE
	{
		NORMAL,
		DISABLED,
		DEAD
	};

	class COMMON_DLL Tag
	{
	public:
		virtual void Initialize() = 0;

		TAG const& tag() const;
		TAG_STATE tag_state() const;

		void set_tag(TAG const& _tag);
		void set_tag_state(TAG_STATE _state);

	protected:
		Tag() = default;
		Tag(Tag const& _other);
		Tag(Tag&& _other) noexcept;
		Tag& operator=(Tag const&) = default;
		Tag& operator=(Tag&&) noexcept = default;

		virtual void _Finalize() = 0;

	private:
		TAG tag_{};
		TAG_STATE tag_state_{};
	};
}
