/*=============================================================================
	UnMath.h: Unreal math routines
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

/*-----------------------------------------------------------------------------
	Defintions.
-----------------------------------------------------------------------------*/

// Forward declarations.
class  FVector;
class  FPlane;
class  FCoords;
class  FRotator;
class  FScale;
class  FGlobalMath;
class  FMatrix;
class  FQuat;

// Fixed point conversion.
inline	INT Fix		(INT A)			{return A<<16;};
inline	INT Fix		(FLOAT A)		{return (INT)(A*65536.f);};
inline	INT Unfix	(INT A)			{return A>>16;};

// Constants.
#undef  PI
#define PI 					(3.1415926535897932)
#define SMALL_NUMBER		(1.e-8)
#define KINDA_SMALL_NUMBER	(1.e-4)

// Aux constants.
#define INV_PI			(0.31830988618)
#define HALF_PI			(1.57079632679)

// Magic numbers for numerical precision.
#define DELTA			(0.00001f)
#define SLERP_DELTA		(0.0001f)

/*-----------------------------------------------------------------------------
	Global functions.
-----------------------------------------------------------------------------*/

//
// Snap a value to the nearest grid multiple.
//
inline FLOAT FSnap( FLOAT Location, FLOAT Grid )
{
	if( Grid==0.f )	return Location;
	else			return appFloor((Location + 0.5*Grid)/Grid)*Grid;
}

//
// Internal sheer adjusting function so it snaps nicely at 0 and 45 degrees.
//
inline FLOAT FSheerSnap (FLOAT Sheer)
{
	if		(Sheer < -0.65f) return Sheer + 0.15f;
	else if (Sheer > +0.65f) return Sheer - 0.15f;
	else if (Sheer < -0.55f) return -0.50f;
	else if (Sheer > +0.55f) return 0.50f;
	else if (Sheer < -0.05f) return Sheer + 0.05f;
	else if (Sheer > +0.05f) return Sheer - 0.05f;
	else					 return 0.f;
}

//
// Find the closest power of 2 that is >= N.
//
inline DWORD FNextPowerOfTwo( DWORD N )
{
	if (N<=0L		) return 0L;
	if (N<=1L		) return 1L;
	if (N<=2L		) return 2L;
	if (N<=4L		) return 4L;
	if (N<=8L		) return 8L;
	if (N<=16L	    ) return 16L;
	if (N<=32L	    ) return 32L;
	if (N<=64L 	    ) return 64L;
	if (N<=128L     ) return 128L;
	if (N<=256L     ) return 256L;
	if (N<=512L     ) return 512L;
	if (N<=1024L    ) return 1024L;
	if (N<=2048L    ) return 2048L;
	if (N<=4096L    ) return 4096L;
	if (N<=8192L    ) return 8192L;
	if (N<=16384L   ) return 16384L;
	if (N<=32768L   ) return 32768L;
	if (N<=65536L   ) return 65536L;
	else			  return 0;
}

inline UBOOL FIsPowerOfTwo( DWORD N )
{
	return !(N & (N - 1));
}

//
// Add to a word angle, constraining it within a min (not to cross)
// and a max (not to cross).  Accounts for funkyness of word angles.
// Assumes that angle is initially in the desired range.
//
inline _WORD FAddAngleConfined( INT Angle, INT Delta, INT MinThresh, INT MaxThresh )
{
	if( Delta < 0 )
	{
		if ( Delta<=-0x10000L || Delta<=-(INT)((_WORD)(Angle-MinThresh)))
			return MinThresh;
	}
	else if( Delta > 0 )
	{
		if( Delta>=0x10000L || Delta>=(INT)((_WORD)(MaxThresh-Angle)))
			return MaxThresh;
	}
	return (_WORD)(Angle+Delta);
}

//
// Eliminate all fractional precision from an angle.
//
INT ReduceAngle( INT Angle );

//
// Fast 32-bit float evaluations. 
// Warning: likely not portable, and useful on Pentium class processors only.
//

inline UBOOL IsSmallerPositiveFloat(float F1,float F2)
{
	return ( (*(DWORD*)&F1) < (*(DWORD*)&F2));
}

inline FLOAT MinPositiveFloat(float F1, float F2)
{
	if ( (*(DWORD*)&F1) < (*(DWORD*)&F2)) return F1; else return F2;
}

//
// Warning: 0 and -0 have different binary representations.
//

inline UBOOL EqualPositiveFloat(float F1, float F2)
{
	return ( *(DWORD*)&F1 == *(DWORD*)&F2 );
}

inline UBOOL IsNegativeFloat(float F1)
{
	return ( (*(DWORD*)&F1) >= (DWORD)0x80000000 ); // Detects sign bit.
}

inline FLOAT MaxPositiveFloat(float F1, float F2)
{
	if ( (*(DWORD*)&F1) < (*(DWORD*)&F2)) return F2; else return F1;
}

// Clamp F0 between F1 and F2, all positive assumed.
inline FLOAT ClampPositiveFloat(float F0, float F1, float F2)
{
	if      ( (*(DWORD*)&F0) < (*(DWORD*)&F1)) return F1;
	else if ( (*(DWORD*)&F0) > (*(DWORD*)&F2)) return F2;
	else return F0;
}

// Clamp any float F0 between zero and positive float Range
#define ClipFloatFromZero(F0,Range)\
{\
	if ( (*(DWORD*)&F0) >= (DWORD)0x80000000) F0 = 0.f;\
	else if	( (*(DWORD*)&F0) > (*(DWORD*)&Range)) F0 = Range;\
}

/*-----------------------------------------------------------------------------
	FVector.
-----------------------------------------------------------------------------*/

// Information associated with a floating point vector, describing its
// status as a point in a rendering context.
enum EVectorFlags
{
	FVF_OutXMin		= 0x04,	// Outcode rejection, off left hand side of screen.
	FVF_OutXMax		= 0x08,	// Outcode rejection, off right hand side of screen.
	FVF_OutYMin		= 0x10,	// Outcode rejection, off top of screen.
	FVF_OutYMax		= 0x20,	// Outcode rejection, off bottom of screen.
	FVF_OutNear     = 0x40, // Near clipping plane.
	FVF_OutFar      = 0x80, // Far clipping plane.
	FVF_OutReject   = (FVF_OutXMin | FVF_OutXMax | FVF_OutYMin | FVF_OutYMax), // Outcode rejectable.
	FVF_OutSkip		= (FVF_OutXMin | FVF_OutXMax | FVF_OutYMin | FVF_OutYMax), // Outcode clippable.
};

//
// Floating point vector.
//

class CORE_API FVector 
{
public:
	// Variables.
	FLOAT X,Y,Z;

	// Constructors.
	FORCEINLINE FVector();
	FORCEINLINE FVector(FLOAT InX, FLOAT InY, FLOAT InZ);

	// Binary math operators.
	FORCEINLINE FVector operator^(const FVector& V) const;
	FORCEINLINE FLOAT operator|(const FVector& V) const;
	friend FVector operator*(FLOAT Scale, const FVector& V)
	{
		return FVector(V.X * Scale, V.Y * Scale, V.Z * Scale);
	}
	FORCEINLINE FVector operator+(const FVector& V) const;
	FORCEINLINE FVector operator-(const FVector& V) const;
	FORCEINLINE FVector operator*(FLOAT Scale) const;
	FVector operator/(FLOAT Scale) const;
	FORCEINLINE FVector operator*(const FVector& V) const;

	// Binary comparison operators.
	UBOOL operator==(const FVector& V) const;
	UBOOL operator!=(const FVector& V) const;

	// Unary operators.
	FORCEINLINE FVector operator-() const;

	// Assignment operators.
	FORCEINLINE FVector operator+=(const FVector& V);
	FORCEINLINE FVector operator-=(const FVector& V);
	FORCEINLINE FVector operator*=(FLOAT Scale);
	FVector operator/=(FLOAT V);
	FVector operator*=(const FVector& V);
	FVector operator/=(const FVector& V);
	FLOAT& operator[](INT i);

	// Simple functions.
	FLOAT GetMax() const;
	FLOAT GetAbsMax() const;
	FLOAT GetMin() const;
	FLOAT Size() const;
	FLOAT SizeSquared() const;
	FLOAT Size2D() const;
	FLOAT SizeSquared2D() const;
	int IsNearlyZero() const;
	UBOOL IsZero() const;
	UBOOL Normalize();
	FVector GetNormalized();
	// Expects a unit vector and returns a vector that is sufficiently non parallel ;)
	FVector GetNonParallel();
	FVector Projection() const;
	FVector UnsafeNormal() const;
	FVector GridSnap(const FVector& Grid);
	FVector BoundToCube(FLOAT Radius);
	void AddBounded(const FVector& V, FLOAT Radius = MAXSWORD);
	FLOAT& Component(INT Index);

