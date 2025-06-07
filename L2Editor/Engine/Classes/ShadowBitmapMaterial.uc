class ShadowBitmapMaterial extends BitmapMaterial
	native;



#exec Texture Import file=Textures\L2DefaultShadow.tga Name=BlobTexture ALPHATEXTURE=1 Mips=On UCLAMPMODE=CLAMP VCLAMPMODE=CLAMP DXT=3
//#exec Texture Import file=Textures\blobshadow.tga Name=BlobTexture Mips=On UCLAMPMODE=CLAMP VCLAMPMODE=CLAMP DXT=3
//#exec Texture Import file=Textures\defaultshadow.tga Name=BlobTexture Mips=On UCLAMPMODE=CLAMP VCLAMPMODE=CLAMP DXT=3

var const transient int	TextureInterfaces[2];
//#ifdef __L2 zodiac 
var const transient int ShadowInterface;
//#endif

var Actor	ShadowActor;
var vector	LightDirection;
var float	LightDistance,
			LightFOV;
var bool	Dirty,
			Invalid,
			bBlobShadow;
var float   CullDistance;
var byte	ShadowDarkness;

var BitmapMaterial	BlobShadow;

// ifdef __L2 zodiac
var bool    bDefaultShadow;
var int		ShadowIndex;
var int		ShadowTextureRevision;
native final function bool Destroy();
// endif

// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)

//
//	Default properties
//

cpptext
{
	virtual void Destroy();

	virtual FBaseTexture* GetRenderInterface();
	virtual UBitmapMaterial* Get(FTime Time,UViewport* Viewport);
}


defaultproperties
{
     Dirty=True
     ShadowDarkness=255
     BlobShadow=Texture'Engine.BlobTexture'
     ShadowIndex=-1
     ShadowTextureRevision=-1
     Format=TEXF_RGBA8
     UClampMode=TC_Clamp
     VClampMode=TC_Clamp
     UBits=7
     VBits=7
     USize=256
     VSize=256
     UClamp=256
     VClamp=256
}
