//---------------------------------------------------------//
//   Sodium (c)1998-99 Nat Tabris of Theoretic Software    //
//	                  All rights reserved                  //
//---------------------------------------------------------//

#include "Globals.h"
#include "FuncDefs.h"
#include <fp.h>



//#define	GREY_LINES	1

void	TestQShells(void);
pascal void SliderProc(ControlHandle theControl, short theCode);

pascal void SliderProc(ControlHandle theControl, short theCode)
{
	int	curVal, maxVal, minVal;
	
	maxVal = GetControlMaximum((ControlHandle)theControl);
	curVal = GetControlValue((ControlHandle)theControl);
	minVal = GetControlMinimum((ControlHandle)theControl);
	
	switch(theCode)
	{
		case kControlPageUpPart:
			if(curVal > minVal + 1)
				curVal = curVal - 1;
			else
				curVal = minVal;
			break;
		case kControlUpButtonPart:
			if(curVal > minVal)
				--curVal;
			break;
		case kControlPageDownPart:
			if(curVal+1 < maxVal)
				curVal = curVal + 1;
			else
				curVal = maxVal;
			break;
		case kControlDownButtonPart:
			if(curVal < maxVal)
				++curVal;
			else
				curVal = maxVal;
			break;
	}
	
	SetControlValue((ControlHandle)theControl, curVal);
	if(curVal > 3000)
		SysBeep(1);
}


#pragma mark Main:
#pragma mark -

void main()
{
	long	SysVersion;
	
	ToolBoxInit();
	gAppFRN = CurResFile();
	if(gAppFRN == -1)
		HandleError(-1);
		
	SysVersion = gWorld.systemVersion;
	if(SysVersion < 0x0700)
	{
		ParamText("\pSystem 7 or greater","\p","\p","\p");
		StopAlert(201, 0L);
		ExitToShell();
	}
	if(!GestaltAppear())
	{
		ParamText("\pthe Appearance Extension","\p","\p","\p");
		StopAlert(201, 0L);
		ExitToShell();
	}
	if(GestaltAppleEvent())
		gAE = true;
	else
		gAE = false;
		
	Setup();
	
	PictureOpen();
	DataOpen();
	IsotopesOpen();
		
	GetPrefs();
	
	//ConvertToLD(kResWeight, kTypeWeight);
	TitleScreen();
		
	MenuBarInit();
	DragRectInit();
	TableInit();
	CorrectMenus();
	
	//TestQShells();
		
	do
		HandleEvents();
	while(!gDone);
	
	Quit();
}

void TitleScreen()
{
	Boolean		waitDone = 0;
	long		ticks;
	EventRecord	wait;
	GWorldPtr	offWorld;
	WindowRef	TitleWindow;
	Rect		TitleRect;
	PicHandle	TitlePict;

	UseData();
	SetRect(&TitleRect, 0, 0, 285, 180);
	
	if((TitleWindow = GetNewCWindow(128, 0L, (WindowRef)-1)) == 0L)
		HandleError(2);
		
	NewGWorld( &offWorld, 0, &TitleRect, 0L, 0L, 0L );
	if(offWorld == 0L)
		HandleError(3);
	else
	{
		SetGWorld( (CGrafPtr)offWorld, 0L );
		
		UsePicts();
		
		if((TitlePict = GetPicture(998)) == 0L)
			HandleError(4);
		
		LockPixels(GetGWorldPixMap(offWorld));
		SetGWorld((CGrafPtr)offWorld, 0L);
			
		DrawPicture(TitlePict, &TitleRect);
		
		UnlockPixels(GetGWorldPixMap(offWorld));

		ShowWindow(TitleWindow);

		CopyBits( &PORTBITMAP( offWorld ), &WINBITMAP( TitleWindow ),
			&TitleRect, &TitleRect, srcCopy, 0L );
		
		waitDone = 0;
		
		ticks = TickCount();
		
		while(!waitDone)
		{
			GetNextEvent(keyDownMask | mUpMask, &wait);
			switch(wait.what)
			{
				case keyDown:
				case mouseUp:
					waitDone = 1;
					break;
			}
			
			if(TickCount() > (ticks + 60))
				waitDone = 1;
		}	

		HideWindow(TitleWindow);
		DisposeWindow(TitleWindow);
		DisposeGWorld(offWorld);
		
		FlushEvents(everyEvent, 0);
	}
}

#pragma mark -
#pragma mark Utility Routines:
#pragma mark -

void HandleError(int type)
{
	Str255	errorText, typeStr;
	
	NumToString(type, typeStr);
	
	//ParamText("\pAn error has occured, try giving Sodium more memory in the Finder.", typeStr, "\p", "\p");
	UseResFile(gAppFRN);
	if(type > 0)
	{
		GetIndString(errorText, 500, type);
		ParamText(errorText, typeStr, "\p", "\p");
	}
	else
		ParamText("\pFatal error! Can't find self.", typeStr, "\p", "\p");
	StopAlert(300, 0L);
	ExitToShell();
}

Boolean	ValidPos(Rect rect)
{
	if(rect.top || rect.bottom || rect.left || rect.right)
		return 1;
	return 0;
}

Rect GetWinPosRect(WindowPtr window)
{
   Rect		rect = window->portRect;
   Point	point = {0,0};
      
   SetPort(window);
   LocalToGlobal(&point);
   
   rect.top		+= point.v;
   rect.bottom	+= point.v;
   rect.left	+= point.h;
   rect.right	+= point.h;
   
   return rect;
}

void CenterRect(Rect *newRect, Rect larger)
{
	Rect	smaller;
	
	smaller = *newRect;
	newRect->top = (larger.bottom - larger.top)/2 - (smaller.bottom - smaller.top)/2;
	newRect->bottom = newRect->top + (smaller.bottom-smaller.top);
	newRect->left = (larger.right - larger.left)/2 - (smaller.right - smaller.left)/2;
	newRect->right = newRect->left + (smaller.right-smaller.left);
}

void AppendString(Str255 a, Str255 b)
{
	int		i, x;
	char	car;
	
	x = StrLength(b);
	for(i = 1; i <= x; i++)
	{
		car = b[i];
		a[StrLength(a)+1] = car;
		a[0]++;
	}
}

#pragma mark -
#pragma mark Setup stuff:
#pragma mark -

void ToolBoxInit()
{
	OSErr		error;

	error = SysEnvirons(1, &gWorld);
	if(error == noErr)
	{
		if(gWorld.hasColorQD == false)
		{
			SysBeep(50);
			BlackWhite();
		}
	}
	else HandleError(1);
		
	InitGraf(&qd.thePort);
	InitFonts();
	FlushEvents(everyEvent, 0L);
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(0L);
	InitCursor();
	
	MoreMasters();
}

void BlackWhite()
{
	ParamText("\pcolor","\p","\p","\p");
	StopAlert(201, 0L);
	ExitToShell();
}

int GestaltAppear()
{
	OSErr osError;
	SInt32 response;
	Boolean appearanceFunctionsAvail = false;
	Boolean version101Present = false;
	Boolean inCompatibilityMode = false;
	
	// Call Gestalt with the gestaltAppearanceAttr selector
	
	osError = Gestalt(gestaltAppearanceAttr,&response);
	
	// If Gestalt returns no error and the bit in response represented by the constant
	// gestaltAppearanceExists is set, proceed, otherwise exit with an error message.
	
	if(osError == noErr && (BitTst(&response,31 - gestaltAppearanceExists)))
	{
		appearanceFunctionsAvail = true;
		
		if(BitTst(&response,31 - gestaltAppearanceCompatMode))
			inCompatibilityMode = true;
				
		Gestalt(gestaltAppearanceVersion, &response);
		if(response & 0x00000101 == 0x00000101)
			version101Present = true;
	}
	
	return(appearanceFunctionsAvail);
}

int GestaltAppleEvent()
{
	OSErr osError;
	SInt32 response;
	Boolean aeFunctionsAvail = false;
		
	osError = Gestalt(gestaltAppleEventsAttr,&response);	
	if(osError == noErr && (BitTst(&response,31 - gestaltAppleEventsPresent)))
	{
		aeFunctionsAvail = true;
	}

	return(aeFunctionsAvail);
}

void Setup()
{
	gDone		= 0;
	gSave		= 1;
	gEWinOpen	= 0;
	gGraphOpen	= 0;
	gIsoTrue	= 0;
	gIWorldOpen	= 0;
	gFrontApp	= 1;
	gList.Dirty	= 0;
	
	gCalcOpen	= 0;
			
	SetRect(&gEWorldRect, 0, 0, kElemWidth, kElemHeight + 201);
	
	gScrollProcUPP	= NewControlActionProc((ProcPtr) ScrollProc);
	gLScrollProcUPP	= NewControlActionProc((ProcPtr) LiveScrollProc);
	gDialogUPP		= NewModalFilterProc((ProcPtr) EventFilter);
	
	if(gAE)
	{
		gAEOpenProc		= NewAEEventHandlerProc((ProcPtr) AEOpenAppEvent);
		gAEOpenDocProc	= NewAEEventHandlerProc((ProcPtr) AEOpenDocEvent);
		gAEPrintDocProc	= NewAEEventHandlerProc((ProcPtr) AEPrintDocEvent);
		gAEQuitProc		= NewAEEventHandlerProc((ProcPtr) AEQuitEvent);
		
		AEInstallEventHandler(kCoreEventClass, kAEOpenApplication, gAEOpenProc, 0, false);
		AEInstallEventHandler(kCoreEventClass, kAEOpenDocuments, gAEOpenDocProc, 0, false);
		AEInstallEventHandler(kCoreEventClass, kAEPrintDocuments, gAEPrintDocProc, 0, false);
		AEInstallEventHandler(kCoreEventClass, kAEQuitApplication, gAEQuitProc, 0, false);
	}
}

void SetupTableRects()
{
	int		theE, xAdd;
	Point	RowPeriod, TopLeft;
	xvar	factor;
	
	if(gWindowSize.Table)
		factor = 1;
	else
		factor = .75;
		
	for(theE = 1; theE < 112; theE++)
	{
		if(theE == 4 || theE == 12 || theE == 56 || theE == 88 || gWindowSize.Table)
			xAdd = 31;
		else
			xAdd = 32;
			
		RowPeriod = ItTable(theE);
		TopLeft.h = (RowPeriod.h-1)*30+10;
		TopLeft.v = (RowPeriod.v-1)*40+15;
		SetRect(&gElementRects[theE], TopLeft.h*factor, TopLeft.v*factor,
			(TopLeft.h+xAdd)*factor, (TopLeft.v+41)*factor);
	}
}

void MenuBarInit()
{
	Handle		MenuBar;
	MenuHandle	AppleMenu, CopyMenu, GradientMenu, ListMenu;
	
	UseData();
	
	if((MenuBar = GetNewMBar(128)) == 0L)
		HandleError(5);
	SetMenuBar(MenuBar);
	
	if((AppleMenu = GetMenuHandle(128)) == 0L)
		HandleError(6);
	AppendResMenu(AppleMenu, 'DRVR');
		
	if((CopyMenu = GetMenu(132)) == 0L)
		HandleError(7);
	InsertMenu(CopyMenu, -1);
	
	if((GradientMenu = GetMenu(133)) == 0L)
		HandleError(7);
	InsertMenu(GradientMenu, -1);
	
	if((ListMenu = GetMenu(134)) == 0L)
		HandleError(7);
	InsertMenu(ListMenu, -1);
	
	DrawMenuBar();
	
	DisableItem(GetMenuHandle(130), 4);
	CorrectMenus();
}

void DragRectInit()
{
	RgnHandle	grey;
	
	grey = GetGrayRgn();
	gDragRect = (*grey)->rgnBBox;
	gDragRect.left += 30;
	gDragRect.right -= 30;
	gDragRect.bottom -=30;
}

void TableInit()
{
	UseData();
	if((gTableWindow = GetNewCWindow(129, 0L, (WindowRef)-1))==0L)
		HandleError(8);
		
	if(ValidPos(gWindPos.TableRect))
		MoveWindow(gTableWindow, gWindPos.TableRect.left, gWindPos.TableRect.top, 0);
		
	gZoom = 1;
	
	SetPort(gTableWindow);	
	SetRect(&gTableRect, 0, 0, kTableWidth, kTableHeight);
	
	NewGWorld( &gTableWorld, 8, &gTableRect, 0L, 0L, 0L );
	if(gTableWorld == 0L)
		HandleError(10);
	else
		Gradient();
}

#pragma mark -
#pragma mark File Setup and Use:
#pragma mark -

void PictureOpen()
{	
	gPictsFile = OpenResFile("\p:Sodium Data Ä:Sodium Picts (HiRes)");
	if(ResError())
	{
		gPictsFile = OpenResFile("\p:Sodium Data Ä:Sodium Picts");
		if(ResError())
		{
			ParamText("\pSodium Picts", "\p", "\p", "\p");
			StopAlert(202, 0L);
			
			gPictsFile = OpenDialog('NaPt');
			if(!gPictsFile)
			{
				StopAlert(200, 0L);
				ExitToShell();
			}
		}
	}	
}

void DataOpen()
{
	gDataFile = OpenResFile("\p:Sodium Data Ä:Sodium Data");
	if(ResError())
	{
		ParamText("\pSodium Data", "\p", "\p", "\p");
		StopAlert(202, 0L);
		
		gDataFile = OpenDialog('NaDt');
		if(!gDataFile)
		{
			StopAlert(200, 0L);
			ExitToShell();
		}
	}
}

void IsotopesOpen()
{
	gIsotopesFile = OpenResFile("\p:Sodium Data Ä:Isotopes");
	if(ResError())
	{
		ParamText("\pIsotopes", "\p", "\p", "\p");
		StopAlert(202, 0L);
		
		gIsotopesFile = OpenDialog('NaDt');
		if(!gIsotopesFile)
		{
			StopAlert(200, 0L);
			ExitToShell();
		}
	}
	
	gIsotopes2File = OpenResFile("\p:Sodium Data Ä:Isotopes 2");
	if(ResError())
	{
		ParamText("\pIsotopes 2", "\p", "\p", "\p");
		StopAlert(202, 0L);
		
		gIsotopes2File = OpenDialog('NaDt');
		if(!gIsotopes2File)
		{
			StopAlert(200, 0L);
			ExitToShell();
		}
	}
	gIsotopes3File = OpenResFile("\p:Sodium Data Ä:Isotopes 3");
	if(ResError())
	{
		ParamText("\pIsotopes 3", "\p", "\p", "\p");
		StopAlert(202, 0L);
		
		gIsotopes3File = OpenDialog('NaDt');
		if(!gIsotopes3File)
		{
			StopAlert(200, 0L);
			ExitToShell();
		}
	}
}

short OpenDialog(OSType type)
{
	StandardFileReply	reply;
	SFTypeList			typeList;
	short				file = 0;
	
	typeList[0] = type;
	StandardGetFile(0L, 1, typeList, &reply);
	
	if(reply.sfGood)
	{
		file = FSpOpenResFile(&reply.sfFile, fsRdWrPerm);
		return file;
	}
	else
		return file;
}

void UsePicts()
{
	UseResFile(gPictsFile);
}

void UseData()
{
	UseResFile(gDataFile);
}

void UseIsotopes(int x)
{
	if(x == 1)
		UseResFile(gIsotopesFile);
	if(x == 2)
		UseResFile(gIsotopes2File);
	if(x == 3)
		UseResFile(gIsotopes3File);
}

#pragma mark -
#pragma mark Event Handlers et al:
#pragma mark -

pascal Boolean EventFilter(DialogPtr dialogPtr,EventRecord *eventStrucPtr,SInt16 *itemHit)
{
	Boolean			handledEvent;
	
	handledEvent = false;
	
	if((eventStrucPtr->what == updateEvt) && ((WindowPtr) eventStrucPtr->message != dialogPtr))
	{
		UpdateWindow((WindowRef)eventStrucPtr->message);
	}
	else
		handledEvent = StdFilterProc(dialogPtr,eventStrucPtr,itemHit);

	return(handledEvent);
}

OSErr AEOpenAppEvent(AppleEvent *appleEvent, AppleEvent *reply, long handlerRefcon)
{
	return(noErr);
}

OSErr AEOpenDocEvent(AppleEvent *appleEvent, AppleEvent *reply, long handlerRefcon)
{
	AEDescList	docList;
	OSErr		osErr, ignoreErr;
	SInt32		numberOfItems, index;
	DescType	returnedType;
	FSSpec		fileSpec;
	AEKeyword	keyWord;
	Size		actualSize;
	short		listFRN, currentFile;
	int			i;
	List 		**listHdl;
	
	osErr = AEGetParamDesc(appleEvent,keyDirectObject,typeAEList,&docList);

	if(osErr == noErr)
	{
		osErr = AEGetAttributePtr(appleEvent,keyMissedKeywordAttr,typeWildCard,&returnedType,0L,0,&actualSize);
		if(osErr == errAEDescNotFound)
		{
			osErr = AECountItems(&docList,&numberOfItems);
			if(osErr == noErr)
			{
				for(index=1;index<=numberOfItems;index++)
				{
					osErr = AEGetNthPtr(&docList,index,typeFSS,&keyWord,&returnedType,(Ptr) &fileSpec,sizeof(fileSpec),&actualSize);
					if(osErr == noErr)
					{
						currentFile  = CurResFile();
						listFRN = FSpOpenResFile(&fileSpec, fsRdWrPerm);
						if(listFRN != -1)
						{
							UseResFile(listFRN);
							
							listHdl = (List**) Get1Resource('L”st', 128);
							if(listHdl != 0L && ResError() != resNotFound)
							{
								for(i = 0; i < kListMax; i++)
									gList.List[i] = (*listHdl)->List[i];
								gList.Sort = (*listHdl)->Sort;
								gList.Dirty = 0;
								
								if(gList.Sort)
								{
									gListColumn = gList.Sort;
									gListPeriod = 0;
								}
								CompareList();
								ShrinkCompareWindow();
							}
							
							UseResFile(currentFile);
						}
					}
					else
						;//doErrorAlert(eGetDescriptorRecord);
				}
			}
		}
		else
			;//doErrorAlert(eMissedRequiredParam);
			
		ignoreErr = AEDisposeDesc(&docList);
	}
	else
		;//doErrorAlert(eGetRequiredParam);
		
	return(osErr);
}

OSErr AEPrintDocEvent(AppleEvent *appleEvent, AppleEvent *reply, long handlerRefcon)
{
	return(noErr);
}

OSErr AEQuitEvent(AppleEvent *appleEvent, AppleEvent *reply, long handlerRefcon)
{
	OSErr		osErr;
	DescType	returnedType;
	Size		actualSize;

	osErr = AEGetAttributePtr(appleEvent,keyMissedKeywordAttr,typeWildCard,&returnedType,0L,0,&actualSize);
	gDone = 1;
	return(noErr);
}

void HandleEvents()
{
	short		item;
	WindowRef	whichWindow;
	char		theChar;
		
	WaitNextEvent(everyEvent, &gEvent, 0L, 0L);
	switch(gEvent.what)
	{
		case mouseDown:
			HandleMouse();
			break;
		case mouseUp:
			HandleMouseUp();
			break;
		case updateEvt:
			UpdateWindow((WindowRef)gEvent.message);
			break;
		case keyDown:
		case autoKey:
			theChar = gEvent.message & charCodeMask;
			if((gEvent.modifiers & cmdKey) != 0)
				HandleMenu(MenuKey(theChar));
			break;
		case app4Evt:
			if((gEvent.message & suspendResumeMessage) == 1)
				gFrontApp = 1;
			else
				gFrontApp = 0;
			CorrectControls();
			break;
		case diskEvt:
			HandleDisk();
			break;
		case kHighLevelEvent:
			HighLevelEvents();
			break;
		case nullEvent:
			break;
	}
	if(IsDialogEvent(&gEvent))
	{
		FindWindow(gEvent.where, &whichWindow);
		if(gEvent.what == keyDown || gEvent.what == autoKey)
		{
			if(HandleKey() == 1)
			{
				DialogSelect(&gEvent, &whichWindow, &item);
			}
		}
		else if(DialogSelect(&gEvent, &whichWindow, &item))
		{
			if(FrontWindow() == gCalcWindow)
				HandleCalculator(item);
		}
	}
	/*if( IsDialogEvent(&gEvent) )
	{
		if(FrontWindow() == gElementWindow)
		{
			if(DialogSelect(&gEvent, &gElementWindow, &item))
				HandleElementDlog(item);
		}
		/*else if(FrontWindow() == gCalcWindow)
		{
			if(DialogSelect(&gEvent, &gCalcWindow, &item))
				HandleCalculator(item);
		}*/
	//}
}

