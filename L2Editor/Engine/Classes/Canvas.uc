//=============================================================================
// Canvas: A drawing canvas.
// This is a built-in Unreal class and it shouldn't be modified.
//
// Notes.
//   To determine size of a drawable object, set Style to STY_None,
//   remember CurX, draw the thing, then inspect CurX and CurYL.
//=============================================================================
class Canvas extends Object
	native
	noexport;


// simple default font, so various stuff doesn't crash
#exec Font Import File=Textures\SmallFont.bmp Name="DefaultFont"

// Modifiable properties.
var font    Font;            // Font for DrawText.
var float   SpaceX, SpaceY;  // Spacing for after Draw*.
var float   OrgX, OrgY;      // Origin for drawing.
var float   ClipX, ClipY;    // Bottom right clipping region.
var float   CurX, CurY;      // Current position for drawing.
var float   Z;               // Z location. 1=no screenflash, 2=yes screenflash.
var byte    Style;           // Drawing style STY_None means don't draw.
var float   CurYL;           // Largest Y size since DrawText.
var color   DrawColor;       // Color for drawing.
var bool    bCenter;         // Whether to center the text.
var bool    bNoSmooth;       // Don't bilinear filter.
var const int SizeX, SizeY;  // Zero-based actual dimensions.

// Stock fonts.
var font TinyFont, SmallFont, MedFont;
var string TinyFontName, SmallFontName, MedFontName;

// Internal.
var const viewport Viewport; // Viewport that owns the canvas.
var const int      pCanvasUtil; 
var texture pNormalLeftTex;
var texture pNormalRightTex;
var texture pTargetLeftTex;
var texture pTargetRightTex;
var texture pAttackLeftTex;
var texture pAttackRightTex;
var texture p2DQuestMarkTex;
var texture pBlueTex;
var texture pBlueCrossTex;
var texture pBlueShieldTex;
var texture pBlueSwordTex;
var texture pRedTex;
var texture pRedCrossTex;
var texture pRedShieldTex;
var texture pRedSwordTex;
var texture pBlueAttLeaderTex;
var texture pBlueLeaderTex;
var texture pRedAttLeaderTex;
var texture pRedLeaderTex;
var texture pRadarTex;
var texture pMyLocTex;
var texture pPartyLocTex;
var texture pTargetLocTex;
var texture pTutorialLocTex1;
var texture pTutorialLocTex2;
var texture pRadarNorthTex;
var texture pRadarSouthTex;
var texture pRadarEastTex;
var texture pRadarWestTex;
var bool m_bForeTutorial;
var bool m_bBroadcastObserverMode;
var bool m_bShowBroadcastObserverTargetName;

// #ifdef __L2 by zodiac
var bool m_IsClipped;
var	float m_OldCurX, m_OldCurY;
var float m_OldOrgX, m_OldOrgY;
var	float m_OldClipX, m_OldClipY;
var font m_L2Font[3];
// #endif

var texture pChatBack1_1;
var texture pChatBack1_2;
var texture pChatBack1_3;
var texture pChatBack1_4;

var texture pChatBack2_1;
var texture pChatBack2_2;
var texture pChatBack2_3;
var texture pChatBack2_4;

var texture m_pBroadcastObserverNumberTexRed[ 9 ];
var texture m_pBroadcastObserverNumberTexBlue[ 9 ];

// #ifdef __L2 by idearain
var const int m_pViewportWindowRenderTarget;
var float m_fRecordMarkTimer;
var texture m_pReplayRecordTimerTex;
// #endif

var float m_FontScaleModifier;

var	float m_fTimePerFont;
var	float m_fStartTime;
var	int m_nTextIndex;
var	bool m_bTextAnim;
var	bool m_bTextEnd;
var bool m_bFirstTextAnim;

// native functions.
native(464) final function StrLen( coerce string String, out float XL, out float YL ); // Wrapped!
native(465) final function DrawText( coerce string Text, optional bool CR );
native(466) final function DrawTile( material Mat, float XL, float YL, float U, float V, float UL, float VL );
native(467) final function DrawActor( Actor A, bool WireFrame, optional bool ClearZ, optional float DisplayFOV );
native(468) final function DrawTileClipped( Material Mat, float XL, float YL, float U, float V, float UL, float VL );
native(469) final function DrawTextClipped( coerce string Text, optional bool bCheckHotKey );
native(470) final function TextSize( coerce string String, out float XL, out float YL ); // Clipped!
native(480) final function DrawPortal( int X, int Y, int Width, int Height, actor CamActor, vector CamLocation, rotator CamRotation, optional int FOV, optional bool ClearZ );

native final function WrapStringToArray(string Text, out array<string> OutArray, float dx, string EOL);

// jmw - These are two helper functions.  The use the whole texture only.  If you need better support, use DrawTile
native final function DrawTileStretched(material Mat, float XL, float YL);
native final function DrawTileJustified(material Mat, byte Justification, float XL, float YL);
native final function DrawTileScaled(material Mat, float XScale, float YScale);
native final function DrawTextJustified(coerce string String, byte Justification, float x1, float y1, float x2, float y2);

