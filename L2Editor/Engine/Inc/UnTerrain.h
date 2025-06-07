/*=============================================================================
	UnTerrain.h: Unreal terrain objects.
	Copyright 1997-2000 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Jack Porter
=============================================================================*/
/*------------------------------------------------------------------------------
    Misc stuff.
------------------------------------------------------------------------------*/

struct FTerrainVertex
{
	FVector	Position,
			Normal;
	FColor	Color;
	FLOAT	U,
			V;

	ENGINE_API friend FArchive& operator<<(FArchive& Ar, FTerrainVertex& V);
};

class FTerrainVertexStream : public FVertexStream
{
public:

	TArray<FTerrainVertex>	Vertices;
	QWORD					CacheId;
	INT						Revision;

	// Constructor.
	FTerrainVertexStream();

	virtual QWORD GetCacheId();
	virtual INT GetRevision();
	virtual INT GetSize();
	virtual INT GetStride();
	virtual INT GetComponents(FVertexComponent* OutComponents);
	virtual void GetStreamData(void* Dest);
	virtual void GetRawStreamData(void ** Dest, INT FirstVertex);

	// Serializer.
	friend ENGINE_API FArchive& operator<<(FArchive& Ar, FTerrainVertexStream& VertexStream);
};

struct FTerrainNormalPair
{
	FVector Normal1;
	FVector Normal2;

	friend FArchive& operator<<(FArchive& Ar, FTerrainNormalPair& N);
};

enum
{
	TEXMAPAXIS_XY	= 0,
	TEXMAPAXIS_XZ	= 1,
	TEXMAPAXIS_YZ	= 2,
	TEXMAPAXIS_MAX	= 3,
};


enum ESortOrder
{
	SORT_NoSort		= 0,
	SORT_BackToFront= 1,
	SORT_FrontToBack= 2,
};

struct FTerrainLayer
{
	UMaterial*		Texture;
	UTexture*		AlphaMap;
	FLOAT			UScale;
	FLOAT			VScale;
	FLOAT			UPan;
	FLOAT			VPan;
	BYTE			TextureMapAxis;
	FLOAT			TextureRotation;
	FRotator		LayerRotation;
	FMatrix			TextureMatrix;
	FLOAT			KFriction;
	FLOAT			KRestitution;
	UTexture*		LayerWeightMap;	// layer weight map (precomputed from alphamap)
	FVector			Scale;
	FVector			ToWorld[4];
	FVector			ToMaskmap[4];
	UBOOL			bUseAlpha;
};

struct FDecorationLayer
{
	UBOOL			ShowOnTerrain;
	UTexture*		ScaleMap;
	UTexture*		DensityMap;
	UTexture*		ColorMap;
	UStaticMesh*	StaticMesh;
	FRangeVector	ScaleMultiplier;
	FRange			FadeoutRadius;
	FRange			DensityMultiplier;
	INT				MaxPerQuad;
	INT				Seed;
	UBOOL			AlignToTerrain;
	BYTE			DrawOrder;
	UBOOL			ShowOnInvisibleTerrain;
	UBOOL			LitDirectional;
	UBOOL			DisregardTerrainLighting;
	UBOOL			RandomYaw;
	UBOOL			bForceRender;
};

struct FDecoInfo
{
	FVector		Location;
	FRotator	Rotation;
	FVector		Scale;
	FVector		TempScale;
	FColor		Color;
	INT			Distance;
	FRotator	OrigRotation;
	INT			VertexPos;
	FLOAT		Speed;
	FLOAT		Accel;

	friend FArchive& operator<<(FArchive& Ar, FDecoInfo& DI);
}; 

struct FDecoSectorInfo
{
	TArray<FDecoInfo>			DecoInfo;
	FVector						Location;
	FLOAT						Radius;
	UBOOL						bDecoGenerated;

	friend FArchive& operator<<(FArchive& Ar, FDecoSectorInfo& DSI);
};

struct FDecorationLayerData
{
	TArray<FDecoSectorInfo>		Sectors;

	friend FArchive& operator<<(FArchive& Ar, FDecorationLayerData& DLD);
};

enum ETerrainRenderMethod
{
	RM_WeightMap			= 0,
	RM_CombinedWeightMap	= 1,
	RM_AlphaMap				= 2,
};

//
// Combination of layers, etc.
//
struct FTerrainRenderCombination
{
	TArray<INT> Layers;
	ETerrainRenderMethod RenderMethod;
	UTexture* CombinedWeightMaps;

	friend FArchive& operator<<(FArchive& Ar, FTerrainRenderCombination& C);
};


