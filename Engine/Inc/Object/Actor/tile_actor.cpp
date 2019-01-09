#include "Engine.h"
#include "tile_actor.h"

#include "Object/object_manager.h"
#include "Object/Component/transform.h"
#include "Object/Component/material.h"

void K::TileActor::Initialize()
{
	try
	{
		auto transform = ObjectManager::singleton()->CreateComponent<Transform>(TAG{ TRANSFORM, 0 });
		AddComponent(transform);

		auto material = ObjectManager::singleton()->CreateComponent<Material>(TAG{ MATERIAL, 0 });

		CPTR_CAST<Material>(material)->SetSampler(LINEAR_SAMPLER, 0, 0, 0);

		MaterialConstantBuffer Material_CB{};
		Material_CB.diffuse = DirectX::Colors::White.v;
		CPTR_CAST<Material>(material)->SetMaterialConstantBuffer(Material_CB, 0, 0);

		AddComponent(transform);

		set_render_group_type(RENDER_GROUP_TYPE::LANDSCAPE);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "TileActor::Initialize" << std::endl;
	}
}

K::APTR K::TileActor::Clone() const
{
	return APTR{ new TileActor{ *this }, [](TileActor* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::TileActor::Serialize(InputMemoryStream& _imstream)
{
	_imstream.Serialize(LT_.x);
	_imstream.Serialize(LT_.y);
	_imstream.Serialize(RB_.x);
	_imstream.Serialize(RB_.y);

	_imstream.Serialize(option_);

	CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }))->Serialize(_imstream);
}

void K::TileActor::Serialize(OutputMemoryStream& _omstream)
{
	_omstream.Serialize(LT_.x);
	_omstream.Serialize(LT_.y);
	_omstream.Serialize(RB_.x);
	_omstream.Serialize(RB_.y);

	_omstream.Serialize(option_);

	CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }))->Serialize(_omstream);
}

K::Vector2 const& K::TileActor::LT() const
{
	return LT_;
}

K::Vector2 const& K::TileActor::RB() const
{
	return RB_;
}

K::TILE_OPTION K::TileActor::option() const
{
	return option_;
}

void K::TileActor::set_LT(Vector2 const& _LT)
{
	LT_ = _LT;
}

void K::TileActor::set_RB(Vector2 const& _RB)
{
	RB_ = _RB;
}

void K::TileActor::set_option(TILE_OPTION _option)
{
	option_ = _option;
}

K::TileActor::TileActor(TileActor const& _other) : Actor(_other)
{
	LT_ = _other.LT_;
	RB_ = _other.RB_;
	option_ = _other.option_;
}

K::TileActor::TileActor(TileActor&& _other) noexcept : Actor(std::move(_other))
{
	LT_ = std::move(_other.LT_);
	RB_ = std::move(_other.RB_);
	option_ = std::move(_other.option_);
}

void K::TileActor::_Finalize()
{
}
