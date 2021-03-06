#include "ExternGlobals.h"
#include "FuncDefs.h"

/*** External Global Variables ***/
/*extern SInt16			gPrefsFRN;
extern Boolean			gSave;
extern TempRes			gTempType;
extern HighlightRes		gHighlight;
extern GraphRes			gGraph;
extern GradientRes		gGradient;
extern PrintRes			gPrintSettings;
extern TableRes			gTablePrefs;
extern SizeRes			gWindowSize;
extern RectRes			gWindPos;
extern WindMove			gWindMove;
extern ListRes			gListSettings;
extern Rect				gListRect;
extern WindowRef		gCompareWindow;

extern WindowRef		gTableWindow, gGraphWindow;
extern DialogPtr		gElementWindow, gCalcWindow;*/

/*** Preference Saving Routines ***/
void SaveTempPrefs()
{
	TempResHandle	prefsHdl;
	Handle 			existingResHdl;
	Str255 			resourceName = "\pSodium Preferences";
	SInt16			currentFile;

	if(gPrefsFRN == -1 || !gSave)
		return;
		
	currentFile = CurResFile();
	UseResFile(gPrefsFRN);
	
	prefsHdl = (TempResHandle) NewHandleClear(sizeof(TempRes));
	HLock((Handle)prefsHdl);
	
	(*prefsHdl)->tempType	= gTempType.tempType;

	existingResHdl = Get1Resource('Temp', 128);
	if(existingResHdl != 0L)
	{
		RemoveResource(existingResHdl);
		if(ResError() == noErr)
			AddResource((Handle) prefsHdl, 'Temp', 128, resourceName);
		if(ResError() == noErr)
			WriteResource((Handle) prefsHdl);
	}

	HUnlock((Handle) prefsHdl);

	ReleaseResource((Handle) prefsHdl);

	UseResFile(currentFile);
}

void SaveHilitePrefs()
{
	HighlightResHandle	prefsHdl;
	Handle 				existingResHdl;
	Str255 				resourceName = "\pSodium Preferences";
	SInt16				currentFile;

	if(gPrefsFRN == -1 || !gSave)
		return;
		
	currentFile = CurResFile();
	UseResFile(gPrefsFRN);
	
	prefsHdl = (HighlightResHandle) NewHandleClear(sizeof(HighlightRes));
	HLock((Handle)prefsHdl);
	
	(*prefsHdl)->enabled	= gHighlight.enabled;
	(*prefsHdl)->color		= gHighlight.color;

	existingResHdl = Get1Resource('Hlit', 128);
	if(existingResHdl != 0L)
	{
		RemoveResource(existingResHdl);
		if(ResError() == noErr)
			AddResource((Handle) prefsHdl, 'Hlit', 128, resourceName);
		if(ResError() == noErr)
			WriteResource((Handle) prefsHdl);
	}

	HUnlock((Handle) prefsHdl);

	ReleaseResource((Handle) prefsHdl);

	UseResFile(currentFile);
}

void SaveGraphPrefs()
{
	GraphResHandle	prefsHdl;
	Handle 			existingResHdl;
	Str255 			resourceName = "\pSodium Preferences";
	SInt16			currentFile;

	if(gPrefsFRN == -1 || !gSave)
		return;
		
	currentFile = CurResFile();
	UseResFile(gPrefsFRN);
	
	prefsHdl = (GraphResHandle) NewHandleClear(sizeof(GraphRes));
	HLock((Handle)prefsHdl);
	
	(*prefsHdl)->WeightColor		= gGraph.WeightColor;
	(*prefsHdl)->RadiiColor			= gGraph.RadiiColor;
	(*prefsHdl)->MeltColor			= gGraph.MeltColor;
	(*prefsHdl)->BoilColor			= gGraph.BoilColor;
	(*prefsHdl)->IonizationColor	= gGraph.IonizationColor;
	(*prefsHdl)->NegativityColor	= gGraph.NegativityColor;
	(*prefsHdl)->Weight				= gGraph.Weight;
	(*prefsHdl)->Radii				= gGraph.Radii;
	(*prefsHdl)->Melt				= gGraph.Melt;
	(*prefsHdl)->Boil				= gGraph.Boil;
	(*prefsHdl)->Ionization			= gGraph.Ionization;
	(*prefsHdl)->Negativity			= gGraph.Negativity;

	existingResHdl = Get1Resource('Graf', 128);
	if(existingResHdl != 0L)
	{
		RemoveResource(existingResHdl);
		if(ResError() == noErr)
			AddResource((Handle) prefsHdl, 'Graf', 128, resourceName);
		if(ResError() == noErr)
			WriteResource((Handle) prefsHdl);
	}

	HUnlock((Handle) prefsHdl);

	ReleaseResource((Handle) prefsHdl);

	UseResFile(currentFile);
}

