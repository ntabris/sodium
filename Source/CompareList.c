#include "ExternGlobals.h"
#include "FuncDefs.h"

void SetupList()
{
	EventRecord		Event;
	DialogPtr		ListDialog;
	Handle			controlHdl;
	SInt16 			itemHit;
	short			itemType;
	Rect			itemRect;
	
	UseData();
	
	if(!(ListDialog = GetNewDialog(134,0L,(WindowPtr) -1)))
		HandleError(20);
	
	SetDialogDefaultItem(ListDialog, 1);
	SetDialogCancelItem(ListDialog, 2);
	SetDialogTracksCursor(ListDialog,true);
 
 	GetDialogItem(ListDialog, 3, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gListSettings.name);
	
	GetDialogItem(ListDialog, 4, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gListSettings.number);
	
	GetDialogItem(ListDialog, 5, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gListSettings.mass);
	
	GetDialogItem(ListDialog, 6, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gListSettings.melt);
		
	GetDialogItem(ListDialog, 7, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gListSettings.boil);
	
	GetDialogItem(ListDialog, 8, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gListSettings.neg);

	GetDialogItem(ListDialog, 9, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gListSettings.shc);

	GetDialogItem(ListDialog, 10, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gListSettings.ion);

	GetDialogItem(ListDialog, 11, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gListSettings.radii);

	GetDialogItem(ListDialog, 12, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gListSettings.density);
	
	GetDialogItem(ListDialog, 13, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gListSettings.symbol);
	
	ShowWindow(ListDialog);
	
	do
	{
		ModalDialog(gDialogUPP, &itemHit);
	
		WaitNextEvent(everyEvent, &Event, 0L, 0L);
		
		if( IsDialogEvent(&Event) )
		{	
			switch(itemHit)
			{
				case 1:
				case 2:
					break;
				default:
					GetDialogItem(ListDialog, itemHit, &itemType, &controlHdl, &itemRect);
					SetControlValue((ControlHandle)controlHdl,!GetControlValue((ControlHandle)controlHdl));
					break;
			}
		}
	}while((itemHit != 1) && (itemHit != 2));
	
	if(itemHit == 1)
	{
		GetDialogItem(ListDialog, 3, &itemType, &controlHdl, &itemRect);	
		gListSettings.name = GetControlValue((ControlHandle)controlHdl);
		
		GetDialogItem(ListDialog, 4, &itemType, &controlHdl, &itemRect);	
		gListSettings.number = GetControlValue((ControlHandle)controlHdl);
		
		GetDialogItem(ListDialog, 5, &itemType, &controlHdl, &itemRect);	
		gListSettings.mass = GetControlValue((ControlHandle)controlHdl);
		
		GetDialogItem(ListDialog, 6, &itemType, &controlHdl, &itemRect);	
		gListSettings.melt = GetControlValue((ControlHandle)controlHdl);
		
		GetDialogItem(ListDialog, 7, &itemType, &controlHdl, &itemRect);	
		gListSettings.boil = GetControlValue((ControlHandle)controlHdl);
		
		GetDialogItem(ListDialog, 8, &itemType, &controlHdl, &itemRect);	
		gListSettings.neg = GetControlValue((ControlHandle)controlHdl);
		
		GetDialogItem(ListDialog, 9, &itemType, &controlHdl, &itemRect);	
		gListSettings.shc = GetControlValue((ControlHandle)controlHdl);
		
		GetDialogItem(ListDialog, 10, &itemType, &controlHdl, &itemRect);	
		gListSettings.ion = GetControlValue((ControlHandle)controlHdl);
		
		GetDialogItem(ListDialog, 11, &itemType, &controlHdl, &itemRect);	
		gListSettings.radii = GetControlValue((ControlHandle)controlHdl);
		
		GetDialogItem(ListDialog, 12, &itemType, &controlHdl, &itemRect);	
		gListSettings.density = GetControlValue((ControlHandle)controlHdl);
		
		GetDialogItem(ListDialog, 13, &itemType, &controlHdl, &itemRect);	
		gListSettings.symbol = GetControlValue((ControlHandle)controlHdl);
		
		gList.Dirty = 1;
		if(gCompareWindow != 0L)
			CompareList();
	}
	DisposeDialog(ListDialog);
	
	CorrectControls();
}