//
// Sector info for terrain layers rendered in a single pass.
//
struct FTerrainSectorRenderPass
{
	ATerrainInfo* TerrainInfo;
	INT RenderCombinationNum;

	TArray<_WORD> Indices;
	INT NumTriangles;
	INT NumIndices;
	DWORD MinIndex;
	DWORD MaxIndex;

	FTerrainSectorRenderPass()
	:	RenderCombinationNum(-1)
	,	NumTriangles		(0)
	,	NumIndices			(0)
	,	MinIndex			(0)
	,	MaxIndex			(0)
	{}

	inline FTerrainRenderCombination* GetRenderCombination(ATerrainInfo* InTerrainInfo);

	friend FArchive& operator<<(FArchive& Ar, FTerrainSectorRenderPass& P);
};

struct FTerrainSectorLightInfo
{
	AActor*			LightActor;
	TArray<BYTE>	VisibilityBitmap;

	FTerrainSectorLightInfo() {}

	FTerrainSectorLightInfo( AActor* InActor )
		: LightActor(InActor) {}

	friend FArchive& operator<<( FArchive& Ar, FTerrainSectorLightInfo& I )
	{
		return Ar << I.LightActor << I.VisibilityBitmap;
	}
};

/*------------------------------------------------------------------------------
    UTerrainSector.
------------------------------------------------------------------------------*/

class ENGINE_API UTerrainSector : public UObject
{
	DECLARE_CLASS(UTerrainSector,UObject,0,Engine);
	NO_DEFAULT_CONSTRUCTOR(UTerrainSector)

	TArray<FTerrainSectorRenderPass> RenderPasses;
	TArray<FTerrainSectorLightInfo> LightInfos;
	ATerrainInfo* Info;
	FBox BoundingBox;			// Bounding box
	INT QuadsX, QuadsY;			// Dimensions
	INT OffsetX, OffsetY;
	FVector Location;			// Center
	FLOAT	Radius;				// Radius
	TArray<FStaticProjectorInfo*> Projectors;

	INT VertexStreamNum;
	_WORD VertexStreamOffset;

	FRawIndexBuffer CompleteIndexBuffer;
	INT CompleteNumTriangles;
	INT CompleteNumIndices;
	DWORD CompleteMinIndex;
	DWORD CompleteMaxIndex;

	BYTE Pad00[160]; //Doesn't check

#ifdef __PSX2_EE__
	// PS2 specific data for this patch
	void* PS2Data;
#endif

	// Constructor.
	UTerrainSector( ATerrainInfo* InInfo, INT InOffsetX, INT InOffsetY, INT QuadsX, INT QuadsY );

	// UObject interface.
	virtual void Serialize(FArchive& Ar);
	virtual void PostLoad();
	virtual void Destroy();

	// UTerrainSector interface
	void GenerateTriangles();
	void StaticLight( UBOOL Force );
#ifdef _XBOX
	INT GetGlobalVertex( INT x, INT y );
#else
	inline INT GetGlobalVertex( INT x, INT y );
#endif
	INT GetLocalVertex( INT x, INT y ) { return x + y*(QuadsX+1); }
	inline UBOOL IsTriangleAll( INT Layer, INT X, INT Y, INT Tri, UBOOL Turned, BYTE AlphaValue );
	UBOOL IsSectorAll( INT Layer, BYTE AlphaValue );
	UBOOL PassShouldRenderTriangle( INT Pass, INT X, INT Y, INT Tri, UBOOL Turned );
		

	// Projectors
	void AttachProjector( AProjector* InProjector, FProjectorRenderInfo* InRenderInfo, INT MinQuadX, INT MinQuadY, INT MaxQuadX, INT MaxQuadY );
};

/*------------------------------------------------------------------------------
    UTerrainPrimitive.
------------------------------------------------------------------------------*/

class ENGINE_API UTerrainPrimitive : public UPrimitive
{
	DECLARE_CLASS(UTerrainPrimitive,UPrimitive,0,Engine);
	NO_DEFAULT_CONSTRUCTOR(UTerrainPrimitive)

	ATerrainInfo* Info;

	// Constructor
	UTerrainPrimitive( ATerrainInfo* InInfo )
		: Info( InInfo ) {}

	// UPrimitive interface
	UBOOL LineCheck(FCheckResult &Result,AActor* Owner,FVector End,FVector Start,FVector Extent,DWORD ExtraNodeFlags,DWORD TraceFlags);
	UBOOL PointCheck(FCheckResult& Result,AActor* Owner,FVector Point,FVector Extent,DWORD ExtraNodeFlags);
	FBox GetRenderBoundingBox( const AActor* Owner, UBOOL Exact );
	void Illuminate(AActor* Owner,UBOOL ChangedOnly);

