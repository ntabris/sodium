#include "ExternGlobals.h"
#include "FuncDefs.h"

/*** External Global Variables ***/
/*extern SInt16			gAppFRN;
extern Boolean			gSave;
extern TempRes			gTempType;
extern HighlightRes		gHighlight;
extern GraphRes			gGraph;
extern GradientRes		gGradient;
extern PrintRes			gPrintSettings;
extern TableRes			gTablePrefs;
extern SizeRes			gWindowSize;
extern RectRes			gWindPos;
extern ListRes			gListSettings;
extern Rect				gListWindowRect;*/

/*** Preference Getting Routines ***/
int GetTempPrefs(SInt16 fileRN)
{
	OSErr				err;
	int					returnErr;
	TempResHandle		prefsHdl;
	SInt16				currentFile;
	
	currentFile  = CurResFile();
	
	if(fileRN != -1)
	{
		UseResFile(fileRN);
		
		prefsHdl = (TempResHandle) Get1Resource('Temp',128);
		if(prefsHdl == 0L || ResError() == resNotFound)
		{
			err = CopyRes('Temp', 128, gAppFRN, fileRN);
			if(err == noErr)
			{
				prefsHdl = (TempResHandle) Get1Resource('Temp',128);
				returnErr = 0;
			}
			else
			{
				fileRN = gAppFRN;
				gSave = 0;
				returnErr = 1;
			}
		}

		gTempType.tempType	= (*prefsHdl)->tempType;
		
		UseResFile(currentFile);
	}
	else
		returnErr = 1;
		
	return(returnErr);
}

int GetHilitePrefs(SInt16 fileRN)
{
	OSErr					err;
	int						returnErr;
	HighlightResHandle		prefsHdl;
	SInt16					currentFile;
	
	currentFile  = CurResFile();
	
	if(fileRN != -1)
	{
		UseResFile(fileRN);
		
		prefsHdl = (HighlightResHandle) Get1Resource('Hlit',128);
		if(prefsHdl == 0L || ResError() == resNotFound)
		{
			err = CopyRes('Hlit', 128, gAppFRN, fileRN);
			if(err == noErr)
			{
				prefsHdl = (HighlightResHandle) Get1Resource('Hlit',128);
				returnErr = 0;
			}
			else
			{
				fileRN = gAppFRN;
				gSave = 0;
				returnErr = 1;
			}
		}

		gHighlight.color		= (*prefsHdl)->color;
		gHighlight.enabled		= (*prefsHdl)->enabled;
		
		UseResFile(currentFile);
	}
	else
		returnErr = 1;
		
	return(returnErr);
}

int GetGraphPrefs(SInt16 fileRN)
{
	OSErr				err;
	int					returnErr;
	GraphResHandle		prefsHdl;
	SInt16				currentFile;
	
	currentFile  = CurResFile();
	
	if(fileRN != -1)
	{
		UseResFile(fileRN);
		
		prefsHdl = (GraphResHandle) Get1Resource('Graf',128);
		if(prefsHdl == 0L || ResError() == resNotFound)
		{
			err = CopyRes('Graf', 128, gAppFRN, fileRN);
			if(err == noErr)
			{
				prefsHdl = (GraphResHandle) Get1Resource('Graf',128);
				returnErr = 0;
			}
			else
			{
				fileRN = gAppFRN;
				gSave = 0;
				returnErr = 1;
			}
		}

		gGraph.WeightColor		= (*prefsHdl)->WeightColor;
		gGraph.RadiiColor		= (*prefsHdl)->RadiiColor;
		gGraph.MeltColor		= (*prefsHdl)->MeltColor;
		gGraph.BoilColor		= (*prefsHdl)->BoilColor;
		gGraph.IonizationColor	= (*prefsHdl)->IonizationColor;
		gGraph.NegativityColor	= (*prefsHdl)->NegativityColor;
		gGraph.Weight			= (*prefsHdl)->Weight;
		gGraph.Radii			= (*prefsHdl)->Radii;
		gGraph.Melt				= (*prefsHdl)->Melt;
		gGraph.Boil				= (*prefsHdl)->Boil;
		gGraph.Ionization		= (*prefsHdl)->Ionization;
		gGraph.Negativity		= (*prefsHdl)->Negativity;
		
		UseResFile(currentFile);
	}
	else
		returnErr = 1;
		
	return(returnErr);
}

