// ====================================================================
//  Class:  Engine.BaseGUIController
// 
//  This is just a stub class that should be subclassed to support menus.
//
//  Written by Joe Wilcox
//  (c) 2002, Epic Games, Inc.  All Rights Reserved
// ====================================================================

class BaseGUIController extends Interaction
		Native;
		
#exec TEXTURE IMPORT NAME=MenuWhite FILE=Textures\White.tga MIPS=0
#exec TEXTURE IMPORT NAME=MenuBlack FILE=Textures\Black.tga MIPS=0
#exec TEXTURE IMPORT NAME=MenuGray  FILE=Textures\Gray.tga MIPS=0

var	Material	DefaultPens[3]; 	// Contain to hold some default pens for drawing purposes 					

// Delegates
Delegate OnAdminReply(string Reply);	// Called By PlayerController

// ================================================
// OpenMenu - Opens a new menu and places it on top of the stack


event bool OpenMenu(string NewMenuName, optional string Param1, optional string Param2)				
{
	return false;		
}

// ================================================
// Replaces a menu in the stack.  returns true if success

event bool ReplaceMenu(string NewMenuName, optional string Param1, optional string Param2)
{
	return false;		
}

event bool CloseMenu(optional bool bCanceled)	// Close the top menu.  returns true if success.
{
	return true;	
}
event CloseAll(bool bCancel);

function SetControllerStatus(bool On)
{
	bActive = On;
	bVisible = On;
	bRequiresTick=On;
	
	// Add code to pause/unpause/hide/etc the game here.
	
}

event InitializeController();	// Should be subclassed.

event bool NeedsMenuResolution(); // Big Hack that should be subclassed
event SetRequiredGameResolution(string GameRes);

defaultproperties
{
     DefaultPens(0)=Texture'Engine.MenuWhite'
     DefaultPens(1)=Texture'Engine.MenuBlack'
     DefaultPens(2)=Texture'Engine.MenuGray'
     bActive=False
     bNativeEvents=True
}
