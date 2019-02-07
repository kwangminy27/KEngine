#include "type.hlsl"

Texture2D g_diffuse_texture : register(t0);
Texture2D g_specular_texture : register(t1);
Texture2D g_bump_texture: register(t2);

PS_OUTPUT_GBUFFER BumpMappingPS(VS_OUTPUT_3D _input)
{
    PS_OUTPUT_GBUFFER output = (PS_OUTPUT_GBUFFER)0;

    float4 specular_color = g_specular_texture.Sample(g_sampler, _input.uv);
    float4 normal_color = g_bump_texture.Sample(g_sampler, _input.uv);

    float3 normal = normal_color.xyz * 2.f - 1.f;

    float3x3 TBN =
    {
        _input.tangent,
        _input.binormal,
        _input.normal
    };

    normal = mul(normal, TBN);

    output.albedo = g_diffuse_texture.Sample(g_sampler, _input.uv);
    output.normal.rgb = normal * 0.5f + 0.5f;
    output.depth.r = _input.position.z / _input.position.w;
    output.depth.g = output.depth.r;
    output.depth.b = output.depth.r;
    output.depth.a = _input.position.w;
    output.material.r = CompressColor(g_material_ambient);
    output.material.g = CompressColor(g_material_diffuse);
    output.material.b = CompressColor(specular_color);
    output.material.a = CompressColor(g_material_emissive);

    return output;
}
