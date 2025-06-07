/*=============================================================================
	UnRenDev.h: 3D rendering device class.

	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
	Compiled with Visual C++ 4.0. Best viewed with Tabs=4.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

#ifndef _UNRENDEV_H_
#define _UNRENDEV_H_

/*
	FRenderInterface
*/

// sjs ---
// dynamic vertex components
enum EVertexComponents
{
    VF_Position = 1 << 0,
    VF_Normal   = 1 << 1,
    VF_Diffuse  = 1 << 2,
    VF_Specular = 1 << 3,
    VF_Tex1     = 1 << 4,
    VF_Tex2     = 1 << 5,
    VF_Tex3     = 1 << 6,
    VF_Tex4     = 1 << 7,
};
// --- sjs

// Cull modes.

enum ECullMode
{
	CM_CW,
	CM_CCW,
	CM_None
};

// Transform types.

enum ETransformType
{
	TT_LocalToWorld,
	TT_WorldToCamera,
	TT_CameraToScreen
};

// Texture coordinate sources.
enum ETexCoordSource
{
	TC_UVStream,
	TC_CameraPosition,
	TC_CameraNormal,
	TC_CameraReflectionVector,
    TC_SphereMapCoords // sjs
};

// Primitive types for DrawPrimitive.

enum EPrimitiveType
{
	PT_TriangleList,
	PT_TriangleStrip,
	PT_TriangleFan,
	PT_PointList,
	PT_LineList,
#ifdef _XBOX // sjs
    PT_QuadList,
#endif
};

// Vertex shaders for SetVertexStreams.

enum EVertexShader
{
	VS_FixedFunction
};

// Stencil buffer operations.

enum EStencilOp
{
	SO_Keep			= 1,
	SO_Zero			= 2,
	SO_Replace		= 3,
	SO_IncrementSat	= 4,
	SO_DecrementSat	= 5,
	SO_Invert		= 6,
	SO_Increment	= 7,
	SO_Decrement	= 8
};

// Comparison functions.

enum ECompareFunction
{
	CF_Never		= 1,
	CF_Less			= 2,
	CF_Equal		= 3,
	CF_LessEqual	= 4,
	CF_Greater		= 5,
	CF_NotEqual		= 6,
	CF_GreaterEqual	= 7,
	CF_Always		= 8
};

enum EFogState
{
	FS_Store		= 0,
	FS_Restore		= 1,
	FS_Ignore		= 2
};

enum EHardwareEmulationMode
{
	HEM_None		= 0,
	HEM_GeForce1	= 1,
	HEM_XBox		= 2,
};

// Precaching options.
enum EPrecacheMode
{
	PRECACHE_VertexBuffers,
	PRECACHE_All
};

enum EPixelShader
{
	PS_None = 0,
	PS_Terrain3Layer = 1,
	PS_Terrain4Layer = 2,
};

enum EShaderConstantType {};

class ENGINE_API FRenderInterface
{
public:

	virtual void PushState() = 0;
	virtual void PopState() = 0;

	virtual UBOOL SetRenderTarget(FRenderTarget* RenderTarget) = 0;
	virtual void SetViewport(INT X,INT Y,INT Width,INT Height) = 0;
	virtual void Clear(UBOOL UseColor = 1,FColor Color = FColor(0,0,0),UBOOL UseDepth = 1,FLOAT Depth = 1.0f,UBOOL UseStencil = 1,DWORD Stencil = 0) = 0;

	virtual void PushHit(const BYTE* Data,INT Count) = 0;
	virtual void PopHit(INT Count,UBOOL Force) = 0;

	virtual void SetCullMode(ECullMode CullMode) = 0;

	virtual void SetAmbientLight(FColor Color) = 0;
	virtual void EnableLighting(UBOOL UseDynamic, UBOOL UseStatic=1, UBOOL Modulate2X=0, FBaseTexture* UseLightmap=NULL, UBOOL LightingOnly=0, FSphere LitSphere=FSphere(FVector(0,0,0),0)) = 0;
	virtual void SetLight(INT LightIndex, FDynamicLight* Light, FLOAT Scale=1.0f) = 0;
	virtual void SetPawnLight(INT LightIndex, FNPawnLight* Light, FLOAT Scale = 1.0f) = 0;
	virtual void SetShaderLight(INT, FDynamicLight*, ELightColorType, FLOAT);