int GetWindPosPrefs(SInt16 fileRN)
{
	OSErr				err;
	int					returnErr;
	RectResHandle		prefsHdl;
	RectRes2Handle		prefs2Hdl;
	SInt16				currentFile;
	
	currentFile  = CurResFile();
	
	if(fileRN != -1)
	{
		UseResFile(fileRN);
		
		prefsHdl = (RectResHandle) Get1Resource('Rect',128);
		if(prefsHdl == 0L || ResError() == resNotFound)
		{
			err = CopyRes('Rect', 128, gAppFRN, fileRN);
			if(err == noErr)
			{
				prefsHdl = (RectResHandle) Get1Resource('Rect',128);
				returnErr = 0;
			}
			else
			{
				fileRN = gAppFRN;
				gSave = 0;
				returnErr = 1;
			}
		}
		
		prefs2Hdl = (RectRes2Handle) Get1Resource('Rect',129);
		if(prefs2Hdl == 0L || ResError() == resNotFound)
		{
			err = CopyRes('Rect', 129, gAppFRN, fileRN);
			if(err == noErr)
			{
				prefs2Hdl = (RectRes2Handle) Get1Resource('Rect',129);
				returnErr = 0;
			}
			else
			{
				fileRN = gAppFRN;
				gSave = 0;
				returnErr = 1;
			}
		}

		gWindPos.TableRect		= (*prefsHdl)->TableRect;
		gWindPos.ElementRect	= (*prefsHdl)->ElementRect;
		gWindPos.GraphRect		= (*prefsHdl)->GraphRect;
		gListWindowRect			= (*prefs2Hdl)->List;
		
		UseResFile(currentFile);
	}
	else
		returnErr = 1;
		
	return(returnErr);
}

int GetTablePrefs(SInt16 fileRN)
{
	OSErr				err;
	int					returnErr;
	TableResHandle		prefsHdl;
	SInt16				currentFile;
	
	currentFile  = CurResFile();
	
	if(fileRN != -1)
	{
		UseResFile(fileRN);
		
		prefsHdl = (TableResHandle) Get1Resource('Tabl',128);
		if(prefsHdl == 0L || ResError() == resNotFound)
		{
			err = CopyRes('Tabl', 128, gAppFRN, fileRN);
			if(err == noErr)
			{
				prefsHdl = (TableResHandle) Get1Resource('Tabl',128);
				returnErr = 0;
			}
			else
			{
				fileRN = gAppFRN;
				gSave = 0;
				returnErr = 1;
			}
		}

		gTablePrefs.style		= (*prefsHdl)->style;
		gTablePrefs.classic		= (*prefsHdl)->classic;
		
		UseResFile(currentFile);
	}
	else
		returnErr = 1;
		
	return(returnErr);
}

int GetWindSizePrefs(SInt16 fileRN)
{
	OSErr				err;
	int					returnErr;
	SizeResHandle		prefsHdl;
	SInt16				currentFile;
	
	currentFile  = CurResFile();
	
	if(fileRN != -1)
	{
		UseResFile(fileRN);
		
		prefsHdl = (SizeResHandle) Get1Resource('Size',128);
		if(prefsHdl == 0L || ResError() == resNotFound)
		{
			err = CopyRes('Size', 128, gAppFRN, fileRN);
			if(err == noErr)
			{
				prefsHdl = (SizeResHandle) Get1Resource('Size', 128);
				returnErr = 0;
			}
			else
			{
				fileRN = gAppFRN;
				gSave = 0;
				returnErr = 1;
			}
		}

		gWindowSize.Table		= (*prefsHdl)->Table;
		gWindowSize.Element		= (*prefsHdl)->Element;
		
		UseResFile(currentFile);
	}
	else
		returnErr = 1;
		
	return(returnErr);
}

