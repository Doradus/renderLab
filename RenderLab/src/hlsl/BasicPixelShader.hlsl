struct PixelIn {
    float4 color : COLOR;
};

float4 BasicPixelShader(PixelIn vIn) : SV_TARGET {
    return vIn.color;
}