void HighLevelEvents()
{
	OSErr	err;
	
	if(gAE)
		err = AEProcessAppleEvent(&gEvent);
}

int HandleKey()
{
	char			key;
	unsigned long	ticks;
	short			itemType;
	Rect			itemRect;
	Handle			itemHandle;
	
	key = gEvent.message & charCodeMask;
	
	if(FrontWindow() == gCalcWindow)
	{
		if(key == kReturnCharCode || key == kEnterCharCode)
		{
			GetDialogItem(gCalcWindow, 1, &itemType, &itemHandle, &itemRect);
			HiliteControl((ControlHandle)itemHandle, 1);
			Delay(8, &ticks);
			HiliteControl((ControlHandle)itemHandle, kControlNoPart);
			
			HandleCalculator(1);
			
			CorrectControls();
			
			return(0);
		}
		if((gEvent.modifiers & cmdKey) != 0)
			return(0);
		else return(1);
	}
	if(FrontWindow() == gCompareWindow)
	{
		if(key == kBackspaceCharCode || key == kDeleteCharCode)
		{
			if(gListPeriod > 0)
			{	
				int	temp;
				
				temp = gListPeriod;
				gListPeriod = 0;
				RemoveElement(temp);
			}
			if(gListColumn > 0)
			{
				switch(gListColumn)
				{
					case name:
						gListSettings.name = 0;
						gListColumn = 0;
						gList.Sort = 0;
						break;
					case number:
						gListSettings.number = 0;
						gListColumn = 0;
						gList.Sort = 0;
						break;
					case weight:
						gListSettings.mass = 0;
						gListColumn = 0;
						gList.Sort = 0;
						break;
					case melt:
						gListSettings.melt = 0;
						gListColumn = 0;
						gList.Sort = 0;
						break;
					case boil:
						gListSettings.boil = 0;
						gListColumn = 0;
						gList.Sort = 0;
						break;
					case neg:
						gListSettings.neg = 0;
						gListColumn = 0;
						gList.Sort = 0;
						break;
					case shc:
						gListSettings.shc = 0;
						gListColumn = 0;
						gList.Sort = 0;
						break;
					case ion:
						gListSettings.ion = 0;
						gListColumn = 0;
						gList.Sort = 0;
						break;
					case radii:
						gListSettings.radii = 0;
						gListColumn = 0;
						gList.Sort = 0;
						break;
					case density:
						gListSettings.density = 0;
						gListColumn = 0;
						gList.Sort = 0;
						break;
				}
				CompareList();
			}
		}
	}
	
	return (1);
}

void HandleMouse()
{
	WindowPtr	whichWindow;
	short int	thePart;
	long int	menuChoice;
	
	thePart = FindWindow(gEvent.where, &whichWindow);
	switch(thePart)
	{
		case inMenuBar:
			menuChoice = MenuSelect(gEvent.where);
			HandleMenu(menuChoice);
			break;
		case inSysWindow:
			SystemClick(&gEvent, whichWindow);
			break;
		case inContent:
			if(whichWindow != FrontWindow())
			{
				SelectWindow(whichWindow);
				SetPort(whichWindow);
								
				CorrectMenus();
				CorrectControls();
			}
			else
				HandleContent(whichWindow);
			break;
		case inGoAway:
			CloseWin(whichWindow, gEvent.where);
			CorrectControls();
			break;
		case inDrag:
			DragWindow(whichWindow, gEvent.where, &gDragRect);
			if(whichWindow == gTableWindow)
				gWindMove.Table = 1;
			if(whichWindow == gElementWindow)
				gWindMove.Element = 1;
			if(whichWindow == gGraphWindow)
				gWindMove.Graph = 1;
			
			CorrectMenus();
			CorrectControls();
			break;
		case inZoomIn:
		case inZoomOut:
			if(TrackBox(whichWindow, gEvent.where, thePart))
			{
				if(whichWindow == gTableWindow)
				{
					xvar	factor;
					
					gWindowSize.Table = !gWindowSize.Table;
					
					if(gWindowSize.Table)
						factor = 1;
					else
						factor = .75;
						
					SizeWindow(gTableWindow, kTableWidth * factor, kTableHeight * factor, 1);
					gZoom = 1;
					Gradient();
				}
				if(whichWindow == gCompareWindow)
				{
					ShrinkCompareWindow();
				}
			}
			break;
		case inGrow:
			if(whichWindow == gCompareWindow)
			{
				long	size;
				Rect	bounds;
				short	itemType;
				Rect	itemRect;
				Handle	itemHandle;
				
				bounds.top = 100;
				if((gCompScroll.items+1)*13 > 100)
					bounds.bottom = (gCompScroll.items)*13 + 3;
				else
					bounds.bottom = 100;
				if((gCompScroll.items)*13 + 2 > 400)
					bounds.bottom = 401;
				if(gMinListWidth)
					bounds.left = gMinListWidth+10;
				else
					bounds.left = 330;
				bounds.right = 600;
				
				size = GrowWindow(whichWindow, gEvent.where, &bounds);
				if(size)
				{
					SizeWindow(whichWindow, LoWord(size), HiWord(size), true);
					GetDialogItem(gCompareWindow, 1, &itemType, &itemHandle, &itemRect);
					SizeControl((ControlHandle)itemHandle, 16, HiWord(size)-13);
					MoveControl((ControlHandle)itemHandle, LoWord(size)-15, -1);
				}
				
				if(gCompScroll.current+(HiWord(size)/13) > gCompScroll.items && gCompScroll.max > gCompScroll.items)
				{
					gCompScroll.current = gCompScroll.items-(HiWord(size)/13);
				}
				UpdateCompare();
				CorrectControls();
			}
			break;
	}
}

void HandleContent(WindowRef whichWindow)
{
	short		itemType, item;
	Rect		itemRect;
	Handle		itemHandle, ScrollHandle;
	GrafPtr		oldPort;
	Point		thePoint;
	short int	thePart;
	
	if(whichWindow == gCompareWindow)
	{
		thePoint = gEvent.where;
		GetPort(&oldPort);
		SetPort(gCompareWindow);
		GlobalToLocal(&(thePoint));
		
		thePart = FindControl(thePoint, gCompareWindow, &(ControlHandle)itemHandle);
		
		GetDialogItem(gCompareWindow, 1, &itemType, &ScrollHandle, &itemRect);
		if(itemHandle == ScrollHandle)
		{
			if(thePart == kControlIndicatorPart)
			{
				thePart = TrackControl((ControlHandle)itemHandle, thePoint, gLScrollProcUPP);
				gCompScroll.current = GetControlValue((ControlHandle)ScrollHandle);
			}
			else
				thePart = TrackControl((ControlHandle)itemHandle, thePoint, gScrollProcUPP);
			
			UpdateCompare();
		}
		else HandleListClick(gEvent.where);
		SetPort(oldPort);
	}
	if(whichWindow == gTableWindow)
		HandleTableDown(gEvent.where);
	if(whichWindow == gGraphWindow)
		HandleGraphClick(gEvent.where);
	if(whichWindow == gElementWindow)
	{
		thePoint = gEvent.where;
		GetPort(&oldPort);
		SetPort(gElementWindow);
		GlobalToLocal(&(thePoint));
		
		thePart = FindControl(thePoint, gElementWindow, &(ControlHandle)itemHandle);
		
		GetDialogItem(gElementWindow, 2, &itemType, &ScrollHandle, &itemRect);
		if(itemHandle == ScrollHandle)
		{
			if(thePart == kControlIndicatorPart)
			{
				thePart = TrackControl((ControlHandle)itemHandle,thePoint,gLScrollProcUPP);
				gElementScroll.current = GetControlValue((ControlHandle)ScrollHandle);
			}
			else
				thePart = TrackControl((ControlHandle)itemHandle,thePoint,gScrollProcUPP);
			
			IsotopeScrolled();
		}
		else if(DialogSelect(&gEvent, &gElementWindow, &item))
			HandleElementDlog(item);
		else
			HandleElementClick(thePoint);
	}
}

void HandleMouseUp()
{
	WindowPtr	whichWindow;
	short int	thePart;

	thePart = FindWindow(gEvent.where, &whichWindow);
	switch(thePart)
	{
		case inContent:
			if(whichWindow == gTableWindow)
			{
				if(gTableWindow == FrontWindow())
				{
					HandleTableUp(gEvent.where);
					gClickingE = 0;
				}
			}
			break;
	}
}

void UpdateWindow(WindowRef whichWindow)
{
	GrafPtr	oldPort;
				
	GetPort(&oldPort);
	SetPort(whichWindow);
	
	BeginUpdate(whichWindow);
	
	if(whichWindow == gElementWindow)
	{
		CopyBitsElement();
		UpdateDialog(gElementWindow, gElementWindow->visRgn);
		DrawControls(gElementWindow);
	}
	else if(whichWindow == gTableWindow)
	{
		SetRect(&gTableRect, 0, 0, 560, 420);
		CopyBits( &PORTBITMAP( gTableWorld ), &WINBITMAP( gTableWindow ),
			&gTableRect, &gTableRect, srcCopy, 0L );
	}
	else if(whichWindow == gGraphWindow)
	{
		CopyBits( &PORTBITMAP( gGraphWorld ), &WINBITMAP( gGraphWindow ),
			&gGraphWorld->portRect, &gGraphWorld->portRect, srcCopy, 0L );
	}
	else if(whichWindow == gCalcWindow)
	{
		UpdateDialog(gCalcWindow, gCalcWindow->visRgn);
	}
	else if(whichWindow == gCompareWindow)
	{
		UpdateCompare();
		UpdateDialog(gCompareWindow, gCompareWindow->visRgn);
		DrawControls(gCompareWindow);
	}
	else
	{
		UpdateDialog(whichWindow, whichWindow->visRgn);
	}
	
	EndUpdate(whichWindow);
	SetPort(oldPort);
}

void HandleMenu(long int menuChoice)
{
	int menu;
	int item;
	
	if(menuChoice != 0)
	{
		menu = HiWord(menuChoice);
		item = LoWord(menuChoice);
		switch(menu)
		{
			case 128:
				HandleApple(item);
				break;
			case 129:
				HandleFile(item);
				break;
			case 130:
				HandleEdit(item);
				break;
			case 131:
				HandleSpecial(item);
				break;
				
			case 132:
				HandleCopy(item);
				break;
			case 133:
				gTablePrefs.style = item;
				Gradient();
				break;
			case 134:
				HandleListMenu(item);
				break;
		}
		HiliteMenu(0);
	}
}

void HandleApple(int item)
{
	Str255		dskName;
	int			dskNum;
	
	switch(item)
	{
		case 1:
			AboutSodium();
			break;
		default:
			GetMenuItemText(GetMenuHandle(128), item, dskName);
			dskNum = OpenDeskAcc(dskName);
			break;
	}
}


void HandleFile(int item)
{
	WindowPtr	window;
	
	switch(item)
	{
		case 1:
			if((window = FrontWindow()) != 0L)
				CloseWinM(window);
			break;
		case 3:
			if(gPrintRec == 0L)
				gPrintRec = (THPrint)NewHandle(sizeof(TPrint));
			PrOpen();
			PrStlDialog(gPrintRec);
			PrClose();
			break;
		case 4:
			HandlePrint();
			break;
		case 6:
			gDone = 1;
			break;
	}
}

void HandleEdit(int item)
{	
	switch(item)
	{
		case 3:
			if(FrontWindow() == gCalcWindow)
				DialogCut(gCalcWindow);
			break;
		case 4:
			if(FrontWindow() == gCalcWindow)
				DialogCopy(gCalcWindow);
			break;
		case 5:
			if(FrontWindow() == gCalcWindow)
				DialogPaste(gCalcWindow);
			break;
		case 8:
			PreferencesDialog();
			Gradient();
			if(gEWinOpen && IsWindowVisible(gElementWindow))
				Element(gE);
			if(gGraphOpen && IsWindowVisible(gGraphWindow))
				Graph();
			break;
	}
}

void HandleSpecial(int item)
{
	Point	where;
	Rect	screen;
	
	switch(item)
	{
		case 1:
			SearchDialog();
			break;
		case 2:
			CalculatorWindow();
			break;
		case 5:
			GraphDialog();
			break;
		case 7:
			screen = qd.screenBits.bounds;
			where.h = screen.right/2-500/2;
			where.v = screen.bottom/2-300/2;
			
			GetColor(where,"\pWhat isotope background color?",&Grey,&Grey);
			break;
	}
}

void HandleListMenu(int item)
{
	int	i;
	
	switch(item)
	{
		case 1:
			CompareList();
			ShowWindow(gCompareWindow);
			SelectWindow(gCompareWindow);
			
			CorrectControls();
			CorrectMenus();
			break;
		case 2:
			SetupList();
			break;
		case 4:
			SaveList();
			break;
		case 5:
			if(LoadList())
			{
				CompareList();
				ShrinkCompareWindow();
			}
			break;
		case 7:
			for(i = 0; i < kListMax; i++)
				gList.List[i] = 0;
			gList.Dirty = 1;
			CompareList();
	}
}

void CloseWin(WindowRef whichWindow, Point whereClose)
{
	Boolean		close;
	
	close = TrackGoAway(whichWindow, whereClose);
	if(close)
	{
		if(whichWindow == gElementWindow)
		{
			HideWindow(gElementWindow);
		}
		if(whichWindow == gCalcWindow)
			HideWindow(gCalcWindow);
		if(whichWindow == gGraphWindow)
		{
			HideWindow(gGraphWindow);
				
			gGraphOpen = 0;
		}
		if(whichWindow == gCompareWindow)
			HideWindow(gCompareWindow);
	}
	CorrectMenus();
	CorrectControls();
}

void HandleTableDown(Point gclick)
{		
	Point		click;
	int			period, row, theE;
	xvar		factor;
	
	if(gWindowSize.Table)
		factor = 1;
	else
		factor = .75;
	
	click = gclick;
	SetPort(gTableWindow);
	GlobalToLocal(&click);
	
	theE = 0;
	
	click.h = click.h / factor;
	click.v = click.v / factor;

	if(click.h >= 10 && click.v >= 15 && click.h <= 580 && click.v <= 415)
	{		
		row = ((click.h - 10) / 30);
		period = ((click.v - 15) / 40);
		
		row++; period++;
	
		theE = TableIt(period, row);
		
		if(theE > 0 && theE < 110)
		{
			gClickingE = theE;
		}
	}
}

Boolean HandleTableUp(Point gclick)
{		
	Point		click;
	int			period, row, theE;
	xvar		factor;
	
	if(gWindowSize.Table)
		factor = 1;
	else
		factor = .75;
	
	click = gclick;
	SetPort(gTableWindow);
	GlobalToLocal(&click);
	
	theE = 0;
	
	click.h = click.h / factor;
	click.v = click.v / factor;

	if(click.h >= 10 && click.v >= 15 && click.h <= 580 && click.v <= 415)
	{		
		row = ((click.h - 10) / 30);
		period = ((click.v - 15) / 40);
		
		row++; period++;
	
		theE = TableIt(period, row);
		
		if(theE > 0 && theE < 110 && theE == gClickingE)
		{
			if(gEvent.modifiers & optionKey)
			{
				AddElement(theE);
			}
			else
			{
				gElementScroll.current = 0;
				Element(theE);
				
				SelectWindow(gElementWindow);
				CorrectControls();
			}
			CorrectMenus();
			return(true);
		}
	}
	return(false);
}

void HandleGraphClick(Point gclick)
{		
	Point		click;
	int			e, types = 0;
	Str255		string;
	Rect		eRect;
//	FontInfo	fontInfo;
	GrafPtr		oldPort;
	GWorldPtr	tempWorld;
	GraphRes	graph;
	
	UseData();
	GetPort(&oldPort);
	HideCursor();
	
	SetRect(&eRect, 0, 0, gGraphWindow->portRect.right, 20);
	
	NewGWorld( &tempWorld, 8, &gGraphWindow->portRect, 0L, 0L, 0L );
	if(gGraphWorld == 0L)
		HandleError(19);
	
	click = gclick;
	SetPort(gGraphWindow);
	GlobalToLocal(&click);
	
	do
	{
		e = (click.h+(kGraphHScale)/2) / kGraphHScale + 1;
		LockPixels(GetGWorldPixMap(tempWorld));
		SetGWorld((CGrafPtr)tempWorld, 0L);
		
		CopyBits( &PORTBITMAP( gGraphWorld ), &PORTBITMAP( tempWorld ),
			&gGraphWorld->portRect, &gGraphWorld->portRect, srcCopy, 0L );
		
		TextFont(courier);
		TextSize(10);
		TextFace(normal);
		
		RGBBackColor(&White);
		EraseRect(&eRect);
		
		MoveTo(3, 13);
		NumToString(e, string);
		DrawString("\pAtomic Number: ");
		DrawString(string);
		
		MoveTo(125, 13);
		GetIndString(string, kResName, e);
		DrawString("\pName: ");
		DrawString(string);
		
		types = 0;
		graph = gGraph;
		if(gGraph.Melt)
		{
			GetIndString(string, kResMelt, e);
			if(IUCompString(string, "\p") != 0)
				types++;
			else
				graph.Melt = 0;
		}
		if(gGraph.Boil)
		{
			GetIndString(string, kResBoil, e);
			if(IUCompString(string, "\p") != 0)
				types++;
			else
				graph.Boil = 0;
		}
		if(gGraph.Ionization)
		{
			GetIndString(string, kResIonization, e);
			if(IUCompString(string, "\p") != 0)
				types++;
			else
				graph.Ionization = 0;
		}
		if(gGraph.Negativity)
		{
			GetIndString(string, kResNeg, e);
			if(IUCompString(string, "\p") != 0)
				types++;
			else
				graph.Negativity = 0;
		}
		if(gGraph.Radii)
		{
			GetIndString(string, kResRadii, e);
			if(IUCompString(string, "\p") != 0)
				types++;
			else
				graph.Radii = 0;
		}
		if(gGraph.Weight)
		{
			GetIndString(string, kResWeight, e);
			if(IUCompString(string, "\p") != 0)
				types++;
			else
				graph.Weight = 0;
		}
			
		MoveTo(250, 13);
		if(types > 0 && types < 3)
		{
			//DrawString("\pValue: ");
			if(graph.Melt)
			{
				RGBForeColor(&gGraph.MeltColor);
				GetIndString(string, kResMelt, e);
				
				ConvertStringCFK(string);
				DrawString(string);
				
				if(gTempType.tempType == kCelcius)
					DrawString("\p¡ C");
				if(gTempType.tempType == kFahrenheit)
					DrawString("\p¡ F");
				if(gTempType.tempType == kKelvin)
					DrawString("\p K");
					
				if(types > 1)
					DrawString("\p, ");
				types--;
			}
			if(graph.Boil)
			{
				RGBForeColor(&gGraph.BoilColor);
				GetIndString(string, kResBoil, e);
				
				ConvertStringCFK(string);
				DrawString(string);
				
				if(gTempType.tempType == kCelcius)
					DrawString("\p¡ C");
				if(gTempType.tempType == kFahrenheit)
					DrawString("\p¡ F");
				if(gTempType.tempType == kKelvin)
					DrawString("\p K");
					
				if(types > 1)
					DrawString("\p, ");
				types--;
			}
			if(graph.Ionization)
			{
				RGBForeColor(&gGraph.IonizationColor);
				GetIndString(string, kResIonization, e);
				DrawString(string);
				DrawString("\p kJ/mol");
				if(types > 1)
					DrawString("\p, ");
				types--;
			}
			if(graph.Negativity)
			{
				RGBForeColor(&gGraph.NegativityColor);
				GetIndString(string, kResNeg, e);
				DrawString(string);
				DrawString("\p negativity");
				if(types > 1)
					DrawString("\p, ");
				types--;
			}
			if(graph.Radii)
			{
				RGBForeColor(&gGraph.RadiiColor);
				GetIndString(string, kResRadii, e);
				DrawString(string);
				DrawString("\p nm");
				if(types > 1)
					DrawString("\p, ");
				types--;
			}
			if(graph.Weight)
			{
				RGBForeColor(&gGraph.WeightColor);
				GetIndString(string, kResWeight, e);
				DrawString(string);
				DrawString("\p amu");
				if(types > 1)
					DrawString("\p, ");
				types--;
			}
			
			RGBForeColor(&Black);
		}
		
		RGBBackColor(&White);
		MoveTo(0, 20);
		LineTo(gGraphWorld->portRect.right, 20);
		RGBForeColor(&Grey);
		MoveTo(0, 19);
		LineTo(gGraphWorld->portRect.right, 19);
		RGBForeColor(&Black);
				
		MoveTo(0, click.v);
		LineTo(gGraphWorld->portRect.right, click.v);
		MoveTo(click.h, 20);
		LineTo(click.h, gGraphWorld->portRect.bottom);
		
		SetGWorld( (CGrafPtr)gGraphWindow, 0L );
		CopyBits( &PORTBITMAP( tempWorld ), &WINBITMAP( gGraphWindow ),
			&gGraphWorld->portRect, &gGraphWorld->portRect, srcCopy, 0L );
		
		GetMouse(&click);
		
		if(click.v < 20)
			click.v = 21;
		if(click.h < 0)
			click.h = 0;
		if(click.v > gGraphWorld->portRect.bottom)
			click.v = gGraphWorld->portRect.bottom - 1;
		if(click.h > gGraphWorld->portRect.right)
			click.h = gGraphWorld->portRect.right - 1;
	} while(Button());
	
	eRect.bottom++;
	CopyBits( &PORTBITMAP( tempWorld ), &PORTBITMAP( gGraphWorld ),
		&eRect, &eRect, srcCopy, 0L );
	DisposeGWorld(tempWorld);
	
	MacShowCursor();
	CopyBits( &PORTBITMAP( gGraphWorld ), &WINBITMAP( gGraphWindow ),
		&gGraphWorld->portRect, &gGraphWorld->portRect, srcCopy, 0L );
	SetPort(oldPort);
}