	// Return a boolean that is based on the vector's direction.
	// When      V==(0,0,0) Booleanize(0)=1.
	// Otherwise Booleanize(V) <-> !Booleanize(!B).
	UBOOL Booleanize();

	// See if X == Y == Z (within fairly small tolerance)
	UBOOL IsUniform();

	// Transformation.
	FVector TransformVectorBy( const FCoords& Coords ) const;
	FVector TransformPointBy( const FCoords& Coords ) const;
	FVector MirrorByVector( const FVector& MirrorNormal ) const;
	FVector MirrorByPlane( const FPlane& MirrorPlane ) const;
	FVector PivotTransform(const FCoords& Coords) const;
	FVector TransformVectorByTranspose( const FCoords& Coords ) const;

	// Complicated functions.
	FRotator Rotation();
	void FindBestAxisVectors( FVector& Axis1, FVector& Axis2 );
	FVector SafeNormal() const; //warning: Not inline because of compiler bug.
	FVector RotateAngleAxis( const INT Angle, const FVector& Axis ) const;

	// Friends.
	friend FLOAT FDist( const FVector& V1, const FVector& V2 );
	friend FLOAT FDistSquared( const FVector& V1, const FVector& V2 );
	friend UBOOL FPointsAreSame( const FVector& P, const FVector& Q );
	friend UBOOL FPointsAreNear( const FVector& Point1, const FVector& Point2, FLOAT Dist);
	friend FLOAT FPointPlaneDist( const FVector& Point, const FVector& PlaneBase, const FVector& PlaneNormal );
	friend FVector FLinePlaneIntersection( const FVector& Point1, const FVector& Point2, const FVector& PlaneOrigin, const FVector& PlaneNormal );
	friend FVector FLinePlaneIntersection( const FVector& Point1, const FVector& Point2, const FPlane& Plane );
	friend UBOOL FParallel( const FVector& Normal1, const FVector& Normal2 );
	friend UBOOL FCoplanar( const FVector& Base1, const FVector& Normal1, const FVector& Base2, const FVector& Normal2 );

	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FVector& V )
	{
		return Ar << V.X << V.Y << V.Z;
	}
};

// Used by the multiple vertex editing function to keep track of selected vertices.
class ABrush;
class CORE_API FVertexHit
{
public:
	// Variables.
	ABrush* pBrush;
	INT PolyIndex;
	INT VertexIndex;

	// Constructors.
	FVertexHit();
	FVertexHit(ABrush* InBrush, INT InPolyIndex, INT InVertexIndex);

	// Functions.
	UBOOL operator==(const FVertexHit& V) const;
	UBOOL operator!=(const FVertexHit& V) const;
};

/*-----------------------------------------------------------------------------
	FEdge.
-----------------------------------------------------------------------------*/

class CORE_API FEdge
{
public:
	// Constructors.
	FEdge();
	FEdge(FVector v1, FVector v2);

	FVector Vertex[2];

	UBOOL operator==(const FEdge& E) const;
};

/*-----------------------------------------------------------------------------
	FPlane.
-----------------------------------------------------------------------------*/

class CORE_API FPlane : public FVector
{
public:
	// Variables.
	FLOAT W;

	// Constructors.
	FORCEINLINE FPlane();
	FORCEINLINE FPlane(const FPlane& P);
	FORCEINLINE FPlane(const FVector& V);
	FORCEINLINE FPlane(FLOAT InX, FLOAT InY, FLOAT InZ, FLOAT InW);
	FORCEINLINE FPlane(FVector InNormal, FLOAT InW);
	FORCEINLINE FPlane(FVector InBase, const FVector &InNormal);
	FPlane(FVector A, FVector B, FVector C);

	// Functions.
	FORCEINLINE FLOAT PlaneDot(const FVector &P) const;
	FPlane Flip() const;
	FPlane TransformPlaneByOrtho( const FMatrix& M ) const;
	FPlane TransformBy( const FMatrix& M ) const;
	FPlane TransformByUsingAdjointT( const FMatrix& M, const FMatrix& TA ) const;
	FPlane TransformPlaneByOrtho( const FCoords& Coords ) const;
	FPlane TransformBy( const FCoords& Coords ) const;

	UBOOL operator==(const FPlane& V) const;
	UBOOL operator!=(const FPlane& V) const;
	FPlane operator+(const FPlane& V) const;
	FPlane operator-(const FPlane& V) const;
	FPlane operator/(FLOAT Scale) const;
	FPlane operator*(FLOAT Scale) const;
	FPlane operator*(const FPlane& V);
	FPlane operator+=(const FPlane& V);
	FPlane operator-=(const FPlane& V);
	FPlane operator*=(FLOAT Scale);
	FPlane operator*=(const FPlane& V);
	FPlane operator/=(FLOAT V);

	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FPlane &P )
	{
		return Ar << (FVector&)P << P.W;
	}
};

// gam ---
template<class T> struct TBox
{
    T X1, Y1, X2, Y2;

    bool Test( T X, T Y ) const
    {
        return (( X >= X1 ) && ( X <= X2 ) && ( Y >= Y1 ) && ( Y <= Y2 ));
    }
};

typedef TBox<INT> FIntBox;
typedef TBox<FLOAT> FFloatBox;
// --- gam

/*-----------------------------------------------------------------------------
	FSphere.
-----------------------------------------------------------------------------*/

class CORE_API FSphere : public FPlane
{
public:
	// Constructors.
	FSphere();
	FSphere(INT);
	FSphere(FVector V, FLOAT W);
	FSphere( const FVector* Pts, INT Count );

	FSphere TransformBy(const FMatrix& M) const;

	friend FArchive& operator<<( FArchive& Ar, FSphere& S )
	{
		guardSlow(FSphere<<);
		if( Ar.Ver()<=61 )//oldver
			Ar << (FVector&)S;
		else
			Ar << (FPlane&)S;
		return Ar;
		unguardSlow
	}
};

/*-----------------------------------------------------------------------------
	FScale.
-----------------------------------------------------------------------------*/

// An axis along which sheering is performed.
enum ESheerAxis
{
	SHEER_None = 0,
	SHEER_XY   = 1,
	SHEER_XZ   = 2,
	SHEER_YX   = 3,
	SHEER_YZ   = 4,
	SHEER_ZX   = 5,
	SHEER_ZY   = 6,
};

//
// Scaling and sheering info associated with a brush.  This is 
// easily-manipulated information which is built into a transformation
// matrix later.
//
class CORE_API FScale 
{
public:
	// Variables.
	FVector		Scale;
	FLOAT		SheerRate;
	BYTE		SheerAxis; // From ESheerAxis

	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FScale &S )
	{
		return Ar << S.Scale << S.SheerRate << S.SheerAxis;
	}

	// Constructors.
	FScale() {}
	FScale(const FVector &InScale, FLOAT InSheerRate, ESheerAxis InSheerAxis);

	// Operators.
	UBOOL operator==(const FScale &S) const;

	// Functions.
	FLOAT  Orientation();
};

/*-----------------------------------------------------------------------------
	FCoords.
-----------------------------------------------------------------------------*/

//
// A coordinate system matrix.
//
class CORE_API FCoords
{
public:
	FVector	Origin;
	FVector	XAxis;
	FVector YAxis;
	FVector ZAxis;

	// Constructors.
	FCoords();
	FCoords(const FVector &InOrigin);
	FCoords(const FVector &InOrigin, const FVector &InX, const FVector &InY, const FVector &InZ);

	// Functions.
	FCoords MirrorByVector( const FVector& MirrorNormal ) const;
	FCoords MirrorByPlane( const FPlane& MirrorPlane ) const;
	FCoords	Transpose() const;
	FCoords Inverse() const;
	FCoords PivotInverse() const;
	FCoords ApplyPivot(const FCoords& CoordsB) const;
	FRotator OrthoRotation() const;
	FMatrix Matrix() const;

	// Operators.
	FCoords& operator*=	(const FCoords   &TransformCoords);
	FCoords	 operator*	(const FCoords   &TransformCoords) const;
	FCoords& operator*=	(const FVector   &Point);
	FCoords  operator*	(const FVector   &Point) const;
	FCoords& operator*=	(const FRotator  &Rot);
	FCoords  operator*	(const FRotator  &Rot) const;
	FCoords& operator*=	(const FScale    &Scale);
	FCoords  operator*	(const FScale    &Scale) const;
	FCoords& operator/=	(const FVector   &Point);
	FCoords  operator/	(const FVector   &Point) const;
	FCoords& operator/=	(const FRotator  &Rot);
	FCoords  operator/	(const FRotator  &Rot) const;
	FCoords& operator/=	(const FScale    &Scale);
	FCoords  operator/	(const FScale    &Scale) const;

	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FCoords& F )
	{
		return Ar << F.Origin << F.XAxis << F.YAxis << F.ZAxis;
	}
};

