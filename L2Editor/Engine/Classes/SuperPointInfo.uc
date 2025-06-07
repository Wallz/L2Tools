class SuperPointInfo extends Info
	showcategories(Movement)
	native
	placeable;

#exec Texture Import File=Textures\Territory_info.pcx Name=S_SuperPointInfo Mips=Off MASKED=1


enum SuperPointMoveType
{
	Follow_Rail,
	Move_Random
};

var()	string				SuperPointName;
var()	SuperPointMoveType	MoveType;
var()	array<vector>		DeltaPoint;
var()	array<vector>		AbsPoint;
var()	array<int>			Delay;
var()	color				LineColor;
var()	color				PathColor;
var()	color				FontColor;
var		int					Paths;

// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)

cpptext
{
	typedef TMap<int, TArray<FVector> > SuperPointPathMapType;
	SuperPointPathMapType* GetPaths() { return (SuperPointPathMapType*)Paths; }
	virtual void PostEditChange() 
	{
		for (int i=0; i<AbsPoint.Num(); i++)
		{
			DeltaPoint(i) = AbsPoint(i) - Location;
		}
	}
}


defaultproperties
{
     LineColor=(B=255,G=10,R=10,A=255)
     PathColor=(B=10,G=255,R=10,A=255)
     FontColor=(B=10,G=255,R=10,A=255)
     bAlwaysVisible=True
     bStatic=True
     Texture=Texture'Engine.S_SuperPointInfo'
     bStaticLighting=True
}
