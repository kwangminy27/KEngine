struct VS_INPUT_POSITION
{
	float3 position : POSITION;
};

struct VS_INPUT_POSITION_COLOR
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VS_INPUT_POSITION_NORMAL_COLOR
{
	float3 position : POSITION;
	float3 normal : NORMAL;
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

struct VS_OUTPUT_POSITION_NORMAL_COLOR
{
	float4 position : SV_POSITION;
    float3 positionV : POSITION;
	float3 normalV : NORMAL;
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

struct PS_OUTPUT_GBUFFER
{
    float4 albedo : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 depth : SV_TARGET2;
    float4 material : SV_TARGET3;
};

cbuffer Transform : register(b0)
{
	matrix g_world;
	matrix g_view;
	matrix g_projection;
    matrix g_WV;
	matrix g_WVP;
}

cbuffer Material : register(b1)
{
    float4 g_material_ambient;
	float4 g_material_diffuse;
	float4 g_material_specular;
    float4 g_material_emissive;
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

cbuffer Light : register(b4)
{
    float4 g_light_ambient : packoffset(c0);
    float4 g_light_diffuse : packoffset(c1);
    float4 g_light_specular : packoffset(c2);
    float3 g_light_position : packoffset(c3);
    float3 g_light_direction : packoffset(c4);
    float3 g_light_attenuation : packoffset(c5);
    float g_light_falloff : packoffset(c5.w);
    int g_light_type : packoffset(c6);
    float g_light_range : packoffset(c6.y);
    float g_light_in_angle : packoffset(c6.z);
    float g_light_out_angle : packoffset(c6.w);
}

cbuffer Common : register(b5)
{
    float g_projection_far : packoffset(c0);
    float3 common_padding : packoffset(c0.y);
}

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

void ComputeDirectionalLight(float3 _normal, float3 _to_camera, out float4 _ambient, out float4 _diffuse, out float4 _specular)
{
    float3 to_light = normalize(mul(float4(-g_light_direction, 0.f), g_view)).xyz;
    float3 halfway = normalize(_to_camera + to_light);

    _ambient = g_material_ambient * g_light_ambient;
    _diffuse = g_material_diffuse * g_light_diffuse * max(dot(_normal, to_light), 0.f);
    _specular = g_material_specular * g_light_specular * pow(max(dot(_normal, halfway), 0), g_light_specular.w);
}

void ComputePointLight(float3 _position, float3 _normal, float3 _to_camera, out float4 _ambient, out float4 _diffuse, out float4 _specular)
{
    float3 light_position = mul(float4(g_light_position, 1.f), g_view).xyz;

    float distance = length(light_position - _position);
    
    if(g_light_range < distance)
        return;

    float3 to_light = normalize(light_position - _position);
    float3 halfway = normalize(_to_camera + to_light);

    float attenuation = 1.f / dot(g_light_attenuation, float3(1, distance, distance * distance));

    _ambient = g_material_ambient * g_light_ambient * attenuation;
    _diffuse = g_material_diffuse * g_light_diffuse * max(dot(_normal, to_light), 0.f) * attenuation;
    _specular = g_material_specular * g_light_specular * pow(max(dot(_normal, halfway), 0.f), g_light_specular.w) * attenuation;
}

void ComputeSpotLight(float3 _position, float3 _normal, float3 _to_camera, out float4 _ambient, out float4 _diffuse, out float4 _specular)
{
    float3 light_position = mul(float4(g_light_position, 1.f), g_view).xyz;
    float3 light_direction = normalize(mul(float4(g_light_direction, 0.f), g_view)).xyz;

    float distance = length(light_position - _position);
    
    if (g_light_range < distance)
        return;

    float3 to_light = normalize(light_position - _position);
    float3 halfway = normalize(_to_camera + to_light);

    float attenuation = 1.f / dot(g_light_attenuation, float3(1, distance, distance * distance));
    float falloff = pow(max(dot(-to_light, light_direction), 0.f), g_light_falloff);

    _ambient = g_material_ambient * g_light_ambient * attenuation * falloff;
    _diffuse = g_material_diffuse * g_light_diffuse * max(dot(_normal, to_light), 0.f) * attenuation * falloff;
    _specular = g_material_specular * g_light_specular * pow(max(dot(_normal, halfway), 0.f), g_light_specular.w) * falloff;
}

float CompressColor(float4 _color)
{
    uint4 color = (uint4)0;
    color.r = uint(_color.r * 255.f);
    color.g = uint(_color.g * 255.f);
    color.b = uint(_color.b * 255.f);
    color.a = uint(_color.a * 255.f);

    uint result = (uint)0;
    result |= color.a << 24;
    result |= color.b << 16;
    result |= color.g << 8;
    result |= color.r;

    return asfloat(result);
}

float4 DecompressColor(float _color)
{
    float4 result = (float4)0;

    uint color = asuint(_color);
    result.r = (color & 0x000000FF) / 255.f;
    result.g = (color & 0x0000FF00) / 255.f;
    result.b = (color & 0x00FF0000) / 255.f;
    result.a = (color & 0xFF000000) / 255.f;

    return result;
}