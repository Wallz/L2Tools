/*=============================================================================
	UnTex.h: Unreal texture related classes.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

/*-----------------------------------------------------------------------------
	Constants.
-----------------------------------------------------------------------------*/

enum {NUM_PAL_COLORS=256};	// Number of colors in a standard palette.

enum
{
	EHiColor565_R = 0xf800,
	EHiColor565_G = 0x07e0,
	EHiColor565_B = 0x001f,

	EHiColor555_R = 0x7c00,
	EHiColor555_G = 0x03e0,
	EHiColor555_B = 0x001f,

	ETrueColor_A  = 0xff000000,
	ETrueColor_R  = 0x00ff0000,
	ETrueColor_G  = 0x0000ff00,
	ETrueColor_B  = 0x000000ff,
};

/*-----------------------------------------------------------------------------
	UPalette.
-----------------------------------------------------------------------------*/

extern ENGINE_API FPlane FGetHSV( BYTE H, BYTE S, BYTE V );

//
// A palette object.  Holds NUM_PAL_COLORS unique FColor values, 
// forming a 256-color palette which can be referenced by textures.
//
class ENGINE_API UPalette : public UObject
{
	DECLARE_CLASS(UPalette,UObject,CLASS_SafeReplace,Engine)

	// Variables.
	TArray<FColor> Colors;

	// Constructors.
	UPalette();

	// UObject interface.
	void Serialize( FArchive& Ar );

	// UPalette interface.
	BYTE BestMatch( FColor Color, INT First );
	UPalette* ReplaceWithExisting();
	void FixPalette();
};

/*-----------------------------------------------------------------------------
	UTexture and FTexture.
-----------------------------------------------------------------------------*/


// Texture level-of-detail sets. (gam) ---
enum ELODSet
{
	LODSET_None        = 0,
	LODSET_World       = 1,
	LODSET_PlayerSkin  = 2,
	LODSET_WeaponSkin  = 3,
	LODSET_Terrain     = 4,
	LODSET_Interface   = 5,
	LODSET_RenderMap   = 6,
	LODSET_Lightmap    = 7,
	LODSET_MAX         = 8,
};
// --- gam

//
// Base mipmap.
//
struct ENGINE_API FMipmapBase
{
public:
	BYTE*			DataPtr;		// Pointer to data, valid only when locked.
	INT				USize,  VSize;	// Power of two tile dimensions.
	BYTE			UBits,  VBits;	// Power of two tile bits.
	FMipmapBase( BYTE InUBits, BYTE InVBits )
	:	DataPtr		(0)
	,	USize		(1<<InUBits)
	,	VSize		(1<<InVBits)
	,	UBits		(InUBits)
	,	VBits		(InVBits)
	{}
	FMipmapBase()
	{}
};

//
// Texture mipmap.
//
struct ENGINE_API FMipmap : public FMipmapBase
{
public:
	TLazyArray<BYTE> DataArray; // Data.
	FMipmap()
	{}
	FMipmap( BYTE InUBits, BYTE InVBits )
	:	FMipmapBase( InUBits, InVBits )
	,	DataArray( USize * VSize )
	{}
	FMipmap( BYTE InUBits, BYTE InVBits, INT InSize )
	:	FMipmapBase( InUBits, InVBits )
	,	DataArray( InSize )
	{}
	void Clear()
	{
		guard(FMipmap::Clear);
		appMemzero( &DataArray(0), DataArray.Num() );
		unguard;
	}
	friend FArchive& operator<<( FArchive& Ar, FMipmap& M )
	{
		guard(FMipmap<<);
		Ar << M.DataArray;
		Ar << M.USize << M.VSize << M.UBits << M.VBits;
		return Ar;
		unguard;
	}
};

//
// Texture clearing flags.
//
enum ETextureClear
{
	TCLEAR_Temporal	= 1,	// Clear temporal texture effects.
	TCLEAR_Bitmap   = 2,    // Clear the immediate bitmap.
};

enum EEnvMapTransformType
{
	EMTT_ViewSpace		= 0x00,
	EMTT_WorldSpace		= 0x01,
	EMTT_LightSpace		= 0x02,
};

//
//	FStaticTexture
//
class ENGINE_API FStaticTexture : public FTexture
{
private:

	QWORD			CacheId;
	class UTexture*	Texture;
	INT				LastRevision;

public:

	FStaticTexture(UTexture* InTexture);

	virtual QWORD GetCacheId();
	virtual INT GetRevision();

	virtual INT GetWidth();
	virtual INT GetHeight();
	virtual INT GetFirstMip();
	virtual INT GetNumMips();
	virtual ETextureFormat GetFormat();

	virtual ETexClampMode GetUClamp();
	virtual ETexClampMode GetVClamp();

