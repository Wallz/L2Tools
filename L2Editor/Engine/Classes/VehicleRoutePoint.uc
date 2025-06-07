class VehicleRoutePoint extends Vehicle
	native;

var()	string				RouteName;
var()	array<vector>		DeltaPoint;
var()	array<vector>		AbsPoint;
var()	array<int>			MovingSpeed;
var()	array<int>			RotatingSpeed;
var()	array<int>			TimeToNextAction;
var()	array<int>			SpeakerID;
var()	array<int>			WaitingMessageID;
var()	array<int>			StationID;
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
	typedef TMap<int, TArray<FVector> > VehicleRoutePointPathMapType;
	VehicleRoutePointPathMapType* GetPaths() { return (VehicleRoutePointPathMapType*)Paths; }
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
     DrawType=DT_StaticMesh
}
