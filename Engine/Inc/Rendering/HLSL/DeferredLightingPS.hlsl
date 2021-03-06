#include "type.hlsl"

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

Texture2D g_MRT_albedo : register(t8);
Texture2D g_MRT_normal : register(t9);
Texture2D g_MRT_depth : register(t10);
Texture2D g_MRT_material : register(t11);

SamplerState g_point_sampler : register(s1);

PS_OUTPUT_LIGHT DeferredLightingPS(VS_OUTPUT_POSITION _input)
{
    PS_OUTPUT_LIGHT output = (PS_OUTPUT_LIGHT)0;

    float2 uv = float2(_input.position.x / g_viewport.x, _input.position.y / g_viewport.y);

    float4 depth = g_MRT_depth.Sample(g_point_sampler, uv);
    float4 normal = g_MRT_normal.Sample(g_point_sampler, uv);
    float4 material = g_MRT_material.Sample(g_point_sampler, uv);

    float3 NDC = float3(uv.x * 2.f - 1.f, uv.y * -2.f + 1.f, depth.r);
    float4 HCS = float4(NDC, 1.f) * depth.a;

    float3 positionV = mul(HCS, g_projection_Inv).xyz;
    float3 normalV = normal.xyz * 2.f - 1.f;

    float3 to_cameraV = normalize(-positionV);

    float4 ambient = DecompressColor(material.r);
    float4 diffuse = DecompressColor(material.g);
    float4 specular = DecompressColor(material.b);

	float specular_exp = normal.w;

    LightingInfo lighting = (LightingInfo)0;

    switch(g_light_type)
    {
        case DIRECTIONAL:
            lighting = ComputeDirectionalLight(normalV, to_cameraV, ambient, diffuse, specular, specular_exp);
            break;
        case POINT:
            lighting = ComputePointLight(positionV, normalV, to_cameraV, ambient, diffuse, specular, specular_exp);
            break;
        case SPOT:
            lighting = ComputeSpotLight(positionV, normalV, to_cameraV, ambient, diffuse, specular, specular_exp);
            break;
    }

    output.ambient = lighting.ambient;
    output.diffuse = lighting.diffuse;
    output.specular = lighting.specular;

    return output;
}
