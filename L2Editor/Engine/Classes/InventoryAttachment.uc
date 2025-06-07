class InventoryAttachment extends Actor
	native
	nativereplication;

function InitFor(Inventory I)
{
	Instigator = I.Instigator;
}
		

defaultproperties
{
     DrawType=DT_Mesh
     bOnlyDrawIfAttached=True
     bOnlyDirtyReplication=True
     RemoteRole=ROLE_SimulatedProxy
     NetUpdateFrequency=10.000000
     bUseLightingFromBase=True
}