/*-----------------------------------------------------------------------------
	FModelCoords.
-----------------------------------------------------------------------------*/

//
// A model coordinate system, describing both the covariant and contravariant
// transformation matrices to transform points and normals by.
//
class CORE_API FModelCoords
{
public:
	// Variables.
	FCoords PointXform;		// Coordinates to transform points by  (covariant).
	FCoords VectorXform;	// Coordinates to transform normals by (contravariant).

	// Constructors.
	FModelCoords();
	FModelCoords(const FCoords& InCovariant, const FCoords& InContravariant);

	// Functions.
	FModelCoords Inverse();
};

/*-----------------------------------------------------------------------------
	FRotator.
-----------------------------------------------------------------------------*/

//
// Rotation.
//
class CORE_API FRotator
{
public:
	// Variables.
	INT Pitch; // Looking up and down (0=Straight Ahead, +Up, -Down).
	INT Yaw;   // Rotating around (running in circles), 0=East, +North, -South.
	INT Roll;  // Rotation about axis of screen, 0=Straight, +Clockwise, -CCW.

	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FRotator& R )
	{
		return Ar << R.Pitch << R.Yaw << R.Roll;
	}

	// Constructors.
	FRotator();
	FRotator(INT InPitch, INT InYaw, INT InRoll);

	// Binary arithmetic operators.
	FRotator operator+(const FRotator &R) const;
	FRotator operator-(const FRotator &R) const;
	FRotator operator*(FLOAT Scale) const;
	friend FRotator operator*(FLOAT Scale, const FRotator &R)
	{
		return FRotator((INT)(R.Pitch*Scale), (INT)(R.Yaw*Scale), (INT)(R.Roll*Scale));
	}
	FRotator operator*= (FLOAT Scale);

	// Binary comparison operators.
	UBOOL operator==(const FRotator &R) const;
	UBOOL operator!=(const FRotator &V) const;

	// Assignment operators.
	FRotator operator+=(const FRotator &R);
	FRotator operator-=(const FRotator &R);

	// Functions.
	FRotator Reduce() const;
	int IsZero() const;
	FRotator Add(INT DeltaPitch, INT DeltaYaw, INT DeltaRoll);
	FRotator AddBounded(INT DeltaPitch, INT DeltaYaw, INT DeltaRoll);
	FRotator GridSnap(const FRotator &RotGrid);
	FVector Vector();
	// Resets the rotation values so they fall within the range -65535,65535
	FRotator Clamp();
	FRotator ClampPos();
};

/*-----------------------------------------------------------------------------
	FPosition.
-----------------------------------------------------------------------------*/

// A convenience class for keep track of positions.
class CORE_API FPosition
{
public:
	// Variables.
	FVector Location;
	FCoords Coords;

	// Constructors.
	FPosition();
	FPosition(FVector InLocation, FCoords InCoords);
};

/*-----------------------------------------------------------------------------
	FRange.
-----------------------------------------------------------------------------*/

//
// Floating point range. Aaron Leiby
//
// - changed to Min/Max: vogel
//
class CORE_API FRange 
{
public:
	// Variables.
	FLOAT Min, Max;

	// Constructors.
	FRange();
	FRange(FLOAT InMin, FLOAT InMax);
	FRange(FLOAT Value);
	// Binary math operators.
	friend FRange operator*( FLOAT Scale, const FRange& R )
	{
		return FRange( R.Min * Scale, R.Max * Scale );
	}
	FRange operator+(const FRange& R) const;
	FRange operator+(FLOAT V) const;
	FRange operator-(FLOAT V) const;
	FRange operator-(const FRange& R) const;
	FRange operator*(FLOAT Scale) const;
	FRange operator/(FLOAT Scale) const;
	FRange operator*(const FRange& R) const;

	// Binary comparison operators.
	UBOOL operator==(const FRange& R) const;
	UBOOL operator!=(const FRange& R) const;

	// Unary operators.
	FRange operator-() const;

	// Assignment operators.
	FRange operator+=(const FRange& R);
	FRange operator-=(const FRange& R);
	FRange operator+=(FLOAT V);
	FRange operator-=(FLOAT V);
	FRange operator*=(FLOAT Scale);
	FRange operator/=(FLOAT Scale);
	FRange operator*=(const FRange& R);
	FRange operator/=(const FRange& R);

	// Simple functions.
	FLOAT GetMax() const;
	FLOAT GetMin() const;
	FLOAT Size() const;
	FLOAT GetCenter() const;
	FLOAT GetRand() const;
	FLOAT GetSRand() const;
#if 0
	INT GetRandInt() const
	{
		return appRandRange( (INT)Min, (INT)Max );
	} 
#endif
	int IsNearlyZero() const;
	UBOOL IsZero() const;

	FRange GridSnap(const FRange& Grid);
	FLOAT& Component(INT Index);

	// When      R==(0.0) Booleanize(0)=1.
	// Otherwise Booleanize(R) <-> !Booleanize(!R).
	UBOOL Booleanize();

	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FRange& R )
	{
		return Ar << R.Min << R.Max;
	}
};

/*-----------------------------------------------------------------------------
	FRangeVector.
-----------------------------------------------------------------------------*/

//
// Vector of floating point ranges.
//
class CORE_API FRangeVector
{
public:
	// Variables.
	FRange X, Y, Z;

	// Constructors.
	FRangeVector();
	FRangeVector(FRange InX, FRange InY, FRange InZ);
	FRangeVector(FVector V);
	// Binary math operators.
	friend FRangeVector operator*( FLOAT Scale, const FRangeVector& R )
	{
		return FRangeVector( R.X * Scale, R.Y * Scale, R.Z * Scale );
	}
	FRangeVector operator+(const FRangeVector& R) const;
	FRangeVector operator-(const FRangeVector& R) const;
	FRangeVector operator+(const FVector& V) const;
	FRangeVector operator-(const FVector& V) const;
	FRangeVector operator*(FLOAT Scale) const;
	FRangeVector operator/(FLOAT Scale) const;
	FRangeVector operator*(const FRangeVector& R) const;

	// Binary comparison operators.
	UBOOL operator==(const FRangeVector& R) const;
	UBOOL operator!=(const FRangeVector& R) const;

	// Unary operators.
	FRangeVector operator-() const;

	// Assignment operators.
	FRangeVector operator+=(const FRangeVector& R);
	FRangeVector operator-=(const FRangeVector& R);
	FRangeVector operator+=(const FVector& V);
	FRangeVector operator-=(const FVector& V);
	FRangeVector operator*=(FLOAT Scale);
	FRangeVector operator/=(FLOAT Scale);
	FRangeVector operator*=(const FRangeVector& R);
	FRangeVector operator/=(const FRangeVector& R);

	FVector GetCenter() const;
	FVector GetMax() const;
	FVector GetRand() const;
	FVector GetSRand() const;

	int IsNearlyZero() const;
	UBOOL IsZero() const;

	FRangeVector GridSnap(const FRangeVector& Grid);
	FRange& Component(INT Index);

	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FRangeVector& R )
	{
		return Ar << R.X << R.Y << R.Z;
	}
};


/*-----------------------------------------------------------------------------
	Bounds.
-----------------------------------------------------------------------------*/

//
// A rectangular minimum bounding volume.
//
class CORE_API FBox
{
public:
	// Variables.
	FVector Min;
	FVector Max;
	BYTE IsValid;

	// Constructors.
	FBox() {}
	FBox(INT);
	FBox(const FVector& InMin, const FVector& InMax);
	FBox( const FVector* Points, INT Count );

	// Accessors.
	FVector& GetExtrema(int i);
	const FVector& GetExtrema(int i) const;

	// Functions.
	void Init();

	FBox& operator+=(const FVector &Other);
	FBox operator+(const FVector& Other) const;
	FBox& operator+=(const FBox& Other);
	FBox operator+(const FBox& Other) const;
	FVector& operator[](INT i);

	FBox TransformBy( const FMatrix& M ) const;
	FBox TransformBy(const FCoords& Coords) const;
	FBox ExpandBy(FLOAT W) const;
	// Returns the midpoint between the min and max points.
	FVector GetCenter() const;
	// Returns the extent around the center
	FVector GetExtent() const;

