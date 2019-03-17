//reserved textures
Texture2DArray shadowMap;
TextureCube omniDirectionalShadowMap;
TextureCube environmentLight;

#if TEXTURE_2D_00 > 0 
Texture2D texture00;
#endif

#if TEXTURE_2D_01 > 0 
Texture2D texture01;
#endif

#if TEXTURE_2D_02 > 0 
Texture2D texture02;
#endif

#if TEXTURE_2D_03 > 0 
Texture2D texture03;
#endif

#if TEXTURE_2D_04 > 0 
Texture2D texture04;
#endif

#if TEXTURE_2D_05 > 0 
Texture2D texture05;
#endif

#if TEXTURE_2D_06 > 0 
Texture2D texture07;
#endif

#if TEXTURE_2D_07 > 0 
Texture2D texture07;
#endif

#if TEXTURE_2D_08 > 0 
Texture2D texture08;
#endif

#if TEXTURE_2D_09 > 0 
Texture2D texture09;
#endif

#if TEXTURE_2D_10 > 0 
Texture2D texture10;
#endif

#if TEXTURE_2D_11 > 0 
Texture2D texture11;
#endif

#if TEXTURE_2D_12 > 0 
Texture2D texture12;
#endif

#if TEXTURE_2D_13 > 0 
Texture2D texture13;
#endif

#if TEXTURE_2D_14 > 0 
Texture2D texture14;
#endif

#if TEXTURE_2D_15 > 0 
Texture2D texture15;
#endif

SamplerComparisonState depthSampler;
SamplerState trilinearSampler;
SamplerState bilinearSampler;