#include "type.hlsl"

Texture2D g_MRT_albedo : register(t8);
Texture2D g_MRT_normal : register(t9);
Texture2D g_MRT_depth : register(t10);
Texture2D g_MRT_material : register(t11);

Texture2D g_MRT_ambient : register(t12);
Texture2D g_MRT_diffuse : register(t13);
Texture2D g_MRT_specular : register(t14);

SamplerState g_point_sampler : register(s1);

PS_OUTPUT_SINGLE_TARGET DeferredLightingCalculateColorPS(VS_OUTPUT_POSITION _input)
{
    PS_OUTPUT_SINGLE_TARGET output = (PS_OUTPUT_SINGLE_TARGET)0;

    float2 uv = float2(_input.position.x / g_viewport.x, _input.position.y / g_viewport.y);

    float4 albedo = g_MRT_albedo.Sample(g_point_sampler, uv);
    float4 ambient = g_MRT_ambient.Sample(g_point_sampler, uv);
    float4 diffuse = g_MRT_diffuse.Sample(g_point_sampler, uv);
    float4 specular = g_MRT_specular.Sample(g_point_sampler, uv);

    output.target = albedo * (ambient + diffuse) + specular;

    return output;
}
