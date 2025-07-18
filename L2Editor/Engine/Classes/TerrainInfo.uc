class TerrainInfo extends Info
	noexport
	showcategories(Movement,Collision,Lighting,LightColor,Karma,Force)
	native
	placeable;

#exec Texture Import File=Textures\Terrain_info.pcx Name=S_TerrainInfo Mips=Off MASKED=1
#exec Texture Import File=Textures\S_WhiteCircle.pcx Name=S_WhiteCircle Mips=Off MASKED=1
#exec Texture Import File=Textures\Bad.pcx Name=TerrainBad Mips=Off
#exec Texture Import File=Textures\DecoPaint.pcx Name=DecoPaint Mips=Off

struct NormalPair
{
	var vector Normal1;
	var vector Normal2;
};

enum ETexMapAxis
{
	TEXMAPAXIS_XY,
	TEXMAPAXIS_XZ,
	TEXMAPAXIS_YZ,
};

enum ESortOrder
{
	SORT_NoSort,
	SORT_BackToFront,
	SORT_FrontToBack
};

struct TerrainLayer
{
	var() Material	Texture;
	var() Texture	AlphaMap;
	var() float		UScale;
	var() float		VScale;
	var() float		UPan;
	var() float		VPan;
	var() ETexMapAxis TextureMapAxis;
	var() float		TextureRotation;
	var() Rotator	LayerRotation;
	var   Matrix	TerrainMatrix;
	var() float		KFriction;
	var() float		KRestitution;
	var   Texture	LayerWeightMap;
//#ifdef	__L2	Hunter
	var	  vector	Scale;
	var	  vector	ToWorld[4];
	var   vector	ToMaskmap[4];
	var() bool		bUseAlpha;
//#endif
};

struct DecorationLayer
{
	var() int			ShowOnTerrain;
	var() Texture		ScaleMap;
	var() Texture		DensityMap;
	var() Texture		ColorMap;
	var() StaticMesh	StaticMesh;
	var() rangevector	ScaleMultiplier;
	var() range			FadeoutRadius;
	var() range			DensityMultiplier;
	var() int			MaxPerQuad;
	var() int			Seed;
	var() int			AlignToTerrain;
	var() ESortOrder	DrawOrder;
	var() int			ShowOnInvisibleTerrain;
	var() int			LitDirectional;
	var() int			DisregardTerrainLighting;
	var() int			RandomYaw;
// __L2, gigadeth
	var() int			bForceRender;
// __L2, gigadeth
};


struct DecoInfo
{
	var vector	Location;
	var rotator	Rotation;
	var vector	Scale;
	var vector	TempScale;
	var color	Color;
	var int		Distance;
// __L2, gigadeth
	var rotator OrigRotation;
	var int     VertexPos;
	var float   Speed;
	var float   Accel;
// __L2, gigadeth
}; 

struct DecoSectorInfo
{
	var array<DecoInfo>	DecoInfo;
	var vector			Location;
	var float			Radius;
// __L2, gigadeth
	var int				bDecoGenerated;
// __L2, gigadeth
};

struct DecorationLayerData
{
	var array<DecoSectorInfo> Sectors;
};

//// __TSHAODW , gigadeth
struct TerrainIntensityMap
{
	var float          Time;
	var array<BYTE>    Intensity;
};
// --

var() int						TerrainSectorSize;
var() Texture					TerrainMap;
var() vector					TerrainScale;
var() TerrainLayer				Layers[32];
var() array<DecorationLayer>	DecoLayers;
var() float						DecoLayerOffset;
var() bool						Inverted;

// This option means use half the graphics res for Karma collision.
// Note - Karma ignores per-quad info (eg. 'invisible' and 'edge-turned') with this set to true.
var() bool						bKCollisionHalfRes;

//
// Internal data
//
var transient int							JustLoaded;
var	native const array<DecorationLayerData> DecoLayerData;
var native const array<TerrainSector>		Sectors;
var native const array<vector>				Vertices;
var native const int						HeightmapX;
var native const int 						HeightmapY;
var native const int 						SectorsX;
var native const int 						SectorsY;
var native const TerrainPrimitive 			Primitive;
var native const array<NormalPair>			FaceNormals;
var native const vector						ToWorld[4];
var native const vector						ToHeightmap[4];
var native const array<int>					SelectedVertices;
var native const int						ShowGrid;
var const array<int>						QuadVisibilityBitmap;
var const array<int>						EdgeTurnBitmap;
var const array<material> QuadDomMaterialBitmap;
var native const array<int>					RenderCombinations;
var native const array<int>					VertexStreams;
var native const array<color>				VertexColors;
var native const array<color>				PaintedColor;		// editor only

// OLD
var native const Texture OldTerrainMap;
var native const array<byte> OldHeightmap;

// __L2 - Hunter
var() int						BaseHeight;
// End
// __L2 && __SW - gigadeth
var int VTGruop;
var int VTGroupOrig;
var int MapX;
var int MapY;
var int bUpdatedHEdge;
var int bUpdatedVEdge;
var int bUpdatedZ;
var array<int> SectorsOrig;
var native const vector ToHeightmapOrig[4];
var const array<int> QuadVisibilityBitmapOrig;
var const array<int> EdgeTurnBitmapOrig;
var int GeneratedSectorCounter;
// End
// __TSHADOW - gigadeth
var int NumIntMap;
var bool bAutoTimeGeneration;
var int NightMapStart;
var int DayMapStart;
var array<TerrainIntensityMap> TIntMap;
var float TickTime;
// End

defaultproperties
{
     TerrainSectorSize=16
     TerrainScale=(X=64.000000,Y=64.000000,Z=64.000000)
     bStatic=True
     bWorldGeometry=True
     Texture=Texture'Engine.S_TerrainInfo'
     bStaticLighting=True
}
