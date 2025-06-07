class TexPanner extends TexModifier
	editinlinenew
	native;

// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)

var() rotator PanDirection;
var() float PanRate;
//always last member : Matrix M
var Matrix M;

cpptext
{
	// UTexModifier interface
	virtual FMatrix* GetMatrix(FLOAT TimeSeconds);
}


defaultproperties
{
     PanRate=0.100000
}
