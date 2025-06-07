class EnvCreatureManagerInfo extends Info
	abstract
	placeable
	native;
	
#exec Texture Import File=..\engine\Textures\Ambientcreatures.pcx Name=S_AmbCreature Mips=Off MASKED=1

defaultproperties
{
     Texture=Texture'Engine.S_AmbCreature'
}
