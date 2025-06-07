class BumpShader extends RenderedMaterial
	noexport
	editinlinenew
	native;

var() editinlineuse Material Diffuse;
var() editinlineuse Material BumpMaterial;
var	Material OldBumpMaterial;

var	const transient byte BumpRawData;
var const transient int	BumpTexInterface;


function Reset()
{
}

function Trigger( Actor Other, Actor EventInstigator )
{
}

defaultproperties
{
}
