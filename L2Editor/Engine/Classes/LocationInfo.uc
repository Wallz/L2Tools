class LocationInfo extends Info
	showcategories(Movement)
	native
	placeable;

//#exec Texture Import File=Textures\Location_info.pcx Name=S_LocationInfo Mips=Off MASKED=1
#exec Texture Import File=Textures\LockLocation.pcx Name=S_LocationInfo Mips=Off MASKED=1

var()	bool	bMovable;

defaultproperties
{
     bStatic=True
     Texture=Texture'Engine.S_LocationInfo'
     bStaticLighting=True
}
