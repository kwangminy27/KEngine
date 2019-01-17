#include "type.hlsl"

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

Texture2D g_MRT_albedo : register(t8);
Texture2D g_MRT_normal : register(t9);
Texture2D g_MRT_depth : register(t10);
Texture2D g_MRT_material : register(t11);

SamplerState g_point_sampler : register(s1);

PS_OUTPUT_LIGHT DeferredLightingPS(VS_OUTPUT_POSITION_TEX _input)
{
    PS_OUTPUT_LIGHT output = (PS_OUTPUT_LIGHT)0;

    float4 depth = g_MRT_depth.Sample(g_point_sampler, _input.uv);
    float4 normal = g_MRT_normal.Sample(g_point_sampler, _input.uv);
    float4 material = g_MRT_material.Sample(g_point_sampler, _input.uv);

    float3 NDC = float3(_input.uv.x * 2.f - 1.f, _input.uv.y * -2.f + 1.f, depth.r);
    float4 HCS = float4(NDC * depth.a, depth.a);

    float3 positionV = mul(HCS, g_projection_Inv).xyz;
    float3 normalV = normal.xyz;

    float3 to_cameraV = normalize(-positionV);

    float4 ambient = DecompressColor(material.r);
    float4 diffuse = DecompressColor(material.g);
    float4 specular = DecompressColor(material.b);

    Lighting lighting = (Lighting)0;

    switch(g_light_type)
    {
        case DIRECTIONAL:
            lighting = ComputeDirectionalLight(normalV, to_cameraV, ambient, diffuse, specular);
            break;
        case POINT:
            lighting = ComputePointLight(positionV, normalV, to_cameraV, ambient, diffuse, specular);
            break;
        case SPOT:
            lighting = ComputeSpotLight(positionV, normalV, to_cameraV, ambient, diffuse, specular);
            break;
    }

    output.ambient = lighting.ambient;
    output.diffuse = lighting.diffuse;
    output.specular = lighting.specular;

    return output;
}