void SaveWindPosPrefs()
{
	RectResHandle	prefsHdl;
	RectRes2Handle	prefs2Hdl;
	Handle 			existingResHdl;
	Str255 			resourceName = "\pSodium Preferences";
	SInt16			currentFile;

	if(gPrefsFRN == -1 || !gSave)
		return;
		
	currentFile = CurResFile();
	UseResFile(gPrefsFRN);
	
	prefsHdl = (RectResHandle) NewHandleClear(sizeof(RectRes));
	HLock((Handle)prefsHdl);
	
	if(gWindMove.Table)
		(*prefsHdl)->TableRect		= gWindPos.TableRect	= GetWinPosRect(gTableWindow);
	else
		(*prefsHdl)->TableRect = gWindPos.TableRect;
		
	if(gWindMove.Element)
		(*prefsHdl)->ElementRect	= gWindPos.ElementRect	= GetWinPosRect(gElementWindow);
	else
		(*prefsHdl)->ElementRect = gWindPos.ElementRect;
		
	if(gWindMove.Graph)
		(*prefsHdl)->GraphRect		= gWindPos.GraphRect	= GetWinPosRect(gGraphWindow);
	else
		(*prefsHdl)->GraphRect = gWindPos.GraphRect;
	gWindMove.Table = gWindMove.Element = gWindMove.Graph = 0;

	existingResHdl = Get1Resource('Rect', 128);
	if(existingResHdl != 0L)
	{
		RemoveResource(existingResHdl);
		if(ResError() == noErr)
			AddResource((Handle) prefsHdl, 'Rect', 128, resourceName);
		if(ResError() == noErr)
			WriteResource((Handle) prefsHdl);
	}

	HUnlock((Handle) prefsHdl);
	ReleaseResource((Handle) prefsHdl);
	
	if(gCompareWindow != 0L)
	{
		prefs2Hdl = (RectRes2Handle) NewHandleClear(sizeof(RectRes2));
		HLock((Handle)prefs2Hdl);
		(*prefs2Hdl)->List		= GetWinPosRect(gCompareWindow);

		existingResHdl = Get1Resource('Rect', 129);
		if(existingResHdl != 0L)
		{
			RemoveResource(existingResHdl);
			if(ResError() == noErr)
				AddResource((Handle) prefsHdl, 'Rect', 129, resourceName);
			if(ResError() == noErr)
				WriteResource((Handle) prefsHdl);
		}

		HUnlock((Handle) prefs2Hdl);
		ReleaseResource((Handle) prefs2Hdl);
	}

	UseResFile(currentFile);
}

void SaveTablePrefs()
{
	TableResHandle	prefsHdl;
	Handle 			existingResHdl;
	Str255 			resourceName = "\pSodium Preferences";
	SInt16			currentFile;

	if(gPrefsFRN == -1 || !gSave)
		return;
		
	currentFile = CurResFile();
	UseResFile(gPrefsFRN);
	
	prefsHdl = (TableResHandle) NewHandleClear(sizeof(TableRes));
	HLock((Handle)prefsHdl);
	
	(*prefsHdl)->style			= gTablePrefs.style;
	(*prefsHdl)->classic		= gTablePrefs.classic;

	existingResHdl = Get1Resource('Tabl', 128);
	if(existingResHdl != 0L)
	{
		RemoveResource(existingResHdl);
		if(ResError() == noErr)
			AddResource((Handle) prefsHdl, 'Tabl', 128, resourceName);
		if(ResError() == noErr)
			WriteResource((Handle) prefsHdl);
	}

	HUnlock((Handle) prefsHdl);

	ReleaseResource((Handle) prefsHdl);

	UseResFile(currentFile);
}

