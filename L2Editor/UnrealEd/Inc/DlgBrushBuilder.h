/*=============================================================================
	BrushBuilder : Properties of a brush builder
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Warren Marshall

    Work-in-progress todo's:

=============================================================================*/

RECT GWBBLastPos = { -1, -1, -1, -1 };

class WDlgBrushBuilder : public WDialog
{
	DECLARE_WINDOWCLASS(WDlgBrushBuilder,WDialog,UnrealEd)

	// Variables.
	WButton BuildButton, CancelButton;

	WObjectProperties* pProps;
	UBrushBuilder* Builder;

	// Constructor.
	WDlgBrushBuilder( UObject* InContext, WWindow* InOwnerWindow, UBrushBuilder* InBuilder )
	:	WDialog		( TEXT("Brush Builder"), IDDIALOG_BRUSH_BUILDER, InOwnerWindow )
	, BuildButton(this, IDPB_BUILD, FDelegate(this, (TDelegate)&WDlgBrushBuilder::OnBuild))
	, CancelButton(this, IDCANCEL, FDelegate(this, (TDelegate)&WDialog::EndDialogFalse))
	{
		Builder = InBuilder;

		pProps = new WObjectProperties( NAME_None, CPF_Edit, TEXT(""), this, 1 );
		pProps->ShowTreeLines = 1;
	}

	// WDialog interface.
	void OnInitDialog()
	{
		guard(WDlgBrushBuilder::OnInitDialog);
		WDialog::OnInitDialog();

		pProps->OpenChildWindow( IDSC_PROPS );
		SetupPropertyList();

		SetText( Builder->GetClass()->GetName() );

		if( GWBBLastPos.left == -1 )
		{
			POINT pt;
			::GetCursorPos( &pt );
			GWBBLastPos.left = pt.x;
			GWBBLastPos.top = pt.y;
		}

		::SetWindowPos( hWnd, HWND_TOP, GWBBLastPos.left, GWBBLastPos.top, 0, 0, SWP_NOSIZE );

		unguard;
	}
	void SetupPropertyList()
	{
		guard(WDlgBrushBuilder::SetupPropertyList);

		// This is a massive hack.  The fields for the appropriate category are being hand fed
		// into the property window and the link to the object is being set up manually.  This
		// feels wrong in many ways to me, but it works and that's what counts at the moment.
		pProps->Root.Sorted = 0;
		pProps->Root._Objects.AddItem( Builder );
		for( TFieldFlagIterator<UProperty,CLASS_IsAUProperty> It(Builder->GetClass()); It; ++It )
			if( It->Category==FName(Builder->GetClass()->GetName()) && pProps->Root.AcceptFlags( It->PropertyFlags ) )
				pProps->Root.Children.AddItem( new(TEXT("FPropertyItem"))FPropertyItem( pProps, &(pProps->Root), *It, It->GetFName(), It->Offset, -1, 666 ) );
		pProps->Root.Expand();
		pProps->ResizeList();
		pProps->bAllowForceRefresh = 0;

		unguard;
	}
	void OnDestroy()
	{
		guard(WDlgBrushBuilder::OnDestroy);
		WDialog::OnDestroy();

		delete pProps;
		unguard;
	}
	virtual void DoModeless( UBOOL bShow )
	{
		guard(WDlgBrushBuilder::DoModeless);
		_Windows.AddItem( this );
		hWnd = CreateDialogParamA( hInstance, MAKEINTRESOURCEA(IDDIALOG_BRUSH_BUILDER), OwnerWindow?OwnerWindow->hWnd:NULL, (DLGPROC)StaticDlgProc, (LPARAM)this);
		if( !hWnd )
			appGetLastError();
		Show( bShow );
		unguard;
	}
	void OnBuild()
	{
		guard(WDlgBrushBuilder::OnBuild);

		// Force all controls to save their values before trying to build the brush.
		for( INT i=0; i<pProps->Root.Children.Num(); ++i )
			((FPropertyItem*)pProps->Root.Children(i))->SendToControl();

		UBOOL GIsSavedScriptableSaved = 1;
		Exchange(GIsScriptable,GIsSavedScriptableSaved);
		Builder->eventBuild();
		Exchange(GIsScriptable,GIsSavedScriptableSaved);

		unguard;
	}
};

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
