

//
// Forward declarations
//
class	UMaterial;
class		URenderedMaterial;
class			UBitmapMaterial;
class				UTexture;

class	UModifier;

class FTexture;
class FCubemap;


//
// EFillMode
//
enum EFillMode
{
	FM_Wireframe	= 0,
	FM_FlatShaded	= 1,
	FM_Solid		= 2,
};

/*-----------------------------------------------------------------------------
	UMaterial.
-----------------------------------------------------------------------------*/

class ENGINE_API UMaterial : public UObject
{
	DECLARE_ABSTRACT_CLASS(UMaterial,UObject,0,Engine)

	UMaterial*	FallbackMaterial;
	UMaterial*	DefaultMaterial;
	BITFIELD	UseFallback:1		GCC_PACK(4);
    BITFIELD	Validated:1;
	INT			Reserved;

	BYTE			TextureTranform;
	BYTE			MAX_SAMPLER_NUM;
	BYTE			MAX_TEXMAT_NUM;
	BYTE			MAX_PASS_NUM;
	BYTE			TwoPassRenderState;
	BYTE			AlphaRef;
	INT				SrcBlend;
	INT				DestBlend;
	INT				OverriddenFogColor;
	FMatrix			matTexMatrix[16];
	INT				FC_Color1;
	INT				FC_Color2;
	INT				FC_FadePeriod;
	INT				FC_FadePhase;
	INT				FC_ColorFadeType;
	INT				BitmapMaterials[16];
	FStringNoInit	strTex[16];
	FStringNoInit	ShaderCode;

	// Constructor.
	UMaterial();

	// UObject interface
	void Serialize( FArchive& Ar );

	// UMaterial interface
	virtual UBOOL CheckCircularReferences( TArray<UMaterial*>& History );
	virtual UBOOL GetValidated();
	virtual void SetValidated(UBOOL InValidated);
	virtual void PreSetMaterial(FLOAT TimeSeconds);

	// Getting information about a combined material:
	virtual INT MaterialUSize();
	virtual INT MaterialVSize();
	virtual INT MaterialMappingUSize();
	virtual INT MaterialMappingVSize();
	virtual UBOOL RequiresSorting();
	virtual UBOOL RequiresSortingEx(FDynamicActor*);
	virtual INT GetSortingLevel();
	virtual UBOOL IsTransparent();
	virtual BYTE RequiredUVStreams();
	virtual UBOOL RequiresNormal();

	// Fallback handling
	static void ClearFallbacks();
	virtual UMaterial* CheckFallback();
	virtual UBOOL HasFallback();

	//!! OLDVER
	UMaterial* ConvertPolyFlagsToMaterial( UMaterial* InMaterial, DWORD InPolyFlags );

	DECLARE_FUNCTION(execMaterialUSize)
	DECLARE_FUNCTION(execMaterialVSize)
};

/*-----------------------------------------------------------------------------
	URenderedMaterial.
-----------------------------------------------------------------------------*/

class ENGINE_API URenderedMaterial : public UMaterial
{
	DECLARE_ABSTRACT_CLASS(URenderedMaterial,UMaterial,0,Engine)
};

/*-----------------------------------------------------------------------------
	UBitmapMaterial.
-----------------------------------------------------------------------------*/
class ENGINE_API UBitmapMaterial : public URenderedMaterial
{
	DECLARE_ABSTRACT_CLASS(UBitmapMaterial,URenderedMaterial,0,Engine)

	BYTE		Format;				// ETextureFormat.
	BYTE		UClampMode;			// Texture U clamp mode
	BYTE		VClampMode;			// Texture V clamp mode

	BYTE		UBits, VBits;		// # of bits in USize, i.e. 8 for 256.
	INT			USize, VSize;		// Size, must be power of 2.
	INT			UClamp, VClamp;		// Clamped width, must be <= size.
	INT			LossDetail;


	// UBitmapMaterial interface.
	virtual FBaseTexture* GetRenderInterface() = 0;
	virtual UBitmapMaterial* Get(FTime Time, UViewport* Viewport);

	// UMaterial Interface
	virtual INT MaterialUSize();
	virtual INT MaterialVSize();
};

/*-----------------------------------------------------------------------------
	UProxyBitmapMaterial
-----------------------------------------------------------------------------*/
class ENGINE_API UProxyBitmapMaterial : public UBitmapMaterial
{
	DECLARE_CLASS(UProxyBitmapMaterial,UBitmapMaterial,0,Engine);

private:

	FBaseTexture*	TextureInterface;

public:

	// UProxyBitmapMaterial interface.
	void SetTextureInterface(FBaseTexture* InTextureInterface)
	{
		TextureInterface = InTextureInterface;
		Format = TextureInterface->GetFormat();
		UClampMode = TextureInterface->GetUClamp();
		VClampMode = TextureInterface->GetVClamp();
		UClamp = USize = TextureInterface->GetWidth();
		VClamp = VSize = TextureInterface->GetHeight();
		UBits = appCeilLogTwo(UClamp);
		VBits = appCeilLogTwo(VClamp);
	}

	// UBitmapMaterial interface.
	virtual FBaseTexture* GetRenderInterface() { return TextureInterface; }
	virtual UBitmapMaterial* Get( FTime Time, UViewport* Viewport ) { return this; }
};

/*-----------------------------------------------------------------------------
	UTexCoordMaterial
-----------------------------------------------------------------------------*/
class ENGINE_API UTexCoordMaterial : public URenderedMaterial
{
    DECLARE_CLASS(UTexCoordMaterial,URenderedMaterial,0,Engine)

	class UBitmapMaterial* Texture;
    class UTexCoordGen* TextureCoords;

	// UMaterial interface
	virtual INT MaterialUSize() { return Texture ? Texture->MaterialUSize() : 0; }
	virtual INT MaterialVSize() { return Texture ? Texture->MaterialVSize() : 0; }
};

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/


