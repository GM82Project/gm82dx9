SamplerState Sampler : register(s0);

struct PS_INPUT {
    float4 position: POSITION0;
    float2 uv: TEXCOORD0;
    float4 color: COLOR;
};

struct PS_OUTPUT {
    float4 color: COLOR;
};

float3 rgb2hsv(float3 c) {
  float4 K = float4(0.f, -1.f / 3.f, 2.f / 3.f, -1.f);
  float4 p = c.g < c.b ? float4(c.bg, K.wz) : float4(c.gb, K.xy);
  float4 q = c.r < p.x ? float4(p.xyw, c.r) : float4(c.r, p.yzx);

  float d = q.x - min(q.w, q.y);
  float e = 1e-10;
  return float3(abs(q.z + (q.w - q.y) / (6.f * d + e)), d / (q.x + e), q.x);
}

float3 hsv2rgb(float3 c) {
  float4 K = float4(1.f, 2.f / 3.f, 1.f / 3.f, 3.f);
  float3 p = abs(frac(c.xxx + K.xyz) * 6.f - K.www);
  return c.z * lerp(K.xxx, saturate(p - K.xxx), c.y);
}

uniform float amount = 0;

PS_OUTPUT main(PS_INPUT input) {
    PS_OUTPUT output;

    float4 diffuse = tex2D(Sampler,input.uv) * input.color;

    float3 hsv = rgb2hsv(diffuse.rgb);
    hsv.x = fmod(hsv.x + amount, 1);

    output.color=float4(hsv2rgb(hsv), diffuse.a);
    
    return output;
}