	virtual void* GetRawTextureData(INT MipIndex);
	virtual void UnloadRawTextureData( INT MipIndex );
	virtual void GetTextureData(INT MipIndex,void* Dest,INT DestStride,ETextureFormat DestFormat,UBOOL ColoredMips=0);
	virtual UTexture* GetUTexture();
};


//
//	FStaticCubemap
//
class ENGINE_API FStaticCubemap : public FCubemap
{
private:

	class UCubemap*	Cubemap;
	QWORD			CacheId;
	INT				LastRevision;

public:

	FStaticCubemap(UCubemap* InCubemap);

	virtual QWORD GetCacheId();
	virtual INT GetRevision();

	virtual INT GetWidth();
	virtual INT GetHeight();
	virtual INT GetFirstMip();
	virtual INT GetNumMips();
	virtual ETextureFormat GetFormat();
	virtual ETexClampMode GetUClamp();
	virtual ETexClampMode GetVClamp();

	virtual FTexture* GetFace( INT Face );
};

enum ETextureArithOp
{
	TAO_Assign					= 0,
	TAO_Add						= 1,
	TAO_Subtract				= 2,
	TAO_Multiply				= 3,
	TAO_AssignAlpha				= 4,
	TAO_MultiplyOneMinusAlpha	= 5,
	TAO_AssignLtoR				= 6,
	TAO_AssignLtoG				= 7,
	TAO_AssignLtoB				= 8,
	TAO_AssignLtoA				= 9,
};

//
// A complex material texture.
//
class ENGINE_API UTexture : public UBitmapMaterial
{
	DECLARE_CLASS(UTexture,UBitmapMaterial,CLASS_SafeReplace,Engine)

	UPalette*	Palette;					// Palette if 8-bit palettized.
	UMaterial*	Detail;						// Detail texture to apply.
	FLOAT		DetailScale;				// Detail texture scale.
	FColor		MipZero;					// Overall average color of texture.
	FColor		MaxColor;					// Maximum color for normalization.
	DOUBLE		LastUpdateTime GCC_PACK(4);	// Last time texture was locked for rendering.

	// Deprecated stuff.
	UTexture*	OLDDetailTexture;			// Detail texture to apply.
	UTexture*	OLDEnvironmentMap;			// Environment map.
	BYTE		OLDEnvMapTransformType;		// Transform type for environment map.
	FLOAT		OLDSpecular;				// Diffuse lighting coefficient (0.f-1.f).

	// Flags.
	BITFIELD	bMasked:1 GCC_PACK(4);		// Texture is masked.
	BITFIELD	bAlphaTexture:1;			// Texture is an alphatexture.
	BITFIELD	bTwoSided:1;				// Texture should be rendered two sided when placed directly on a surface.
	BITFIELD	bHighColorQuality:1;		// High color quality hint.
	BITFIELD	bHighTextureQuality:1;		// High color quality hint.
	BITFIELD	bRealtime:1;				// Texture changes in realtime.
	BITFIELD	bParametric:1;				// Texture data need not be stored.
	BITFIELD	bRealtimeChanged:1;			// Changed since last render.

	BITFIELD    OLDbHasComp:1;				//!!OLDVER Compressed version included?
	BYTE        LODSet GCC_PACK(4);			// Level of detail type.
    // gam ---
    INT         NormalLOD, MinLOD, MaxLOD;
    // --- gam

	// Animation related.
	UTexture*	AnimNext;					// Next texture in looped animation sequence.
	UTexture*	AnimCur;					// Current animation frame.
	BYTE		PrimeCount;					// Priming total for algorithmic textures.
	BYTE		PrimeCurrent;				// Priming current for algorithmic textures.
	FLOAT		MinFrameRate;				// Minimum animation rate in fps.
	FLOAT		MaxFrameRate;				// Maximum animation rate in fps.
	INT			TotalFrameNum;
	FLOAT		Accumulator;				// Frame accumulator.

	// Table of mipmaps.
	TArray<FMipmap> Mips;					// Mipmaps in native format.
	BYTE            OLDCompFormat;			//!!OLDVER Compressed texture format.

	// SL: Made this a pointer because the QWORD in the RenderInterface was screwing up alignment on the PS2 somethin' fierce
	FStaticTexture*	RenderInterface;		// The interface used to render this texture.

	// Static.
	static class UClient* __Client;

	// Constructor.
	UTexture();

	// UObject interface.
	void Serialize( FArchive& Ar );
	const TCHAR* Import( const TCHAR* Buffer, const TCHAR* BufferEnd, const TCHAR* FileType );
	void Export( FArchive& Ar, const TCHAR* FileType, INT Indent );
	void PostLoad();
	void Destroy();
	void PostEditChange(); // gam
	
	// UBitmap interface.
	DWORD GetColorsIndex();
	FColor* GetColors();
	INT GetNumMips();
	FMipmapBase* GetMip(INT i);
	FString GetFormatDesc();

