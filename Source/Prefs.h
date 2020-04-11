/*** Preference Types ***/
typedef struct
{
	int	tempType;
} TempRes, *TempResPtr, **TempResHandle;

typedef struct
{
	RGBColor	color;
	Boolean 	enabled;
} HighlightRes, *HighlightResPtr, **HighlightResHandle;

typedef struct
{
	RGBColor	WeightColor;
	RGBColor	RadiiColor;
	RGBColor	MeltColor;
	RGBColor	BoilColor;
	RGBColor	IonizationColor;
	RGBColor	NegativityColor;
	Boolean 	Weight;
	Boolean 	Radii;
	Boolean 	Melt;
	Boolean 	Boil;
	Boolean 	Ionization;
	Boolean 	Negativity;
	Boolean		Lines;
} GraphRes, *GraphResPtr, **GraphResHandle;

typedef struct
{
	Rect	TableRect;
	Rect	ElementRect;
	Rect	GraphRect;
} RectRes, *RectResPtr, **RectResHandle;

typedef struct
{
	Boolean 	Radii;
	Boolean 	Melt;
	Boolean 	Boil;
	Boolean 	Ionization;
	Boolean 	Negativity;
} GradientRes, *GradientResPtr, **GradientResHandle;

typedef struct
{
	Boolean		Pict;
	Boolean		Center;
} PrintRes, *PrintResPtr, **PrintResHandle;

typedef struct
{
	int			style;
	Boolean		classic;
} TableRes, *TableResPtr, **TableResHandle;

typedef struct
{
	Boolean		Table;
	Boolean		Element;
} SizeRes, *SizeResPtr, **SizeResHandle;

typedef struct
{
	Boolean 	Table, Element, Graph;
} WindMove, *WindMovePrt, **WindMoveHandle;

typedef struct
{
	Boolean		name, number, mass, melt, boil, neg, shc, ion, radii, density, cstruct, symbol;
} ListRes, *ListResPtr, **ListResHandle;

typedef	struct
{
	Rect	List, CalcRect;
} RectRes2, *RectRes2Ptr, **RectRes2Handle;