void HandleListClick(Point gclick)
{		
	Point		click;
	int			x, line;
	GrafPtr		oldPort;
	
	UseData();
	GetPort(&oldPort);
		
	click = gclick;
	SetPort(gCompareWindow);
	GlobalToLocal(&click);
	
	if((click.v + gCompScroll.current*13) < 16)
	{
		x = 0;
		if(gListSettings.name)
		{
			if(click.h > x && click.h < x + 80)
			{
				gListColumn = name;
				gList.Sort = name;
			}
			x += 80;
		}
		if(gListSettings.symbol)
		{
			if(click.h > x && click.h < x + 30)
			{
				gListColumn = symbol;
				gList.Sort = symbol;
			}
			x += 30;
		}
		if(gListSettings.number)
		{
			if(click.h > x && click.h < x + 30)
			{
				gListColumn = number;
				gList.Sort = number;
			}
			x += 30;
		}
		if(gListSettings.mass)
		{
			if(click.h > x && click.h < x + 70)
			{
				gListColumn = weight;
				gList.Sort = weight;
			}
			x += 70;
		}
		if(gListSettings.melt)
		{
			if(click.h > x && click.h < x + 60)
			{
				gListColumn = melt;
				gList.Sort = melt;
			}
			x += 60;
		}
		if(gListSettings.boil)
		{
			if(click.h > x && click.h < x + 60)
			{
				gListColumn = boil;
				gList.Sort = boil;
			}
			x += 60;
		}
		if(gListSettings.neg)
		{
			if(click.h > x && click.h < x + 40)
			{
				gListColumn = neg;
				gList.Sort = neg;
			}
			x += 40;
		}
		if(gListSettings.shc)
		{
			if(click.h > x && click.h < x + 40)
			{
				gListColumn = shc;
				gList.Sort = shc;
			}
			x += 40;
		}
		if(gListSettings.ion)
		{
			if(click.h > x && click.h < x + 40)
			{
				gListColumn = ion;
				gList.Sort = ion;
			}
			x += 40;
		}
		if(gListSettings.radii)
		{
			if(click.h > x && click.h < x + 40)
			{
				gListColumn = radii;
				gList.Sort = radii;
			}
			x += 40;
		}
		if(gListSettings.density)
		{
			if(click.h > x && click.h < x + 80)
			{
				gListColumn = density;
				gList.Sort = density;
			}
			x += 80;
		}
		
		gListPeriod = 0;
		CompareList();
	}
	else
	{
		line = click.v/13 + gCompScroll.current;
		if(line < gCompScroll.items)
		{
			gListPeriod = line;
			gListColumn = 0;
			CompareList();
		}
	}
	SetPort(oldPort);
}

void AboutSodium()
{
	Handle			controlHdl;
	SInt16 			itemHit;
	short			itemType;
	Rect			itemRect;
	EventRecord		Event;
	DialogPtr		About;
	
	UseData();
	if(gE == 11)
	{
		SetCCursor(GetCCursor(128));
	}
	
	if(!(About = GetNewDialog(128,0L,(WindowPtr) -1)))
		return;

	SetDialogDefaultItem(About, 1);
		
	ShowWindow(About);
	
	do
	{
		ModalDialog(gDialogUPP, &itemHit);
		
		WaitNextEvent(everyEvent, &Event, 0L, 0L);
		
		if( IsDialogEvent(&Event) )
		{
			GetDialogItem(About, itemHit, &itemType, &controlHdl, &itemRect);
			SetControlValue((ControlHandle)controlHdl,!GetControlValue((ControlHandle) controlHdl));
		}
	} while(itemHit != 1);
	
	HideWindow(About);
	DisposeDialog(About);
		
	SetCursor(&qd.arrow);
}

void CloseWinM(WindowRef whichWindow)
{
	if(whichWindow == gElementWindow)
		HideWindow(gElementWindow);
	if(whichWindow == gCalcWindow)
		HideWindow(gCalcWindow);
	if(whichWindow == gGraphWindow)
	{
		HideWindow(gGraphWindow);
				
		gGraphOpen = 0;
	}
	if(whichWindow == gCompareWindow)
		HideWindow(gCompareWindow);
	
	CorrectMenus();
	CorrectControls();
}

void HandleDisk()
{
	OSErr	result;
	
	if ( HiWord(gEvent.message) != noErr )
	{
		Point	where;
		
		SetPt(&where, 40, 40);
		result = DIBadMount(where, gEvent.message);
	}
}

void HandleElementClick(Point where)
{
	int				column, isotope;
	Str255			pE;
	unsigned long	now;
	
	if(where.v > kElemHeight+6 && gEvent.modifiers & controlKey)
	{
		if(where.h < 60)
			column = 1;
		else if(where.h < 160)
			column = 2;
		else if(where.h < 285)
			column = 3;
		else column = 4;
		
		isotope = (where.v-(kElemHeight+6))/13 + gElementScroll.current + 1;
		
		switch(column)
		{
			case 1:
				if(IUCompString(gDIsoW[isotope], "\p") != 0 && IUCompString(gDIsoW[isotope], "\p~") != 0)
					CopyText(&(gDIsoW[isotope]), 0);
				else
					CopyText((Str255 *)&"\p0", 0);
				break;
			case 2:
				if(IUCompString(gDWeight[isotope], "\p") != 0 && IUCompString(gDWeight[isotope], "\p~") != 0)
					CopyText(&(gDWeight[isotope]), 0);
				else
					CopyText((Str255 *)&"\p0", 0);
				break;
			case 3:
				if(IUCompString(gDAbund[isotope], "\p") != 0 && IUCompString(gDAbund[isotope], "\p~") != 0)
					CopyText(&(gDAbund[isotope]), 0);
				else
					CopyText((Str255 *)&"\p0", 0);
				break;
			case 4:
				if(IUCompString(gDHalfL[isotope], "\p") != 0 && IUCompString(gDHalfL[isotope], "\p~") != 0)
					CopyText(&(gDHalfL[isotope]), 0);
				else
					CopyText((Str255 *)&"\p0", 0);
				break;
		}
	}
	else if(gEvent.modifiers & controlKey)
	{
		switch(ElementClick(where))
		{
			case name:
				if(IUCompString(gNameText, "\p") != 0)
					CopyText(&gNameText, 1);
				else
					CopyText((Str255 *)&"\p0", 0);
				
				RGBForeColor(&Black);
				FrameRect(&gTextRects.Name);
				Delay(kFlashDelay, &now);
				CopyBits( &PORTBITMAP(gElementWorld), &WINBITMAP(gElementWindow),
					&gTextRects.Name, &gTextRects.Name, srcCopy, 0L );
				break;
			case number:
				NumToString(gE, pE);
				if(IUCompString(pE, "\p") != 0)
					CopyText(&pE, 0);
				else
					CopyText((Str255 *)&"\p0", 0);
				
				RGBForeColor(&Black);
				FrameRect(&gTextRects.Number);
				Delay(kFlashDelay, &now);
				CopyBits( &PORTBITMAP(gElementWorld), &WINBITMAP(gElementWindow),
					&gTextRects.Number, &gTextRects.Number, srcCopy, 0L );
				break;
			case weight:
				if(IUCompString(gWeightText, "\p") != 0)
					CopyText(&gWeightText, 2);
				else
					CopyText((Str255 *)&"\p0", 0);
				
				RGBForeColor(&Black);
				FrameRect(&gTextRects.Weight);
				Delay(kFlashDelay, &now);
				CopyBits( &PORTBITMAP(gElementWorld), &WINBITMAP(gElementWindow),
					&gTextRects.Weight, &gTextRects.Weight, srcCopy, 0L );
				break;
			case config:
				if(IUCompString(gEConfigText, "\p") != 0)
					CopyText(&gEConfigText, 0);
				else
					CopyText((Str255 *)&"\p0", 0);
				
				RGBForeColor(&Black);
				FrameRect(&gTextRects.ElecConfig);
				Delay(kFlashDelay, &now);
				CopyBits( &PORTBITMAP(gElementWorld), &WINBITMAP(gElementWindow),
					&gTextRects.ElecConfig, &gTextRects.ElecConfig, srcCopy, 0L );
				break;
			case melt:
				if(IUCompString(gMeltText, "\p") != 0)
					CopyText(&gMeltText, 5);
				else
					CopyText((Str255 *)&"\p0", 0);
				
				RGBForeColor(&Black);
				FrameRect(&gTextRects.Melt);
				Delay(kFlashDelay, &now);
				CopyBits( &PORTBITMAP(gElementWorld), &WINBITMAP(gElementWindow),
					&gTextRects.Melt, &gTextRects.Melt, srcCopy, 0L );
				break;
			case boil:
				if(IUCompString(gBoilText, "\p") != 0)
					CopyText(&gBoilText, 6);
				else
					CopyText((Str255 *)&"\p0", 0);
				
				RGBForeColor(&Black);
				FrameRect(&gTextRects.Boil);
				Delay(kFlashDelay, &now);
				CopyBits( &PORTBITMAP(gElementWorld), &WINBITMAP(gElementWindow),
					&gTextRects.Boil, &gTextRects.Boil, srcCopy, 0L );
				break;
			case oxi:
				if(IUCompString(gOxiText, "\p") != 0)
					CopyText(&gOxiText, 7);
				else
					CopyText((Str255 *)&"\p0", 0);
				
				RGBForeColor(&Black);
				FrameRect(&gTextRects.Oxi);
				Delay(kFlashDelay, &now);
				CopyBits( &PORTBITMAP(gElementWorld), &WINBITMAP(gElementWindow),
					&gTextRects.Oxi, &gTextRects.Oxi, srcCopy, 0L );
				break;
			case neg:
				if(IUCompString(gNegText, "\p") != 0)
					CopyText(&gNegText, 8);
				else
					CopyText((Str255 *)&"\p0", 0);
				
				RGBForeColor(&Black);
				FrameRect(&gTextRects.Neg);
				Delay(kFlashDelay, &now);
				CopyBits( &PORTBITMAP(gElementWorld), &WINBITMAP(gElementWindow),
					&gTextRects.Neg, &gTextRects.Neg, srcCopy, 0L );
				break;
			case shc:
				if(IUCompString(gSHCText, "\p") != 0)
					CopyText(&gSHCText, 9);
				else
					CopyText((Str255 *)&"\p0", 0);
				
				RGBForeColor(&Black);
				FrameRect(&gTextRects.SHC);
				Delay(kFlashDelay, &now);
				CopyBits( &PORTBITMAP(gElementWorld), &WINBITMAP(gElementWindow),
					&gTextRects.SHC, &gTextRects.SHC, srcCopy, 0L );
				break;
			case ion:
				if(IUCompString(gIonText, "\p") != 0)
					CopyText(&gIonText, 10);
				else
					CopyText((Str255 *)&"\p0", 0);
				
				RGBForeColor(&Black);
				FrameRect(&gTextRects.Ion);
				Delay(kFlashDelay, &now);
				CopyBits( &PORTBITMAP(gElementWorld), &WINBITMAP(gElementWindow),
					&gTextRects.Ion, &gTextRects.Ion, srcCopy, 0L );
				break;
			case disc:
				if(IUCompString(gDateText, "\p") != 0)
					CopyText(&gDateText, 0);
				else
					CopyText((Str255 *)&"\p0", 0);
				
				RGBForeColor(&Black);
				FrameRect(&gTextRects.Disc);
				Delay(kFlashDelay, &now);
				CopyBits( &PORTBITMAP(gElementWorld), &WINBITMAP(gElementWindow),
					&gTextRects.Disc, &gTextRects.Disc, srcCopy, 0L );
				break;
			case radii:
				if(IUCompString(gRadiiText, "\p") != 0)
					CopyText(&gRadiiText, 3);
				else
					CopyText((Str255 *)&"\p0", 0);
				
				RGBForeColor(&Black);
				FrameRect(&gTextRects.Radii);
				Delay(kFlashDelay, &now);
				CopyBits( &PORTBITMAP(gElementWorld), &WINBITMAP(gElementWindow),
					&gTextRects.Radii, &gTextRects.Radii, srcCopy, 0L );
				break;
			case density:
				if(IUCompString(gDensityText, "\p") != 0)
					CopyText(&gDensityText, 4);
				else
					CopyText((Str255 *)&"\p0", 0);
				
				RGBForeColor(&Black);
				FrameRect(&gTextRects.Density);
				Delay(kFlashDelay, &now);
				CopyBits( &PORTBITMAP(gElementWorld), &WINBITMAP(gElementWindow),
					&gTextRects.Density, &gTextRects.Density, srcCopy, 0L );
				break;
			case cstruct:
				if(IUCompString(gDensityText, "\p") != 0)
					CopyText((Str255 *)&"\pCubic face centered", 0);
				else
					CopyText((Str255 *)&"\p", 0);
				
				RGBForeColor(&Black);
				FrameRect(&gTextRects.Crystal);
				Delay(kFlashDelay, &now);
				CopyBits( &PORTBITMAP(gElementWorld), &WINBITMAP(gElementWindow),
					&gTextRects.Crystal, &gTextRects.Crystal, srcCopy, 0L );
				break;
		}
	}
	else if(gEvent.modifiers & optionKey)
	{
		switch(ElementClick(where))
		{
			case name:
				gTablePrefs.style = kGradName;
				Gradient();
				break;
			case radii:
				gTablePrefs.style = kGradRadii;
				Gradient();
				break;
			case melt:	
				gTablePrefs.style = kGradMelt;
				Gradient();
				break;
			case boil:
				gTablePrefs.style = kGradBoil;
				Gradient();
				break;
			case ion:
				gTablePrefs.style = kGradIon;
				Gradient();
				break;
			case neg:
				gTablePrefs.style = kGradNeg;
				Gradient();
				break;
			case density:
				gTablePrefs.style = kGradDensity;
				Gradient();
				break;
		}
	}
}

int ElementClick(Point where)
{
	if(MacPtInRect(where, &gTextRects.Name))
		return(name);
	if(MacPtInRect(where, &gTextRects.Number))
		return(number);
	if(MacPtInRect(where, &gTextRects.Weight))
		return(weight);
	if(MacPtInRect(where, &gTextRects.ElecConfig))
		return(config);
	if(MacPtInRect(where, &gTextRects.Melt))
		return(melt);
	if(MacPtInRect(where, &gTextRects.Boil))
		return(boil);
	if(MacPtInRect(where, &gTextRects.Oxi))
		return(oxi);
	if(MacPtInRect(where, &gTextRects.Neg))
		return(neg);
	if(MacPtInRect(where, &gTextRects.SHC))
		return(shc);
	if(MacPtInRect(where, &gTextRects.Ion))
		return(ion);
	if(MacPtInRect(where, &gTextRects.Disc))
		return(disc);
	if(MacPtInRect(where, &gTextRects.Radii))
		return(radii);
	if(MacPtInRect(where, &gTextRects.Density))
		return(density);
	if(MacPtInRect(where, &gTextRects.Crystal))
		return(cstruct);
}

void HandleElementDlog(int item)
{
	short	itemType;
	Rect	itemRect;
	Handle	itemHandle;
	
	switch(item)
	{
		case 1:
			GetDialogItem(gElementWindow, 1, &itemType, &itemHandle, &itemRect);
			SetControlValue((ControlHandle)itemHandle, !GetControlValue((ControlHandle)itemHandle));
			
			if(gIsoTrue)
			{
				gIsoTrue = 0;
				SizeWindow(gElementWindow, kElemWidth, kElemHeight, true);
			}
			else
			{
				gIsoTrue = 1;
				Isotope(gE);
			}

			UpdateDialog(gElementWindow, gElementWindow->visRgn);
			DrawControls(gElementWindow);
			break;
	}
}

void HandleCalculator(int item)
{
	Str255	string, answer;
	short	itemType;
	Rect	itemRect;
	Handle	itemHandle;

	switch(item)
	{
		case 1:
			GetDialogItem(gCalcWindow, 2, &itemType, &itemHandle, &itemRect);
			GetDialogItemText(itemHandle, string);
			
			Calculator(string, &answer);
			
			GetDialogItem(gCalcWindow, 3, &itemType, &itemHandle, &itemRect);
			SetDialogItemText(itemHandle, answer);
			SelectDialogItemText(gCalcWindow, 3, 0, 32767);
			break;
	}
}

void CorrectMenus()
{
	WindowRef	whichWindow;
	
	whichWindow = FrontWindow();
	if(whichWindow == gTableWindow)
	{
		DisableItem(GetMenuHandle(129), 1);
		EnableItem(GetMenuHandle(129), 3);
		EnableItem(GetMenuHandle(129), 4);
		
		DisableItem(GetMenuHandle(130), 3);
		DisableItem(GetMenuHandle(130), 4);
		DisableItem(GetMenuHandle(130), 5);
	}
	if(whichWindow == gElementWindow)
	{
		EnableItem(GetMenuHandle(129), 1);
		EnableItem(GetMenuHandle(129), 3);
		EnableItem(GetMenuHandle(129), 4);
		
		EnableItem(GetMenuHandle(130), 4);
		SetItemCmd(GetMenuHandle(130), 4, 0x1B);
		SetItemMark(GetMenuHandle(130), 4, 0x84);
		DisableItem(GetMenuHandle(130), 3);
		DisableItem(GetMenuHandle(130), 5);
	}
	if(whichWindow == gGraphWindow)
	{
		EnableItem(GetMenuHandle(129), 1);
		EnableItem(GetMenuHandle(129), 3);
		EnableItem(GetMenuHandle(129), 4);
		
		DisableItem(GetMenuHandle(130), 3);
		DisableItem(GetMenuHandle(130), 4);
		DisableItem(GetMenuHandle(130), 5);
	}
	if(whichWindow == gCalcWindow)
	{
		EnableItem(GetMenuHandle(129), 1);
		EnableItem(GetMenuHandle(129), 3);
		DisableItem(GetMenuHandle(129), 4);

		SetItemCmd(GetMenuHandle(130), 4, 'C');
		SetItemMark(GetMenuHandle(130), 4, 0);
		EnableItem(GetMenuHandle(130), 3);
		EnableItem(GetMenuHandle(130), 4);
		EnableItem(GetMenuHandle(130), 5);
	}
	if(whichWindow == gCompareWindow)
	{
		EnableItem(GetMenuHandle(129), 1);
		EnableItem(GetMenuHandle(129), 3);
		EnableItem(GetMenuHandle(129), 4);
		
		SetItemCmd(GetMenuHandle(130), 4, 'C');
		SetItemMark(GetMenuHandle(130), 4, 0);
		DisableItem(GetMenuHandle(130), 3);
		DisableItem(GetMenuHandle(130), 4);
		DisableItem(GetMenuHandle(130), 5);
	}
	
	if(gList.Dirty)
	{
		EnableItem(GetMenuHandle(134), 4);
	}
	else
	{
		DisableItem(GetMenuHandle(134), 4);
	}
}