void SaveWindSizePrefs()
{
	SizeResHandle	prefsHdl;
	Handle 			existingResHdl;
	Str255 			resourceName = "\pSodium Preferences";
	SInt16			currentFile;

	if(gPrefsFRN == -1 || !gSave)
		return;
		
	currentFile = CurResFile();
	UseResFile(gPrefsFRN);
	
	prefsHdl = (SizeResHandle) NewHandleClear(sizeof(SizeRes));
	HLock((Handle)prefsHdl);
	
	(*prefsHdl)->Table			= gWindowSize.Table;
	(*prefsHdl)->Element		= gWindowSize.Element;

	existingResHdl = Get1Resource('Size', 128);
	if(existingResHdl != 0L)
	{
		RemoveResource(existingResHdl);
		if(ResError() == noErr)
			AddResource((Handle) prefsHdl, 'Size', 128, resourceName);
		if(ResError() == noErr)
			WriteResource((Handle) prefsHdl);
	}

	HUnlock((Handle) prefsHdl);

	ReleaseResource((Handle) prefsHdl);

	UseResFile(currentFile);
}

void SavePrintingPrefs()
{
	PrintResHandle	prefsHdl;
	Handle 			existingResHdl;
	Str255 			resourceName = "\pSodium Preferences";
	SInt16			currentFile;

	if(gPrefsFRN == -1 || !gSave)
		return;
		
	currentFile = CurResFile();
	UseResFile(gPrefsFRN);
	
	prefsHdl = (PrintResHandle) NewHandleClear(sizeof(PrintRes));
	HLock((Handle)prefsHdl);
	
	(*prefsHdl)->Pict		= gPrintSettings.Pict;
	(*prefsHdl)->Center		= gPrintSettings.Center;

	existingResHdl = Get1Resource('Prnt', 128);
	if(existingResHdl != 0L)
	{
		RemoveResource(existingResHdl);
		if(ResError() == noErr)
			AddResource((Handle) prefsHdl, 'Prnt', 128, resourceName);
		if(ResError() == noErr)
			WriteResource((Handle) prefsHdl);
	}

	HUnlock((Handle) prefsHdl);

	ReleaseResource((Handle) prefsHdl);

	UseResFile(currentFile);
}

void SaveListPrefs()
{
	ListResHandle	prefsHdl;
	Handle 			existingResHdl;
	Str255 			resourceName = "\pSodium Preferences";
	SInt16			currentFile;

	if(gPrefsFRN == -1 || !gSave)
		return;
		
	currentFile = CurResFile();
	UseResFile(gPrefsFRN);
	
	prefsHdl = (ListResHandle) NewHandleClear(sizeof(ListRes));
	HLock((Handle)prefsHdl);
	
	(*prefsHdl)->name		= gListSettings.name;
	(*prefsHdl)->number		= gListSettings.number;
	(*prefsHdl)->mass		= gListSettings.mass;
	(*prefsHdl)->melt		= gListSettings.melt;
	(*prefsHdl)->boil		= gListSettings.boil;
	(*prefsHdl)->neg		= gListSettings.neg;
	(*prefsHdl)->shc		= gListSettings.shc;
	(*prefsHdl)->ion		= gListSettings.ion;
	(*prefsHdl)->radii		= gListSettings.radii;
	(*prefsHdl)->density	= gListSettings.density;
	(*prefsHdl)->cstruct	= gListSettings.cstruct;
	(*prefsHdl)->symbol		= gListSettings.symbol;

	existingResHdl = Get1Resource('List', 128);
	if(existingResHdl != 0L)
	{
		RemoveResource(existingResHdl);
		if(ResError() == noErr)
			AddResource((Handle) prefsHdl, 'List', 128, resourceName);
		if(ResError() == noErr)
			WriteResource((Handle) prefsHdl);
	}

	HUnlock((Handle) prefsHdl);

	ReleaseResource((Handle) prefsHdl);

	UseResFile(currentFile);
}

void SaveCopyLabelsPrefs()
{
	Boolean **	prefsHdl;
	Handle 		existingResHdl;
	Str255 		resourceName = "\pSodium Preferences";
	SInt16		currentFile;

	if(gPrefsFRN == -1 || !gSave)
		return;
		
	currentFile = CurResFile();
	UseResFile(gPrefsFRN);
	
	prefsHdl = (Boolean **) NewHandleClear(sizeof(Boolean));
	HLock((Handle)prefsHdl);
	
	(**prefsHdl)		= gCopyLabels;

	existingResHdl = Get1Resource('CpLb', 128);
	if(existingResHdl != 0L)
	{
		RemoveResource(existingResHdl);
		if(ResError() == noErr)
			AddResource((Handle) prefsHdl, 'CpLb', 128, resourceName);
		if(ResError() == noErr)
			WriteResource((Handle) prefsHdl);
	}

	HUnlock((Handle) prefsHdl);

	ReleaseResource((Handle) prefsHdl);

	UseResFile(currentFile);
}