void SaveList()
{
	int					i;
	short				listFRN;
	List				**ListHdl;
	Handle 				existingResHdl;
	Str255 				resourceName = "\pComparison Table";
	StandardFileReply	reply;
	OSErr				err;
	SInt16				currentFile;
	
	currentFile = CurResFile();
	
	StandardPutFile("\pSave this list as:", "\pComparison List", &reply);
	if(reply.sfGood)
	{
		FSpCreateResFile(&reply.sfFile, 'Na11', 'NaLs', smAllScripts);
		err = ResError();
		
		if(err == wPrErr || err == dskFulErr)
		{
			// Can't save to write protected disk.
			// Disk full.
		}
		else if(err == noErr)
		{
			listFRN = FSpOpenResFile(&reply.sfFile, fsRdWrPerm);
			UseResFile(listFRN);
			ListHdl = (List **) NewHandleClear(sizeof(List));
			HLock((Handle)ListHdl);
			
			for(i = 0; i < kListMax; i++)
				(*ListHdl)->List[i] = gList.List[i];
			(*ListHdl)->Sort = gList.Sort;
			(*ListHdl)->Dirty = 0;

			existingResHdl = Get1Resource('L”st', 128);
			if(existingResHdl != 0L)
				RemoveResource(existingResHdl);
			AddResource((Handle) ListHdl, 'L”st', 128, resourceName);
			if(ResError() == noErr)
				WriteResource((Handle) ListHdl);

			HUnlock((Handle) ListHdl);
			ReleaseResource((Handle) ListHdl);
		}
	}
	UseResFile(currentFile);
}

int LoadList()
{
	short		listFRN;
	int			i, returnErr;
	List 		**listHdl;
	SInt16		currentFile;
	
	currentFile  = CurResFile();
	listFRN = OpenDialog('NaLs');
	
	if(listFRN != -1)
	{
		UseResFile(listFRN);
		
		listHdl = (List**) Get1Resource('L”st', 128);
		if(listHdl == 0L || ResError() == resNotFound)
		{
			returnErr = 0;
		}
			else returnErr = 1;

		for(i = 0; i < kListMax; i++)
			gList.List[i] = (*listHdl)->List[i];
		gList.Sort = (*listHdl)->Sort;
		gList.Dirty = 0;
		
		if(gList.Sort)
		{
			gListColumn = gList.Sort;
			gListPeriod = 0;
		}
		
		UseResFile(currentFile);
	}
		else returnErr = 0;
		
	return(returnErr);
}

void CompareWindow()
{
	short	itemType;
	Rect	itemRect;
	Handle	itemHandle;
				
	if(gCompareWindow == 0L)
	{
		Rect	WorldRect;
		SetRect(&WorldRect, 0, 0, 600, 13*kListMax);
		
		UseData();
		if((gCompareWindow = GetNewDialog(133, 0L, (WindowRef)-1)) == 0L)
			HandleError(25);
		HNoPurge((Handle)gCompareWindow);
		
		NewGWorld( &gCompareWorld, 8, &WorldRect, 0L, 0L, 0L );
		if(gCompareWorld == 0L)
			HandleError(26);
	}
	
	if(ValidPos(gListWindowRect))
		MoveWindow(gCompareWindow, gListWindowRect.left, gListWindowRect.top, 0);
	if(ValidPos(gListWindowRect))
		SizeWindow(gCompareWindow, gListWindowRect.right-gListWindowRect.left,
		 gListWindowRect.bottom-gListWindowRect.top, 0);
	GetDialogItem(gCompareWindow, 1, &itemType, &itemHandle, &itemRect);
	
	SizeControl((ControlHandle)itemHandle, 16, gCompareWindow->portRect.bottom-13);
	MoveControl((ControlHandle)itemHandle, gCompareWindow->portRect.right-15, -1);
	
	ShowWindow(gCompareWindow);
	SelectWindow(gCompareWindow);

	CorrectMenus();
	CorrectControls();
}

