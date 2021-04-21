[[vk::binding(0, 0)]] RWTexture2D<float4> outImage;

[[vk::binding(1, 0)]] cbuffer compositeDesc
{
    float flScale0X;
    float flScale0Y;
    float flScale1X;
    float flScale1Y;
    float flScale2X;
    float flScale2Y;
    float flScale3X;
    float flScale3Y;

	float flOffset0X;
	float flOffset0Y;
	float flOffset1X;
	float flOffset1Y;
	float flOffset2X;
	float flOffset2Y;
	float flOffset3X;
	float flOffset3Y;

	float flOpacity0;
	float flOpacity1;
	float flOpacity2;
	float flOpacity3;
}

[[vk::binding(2, 0)]] Texture2D inLayerTex0;
[[vk::binding(3, 0)]] SamplerState sampler0;

[[vk::binding(4, 0)]] Texture2D inLayerTex1;
[[vk::binding(5, 0)]] SamplerState sampler1;

[[vk::binding(6, 0)]] Texture2D inLayerTex2;
[[vk::binding(7, 0)]] SamplerState sampler2;

[[vk::binding(8, 0)]] Texture2D inLayerTex3;
[[vk::binding(9, 0)]] SamplerState sampler3;

[[vk::constant_id(0)]] const int  nLayerCount   = 1;
[[vk::constant_id(1)]] const bool bSwapChannels = false;

[numthreads(8, 8, 1)]
void main(
    uint3 groupId : SV_GroupID,
    uint3 groupThreadId : SV_GroupThreadID,
    uint3 dispatchThreadId : SV_DispatchThreadID,
    uint groupIndex : SV_GroupIndex)
{
	uint2 index = uint2(dispatchThreadId.x, dispatchThreadId.y);
	
	uint2 outSize;
	outImage.GetDimensions( outSize.x, outSize.y );
	
	if ( index.x >= outSize.x || index.y >= outSize.y )
	{
		return;
	}

	float4 outputValue;

	if ( nLayerCount >= 1 )
	{
		outputValue = inLayerTex0.Sample( sampler0, ( float2( index ) + float2( flOffset0X, flOffset0Y ) ) * float2( flScale0X, flScale0Y ) );
	}
	
	if ( nLayerCount >= 2 )
	{
		float4 layerSample = inLayerTex1.Sample( sampler1, ( float2( index ) + float2( flOffset1X, flOffset1Y ) ) * float2( flScale1X, flScale1Y ) );
		float layerAlpha = flOpacity1 * layerSample.a;
		outputValue = layerSample * layerAlpha + outputValue * ( 1.0 - layerAlpha );
	}
	
	if ( nLayerCount >= 3 )
	{
		float4 layerSample = inLayerTex2.Sample( sampler2, ( float2( index ) + float2( flOffset2X, flOffset2Y ) ) * float2( flScale2X, flScale2Y ) );
		float layerAlpha = flOpacity2 * layerSample.a;
		outputValue = layerSample * layerAlpha + outputValue * ( 1.0 - layerAlpha );
	}
	
	if ( bSwapChannels )
	{
		outImage [index] = outputValue.bgra;
    }
    else
    {
		outImage [index] = outputValue;
    }
    
    // indicator to quickly tell if we're in the compositing path or not
    if ( 0 && index.x > 50 && index.x < 100 && index.y > 50 && index.y < 100 )
    {
		outImage [index] = float4(1.0,0.0,1.0,1.0);
    }
}
