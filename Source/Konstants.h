#ifdef powerc
typedef	long double		xvar;
#else
typedef	extended80		xvar;
#endif

/** OffScreen Stuff **/
#define WINBITMAP(w)		((((WindowPeek)(w))->port).portBits)
#define PORTBITMAP(w)		(((GrafPtr)(w))->portBits)

/*** Temperature Types **/
#define kCelcius			0
#define kFahrenheit			1
#define kKelvin				2

#define kAppMissID			-16397

/*** Recources ***/
#define kElemPictID			127
#define k3dPictID			239

#define	kResName			128
#define	kResWeight			129
#define	kResMelt			130
#define	kResBoil			131
#define	kResDate			132
#define	kElecConfig			133
#define	kResRadii			134
#define	kResNeg				135
#define kResDensity			136
#define	kResSHC				137
#define	kResOxidation		138
#define	kResIonization		139
#define	kResSymbol			140
#define	kResGroupName		141
#define kResGroup			142

#define	kTypeWeight			'amu '


#define	kScrollJump			1			// How far the a window scrolls in 1 click
#define kMaxIsotopes		35			// Highest number of possible isotopes for an element
#define kGraphHScale		(460/110)	// What to stretch horiz. part of graph by
#define kTableHeight		420
#define kTableWidth			560
#define	kElemHeight			220
#define kElemWidth			460
#define kFlashDelay			5
#define	kListMax			120
#define	kListDown			20

#define	kSide	7
#define	kPart	1
#define	kTop	-1
#define	kBottom	1
#define	kLeft	-1
#define	kRight	1


enum {
	prElement,
	prTable,
	prGraph,
	prCompare
};

enum
{
	name = 1,
	number,
	weight,
	config,
	melt,
	boil,
	oxi,
	neg,
	shc,
	ion,
	disc,
	radii,
	density,
	cstruct,
	symbol
};

enum
{
	kGradName = 1,
	kGradRadii,
	kGradDensity,
	kGradMelt,
	kGradBoil,
	kGradIon,
	kGradNeg,
	kGradLewis
};


/*** Fonts ***/
enum {
	newYork					= 2,
	geneva					= 3,
	monaco					= 4,
	cairo					= 11,
	times					= 20,
	helvetica				= 21,
	courier					= 22,
};