	void GetCenterAndExtents(FVector & center, FVector & Extents) const;
	bool Intersect(const FBox & other) const;
	
	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FBox& Bound )
	{
		return Ar << Bound.Min << Bound.Max << Bound.IsValid;
	}
};

/*-----------------------------------------------------------------------------
	FInterpCurve.
-----------------------------------------------------------------------------*/
class CORE_API FInterpCurvePoint
{
public:
	FLOAT	InVal;
	FLOAT	OutVal;

	FInterpCurvePoint();
	FInterpCurvePoint(FLOAT I, FLOAT O);

	UBOOL operator==(const FInterpCurvePoint &Other);

	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FInterpCurvePoint& Point )
	{
		return Ar << Point.InVal << Point.OutVal;
	}
};

class CORE_API FInterpCurve
{
public:
	TArray<FInterpCurvePoint>	Points;

	FInterpCurve();
	
	void	AddPoint( FLOAT inV, FLOAT outV );
	void	Reset( );
	FLOAT	Eval( FLOAT in);

	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FInterpCurve& Curve )
	{
		return Ar << Curve.Points;
	}

	// Assignment (copy)
	void operator=(const FInterpCurve &Other);
};


/*-----------------------------------------------------------------------------
	FGlobalMath.
-----------------------------------------------------------------------------*/

//
// Global mathematics info.
//
class CORE_API FGlobalMath
{
public:
	// Constants.
	enum {ANGLE_SHIFT 	= 2};		// Bits to right-shift to get lookup value.
	enum {ANGLE_BITS	= 14};		// Number of valid bits in angles.
	enum {NUM_ANGLES 	= 16384}; 	// Number of angles that are in lookup table.
	enum {NUM_SQRTS		= 16384};	// Number of square roots in lookup table.
	enum {ANGLE_MASK    =  (((1<<ANGLE_BITS)-1)<<(16-ANGLE_BITS))};

	// Class constants.
	const FVector  	WorldMin;
	const FVector  	WorldMax;
	const FCoords  	UnitCoords;
	const FScale   	UnitScale;
	const FCoords	ViewCoords;

	// Basic math functions.
	FLOAT Sqrt(int i);
	FORCEINLINE FLOAT SinTab(int i);
	FORCEINLINE FLOAT CosTab(int i);
	FLOAT SinFloat(FLOAT F);
	FLOAT CosFloat(FLOAT F);

	// Constructor.
	FGlobalMath();

private:
	// Tables.
	FLOAT  TrigFLOAT		[NUM_ANGLES];
	FLOAT  SqrtFLOAT		[NUM_SQRTS];
	FLOAT  LightSqrtFLOAT	[NUM_SQRTS];
};

inline INT ReduceAngle( INT Angle )
{
	return Angle & FGlobalMath::ANGLE_MASK;
};

/*-----------------------------------------------------------------------------
	Floating point constants.
-----------------------------------------------------------------------------*/

//
// Lengths of normalized vectors (These are half their maximum values
// to assure that dot products with normalized vectors don't overflow).
//
#define FLOAT_NORMAL_THRESH				(0.0001f)

//
// Magic numbers for numerical precision.
//
#define THRESH_POINT_ON_PLANE			(0.10f)		/* Thickness of plane for front/back/inside test */
#define THRESH_POINT_ON_SIDE			(0.20f)		/* Thickness of polygon side's side-plane for point-inside/outside/on side test */
#define THRESH_POINTS_ARE_SAME			(0.002f)	/* Two points are same if within this distance */
#define THRESH_POINTS_ARE_NEAR			(0.015f)	/* Two points are near if within this distance and can be combined if imprecise math is ok */
#define THRESH_NORMALS_ARE_SAME			(0.00002f)	/* Two normal points are same if within this distance */
													/* Making this too large results in incorrect CSG classification and disaster */
#define THRESH_VECTORS_ARE_NEAR			(0.0004f)	/* Two vectors are near if within this distance and can be combined if imprecise math is ok */
													/* Making this too large results in lighting problems due to inaccurate texture coordinates */
#define THRESH_SPLIT_POLY_WITH_PLANE	(0.25f)		/* A plane splits a polygon in half */
#define THRESH_SPLIT_POLY_PRECISELY		(0.01f)		/* A plane exactly splits a polygon */
#define THRESH_ZERO_NORM_SQUARED		(0.0001f)	/* Size of a unit normal that is considered "zero", squared */
#define THRESH_VECTORS_ARE_PARALLEL		(0.02f)		/* Vectors are parallel if dot product varies less than this */


// gam ---

//
//	FVerticesEqual
//

inline UBOOL FVerticesEqual(const FVector& V1, const FVector& V2)
{
	if(Abs(V1.X - V2.X) > THRESH_POINTS_ARE_SAME * 4.0f)
		return 0;

	if(Abs(V1.Y - V2.Y) > THRESH_POINTS_ARE_SAME * 4.0f)
		return 0;

	if(Abs(V1.Z - V2.Z) > THRESH_POINTS_ARE_SAME * 4.0f)
		return 0;

	return 1;
}
// --- gam

/*-----------------------------------------------------------------------------
	FVector friends.
-----------------------------------------------------------------------------*/

//
// Compare two points and see if they're the same, using a threshold.
// Returns 1=yes, 0=no.  Uses fast distance approximation.
//
inline int FPointsAreSame( const FVector &P, const FVector &Q )
{
	FLOAT Temp;
	Temp=P.X-Q.X;
	if( (Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME) )
	{
		Temp=P.Y-Q.Y;
		if( (Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME) )
		{
			Temp=P.Z-Q.Z;
			if( (Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME) )
			{
				return 1;
			}
		}
	}
	return 0;
}

//
// Compare two points and see if they're the same, using a threshold.
// Returns 1=yes, 0=no.  Uses fast distance approximation.
//
inline int FPointsAreNear( const FVector &Point1, const FVector &Point2, FLOAT Dist )
{
	FLOAT Temp;
	Temp=(Point1.X - Point2.X); if (Abs(Temp)>=Dist) return 0;
	Temp=(Point1.Y - Point2.Y); if (Abs(Temp)>=Dist) return 0;
	Temp=(Point1.Z - Point2.Z); if (Abs(Temp)>=Dist) return 0;
	return 1;
}

//
// Calculate the signed distance (in the direction of the normal) between
// a point and a plane.
//
inline FLOAT FPointPlaneDist
(
	const FVector &Point,
	const FVector &PlaneBase,
	const FVector &PlaneNormal
)
{
	return (Point - PlaneBase) | PlaneNormal;
}

//
// Euclidean distance between two points.
//
inline FLOAT FDist( const FVector &V1, const FVector &V2 )
{
	return appSqrt( Square(V2.X-V1.X) + Square(V2.Y-V1.Y) + Square(V2.Z-V1.Z) );
}

//
// Squared distance between two points.
//
inline FLOAT FDistSquared( const FVector &V1, const FVector &V2 )
{
	return Square(V2.X-V1.X) + Square(V2.Y-V1.Y) + Square(V2.Z-V1.Z);
}

//
// See if two normal vectors (or plane normals) are nearly parallel.
//
inline int FParallel( const FVector &Normal1, const FVector &Normal2 )
{
	FLOAT NormalDot = Normal1 | Normal2;
	return (Abs (NormalDot - 1.f) <= THRESH_VECTORS_ARE_PARALLEL);
}

//
// See if two planes are coplanar.
//
inline int FCoplanar( const FVector &Base1, const FVector &Normal1, const FVector &Base2, const FVector &Normal2 )
{
	if      (!FParallel(Normal1,Normal2)) return 0;
	else if (FPointPlaneDist (Base2,Base1,Normal1) > THRESH_POINT_ON_PLANE) return 0;
	else    return 1;
}

//
// Triple product of three vectors.
//
inline FLOAT FTriple( const FVector& X, const FVector& Y, const FVector& Z )
{
	return
	(	(X.X * (Y.Y * Z.Z - Y.Z * Z.Y))
	+	(X.Y * (Y.Z * Z.X - Y.X * Z.Z))
	+	(X.Z * (Y.X * Z.Y - Y.Y * Z.X)) );
}

/*-----------------------------------------------------------------------------
	Random numbers.
-----------------------------------------------------------------------------*/