	// UObject interface.
	virtual void Serialize(FArchive& Ar);
};

/*------------------------------------------------------------------------------
    ATerrainInfo.
------------------------------------------------------------------------------*/

struct FSelectedTerrainVertex
{
	INT X, Y;
	INT OldHeight;
	FLOAT Weight, Delta;

	friend FArchive& operator<<(FArchive& Ar, FSelectedTerrainVertex& V);
};

class ENGINE_API ATerrainInfo : public AInfo
{
    DECLARE_CLASS(ATerrainInfo,AInfo,0,Engine)

	// Editor Properties.
	INT							TerrainSectorSize;
	class UTexture*				TerrainMap;
    FVector						TerrainScale;
	FTerrainLayer				Layers[32];
	TArray<FDecorationLayer>	DecoLayers;
	FLOAT						DecoLayerOffset;
    BITFIELD					Inverted:1		GCC_PACK(4);
    BITFIELD					bKCollisionHalfRes:1;

	// Variables.
	UBOOL						JustLoaded		GCC_PACK(4);
	TArray<FDecorationLayerData>DecoLayerData;
	TArray<UTerrainSector*>		Sectors;
	TArray<FVector>				VerticesIvar;
	INT							HeightmapX;
	INT							HeightmapY;
	INT							SectorsX;
	INT							SectorsY;
	UTerrainPrimitive*			Primitive;
	
	TArray<FTerrainNormalPair>	FaceNormals;
	FCoords						ToWorld;
	FCoords						ToHeightmap;
	TArray<FSelectedTerrainVertex>		SelectedVertices;
	INT							ShowGrid;
	TArray<DWORD>				QuadVisibilityBitmap;
	TArray<DWORD>				EdgeTurnBitmap;
	TArray<UMaterial*>			QuadDomMaterialBitmap; // Dominant material of each quad.
	TArray<FTerrainRenderCombination> RenderCombinations;
	TArray<FTerrainVertexStream> VertexStreams;
	TArray<FColor>				VertexColors;
	TArray<FColor>				PaintedColor;		// editor only

	// Deprecated.
	class UTexture*				OldTerrainMap;
	TArray<_WORD>				OldHeightmap;

	INT BaseHeight;
	INT VTGruop;
	INT VTGroupOrig;
	INT MapX;
	INT MapY;
	INT bUpdatedHEdge;
	INT bUpdatedVEdge;
	INT bUpdatedZ;
	TArrayNoInit<INT> SectorsOrig;
	FVector ToHeightmapOrig[4];
	TArrayNoInit<INT> QuadVisibilityBitmapOrig;
	TArrayNoInit<INT> EdgeTurnBitmapOrig;
	INT GeneratedSectorCounter;
	INT NumIntMap;
	BITFIELD bAutoTimeGeneration : 1 GCC_PACK(4);
	INT NightMapStart GCC_PACK(4);
	INT DayMapStart;
	TArrayNoInit<FTerrainIntensityMap> TIntMap;
	FLOAT TickTime;
	
	// Constructors.
	ATerrainInfo(); 

	// UObject Interface
	virtual void PostEditChange();
	virtual void Serialize(FArchive& Ar);
	virtual void PostLoad();
	virtual void Destroy();

	// AActor Interface.
	virtual UPrimitive* GetPrimitive();
	virtual void CheckForErrors();

	// ATerrainInfo Interface.
	void SetupSectors();
	void Update( FLOAT Time, INT StartX=0, INT StartY=0, INT EndX=0, INT EndY=0, UBOOL UpdateLighting=0 );

	inline INT GetGlobalVertex( INT x, INT y );

	void Render(FLevelSceneNode* SceneNode,FRenderInterface* RI, FVisibilityInterface* VI, FDynamicLight** DynamicLights, INT NumDynamicLights, FProjectorRenderInfo** DynamicProjectors, INT NumDynamicProjectors); // sjs

	// DecoLayers.
	void UpdateDecorations( INT SectorIndex );
	void RenderDecorations(FLevelSceneNode* SceneNode,FRenderInterface* RI,FVisibilityInterface* VI);	

	// Collision
	UBOOL LineCheck( FCheckResult &Result, FVector End, FVector Start, FVector Extent, DWORD TraceFlags, UBOOL CheckInvisibleQuads );
	UBOOL LineCheckWithQuad( INT X, INT Y, FCheckResult &Result, FVector End, FVector Start, FVector Extent, DWORD TraceFlags, UBOOL CheckInvisibleQuads );