int GetPrintingPrefs(SInt16 fileRN)
{
	OSErr				err;
	int					returnErr;
	PrintResHandle		prefsHdl;
	SInt16				currentFile;
	
	currentFile  = CurResFile();
	
	if(fileRN != -1)
	{
		UseResFile(fileRN);
		
		prefsHdl = (PrintResHandle) Get1Resource('Prnt',128);
		if(prefsHdl == 0L || ResError() == resNotFound)
		{
			err = CopyRes('Prnt', 128, gAppFRN, fileRN);
			if(err == noErr)
			{
				prefsHdl = (PrintResHandle) Get1Resource('Prnt', 128);
				returnErr = 0;
			}
			else
			{
				fileRN = gAppFRN;
				gSave = 0;
				returnErr = 1;
			}
		}

		gPrintSettings.Pict		= (*prefsHdl)->Pict;
		gPrintSettings.Center	= (*prefsHdl)->Center;
		
		UseResFile(currentFile);
	}
	else
		returnErr = 1;
		
	return(returnErr);
}

int GetListPrefs(SInt16 fileRN)
{
	OSErr				err;
	int					returnErr;
	ListResHandle		prefsHdl;
	SInt16				currentFile;
	
	currentFile  = CurResFile();
	
	if(fileRN != -1)
	{
		UseResFile(fileRN);
		
		prefsHdl = (ListResHandle) Get1Resource('List',128);
		if(prefsHdl == 0L || ResError() == resNotFound)
		{
			err = CopyRes('List', 128, gAppFRN, fileRN);
			if(err == noErr)
			{
				prefsHdl = (ListResHandle) Get1Resource('List', 128);
				returnErr = 0;
			}
			else
			{
				fileRN = gAppFRN;
				gSave = 0;
				returnErr = 1;
			}
		}

		gListSettings.name		= (*prefsHdl)->name;
		gListSettings.number	= (*prefsHdl)->number;
		gListSettings.mass		= (*prefsHdl)->mass;
		gListSettings.melt		= (*prefsHdl)->melt;
		gListSettings.boil		= (*prefsHdl)->boil;
		gListSettings.neg		= (*prefsHdl)->neg;
		gListSettings.shc		= (*prefsHdl)->shc;
		gListSettings.ion		= (*prefsHdl)->ion;
		gListSettings.radii		= (*prefsHdl)->radii;
		gListSettings.density	= (*prefsHdl)->density;
		gListSettings.cstruct	= (*prefsHdl)->cstruct;
		gListSettings.symbol	= (*prefsHdl)->symbol;
		
		UseResFile(currentFile);
	}
	else
		returnErr = 1;
		
	return(returnErr);
}

int GetCopyLabelsPrefs(SInt16 fileRN)
{
	OSErr			err;
	int				returnErr;
	Boolean **		prefsHdl;
	SInt16			currentFile;
	
	currentFile  = CurResFile();
	
	if(fileRN != -1)
	{
		UseResFile(fileRN);
		
		prefsHdl = (Boolean **) Get1Resource('CpLb',128);
		if(prefsHdl == 0L || ResError() == resNotFound)
		{
			err = CopyRes('CpLb', 128, gAppFRN, fileRN);
			if(err == noErr)
			{
				prefsHdl = (Boolean **) Get1Resource('CpLb', 128);
				returnErr = 0;
			}
			else
			{
				fileRN = gAppFRN;
				gSave = 0;
				returnErr = 1;
			}
		}

		gCopyLabels		= (**prefsHdl);
		
		UseResFile(currentFile);
	}
	else
		returnErr = 1;
		
	return(returnErr);
}