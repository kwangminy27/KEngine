#include "Common.h"
#include "tag.h"

K::TAG const& K::Tag::tag() const
{
	return tag_;
}

K::TAG_STATE K::Tag::tag_state() const
{
	return tag_state_;
}

void K::Tag::set_tag(TAG const& _tag)
{
	tag_ = _tag;
}

void K::Tag::set_tag_state(TAG_STATE _state)
{
	tag_state_ = _state;
}

K::Tag::Tag(Tag const& _other)
{
	tag_ = _other.tag_;
	tag_state_ = _other.tag_state_;
}

K::Tag::Tag(Tag&& _other) noexcept
{
	tag_ = std::move(_other.tag_);
	tag_state_ = std::move(_other.tag_state_);
}
