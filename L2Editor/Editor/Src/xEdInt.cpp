/*=============================================================================
	xEdInt.cpp: Editor internationalization file import/export routines.
	Copyright 2001 Digital Extremes, Inc. All Rights Reserved.
	Confidential.
=============================================================================*/

#include "EditorPrivate.h"

static INT Compare (const UObject *p1, const UObject *p2)
{
    int rc;

    rc = appStrcmp (p1->GetClass()->GetName(), p2->GetClass()->GetName());

    if (rc != 0)
        return (rc);
    
    return (appStrcmp (p1->GetName(), p2->GetName()));
}

static INT GPropertyCount;

static void IntExportProp( UClass* Class, UClass* SuperClass, UClass* OuterClass, UProperty* Prop, const TCHAR *IntName, const TCHAR *SectionName, const TCHAR *KeyPrefix, BYTE* DataBase, INT DataOffset );
static void IntExportStruct( UClass* Class, UClass* SuperClass, UClass* OuterClass, UStruct* Struct, const TCHAR *IntName, const TCHAR *SectionName, const TCHAR *KeyPrefix, BYTE* DataBase, INT DataOffset, bool AtRoot = false );

static void IntExportProp( UClass* Class, UClass* SuperClass, UClass* OuterClass, UProperty* Prop, const TCHAR *IntName, const TCHAR *SectionName, const TCHAR *KeyPrefix, BYTE* DataBase, INT DataOffset )
{
	UStructProperty* StructProperty = Cast<UStructProperty>( Prop );
	
	if( StructProperty )
	{
        IntExportStruct( Class, SuperClass, OuterClass, StructProperty->Struct, IntName, SectionName, KeyPrefix, DataBase, DataOffset );
        return;
    }

    if( !(Prop->PropertyFlags & CPF_Localized) ) 
        return;

    TCHAR RealValue[256 * 1024];
	Prop->ExportTextItem( RealValue, DataBase + DataOffset, NULL, PPF_Delimited );
	INT RealLength = appStrlen( RealValue );
    check( RealLength < ARRAY_COUNT(RealValue) );

    if( ( RealLength == 0 ) || !appStrcmp( RealValue, TEXT("\"\"") ) )
        return;

    if( Class && SuperClass && OuterClass && (OuterClass != Class) )
    {
        // Only export if value has changed from base class:
        
        TCHAR DefaultValue[256 * 1024];
        
        BYTE* DefaultDataBase = (BYTE*)&SuperClass->Defaults(0);
        
	    Prop->ExportTextItem( DefaultValue, DefaultDataBase + DataOffset, NULL, PPF_Delimited );
        check( appStrlen( DefaultValue ) < ARRAY_COUNT(DefaultValue) );

        if( appStrcmp( DefaultValue, RealValue ) == 0 )
            return;
    }
    
	// if it's an instance, check it 
    if( Class && SuperClass && OuterClass && (OuterClass != Class) )
    {
        // Only export if value has changed from base class:
        
        TCHAR DefaultValue[256 * 1024];
        
        BYTE* DefaultDataBase = (BYTE*)&SuperClass->Defaults(0);
        
	    Prop->ExportTextItem( DefaultValue, DefaultDataBase + DataOffset, NULL, PPF_Delimited );
        check( appStrlen( DefaultValue ) < ARRAY_COUNT(DefaultValue) );

        if( appStrcmp( DefaultValue, RealValue ) == 0 )
            return;
    }
	
	GConfig->SetString( SectionName, KeyPrefix, RealValue, IntName );
    GPropertyCount++;
}

