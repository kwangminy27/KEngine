struct VS_INPUT_POSITION
{
	float3 position : POSITION;
};

struct VS_INPUT_POSITION_COLOR
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VS_INPUT_POSITION_TEX
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_INPUT_POSITION_TEX_INSTANCE
{
	float3 position : POSITION;
	float2 LT : TEXCOORD;
	float2 RB : TEXCOORD1;
	float4x4 world : WORLD;
};

struct VS_OUTPUT_POSITION
{
	float4 position : SV_POSITION;
};

struct VS_OUTPUT_POSITION_COLOR
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT_POSITION_TEX
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

struct PS_OUTPUT_SINGLE_TARGET
{
	float4 target : SV_TARGET;
};

cbuffer Transform : register(b0)
{
	matrix g_world;
	matrix g_view;
	matrix g_projection;
	matrix g_WVP;
}

cbuffer Material : register(b1)
{
	float4 g_diffuse;
}

cbuffer Animation2D : register(b2)
{
	float2 g_LT;
	float2 g_RB;
}

cbuffer Collider : register(b3)
{
	float4 g_color;
}

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);