void CompareList()
{
	int			i, x, items, item, height;
	Str255		string;
	Rect		erase;
	RGBColor	oldColor;
	items = 0;
	item = 1;

	if(gCompareWindow == 0L)
		CompareWindow();
		
	LockPixels(GetGWorldPixMap(gCompareWorld));
	SetGWorld((CGrafPtr)gCompareWorld, 0L);
	
	GetBackColor(&oldColor);
	RGBBackColor(&Grey);
	erase = gCompareWorld->portRect;
	erase.right -= 15;
	EraseRect(&erase);
	RGBBackColor(&oldColor);
	
	for(i = 0; i < kListMax; i++)
	{
		if(gList.List[i])
			items++;
	}
	
	gCompScroll.items = items+1;
	SortList();
	
	if(!items)
	{
		MoveTo(5, 13);
		TextFont(geneva);
		TextSize(10);
		TextFace(bold);
		DrawString("\pOption-click on the periodic table to add elements...");
	}
	else
	{
		RGBColor	dGrey = { 0xBBBB, 0xBBBB, 0xBBBB };
		Rect		theRect;
		
		UseData();
		
		GetForeColor(&oldColor);
		if(gListPeriod > 0)
		{
			RGBBackColor(&dGrey);
			SetRect(&theRect, 0, gListPeriod*13+3, 600-16, (gListPeriod+1)*13+3);
			EraseRect(&theRect);
			RGBBackColor(&White);
		}
		RGBForeColor(&White);
		x = 0;
		if(gListSettings.name)
		{
			x += 80;
			MoveTo(x, 0);
			LineTo(x, (items+1)*13 + 2);
			
			if(gListColumn == name)
			{
				Rect		theRect;
				
				SetRect(&theRect, x-79, 0, x, (items+1)*13 + 2);
				RGBBackColor(&dGrey);
				EraseRect(&theRect);
				RGBBackColor(&White);
			}
		}
		if(gListSettings.symbol)
		{
			x += 30;
			MoveTo(x, 0);
			LineTo(x, (items+1)*13 + 2);
			
			if(gListColumn == symbol)
			{
				Rect		theRect;
				
				SetRect(&theRect, x-29, 0, x, (items+1)*13 + 2);
				RGBBackColor(&dGrey);
				EraseRect(&theRect);
				RGBBackColor(&White);
			}
		}
		if(gListSettings.number)
		{
			x += 30;
			MoveTo(x, 0);
			LineTo(x, (items+1)*13 + 2);
			
			if(gListColumn == number)
			{
				Rect		theRect;
				
				SetRect(&theRect, x-29, 0, x, (items+1)*13 + 2);
				RGBBackColor(&dGrey);
				EraseRect(&theRect);
				RGBBackColor(&White);
			}
		}
		if(gListSettings.mass)
		{
			x += 70;
			MoveTo(x, 0);
			LineTo(x, (items+1)*13 + 2);
			
			if(gListColumn == weight)
			{
				Rect		theRect;
				
				SetRect(&theRect, x-69, 0, x, (items+1)*13 + 2);
				RGBBackColor(&dGrey);
				EraseRect(&theRect);
				RGBBackColor(&White);
			}
		}
		if(gListSettings.melt)
		{
			x += 60;
			MoveTo(x, 0);
			LineTo(x, (items+1)*13 + 2);
			
			if(gListColumn == melt)
			{
				Rect		theRect;
				
				SetRect(&theRect, x-59, 0, x, (items+1)*13 + 2);
				RGBBackColor(&dGrey);
				EraseRect(&theRect);
				RGBBackColor(&White);
			}
		}
		if(gListSettings.boil)
		{
			x += 60;
			MoveTo(x, 0);
			LineTo(x, (items+1)*13 + 2);
			
			if(gListColumn == boil)
			{
				Rect		theRect;
				
				SetRect(&theRect, x-59, 0, x, (items+1)*13 + 2);
				RGBBackColor(&dGrey);
				EraseRect(&theRect);
				RGBBackColor(&White);
			}
		}
		if(gListSettings.neg)
		{
			x += 40;
			MoveTo(x, 0);
			LineTo(x, (items+1)*13 + 2);
			
			if(gListColumn == neg)
			{
				Rect		theRect;
				
				SetRect(&theRect, x-39, 0, x, (items+1)*13 + 2);
				RGBBackColor(&dGrey);
				EraseRect(&theRect);
				RGBBackColor(&White);
			}
		}
		if(gListSettings.shc)
		{
			x += 40;
			MoveTo(x, 0);
			LineTo(x, (items+1)*13 + 2);
			
			if(gListColumn == shc)
			{
				Rect		theRect;
				
				SetRect(&theRect, x-39, 0, x, (items+1)*13 + 2);
				RGBBackColor(&dGrey);
				EraseRect(&theRect);
				RGBBackColor(&White);
			}
		}
		if(gListSettings.ion)
		{
			x += 40;
			MoveTo(x, 0);
			LineTo(x, (items+1)*13 + 2);
			
			if(gListColumn == ion)
			{
				Rect		theRect;
				
				SetRect(&theRect, x-39, 0, x, (items+1)*13 + 2);
				RGBBackColor(&dGrey);
				EraseRect(&theRect);
				RGBBackColor(&White);
			}
		}
		if(gListSettings.radii)
		{
			x += 40;
			MoveTo(x, 0);
			LineTo(x, (items+1)*13 + 2);
			
			if(gListColumn == radii)
			{
				Rect		theRect;
				
				SetRect(&theRect, x-39, 0, x, (items+1)*13 + 2);
				RGBBackColor(&dGrey);
				EraseRect(&theRect);
				RGBBackColor(&White);
			}
		}
		if(gListSettings.density)
		{
			x += 80;
			MoveTo(x, 0);
			LineTo(x, (items+1)*13 + 2);
			
			if(gListColumn == density)
			{
				Rect		theRect;
				
				SetRect(&theRect, x-79, 0, x, (items+1)*13 + 2);
				RGBBackColor(&dGrey);
				EraseRect(&theRect);
				RGBBackColor(&White);
			}
		}
		for(i = 1; i <= items+1; i++)
		{
			MoveTo(0, i*13 + 2);
			LineTo(600-16, i*13 + 2);
		}
		RGBForeColor(&oldColor);
		
		x = 5;
		TextFont(geneva);
		TextSize(9);
		TextFace(bold);

		if(gListSettings.name)
		{
			if(gList.Sort == name)
				TextFace(bold+underline);
			MoveTo(x, 12);
			DrawString("\pName");
			x += 80;
			TextFace(bold);
		}
		
		if(gListSettings.symbol)
		{
			if(gList.Sort == symbol)
				TextFace(bold+underline);
			MoveTo(x, 12);
			DrawString("\pSym");
			x += 30;
			TextFace(bold);
		}
		
		if(gListSettings.number)
		{
			if(gList.Sort == number)
				TextFace(bold+underline);
			MoveTo(x, 12);
			DrawString("\pZ");
			x += 30;
			TextFace(bold);
		}
		
		if(gListSettings.mass)
		{
			if(gList.Sort == weight)
				TextFace(bold+underline);
			MoveTo(x, 12);
			DrawString("\pAMU");
			x += 70;
			TextFace(bold);
		}
		
		if(gListSettings.melt)
		{
			if(gList.Sort == melt)
				TextFace(bold+underline);
			MoveTo(x, 12);
			DrawString("\pMelt. Pnt");
			x += 60;
			TextFace(bold);
		}
		
		if(gListSettings.boil)
		{
			if(gList.Sort == boil)
				TextFace(bold+underline);
			MoveTo(x, 12);
			DrawString("\pBoil. Pnt");
			x += 60;
			TextFace(bold);
		}
		
		if(gListSettings.neg)
		{
			if(gList.Sort == neg)
				TextFace(bold+underline);
			MoveTo(x, 12);
			DrawString("\pNeg.");
			x += 40;
			TextFace(bold);
		}
		
		if(gListSettings.shc)
		{
			if(gList.Sort == shc)
				TextFace(bold+underline);
			MoveTo(x, 12);
			DrawString("\pS.H.");
			x += 40;
			TextFace(bold);
		}
		
		if(gListSettings.ion)
		{
			if(gList.Sort == ion)
				TextFace(bold+underline);
			MoveTo(x, 12);
			DrawString("\pIon.");
			x += 40;
			TextFace(bold);
		}
		
		if(gListSettings.radii)
		{
			if(gList.Sort == radii)
				TextFace(bold+underline);
			MoveTo(x, 12);
			DrawString("\pRadii");
			x += 40;
			TextFace(bold);
		}
		
		if(gListSettings.density)
		{
			if(gList.Sort == density)
				TextFace(bold+underline);
			MoveTo(x, 12);
			DrawString("\pDensity");
			x += 80;
			TextFace(bold);
		}

		for(i = 0; i < kListMax; i++)
		{
			if(gList.List[i])
			{
				x = 5;
				item++;
				TextFont(geneva);
				TextSize(9);
				TextFace(normal);
				
				if(gListSettings.name)
				{
					MoveTo(x, item*13);
					GetIndString(string, kResName, gList.List[i]);
					DrawString(string);
					x += 80;
				}
				
				if(gListSettings.symbol)
				{
					MoveTo(x, item*13);
					GetIndString(string, kResSymbol, gList.List[i]);
					DrawString(string);
					x += 30;
				}
				
				if(gListSettings.number)
				{
					MoveTo(x, item*13);
					NumToString((long)gList.List[i], string);
					DrawString(string);
					x += 30;
				}
				
				if(gListSettings.mass)
				{
					MoveTo(x, item*13);
					GetIndString(string, kResWeight, gList.List[i]);
					DrawString(string);
					x += 70;
				}
				
				if(gListSettings.melt)
				{
					MoveTo(x, item*13);
					GetIndString(string, kResMelt, gList.List[i]);
					ConvertStringCFK(string);
					DrawString(string);
					//if(StrLength(string))
					//	DrawString("\p¡");
					x += 60;
				}
				
				if(gListSettings.boil)
				{
					MoveTo(x, item*13);
					GetIndString(string, kResBoil, gList.List[i]);
					ConvertStringCFK(string);
					DrawString(string);
					//if(StrLength(string))
					//	DrawString("\p¡");
					x += 60;
				}
				
				if(gListSettings.neg)
				{
					MoveTo(x, item*13);
					GetIndString(string, kResNeg, gList.List[i]);
					DrawString(string);
					x += 40;
				}
				
				if(gListSettings.shc)
				{
					MoveTo(x, item*13);
					GetIndString(string, kResSHC, gList.List[i]);
					DrawString(string);
					x += 40;
				}
				
				if(gListSettings.ion)
				{
					MoveTo(x, item*13);
					GetIndString(string, kResIonization, gList.List[i]);
					DrawString(string);
					x += 40;
				}
				
				if(gListSettings.radii)
				{
					MoveTo(x, item*13);
					GetIndString(string, kResRadii, gList.List[i]);
					DrawString(string);
					x += 40;
				}
				
				if(gListSettings.density)
				{
					MoveTo(x, item*13);
					GetIndString(string, kResDensity, gList.List[i]);
					DrawString(string);
					x += 80;
				}
				
				gMinListWidth = x;
			}
		}
	}
	
	height = (gCompareWindow->portRect.bottom)/13;
	
	if(gCompScroll.items > height)
	{
		gCompScroll.max = gCompScroll.items;
		gCompScroll.min = 0;
	}
	else
	{
		gCompScroll.max = 0;
		gCompScroll.current = gCompScroll.old = 0;
	}
	
	UnlockPixels(GetGWorldPixMap(gCompareWorld));
	SetGWorld((CGrafPtr)gCompareWindow, 0L);
	UpdateCompare();
		
	CorrectMenus();
	CorrectControls();
	
	UpdateDialog(gCompareWindow, gCompareWindow->visRgn);
	DrawControls(gCompareWindow);
}