static void IntExportStruct( UClass* Class, UClass* SuperClass, UClass* OuterClass, UStruct* Struct, const TCHAR *IntName, const TCHAR *SectionName, const TCHAR *KeyPrefix, BYTE* DataBase, INT DataOffset, bool AtRoot )
{
	for( TFieldFlagIterator<UProperty,CLASS_IsAUProperty> It( Struct ); It; ++It )
	{
	    UProperty* Prop = *It;

	    for( INT i = 0; i < Prop->ArrayDim; i++ )
	    {
    	    FString NewPrefix;

            if( KeyPrefix )
                NewPrefix = FString::Printf( TEXT("%s."), KeyPrefix );

	        if( Prop->ArrayDim > 1 )
                NewPrefix += FString::Printf( TEXT("%s[%d]"), Prop->GetName(), i );
	        else
                NewPrefix += Prop->GetName();

            INT NewOffset = DataOffset + (Prop->Offset) + (i * Prop->ElementSize );

            IntExportProp( Class, SuperClass, AtRoot ? CastChecked<UClass>(Prop->GetOuter()) : OuterClass, Prop, IntName, SectionName, *NewPrefix, DataBase, NewOffset );
	    }
	}
}

EDITOR_API UBOOL IntExport (UObject *Package, const TCHAR *IntName, UBOOL ExportFresh, UBOOL ExportInstances)
{
    TArray<UObject *> Objects;
    INT objectNumber;

    // Flush/empty any existing file so as to not confuse the GConfig object.

    if( ExportFresh )
    {
	    GConfig->UnloadFile( IntName );

        if (GFileManager->FileSize (IntName) >= 0)
            GFileManager->Delete (IntName, 0, 1);
    }

	for( FObjectIterator It; It; ++It )
	{
        UObject *Obj = *It;

		if( !Obj->IsIn(Package) )
            continue;

        if( Obj->GetFlags() & (RF_Transient | RF_NotForClient | RF_NotForServer | RF_Destroyed) )
            continue;

        Objects.AddItem (Obj);
	}

    if( Objects.Num() )
        Sort (&Objects(0), Objects.Num());

    GPropertyCount = 0;

    for (objectNumber = 0; objectNumber < Objects.Num(); objectNumber++)
    {
		UClass* Class = Cast<UClass>( Objects(objectNumber) );

		if( Class && (Class->ClassFlags & CLASS_Localized) )
		    IntExportStruct( Class, Class->GetSuperClass(), Class, Class, IntName, Class->GetName(), NULL, &Class->Defaults(0), 0, true );
		else if( Objects(objectNumber)->GetClass()->ClassFlags & CLASS_Localized )
		{
			UClass* SubObjectOuter = Cast<UClass>(Objects(objectNumber)->GetOuter());
			if( SubObjectOuter )	// if it's a subobject, export as [OuterClass] SubObjectName.Key=Value
			{
				// Find the Class property which references this subobject
				if( Objects(objectNumber)->GetFlags()&RF_PerObjectLocalized )
					IntExportStruct( NULL, NULL, NULL, Objects(objectNumber)->GetClass(), IntName, Objects(objectNumber)->GetOuter()->GetName(), Objects(objectNumber)->GetName(), (BYTE*)(Objects(objectNumber)), 0, true );
			}
			else
			if( ExportInstances )
			{
				IntExportStruct( NULL, NULL, NULL, Objects(objectNumber)->GetClass(), IntName, Objects(objectNumber)->GetName(), NULL, (BYTE*)(Objects(objectNumber)), 0, true );
			}
		}
    }

	GConfig->Flush( 0, IntName );

	GWarn->Logf( NAME_Log, TEXT("Exported %d properties."), GPropertyCount );

    return 1;
}

EDITOR_API UBOOL IntExport (const TCHAR *PackageName, const TCHAR *IntName, UBOOL ExportFresh, UBOOL ExportInstances)
{
    UObject* Package;
    UBOOL rc;
    bool bNeedToUnload = false;

    Package = FindObject<UPackage>( NULL, PackageName );

    if( !Package )
    {
        Package = UObject::LoadPackage (NULL, PackageName, 0 );
        bNeedToUnload = true;
    }

	if( !Package )
    {
		GWarn->Logf( NAME_Error, TEXT("Could not load package %s"), PackageName );
		return 0;
    }

    rc = IntExport (Package, IntName, ExportFresh, ExportInstances);

    if( bNeedToUnload )
        UObject::ResetLoaders (Package, 0, 1);

    return (rc);
}

