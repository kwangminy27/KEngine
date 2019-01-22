#include "type.hlsl"

PS_OUTPUT_GBUFFER GBufferPS(VS_OUTPUT_POSITION_NORMAL_COLOR _input)
{
    PS_OUTPUT_GBUFFER output = (PS_OUTPUT_GBUFFER)0;

    output.albedo = _input.color;
    output.normal.rgb = _input.normalV * 0.5f + 0.5f;
    output.depth.r = _input.position.z / _input.position.w;
    output.depth.g = output.depth.r;
    output.depth.b = output.depth.r;
    output.depth.a = _input.position.w;
    output.material.r = CompressColor(g_material_ambient);
    output.material.g = CompressColor(g_material_diffuse);
    output.material.b = CompressColor(g_material_specular);
    output.material.a = CompressColor(g_material_emissive);
    
    return output;
}
