#include "Konstants.h"
#include "Structs.h"


/*** Variables ***/
extern Rect				gDragRect, gTableRect, gElementRect, gEWorldRect,
						gIsoRect, gIsoWorldRect, gGraphRect, gElementRects[114], gListWindowRect;
extern TextRects		gTextRects;
extern Boolean			gDone, gSave, gFrontApp, gZoom,
						gEWinOpen, gGraphOpen, gIsoTrue, gIWorldOpen, gCalcOpen,
						gEConfigValid, gDateValid, gRadiiValid, gBoilValid, gMeltValid,
						gDensityValid, gSHCValid, gOxiValid, gIonValid, gNegValid,
						DWeightx[kMaxIsotopes], DAbundx[kMaxIsotopes],
						DHalfLx[kMaxIsotopes], DIsoWx[kMaxIsotopes], gShellValid, gCopyLabels;
extern EventRecord		gEvent;
extern WindowRef		gTableWindow, gGraphWindow;
extern DialogPtr		gElementWindow, gCalcWindow, gPrefsDialog, gGraphDialog, gCompareWindow;
extern Str255			gNameText, gEConfigText, gWeightText, gRadiiText, gDensityText,
						gMeltText, gBoilText, gSymbText, gDateText, gNegText, gGroupText,
						gDWeight[kMaxIsotopes], gDAbund[kMaxIsotopes],
						gDHalfL[kMaxIsotopes], gDIsoW[kMaxIsotopes],
						gSHCText, gOxiText, gIonText, gIsoTxt, gIsoStrtTxt;
extern Shell			gShell;
extern List				gList;
extern short int		gDataFile, gPictsFile, gIsotopesFile, gIsotopes2File, gIsotopes3File;
extern int				gE, gPrintWhat, gListColumn, gListPeriod, gClickingE, gAE;
extern long				gMinListWidth;
extern SInt16			gAppFRN, gPrefsFRN;
extern SysEnvRec		gWorld;
extern GWorldPtr		gTableWorld, gElementWorld, gIsotopeWorld, gGraphWorld, gCompareWorld;
extern PicHandle		gTablePict;
extern ControlActionUPP	gScrollProcUPP, gLScrollProcUPP;
extern ModalFilterUPP	gDialogUPP;
extern THPrint			gPrintRec;
extern TPPrDlg			gTPrDlgStructurePtr;
extern SInt32			gPrintDITLAppendNumb;
extern PDlgInitUPP 		gPrintInitFunctUPP;
extern PItemUPP			gOldItemEvaluateFunctUPP;
extern PItemUPP			gNewItemEvaluateFunctUPP;
extern ModalFilterUPP	gPrintEventFilterUPP;

extern ScrollType		gElementScroll, gCompScroll;

extern RGBColor			White;
extern RGBColor			Grey;
extern RGBColor			Black;
extern RGBColor			Blue;

extern TempRes			gTempType;
extern HighlightRes		gHighlight;
extern GraphRes			gGraph;
extern GradientRes		gGradient;
extern PrintRes			gPrintSettings;
extern TableRes			gTablePrefs;
extern SizeRes			gWindowSize;
extern RectRes			gWindPos;
extern ListRes			gListSettings;

extern WindMove			gWindMove;