	// UTexture interface.
	virtual void Clear(FColor TexelColor);
	virtual void Clear( DWORD ClearFlags );
	virtual void Init(INT InUSize, INT InVSize);
	virtual void Tick(DOUBLE DeltaSeconds);
	virtual void Tick(FLOAT DeltaSeconds );
	virtual void ConstantTimeTick();
	virtual void MousePosition(DWORD Buttons, FLOAT X, FLOAT Y);
	virtual void Click(DWORD Buttons, FLOAT X, FLOAT Y);
	virtual void Update(FTime Time);
	virtual void Prime();
	virtual void ArithOp(UTexture* Operand, ETextureArithOp Operation);
	virtual void TimeReset();

	// UTexture functions.
	FColor GetTexel(FLOAT u1, FLOAT v1, FLOAT u2, FLOAT v2);
	void BuildRemapIndex( UBOOL Masked );
	void CreateMips( UBOOL FullMips, UBOOL Downsample );
	void CreateColorRange();
	UBOOL Compress( ETextureFormat Format, UBOOL Mipmaps = 1, struct FDXTCompressionOptions* Options = NULL );
	UBOOL Decompress( ETextureFormat Format );
	ETextureFormat ConvertDXT();
	ETextureFormat ConvertDXT( INT Miplevel, UBOOL ForceRGBA, UBOOL Use4444, BYTE** Data );
	INT DefaultLOD();

	UBOOL Crest_Compress(INT);
	UBOOL TM_Compress(INT);
	UBOOL TM_Decompress();

	// UBitmapMaterial interface.
	virtual FBaseTexture* GetRenderInterface();
	virtual UBitmapMaterial* Get(FTime Time, UViewport* Viewport);

	// UMaterial interface
	virtual UBOOL RequiresSorting();
	virtual UBOOL IsTransparent();


#if __PSX2_EE__ || __GCN__
	INT			__LastUpdateTime[2];// Last time texture was locked for rendering.
	FTime GetLastUpdateTime() {FTime T; appMemcpy(&T,&__LastUpdateTime,sizeof(FTime)); return T;}
	void SetLastUpdateTime(FTime T) {appMemcpy(&__LastUpdateTime,&T,sizeof(FTime));}
#else
	DOUBLE		__LastUpdateTime GCC_PACK(4);		// Last time texture was locked for rendering.
	DOUBLE GetLastUpdateTime();
	void SetLastUpdateTime(DOUBLE T);
#endif
};

//
// Cubemap.
//
class ENGINE_API UCubemap : public UTexture
{
	DECLARE_CLASS(UCubemap,UTexture,CLASS_SafeReplace,Engine)

	// Constructor.
	UCubemap();

	virtual void Destroy();

	// The six faces of a cubemap.
	UTexture* Faces[6];

	// The interface used to render this texture.
	FStaticCubemap* CubemapRenderInterface;

	virtual FBaseTexture* GetRenderInterface();
};


/*-----------------------------------------------------------------------------
	UFont.
-----------------------------------------------------------------------------*/

struct ENGINE_API FFontCharacter
{
	// Variables.
	INT StartU, StartV;
	INT USize, VSize;
	BYTE TextureIndex;

	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FFontCharacter& Ch )
	{
		guard(FFontCharacter<<);
		return Ar << Ch.StartU << Ch.StartV << Ch.USize << Ch.VSize << Ch.TextureIndex;
		unguard;
	}
};

//
// A font object, containing information about a set of glyphs.
// The glyph bitmaps are stored in the contained textures, while
// the font database only contains the coordinates of the individual
// glyph.
//
class ENGINE_API UFont : public UObject
{
	DECLARE_CLASS(UFont,UObject,0,Engine)

	// Variables.
	TArray<FFontCharacter>	Characters;
	TArray<UTexture*>		Textures;
	
	TMap<TCHAR,TCHAR> CharRemap;
	UBOOL IsRemapped;
    INT Kerning;

	// Constructors.
	UFont();

	// UObject interface.
	void Serialize( FArchive& Ar );

	// UFont interface
	TCHAR RemapChar(TCHAR ch)
	{
		TCHAR *p;
		if( !IsRemapped )
			return ch;
		p = CharRemap.Find(ch);
		return p ? *p : 32; // return space if not found.
	}
};

//
// NVDXT Tool options.
//
struct ENGINE_API FDXTCompressionOptions
{
	UBOOL		BinaryAlpha;
	UBOOL		NormalMap;
	UBOOL		DuDvMap;
	UBOOL		AlphaBorder;
	UBOOL		Border;
	FColor		BorderColor;
	UBOOL		Fade;
	FColor		FadeToColor;
	INT			FadeAmount;
};

/*----------------------------------------------------------------------------
	The End.
----------------------------------------------------------------------------*/

