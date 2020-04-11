#include "Konstants.h"
#include "Structs.h"


/*** Variables ***/
Rect				gDragRect, gTableRect, gElementRect, gEWorldRect,
					gIsoRect, gIsoWorldRect, gGraphRect, gElementRects[114], gListWindowRect;
TextRects			gTextRects;
Boolean				gDone, gSave, gFrontApp, gZoom,
					gEWinOpen, gGraphOpen, gIsoTrue, gIWorldOpen, gCalcOpen,
					gEConfigValid, gDateValid, gRadiiValid, gBoilValid, gMeltValid,
					gDensityValid, gSHCValid, gOxiValid, gIonValid, gNegValid,
					DWeightx[kMaxIsotopes], DAbundx[kMaxIsotopes],
					DHalfLx[kMaxIsotopes], DIsoWx[kMaxIsotopes], gShellValid, gCopyLabels;
EventRecord			gEvent;
WindowRef			gTableWindow, gGraphWindow;
DialogPtr			gElementWindow, gCalcWindow, gPrefsDialog, gGraphDialog, gCompareWindow;
Str255				gNameText, gEConfigText, gWeightText, gRadiiText, gDensityText,
					gMeltText, gBoilText, gSymbText, gDateText, gNegText, gGroupText,
					gDWeight[kMaxIsotopes], gDAbund[kMaxIsotopes],
					gDHalfL[kMaxIsotopes], gDIsoW[kMaxIsotopes],
					gSHCText, gOxiText, gIonText, gIsoTxt, gIsoStrtTxt;
Shell				gShell;
List				gList;
short int			gDataFile, gPictsFile, gIsotopesFile, gIsotopes2File, gIsotopes3File;
int					gE, gPrintWhat, gListColumn, gListPeriod, gClickingE, gAE;
long				gMinListWidth;
SInt16				gAppFRN, gPrefsFRN;
SysEnvRec			gWorld;
GWorldPtr			gTableWorld, gElementWorld, gIsotopeWorld, gGraphWorld, gCompareWorld;
PicHandle			gTablePict;
//PicHandle			gElemWindPict;
ControlActionUPP	gScrollProcUPP, gLScrollProcUPP;
ModalFilterUPP		gDialogUPP;
THPrint				gPrintRec;
TPPrDlg				gTPrDlgStructurePtr;
SInt32				gPrintDITLAppendNumb;
PDlgInitUPP 		gPrintInitFunctUPP;
PItemUPP			gOldItemEvaluateFunctUPP;
PItemUPP			gNewItemEvaluateFunctUPP;
ModalFilterUPP		gPrintEventFilterUPP;
AEEventHandlerUPP	gAEOpenProc, gAEOpenDocProc, gAEPrintDocProc, gAEQuitProc;

ScrollType			gElementScroll, gCompScroll;

/*pascal void			ScrollProc(ControlHandle theControl,short theCode);
pascal Boolean		EventFilter(DialogPtr,EventRecord *,SInt16 *);
Point				ItTable(int x);*/

RGBColor			White	= { 0xFFFF, 0xFFFF, 0xFFFF };
RGBColor			Grey	= { 0xDEDE, 0xDEDE, 0xDEDE };
RGBColor			Black	= { 0x0000, 0x0000, 0x0000 };
RGBColor			Blue	= { 0x3000, 0x3000, 0xFFFF };

TempRes				gTempType;
HighlightRes		gHighlight;
GraphRes			gGraph;
GradientRes			gGradient;
PrintRes			gPrintSettings;
TableRes			gTablePrefs;
SizeRes				gWindowSize;
RectRes				gWindPos;
ListRes				gListSettings;

WindMove			gWindMove = {0, 0, 0};