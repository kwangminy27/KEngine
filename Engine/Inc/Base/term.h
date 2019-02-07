#pragma once

namespace K
{
	class Actor;
	class Component;

	using APTR = std::shared_ptr<Actor>;
	using CPTR = std::unique_ptr<Component, std::function<void(Component*)>>;

	template <typename T> constexpr auto APTR_CAST(APTR const& _p) { return std::static_pointer_cast<T>(_p); }
	template <typename T> constexpr auto CPTR_CAST(CPTR const& _p) { return static_cast<T*>(_p.get()); }

	//////////////////// Path Key ////////////////////
	constexpr auto ROOT_PATH = "RootPath";
	constexpr auto VIDEO_PATH = "VideoPath";
	constexpr auto AUDIO_PATH = "AudioPath";
	constexpr auto TEXTURE_PATH = "TexturePath";
	constexpr auto SHADER_PATH = "ShaderPath";
	constexpr auto FBX_PATH = "FBXPath";

	//////////////////// Mesh Key ////////////////////
	constexpr auto TEX_RECT = "TexRect";
	constexpr auto COLLIDER_RECT = "ColliderRect";
	constexpr auto COLLIDER_CIRCLE = "ColliderCircle";
	constexpr auto INSTANCE_TEX_RECT = "InstanceTexRect";
	constexpr auto NORMAL_PYRAMID = "NormalPyramid";
	constexpr auto FULL_SCREEN_RECT = "FullScreenRect";
	constexpr auto SPHERE_MESH = "SphereMesh";
	constexpr auto CYLINDER_MESH = "CylinderMesh";
	constexpr auto CAPSULE_MESH = "CapsuleMesh";
	constexpr auto SPOTLIGHT_MESH = "SpotlightMesh";
	constexpr auto SPHERE_VOLUME = "SphereVolume";
	constexpr auto CYLINDER_VOLUME = "CylinderVolume";
	constexpr auto CAPSULE_VOLUME = "CapsuleVolume";
	constexpr auto SPOTLIGHT_VOLUME = "SpotlightVolume";

	//////////////////// Sampler Key ////////////////////
	constexpr auto LINEAR_SAMPLER = "LinearSampler";
	constexpr auto POINT_SAMPLER = "PointSampler";

	//////////////////// Shader Key ////////////////////
	constexpr auto BASIC_SHADER = "BasicShader";
	constexpr auto BASIC_TEX_SHADER = "BasicTexShader";
	constexpr auto BASIC_ANIMATION_2D_SHADER = "BasicAnimation2DShader";
	constexpr auto BASIC_NORMAL_SHADER = "BasicNormalShader";
	constexpr auto COLLIDER_SHADER = "ColliderShader";
	constexpr auto INSTANCE_TEX_SHADER = "InstanceTexShader";
	constexpr auto GBUFFER_SHADER = "GBufferShader";
	constexpr auto DEFERRED_LIGHTING_SHADER = "DeferredLightingShader";
	constexpr auto DEFERRED_LIGHTING_CALCULATE_COLOR = "DeferredLightingCalculateColor";
	constexpr auto BUMP_MAPPING_SHADER = "BumpMappingShader";

	//////////////////// RenderState Key ////////////////////
	constexpr auto RS_LIGHT_VOLUME_PASS_1 = "RSLightVolumePass1";
	constexpr auto RS_LIGHT_VOLUME_PASS_2 = "RSLightVolumePass2";
	constexpr auto RS_WIREFRAME_CULL_FRONT = "RSWireFrameCullFront";
	constexpr auto RS_WIREFRAME_CULL_BACK = "RSWireFrameCullBack";
	constexpr auto RS_WIREFRAME_CULL_NONE = "RSWireFrameCullNone";
	constexpr auto ALPHA_BLEND = "AlphaBlend";
	constexpr auto LIGHT_BLEND = "LightBlend";
	constexpr auto DEPTH_DISABLE = "DepthDisable";
	constexpr auto DSS_LIGHT_VOLUME_PASS_1 = "DSSLightVolumePass1";
	constexpr auto DSS_LIGHT_VOLUME_PASS_2 = "DSSLightVolumePass2";

