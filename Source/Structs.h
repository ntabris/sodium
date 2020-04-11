#include "Prefs.h"


/*** Structs ***/
typedef struct
{
	Rect 	Name, Number, Weight, ElecConfig, Melt, Boil, Oxi, Neg, SHC, Ion,
			Disc, Radii, Density, Crystal;
} TextRects, *TextRectsPrt, **TextRectsHandle;

typedef struct
{
	long	min, max, items, current, old;
} ScrollType, *ScrollTypePtr, **ScrollTypeHandle;

typedef struct
{
	char 	q1s;
	char 	q2s;
	char 	q2p;
	char 	q3s;
	char 	q3p;
	char 	q3d;
	char 	q4s;
	char 	q4p;
	char 	q4d;
	char 	q4f;
	char 	q5s;
	char 	q5p;
	char 	q5d;
	char 	q5f;
	char 	q6s;
	char 	q6p;
	char 	q6d;
	char 	q6f;
	char 	q7s;
} Shell, *ShellPrt, **ShellHandle;

typedef struct
{
	int		List[kListMax];
	int		Sort;
	Boolean	Dirty;
} List;