void CorrectControls()
{
	short		itemType;
	Rect		itemRect;
	Handle		itemHandle;

	if(gElementWindow != 0L)
	{
		if(FrontWindow() != gElementWindow || !gFrontApp)
		{
			GetDialogItem(gElementWindow, 2, &itemType, &itemHandle, &itemRect);
			gElementScroll.old = GetControlValue((ControlHandle)itemHandle);
			SetControlMaximum((ControlHandle)itemHandle, 0);
		}
		else
		{
			GetDialogItem(gElementWindow, 2, &itemType, &itemHandle, &itemRect);
			SetControlMaximum((ControlHandle)itemHandle,(gElementScroll.max-15)*(14-kScrollJump));
			if(gElementScroll.old != 0)
				SetControlValue((ControlHandle)itemHandle, gElementScroll.old);
		}
	}
	
	if(gCompareWindow != 0L)
	{
		if(FrontWindow() != gCompareWindow || !gFrontApp)
		{
			GetDialogItem(gCompareWindow, 1, &itemType, &itemHandle, &itemRect);
			gCompScroll.old = GetControlValue((ControlHandle)itemHandle);
			SetControlMaximum((ControlHandle)itemHandle, 0);
		}
		else
		{
			int	height;	
			height = gCompareWindow->portRect.bottom/13;
			
			if(gCompScroll.items > height)
			{
				gCompScroll.max = gCompScroll.items - height;
			}
			else
				gCompScroll.max = 0;
				
			GetDialogItem(gCompareWindow, 1, &itemType, &itemHandle, &itemRect);
			SetControlMaximum((ControlHandle)itemHandle,(gCompScroll.max)*(14-kScrollJump));
			if(gCompScroll.old != 0)
			{
				gCompScroll.current = gCompScroll.old;
				gCompScroll.old = 0;
			}
			SetControlValue((ControlHandle)itemHandle, gCompScroll.current);
		}
	}
	
	if(gCalcWindow != 0L)
	{
		if(FrontWindow() != gCalcWindow || !gFrontApp)
		{
			GetDialogItem(gCalcWindow, 1, &itemType, &itemHandle, &itemRect);
			HiliteControl((ControlHandle)itemHandle, kControlDisabledPart);
			/*GetDialogItem(gCalcWindow, 2, &itemType, &itemHandle, &itemRect);
			HiliteControl((ControlHandle)itemHandle, kControlDisabledPart);
			GetDialogItem(gCalcWindow, 3, &itemType, &itemHandle, &itemRect);
			HiliteControl((ControlHandle)itemHandle, kControlDisabledPart);
			GetDialogItem(gCalcWindow, 4, &itemType, &itemHandle, &itemRect);
			HiliteControl((ControlHandle)itemHandle, kControlDisabledPart);*/
		}
		else
		{
			GetDialogItem(gCalcWindow, 1, &itemType, &itemHandle, &itemRect);
			HiliteControl((ControlHandle)itemHandle, kControlNoPart);
			/*GetDialogItem(gCalcWindow, 2, &itemType, &itemHandle, &itemRect);
			HiliteControl((ControlHandle)itemHandle, kControlNoPart);
			GetDialogItem(gCalcWindow, 3, &itemType, &itemHandle, &itemRect);
			HiliteControl((ControlHandle)itemHandle, kControlNoPart);
			GetDialogItem(gCalcWindow, 4, &itemType, &itemHandle, &itemRect);
			HiliteControl((ControlHandle)itemHandle, kControlLabelPart);*/
		}
	}
}

#pragma mark -
#pragma mark Elements:
#pragma mark -

void Element(int e)
{	
	int		y;
	
	if(!gIsoTrue)
		y = kElemHeight;
	else
	{
		LoadIsotopes(e);
		y = kElemHeight + Lines()*13 +6;
	}
	SetRect(&gElementRect, 0, 0, kElemWidth, y);
	
	if(gElementWindow == 0L)
	{
		UseData();
		if((gElementWindow = GetNewDialog(130, 0L, (WindowRef)-1)) == 0L)
			HandleError(13);
		HNoPurge((Handle)gElementWindow);
		
		NewGWorld( &gElementWorld, 8, &gEWorldRect, 0L, 0L, 0L );
		if(gElementWorld == 0L)
			HandleError(14);
			
		if(ValidPos(gWindPos.ElementRect))
			MoveWindow(gElementWindow, gWindPos.ElementRect.left, gWindPos.ElementRect.top, 0);
	}
		
	LoadText(e);
	SetWTitle(gElementWindow, gNameText);
	
	LockPixels(GetGWorldPixMap(gElementWorld));
	SetGWorld((CGrafPtr)gElementWorld, 0L);
	
	ClearElement();
	DrawElemPict(e);	
	E3d(e);
	DrawElemText(e);
	
	gE = e;
	gEWinOpen = 1;

	if(gIsoTrue)
		Isotope(e);

	UnlockPixels(GetGWorldPixMap(gElementWorld));
	
	SetGWorld( (CGrafPtr)gElementWindow, 0L );
	
	SizeWindow(gElementWindow, gElementRect.right, gElementRect.bottom, true);
	ShowWindow(gElementWindow);
	
	CopyBitsElement();
		
	UpdateDialog(gElementWindow, gElementWindow->visRgn);
	DrawControls(gElementWindow);
	
	CorrectMenus();
}

void CopyBitsElement()
{
	Rect	theRect;
	int		y;
	
	if(gElementScroll.max > 15)
		y = kElemWidth - 15;
	else
		y = kElemWidth;
	
	DrawDialog(gElementWindow);
	
	SetRect(&theRect, 0, 0, kElemWidth, kElemHeight);
	CopyBits( &PORTBITMAP( gElementWorld ), &WINBITMAP( gElementWindow ),
		&theRect, &theRect, srcCopy, 0L);
	if(gElementWindow->portRect.bottom > kElemHeight + 6)
	{
		SetRect(&theRect, 0, kElemHeight+7, y, gElementWindow->portRect.bottom);
		CopyBits( &PORTBITMAP( gElementWorld ), &WINBITMAP( gElementWindow ),
			&theRect, &theRect, srcCopy, 0L);
	}
}

void E3d(int e)
{
	Rect 		the3dRect;
	PicHandle	the3dPict;

	UsePicts();
	if((the3dPict = GetPicture(k3dPictID + e)) == 0L)
	{
		if((the3dPict = GetPicture(1002)) == 0L)
			HandleError(15);
	}
	HNoPurge((Handle)the3dPict);
		
	SetRect(&the3dRect, 250, 0, 450, 200);
	DrawPicture(the3dPict, &the3dRect);
	HPurge((Handle)the3dPict);
}

void LoadText(int e)
{
	long	groupNum;

	UseData();
	GetIndString(gNameText, kResName, e);
	
	GetIndString(gGroupText, kResGroup, e);
	StringToNum(gGroupText, &groupNum);
	GetIndString(gGroupText, kResGroupName, groupNum);
	
	GetIndString(gEConfigText, kElecConfig, e);
	if(StrLength(gEConfigText))
		gEConfigValid = true;
	else
		gEConfigValid = false;
	
	GetIndString(gSymbText, kResSymbol, e);
	
	GetIndString(gWeightText, kResWeight, e);
	
	GetIndString(gRadiiText, kResRadii, e);
	if(StrLength(gRadiiText))
		gRadiiValid = true;
	else
		gRadiiValid = false;
		
	GetIndString(gDensityText, kResDensity, e);
	if(StrLength(gDensityText))
		gDensityValid = true;
	else
		gDensityValid = false;
		
	GetIndString(gMeltText, kResMelt, e);
	if(StrLength(gMeltText))
		gMeltValid = true;
	else
		gMeltValid = false;
	
	GetIndString(gBoilText, kResBoil, e);
	if(StrLength(gBoilText))
		gBoilValid = true;
	else
		gBoilValid = false;
	
	GetIndString(gSHCText, kResSHC, e);
	if(StrLength(gSHCText))
		gSHCValid = true;
	else
		gSHCValid = false;
		
	GetIndString(gOxiText, kResOxidation, e);
	if(StrLength(gOxiText))
		gOxiValid = true;
	else
		gOxiValid = false;
		
	GetIndString(gIonText, kResIonization, e);
	if(StrLength(gIonText))
		gIonValid = true;
	else
		gIonValid = 0;
		
	GetIndString(gNegText, kResNeg, e);
	if(StrLength(gNegText))
		gNegValid = true;
	else
		gNegValid = false;
		
	GetIndString(gDateText, kResDate, e);
	if(StrLength(gDateText))
		gDateValid = true;
	else
		gDateValid = false;
		
	LoadQShell(e);
}

void LoadQShell(int e)
{
	ShellHandle		shellHdl;
	
	UseData();
	shellHdl = (ShellHandle) Get1Resource('qshl',127+e);
	if(shellHdl != 0L)
	{
		gShell = (**shellHdl);
		gShellValid = 1;
	}
	else
		gShellValid = 0;
}

void ConvertStringCFK(Str255 String1)
{
	extended80			number;
	NumFormatStringRec	nfRec;
	Handle				itlHandle;
	long				offset, length;
	
	#ifdef powerc
		long double		x;
	#else
		extended80		x;
	#endif
	
	GetIntlResourceTable(smCurrentScript, smNumberPartsTable, &itlHandle, &offset, &length);
	StringToFormatRec("\p#,###,###.###;-#,###,###.###;0", (NumberParts *)(*itlHandle + offset), &nfRec);
	StringToExtended(String1, &nfRec, (NumberParts *)(*itlHandle + offset), &number);
	
	#ifdef powerc
		x80told(&number, &x);
	#else
		x = number;
	#endif
	
	if(gTempType.tempType == kFahrenheit)
	{	
		x = x * 9;
		x = x / 5;
		x = x + 32;
	}
	if(gTempType.tempType == kKelvin)
	{
		x = x + 273.15;
	}
	
	#ifdef powerc
		ldtox80(&x, &number);
	#else
		number = x;
	#endif
	
	GetIntlResourceTable(smCurrentScript, smNumberPartsTable, &itlHandle, &offset, &length);
	StringToFormatRec("\p#,###,###.###;-#,###,###.###;0", (NumberParts *)(*itlHandle + offset), &nfRec);
	ExtendedToString(&number, &nfRec, (NumberParts *)(*itlHandle + offset), String1);
}

void ConvertLowStringCFK(Str255 String1)
{
	extended80			number;
	NumFormatStringRec	nfRec;
	Handle				itlHandle;
	long				offset, length;
	
	#ifdef powerc
		long double		x;
	#else
		extended80		x;
	#endif
	
	GetIntlResourceTable(smCurrentScript, smNumberPartsTable, &itlHandle, &offset, &length);
	StringToFormatRec("\p#######.#;-#######.#;0", (NumberParts *)(*itlHandle + offset), &nfRec);
	StringToExtended(String1, &nfRec, (NumberParts *)(*itlHandle + offset), &number);
	
	#ifdef powerc
		x80told(&number, &x);
	#else
		x = number;
	#endif
	
	if(gTempType.tempType == kFahrenheit)
	{	
		x = x * 9;
		x = x / 5;
		x = x + 32;
	}
	if(gTempType.tempType == kKelvin)
	{
		x = x + 273.15;
	}
	
	#ifdef powerc
		ldtox80(&x, &number);
	#else
		number = x;
	#endif
	
	GetIntlResourceTable(smCurrentScript, smNumberPartsTable, &itlHandle, &offset, &length);
	StringToFormatRec("\p#######.#;-#######.#;0", (NumberParts *)(*itlHandle + offset), &nfRec);
	ExtendedToString(&number, &nfRec, (NumberParts *)(*itlHandle + offset), String1);
}

void DrawElemText(int e)
{
	Str255		pe;
	FontInfo	fontInfo;
	int			x, y, h, v;
	
	NumToString(e, pe);
	
	h = 70; v = 25;
	MoveTo(h, v);
	TextFont(times);
	TextFace(bold);
	TextSize(18);
	DrawString(gNameText);
	GetFontInfo(&fontInfo);
	SetRect(&gTextRects.Name, h-2, v-2 -(fontInfo.ascent+fontInfo.leading),
		h+2+StringWidth(gNameText), v+fontInfo.descent);
	x = h+StringWidth(gNameText);
		
	if(gDateValid)
	{
		TextFace(0);
		TextSize(12);	

		DrawString("\p, ");
		DrawString(gDateText);
		
		GetFontInfo(&fontInfo);
		SetRect(&gTextRects.Disc, x-2+StringWidth("\p, "), v-2 -(fontInfo.ascent+fontInfo.leading),
			x+2+StringWidth(gDateText)+StringWidth("\p, "), v +fontInfo.descent);
	}
	
	TextFace(0);
	TextSize(14);
	
	h = 70; v = 40;
	MoveTo(h, v);
	DrawString("\pAtomic Number: ");
	DrawString(pe);
	GetFontInfo(&fontInfo);
	SetRect(&gTextRects.Number, h-2, v -(fontInfo.ascent+fontInfo.leading),
		h+2+StringWidth("\pAtomic Number: ")+StringWidth(pe), v +fontInfo.descent);

	h = 70; v = 54;
	MoveTo(h, v);
	DrawString("\pAtomic Weight: ");
	DrawString(gWeightText);
	GetFontInfo(&fontInfo);
	SetRect(&gTextRects.Weight, h-2, v -(fontInfo.ascent+fontInfo.leading),
		h+2+StringWidth("\pAtomic Weight: ")+StringWidth(gWeightText), v +fontInfo.descent);
	
	TextFont(times);
	TextSize(10);
	
	y = 75;
	if(gEConfigValid)
	{
		h = 10; v = y;
		MoveTo(h, v);
		DrawString("\pElectron Configuration: ");
		if(gHighlight.enabled)
			RGBForeColor(&gHighlight.color);
		DrawString(gEConfigText);
		
		RGBForeColor(&Black);
		GetFontInfo(&fontInfo);
		SetRect(&gTextRects.ElecConfig, h-2, v -(fontInfo.ascent+fontInfo.leading),
			h+2+StringWidth("\pElectron Configuration: ")+StringWidth(gEConfigText), v+1+fontInfo.descent);
	}
	
	LewisDots(e, 30+StringWidth("\pElectron Configuration: ")+StringWidth(gEConfigText), y-4);
	
	y += 13;
	h = 10; v = y;
	MoveTo(h, v);
	DrawString("\pAtomic Radius: ");
	if(gHighlight.enabled)
		RGBForeColor(&gHighlight.color);
	if(gRadiiValid)
	{
		DrawString(gRadiiText);
		DrawString("\p nm");
	}
	else
		DrawString("\p --");
	RGBForeColor(&Black);
	GetFontInfo(&fontInfo);
	SetRect(&gTextRects.Radii, h-2, v -(fontInfo.ascent+fontInfo.leading),
		h+2+StringWidth("\pAtomic Radius:  nm")+StringWidth(gRadiiText), v+1+fontInfo.descent);
	
	y += 13;
	h = 10; v = y;
	MoveTo(h, v);
	DrawString("\pDensity: ");
	if(gHighlight.enabled)
		RGBForeColor(&gHighlight.color);
	if(gDensityValid)
	{
		DrawString(gDensityText);
		DrawString("\p g/cm");
		TextSize(9);
		Move(0, -3);
		DrawString("\p3");
		TextSize(10);
	}
	else
		DrawString("\p --");
	RGBForeColor(&Black);
	GetFontInfo(&fontInfo);
	SetRect(&gTextRects.Density, h-2, v -(fontInfo.ascent+fontInfo.leading),
		h+2+StringWidth("\pDensity:  g/cm3")+StringWidth(gDensityText), v+1+fontInfo.descent);
	
	/*y += 13;
	h = 10; v = y;
	MoveTo(h, v);
	DrawString("\pCrystal Structure: ");
	if(gHighlight.enabled)
		RGBForeColor(&gHighlight.color);
	if(true)
	{
		DrawString("\pCubic face centered");
	}
	else
		DrawString("\p  --");
	RGBForeColor(&Black);
	GetFontInfo(&fontInfo);
	SetRect(&gTextRects.Crystal, h-2, v -(fontInfo.ascent+fontInfo.leading),
		h+2+StringWidth("\pCrystal Structure: ")+StringWidth("\pCubic face centered"), v+1+fontInfo.descent);
	*/
#ifdef	GREY_LINES
	RGBForeColor(&Grey);
	MoveTo(h+3, y+4);
	LineTo(h+63, y+4);
	RGBForeColor(&Black);
#endif
	
	y += 17;
	h = 10; v = y;
	MoveTo(h, v);
	DrawString("\pMelting Point: ");
	
	if(gHighlight.enabled)
		RGBForeColor(&gHighlight.color);
	if(!gMeltValid)
			DrawString("\p --");
	else
	{
		ConvertStringCFK(gMeltText);
		DrawString(gMeltText);
		
		if(gTempType.tempType == kCelcius)
			DrawString("\p¡ C");
		if(gTempType.tempType == kFahrenheit)
			DrawString("\p¡ F");
		if(gTempType.tempType == kKelvin)
			DrawString("\p K");
	}
	RGBForeColor(&Black);
	GetFontInfo(&fontInfo);
	SetRect(&gTextRects.Melt, h-2, v -(fontInfo.ascent+fontInfo.leading),
		h+2+StringWidth("\pMelting Point:  K")+StringWidth(gMeltText), v+1+fontInfo.descent);
	
	y += 13;
	h = 10; v = y;
	MoveTo(h, v);
	DrawString("\pBoiling Point: ");
	
	if(gHighlight.enabled)
		RGBForeColor(&gHighlight.color);
	if(!gBoilValid)
		DrawString("\p --");
	else
	{
		ConvertStringCFK(gBoilText);
		DrawString(gBoilText);
		
		if(gTempType.tempType == kCelcius)
			DrawString("\p¡ C");
		if(gTempType.tempType == kFahrenheit)
			DrawString("\p¡ F");
		if(gTempType.tempType == kKelvin)
			DrawString("\p K");
	}
	RGBForeColor(&Black);
	GetFontInfo(&fontInfo);
	SetRect(&gTextRects.Boil, h-2, v -(fontInfo.ascent+fontInfo.leading),
		h+2+StringWidth("\pBoiling Point:  K")+StringWidth(gBoilText), v+1+fontInfo.descent);
	
#ifdef	GREY_LINES	
	RGBForeColor(&Grey);
	MoveTo(h+3, y+4);
	LineTo(h+63, y+4);
	RGBForeColor(&Black);
#endif
	
	y += 17;
	h = 10; v = y;
	MoveTo(h, v);
	DrawString("\pMajor Oxidation States: ");
	if(gHighlight.enabled)
		RGBForeColor(&gHighlight.color);
	if(gOxiValid)
		DrawString(gOxiText);
	else
		DrawString("\p  --");
	RGBForeColor(&Black);
	GetFontInfo(&fontInfo);
	SetRect(&gTextRects.Oxi, h-2, v -(fontInfo.ascent+fontInfo.leading),
		h+2+StringWidth("\pMajor Oxidation States: ")+StringWidth(gOxiText), v+1+fontInfo.descent);
	
	y += 13;
	h = 10; v = y;
	MoveTo(h, v);
	DrawString("\pElectronegativity: ");
	if(gHighlight.enabled)
		RGBForeColor(&gHighlight.color);
	if(gNegValid)
		DrawString(gNegText);
	else
		DrawString("\p  --");
	RGBForeColor(&Black);
	GetFontInfo(&fontInfo);
	SetRect(&gTextRects.Neg, 10-2, v -(fontInfo.ascent+fontInfo.leading),
		h+2+StringWidth("\pElectronegativity: ")+StringWidth(gNegText), v+1+fontInfo.descent);
	
	y += 13;
	h = 10; v = y;
	MoveTo(h, v);
	DrawString("\pSpecific Heat Capacity:  ");
	if(gHighlight.enabled)
		RGBForeColor(&gHighlight.color);
	if(gSHCValid)
	{
		DrawString(gSHCText);
		DrawString("\p J/g-C");
	}
	else
		DrawString("\p --");
	RGBForeColor(&Black);
	GetFontInfo(&fontInfo);
	SetRect(&gTextRects.SHC, h-2, v -(fontInfo.ascent+fontInfo.leading),
		h+2+StringWidth("\pSpecific Heat Capacity:   J/g-C")+StringWidth(gSHCText), v+1+fontInfo.descent);
	
	y += 13;
	h = 10; v = y;
	MoveTo(h, v);
	DrawString("\pFirst Ionization Energy: ");
	if(gHighlight.enabled)
		RGBForeColor(&gHighlight.color);
	if(gIonValid)
	{
		DrawString(gIonText);
		DrawString("\p kJ/mol");
	}
	else
		DrawString("\p  --");
	RGBForeColor(&Black);
	GetFontInfo(&fontInfo);
	SetRect(&gTextRects.Ion, h-2, v -(fontInfo.ascent+fontInfo.leading),
		h+2+StringWidth("\pFirst Ionization Energy:  kJ/mol")+StringWidth(gIonText), v+1+fontInfo.descent);
	
	MoveTo(20, 214);
	if(gShellValid)
	{
		LoadQShell(e);
		DrawQShells();
	}
}

