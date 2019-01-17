#include "type.hlsl"

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

PS_OUTPUT_SINGLE_TARGET BasicNormalPS(VS_OUTPUT_POSITION_NORMAL_COLOR _input)
{
    PS_OUTPUT_SINGLE_TARGET output = (PS_OUTPUT_SINGLE_TARGET) 0;

    float3 to_cameraV = normalize(-_input.positionV);

    Lighting lighting = (Lighting)0;

    switch (g_light_type)
    {
        case DIRECTIONAL:
            lighting = ComputeDirectionalLight(_input.normalV, to_cameraV, g_material_ambient, g_material_diffuse, g_material_specular);
            break;
        case POINT:
            lighting = ComputePointLight(_input.positionV, _input.normalV, to_cameraV, g_material_ambient, g_material_diffuse, g_material_specular);
            break;
        case SPOT:
            lighting = ComputeSpotLight(_input.positionV, _input.normalV, to_cameraV, g_material_ambient, g_material_diffuse, g_material_specular);
            break;
    }

    output.target = _input.color * (lighting.ambient + lighting.diffuse) + lighting.specular;

    return output;
}