EDITOR_API UBOOL IntMatchesPackage (UObject *Package, const TCHAR *IntName)
{
    FString TempIntName;
    FString TextBufferA, TextBufferB;

    TempIntName = IntName;
    TempIntName += TEXT ("-temp.int");

    bool isCaseSensitive;
    int i;

    if (GFileManager->FileSize (*TempIntName) >= 0)
        GFileManager->Delete (*TempIntName);

    if (GFileManager->FileSize (*TempIntName) >= 0)
    {
        GWarn->Logf(NAME_Warning, TEXT ("Could not remove \"%s\"."), *TempIntName);
        return (1);
    }
    
    if (!IntExport (Package, *TempIntName, true, true))
    {
        GWarn->Logf(NAME_Error, TEXT ("Could not export \"%s\"."), *TempIntName);
        return (1);
    }

    if ((GFileManager->FileSize (*TempIntName) <= 0) && (GFileManager->FileSize (IntName) <= 0))
        return (1);

    if (GFileManager->FileSize (*TempIntName) != GFileManager->FileSize (IntName))
    {
        GFileManager->Delete (*TempIntName);
        return (0);
    }

	if( !appLoadFileToString( TextBufferA, IntName ) )
    {
        GWarn->Logf( NAME_Error, TEXT("Could not open %s"), IntName );
        return (1);
    }

	if( !appLoadFileToString( TextBufferB, *TempIntName ) )
    {
        GWarn->Logf( NAME_Error, TEXT("Could not open %s"), *TempIntName );
        return (1);
    }

    if (GFileManager->FileSize (*TempIntName) >= 0)
        GFileManager->Delete (*TempIntName);

    if (GFileManager->FileSize (*TempIntName) >= 0)
        GWarn->Logf(NAME_Warning, TEXT ("Could not remove \"%s\"."), *TempIntName);

    isCaseSensitive = false;

    check (TextBufferA.Len() == TextBufferB.Len());

    for (i = 0; i < TextBufferA.Len(); i++)
    {
        const TCHAR *a, *b;

        a = *TextBufferA;
        b = *TextBufferB;

        if (isCaseSensitive)
        {
            if (a[i] != b[i])
                return (0);
        }
        else
        {
            if (appToLower (a[i]) != appToLower (b[i]))
                return (0);
        }

        if (a[i] == '=')
            isCaseSensitive = true;
        else if (a[i] == '\n')
            isCaseSensitive = false;
    }

    return (1);
}

EDITOR_API UBOOL IntMatchesPackage (const TCHAR *PackageName, const TCHAR *IntName)
{
    UObject* Package;
    UBOOL rc;
    bool bNeedToUnload = false;

    Package = FindObject<UPackage>( NULL, PackageName );

    if( !Package )
    {
        Package = UObject::LoadPackage (NULL, PackageName, 0 );
        bNeedToUnload = true;
    }

	if( !Package )
    {
		GWarn->Logf( NAME_Error, TEXT("Could not load package %s"), PackageName );
		return 0;
    }

    rc = IntMatchesPackage (Package, IntName);

    if( bNeedToUnload )
        UObject::ResetLoaders (Package, 0, 1);

    return (rc);
}

EDITOR_API void IntGetNameFromPackageName (const FString &PackageName, FString &IntName)
{
    INT i;

    IntName = PackageName;

    i = IntName.InStr (TEXT ("."), 1);

    if (i >= 0)
        IntName = IntName.Left (i);

    IntName += TEXT (".int");

    i = IntName.InStr (TEXT ("/"), 1);
    
    if (i >= 0)
        IntName = IntName.Right (IntName.Len () - i - 1);

    i = IntName.InStr (TEXT ("\\"), 1);
    
    if (i >= 0)
        IntName = IntName.Right (IntName.Len () - i - 1);

    IntName = FString (appBaseDir()) + IntName;
}

EDITOR_API UBOOL IntMatchesPackage (const TCHAR *PackageName)
{
    FString IntName;
    IntGetNameFromPackageName (PackageName, IntName);
    return( IntMatchesPackage( PackageName, *IntName ));
}
