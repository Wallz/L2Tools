#exec Texture Import File=Textures\S_KVehFact.pcx Name=S_KVehFact Mips=Off MASKED=1

//=============================================================================
// KVehicle spawner location.
//=============================================================================
class KVehicleFactory extends Actor 
	placeable;

var()	class<KVehicle>		VehicleClass;
var()	int					MaxVehicleCount;

var		int					VehicleCount;

event Trigger( Actor Other, Pawn EventInstigator )
{
	local KVehicle CreatedVehicle;

	if(VehicleCount >= MaxVehicleCount)
		return;

	if(VehicleClass != None)
	{
		CreatedVehicle = spawn(VehicleClass, , , Location, Rotation);
		VehicleCount++;
		CreatedVehicle.ParentFactory = self;
	}
}

defaultproperties
{
     bHidden=True
     bNoDelete=True
     RemoteRole=ROLE_None
     Texture=Texture'Engine.S_KVehFact'
     bDirectional=True
}
