#include "type.hlsl"

PS_OUTPUT_GBUFFER GBufferPS(VS_OUTPUT_POSITION_NORMAL_COLOR _input)
{
    PS_OUTPUT_GBUFFER output = (PS_OUTPUT_GBUFFER)0;

    output.albedo = _input.color;
    output.normal.xyz = _input.normalV;
    output.normal.w = g_light_specular.w;
    output.depth.rgb = float3(g_projection_far - _input.positionV.z, g_projection_far - _input.positionV.z, g_projection_far - _input.positionV.z) / g_projection_far;
    output.depth.a = _input.position.w;
    output.material.r = CompressColor(g_material_ambient);
    output.material.g = CompressColor(g_material_diffuse);
    output.material.b = CompressColor(g_material_specular);
    output.material.a = CompressColor(g_material_emissive);
    
    return output;
}
