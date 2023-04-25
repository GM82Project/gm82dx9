struct VS_INPUT {
   float4 position: POSITION0;
   float2 texcoord: TEXCOORD0;
   float4 color: COLOR0;
};

struct VS_OUTPUT {
   float4 position: POSITION0;
   float2 texcoord: TEXCOORD0;
   float4 color: COLOR0;
};

matrix rMatrixWVP;

VS_OUTPUT main(VS_INPUT input) {
   VS_OUTPUT output;
   
   output.position = mul(rMatrixWVP, input.position);   
   output.texcoord = input.texcoord;   
   output.color = input.color;

   return output;
}
