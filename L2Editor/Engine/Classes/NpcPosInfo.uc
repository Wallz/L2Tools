class NpcPosInfo extends Info
	showcategories(Movement)
	native
	placeable;

#exec Texture Import File=Textures\Territory_info.pcx Name=S_TerritoryInfo Mips=Off MASKED=1

struct NpcPos
{
	var() vector Delta;
	var() int	Yaw;
	var() int Percent;
};

var()	array<NpcPos>	Pos;
var()	color	LineColor;
var()	name	NpcName;
var()	string	nickname;
var()	name	ai;
var()	array<NpcPrivate> Privates;
var()	WhenExtinctionCreate when_extinction_create;
var()	bool	bWayPointsShow;
var()	array<WayPoint> WayPoints;

defaultproperties
{
     LineColor=(B=255,G=255,R=10,A=255)
     bStatic=True
     Texture=Texture'Engine.S_TerritoryInfo'
     bStaticLighting=True
}