	UBOOL PointCheck( FCheckResult &Result, FVector Point, FVector Extent, UBOOL CheckInvisibleQuads=0 );

	// Editor
	UBOOL GetClosestVertex( FVector& InLocation, FVector* InOutput, INT* InX, INT* InY );
	UBOOL SelectVertexX( INT InX, INT InY );
	UBOOL SelectVertex( FVector Location );
	void ConvertHeightmapFormat();
	void SoftSelect( FLOAT InnerRadius, FLOAT OuterRadius );
	void SoftDeselect();
	void SelectVerticesInBox( FBox& InRange );
	void MoveVertices( FLOAT Delta );
	void ResetMove();
	FBox GetSelectedVerticesBounds();
	void UpdateFromSelectedVertices();

	// internal
	void CheckComputeDataOnLoad();
	INT GetRenderCombinationNum( TArray<INT>& Layers, ETerrainRenderMethod RenderMethod );
	void UpdateVertices( FLOAT Time, INT StartX, INT StartY, INT EndX, INT EndY );
	void UpdateTriangles( INT StartX, INT StartY, INT EndX, INT EndY, UBOOL UpdateLighting=0 );
	void CalcCoords();
	void CalcLayerTexCoords();
	FVector GetVertexNormal( INT x, INT y ); 
	void PrecomputeLayerWeights();
	void CombineLayerWeights();

	inline BYTE GetLayerAlpha( INT x, INT y, INT Layer, UTexture* InAlphaMap = NULL );
	inline void SetLayerAlpha( FLOAT x, FLOAT y, INT Layer, BYTE Alpha, UTexture* InAlphaMap = NULL );
	inline _WORD GetHeightmap( INT x, INT y );
	inline void SetHeightmap( INT x, INT y, _WORD w );
	inline FColor GetTextureColor( INT x, INT y, UTexture* Texture );
	inline void SetTextureColor( INT x, INT y, UTexture* Texture, FColor& Color );
	
	UBOOL GetQuadVisibilityBitmap(INT x, INT y);
	void SetQuadVisibilityBitmap(INT x, INT y, UBOOL Visible);
	UBOOL GetEdgeTurnBitmap(INT x, INT y);
	void SetEdgeTurnBitmap(INT x, INT y, UBOOL Turned);
	UMaterial* GetQuadDomMaterialBitmap(INT x, INT y);

	FVector HeightmapToWorld(FVector H);
	FVector WorldToHeightmap(FVector W);

	// L2
	void UpdateShadow(INT*, INT);
	void UpdateShadow(UTerrainSector**, INT);
	void UpdateVTGroup();
	FCoords GetLayerToMap(INT);
	FLOAT GetLayerXScaleToHeightmap(INT);
	FLOAT GetLayerYScaleToHeightmap(INT);
	FVector GetSafeDirectionForIntMap(INT);
	FVector MakeVertex2DWithLayerPos(INT, INT, INT);
	FVector Vertices(INT);
	FVector Vertices(INT, INT);
	INT GetClosestTilePos(INT, FVector&, FVector*, INT*, INT*);
	INT GetDecoLayerAlpha(FVector, INT, FLOAT);
	INT GetEdgeTurnBitmapOrig(INT, INT);
	INT GetLayerHeight(INT);
	INT GetLayerWidth(INT);
	INT GetQuadVisibilityBitmapOrig(INT, INT);
	INT GetShadowMapIndex(FLOAT);
	unsigned char GetLayerAlphaWithMaskPos(INT, INT, INT, UTexture*);
	void ClearDecorations();
	void ClearTerrain();
	void InterpolateGlobalIntensityMap(struct FTerrainIntensityMap&, struct FTerrainIntensityMap const&, struct FTerrainIntensityMap const&, FLOAT, FLOAT);
	void MakeIntensityMap();
	void RenderEditorSWTerrain(FLevelSceneNode*, FRenderInterface*, FVisibilityInterface*);
	void SWUpdate(FLOAT, INT, INT, INT, INT, INT);
	void SWUpdateTriangles(INT, INT, INT, INT, INT);
	void SetEditorSWTerrain(INT);
	void SetEndVertexZ(ATerrainInfo*);
	void SetHoriEdge(ATerrainInfo*);
	void SetLayerAlphaWithMaskPos(INT, INT, INT, unsigned char);
	void SetTimeForIndex(INT);
	void SetVertiEdge(ATerrainInfo*);
	void SoftSelectNeighbor(ATerrainInfo*, FLOAT, FLOAT);
	void ToggleShowMapBug();
};

/*----------------------------------------------------------------------------
	The End.
----------------------------------------------------------------------------*/

