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

struct PS_OUTPUT_LIGHT
{
    float4 ambient : SV_TARGET0;
    float4 diffuse : SV_TARGET1;
    float4 specular : SV_TARGET2;
};

cbuffer Transform : register(b0)
{
	matrix g_world;
	matrix g_view;
	matrix g_projection;
    matrix g_WV;
	matrix g_WVP;
    matrix g_projection_Inv;
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
    float3 g_light_position : packoffset(c3.x);
    float3 g_light_direction : packoffset(c4.x);
    float3 g_light_attenuation : packoffset(c5.x);
    float g_light_falloff : packoffset(c5.w);
    int g_light_type : packoffset(c6.x);
    float g_light_range : packoffset(c6.y);
    float g_light_in_angle : packoffset(c6.z);
    float g_light_out_angle : packoffset(c6.w);
}

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

struct Lighting
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
};

Lighting ComputeDirectionalLight(float3 _normal, float3 _to_camera, float4 _material_ambient, float4 _material_diffuse, float4 _material_specular)
{
    Lighting output = (Lighting)0;

    float3 to_light = normalize(mul(float4(-g_light_direction, 0.f), g_view)).xyz;
    float3 halfway = normalize(_to_camera + to_light);

    output.ambient = _material_ambient * g_light_ambient;
    output.diffuse = _material_diffuse * g_light_diffuse * max(dot(_normal, to_light), 0.f);
    output.specular = _material_specular * g_light_specular * pow(max(dot(_normal, halfway), 0.f), g_light_specular.w);

    return output;
}

Lighting ComputePointLight(float3 _position, float3 _normal, float3 _to_camera, float4 _material_ambient, float4 _material_diffuse, float4 _material_specular)
{
    Lighting output = (Lighting)0;

    float3 light_position = mul(float4(g_light_position, 1.f), g_view).xyz;

    float distance = length(light_position - _position);
    
    if(g_light_range < distance)
        return output;

    float3 to_light = normalize(light_position - _position);
    float3 halfway = normalize(_to_camera + to_light);

    float attenuation = 1.f / dot(g_light_attenuation, float3(1, distance, distance * distance));

    output.ambient = _material_ambient * g_light_ambient;
    output.diffuse = _material_diffuse * g_light_diffuse * max(dot(_normal, to_light), 0.f) * attenuation;
    output.specular = _material_specular * g_light_specular * pow(max(dot(_normal, halfway), 0.f), g_light_specular.w) * attenuation;

    return output;
}

Lighting ComputeSpotLight(float3 _position, float3 _normal, float3 _to_camera, float4 _material_ambient, float4 _material_diffuse, float4 _material_specular)
{
    Lighting output = (Lighting)0;

    float3 light_position = mul(float4(g_light_position, 1.f), g_view).xyz;
    float3 light_direction = normalize(mul(float4(g_light_direction, 0.f), g_view)).xyz;

    float distance = length(light_position - _position);
    
    if (g_light_range < distance)
        return output;

    float3 to_light = normalize(light_position - _position);
    float3 halfway = normalize(_to_camera + to_light);

    float attenuation = 1.f / dot(g_light_attenuation, float3(1, distance, distance * distance));
    float falloff = pow(max(dot(-to_light, light_direction), 0.f), g_light_falloff);

    output.ambient = _material_ambient * g_light_ambient;
    output.diffuse = _material_diffuse * g_light_diffuse * max(dot(_normal, to_light), 0.f) * attenuation * falloff;
    output.specular = _material_specular * g_light_specular * pow(max(dot(_normal, halfway), 0.f), g_light_specular.w) * falloff;

    return output;
}

float CompressColor(float4 _color)
{
    uint4 color = (uint4)0;
    color.r = (uint)(_color.r * 255.f);
    color.g = (uint)(_color.g * 255.f);
    color.b = (uint)(_color.b * 255.f);
    color.a = (uint)(_color.a * 255.f);

    uint result = (uint)0;
    result |= color.r & 0x000000FF;
    result |= (color.g << 8) & 0x0000FF00;
    result |= (color.b << 16) & 0x00FF0000;
    result |= (color.a << 24) & 0xFF000000;

    return (float)result;
}

float4 DecompressColor(float _color)
{
    uint color = (uint)_color;

    float4 result = (float4)0;
    result.r = (color & 0x000000FF) / 255.f;
    result.g = ((color >> 8) & 0x000000FF) / 255.f;
    result.b = ((color >> 16) & 0x000000FF) / 255.f;
    result.a = ((color >> 24) & 0x000000FF) / 255.f;

    return result;
}