void DrawQShells()
{
	Str255	string;
	
//	TextSize(10);
//	TextFont(courier);
//	TextFace(condense);
	if(gShell.q1s)
	{
		DrawString("\p1s");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q1s, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
	if(gShell.q2s)
	{
		DrawString("\p2s");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q2s, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
	if(gShell.q2p)
	{
		DrawString("\p2p");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q2p, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
	if(gShell.q3s)
	{
		DrawString("\p3s");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q3s, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
	if(gShell.q3p)
	{
		DrawString("\p3p");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q3p, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
	if(gShell.q4s)
	{
		DrawString("\p4s");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q4s, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
	if(gShell.q3d)
	{
		DrawString("\p3d");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q3d, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
	if(gShell.q4p)
	{
		DrawString("\p4p");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q4p, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
	if(gShell.q5s)
	{
		DrawString("\p5s");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q5s, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
	if(gShell.q4d)
	{
		DrawString("\p4d");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q4d, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
	if(gShell.q5p)
	{
		DrawString("\p5p");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q5p, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
	if(gShell.q6s)
	{
		DrawString("\p6s");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q6s, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
	if(gShell.q4f)
	{
		DrawString("\p4f");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q4f, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
	if(gShell.q5d)
	{
		DrawString("\p5d");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q5d, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
	if(gShell.q6p)
	{
		DrawString("\p6p");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q6p, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
	if(gShell.q7s)
	{
		DrawString("\p7s");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q7s, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
	if(gShell.q5f)
	{
		DrawString("\p5f");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q5f, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
	if(gShell.q6d)
	{
		DrawString("\p6d");
		Move(0, -3);
		TextSize(9);
		NumToString((int)gShell.q6d, string);
		DrawString(string);
		Move(2, 3);
		TextSize(10);
	}
}

void TestQShells()
{
	int	e, i;
	
	for(i = 1; i < 105; i++)
	{
		LoadQShell(i);
		e = gShell.q1s;
		e += gShell.q2s;
		e += gShell.q2p;
		e += gShell.q3s;
		e += gShell.q3p;
		e += gShell.q3d;
		e += gShell.q4s;
		e += gShell.q4p;
		e += gShell.q4f;
		e += gShell.q5s;
		e += gShell.q5p;
		e += gShell.q5f;
		e += gShell.q6s;
		e += gShell.q6p;
		e += gShell.q6d;
		e += gShell.q6f;
		e += gShell.q7s;
		
		if(e != i)
			SysBeep(1);
	}
}

void LewisDots(int e, int h, int v)
{
	FontInfo	fontInfo;
	Point		Dots[9];
	int			lastShell, ShellS, ShellP, Side, Part, vSide, vPart;
	
	GetFontInfo(&fontInfo);
	Side = StringWidth(gSymbText)/2;
	Part = 1;
	vSide = (fontInfo.ascent+fontInfo.leading+fontInfo.descent)/2;
	vPart = 1;
	
	MoveTo(h - StringWidth(gSymbText)/2, v + (fontInfo.ascent+fontInfo.leading)/2 - 1);
	DrawString(gSymbText);
	
	SetPt(&Dots[1], kRight*Side + 2, kTop*vPart - 2);
	SetPt(&Dots[2], kRight*Side + 2, kBottom*vPart);
	SetPt(&Dots[3], kRight*Part, kBottom*vSide);
	SetPt(&Dots[4], kLeft*Side - 4, kBottom*vPart);
	SetPt(&Dots[5], kLeft*Part - 2, kTop*vSide - 2);
	SetPt(&Dots[6], kLeft*Part - 2, kBottom*vSide);
	SetPt(&Dots[7], kLeft*Side - 4, kTop*vPart - 2);
	SetPt(&Dots[8], kRight*Part, kTop*vSide - 2);
	
	LoadQShell(e);
	
	if(gShell.q1s > 0)
	{
		lastShell = 1;
		ShellS = gShell.q1s;
		ShellP = 0;
	}
	if(gShell.q2s > 0)
	{
		lastShell = 2;
		ShellS = gShell.q2s;
		ShellP = gShell.q2p;
	}
	if(gShell.q3s > 0)
	{
		lastShell = 3;
		ShellS = gShell.q3s;
		ShellP = gShell.q3p;
	}
	if(gShell.q4s > 0)
	{
		lastShell = 4;
		ShellS = gShell.q4s;
		ShellP = gShell.q4p;
	}
	if(gShell.q5s > 0)
	{
		lastShell = 5;
		ShellS = gShell.q5s;
		ShellP = gShell.q5p;
	}
	if(gShell.q6s > 0)
	{
		lastShell = 6;
		ShellS = gShell.q6s;
		ShellP = gShell.q6p;
	}
	if(gShell.q7s > 0)
	{
		lastShell = 7;
		ShellS = gShell.q7s;
		ShellP = 0;
	}
	
	if(e == 46 || !gShellValid)
		ShellS = ShellP = 0;
	
	if(ShellS > 0)
		Dot(h + Dots[1].h, v + Dots[1].v);		// 1
	if(ShellS > 1)
		Dot(h + Dots[2].h, v + Dots[2].v);		// 2
	if(ShellP > 0)
		Dot(h + Dots[3].h, v + Dots[3].v);		// 3
	if(ShellP > 1)
		Dot(h + Dots[4].h, v + Dots[4].v);		// 4
	if(ShellP > 2)
		Dot(h + Dots[5].h, v + Dots[5].v);		// 5
	if(ShellP > 3)
		Dot(h + Dots[6].h, v + Dots[6].v);		// 6
	if(ShellP > 4)
		Dot(h + Dots[7].h, v + Dots[7].v);		// 7
	if(ShellP > 5)
		Dot(h + Dots[8].h, v + Dots[8].v);		// 8
}

void Dot(int h, int v)
{
	Rect	rect;
	
	RGBForeColor(&Black);
	SetRect(&rect, h, v, h+2, v+2);
	PaintOval(&rect);
}

void DrawElemPict(int e)
{
	Rect		theRect;
	PicHandle	thePicture;
	
	UsePicts();
	if((thePicture = GetPicture(kElemPictID + e)) == 0L)
		HandleError(16);
	HNoPurge((Handle)thePicture);
	
	SetRect(&theRect, 10, 10, 60, 60);
	DrawPicture(thePicture, &theRect);
	HPurge((Handle)thePicture);
}

void ClearElement()
{
	RGBColor	oldColor;
	Rect		theRect;

	GetBackColor(&oldColor);	
	RGBBackColor(&White);
	
	SetRect(&theRect, 0, 0, kElemWidth, kElemHeight + 6);
	EraseRect(&theRect);
	
	RGBBackColor(&oldColor);
}

#pragma mark -
#pragma mark Isotopes:
#pragma mark -

pascal void ScrollProc(ControlHandle theControl, short theCode)
{
	int	curVal, maxVal, minVal;
	
	maxVal = GetControlMaximum((ControlHandle)theControl);
	curVal = GetControlValue((ControlHandle)theControl);
	minVal = GetControlMinimum((ControlHandle)theControl);
	
	switch(theCode)
	{
		case kControlUpButtonPart:
		case kControlPageUpPart:
			if(curVal > minVal+(14-1))
				curVal = curVal - (14-1);
			else
				curVal = minVal;
			break;
		
			/*if(curVal > minVal)
				--curVal;
			break;*/
		case kControlDownButtonPart:
		case kControlPageDownPart:
			if(curVal+(14-1) < maxVal)
				curVal = curVal + (14-1);
			else
				curVal = maxVal;
			break;
		
			/*if(curVal < maxVal)
				++curVal;
			else
				curVal = maxVal;
			break;*/
	}
	
	SetControlValue((ControlHandle)theControl, curVal);
	
	
	if(FrontWindow() == gElementWindow)
	{
		gElementScroll.current = curVal;
		IsotopeScrolled();
	}
	if(FrontWindow() == gCompareWindow)
	{
		gCompScroll.current = curVal;
		UpdateCompare();
	}

}

pascal void LiveScrollProc(ControlHandle theControl, short theCode)
{
	int	curVal, maxVal, minVal;
	
	maxVal = GetControlMaximum((ControlHandle)theControl);
	curVal = GetControlValue((ControlHandle)theControl);
	minVal = GetControlMinimum((ControlHandle)theControl);
	
	switch(theCode)
	{
		case kControlPageUpPart:
			if(curVal > minVal + 1)
				curVal = curVal - 1;
			else
				curVal = minVal;
			break;
		case kControlUpButtonPart:
			if(curVal > minVal)
				--curVal;
			break;
		case kControlPageDownPart:
			if(curVal+1 < maxVal)
				curVal = curVal + 1;
			else
				curVal = maxVal;
			break;
		case kControlDownButtonPart:
			if(curVal < maxVal)
				++curVal;
			else
				curVal = maxVal;
			break;
	}
	
	SetControlValue((ControlHandle)theControl, curVal);
	
	
	if(FrontWindow() == gElementWindow)
	{
		gElementScroll.current = curVal;
		IsotopeScrolled();
	}
	if(FrontWindow() == gCompareWindow)
	{
		gCompScroll.current = curVal;
		UpdateCompare();
	}

}

void Isotope(int e)
{
	Handle			controlHdl;
	short			itemType, x;
	Rect			itemRect;
	Handle			itemHandle;
	RGBColor		oldColor;

	UseData();
	if(gIsotopeWorld == 0L)
	{
		SetRect(&gIsoWorldRect, 0, 0, kElemWidth, 13*kMaxIsotopes);
		NewGWorld( &gIsotopeWorld, 0, &gIsoWorldRect, 0L, 0L, 0L );
		if(gIsotopeWorld == 0L)
			HandleError(17);
		
		gIWorldOpen = 1;
	}
	GetDialogItem(gElementWindow, 2, &itemType, &itemHandle, &itemRect);
	LoadIsotopes(e);
	
	if(gElementScroll.max>15)
		x = kElemWidth - 15;
	else
		x = kElemWidth;	
	SetRect(&gIsoRect, 0, kElemHeight +6, x, kElemHeight + Lines()*13 +6);
	SetRect(&gIsoWorldRect, 0, 0, x, Lines()*13);

	CorrectControls();
	
	gElementScroll.current = 0;
	
	LockPixels(GetGWorldPixMap(gIsotopeWorld));
	SetGWorld((CGrafPtr)gIsotopeWorld, 0L);
	
	GetBackColor(&oldColor);
	RGBBackColor(&Grey);
	EraseRect(&gIsotopeWorld->portRect);
	RGBBackColor(&oldColor);
	
	DrawIsotopeText(e, 5, 10);
	
	SetGWorld((CGrafPtr)gElementWorld, 0L);
	CopyBits( &PORTBITMAP( gIsotopeWorld ), &PORTBITMAP( gElementWorld ),
		&gIsoWorldRect, &gIsoRect, srcCopy, 0L );
	
	SetGWorld((CGrafPtr)gElementWindow, 0L);
	SizeWindow(gElementWindow, kElemWidth, kElemHeight + Lines()*13 +6,true);
	CopyBitsElement();
		
	UnlockPixels(GetGWorldPixMap(gIsotopeWorld));
	UnlockPixels(GetGWorldPixMap(gElementWorld));
	GetDialogItem(gElementWindow, 2, &itemType, &controlHdl, &itemRect);
	
	if(gElementScroll.max <= 15)
		HideControl((ControlHandle)controlHdl);
	else if(gElementScroll.max > 15)
	{
		SetControlMinimum((ControlHandle)controlHdl, 0);
		SetControlMaximum((ControlHandle)controlHdl, (gElementScroll.max-15)*(14-kScrollJump));
		
		ShowControl((ControlHandle)controlHdl);
	}
}

void ClearIsotope()
{
	RGBColor	oldColor;
	int			x;

	GetBackColor(&oldColor);	
	RGBBackColor(&Grey);
	
	if(gElementScroll.max>15)
		x = kElemWidth - 15;
	else
		x = kElemWidth;
		
	SetRect(&gIsoRect, 0, kElemHeight +6, x, kElemHeight + Lines()*13 +6);
	EraseRect(&gIsoRect);
	
	RGBBackColor(&oldColor);
}

void IsotopeScrolled()
{
	Rect	isoWorldRect, theRect;
	int		y;
	isoWorldRect = gIsoWorldRect;
	
	isoWorldRect.top = isoWorldRect.top + (gElementScroll.current*kScrollJump);
	isoWorldRect.bottom = isoWorldRect.bottom + (gElementScroll.current*kScrollJump);
	
	LockPixels(GetGWorldPixMap(gIsotopeWorld));
	SetGWorld((CGrafPtr)gIsotopeWorld, 0L);
	
	SetGWorld( (CGrafPtr)gElementWindow, 0L );
		
	CopyBits( &PORTBITMAP( gIsotopeWorld ), &PORTBITMAP( gElementWorld ),
		&isoWorldRect, &gIsoRect, srcCopy, 0L );
	
	if(gElementScroll.max > 15)
		y = kElemWidth - 15;
	else
		y = kElemWidth;
	
	SetRect(&theRect, 0, kElemHeight+7, y, gElementWindow->portRect.bottom);
	CopyBits( &PORTBITMAP( gElementWorld ), &WINBITMAP( gElementWindow ),
		&theRect, &theRect, srcCopy, 0L);
}

int Lines()
{
	if(gElementScroll.max > 15)
		return(15);
	else if(!gElementScroll.max)
		return(1);
	else
		return(gElementScroll.max);
}

void LoadIsotopes(int e)
{
	int		i, x, y;
	long	IsoStrt;
	
	if(e > 80)
	{
		x = 3;
		y = e-80;
	}
	else if(e > 49)
	{
		x = 2;
		y = e-49;
	}
	else
	{
		x = 1;
		y = e;
	}
	
	UseIsotopes(x);

	GetIndString(gIsoTxt, 135, y*2-1);
	GetIndString(gIsoStrtTxt, 135, y*2);
	
	StringToNum(gIsoTxt, &gElementScroll.max);
	StringToNum(gIsoStrtTxt, &IsoStrt);
	
	for(i=IsoStrt; i < gElementScroll.max+IsoStrt; i++)
	{
		DWeightx[i-IsoStrt+1] = 1;
		GetIndString(gDWeight[i-IsoStrt+1], 132, i);
		if(IUCompString(gDWeight[i-IsoStrt+1], "\p") == 0)
			DWeightx[i-IsoStrt+1] = 0;
		if(IUCompString(gDWeight[i-IsoStrt+1], "\p~") == 0)
			DWeightx[i-IsoStrt+1] = 0;
		
		DAbundx[i-IsoStrt+1] = 1;
		GetIndString(gDAbund[i-IsoStrt+1], 133, i);
		if(IUCompString(gDAbund[i-IsoStrt+1], "\p") == 0)
			DAbundx[i-IsoStrt+1] = 0;
		if(IUCompString(gDAbund[i-IsoStrt+1], "\p~") == 0)
			DAbundx[i-IsoStrt+1] = 0;
		
		DHalfLx[i-IsoStrt+1] = 1;
		GetIndString(gDHalfL[i-IsoStrt+1], 134, i);
		if(IUCompString(gDHalfL[i-IsoStrt+1], "\p") == 0)
			DHalfLx[i-IsoStrt+1] = 0;
		if(IUCompString(gDHalfL[i-IsoStrt+1], "\p~") == 0)
			DHalfLx[i-IsoStrt+1] = 0;
		
		DIsoWx[i-IsoStrt+1] = 1;
		GetIndString(gDIsoW[i-IsoStrt+1], 136, i);
		if(IUCompString(gDIsoW[i-IsoStrt+1], "\p") == 0)
			DIsoWx[i-IsoStrt+1] = 0;
		if(IUCompString(gDIsoW[i-IsoStrt+1], "\p~") == 0)
			DIsoWx[i-IsoStrt+1] = 0;
	}
}

void DrawIsotopeText(int e, int x, int y)
{
	int line = 13;
	int	i;
		
	TextFont(helvetica);
	TextFace(0);
	TextSize(10);
	
	MoveTo(x, y);

	for(i=1; i<=gElementScroll.max; i++)
	{
		TextFace(bold);
		DrawString(gSymbText);
		TextFace(0);
		DrawString("\p ");
		TextSize(9);
		DrawString(gDIsoW[i]);
		TextSize(10);
		DrawString("\p -  ");
		
		MoveTo(x+70, y+(i*line)-line);
		DrawString("\pWeight: ");
		if(DWeightx[i])
			DrawString(gDWeight[i]);
		
		
		MoveTo(x+165, y+(i*line)-line);
		DrawString("\pAbundance: ");
		if(DAbundx[i])
		{
			DrawString(gDAbund[i]);
			DrawString("\p%");
		}
		
		MoveTo(x+290, y+(i*line)-line);
		DrawString("\pHalf Life: ");
		if(DHalfLx[i])
			DrawString(gDHalfL[i]);
		
		MoveTo(x, y+(i*line));
	}
	if(!gElementScroll.max)
	{
		TextSize(10);
		TextFace(bold);
		DrawString("\pNo known isotopes");
	}
	
	RGBForeColor(&White);
	if(gElementScroll.max > 0)
	{
		for(i=1; i <= gElementScroll.max; i++)
		{
			MoveTo(0, (i-1)*13);
			LineTo(kElemWidth, (i-1)*13);
		}
		MoveTo(160, 0);
		LineTo(160, gElementScroll.max*13);
		MoveTo(285, 0);
		LineTo(285, gElementScroll.max*13);
	}
	RGBForeColor(&Black);
}

#pragma mark -
#pragma mark Preferences:
#pragma mark -

void PreferencesDialog()
{
	EventRecord		Event;
	Point			where;
	Handle			controlHdl;
	SInt16 			itemHit;
	short			itemType;
	Rect			itemRect;
	
	UseData();
	
	if(!(gPrefsDialog = GetNewDialog(150,0L,(WindowPtr) -1)))
		HandleError(22);
	
	SetDialogDefaultItem(gPrefsDialog, 1);								// OK
	SetDialogCancelItem(gPrefsDialog, 2);								// Cancel
	SetDialogTracksCursor(gPrefsDialog,true);
 
 	GetDialogItem(gPrefsDialog, 4, &itemType, &controlHdl, &itemRect);	// Celcius
	SetControlValue((ControlHandle)controlHdl, 0);
	
	GetDialogItem(gPrefsDialog, 5, &itemType, &controlHdl, &itemRect);	// Fahrenheit
	SetControlValue((ControlHandle)controlHdl, 0);
	
	GetDialogItem(gPrefsDialog, 6, &itemType, &controlHdl, &itemRect);	// Kelvin
	SetControlValue((ControlHandle)controlHdl, 0);
	
	GetDialogItem(gPrefsDialog, 7, &itemType, &controlHdl, &itemRect);	// Highlight bool
	SetControlValue((ControlHandle)controlHdl, gHighlight.enabled);
	
	GetDialogItem(gPrefsDialog, 9, &itemType, &controlHdl, &itemRect);	// Classic labels
	SetControlValue((ControlHandle)controlHdl, gTablePrefs.classic);

	GetDialogItem(gPrefsDialog, 10, &itemType, &controlHdl, &itemRect);	// Copy labels
	SetControlValue((ControlHandle)controlHdl, gCopyLabels);

	GetDialogItem(gPrefsDialog, 4+gTempType.tempType, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, 1);
	
	ShowWindow(gPrefsDialog);
	
	do
	{
		ModalDialog(gDialogUPP, &itemHit);
		
		WaitNextEvent(everyEvent, &Event, 0L, 0L);
		
		if( IsDialogEvent(&Event) )
		{
			switch(itemHit)
			{
				case 4:
					GetDialogItem(gPrefsDialog, 4, &itemType, &controlHdl, &itemRect);
					SetControlValue((ControlHandle)controlHdl, 1);
					
					GetDialogItem(gPrefsDialog, 5, &itemType, &controlHdl, &itemRect);
					SetControlValue((ControlHandle)controlHdl, 0);
					
					GetDialogItem(gPrefsDialog, 6, &itemType, &controlHdl, &itemRect);
					SetControlValue((ControlHandle)controlHdl, 0);
					break;
				case 5:
					GetDialogItem(gPrefsDialog, 4, &itemType, &controlHdl, &itemRect);
					SetControlValue((ControlHandle)controlHdl, 0);
					
					GetDialogItem(gPrefsDialog, 5, &itemType, &controlHdl, &itemRect);
					SetControlValue((ControlHandle)controlHdl, 1);
					
					GetDialogItem(gPrefsDialog, 6, &itemType, &controlHdl, &itemRect);
					SetControlValue((ControlHandle)controlHdl, 0);
					break;
				case 6:
					GetDialogItem(gPrefsDialog, 4, &itemType, &controlHdl, &itemRect);
					SetControlValue((ControlHandle)controlHdl, 0);
					
					GetDialogItem(gPrefsDialog, 5, &itemType, &controlHdl, &itemRect);
					SetControlValue((ControlHandle)controlHdl, 0);
					
					GetDialogItem(gPrefsDialog, 6, &itemType, &controlHdl, &itemRect);
					SetControlValue((ControlHandle)controlHdl, 1);
					break;
				case 7:
				case 9:
				case 10:
					GetDialogItem(gPrefsDialog, itemHit, &itemType, &controlHdl, &itemRect);
					SetControlValue((ControlHandle)controlHdl,!GetControlValue((ControlHandle)controlHdl));
					break;
				case 8:
					where.h =  qd.screenBits.bounds.right/2-500/2;
					where.v =  qd.screenBits.bounds.bottom/2-300/2;
					GetColor(where,"\pChoose a new hilite color...",&gHighlight.color,&gHighlight.color);
					break;
			}
		}
	} while((itemHit != 1) && (itemHit != 2));
	
	if(itemHit == 1)
	{
		GetDialogItem(gPrefsDialog, 4, &itemType, &controlHdl, &itemRect);
		if(GetControlValue((ControlHandle)controlHdl))
			gTempType.tempType = kCelcius;
		
		GetDialogItem(gPrefsDialog, 5, &itemType, &controlHdl, &itemRect);	
		if(GetControlValue((ControlHandle)controlHdl))
			gTempType.tempType = kFahrenheit;
		
		GetDialogItem(gPrefsDialog, 6, &itemType, &controlHdl, &itemRect);	
		if(GetControlValue((ControlHandle)controlHdl))
			gTempType.tempType = kKelvin;
			
		GetDialogItem(gPrefsDialog, 7, &itemType, &controlHdl, &itemRect);	
		gHighlight.enabled = GetControlValue((ControlHandle)controlHdl);
		
		GetDialogItem(gPrefsDialog, 9, &itemType, &controlHdl, &itemRect);	
		gTablePrefs.classic = GetControlValue((ControlHandle)controlHdl);
		
		GetDialogItem(gPrefsDialog, 10, &itemType, &controlHdl, &itemRect);	
		gCopyLabels = GetControlValue((ControlHandle)controlHdl);
					
		SavePreferences();
	}

	DisposeDialog(gPrefsDialog);
}

void GetPrefs()
{
	OSErr				err;
	Str255				fileName = "\pSodium Preferences";
	SInt16				volRN;
	long				dirID;
	FSSpec 				fileSSpec;
	SInt16				fileRN;
	
	
	err = FindFolder(kOnSystemDisk,kPreferencesFolderType,kDontCreateFolder,&volRN, &dirID);
	
	if(err == noErr)
		err = FSMakeFSSpec(volRN, dirID, fileName, &fileSSpec);
	if(err == noErr || err == fnfErr)
		fileRN = FSpOpenResFile(&fileSSpec, fsCurPerm);
	
	if(fileRN == -1)
	{
		FSpCreateResFile(&fileSSpec, 'Na12', 'pref', smSystemScript);
		err = ResError();
		
		if(err == wPrErr)
		{
			fileRN = gAppFRN;
			gSave = 0;
		}
		if(err == noErr)
		{
			fileRN = FSpOpenResFile(&fileSSpec, fsCurPerm);
			if(fileRN != -1)
			{
				UseResFile(gAppFRN);
				
				err = CopyRes('Temp', 128, gAppFRN, fileRN);
				
				if(err == noErr)
					err = CopyRes('Hlit', 128, gAppFRN, fileRN);
					
				if(err == noErr)
					err = CopyRes('Graf', 128, gAppFRN, fileRN);
					
				if(err == noErr)
					err = CopyRes('Rect', 128, gAppFRN, fileRN);
				if(err == noErr)
					err = CopyRes('Rect', 129, gAppFRN, fileRN);
					
				if(err == noErr)
					err = CopyRes('Prnt', 128, gAppFRN, fileRN);
				
				if(err == noErr)
					err = CopyRes('Tabl', 128, gAppFRN, fileRN);
					
				if(err == noErr)
					err = CopyRes('Size', 128, gAppFRN, fileRN);
					
				if(err == noErr)
					err = CopyRes('List', 128, gAppFRN, fileRN);
					
				if(err == noErr)
					err = CopyRes('CpLb', 128, gAppFRN, fileRN);
					
				if(err == noErr)
					err = CopyRes('STR ', kAppMissID,gAppFRN,fileRN);
					
				if(err != noErr)
				{
					CloseResFile(fileRN);
					err = FSpDelete(&fileSSpec);
					fileRN = -1;
					gSave = 0;
				}
			}
		}
	}
	if(fileRN != -1)
	{
		UseResFile(fileRN);
		
		err = GetTempPrefs(fileRN);
		if(err == 0)
			GetHilitePrefs(fileRN);
		if(err == 0)
			GetGraphPrefs(fileRN);
		if(err == 0)
			GetWindPosPrefs(fileRN);
		if(err == 0)
			GetTablePrefs(fileRN);
		if(err == 0)
			GetWindSizePrefs(fileRN);
		if(err == 0)
			GetPrintingPrefs(fileRN);
		if(err == 0)
			GetListPrefs(fileRN);
		if(err == 0)
			GetCopyLabelsPrefs(fileRN);

		gPrefsFRN = fileRN;

		UseResFile(gAppFRN);
	}
}

short CopyRes(ResType resType, int resID, int sourceFileRefNum, int destFileRefNum)
{
	SInt16	oldResFileRefNum;
	Handle	sourceResourceHdl;
	ResType	ignoredType;
	SInt16	ignoredID;
	Str255 	resourceName;
	SInt16	resAttributes;
	OSErr	osError;

	oldResFileRefNum = CurResFile();
	UseResFile(sourceFileRefNum);

	sourceResourceHdl = Get1Resource(resType,resID);

	if(sourceResourceHdl != 0L)
	{
		GetResInfo(sourceResourceHdl,&ignoredID,&ignoredType,resourceName);
		resAttributes = GetResAttrs(sourceResourceHdl);
		DetachResource(sourceResourceHdl);
		UseResFile(destFileRefNum);
		if(ResError() == noErr)
			AddResource(sourceResourceHdl,resType,resID,resourceName);
		if(ResError() == noErr)
			SetResAttrs(sourceResourceHdl,resAttributes);
		if(ResError() == noErr)
			ChangedResource(sourceResourceHdl);
		if(ResError() == noErr)
			WriteResource(sourceResourceHdl);
	}

	osError = ResError();

	ReleaseResource(sourceResourceHdl);
	UseResFile(oldResFileRefNum);

	return(osError);
}

void SavePreferences()
{
	if(gPrefsFRN == -1 || !gSave)
		return;

	SaveTempPrefs();
	SaveHilitePrefs();
	SaveGraphPrefs();
	SaveWindPosPrefs();
	SaveTablePrefs();
	SaveWindSizePrefs();
	SavePrintingPrefs();
	SaveListPrefs();
	SaveCopyLabelsPrefs();

	UseResFile(gAppFRN);
}

#pragma mark -
#pragma mark Graph:
#pragma mark -

void GraphDialog()
{
	EventRecord		Event;
	Point			where;
	Handle			controlHdl;
	SInt16 			itemHit;
	short			itemType;
	Rect			itemRect;
	
	UseData();
	
	if(!(gGraphDialog = GetNewDialog(131,0L,(WindowPtr) -1)))
		HandleError(20);
	
	SetDialogDefaultItem(gGraphDialog, 1);
	SetDialogCancelItem(gGraphDialog, 2);
	SetDialogTracksCursor(gGraphDialog,true);
 
 	GetDialogItem(gGraphDialog, 3, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gGraph.Weight);
	
	GetDialogItem(gGraphDialog, 4, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gGraph.Radii);
	
	GetDialogItem(gGraphDialog, 5, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gGraph.Melt);
	
	GetDialogItem(gGraphDialog, 6, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gGraph.Boil);
		
	GetDialogItem(gGraphDialog, 7, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gGraph.Ionization);
	
	GetDialogItem(gGraphDialog, 8, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gGraph.Negativity);
	
	ShowWindow(gGraphDialog);
	
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
				case 9:
					where.h =  qd.screenBits.bounds.right/2-500/2;
					where.v =  qd.screenBits.bounds.bottom/2-300/2;
					GetColor(where,"\pChoose a new color...",&gGraph.WeightColor,&gGraph.WeightColor);
					break;
				case 10:
					where.h =  qd.screenBits.bounds.right/2-500/2;
					where.v =  qd.screenBits.bounds.bottom/2-300/2;
					GetColor(where,"\pChoose a new color...",&gGraph.RadiiColor,&gGraph.RadiiColor);
					break;
				case 11:
					where.h =  qd.screenBits.bounds.right/2-500/2;
					where.v =  qd.screenBits.bounds.bottom/2-300/2;
					GetColor(where,"\pChoose a new color...",&gGraph.MeltColor,&gGraph.MeltColor);
					break;
				case 12:
					where.h =  qd.screenBits.bounds.right/2-500/2;
					where.v =  qd.screenBits.bounds.bottom/2-300/2;
					GetColor(where,"\pChoose a new color...",&gGraph.BoilColor,&gGraph.BoilColor);
					break;
				case 13:
					where.h =  qd.screenBits.bounds.right/2-500/2;
					where.v =  qd.screenBits.bounds.bottom/2-300/2;
					GetColor(where,"\pChoose a new color...",&gGraph.IonizationColor,&gGraph.IonizationColor);
					break;
				case 14:
					where.h =  qd.screenBits.bounds.right/2-500/2;
					where.v =  qd.screenBits.bounds.bottom/2-300/2;
					GetColor(where,"\pChoose a new color...",&gGraph.NegativityColor,&gGraph.NegativityColor);
					break;
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
					GetDialogItem(gGraphDialog, itemHit, &itemType, &controlHdl, &itemRect);
					SetControlValue((ControlHandle)controlHdl,!GetControlValue((ControlHandle)controlHdl));
					break;
			}
		}
	}while((itemHit != 1) && (itemHit != 2));
	
	
	
	if(itemHit == 1)
	{
		GetDialogItem(gGraphDialog, 3, &itemType, &controlHdl, &itemRect);	
		gGraph.Weight = GetControlValue((ControlHandle)controlHdl);
		GetDialogItem(gGraphDialog, 4, &itemType, &controlHdl, &itemRect);	
		gGraph.Radii = GetControlValue((ControlHandle)controlHdl);
		GetDialogItem(gGraphDialog, 5, &itemType, &controlHdl, &itemRect);	
		gGraph.Melt = GetControlValue((ControlHandle)controlHdl);
		GetDialogItem(gGraphDialog, 6, &itemType, &controlHdl, &itemRect);	
		gGraph.Boil = GetControlValue((ControlHandle)controlHdl);
		GetDialogItem(gGraphDialog, 7, &itemType, &controlHdl, &itemRect);	
		gGraph.Ionization = GetControlValue((ControlHandle)controlHdl);
		GetDialogItem(gGraphDialog, 8, &itemType, &controlHdl, &itemRect);	
		gGraph.Negativity = GetControlValue((ControlHandle)controlHdl);
		
		Graph();
		
		SelectWindow(gGraphWindow);
		
		CorrectMenus();
		CorrectControls();
	}
	DisposeDialog(gGraphDialog);
}

void Graph()
{
	int			keyCount;
	Rect		keyBox;
	keyCount	= 7;
		
	UseData();
	
	if(gGraphWindow == 0L)
	{
		gGraphWindow = GetNewCWindow(130, 0L, (WindowRef)-1);
	
		if(gGraphWindow == 0L)
			HandleError(18);

		HNoPurge((Handle)gGraphWindow);
		
		NewGWorld( &gGraphWorld, 8, &gGraphWindow->portRect, 0L, 0L, 0L );
		if(gGraphWorld == 0L)
			HandleError(19);
		
		if(ValidPos(gWindPos.GraphRect))
			MoveWindow(gGraphWindow, gWindPos.GraphRect.left, gWindPos.GraphRect.top, 0);
	}
	gGraphOpen = 1;
	
	LockPixels(GetGWorldPixMap(gGraphWorld));
	SetGWorld((CGrafPtr)gGraphWorld, 0L);
	
	RGBBackColor(&Grey);
	EraseRect(&gGraphWindow->portRect);
	PenSize(1,1);	
			
	RGBForeColor(&White);
	MoveTo(1*kGraphHScale, 20);
	LineTo(1*kGraphHScale, 270);
	MoveTo(9*kGraphHScale, 20);
	LineTo(9*kGraphHScale, 270);
	MoveTo(17*kGraphHScale, 20);
	LineTo(17*kGraphHScale, 270);
	MoveTo(35*kGraphHScale, 20);
	LineTo(35*kGraphHScale, 270);
	MoveTo(53*kGraphHScale, 20);
	LineTo(53*kGraphHScale, 270);
	MoveTo(85*kGraphHScale, 20);
	LineTo(85*kGraphHScale, 270);
	RGBForeColor(&Black);
	
	TextFont(times);
	TextSize(12);
	TextFace(normal);
	
	SetCursor(*GetCursor(watchCursor));

	if(gGraph.Melt)
	{
		GraphLine(kResMelt, gGraph.MeltColor);
		SetRect(&keyBox, 410, 15*keyCount - 5, 420, 15*keyCount + 5);
		RGBForeColor(&gGraph.MeltColor);
		PaintRect(&keyBox);
		RGBForeColor(&Black);
		MoveTo(425, 15*keyCount + 5);
		DrawString("\pMelt ¡");
		keyCount--;
	}
	if(gGraph.Boil)
	{
		GraphLine(kResBoil, gGraph.BoilColor);
		SetRect(&keyBox, 410, 15*keyCount - 5, 420, 15*keyCount + 5);
		RGBForeColor(&gGraph.BoilColor);
		PaintRect(&keyBox);
		RGBForeColor(&Black);
		MoveTo(425, 15*keyCount + 5);
		DrawString("\pBoil ¡");
		keyCount--;
	}
	if(gGraph.Ionization)
	{
		GraphLine(kResIonization, gGraph.IonizationColor);
		SetRect(&keyBox, 410, 15*keyCount - 5, 420, 15*keyCount + 5);
		RGBForeColor(&gGraph.IonizationColor);
		PaintRect(&keyBox);
		RGBForeColor(&Black);
		MoveTo(425, 15*keyCount + 5);
		DrawString("\pIonization");
		keyCount--;
	}
	if(gGraph.Negativity)
	{
		GraphLine(kResNeg, gGraph.NegativityColor);
		SetRect(&keyBox, 410, 15*keyCount - 5, 420, 15*keyCount + 5);
		RGBForeColor(&gGraph.NegativityColor);
		PaintRect(&keyBox);
		RGBForeColor(&Black);
		MoveTo(425, 15*keyCount + 5);
		DrawString("\pNegativity");
		keyCount--;
	}
	if(gGraph.Radii)
	{
		GraphLine(kResRadii, gGraph.RadiiColor);
		SetRect(&keyBox, 410, 15*keyCount - 5, 420, 15*keyCount + 5);
		RGBForeColor(&gGraph.RadiiColor);
		PaintRect(&keyBox);
		RGBForeColor(&Black);
		MoveTo(425, 15*keyCount + 5);
		DrawString("\pRadii");
		keyCount--;
	}
	if(gGraph.Weight)
	{
		GraphLine(kResWeight, gGraph.WeightColor);
		SetRect(&keyBox, 410, 15*keyCount - 5, 420, 15*keyCount + 5);
		RGBForeColor(&gGraph.WeightColor);
		PaintRect(&keyBox);
		RGBForeColor(&Black);
		MoveTo(425, 15*keyCount + 5);
		DrawString("\pWeight");
		keyCount--;
	}
	
	TextFont(courier);
	TextSize(10);
	TextFace(normal);
	
	SetRect(&keyBox, 0, 0, gGraphWindow->portRect.right, 20);
	RGBBackColor(&White);
	EraseRect(&keyBox);
	
	MoveTo(3, 13);
	DrawString("\pAtomic Number: ");
	
	MoveTo(125, 13);
	DrawString("\pName: ");
	
	RGBBackColor(&White);
	MoveTo(0, 20);
	LineTo(gGraphWorld->portRect.right, 20);
	RGBForeColor(&Grey);
	MoveTo(0, 19);
	LineTo(gGraphWorld->portRect.right, 19);
	RGBForeColor(&Black);
	
	SetCursor(&qd.arrow);
	ShowWindow(gGraphWindow);
	
	SetGWorld( (CGrafPtr)gGraphWindow, 0L );
	CopyBits( &PORTBITMAP( gGraphWorld ), &WINBITMAP( gGraphWindow ),
		&gGraphWindow->portRect, &gGraphWindow->portRect, srcCopy, 0L );
}

void GraphLine(int id, RGBColor color)
{
	Str255				string;
	extended80			value;
	long double			number[110], high, low, scaledNumber[110], scale;
	Boolean				invalid[110];
	int					i;
	NumFormatStringRec	nfRec;
	Handle				itlHandle;
	long				offset, length;
		
	UseData();
		
	for(i = 0; i < 110; i++)
		invalid[i] = 0;
	
	for(i = 0; i < 110; i++)
	{
		GetIndString(string, id, i+1);
		
		if(IUCompString(string, "\p") == 0)
			invalid[i] = 1;
		
		GetIntlResourceTable(smCurrentScript, smNumberPartsTable, &itlHandle, &offset, &length);
		StringToFormatRec("\p#######.###;-#######.###;0", (NumberParts *)(*itlHandle + offset), &nfRec);
		StringToExtended(string, &nfRec, (NumberParts *)(*itlHandle + offset), &value);
		
		#ifdef powerc
			x80told(&value, &number[i]);
		#else
			number[i] = value;
		#endif
	}
	
	high = 0;
	for(i = 0; i < 110; i++)
	{
		if(number[i] > high && !invalid[i])
			high = number[i];
	}
	low = high;
	for(i = 0; i < 110; i++)
	{
		if(number[i] < low && !invalid[i])
			low = number[i];
	}
	scale = (gGraphWindow->portRect.bottom - 20)/(high-low);
	for(i = 0; i < 110; i++)
	{
		scaledNumber[i] = (number[i]-low)*scale;
	}
	
	RGBForeColor(&color);
	MoveTo(0,gGraphWindow->portRect.bottom-scaledNumber[0]);
	for(i = 0; i < 110; i++)
	{
		if(!invalid[i])
			LineTo(i*kGraphHScale, gGraphWindow->portRect.bottom-scaledNumber[i]);
	}
}

#pragma mark -
#pragma mark Search:
#pragma mark -

void SearchDialog()
{
	Handle			controlHdl;
	SInt16 			itemHit;
	short			itemType;
	Rect			itemRect;
	Handle			itemHandle;
	EventRecord		Event;
	DialogPtr		searchDialog;
	Str255			searchText = "\pSodium";
	int				it;
	
	UseData();
	
	if(!(searchDialog = GetNewDialog(129,0L,(WindowPtr) -1)))
		HandleError(21);

	SetDialogDefaultItem(searchDialog, 1);
	SetDialogCancelItem(searchDialog, 2);
	SetDialogTracksCursor(searchDialog,true);
		
	GetDialogItem(searchDialog, 3, &itemType, &itemHandle, &itemRect);
	SetDialogItemText(itemHandle, searchText);
 	SelectDialogItemText(searchDialog, 3, 0, 32767);
 		
	ShowWindow(searchDialog);
	
	do
	{
		ModalDialog(gDialogUPP, &itemHit);
		
		WaitNextEvent(everyEvent, &Event, 0L, 0L);
		
		if( IsDialogEvent(&Event) )
		{
			GetDialogItem(searchDialog, itemHit, &itemType, &controlHdl, &itemRect);
			SetControlValue((ControlHandle)controlHdl,!GetControlValue((ControlHandle) controlHdl));
		}
	} while((itemHit != 1) && (itemHit != 2));
	
	if(itemHit == 1)
	{
		GetDialogItem(searchDialog, 3, &itemType, &itemHandle, &itemRect);
		GetDialogItemText(itemHandle, searchText);
	
		it = SearchName(searchText);
	
		if(it)
		{
			gElementScroll.current = 0;
			Element(it);
			SelectWindow(gElementWindow);
			
			CorrectControls();
			CorrectMenus();
		}
		else
		{
			ParamText(searchText, "\p", "\p", "\p");
			NoteAlert(140, 0L);
		}
	}
	
	DisposeDialog(searchDialog);
}

int SearchName(Str255 str)
{
	int		it, i = 1;
	long	number;
	Str255	name, numberString;
	Boolean	found = 0;
	
	for(i=1; i<110; i++)
	{
	
		GetIndString(name, kResSymbol, i);
		if(IUEqualString(name, str) == 0)
		{
			it = i;
			i = 110;
			found = 1;
		}
	}
	
	if(found)
		return(it);
	else
	{
		for(i=1; i<110; i++)
		{
		
			GetIndString(name, kResName, i);
			if(IUEqualString(name, str) == 0)
			{
				it = i;
				i = 110;
				found = 1;
			}
		}
	}
	
	if(found)
		return(it);
	else
	{	
		StringToNum(str, &number);
		if(number > 0 && number < 110)
		{
			NumToString(number, numberString);
			if(IUEqualString(str, numberString) == 0)
			{
				it = number;
				found = 1;
			}
		}
	}
	
	if(found)
		return(it);
	else
		return(0);
}

#pragma mark -
#pragma mark Printing:
#pragma mark -

void HandlePrint()
{
	if(gElementWindow == FrontWindow())
	{
		Print(prElement);
	}
	if(gTableWindow == FrontWindow())
	{
		Print(prTable);
	}
	if(gGraphWindow == FrontWindow())
	{
		Print(prGraph);
	}
	if(gCompareWindow == FrontWindow())
	{
		Print(prCompare);
	}
}

void PrintPage(int what, TPPrPort port)
{
	SInt16		oldFile;
	Rect		rect, rect2;
	
	oldFile = CurResFile();
	
	switch(what)
	{
		case prElement:
			if(gPrintSettings.Pict)
			{
				SetRect(&rect, 0, 0, kElemWidth, kElemHeight);
				rect2 = rect;
				if(gPrintSettings.Center)
					CenterRect(&rect, port->gPort.portRect);
					
				CopyBits( &PORTBITMAP( gElementWorld ), &port->gPort.portBits,
					&rect2, &rect, srcCopy, 0L );
				FrameRect(&rect);
			}
			else
				PrintElementText();
			break;
		case prTable:
			if(gPrintSettings.Pict)
			{
				rect = gTableWindow->portRect;
				if(gPrintSettings.Center)
					CenterRect(&rect, port->gPort.portRect);
					
				CopyBits( &PORTBITMAP( gTableWorld ), &port->gPort.portBits,
					&gTableWindow->portRect, &rect, srcCopy, 0L );
				FrameRect(&rect);
			}
			else
				PrintTableText(port->gPort.portRect.bottom - port->gPort.portRect.top);
			break;
		case prGraph:
			if(gPrintSettings.Pict)
			{
				rect = gGraphWindow->portRect;
				if(gPrintSettings.Center)
					CenterRect(&rect, port->gPort.portRect);
					
				CopyBits( &PORTBITMAP( gGraphWorld ), &port->gPort.portBits,
					&gGraphWindow->portRect, &rect, srcCopy, 0L );
				FrameRect(&rect);
			}
			break;
		case prCompare:
			{
				int			x, h, row, perRow, page, item, items, i, j, k;
				Str255		string;
				
				item = 1; items = row = page = 0;
				for(i = 0; i < kListMax; i++)
				{
					if(gList.List[i])
						items++;
				}
				SortList();
				
				UseData();

				for(i = 0; i < kListMax; i++)
				{
					if(gList.List[i])
					{
						if(item*13 > port->gPort.portRect.bottom - port->gPort.portRect.top)
						{
							if(x+h < port->gPort.portRect.right - port->gPort.portRect.left)
							{
								perRow = item-2;
								item = 1;
								row++;
							}
							else
							{
								x = 0;
								for(j = 0; j <= row; j++)
								{
									x = j * (h + 20);
									MoveTo(x, 0);
									LineTo(x, (items+1 - (j*perRow))*13 + 2);
									if(gListSettings.name)
									{
										x += 80;
										MoveTo(x, 0);
										LineTo(x, (items+1 - (j*perRow))*13 + 2);
									}
									if(gListSettings.symbol)
									{
										x += 30;
										MoveTo(x, 0);
										LineTo(x, (items+1 - (j*perRow))*13 + 2);
									}
									if(gListSettings.number)
									{
										x += 30;
										MoveTo(x, 0);
										LineTo(x, (items+1 - (j*perRow))*13 + 2);
									}
									if(gListSettings.mass)
									{
										x += 70;
										MoveTo(x, 0);
										LineTo(x, (items+1 - (j*perRow))*13 + 2);
									}
									if(gListSettings.melt)
									{
										x += 60;
										MoveTo(x, 0);
										LineTo(x, (items+1 - (j*perRow))*13 + 2);
									}
									if(gListSettings.boil)
									{
										x += 60;
										MoveTo(x, 0);
										LineTo(x, (items+1 - (j*perRow))*13 + 2);
									}
									if(gListSettings.neg)
									{
										x += 40;
										MoveTo(x, 0);
										LineTo(x, (items+1 - (j*perRow))*13 + 2);
									}
									if(gListSettings.shc)
									{
										x += 40;
										MoveTo(x, 0);
										LineTo(x, (items+1 - (j*perRow))*13 + 2);
									}
									if(gListSettings.ion)
									{
										x += 40;
										MoveTo(x, 0);
										LineTo(x, (items+1 - (j*perRow))*13 + 2);
									}
									if(gListSettings.radii)
									{
										x += 40;
										MoveTo(x, 0);
										LineTo(x, (items+1 - (j*perRow))*13 + 2);
									}
									if(gListSettings.density)
									{
										x += 80;
										MoveTo(x, 0);
										LineTo(x, (items+1 - (j*perRow))*13 + 2);
									}
									for(k = 1; k <= items+1 - (j*perRow); k++)
									{
										MoveTo(j*(h + 20), k*13 + 2);
										LineTo(x, k*13 + 2);
									}
								}
								
								PrClosePage(port);
								if(PrError() != noErr)
									HandleError(23);
								PrOpenPage(port, 0L);
								if(PrError() != noErr)
									HandleError(23);
								
								perRow = item-2;
								item = 1;
								row = 0;
								page++;
							}
						}
						if(item == 1)
						{
							i--;
							x = 5 + (row * (h + 20));
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
								DrawString("\pMelting ¼");
								x += 60;
								TextFace(bold);
							}

							if(gListSettings.boil)
							{
								if(gList.Sort == boil)
									TextFace(bold+underline);
								MoveTo(x, 12);
								DrawString("\pBoiling ¼");
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
							if(!row)
								h = x;
						}
						else
						{
							TextFont(geneva);
							TextSize(9);
							TextFace(normal);
							x = 5 + (row * (h + 20));
							
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
								if(StrLength(string))
									DrawString("\p¼");
								x += 60;
							}
							
							if(gListSettings.boil)
							{
								MoveTo(x, item*13);
								GetIndString(string, kResBoil, gList.List[i]);
								ConvertStringCFK(string);
								DrawString(string);
								if(StrLength(string))
									DrawString("\p¼");
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
						}
						item++;
					}
					x = 0;
					for(j = 0; j <= row; j++)
					{
						x = j * (h + 20);
						MoveTo(x, 0);
						LineTo(x, (items+1 - (j*perRow))*13 + 2);
						if(gListSettings.name)
						{
							x += 80;
							MoveTo(x, 0);
							LineTo(x, (items+1 - (j*perRow))*13 + 2);
						}
						if(gListSettings.symbol)
						{
							x += 30;
							MoveTo(x, 0);
							LineTo(x, (items+1 - (j*perRow))*13 + 2);
						}
						if(gListSettings.number)
						{
							x += 30;
							MoveTo(x, 0);
							LineTo(x, (items+1 - (j*perRow))*13 + 2);
						}
						if(gListSettings.mass)
						{
							x += 70;
							MoveTo(x, 0);
							LineTo(x, (items+1 - (j*perRow))*13 + 2);
						}
						if(gListSettings.melt)
						{
							x += 60;
							MoveTo(x, 0);
							LineTo(x, (items+1 - (j*perRow))*13 + 2);
						}
						if(gListSettings.boil)
						{
							x += 60;
							MoveTo(x, 0);
							LineTo(x, (items+1 - (j*perRow))*13 + 2);
						}
						if(gListSettings.neg)
						{
							x += 40;
							MoveTo(x, 0);
							LineTo(x, (items+1 - (j*perRow))*13 + 2);
						}
						if(gListSettings.shc)
						{
							x += 40;
							MoveTo(x, 0);
							LineTo(x, (items+1 - (j*perRow))*13 + 2);
						}
						if(gListSettings.ion)
						{
							x += 40;
							MoveTo(x, 0);
							LineTo(x, (items+1 - (j*perRow))*13 + 2);
						}
						if(gListSettings.radii)
						{
							x += 40;
							MoveTo(x, 0);
							LineTo(x, (items+1 - (j*perRow))*13 + 2);
						}
						if(gListSettings.density)
						{
							x += 80;
							MoveTo(x, 0);
							LineTo(x, (items+1 - (j*perRow))*13 + 2);
						}
						for(k = 1; k <= items+1 - (j*perRow); k++)
						{
							MoveTo(j*(h + 20), k*13 + 2);
							LineTo(x, k*13 + 2);
						}
					}
				}
			}
			break;
	}
	
	UseResFile(oldFile);
}

pascal TPPrDlg PrintInitFunct(THPrint hPrint)
{
	Handle			controlHdl;
	short			itemType;
	Rect			itemRect;
	Boolean			oldPictPref;
	
	oldPictPref = gPrintSettings.Pict;

	AppendTheDITL(gTPrDlgStructurePtr);
	
	if(gPrintWhat == prGraph)
	{
		gPrintSettings.Pict = 1;
		GetDialogItem((DialogPtr)gTPrDlgStructurePtr, gPrintDITLAppendNumb, &itemType, &controlHdl, &itemRect);
		HiliteControl((ControlHandle)controlHdl, 255);
	}
	if(gPrintWhat == prCompare)
	{
		gPrintSettings.Pict = 0;
		GetDialogItem((DialogPtr)gTPrDlgStructurePtr, gPrintDITLAppendNumb+1, &itemType, &controlHdl, &itemRect);
		HiliteControl((ControlHandle)controlHdl, 255);
		GetDialogItem((DialogPtr)gTPrDlgStructurePtr, gPrintDITLAppendNumb+2, &itemType, &controlHdl, &itemRect);
		HiliteControl((ControlHandle)controlHdl, 255);
	}
	
	if(gPrintSettings.Pict)
	{
		GetDialogItem((DialogPtr)gTPrDlgStructurePtr, gPrintDITLAppendNumb, &itemType, &controlHdl, &itemRect);
		SetControlValue((ControlHandle)controlHdl, 0);
		
		GetDialogItem((DialogPtr)gTPrDlgStructurePtr, gPrintDITLAppendNumb + 2, &itemType, &controlHdl, &itemRect);
		HiliteControl((ControlHandle)controlHdl, 0);
		
		GetDialogItem((DialogPtr)gTPrDlgStructurePtr, gPrintDITLAppendNumb + 1, &itemType, &controlHdl, &itemRect);
		SetControlValue((ControlHandle)controlHdl, 1);
	}
	else
	{
		GetDialogItem((DialogPtr)gTPrDlgStructurePtr, gPrintDITLAppendNumb, &itemType, &controlHdl, &itemRect);
		SetControlValue((ControlHandle)controlHdl, 1);
		
		GetDialogItem((DialogPtr)gTPrDlgStructurePtr, gPrintDITLAppendNumb + 2, &itemType, &controlHdl, &itemRect);
		HiliteControl((ControlHandle)controlHdl, 255);
		
		GetDialogItem((DialogPtr)gTPrDlgStructurePtr, gPrintDITLAppendNumb + 1, &itemType, &controlHdl, &itemRect);
		SetControlValue((ControlHandle)controlHdl, 0);
	}
	
	GetDialogItem((DialogPtr)gTPrDlgStructurePtr, gPrintDITLAppendNumb + 2, &itemType, &controlHdl, &itemRect);
	SetControlValue((ControlHandle)controlHdl, gPrintSettings.Center);

	gOldItemEvaluateFunctUPP = gTPrDlgStructurePtr->pItemProc;
	gTPrDlgStructurePtr->pItemProc = gNewItemEvaluateFunctUPP;


	gTPrDlgStructurePtr->pFltrProc = gPrintEventFilterUPP;
	
	gPrintSettings.Pict = oldPictPref;

	return gTPrDlgStructurePtr;
}

void AppendTheDITL(TPPrDlg theDialog)
{
	Handle	ditlHdl;
	SInt16	numberOfExistingItems, curResFile;
	
	curResFile = CurResFile();
	UseData();
	ditlHdl = GetResource('DITL', 200);
	UseResFile(curResFile);
	
	numberOfExistingItems = CountDITL((DialogPtr) theDialog);
	AppendDITL((DialogPtr)theDialog, ditlHdl, appendDITLBottom);
	gPrintDITLAppendNumb = numberOfExistingItems + 1;
}

pascal PrintItemEvaluationFunct(TPPrDlg theDialog, short itemHit)
{
	SInt16			localizedItemNo;
	Handle			controlHdl;
	short			itemType;
	Rect			itemRect;
	
	localizedItemNo = itemHit - gPrintDITLAppendNumb + 1;

	if(localizedItemNo > 0)
	{
		switch(localizedItemNo)
		{
			case 1:
			case 2:
				GetDialogItem((DialogPtr)theDialog, gPrintDITLAppendNumb, &itemType, &controlHdl, &itemRect);
				SetControlValue((ControlHandle)controlHdl, 0);
				GetDialogItem((DialogPtr)theDialog, gPrintDITLAppendNumb + 1, &itemType, &controlHdl, &itemRect);
				SetControlValue((ControlHandle)controlHdl, 0);
				GetDialogItem((DialogPtr)theDialog, itemHit, &itemType, &controlHdl, &itemRect);
				SetControlValue((ControlHandle)controlHdl, 1);
				if(localizedItemNo == 1)
				{
					gPrintSettings.Pict = 0;
					GetDialogItem((DialogPtr)theDialog, gPrintDITLAppendNumb + 2, &itemType, &controlHdl, &itemRect);
					HiliteControl((ControlHandle)controlHdl, 255);
				}
				if(localizedItemNo == 2)
				{
					gPrintSettings.Pict = 1;
					GetDialogItem((DialogPtr)theDialog, gPrintDITLAppendNumb + 2, &itemType, &controlHdl, &itemRect);
					HiliteControl((ControlHandle)controlHdl, 0);
				}
				break;
			case 3:
				GetDialogItem((DialogPtr)theDialog, itemHit, &itemType, &controlHdl, &itemRect);
				SetControlValue((ControlHandle)controlHdl,!GetControlValue((ControlHandle)controlHdl));
				gPrintSettings.Center = GetControlValue((ControlHandle)controlHdl);
				break;
		}
	}
	else
	{
		CallPItemProc(gOldItemEvaluateFunctUPP,(DialogPtr) theDialog,itemHit);
	}
}

pascal Boolean PrintEventFilter(DialogPtr dialogPtr, EventRecord *EventPtr,
														SInt16 *itemHit)
{
	Boolean				handledEvent;
	GrafPtr				oldPort;

	handledEvent = false;

	if((EventPtr->what == updateEvt) && ((WindowPtr) EventPtr->message != dialogPtr))
	{
		UpdateWindow((WindowPtr)EventPtr->message);
	}
	else
	{
		GetPort(&oldPort);
		SetPort(dialogPtr);

		handledEvent = StdFilterProc(dialogPtr,EventPtr,itemHit);

		SetPort(oldPort);
	}

	return(handledEvent);
}

void Print(int what)
{
	GrafPtr		oldPort;
	TPPrPort	printPort;
	TPrStatus	printStatus;
	Boolean		ok;
	SInt16		copy, copies;
	
	GetPort(&oldPort);
	
	PrOpen();
	if(PrError() == noErr)
	{	
		if(gPrintRec == 0L)
		{
			gPrintRec = (THPrint)NewHandle(sizeof(TPrint));
			PrintDefault(gPrintRec);
		}
		
		PrValidate(gPrintRec);
		
		gTPrDlgStructurePtr = PrJobInit(gPrintRec);
		if(PrError() == noErr)
		{
			gPrintInitFunctUPP = NewPDlgInitProc((ProcPtr) PrintInitFunct);
			gNewItemEvaluateFunctUPP = NewPItemProc((ProcPtr) PrintItemEvaluationFunct);
			gPrintEventFilterUPP = NewModalFilterProc((ProcPtr) PrintEventFilter);
			
			gPrintWhat = what;
			ok = PrDlgMain(gPrintRec,gPrintInitFunctUPP);
			
			DisposeRoutineDescriptor(gPrintInitFunctUPP);
			DisposeRoutineDescriptor(gNewItemEvaluateFunctUPP);
			DisposeRoutineDescriptor(gPrintEventFilterUPP);
			
			if(ok)
			{
				if(PrError() == noErr)
				{
					printPort = PrOpenDoc(gPrintRec, 0L, 0L);
					if(PrError() == noErr)
					{
						copies = (*gPrintRec)->prJob.iCopies;
						for(copy = 1; copy <= copies; copy++)
						{
							PrOpenPage(printPort, 0L);
							if(PrError() != noErr)
								HandleError(23);
								
							PrintPage(what, printPort);
							
							PrClosePage(printPort);
							if(PrError() != noErr)
								HandleError(23);		//DebugStr("\pPrClosePage error");
						}
						
						PrCloseDoc(printPort);
						if(PrError() != noErr)
							HandleError(23);		//DebugStr("\pPrCloseDoc error");
							
						if((((TPPrint)*gPrintRec)->prJob.bJDocLoop == bSpoolLoop))
						{
							PrPicFile(gPrintRec, 0L, 0L, 0L, &printStatus);
							if(PrError() != noErr)
								HandleError(23);	//DebugStr("\pPrPicFile error");
						}
					} else HandleError(23);				//DebugStr("\pPrOpenDoc error");
				} else HandleError(23);					//DebugStr("\pPrJobDialog error");
				
				PrClose();
			}
		} //else HandleError(23);						//DebugStr("\pPrOpen error");
	}
	SetPort(oldPort);
}

void PrintElementText()
{
	Str255	pE;
	
	LoadText(gE);

	NumToString(gE, pE);

	MoveTo(5, 25);
	TextFont(times);
	TextFace(bold);
	TextSize(18);
	DrawString(gNameText);

	TextFont(geneva);
	TextFace(0);
	TextSize(12);

	MoveTo(5, 40);
	DrawString("\pAtomic Number: ");
	DrawString(pE);

	MoveTo(5, 55);
	DrawString("\pAtomic Weight: ");
	DrawString(gWeightText);

	//TextFont(helvetica);
	//TextSize(10);

	MoveTo(5, 70);
	DrawString("\pElectron Configuration: ");

	if(gEConfigValid)
		DrawString(gEConfigText);
	else
		DrawString("\p --");

	MoveTo(5, 85);
	DrawString("\pAtomic Radius: ");

	if(gRadiiValid)
	{
		DrawString(gRadiiText);
		DrawString("\p nm");
	}	
	else
		DrawString("\p --");

	MoveTo(5, 100);
	DrawString("\pMelting Point: ");

	if(!gMeltValid)
		DrawString("\p --");
	else
	{
		ConvertStringCFK(gMeltText);
		DrawString(gMeltText);
		
		if(gTempType.tempType == kCelcius)
			DrawString("\p¡ C");
		if(gTempType.tempType == kFahrenheit)
			DrawString("\p¡ F");
		if(gTempType.tempType == kKelvin)
			DrawString("\p K");
	}

	MoveTo(5, 115);
	DrawString("\pBoiling Point: ");

	if(!gBoilValid)
		DrawString("\p --");
	else
	{
		ConvertStringCFK(gBoilText);
		DrawString(gBoilText);
		
		if(gTempType.tempType == kCelcius)
			DrawString("\p¡ C");
		if(gTempType.tempType == kFahrenheit)
			DrawString("\p¡ F");
		if(gTempType.tempType == kKelvin)
			DrawString("\p K");
	}
		
	MoveTo(5, 130);
	DrawString("\pMajor Oxidation States: ");
	if(gOxiValid)
		DrawString(gOxiText);
	else
		DrawString("\p  --");

	MoveTo(5, 145);
	DrawString("\pElectronegativity: ");
	if(gNegValid)
		DrawString(gNegText);
	else
		DrawString("\p  --");

	MoveTo(5, 160);
	DrawString("\pSpecific Heat Capacity:  ");
	if(gSHCValid)
	{
		DrawString(gSHCText);
		DrawString("\p J/g-C");
	}
	else
		DrawString("\p --");

	MoveTo(5, 175);
	DrawString("\pFirst Ionization Energy: ");
	if(gIonValid)
	{
		DrawString(gIonText);
		DrawString("\p kJ/mol");
	}
	else
	DrawString("\p  --");

	if(gDateValid)
	{
		MoveTo(5, 190);
		DrawString("\pDiscovered in ");
		DrawString(gDateText);
	}
}

void PrintTableText(int height)
{
	Str255	name, symbol, pE;
	int		e, eBottom = 0, x, column = 1;
	
	TextFont(courier);
	TextSize(10);
	TextFace(normal);
		
	for(e=1;e<110;e++)
	{
		GetIndString(name, kResName, e);
		GetIndString(symbol, kResSymbol, e);
		NumToString(e, pE);
		
		x = ((e - eBottom)*15) + 15;
		if(x > height)
		{
			eBottom = e - 1;
			column++;
			x = ((e - eBottom)*15) + 15;
		}
		
		MoveTo(5 + (column-1)*170, x);
		DrawString(pE);
		MoveTo(35 + (column-1)*170, x);
		DrawString(symbol);
		MoveTo(95 + (column-1)*170, x);
		DrawString(name);
	}
	
	TextFace(bold);
	for(x = 1; x <= column; x++)
	{	
		MoveTo(5 + (x-1)*170, 15);
		DrawString("\pZ");
		MoveTo(40 + (x-1)*170, 15);
		DrawString("\pSymbol");
		MoveTo(95 + (x-1)*170, 15);
		DrawString("\pName");
	}
	
	for(x = 1; x < column; x++)
	{
		MoveTo(x*170, 5);
		LineTo(x*170, height - 5);
	}
	
}

#pragma mark -
#pragma mark Copying:
#pragma mark -

void HandleCopy(int item)
{	
	switch(item)
	{
		case 1:
			CopyText(&gNameText, item);
			break;
		case 2:
			CopyText(&gWeightText, item);
			break;
		case 3:
			CopyText(&gRadiiText, item);
			break;
		case 4:
			CopyText(&gDensityText, item);
			break;
		case 5:
			CopyText(&gMeltText, item);
			break;
		case 6:
			CopyText(&gBoilText, item);
			break;
		case 7:
			CopyText(&gOxiText, item);
			break;
		case 8:
			CopyText(&gNegText, item);
			break;
		case 9:
			CopyText(&gSHCText, item);
			break;
		case 10:
			CopyText(&gIonText, item);
			break;
	}
}

void AppendLabel(Str255 a, int item)
{
	switch(item)
	{
		case 2:
			AppendString(a, "\p u");
			break;
		case 3:
			AppendString(a, "\p nm");
			break;
		case 4:
			AppendString(a, "\p g/cm3");
			break;
		case 5:
		case 6:
			if(gTempType.tempType == kCelcius)
				AppendString(a, "\p ¼C");
			if(gTempType.tempType == kKelvin)
				AppendString(a, "\p ¼K");
			if(gTempType.tempType == kFahrenheit)
				AppendString(a, "\p ¼F");
			break;
		case 9:
			AppendString(a, "\p J/g-C");
			break;
		case 10:
			AppendString(a, "\p kJ/mol");
			break;
	}
}

void CopyText(Str255 *String, int item)
{
	int				stringLen;
	Size			dataLength;
	StringHandle	TextHandle;
	
	stringLen = StrLength(*String);
	if(gCopyLabels)
		AppendLabel(*String, item);
	
	TextHandle = NewString(*String);

	dataLength = GetHandleSize((Handle)TextHandle);
	++*TextHandle;
	--dataLength;
		
	HLock((Handle)TextHandle);
	Copy((Handle)TextHandle, 'TEXT', dataLength);
	HUnlock((Handle)TextHandle);
	*String[0] = stringLen;
}

void CopyPict(PicHandle pict)
{
	HLock((Handle)pict);
	Copy((Handle)pict, 'PICT', GetHandleSize((Handle)pict));
	HUnlock((Handle)pict);
}

void Copy(Handle scrapHdl, ResType type, Size dataLength)
{
	SInt32			errorCode;
	
	if(ZeroScrap() == noErr)
	{
		errorCode = PutScrap((SInt32) dataLength,type,*((Handle)scrapHdl));
		if(errorCode != noErr)
			HandleError(11);
	}
	else
		HandleError(12);
}

#pragma mark -
#pragma mark Gradiential Table:
#pragma mark -

void Gradient()
{
	int			which, keyCount = 6;
	xvar		factor;
	
	
	which = gTablePrefs.style;
	if(gZoom)
		SetupTableRects();
	gZoom = 0;
	
	if(gWindowSize.Table)
		factor = 1;
	else
		factor = .75;
	
	SizeWindow(gTableWindow, kTableWidth * factor, kTableHeight * factor, 0);
	ShowWindow(gTableWindow);
	
	UseData();
			
	LockPixels(GetGWorldPixMap(gTableWorld));
	SetGWorld((CGrafPtr)gTableWorld, 0L);
	
	RGBBackColor(&Grey);
	EraseRect(&gTableWindow->portRect);
		
	TextFont(times);
	
	SetCursor(*GetCursor(watchCursor));
	
	switch(which)
	{
		case kGradName:
			UsePicts();
			if(gWindowSize.Table)
			{
				if((gTablePict = GetPicture(999))==0L)
					HandleError(9);
			}
			else
			{
				if((gTablePict = GetPicture(1000))==0L)
					HandleError(9);
			}
			DrawPicture(gTablePict, &gTableWindow->portRect);
			UseData();
			break;
		case kGradRadii:
			GradientTable(kResRadii);
			GradientName("\pAtomic Radii");
			break;
		case kGradDensity:
			GradientTable(kResDensity);
			GradientName("\pDensity at 25¼ C");
			break;
		case kGradMelt:
			GradientTable(kResMelt);
			GradientName("\pMelting Point");
			break;
		case kGradBoil:
			GradientTable(kResBoil);
			GradientName("\pBoiling Point");
			break;
		case kGradIon:
			GradientTable(kResIonization);
			GradientName("\pFirst Ionization Energy");
			break;
		case kGradNeg:
			GradientTable(kResNeg);
			GradientName("\pElectronegativity");
			break;
		case kGradLewis:
			LewisTable();
			GradientName("\pLewis Dot Diagram");
			break;
	}
	
	if(gWindowSize.Table)
	{
		TextFont(geneva);
		TextSize(10);
		TextFace(bold);
	}
	else
	{
		TextFont(geneva);
		TextSize(9);
		TextFace(normal);
	}
	
	if(!gTablePrefs.classic)
	{
		DrawRow(1, "\p1");
		DrawRow(4, "\p2");
		DrawRow(21, "\p3");
		DrawRow(22, "\p4");
		DrawRow(23, "\p5");
		DrawRow(24, "\p6");
		DrawRow(25, "\p7");
		DrawRow(26, "\p8");
		DrawRow(27, "\p9");
		DrawRow(28, "\p10");
		DrawRow(29, "\p11");
		DrawRow(30, "\p12");
		DrawRow(5, "\p13");
		DrawRow(6, "\p14");
		DrawRow(7, "\p15");
		DrawRow(8, "\p16");
		DrawRow(9, "\p17");
		DrawRow(2, "\p18");
	}
	else
	{
		DrawRow(1, "\p1A");
		DrawRow(4, "\p2A");
		DrawRow(21, "\p3B");
		DrawRow(22, "\p4B");
		DrawRow(23, "\p5B");
		DrawRow(24, "\p6B");
		DrawRow(25, "\p7B");
		DrawRow(26, "\p8B");
		DrawRow(27, "\p8B");
		DrawRow(28, "\p8B");
		DrawRow(29, "\p1B");
		DrawRow(30, "\p2B");
		DrawRow(5, "\p3A");
		DrawRow(6, "\p4A");
		DrawRow(7, "\p5A");
		DrawRow(8, "\p6A");
		DrawRow(9, "\p7A");
		DrawRow(2, "\p8A");
	}
		
	CheckItem(GetMenuHandle(133), 1, 0);
	CheckItem(GetMenuHandle(133), 2, 0);
	CheckItem(GetMenuHandle(133), 3, 0);
	CheckItem(GetMenuHandle(133), 4, 0);
	CheckItem(GetMenuHandle(133), 5, 0);
	CheckItem(GetMenuHandle(133), 6, 0);
	CheckItem(GetMenuHandle(133), 7, 0);
	CheckItem(GetMenuHandle(133), 8, 0);
	CheckItem(GetMenuHandle(133), which, 1);
	
	SetCursor(&qd.arrow);
	
	SetGWorld( (CGrafPtr)gTableWindow, 0L );
	CopyBits( &PORTBITMAP( gTableWorld ), &WINBITMAP( gTableWindow ),
		&gTableWindow->portRect, &gTableWindow->portRect, srcCopy, 0L );
}

void GradientName(Str255 string)
{
	int	length;
	
	length = gElementRects[5].left - gElementRects[4].right;
	
	if(gWindowSize.Table)
	{
		TextFont(newYork);
		TextSize(18);
		TextFace(normal);
	}
	else
	{
		TextFont(newYork);
		TextSize(14);
		TextFace(normal);
	}
	
	MoveTo(gElementRects[4].right + (length-StringWidth(string))/2, gElementRects[4].top);
	DrawString(string);
}

void DrawRow(int e, Str255 string)
{
	Rect	theRect;
	Point	thePoint;
	
	theRect = gElementRects[e];
	thePoint.h = (theRect.left+((theRect.right-theRect.left)/2) - (StringWidth(string)/2));
	thePoint.v = (theRect.top - 3);

	MoveTo(thePoint.h, thePoint.v);
	DrawString(string);
}

void GradientTable(int id)
{
	Str255				string;
	extended80			value;
	xvar				number[110], high, low, scale, scaledNumber[110], j;
	Boolean				invalid[110];
	int					midh, midv, space, highE, lowE;
	NumFormatStringRec	nfRec;
	Handle				itlHandle;
	long				offset, length, i;
	RGBColor			color;
	FontInfo			fontInfo;
	Rect				theOutlineRect;
		
	UseData();
		
	for(i = 0; i < 110; i++)
		invalid[i] = 0;
	
	for(i = 0; i < 110; i++)
	{
		GetIndString(string, id, i+1);
		
		if(IUCompString(string, "\p") == 0)
			invalid[i] = 1;
		
		GetIntlResourceTable(smCurrentScript, smNumberPartsTable, &itlHandle, &offset, &length);
		StringToFormatRec("\p#######.###;-#######.###;0", (NumberParts *)(*itlHandle + offset), &nfRec);
		StringToExtended(string, &nfRec, (NumberParts *)(*itlHandle + offset), &value);
		
		#ifdef powerc
			x80told(&value, &number[i]);
		#else
			number[i] = value;
		#endif
	}
	
	high = 0; highE = 0;
	for(i = 0; i < 110; i++)
	{
		if(number[i] > high && !invalid[i])
		{
			high = number[i];
			highE = i + 1;
		}
	}
	low = high; lowE = highE;
	for(i = 0; i < 110; i++)
	{
		if(number[i] < low && !invalid[i])
		{
			low = number[i];
			lowE = i + 1;
		}
	}
	
	scale = 1/(high-low);
	for(i = 0; i < 110; i++)
	{
		scaledNumber[i] = (number[i]-low)*scale;
	}
	
	for(i = 0; i < 109; i++)
	{
		if(!invalid[i])
		{
			color.red = color.blue = 0x0000;
			color.green = 0xD000 * (1-scaledNumber[i]) + 0x1FFF;
			RGBBackColor(&color);
		}
		else
			RGBBackColor(&Blue);
		EraseRect(&gElementRects[i+1]);
		FrameRect(&gElementRects[i+1]);
		
		GetIndString(string, kResSymbol, i+1);
		ConvertLowStringCFK(string);
		
		if(scaledNumber[i] > .6)
			color.red = color.blue = color.green = 0xFFFF;
		else
			color.red = color.blue = color.green = 0x0000;
		RGBForeColor(&color);
		TextFont(newYork);
		TextSize(12);
		TextFace(0);
		GetFontInfo(&fontInfo);
		midh = gElementRects[i+1].left + (gElementRects[i+1].right-gElementRects[i+1].left - StringWidth(string))/2;
		midv = gElementRects[i+1].top + fontInfo.ascent+fontInfo.leading + 5;
		MoveTo(midh, midv);
		DrawString(string);
		
		if(gWindowSize.Table)
		{
			GetIndString(string, id, i+1);
			if(id == kResMelt || id == kResBoil)
				ConvertLowStringCFK(string);
			if(id == kResDensity)
				SmallerNumber(string);
			
			TextFont(helvetica);
			TextFace(condense);
			TextSize(10);
			GetFontInfo(&fontInfo);
			midh = gElementRects[i+1].left + (gElementRects[i+1].right-gElementRects[i+1].left - StringWidth(string))/2;
			midv = gElementRects[i+1].bottom - fontInfo.descent - 5;
			MoveTo(midh, midv);
			DrawString(string);
		}
		RGBForeColor(&Black);
	}
	
	space = (gElementRects[5].left - gElementRects[4].right)/2 + gElementRects[4].right;
	j = 0;
	for(i = 1; i < 100; i++)
	{
		j += 0.01;
		
		color.red = color.blue = 0x0000;
		color.green = 0xD000 * (1-j) + 0x1FFF;
		RGBForeColor(&color);
		MoveTo(space-50 + i, gElementRects[4].top + 5);
		LineTo(space-50 + i, gElementRects[4].top + 20);
	}
	RGBForeColor(&Black);
	SetRect(&theOutlineRect,
			space-50, gElementRects[4].top + 5, space+50, gElementRects[4].top + 21);
	FrameRect(&theOutlineRect);
	
	TextFont(helvetica);
	TextFace(condense);
	TextSize(10);
	
	GetIndString(string, id, highE);
	MoveTo(space+55, gElementRects[4].top + 20);
	DrawString(string);
	
	GetIndString(string, id, lowE);
	MoveTo(space-55-StringWidth(string), gElementRects[4].top + 20);
	DrawString(string);
}

void SmallerNumber(Str255 String1)
{
	extended80			number;
	NumFormatStringRec	nfRec;
	Handle				itlHandle;
	long				offset, length;
	
	GetIntlResourceTable(smCurrentScript, smNumberPartsTable, &itlHandle, &offset, &length);
	StringToFormatRec("\p#######.###;-#######.###;0", (NumberParts *)(*itlHandle + offset), &nfRec);
	StringToExtended(String1, &nfRec, (NumberParts *)(*itlHandle + offset), &number);
		
	GetIntlResourceTable(smCurrentScript, smNumberPartsTable, &itlHandle, &offset, &length);
	StringToFormatRec("\p#######.###;-#######.###;0", (NumberParts *)(*itlHandle + offset), &nfRec);
	ExtendedToString(&number, &nfRec, (NumberParts *)(*itlHandle + offset), String1);
}

void LewisTable()
{
	int	i;
		
	UseData();
	TextFont(times);
	if(gWindowSize.Table)
		TextSize(12);
	else
		TextSize(10);
	TextFace(normal);
	
	for(i = 1; i < 110; i++)
	{
		RGBBackColor(&White);
		EraseRect(&gElementRects[i]);
		RGBBackColor(&White);
		FrameRect(&gElementRects[i]);
		
		GetIndString(gSymbText, kResSymbol, i);
		LewisDots(i,
			gElementRects[i].left + (gElementRects[i].right-gElementRects[i].left)/2,
			gElementRects[i].top + (gElementRects[i].bottom-gElementRects[i].top)/2);
	}
}


#pragma mark -
#pragma mark Long Doubles:
#pragma mark -

void ConvertToLD(int id, ResType type)
{
	Str255		String;
	long double	Number;
	int			i;
	
	UseData();
	
	for(i = 1; i < 110; i++)
	{
		GetIndString(String, id, i);
		if(StrLength(String))
			StringToDouble(String, &Number);
		else
			Number = 0;
		WriteNumber(i, type, &Number);
	}
}

void StringToDouble(Str255 String, long double *Double)
{
	int 		i, Decimal, DecimalFactor, LeftNumber, RightNumber, LeftDigits, RightDigits;
	Boolean		Neg, Done;
	Str255		LeftSide, RightSide;
	char		letter;
	Done = Neg = false;
	
	*Double = 0;
	if(StrLength(String))
	{
		LeftDigits = RightDigits = Decimal = 0;
		for(i = 1; i <= StrLength(String); i++)
		{
			letter = String[i];
			if(i == 1 && letter == '-')
				Neg = true;
				
			if(!Done)
			{
				switch(letter)
				{
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
					case '0':
						LeftDigits++;
						LeftSide[LeftDigits] = letter;
						break;
					case '.':
						Done = true;
						LeftSide[0] = LeftDigits;
						Decimal = i;
						break;
				}
			}
		}
		if(LeftDigits == 0 && !Done)
			LeftDigits = i;
		else
			LeftSide[0] = LeftDigits;
			
		if(Decimal)
		{
			for(i = Decimal+1; i <= StrLength(String); i++)
			{
				letter = String[i];
				switch(letter)
				{
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
					case '0':
						RightDigits++;
						RightSide[RightDigits] = letter;
						break;
				}
			}
			RightSide[0] = RightDigits;
			StringToNum(RightSide, (long *)&RightNumber);
			
			DecimalFactor = 1;
			for(i = 0; i < RightDigits; i++)
				DecimalFactor = DecimalFactor * 10;
		}
		else
		{
			RightNumber = 0;
			DecimalFactor = 0;
		}
			
		StringToNum(LeftSide, (long *)&LeftNumber);
		
		if(RightNumber > 0)
		{
			*Double = RightNumber;
			*Double = *Double / DecimalFactor;
		}
		*Double += LeftNumber;
		if(Neg)
		*Double = *Double * -1;
	}
}

void StringToX(Str255 String, extended80 *Number)
{
	NumFormatStringRec	nfRec;
	Handle				itlHandle;
	long				offset, length;
	
	GetIntlResourceTable(smCurrentScript, smNumberPartsTable, &itlHandle, &offset, &length);
	StringToFormatRec("\p#,###,###.###;-#,###,###.###;0", (NumberParts *)(*itlHandle + offset), &nfRec);
	StringToExtended(String, &nfRec, (NumberParts *)(*itlHandle + offset), Number);
}

void WriteNumber(int e, ResType type, long double *data)
{
	long double **dataHandle;
	Handle 		existingResHdl;
	Str255		Name;
	
	NumToString((long)e, Name);

	dataHandle = (long double **)NewHandleClear(sizeof(long double));
	HLock((Handle)dataHandle);

	**dataHandle = *data;
	
	existingResHdl = Get1Resource(type, 127 + e);
	if(existingResHdl != 0L)
		RemoveResource(existingResHdl);
	
	if(ResError() == noErr)
		AddResource((Handle) dataHandle, type, 127 + e, Name);
	if(ResError() == noErr)
		WriteResource((Handle) dataHandle);

	HUnlock((Handle) dataHandle);
	ReleaseResource((Handle) dataHandle);
}

void GetNumber(int e, ResType type, xvar *number)
{
	long double		**numberHandle;
	
	UseData();
	numberHandle = (long double **) Get1Resource(type,127+e);
	if(numberHandle != 0L)
	{
#ifdef powerc
		*number = **numberHandle;
#else	
		ldtox80(*numberHandle, number);
#endif
	}
	else
		*number = 0;
}

#pragma mark -
#pragma mark Goodbye:
#pragma mark -

void Quit()
{
	SavePreferences();
	
	CloseResFile(gPrefsFRN);
	CloseResFile(gDataFile);
	CloseResFile(gPictsFile);
	CloseResFile(gIsotopesFile);
	CloseResFile(gIsotopes2File);
	CloseResFile(gIsotopes3File);
	
	if(gElementWindow != 0L)
		HideWindow(gElementWindow);
	if(gTableWindow != 0L)
		HideWindow(gTableWindow);
	
	if(gIsotopeWorld != 0L)
		DisposeGWorld(gIsotopeWorld);
	if(gElementWorld != 0L)
		DisposeGWorld(gElementWorld);
	if(gTableWorld != 0L)
		DisposeGWorld(gTableWorld);
}