//
// Compute pushout of a box from a plane.
//
#ifdef _MSC_VER // sjs - IEEE float abuse
const int FPSignBitMask = ~(1 << 31); // sjs
inline FLOAT FBoxPushOut( const FVector & Normal, const FVector & Size )
{
    float dx = Normal.X*Size.X;
    float dy = Normal.Y*Size.Y;
    float dz = Normal.Z*Size.Z;
    *(int*)(&dx) &= FPSignBitMask;
    *(int*)(&dy) &= FPSignBitMask;
    *(int*)(&dz) &= FPSignBitMask;
    return dx+dy+dz;
}
#else
inline FLOAT FBoxPushOut( const FVector & Normal, const FVector & Size )
{
    return Abs(Normal.X*Size.X) + Abs(Normal.Y*Size.Y) + Abs(Normal.Z*Size.Z);
}
#endif

//
// Return a uniformly distributed random unit vector.
//
inline FVector VRand()
{
	FVector Result;
	do
	{
		// Check random vectors in the unit sphere so result is statistically uniform.
		Result.X = appFrand()*2 - 1;
		Result.Y = appFrand()*2 - 1;
		Result.Z = appFrand()*2 - 1;
	} while( Result.SizeSquared() > 1.f );
	return Result.UnsafeNormal();
}

// sjs ---
// quick and dirty random numbers ( > 10x faster than rand() and 100x worse! )
extern CORE_API unsigned long qRandSeed;
const float INV_MAX_QUICK_RAND = 1.0f/0xffff;

inline void qSeedRand( unsigned int inSeed )
{
	qRandSeed = inSeed;
}

inline unsigned int qRand( void )
{
   qRandSeed = (qRandSeed * 196314165) + 907633515;
   return (qRandSeed >> 17);
}

inline float qFRand( void )
{
   qRandSeed = (qRandSeed * 196314165) + 907633515;
   return (float)(qRandSeed>>16) * INV_MAX_QUICK_RAND;
}
// --- sjs

/*-----------------------------------------------------------------------------
	Texturing.
-----------------------------------------------------------------------------*/


// Returns the UV texture coordinates for the specified vertex.
inline void FVectorsToTexCoords( FVector InVtx, FVector InPolyBase, FVector InTextureU, FVector InTextureV, FLOAT InMaterialUSize, FLOAT InMaterialVSize, FLOAT* InU, FLOAT* InV )
{
	*InU = ((InVtx - InPolyBase) | InTextureU) / InMaterialUSize;
	*InV = ((InVtx - InPolyBase) | InTextureV) / InMaterialVSize;
}

// Accepts a triangle (XYZ and UV values for each point) and returns a poly base and UV vectors
// NOTE : the UV coords should be scaled by the texture size
inline void FTexCoordsToVectors( FVector V0, FVector UV0, FVector V1, FVector UV1, FVector V2, FVector UV2, FVector* InBaseResult, FVector* InUResult, FVector* InVResult )
{
	guard(FTexCoordsToVectors);

	// Create polygon normal.
	FVector PN = FVector((V0-V1) ^ (V2-V0));
	PN = PN.SafeNormal();

	// Fudge UV's to make sure no infinities creep into UV vector math, whenever we detect identical U or V's.
	if( ( UV0.X == UV1.X ) || ( UV2.X == UV1.X ) || ( UV2.X == UV0.X ) ||
		( UV0.Y == UV1.Y ) || ( UV2.Y == UV1.Y ) || ( UV2.Y == UV0.Y ) )
	{
		UV1 += FVector(0.004173f,0.004123f,0.0f);
		UV2 += FVector(0.003173f,0.003123f,0.0f);
	}

	//
	// Solve the equations to find our texture U/V vectors 'TU' and 'TV' by stacking them 
	// into a 3x3 matrix , one for  u(t) = TU dot (x(t)-x(o) + u(o) and one for v(t)=  TV dot (.... , 
	// then the third assumes we're perpendicular to the normal. 
	//
	FCoords TexEqu; 
	TexEqu.XAxis = FVector(	V1.X - V0.X, V1.Y - V0.Y, V1.Z - V0.Z );
	TexEqu.YAxis = FVector( V2.X - V0.X, V2.Y - V0.Y, V2.Z - V0.Z );
	TexEqu.ZAxis = FVector( PN.X,        PN.Y,        PN.Z        );
	TexEqu.Origin =FVector( 0.0f, 0.0f, 0.0f );
	TexEqu = TexEqu.Inverse();

	FVector UResult( UV1.X-UV0.X, UV2.X-UV0.X, 0.0f );
	FVector TUResult = UResult.TransformVectorBy( TexEqu );

	FVector VResult( UV1.Y-UV0.Y, UV2.Y-UV0.Y, 0.0f );
	FVector TVResult = VResult.TransformVectorBy( TexEqu );

	//
	// Adjust the BASE to account for U0 and V0 automatically, and force it into the same plane.
	//				
	FCoords BaseEqu;
	BaseEqu.XAxis = TUResult;
	BaseEqu.YAxis = TVResult; 
	BaseEqu.ZAxis = FVector( PN.X, PN.Y, PN.Z );
	BaseEqu.Origin = FVector( 0.0f, 0.0f, 0.0f );

	FVector BResult = FVector( UV0.X - ( TUResult|V0 ), UV0.Y - ( TVResult|V0 ),  0.0f );

	*InBaseResult = - 1.0f *  BResult.TransformVectorBy( BaseEqu.Inverse() );
	*InUResult = TUResult;
	*InVResult = TVResult;

	unguard;
}

/*
	FProjectTextureToPlane
	Projects a texture coordinate system onto a plane.
*/

inline void FProjectTextureToPlane(FVector& Base,FVector& X,FVector& Y,FPlane Plane)
{
	guard(FTexCoordsProjectToPlane);

	// Calculate a vector perpendicular to the texture(the texture normal).
	// Moving the texture base along this vector doesn't affect texture mapping.

	FVector	Z = (X ^ Y).SafeNormal();

	// Calculate the ratio of distance along the plane normal to distance along the texture normal.

	FLOAT	Ratio = 1.0f / (Z | Plane);

	// Project each component of the texture coordinate system onto the plane.

	Base = Base - Z * Plane.PlaneDot(Base) * Ratio;
	X = X - Z * (X | Plane) * Ratio;
	Y = Y - Z * (Y | Plane) * Ratio;

	unguard;
}

/*-----------------------------------------------------------------------------
	Advanced geometry.
-----------------------------------------------------------------------------*/

//
// Find the intersection of an infinite line (defined by two points) and
// a plane.  Assumes that the line and plane do indeed intersect; you must
// make sure they're not parallel before calling.
//
inline FVector FLinePlaneIntersection
(
	const FVector &Point1,
	const FVector &Point2,
	const FVector &PlaneOrigin,
	const FVector &PlaneNormal
)
{
	return
		Point1
	+	(Point2-Point1)
	*	(((PlaneOrigin - Point1)|PlaneNormal) / ((Point2 - Point1)|PlaneNormal));
}
inline FVector FLinePlaneIntersection
(
	const FVector &Point1,
	const FVector &Point2,
	const FPlane  &Plane
)
{
	return
		Point1
	+	(Point2-Point1)
	*	((Plane.W - (Point1|Plane))/((Point2 - Point1)|Plane));
}

//
// Determines whether a point is inside a box.
//

inline UBOOL FPointBoxIntersection
(
	const FVector&	Point,
	const FBox&		Box
)
{
	if(Point.X >= Box.Min.X && Point.X <= Box.Max.X &&
	   Point.Y >= Box.Min.Y && Point.Y <= Box.Max.Y &&
	   Point.Z >= Box.Min.Z && Point.Z <= Box.Max.Z)
		return 1;
	else
		return 0;
}

//
// Determines whether a line intersects a box.
//

#define BOX_SIDE_THRESHOLD	0.1f