void UpdateCompare()
{
	Rect	fromRect, toRect;
	
	toRect = gCompareWindow->portRect;
	toRect.right -= 15;
	fromRect = toRect;
	fromRect.top += gCompScroll.current*kScrollJump;
	fromRect.bottom += gCompScroll.current*kScrollJump;
	
	CopyBits( &PORTBITMAP( gCompareWorld ), &WINBITMAP( gCompareWindow ),
		&fromRect, &toRect, srcCopy, 0L );
}

void AddElement(int e)
{
	int	i, newItem;
	newItem = 1;
	
	for(i = 0; i < kListMax; i++)
	{
		if(gList.List[i] == e)
			newItem = 0;
	}
	
	if(newItem)
	{
		for(i = 0; i < kListMax; i++)
		{
			if(!gList.List[i])
			{
				gList.List[i] = e;
				i = kListMax;
			}
		}
	}
	
	gList.Dirty = 1;
	CompareList();
}

void RemoveElement(int item)
{
	int	i, count;
	count = 0;
	
	for(i = 0; i < kListMax; i++)
	{
		if(gList.List[i])
		{
			count++;
			if(count == item)
			{
				gList.List[i] = 0;
				item = 0;
			}
		}
	}
	
	gList.Dirty = 1;
	CompareList();
}

