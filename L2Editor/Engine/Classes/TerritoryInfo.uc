class TerritoryInfo extends Info
	showcategories(Movement)
	native
	placeable;

#exec Texture Import File=Textures\Territory_info.pcx Name=S_TerritoryInfo Mips=Off MASKED=1

enum InitalSpawn
{
	all,
	random1,
	random2
};

struct AnywhereNpc
{
	var() name NpcName;
	var() int	total;
	var() string respawn;
	var() string nickname;
	var() name ai;
	var() array<NpcPrivate> Privates;
	var() WhenExtinctionCreate when_extinction_create;
	var() bool bWayPointsShow;
	var() array<WayPoint> WayPoints;
};

struct AnywhereNpcMaker
{
	var() bool bGroup;
	var() InitalSpawn inital_spawn;
	var() int maximum_npc;
	var() array<AnywhereNpc> Npc;
};

var()	string	TerritoryName;
var()	int		PointNum;
var()	float	TerritoryHeight;
var()	vector	DeltaPoint[64];
var()	color	LineColor;
var()	array<AnywhereNpcMaker> npcmaker;

defaultproperties
{
     PointNum=4
     TerritoryHeight=200.000000
     DeltaPoint(0)=(X=-100.000000,Y=-100.000000)
     DeltaPoint(1)=(X=100.000000,Y=-100.000000)
     DeltaPoint(2)=(X=100.000000,Y=100.000000)
     DeltaPoint(3)=(X=-100.000000,Y=100.000000)
     LineColor=(B=255,G=10,R=10,A=255)
     bAlwaysVisible=True
     bStatic=True
     Texture=Texture'Engine.S_TerritoryInfo'
     bStaticLighting=True
}