	virtual void SetNPatchTesselation( FLOAT Tesselation ) = 0;
	virtual void SetDistanceFog(UBOOL Enable,FLOAT FogStart,FLOAT FogEnd,FColor Color) = 0;
	virtual void SetGlobalColor(FColor Color) = 0;
	virtual void SetTransform(ETransformType Type,FMatrix Matrix) = 0;
	virtual void SetMaterial(UMaterial* Material,FString* ErrorString=NULL, UMaterial** ErrorMaterial=NULL, INT* NumPasses=NULL) = 0;

	virtual void StartCompileMaterial(UMaterial *);
	virtual void SetL2ShaderMaterial(UMaterial *);
	virtual void SetL2ShaderVertexStreams(FVertexStream * *, INT);
	virtual void SetL2ShaderFunction(DWORD);
	virtual void UnSetL2ShaderFunction(DWORD);
	virtual void SetL2ShaderFunctionEx(DWORD);
	virtual void UnSetL2ShaderFunctionEx(DWORD);
	virtual void ClearMaterialCode();
	virtual INT SetL2ShaderDynamicStream(FVertexStream *);
	virtual void SetVertexShaderConstantByName(TCHAR*, FLOAT const *, unsigned INT);
	virtual void SetPixelShaderConstantByName(TCHAR*, FLOAT const *, unsigned INT);
	virtual void SetL2ShaderPostProcessType(INT);

	virtual void SetMaterialTime(DOUBLE);
	virtual void EnableMaterialTime();
	virtual void DisableMaterialTim();

	virtual INT CheckCachedMaterial(INT, INT, INT, INT, INT); //Doesn't checked

	virtual void SetRefractionPass(INT);
	virtual void SetRefractionTex(FRenderTarget *, FRenderTarget *);
	virtual void SetOverrideMaterial(UMaterial *);

	virtual void SetStencilOp(ECompareFunction Test,DWORD Ref,DWORD Mask,EStencilOp FailOp,EStencilOp ZFailOp,EStencilOp PassOp,DWORD WriteMask) = 0;
	virtual void SetPrecacheMode( EPrecacheMode PrecacheMode ) = 0;
    virtual void SetZBias(INT ZBias) = 0;
	virtual void SetSlopeZBias(FLOAT) = 0; //Doesn't checked

	virtual INT SetVertexStreams(EVertexShader Shader,FVertexStream** Streams,INT NumStreams) = 0;
	virtual INT SetDynamicStream(EVertexShader Shader,FVertexStream* Stream) = 0;

	virtual INT SetIndexBuffer(FIndexBuffer* IndexBuffer,INT BaseIndex) = 0;
	virtual INT SetDynamicIndexBuffer(FIndexBuffer* IndexBuffer,INT BaseIndex) = 0;

	virtual void DrawPrimitive(EPrimitiveType PrimitiveType,INT FirstIndex,INT NumPrimitives,INT MinIndex = INDEX_NONE,INT MaxIndex = INDEX_NONE) = 0;

	virtual void SetTextureStageState(DWORD, INT, DWORD);
	virtual INT GetTextureStageState(DWORD, INT, DWORD*);

	virtual void RenderUnderWaterEffect(QWORD);

	virtual void SetClipPlane(DWORD, FPlane*, DWORD);
	virtual void UnSetClipPlane();

	virtual INT GetCurrentZBias();

	virtual INT GetRenderTarget(FRenderTarget*);

	virtual void SetPixelShader(EPixelShader);
	virtual void SetUnk00(INT); //Doesn't checked
	virtual INT GetUnk00(); //Doesn't checked
	virtual EPixelShader GetPixelShaderType();
	virtual void SetPixelShaderConstant(DWORD, void const*, DWORD);
	virtual void SetPixelShaderConstant(EShaderConstantType, TCHAR*, void*, INT);
	virtual INT PossibleShader(INT, INT);

	virtual void QueryIssue();

	virtual void DrawTextW(INT, INT, INT, INT, TCHAR*, DWORD, INT, DWORD);
	virtual void GetCharSize(TCHAR, size_t*, INT);
	virtual void TextSpriteBegin(TCHAR*);
	virtual void TextSpriteEnd();
	virtual void TextSpriteSetTranform(FMatrix);
	virtual void TextSpriteSetWorldViewTranform(FMatrix, FMatrix);
	virtual void TextSpriteSetProjTranform(FMatrix);
	
	virtual FLOAT* GetVertexShaderConstants();
	virtual FLOAT* GetVertexShaderSkinningData();

	virtual UBOOL IsShadow();

	virtual UBOOL IsCurrentRenderTarget(FRenderTarget*);
};

/*
	URenderDevice
*/

