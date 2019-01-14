#include "type.hlsl"

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

PS_OUTPUT_SINGLE_TARGET BasicNormalPS(VS_OUTPUT_POSITION_NORMAL_COLOR _input)
{
    PS_OUTPUT_SINGLE_TARGET output = (PS_OUTPUT_SINGLE_TARGET) 0;

    float4 ambient = (float4) 0;
    float4 diffuse = (float4) 0;
    float4 specular = (float4) 0;

    float3 to_cameraV = normalize(-_input.positionV);

    switch (g_light.type)
    {
        case DIRECTIONAL:
            ComputeDirectionalLight(_input.normalV, to_cameraV, ambient, diffuse, specular);
            break;
        case POINT:
            ComputePointLight(_input.positionV, _input.normalV, to_cameraV, ambient, diffuse, specular);
            break;
        case SPOT:
            ComputeSpotLight(_input.positionV, _input.normalV, to_cameraV, ambient, diffuse, specular);
            break;
    }

    output.target = _input.color * (ambient + diffuse) + specular;

    return output;
}