	//////////////////// RenderTarget Key ////////////////////
	constexpr auto BASIC_RENDER_TARGET = "BasicRenderTarget";
	constexpr auto ALBEDO_RENDER_TARGET = "AlbedoRenderTarget";
	constexpr auto NORMAL_RENDER_TARGET = "NormalRenderTarget";
	constexpr auto DEPTH_RENDER_TARGET = "DepthRenderTarget";
	constexpr auto MATERIAL_RENDER_TARGET = "MaterialRenderTarget";
	constexpr auto LIGHT_AMBIENT_RENDER_TARGET = "LightAmbientRenderTarget";
	constexpr auto LIGHT_DIFFUSE_RENDER_TARGET = "LightDiffuseRenderTarget";
	constexpr auto LIGHT_SPECULAR_RENDER_TARGET = "LightSpecularRenderTarget";
	constexpr auto DEFERRED_LIGHTING_RENDER_TARGET = "DeferredLightingRenderTarget";
	constexpr auto GBUFFER_MRT = "GBufferMRT";
	constexpr auto LIGHT_MRT = "LightMRT";

	//////////////////// Common Key ////////////////////
	constexpr auto COMMON = "Common";
	constexpr auto BACKGROUND = "Background";
	constexpr auto DEFAULT = "Default";
	constexpr auto UI = "UI";

	//////////////////// Actor Key ////////////////////
	constexpr auto DEFAULT_CAMERA = "DefaultCamera";
	constexpr auto UI_CAMERA = "UICamera";
	constexpr auto TILE = "Tile";
	constexpr auto TILE_MAP = "TileMap";
	constexpr auto DEFAULT_LIGHT = "DefaultLight";
	constexpr auto GLOBAL_AMBIENT_LIGHT = "GlobalAmbientLight";

	//////////////////// Component Key ////////////////////
	constexpr auto TRANSFORM = "Transform";
	constexpr auto CAMERA = "Camera";
	constexpr auto TEXT = "Text";
	constexpr auto MATERIAL = "Material";
	constexpr auto RENDERER = "Renderer";
	constexpr auto ANIMATION_2D = "Animation2D";
	constexpr auto COLLIDER = "Collider";
	constexpr auto NAVIGATOR = "Navigator";
	constexpr auto LIGHT = "Light";

	//////////////////// Constant ////////////////////
	const auto MTU_SIZE = 1024;
	const auto MAX_USER_SIZE = 100;
	const auto MAX_MESSAGE_SIZE = 128;
	const auto MAX_KEY_SIZE = 256;

	//////////////////// Protocol ////////////////////
	const auto CS_LOGIN = 1;
	const auto CS_CHAT = 2;

	const auto SC_LOGIN = 1;
	const auto SC_CHAT = 2;

#pragma pack(push, 1)
	struct CS_PACKET_LOGIN
	{
		uint32_t size;
		uint8_t type;
	};

	struct CS_PACKET_CHAT
	{
		uint32_t size;
		uint8_t type;
		wchar_t message[MAX_MESSAGE_SIZE];
	};

	struct SC_PACKET_LOGIN
	{
		uint32_t size;
		uint8_t type;
		uint8_t id;
	};

	struct SC_PACKET_CHAT
	{
		uint32_t size;
		uint8_t type;
		uint8_t id;
		wchar_t message[MAX_MESSAGE_SIZE];
	};
#pragma pack(pop)

	struct VertexColor
	{
		Vector3 position;
		Vector4 color;
	};

	struct VertexNormalColor
	{
		Vector3 position;
		Vector3 normal;
		Vector4 color;
	};

	struct VertexTex
	{
		Vector3 position;
		Vector2 uv;
	};

	struct Vertex3D
	{
		Vector3 position;
		Vector2 uv;
		Vector3 normal;
		Vector3 binormal;
		Vector3 tangent;
		Vector4 joint_weights;
		Vector4 joint_indices;
	};

	struct MatrixTex
	{
		Vector2 LT;
		Vector2 RB;
		Matrix world;
	};

	struct CSO_DESC
	{
		SHADER_TYPE type;
		std::wstring file_name;
	};

	struct ANIMATION_2D_FRAME_DESC
	{
		Vector2 LT;
		Vector2 RB;
	};

	struct ANIMATION_2D_CLIP_DESC
	{
		std::string clip_tag;
		std::string texture_tag;
		ANIMATION_2D_TYPE type;
		ANIMATION_OPTION option;
		float width;
		float height;
		float completion_time;
		std::vector<ANIMATION_2D_FRAME_DESC> frame_vector;
	};

	struct TransformConstantBuffer
	{
		Matrix world;
		Matrix view;
		Matrix projection;
		Matrix WV;
		Matrix WVP;
		Matrix projection_Inv;
	};

	struct MaterialConstantBuffer
	{
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
		Vector4 emissive;
	};

	struct LightConstantBuffer
	{
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
		Vector3 position;
		int type;
		Vector3 direction;
		float range;
		Vector3 attenuation;
		float falloff;
	};

	struct CommonConstantBuffer
	{
		Vector2 viewport;
		float time;
		float padding;
	};
}