enum EDescriptionFlags
{
	RDDESCF_Certified       = 1,
	RDDESCF_Incompatible    = 2,
	RDDESCF_LowDetailWorld  = 4,
	RDDESCF_LowDetailSkins  = 8,
	RDDESCF_LowDetailActors = 16,
};

// Usage types (determine hardware/software shader and buffer choices)
enum EUsageType
{
	UT_Terrain,
	UT_Static,
	UT_Skin,	
	UT_Morphed,
	UT_World
};

//
// FRenderCaps - render device capabilities exposed to the engine
//
struct FRenderCaps
{
	INT MaxSimultaneousTerrainLayers;
	INT		PixelShaderVersion;	//!!powervr_aaron: Terrain optimisation code needs to know whether pixel shaders are being used
	UBOOL	HardwareTL;			//!!powervr_aaron: Terrain optimisation code needs to know whether SW TnL is being used

	// set default render caps in constructor
	FRenderCaps()
	:	MaxSimultaneousTerrainLayers(1),
		PixelShaderVersion(0),
		HardwareTL(0)
	{}
};

//
// A low-level 3D rendering device.
//
class ENGINE_API URenderDevice : public USubsystem
{
	DECLARE_ABSTRACT_CLASS(URenderDevice,USubsystem,CLASS_Config,Engine)

	// Variables.
	BYTE			DecompFormat;
	INT				RecommendedLOD;
	FString			Description;
	DWORD			DescFlags;
	BITFIELD		HighDetailActors;
	BITFIELD		SuperHighDetailActors;
	BITFIELD		DetailTextures;
	BITFIELD		PrecacheOnFlip;
	BITFIELD		SupportsCubemaps;
	BITFIELD		SupportsZBIAS;
	BITFIELD		UseCompressedLightmaps;
	BITFIELD		UseStencil;
	BITFIELD		Use16bit;
	BITFIELD		Use16bitTextures;
	BITFIELD		IsVoodoo3;
	BITFIELD		Is3dfx;
	BITFIELD		Pad1[8];
	DWORD			Pad0[8];

	BYTE			Pad00[12]; //Doesn't check

	// Constructors.
	void StaticConstructor();

	virtual UBOOL Exec(const TCHAR* Cmd, FOutputDevice& Ar) = 0;

	// URenderDevice low-level functions that drivers must implement.
	virtual UBOOL Init()=0;
	virtual UBOOL SetRes(UViewport *, INT, INT, INT, INT, INT, INT)=0;
	virtual UBOOL SetRes( UViewport* Viewport, INT NewX, INT NewY, UBOOL Fullscreen, INT ColorBytes=0,UBOOL bSaveSize=true)=0;
	virtual INT GetRes(INT, INT*, INT*, INT*) = 0;
	virtual INT GetRefreshRate(INT, INT, INT, INT, INT*) = 0;
	virtual void Exit( UViewport* Viewport )=0;

	virtual void Flush( UViewport* Viewport )=0;
	virtual void FlushResource( QWORD CacheId )=0;
	virtual UBOOL ResourceCached( QWORD CacheId ) { return 0; }

	virtual void UpdateGamma( UViewport* Viewport )=0;
	virtual void RestoreGamma()=0;

	virtual FRenderInterface* Lock( UViewport* Viewport, BYTE* HitData, INT* HitSize )=0;
	virtual void Unlock( FRenderInterface* RI ) = 0;
	virtual void Present( UViewport* Viewport ) = 0;

	virtual void ReadPixels( UViewport* Viewport, FColor* Pixels )=0;
	virtual void HoldBackBuffer() = 0;
	virtual void DeleteAllTexture() = 0;
	virtual UBOOL IsUseTrilinear() = 0;
	virtual void SetUseTrilinear(INT) = 0;

	virtual DWORD GetCreateTextureBytes();

	virtual void SetEmulationMode(EHardwareEmulationMode Mode) = 0;
	virtual FRenderCaps* GetRenderCaps() = 0;

	virtual void ReLoadPixelShader();
	virtual INT GetResParams(INT&, INT&, INT&, INT&, INT&);
	virtual void GetShaderVersion(INT&, INT&);
	virtual INT GetMultiSample();
	virtual INT SupportsTextureFormat(enum ETextureFormat);
	virtual FRenderInterface* GetRenderInterface();
	virtual void ClearCompiledShaders();
	virtual void ClearCompiledShader(unsigned long, UMaterial*);
};

#endif
/*------------------------------------------------------------------------------------
	The End.
------------------------------------------------------------------------------------*/

