class ServerPrimitive extends Info
	showcategories(Movement)
	native
	placeable;

#exec Texture Import File=Textures\ServerPrimitive_info.pcx Name=S_ServerPrimitive Mips=Off MASKED=1

struct ServerPointStruct
{
	var() string Name;
	var() vector Point;
	var() plane Color;
};

struct ServerLineStruct
{
	var() string Name;
	var() vector Start;
	var() vector End;
	var() plane Color;
};

var()	array<ServerPointStruct>	PointArray;
var()	array<ServerLineStruct>		LineArray;
var()	color						LineColor;
var()	string						Name;

defaultproperties
{
     LineColor=(B=255,G=10,R=10,A=255)
     bAlwaysVisible=True
     bHidden=False
     bDisableSorting=True
     Texture=Texture'Engine.S_ServerPrimitive'
     bStaticLighting=True
     CollisionRadius=0.000000
     CollisionHeight=0.000000
}