inline UBOOL FLineBoxIntersection
(
	const FBox&		Box,
	const FVector&	Start,
	const FVector&	End,
	const FVector&	Direction,
	const FVector&	OneOverDirection
)
{
	FVector	Time;
	UBOOL	Inside = 1;

	if(Start.X < Box.Min.X)
	{
		if(Direction.X <= 0.0f)
			return 0;
		else
		{
			Inside = 0;
			Time.X = (Box.Min.X - Start.X) * OneOverDirection.X;
		}
	}
	else if(Start.X > Box.Max.X)
	{
		if(Direction.X >= 0.0f)
			return 0;
		else
		{
			Inside = 0;
			Time.X = (Box.Max.X - Start.X) * OneOverDirection.X;
		}
	}
	else
		Time.X = 0.0f;

	if(Start.Y < Box.Min.Y)
	{
		if(Direction.Y <= 0.0f)
			return 0;
		else
		{
			Inside = 0;
			Time.Y = (Box.Min.Y - Start.Y) * OneOverDirection.Y;
		}
	}
	else if(Start.Y > Box.Max.Y)
	{
		if(Direction.Y >= 0.0f)
			return 0;
		else
		{
			Inside = 0;
			Time.Y = (Box.Max.Y - Start.Y) * OneOverDirection.Y;
		}
	}
	else
		Time.Y = 0.0f;

	if(Start.Z < Box.Min.Z)
	{
		if(Direction.Z <= 0.0f)
			return 0;
		else
		{
			Inside = 0;
			Time.Z = (Box.Min.Z - Start.Z) * OneOverDirection.Z;
		}
	}
	else if(Start.Z > Box.Max.Z)
	{
		if(Direction.Z >= 0.0f)
			return 0;
		else
		{
			Inside = 0;
			Time.Z = (Box.Max.Z - Start.Z) * OneOverDirection.Z;
		}
	}
	else
		Time.Z = 0.0f;

	if(Inside)
		return 1;
	else
	{
		FLOAT	MaxTime = Max(Time.X,Max(Time.Y,Time.Z));

		if(MaxTime >= 0.0f && MaxTime <= 1.0f)
		{
			FVector	Hit = Start + Direction * MaxTime;

			if(	Hit.X > Box.Min.X - BOX_SIDE_THRESHOLD && Hit.X < Box.Max.X + BOX_SIDE_THRESHOLD &&
				Hit.Y > Box.Min.Y - BOX_SIDE_THRESHOLD && Hit.Y < Box.Max.Y + BOX_SIDE_THRESHOLD &&
				Hit.Z > Box.Min.Z - BOX_SIDE_THRESHOLD && Hit.Z < Box.Max.Z + BOX_SIDE_THRESHOLD)
				return 1;
		}

		return 0;
	}
}

CORE_API UBOOL FLineExtentBoxIntersection(const FBox& inBox, 
								 const FVector& Start, 
								 const FVector& End,
								 const FVector& Extent,
								 FVector& HitLocation,
								 FVector& HitNormal,
								 FLOAT& HitTime);

//
// Determines whether a line intersects a sphere.
//

