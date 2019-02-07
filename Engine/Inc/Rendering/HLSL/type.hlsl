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

struct VS_INPUT_3D
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 binormal : BINORMAL;
    float3 tangent : TANGENT;
    float4 joint_weights : JOINT_WEIGHTS;
    uint4 joint_indices : JOINT_INDICES;
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

struct VS_OUTPUT_3D
{
    float4 position : SV_POSITION;
    float3 positionV : POSITION0;
    float4 positionH : POSITION1;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 binormal : BINORMAL;
    float3 tangent : TANGENT;
    float4 joint_weights : JOINT_WEIGHTS;
    uint4 joint_indices : JOINT_INDICES;
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
    float3 g_light_position : packoffset(c3);
    int g_light_type : packoffset(c3.w);
    float3 g_light_direction : packoffset(c4);
    float g_light_range : packoffset(c4.w);
    float3 g_light_attenuation : packoffset(c5);
    float g_light_falloff : packoffset(c5.w);
}

cbuffer Common : register(b5)
{
    float2 g_viewport : packoffset(c0);
    float g_time : packoffset(c0.z);
    float padding : packoffset(c0.w);
}

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

struct LightingInfo
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
};

LightingInfo ComputeDirectionalLight(float3 _normalV, float3 _to_cameraV, float4 _material_ambient, float4 _material_diffuse, float4 _material_specular)
{
    LightingInfo output = (LightingInfo)0;

    output.ambient = _material_ambient * g_light_ambient;

    float3 light_direction = normalize(g_light_direction);

    float3 to_lightV = mul(float4(-light_direction, 0.f), g_view).xyz;
    float diffuse_factor = saturate(dot(_normalV, to_lightV));

    output.diffuse = _material_diffuse * g_light_diffuse * diffuse_factor;

    if(diffuse_factor < 0.01f)
        return output;

    float3 halfwayV = normalize(_to_cameraV + to_lightV);
    float specular_factor = pow(saturate(dot(_normalV, halfwayV)), g_light_specular.w);

    output.specular = _material_specular * g_light_specular * specular_factor;

    return output;
}

LightingInfo ComputePointLight(float3 _positionV, float3 _normalV, float3 _to_cameraV, float4 _material_ambient, float4 _material_diffuse, float4 _material_specular)
{
    LightingInfo output = (LightingInfo)0;

    float3 light_positionV = mul(float4(g_light_position, 1.f), g_view).xyz;
    float distance = length(light_positionV - _positionV);

    if(g_light_range < distance)
        return output;

    float attenuation = 1.f / dot(g_light_attenuation, float3(1, distance, distance * distance));

    output.ambient = _material_ambient * g_light_ambient;
        
    float3 to_lightV = normalize(light_positionV - _positionV);
    float diffuse_factor = saturate(dot(_normalV, to_lightV));

    output.diffuse = _material_diffuse * g_light_diffuse * diffuse_factor * attenuation;

    if(diffuse_factor < 0.01f)
        return output;

    float3 halfwayV = normalize(_to_cameraV + to_lightV);
    float specular_factor = pow(saturate(dot(_normalV, halfwayV)), g_light_specular.w);

    output.specular = _material_specular * g_light_specular * specular_factor * attenuation;

    return output;
}

LightingInfo ComputeSpotLight(float3 _positionV, float3 _normalV, float3 _to_cameraV, float4 _material_ambient, float4 _material_diffuse, float4 _material_specular)
{
    LightingInfo output = (LightingInfo)0;

    float3 light_positionV = mul(float4(g_light_position, 1.f), g_view).xyz;
    float3 light_directionV = mul(float4(normalize(g_light_direction), 0.f), g_view).xyz;

    float distance = length(light_positionV - _positionV);
    
    if (g_light_range < distance)
        return output;

    float attenuation = 1.f / dot(g_light_attenuation, float3(1, distance, distance * distance));

    output.ambient = _material_ambient * g_light_ambient;

    float3 to_lightV = normalize(light_positionV - _positionV);
    float diffuse_factor = saturate(dot(_normalV, to_lightV));

    float falloff = pow(saturate(dot(-to_lightV, light_directionV)), g_light_falloff);

    output.diffuse = _material_diffuse * g_light_diffuse * diffuse_factor * attenuation * falloff;

    if(diffuse_factor < 0.01f)
        return output;

    float3 halfwayV = normalize(_to_cameraV + to_lightV);
    float specular_factor = pow(saturate(dot(_normalV, halfwayV)), g_light_specular.w);

    output.specular = _material_specular * g_light_specular * specular_factor * falloff;

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

    return asfloat(result);
}

float4 DecompressColor(float _color)
{
    uint color = asuint(_color);

    float4 result = (float4)0;
    result.r = (color & 0x000000FF) / 255.f;
    result.g = ((color >> 8) & 0x000000FF) / 255.f;
    result.b = ((color >> 16) & 0x000000FF) / 255.f;
    result.a = ((color >> 24) & 0x000000FF) / 255.f;

    return result;
}