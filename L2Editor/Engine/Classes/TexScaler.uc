class TexScaler extends TexModifier
	editinlinenew
	native;

// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)

var() float UScale;
var() float VScale;
var() float UOffset;
var() float VOffset;

//always last member : Matrix M
var Matrix M;

cpptext
{
	// UTexModifier interface
	virtual FMatrix* GetMatrix(FLOAT TimeSeconds);
}


defaultproperties
{
     UScale=1.000000
     VScale=1.000000
}
