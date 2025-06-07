class CameraEffect extends Object
	abstract
	native
	noexport
	noteditinlinenew;

var float	Alpha;			// Used to transition camera effects. 0 = no effect, 1 = full effect
var bool	FinalEffect;	// Forces the renderer to ignore effects on the stack below this one.


// by nonblock.
// ���� Ŭ���� �� �ܰ��� �ִ� ����Ʈ���� ����. ����Ʈ���������� nEmbrace�� �����ͺ��� ū������ �����.
// ���õȰɷ� �����ϰ� Render��. CameraEffect�� Add�Ҷ� �����ؾ���.
// var int		nEmbrace;

//
//	Default properties
//

defaultproperties
{
     Alpha=1.000000
}