inline UBOOL FLineSphereIntersection(FVector Start,FVector Dir,FLOAT Length,FVector Origin,FLOAT Radius)
{
	FVector	EO = Start - Origin;
	FLOAT	v = (Dir | (Origin - Start)),
			disc = Radius * Radius - ((EO | EO) - v * v);

	if(disc >= 0.0f)
	{
		FLOAT	Time = (v - appSqrt(disc)) / Length;

		if(Time >= 0.0f && Time <= 1.0f)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

/*-----------------------------------------------------------------------------
	FPlane functions.
-----------------------------------------------------------------------------*/

//
// Compute intersection point of three planes.
// Return 1 if valid, 0 if infinite.
//
inline UBOOL FIntersectPlanes3( FVector& I, const FPlane& P1, const FPlane& P2, const FPlane& P3 )
{
	guard(FIntersectPlanes3);

	// Compute determinant, the triple product P1|(P2^P3)==(P1^P2)|P3.
	FLOAT Det = (P1 ^ P2) | P3;
	if( Square(Det) < Square(0.001f) )
	{
		// Degenerate.
		I = FVector(0,0,0);
		return 0;
	}
	else
	{
		// Compute the intersection point, guaranteed valid if determinant is nonzero.
		I = (P1.W*(P2^P3) + P2.W*(P3^P1) + P3.W*(P1^P2)) / Det;
	}
	return 1;
	unguard;
}

//
// Compute intersection point and direction of line joining two planes.
// Return 1 if valid, 0 if infinite.
//
inline UBOOL FIntersectPlanes2( FVector& I, FVector& D, const FPlane& P1, const FPlane& P2 )
{
	guard(FIntersectPlanes2);

	// Compute line direction, perpendicular to both plane normals.
	D = P1 ^ P2;
	FLOAT DD = D.SizeSquared();
	if( DD < Square(0.001f) )
	{
		// Parallel or nearly parallel planes.
		D = I = FVector(0,0,0);
		return 0;
	}
	else
	{
		// Compute intersection.
		I = (P1.W*(P2^D) + P2.W*(D^P1)) / DD;
		D.Normalize();
		return 1;
	}
	unguard;
}

/*-----------------------------------------------------------------------------
	FQuat.          
-----------------------------------------------------------------------------*/

// floating point quaternion.
class CORE_API FQuat 
{
	public:
	// Variables.
	FLOAT X,Y,Z,W;
	// X,Y,Z, W also doubles as the Axis/Angle format.

	// Constructors.
	FQuat();
	FQuat(FLOAT InX, FLOAT InY, FLOAT InZ, FLOAT InA);

	// Binary operators.
	FQuat operator+(const FQuat& Q) const;
	FQuat operator-(const FQuat& Q) const;

	FQuat operator*(const FQuat& Q) const;
	FQuat operator*(const FLOAT& Scale) const;

	// Unary operators.
	FQuat operator-() const;

    // Misc operators;
	
	UBOOL Normalize();

	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FQuat& F )
	{
		return Ar << F.X << F.Y << F.Z << F.W;
	}

	// Warning : assumes normalized quaternions.
	FQuat FQuatToAngAxis();

	//
	// Angle-Axis to Quaternion. No normalized axis assumed.
	//
	FQuat AngAxisToFQuat();

	FVector RotateVector(FVector v);
};

// Dot product of axes to get cos of angle  #Warning some people use .W component here too !
inline FLOAT FQuatDot(const FQuat& Q1,const FQuat& Q2)
{
	return( Q1.X*Q2.X + Q1.Y*Q2.Y + Q1.Z*Q2.Z );
};

// Error measure (angle) between two quaternions, ranged [0..1]
inline FLOAT FQuatError(FQuat& Q1,FQuat& Q2)
{
	// Returns the hypersphere-angle between two quaternions; alignment shouldn't matter, though 
	// normalized input is expected.
	FLOAT cosom = Q1.X*Q2.X + Q1.Y*Q2.Y + Q1.Z*Q2.Z + Q1.W*Q2.W;
	return (Abs(cosom) < 0.9999999f) ? appAcos(cosom)*(1.f/PI) : 0.0f;
}

// Ensure quat1 points to same side of the hypersphere as quat2
inline void AlignFQuatWith(FQuat &quat1, const FQuat &quat2)
{
	FLOAT Minus  = Square(quat1.X-quat2.X) + Square(quat1.Y-quat2.Y) + Square(quat1.Z-quat2.Z) + Square(quat1.W-quat2.W);
	FLOAT Plus   = Square(quat1.X+quat2.X) + Square(quat1.Y+quat2.Y) + Square(quat1.Z+quat2.Z) + Square(quat1.W+quat2.W);

	if (Minus > Plus)
	{
		quat1.X = - quat1.X;
		quat1.Y = - quat1.Y;
		quat1.Z = - quat1.Z;
		quat1.W = - quat1.W;
	}
}

// No-frills spherical interpolation. Assumes aligned quaternions, and the output is not normalized.
inline FQuat SlerpQuat(const FQuat &quat1,const FQuat &quat2, float slerp)
{
	FQuat result;
	float omega,cosom,sininv,scale0,scale1;

	// Get cosine of angle betweel quats.
	cosom = quat1.X * quat2.X +
			quat1.Y * quat2.Y +
			quat1.Z * quat2.Z +
			quat1.W * quat2.W;

	if( cosom < 0.99999999f )
	{	
		omega = appAcos(cosom);
		sininv = 1.f/appSin(omega);
		scale0 = appSin((1.f - slerp) * omega) * sininv;
		scale1 = appSin(slerp * omega) * sininv;
		
		result.X = scale0 * quat1.X + scale1 * quat2.X;
		result.Y = scale0 * quat1.Y + scale1 * quat2.Y;
		result.Z = scale0 * quat1.Z + scale1 * quat2.Z;
		result.W = scale0 * quat1.W + scale1 * quat2.W;
		return result;
	}
	else
	{
		return quat1;
	}
	
}

/*-----------------------------------------------------------------------------
	FMatrix classes.
-----------------------------------------------------------------------------*/

/*
	FMatrix
	Floating point 4x4 matrix
*/

class CORE_API FMatrix
{
public:
	static FMatrix	Identity;

	// Variables.
	FLOAT M[4][4];

	// Constructors.
	FORCEINLINE FMatrix();
	FORCEINLINE FMatrix(FPlane InX, FPlane InY, FPlane InZ, FPlane InW);

	// Destructor.
	~FMatrix();

	void SetIdentity();

	// Concatenation operator.
	FORCEINLINE FMatrix operator*(FMatrix Other) const;
	FORCEINLINE void operator*=(FMatrix Other);

	// Comparison operators.
	inline UBOOL operator==(FMatrix& Other) const;

	inline UBOOL operator!=(FMatrix& Other) const;

	// Homogeneous transform.
	FORCEINLINE FPlane TransformFPlane(const FPlane &P) const;

	// Regular transform.
	FORCEINLINE FVector TransformFVector(const FVector &V) const;

	// Normal transform.
	FORCEINLINE FPlane TransformNormal(const FVector& V) const;

	// Transpose.
	FORCEINLINE FMatrix Transpose();

	// Determinant.

	inline FLOAT Determinant() const;
	FMatrix Inverse();
	FMatrix TransposeAdjoint() const;

	// Conversions.
	FCoords Coords();

	// Serializer.

	friend FArchive& operator<<(FArchive& Ar,FMatrix& M)
	{
		return Ar << 
			M.M[0][0] << M.M[0][1] << M.M[0][2] << M.M[0][3] << 
			M.M[1][0] << M.M[1][1] << M.M[1][2] << M.M[1][3] << 
			M.M[2][0] << M.M[2][1] << M.M[2][2] << M.M[2][3] << 
			M.M[3][0] << M.M[3][1] << M.M[3][2] << M.M[3][3];
	}
};

// Matrix operations.

class FPerspectiveMatrix : public FMatrix
{
public:

	FPerspectiveMatrix(float FOVX, float FOVY, float MultFOVX, float MultFOVY, float MinZ, float MaxZ) :
	  FMatrix(
			FPlane(MultFOVX / appTan(FOVX),		0.0f,							0.0f,							0.0f),
			FPlane(0.0f,						MultFOVY / appTan(FOVY),		0.0f,							0.0f),
			FPlane(0.0f,						0.0f,							MaxZ / (MaxZ - MinZ),			1.0f),
			FPlane(0.0f,						0.0f,							-MinZ * (MaxZ / (MaxZ - MinZ)),	0.0f))
	{
	}

	FPerspectiveMatrix(float FOV, float Width, float Height, float MinZ, float MaxZ) :
	  FMatrix(
			FPlane(1.0f / appTan(FOV),	0.0f,							0.0f,							0.0f),
			FPlane(0.0f,				Width / appTan(FOV) / Height,	0.0f,							0.0f),
			FPlane(0.0f,				0.0f,							MaxZ / (MaxZ - MinZ),			1.0f),
			FPlane(0.0f,				0.0f,							-MinZ * (MaxZ / (MaxZ - MinZ)),	0.0f))
	{
	}
};

class FOrthoMatrix : public FMatrix
{
public:

	FOrthoMatrix(float Width,float Height,float ZScale,float ZOffset) :
		FMatrix(
			FPlane(1.0f / Width,	0.0f,			0.0f,				0.0f),
			FPlane(0.0f,			1.0f / Height,	0.0f,				0.0f),
			FPlane(0.0f,			0.0f,			ZScale,				0.0f),
			FPlane(0.0f,			0.0f,			ZOffset * ZScale,	1.0f))
	{
	}
};

class FTranslationMatrix : public FMatrix
{
public:

	FTranslationMatrix(FVector Delta) :
		FMatrix(
			FPlane(1.0f,	0.0f,	0.0f,	0.0f),
			FPlane(0.0f,	1.0f,	0.0f,	0.0f),
			FPlane(0.0f,	0.0f,	1.0f,	0.0f),
			FPlane(Delta.X,	Delta.Y,Delta.Z,1.0f))
	{
	}
};

class FRotationMatrix : public FMatrix
{
public:

#if 0
	FRotationMatrix(FRotator Rot) :
	  FMatrix(
			FMatrix(	// Roll
				FPlane(1.0f,					0.0f,					0.0f,						0.0f),
				FPlane(0.0f,					+GMath.CosTab(Rot.Roll),-GMath.SinTab(Rot.Roll),	0.0f),
				FPlane(0.0f,					+GMath.SinTab(Rot.Roll),+GMath.CosTab(Rot.Roll),	0.0f),
				FPlane(0.0f,					0.0f,					0.0f,						1.0f)) *
			FMatrix(	// Pitch
				FPlane(+GMath.CosTab(Rot.Pitch),0.0f,					+GMath.SinTab(Rot.Pitch),	0.0f),
				FPlane(0.0f,					1.0f,					0.0f,						0.0f),
				FPlane(-GMath.SinTab(Rot.Pitch),0.0f,					+GMath.CosTab(Rot.Pitch),	0.0f),
				FPlane(0.0f,					0.0f,					0.0f,						1.0f)) *
			FMatrix(	// Yaw
				FPlane(+GMath.CosTab(Rot.Yaw),	+GMath.SinTab(Rot.Yaw), 0.0f,	0.0f),
				FPlane(-GMath.SinTab(Rot.Yaw),	+GMath.CosTab(Rot.Yaw), 0.0f,	0.0f),
				FPlane(0.0f,					0.0f,					1.0f,	0.0f),
				FPlane(0.0f,					0.0f,					0.0f,	1.0f))
			)
	  {
	  }
#else
	FRotationMatrix(FRotator Rot)
	{
		FLOAT	SR	= GMath.SinTab(Rot.Roll),
				SP	= GMath.SinTab(Rot.Pitch),
				SY	= GMath.SinTab(Rot.Yaw),
				CR	= GMath.CosTab(Rot.Roll),
				CP	= GMath.CosTab(Rot.Pitch),
				CY	= GMath.CosTab(Rot.Yaw);

		M[0][0]	= CP * CY;
		M[0][1]	= CP * SY;
		M[0][2]	= SP;
		M[0][3]	= 0.f;

		M[1][0]	= SR * SP * CY - CR * SY;
		M[1][1]	= SR * SP * SY + CR * CY;
		M[1][2]	= - SR * CP;
		M[1][3]	= 0.f;

		M[2][0]	= -( CR * SP * CY + SR * SY );
		M[2][1]	= CY * SR - CR * SP * SY;
		M[2][2]	= CR * CP;
		M[2][3]	= 0.f;

		M[3][0]	= 0.f;
		M[3][1]	= 0.f;
		M[3][2]	= 0.f;
		M[3][3]	= 1.f;
	}
#endif
};

class FInverseRotationMatrix : public FMatrix
{
public:

	FInverseRotationMatrix(FRotator Rot) :
		FMatrix(
			FMatrix(	// Yaw
				FPlane(+GMath.CosTab(-Rot.Yaw),	+GMath.SinTab(-Rot.Yaw), 0.0f,	0.0f),
				FPlane(-GMath.SinTab(-Rot.Yaw),	+GMath.CosTab(-Rot.Yaw), 0.0f,	0.0f),
				FPlane(0.0f,					0.0f,					1.0f,	0.0f),
				FPlane(0.0f,					0.0f,					0.0f,	1.0f)) *
			FMatrix(	// Pitch
				FPlane(+GMath.CosTab(-Rot.Pitch),0.0f,					+GMath.SinTab(-Rot.Pitch),	0.0f),
				FPlane(0.0f,					1.0f,					0.0f,						0.0f),
				FPlane(-GMath.SinTab(-Rot.Pitch),0.0f,					+GMath.CosTab(-Rot.Pitch),	0.0f),
				FPlane(0.0f,					0.0f,					0.0f,						1.0f)) *
			FMatrix(	// Roll
				FPlane(1.0f,					0.0f,					0.0f,						0.0f),
				FPlane(0.0f,					+GMath.CosTab(-Rot.Roll),-GMath.SinTab(-Rot.Roll),	0.0f),
				FPlane(0.0f,					+GMath.SinTab(-Rot.Roll),+GMath.CosTab(-Rot.Roll),	0.0f),
				FPlane(0.0f,					0.0f,					0.0f,						1.0f))
			)
	{
	}
};

class FQuaternionMatrix : public FMatrix
{
public:

	FQuaternionMatrix(FQuat Q)
	{
		FLOAT wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

		x2 = Q.X + Q.X;  y2 = Q.Y + Q.Y;  z2 = Q.Z + Q.Z;
		xx = Q.X * x2;   xy = Q.X * y2;   xz = Q.X * z2;
		yy = Q.Y * y2;   yz = Q.Y * z2;   zz = Q.Z * z2;
		wx = Q.W * x2;   wy = Q.W * y2;   wz = Q.W * z2;

		M[0][0] = 1.0f - (yy + zz);
		M[1][0] = xy - wz;
		M[2][0] = xz + wy;
		M[3][0] = 0.0f;

		M[0][1] = xy + wz;
		M[1][1] = 1.0f - (xx + zz);
		M[2][1] = yz - wx;
		M[3][1] = 0.0f;

		M[0][2] = xz - wy;
		M[1][2] = yz + wx;
		M[2][2] = 1.0f - (xx + yy);
		M[3][2] = 0.0f;

		M[0][3] = 0.0f;
		M[1][3] = 0.0f;
		M[2][3] = 0.0f;
		M[3][3] = 1.0f;
	}
};

class FScaleMatrix : public FMatrix
{
public:

	FScaleMatrix(FVector Scale) :
		FMatrix(
			FPlane(Scale.X,	0.0f,		0.0f,		0.0f),
			FPlane(0.0f,	Scale.Y,	0.0f,		0.0f),
			FPlane(0.0f,	0.0f,		Scale.Z,	0.0f),
			FPlane(0.0f,	0.0f,		0.0f,		1.0f))
	{
	}
};


// Transform a (rotation) matrix into a Quaternion.
class FMatrixQuaternion : public FQuat
{
public:
	FMatrixQuaternion( FMatrix M )
	{
		// Trace.
		FLOAT Trace = M.M[0][0] + M.M[1][1] + M.M[2][2] + 1.0f;
		// Calculate directly for positive trace.
		if( Trace > 0.f)
		{
			 FLOAT S = 0.5f / appSqrt(Trace);
			 W = 0.25f / S;
			 X = ( M.M[1][2] - M.M[2][1] ) * S;
			 Y = ( M.M[2][0] - M.M[0][2] ) * S;
			 Z = ( M.M[0][1] - M.M[1][0] ) * S;
			 return;
		}
		// Or determine the major diagonal element.
		if( (M.M[0][0] > M.M[1][1]) &&  (M.M[0][0] > M.M[2][2]) )
		{
			FLOAT SZ = 0.5f/appSqrt( 1.0f + M.M[0][0] - M.M[1][1] - M.M[2][2] );
			X = 0.5f * SZ;
			Y = (M.M[1][0] + M.M[0][1] ) * SZ;
			Z = (M.M[2][0] + M.M[0][2] ) * SZ;
			W = (M.M[2][1] + M.M[1][2] ) * SZ;			
		}
		else if( M.M[1][1] > M.M[2][2] )
		{
			FLOAT SZ = 0.5f/appSqrt( 1.0f + M.M[1][1] - M.M[0][0] - M.M[2][2] );
			X = (M.M[1][0] + M.M[0][1] ) * SZ;
			Y = 0.5f * SZ;
			Z = (M.M[2][1] + M.M[1][2] ) * SZ;
			W = (M.M[2][0] + M.M[0][2] ) * SZ;
		}
		else 
		{			  
			FLOAT SZ = 0.5f/appSqrt( 1.0f + M.M[2][2] - M.M[0][0] - M.M[1][1] );
			X = (M.M[2][0] + M.M[0][2] ) * SZ;
			Y = (M.M[2][1] + M.M[1][2] ) * SZ;
			Z = 0.5f * SZ;
			W = (M.M[1][0] + M.M[0][1] ) * SZ;
		}
	}
};

// Transform a (rotation) FCoords into a Quaternion.
class FCoordsQuaternion : public FQuat
{
public:
	FCoordsQuaternion( FCoords C )
	{
		// Trace.
		FLOAT Trace = C.XAxis.X + C.YAxis.Y + C.ZAxis.Z + 1.0f;
		// Calculate directly for positive trace.
		if( Trace > 0.f)
		{
			 FLOAT S = 0.5f / appSqrt(Trace);
			 W = 0.25f / S;
			 X = ( C.ZAxis.Y - C.YAxis.Z ) * S;
			 Y = ( C.XAxis.Z - C.ZAxis.X ) * S;
			 Z = ( C.YAxis.X - C.XAxis.Y ) * S;
			 return;
		}
		// Or determine the major diagonal element.
		if( (C.XAxis.X > C.YAxis.Y) &&  (C.XAxis.X > C.ZAxis.Z) )
		{
			FLOAT SZ = 0.5f/appSqrt( 1.0f + C.XAxis.X - C.YAxis.Y - C.ZAxis.Z );
			X = 0.5f * SZ;
			Y = (C.XAxis.Y + C.YAxis.X ) * SZ;
			Z = (C.XAxis.Z + C.ZAxis.X ) * SZ;
			W = (C.YAxis.Z + C.ZAxis.Y ) * SZ;			
		}
		else if( C.YAxis.Y > C.ZAxis.Z )
		{
			FLOAT SZ = 0.5f/appSqrt( 1.0f + C.YAxis.Y - C.XAxis.X - C.ZAxis.Z );
			X = (C.XAxis.Y + C.YAxis.X ) * SZ;
			Y = 0.5f * SZ;
			Z = (C.YAxis.Z + C.ZAxis.Y ) * SZ;
			W = (C.XAxis.Z + C.ZAxis.X ) * SZ;
		}
		else 
		{			  
			FLOAT SZ = 0.5f/appSqrt( 1.0f + C.ZAxis.Z - C.XAxis.X - C.YAxis.Y );
			X = (C.XAxis.Z + C.ZAxis.X ) * SZ;
			Y = (C.YAxis.Z + C.ZAxis.Y ) * SZ;
			Z = 0.5f * SZ;
			W = (C.XAxis.Y + C.YAxis.X ) * SZ;
		}
	}
};


class FQuaternionCoords : public FCoords
{
public:
	FQuaternionCoords(FQuat Q)
	{
		FLOAT wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

		x2 = Q.X + Q.X;  y2 = Q.Y + Q.Y;  z2 = Q.Z + Q.Z;
		xx = Q.X * x2;   xy = Q.X * y2;   xz = Q.X * z2;
		yy = Q.Y * y2;   yz = Q.Y * z2;   zz = Q.Z * z2;
		wx = Q.W * x2;   wy = Q.W * y2;   wz = Q.W * z2;

		XAxis.X = 1.0f - (yy + zz);
		XAxis.Y = xy - wz;
		XAxis.Z = xz + wy;		

		YAxis.X = xy + wz;
		YAxis.Y = 1.0f - (xx + zz);
		YAxis.Z = yz - wx;
		
		ZAxis.X = xz - wy;
		ZAxis.Y = yz + wx;
		ZAxis.Z = 1.0f - (xx + yy);
		
		Origin.X = 0.0f;
		Origin.Y = 0.0f;
		Origin.Z = 0.0f;
	}
};

/*-----------------------------------------------------------------------------
	FPlane implementation.
-----------------------------------------------------------------------------*/

//
// Transform a point by a coordinate system, moving
// it by the coordinate system's origin if nonzero.
//
inline FPlane FPlane::TransformPlaneByOrtho( const FCoords &Coords ) const
{
	FVector Normal = TransformVectorBy(Coords);
	return FPlane( Normal, W - (Coords.Origin.TransformVectorBy(Coords) | Normal) );
}

inline FPlane FPlane::TransformBy( const FCoords &Coords ) const
{
	return FPlane((*this * W).TransformPointBy(Coords),TransformVectorBy(Coords).SafeNormal());
}

inline FPlane FPlane::TransformPlaneByOrtho( const FMatrix& M ) const
{
	FVector Normal = M.TransformFPlane(FPlane(X,Y,Z,0));
	return FPlane( Normal, W - (M.TransformFVector(FVector(0,0,0)) | Normal) );
}

inline FPlane FPlane::TransformBy( const FMatrix& M ) const
{
	FMatrix tmpTA = M.TransposeAdjoint();
	return this->TransformByUsingAdjointT(M, tmpTA);
}

// You can optionally pass in the matrices transpose-adjoint, which save it recalculating it.
inline FPlane FPlane::TransformByUsingAdjointT( const FMatrix& M, const FMatrix& TA ) const
{
	FVector newNorm = TA.TransformNormal(*this).SafeNormal();

	if(M.Determinant() < 0.f)
		newNorm *= -1;

	return FPlane(M.TransformFVector(*this * W), newNorm);
}

inline FSphere FSphere::TransformBy(const FMatrix& M) const
{
	FSphere	Result;

	(FVector&)Result = M.TransformFVector(*this);

	FVector	XAxis(M.M[0][0],M.M[0][1],M.M[0][2]),
			YAxis(M.M[1][0],M.M[1][1],M.M[1][2]),
			ZAxis(M.M[2][0],M.M[2][1],M.M[2][2]);

	Result.W = appSqrt(MaxPositiveFloat(XAxis|XAxis,MaxPositiveFloat(YAxis|YAxis,ZAxis|ZAxis))) * W;

	return Result;
}

struct FCompressedPosition
{
	FVector Location;
	FRotator Rotation;
	FVector Velocity;
};

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

