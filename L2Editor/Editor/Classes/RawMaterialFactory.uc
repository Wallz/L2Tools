class RawMaterialFactory extends MaterialFactory;

var() Class<Material> MaterialClass;

function Material CreateMaterial( Object InOuter, string InPackage, string InGroup, string InName )
{		
	if( MaterialClass == None )
		return None;

	return New(InOuter, InName, 0x00080004) MaterialClass; //RF_Public+RF_Standalone
}

defaultproperties
{
	MaterialClass=class'Shader';
	Description="Raw Material"
}