void SortList()
{
	int	TempInt, i;
	i = 0;
	
	SetCursor(*GetCursor(watchCursor));
	if(gList.Sort)
	{
		while(i < gCompScroll.items)
		{
			if(SortCompare(i))
			{
				TempInt = gList.List[i];
				gList.List[i] = gList.List[i+1];
				gList.List[i+1] = TempInt;
				if(i > 0)
					i--;
				gList.Dirty = 1;
			}
			else
			{
				i++;
			}
		}
	}
	SetCursor(&qd.arrow);
}

int SortCompare(int i)
{
	Str255	String1, String2;
	xvar	Number1, Number2;
	
	UseData();
	
	if(gList.Sort == number)
	{
		if(gList.List[i+1] < gList.List[i])
			return(1);
	}
	if(gList.Sort == name)
	{	
		GetIndString(String1, kResName, gList.List[i+1]);
		GetIndString(String2, kResName, gList.List[i]);
		if(IUCompString(String1, String2) == -1)
			return(1);
	}
	if(gList.Sort == symbol)
	{	
		GetIndString(String1, kResSymbol, gList.List[i+1]);
		GetIndString(String2, kResSymbol, gList.List[i]);
		if(IUCompString(String1, String2) == -1)
			return(1);
	}
	if(gList.Sort == weight)
	{	
		GetIndString(String1, kResWeight, gList.List[i+1]);
		GetIndString(String2, kResWeight, gList.List[i]);
#ifdef powerc
		StringToDouble(String1, &Number1);
		StringToDouble(String2, &Number2);
#else
		StringToX(String1, &Number1);
		StringToX(String2, &Number2);
#endif
		if(Number1 < Number2)
			return(1);
	}
	if(gList.Sort == melt)
	{	
		GetIndString(String1, kResMelt, gList.List[i+1]);
		GetIndString(String2, kResMelt, gList.List[i]);
#ifdef powerc
		StringToDouble(String1, &Number1);
		StringToDouble(String2, &Number2);
#else
		StringToX(String1, &Number1);
		StringToX(String2, &Number2);
#endif
		if(Number1 < Number2)
			return(1);
	}
	if(gList.Sort == boil)
	{	
		GetIndString(String1, kResBoil, gList.List[i+1]);
		GetIndString(String2, kResBoil, gList.List[i]);
#ifdef powerc
		StringToDouble(String1, &Number1);
		StringToDouble(String2, &Number2);
#else
		StringToX(String1, &Number1);
		StringToX(String2, &Number2);
#endif
		if(Number1 < Number2)
			return(1);
	}
	if(gList.Sort == neg)
	{	
		GetIndString(String1, kResNeg, gList.List[i+1]);
		GetIndString(String2, kResNeg, gList.List[i]);
#ifdef powerc
		StringToDouble(String1, &Number1);
		StringToDouble(String2, &Number2);
#else
		StringToX(String1, &Number1);
		StringToX(String2, &Number2);
#endif
		if(Number1 < Number2)
			return(1);
	}
	if(gList.Sort == shc)
	{	
		GetIndString(String1, kResSHC, gList.List[i+1]);
		GetIndString(String2, kResSHC, gList.List[i]);
#ifdef powerc
		StringToDouble(String1, &Number1);
		StringToDouble(String2, &Number2);
#else
		StringToX(String1, &Number1);
		StringToX(String2, &Number2);
#endif
		if(Number1 < Number2)
			return(1);
	}
	if(gList.Sort == ion)
	{	
		GetIndString(String1, kResIonization, gList.List[i+1]);
		GetIndString(String2, kResIonization, gList.List[i]);
#ifdef powerc
		StringToDouble(String1, &Number1);
		StringToDouble(String2, &Number2);
#else
		StringToX(String1, &Number1);
		StringToX(String2, &Number2);
#endif
		if(Number1 < Number2)
			return(1);
	}
	if(gList.Sort == radii)
	{	
		GetIndString(String1, kResRadii, gList.List[i+1]);
		GetIndString(String2, kResRadii, gList.List[i]);
#ifdef powerc
		StringToDouble(String1, &Number1);
		StringToDouble(String2, &Number2);
#else
		StringToX(String1, &Number1);
		StringToX(String2, &Number2);
#endif
		if(Number1 < Number2)
			return(1);
	}
	if(gList.Sort == density)
	{	
		GetIndString(String1, kResDensity, gList.List[i+1]);
		GetIndString(String2, kResDensity, gList.List[i]);
#ifdef powerc
		StringToDouble(String1, &Number1);
		StringToDouble(String2, &Number2);
#else
		StringToX(String1, &Number1);
		StringToX(String2, &Number2);
#endif
		if(Number1 < Number2)
			return(1);
	}

	return(0);
}

void ShrinkCompareWindow()
{
	Point	WindSize;
	short	itemType;
	Rect	itemRect;
	Handle	itemHandle;
	
	WindSize.v = (gCompScroll.items)*13 + 2;
	if(WindSize.v < 99)
		WindSize.v = 99;
	if(WindSize.v > 400)
		WindSize.v = 401;
	if(gMinListWidth)
		WindSize.h = gMinListWidth+10;
	else
		WindSize.h = 330;
	
	SizeWindow(gCompareWindow, WindSize.h, WindSize.v, true);
	GetDialogItem(gCompareWindow, 1, &itemType, &itemHandle, &itemRect);
	SizeControl((ControlHandle)itemHandle, 16, WindSize.v-13);
	MoveControl((ControlHandle)itemHandle, WindSize.h-15, -1);
	
	if(gCompScroll.current+(WindSize.v/13) > gCompScroll.items && gCompScroll.max > gCompScroll.items)
	{
		gCompScroll.current = gCompScroll.items-(WindSize.v/13);
	}
	ShowWindow(gCompareWindow);
	UpdateCompare();
	CorrectControls();
}