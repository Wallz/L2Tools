class CameraEffect extends Object
	abstract
	native
	noexport
	noteditinlinenew;

var float	Alpha;			// Used to transition camera effects. 0 = no effect, 1 = full effect
var bool	FinalEffect;	// Forces the renderer to ignore effects on the stack below this one.


// by nonblock.
// 값이 클수록 더 외곽에 있는 이펙트임을 말함. 포스트렌더순서는 nEmbrace가 작은것부터 큰것으로 진행됨.
// 소팅된걸로 가정하고 Render함. CameraEffect에 Add할때 소팅해야함.
// var int		nEmbrace;

//
//	Default properties
//

defaultproperties
{
     Alpha=1.000000
}
