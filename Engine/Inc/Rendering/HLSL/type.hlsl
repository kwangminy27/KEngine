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

cbuffer Transform : register(b0)
{
	matrix g_world;
	matrix g_view;
	matrix g_projection;
    matrix g_WV;
	matrix g_WVP;
}

struct MATERIAL
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
};

cbuffer Material : register(b1)
{
    MATERIAL g_material;
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

struct LIGHT
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 position;
    float padding1;
    float3 direction;
    float padding2;
    float3 attenuation;
    float falloff;
    int type;
    float range;
    float in_angle;
    float out_angle;
};

cbuffer Light : register(b4)
{
    LIGHT g_light;
}

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

void ComputeDirectionalLight(float3 _normal, float3 _to_camera, out float4 _ambient, out float4 _diffuse, out float4 _specular)
{
    float3 to_light = normalize(mul(float4(-g_light.direction, 0.f), g_view)).xyz;
    float3 halfway = normalize(_to_camera + to_light);

    _ambient = g_material.ambient * g_light.ambient;
    _diffuse = g_material.diffuse * g_light.diffuse * max(dot(_normal, to_light), 0.f);
    _specular = g_material.specular * g_light.specular * pow(max(dot(_normal, halfway), 0), g_light.specular.w);
}

void ComputePointLight(float3 _position, float3 _normal, float3 _to_camera, out float4 _ambient, out float4 _diffuse, out float4 _specular)
{
    float3 light_position = mul(float4(g_light.position, 1.f), g_view).xyz;

    float distance = length(light_position - _position);
    
    if(g_light.range < distance)
        return;

    float3 to_light = normalize(light_position - _position);
    float3 halfway = normalize(_to_camera + to_light);

    float attenuation = 1.f / dot(g_light.attenuation, float3(1, distance, distance * distance));

    _ambient = g_material.ambient * g_light.ambient * attenuation;
    _diffuse = g_material.diffuse * g_light.diffuse * max(dot(_normal, to_light), 0.f) * attenuation;
    _specular = g_material.specular * g_light.specular * pow(max(dot(_normal, halfway), 0.f), g_light.specular.w) * attenuation;
}

void ComputeSpotLight(float3 _position, float3 _normal, float3 _to_camera, out float4 _ambient, out float4 _diffuse, out float4 _specular)
{
    float3 light_position = mul(float4(g_light.position, 1.f), g_view).xyz;
    float3 light_direction = normalize(mul(float4(g_light.direction, 0.f), g_view)).xyz;

    float distance = length(light_position - _position);
    
    if (g_light.range < distance)
        return;

    float3 to_light = normalize(light_position - _position);
    float3 halfway = normalize(_to_camera + to_light);

    float attenuation = 1.f / dot(g_light.attenuation, float3(1, distance, distance * distance));
    float falloff = pow(max(dot(-to_light, light_direction), 0.f), g_light.falloff);

    _ambient = g_material.ambient * g_light.ambient * attenuation * falloff;
    _diffuse = g_material.diffuse * g_light.diffuse * max(dot(_normal, to_light), 0.f) * attenuation * falloff;
    _specular = g_material.specular * g_light.specular * pow(max(dot(_normal, halfway), 0.f), g_light.specular.w) * falloff;
}
