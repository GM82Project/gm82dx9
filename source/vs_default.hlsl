float4 rLights_Color[8]: register(c0);
float4 rLights_Direction[8]: register(c8);
float4 rLights_PosRange[8]: register(c16);

matrix rMatrixW: register(c24);
matrix rMatrixWV: register(c28);
matrix rMatrixWVP: register(c32);

float rLightingEnabled: register(c36);
float4 rAmbientColor: register(c37);

float rFogEnabled: register(c38);
float rFogStart: register(c39);
float rRcpFogRange: register(c40);
float4 rFogColor: register(c41);

float4 vec4(float x0, float x1, float x2, float x3) {
    return float4(x0, x1, x2, x3);
}
float4 vec4(float3 x0, float x1) {
    return float4(x0, x1);
}
float4 vec4(float x0) {
    return float4(x0,x0,x0,x0);
}

float4 fDoFog(in float4 _pos, in float4 _color) {    
    float4 _viewpos = mul((rMatrixWV), _pos);
    float _fogfactor = (_viewpos.z - rFogStart) * rRcpFogRange;
    return lerp(_color, rFogColor, _fogfactor);       
}

float4 fDoDirLight(in float3 _ws_normal, in float4 _dir, in float4 _diffusecol) {    
    float _dotresult = dot(_ws_normal, _dir.xyz);
    _dotresult = max(0.0, _dotresult);
    return (_dotresult * _diffusecol);
}

float4 fDoPointLight(in float3 _ws_pos, in float3 _ws_normal, in float4 _posrange, in float4 _diffusecol) {   
    float3 _diffvec = _ws_pos - _posrange.xyz;
    float _veclen = length(_diffvec);
    _diffvec /= _veclen;
    float _atten = 1.0 / (_veclen / _posrange.w);
    if ((_veclen > _posrange.w)) {
        _atten = 0.0;
    }
    float _dotresult = dot(_ws_normal, _diffvec);
    _dotresult = max(0.0, _dotresult);
    return (_dotresult * _atten) * _diffusecol;
}

float4 fDoLighting(in float4 _vertexcolour, in float4 _objectspacepos, in float3 _objectspacenormal) {    
    float4 _objectspacenormal4 = vec4(_objectspacenormal, 0.0);
    float3 _ws_normal = {0, 0, 0};
    _ws_normal = mul((rMatrixWV), _objectspacenormal4).xyz;
    _ws_normal = (normalize(_ws_normal));
    _ws_normal.z=-_ws_normal.z;
    float3 _ws_pos = {0, 0, 0};
    _ws_pos = mul((rMatrixW), _objectspacepos).xyz;
    float4 _accumcol = vec4(0.0);
    
    int _i=0;
    
    for (_i = 0; _i < 8; _i++) {            
        _accumcol += fDoDirLight(_ws_normal, rLights_Direction[_i], rLights_Color[_i]);
    }
    
    for (_i = 0; _i < 8; _i++) {
        _accumcol += fDoPointLight(_ws_pos, _ws_normal, rLights_PosRange[_i], rLights_Color[_i]);
    }
    
    _accumcol += rAmbientColor;
    _accumcol *= _vertexcolour;
    _accumcol = min(vec4(1.0), _accumcol);
    _accumcol.w = _vertexcolour.w;
    return _accumcol;
}

struct VS_INPUT {
    float4 position: POSITION;
    float2 texcoord: TEXCOORD0;
    float3 normal: NORMAL;
    float4 color: COLOR;
};

struct VS_OUTPUT {
    float4 position: POSITION;
    float2 texcoord: TEXCOORD0;
    float4 color: COLOR;
};

VS_OUTPUT main (VS_INPUT input) {
   VS_OUTPUT output;
   
   float4 color=input.color;
   if (rLightingEnabled>0.5) color=fDoLighting(color,input.position,input.normal);
   
   if (rFogEnabled>0.5) color = fDoFog(input.position,color);
   
   output.position = mul(rMatrixWVP, input.position);   
   output.texcoord = input.texcoord;   
   output.color = color;

   return output;
}