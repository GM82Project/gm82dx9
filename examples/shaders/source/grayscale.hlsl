SamplerState Sampler : register(s0);

struct PS_INPUT {
    float4 position: POSITION0;
    float2 uv: TEXCOORD0;
    float4 color: COLOR;
};

struct PS_OUTPUT {
    float4 color: COLOR;
};

uniform float fade = 0.5;

PS_OUTPUT main(PS_INPUT input) {
    PS_OUTPUT output;

    float4 diffuse = tex2D(Sampler,input.uv) * input.color;

    float gray = dot(diffuse.rgb,float3(0.21,0.71,0.07));

    output.color = float4(lerp(diffuse.rgb,float3(gray,gray,gray),fade),diffuse.a);
    
    return output;
}
