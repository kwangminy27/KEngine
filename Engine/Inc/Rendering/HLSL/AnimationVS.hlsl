#include "type.hlsl"

VS_OUTPUT_3D AnimationVS(VS_INPUT_3D _input)
{
	VS_OUTPUT_3D output = (VS_OUTPUT_3D)0;

	SkinningInfo skinning_info = Skinning(_input.position, _input.normal, _input.binormal, _input.tangent, _input.joint_indices, _input.joint_weights);

	output.position = mul(float4(skinning_info.position, 1.f), g_WVP);
	output.positionV = mul(float4(skinning_info.position, 1.f), g_WV).xyz;
	output.positionH = output.position;
	output.uv = _input.uv;
	output.normal = normalize(mul(float4(skinning_info.normal, 0.f), g_WV)).xyz;
	output.binormal = normalize(mul(float4(skinning_info.binormal, 0.f), g_WV)).xyz;
	output.tangent = normalize(mul(float4(skinning_info.tangent, 0.f), g_WV)).xyz;

	return output;
}