// UnrealScript functions.
event Reset()
{
	Font        = Default.Font;
	SpaceX      = Default.SpaceX;
	SpaceY      = Default.SpaceY;
	OrgX        = Default.OrgX;
	OrgY        = Default.OrgY;
	CurX        = Default.CurX;
	CurY        = Default.CurY;
	Style       = Default.Style;
	DrawColor   = Default.DrawColor;
	CurYL       = Default.CurYL;
	bCenter     = false;
	bNoSmooth   = false;
	Z           = 1.0;
}
final function SetPos( float X, float Y )
{
	CurX = X;
	CurY = Y;
}
final function SetOrigin( float X, float Y )
{
	OrgX = X;
	OrgY = Y;
}
final function SetClip( float X, float Y )
{
	ClipX = X;
	ClipY = Y;
}
final function DrawPattern( material Tex, float XL, float YL, float Scale )
{
	DrawTile( Tex, XL, YL, (CurX-OrgX)*Scale, (CurY-OrgY)*Scale, XL*Scale, YL*Scale );
}
final function DrawIcon( texture Tex, float Scale )
{
	if ( Tex != None )
		DrawTile( Tex, Tex.USize*Scale, Tex.VSize*Scale, 0, 0, Tex.USize, Tex.VSize );
}
final function DrawRect( texture Tex, float RectX, float RectY )
{
	DrawTile( Tex, RectX, RectY, 0, 0, Tex.USize, Tex.VSize );
}

final function SetDrawColor(byte R, byte G, byte B, optional byte A)
{
	local Color C;
	
	C.R = R;
	C.G = G;
	C.B = B;
	if ( A == 0 )
		A = 255;
	C.A = A;
	DrawColor = C;
}

static final function Color MakeColor(byte R, byte G, byte B, optional byte A)
{
	local Color C;
	
	C.R = R;
	C.G = G;
	C.B = B;
	if ( A == 0 )
		A = 255;
	C.A = A;
	return C;
}

// Draw a vertical line
final function DrawVertical(float X, float height)
{
    SetPos( X, CurY);
    DrawRect(Texture'engine.WhiteSquareTexture', 2, height);
}

// Draw a horizontal line
final function DrawHorizontal(float Y, float width)
{
    SetPos(CurX, Y);
    DrawRect(Texture'engine.WhiteSquareTexture', width, 2);
}

// Draw Line is special as it saves it's original position

final function DrawLine(int direction, float size)
{
    local float X, Y;

    // Save current position
    X = CurX;
    Y = CurY;

    switch (direction) 
    {
      case 0:
		  SetPos(X, Y - size);
		  DrawRect(Texture'engine.WhiteSquareTexture', 2, size);
		  break;
    
      case 1:
		  DrawRect(Texture'engine.WhiteSquareTexture', 2, size);
		  break;

      case 2:
		  SetPos(X - size, Y);
		  DrawRect(Texture'engine.WhiteSquareTexture', size, 2);
		  break;
		  
	  case 3:
		  DrawRect(Texture'engine.WhiteSquareTexture', size, 2);
		  break;
    }
    // Restore position
    SetPos(X, Y);
}

final simulated function DrawBracket(float width, float height, float bracket_size)
{
    local float X, Y;
    X = CurX;
    Y = CurY;

	Width  = max(width,5);
	Height = max(height,5);
	
    DrawLine(3, bracket_size);
    DrawLine(1, bracket_size);
    SetPos(X + width, Y);
    DrawLine(2, bracket_size);
    DrawLine(1, bracket_size);
    SetPos(X + width, Y + height);
    DrawLine(0, bracket_size);
    DrawLine(2, bracket_size);
    SetPos(X, Y + height);
    DrawLine(3, bracket_size);
    DrawLine( 0, bracket_size);

    SetPos(X, Y);
}

final simulated function DrawBox(canvas canvas, float width, float height)
{
	local float X, Y;
	X = canvas.CurX;
	Y = canvas.CurY;
	canvas.DrawRect(Texture'engine.WhiteSquareTexture', 2, height);
	canvas.DrawRect(Texture'engine.WhiteSquareTexture', width, 2);
	canvas.SetPos(X + width, Y);
	canvas.DrawRect(Texture'engine.WhiteSquareTexture', 2, height);
	canvas.SetPos(X, Y + height);
	canvas.DrawRect(Texture'engine.WhiteSquareTexture', width+1, 2);
	canvas.SetPos(X, Y);
}

defaultproperties
{
     Font=Font'Engine.DefaultFont'
     Z=1.000000
     Style=1
     DrawColor=(B=127,G=127,R=127,A=255)
     TinyFontName="WarfareFonts.FontMono"
     SmallFontName="WarfareFonts.FontMono"
     MedFontName="WarfareFonts.FontMono800x600"
}
