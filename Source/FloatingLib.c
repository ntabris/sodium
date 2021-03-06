// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××
// FloatingLib.c
// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××

// ÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉ includes

#include <LowMem.h>

// ÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉ defines	

#define kFloaterKind									7
#define eWindowNotCreatedError				-2
#define eInvalidWindowOrderingError		-3

// ÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉ define routine for creating a routine descriptor

typedef pascal void (*ActivateHandlerProcPtr)
										(WindowPtr theWindow,Boolean activateWindow);

typedef ActivateHandlerProcPtr ActivateHandlerUPP;
#define CallActivateHandlerProc(userRoutine,theWindow,activateWindow) \
				(*userRoutine)(theWindow,activateWindow)
#define NewActivateHandlerProc(userRoutine) (ActivateHandlerUPP) (userRoutine)

// ÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉ typedefs

typedef struct 
{
	ActivateHandlerUPP	activateFunctionUPP;
	Boolean							wasVisible;
	SInt32							replacementRefCon;
} refConExtend;

typedef refConExtend **refConExtendHandle;

// ÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉ function prototypes

OSErr								FW_GetNewCWindow									(WindowPtr *,SInt16,WindowPtr,
																											 ActivateHandlerUPP,Boolean);
OSErr								FW_GetNewDialog										(WindowPtr *,SInt16,WindowPtr,
																											 ActivateHandlerUPP,Boolean);
void 								FW_DisposeWindow									(WindowPtr);
void 								FW_SelectWindow										(WindowPtr);
void 								FW_HideWindow											(WindowPtr);
void 								FW_ShowWindow											(WindowPtr);
void 								FW_DragWindow											(WindowPtr,Point,const Rect *);
void 								FW_doSuspendEvent									(void);
void 								FW_doResumeEvent									(void);
void 								FW_deactivateFloatsAndFirstDocWin	(void);
void 								FW_activateFloatsAndFirstDocWin		(void);
WindowPtr						FW_findFrontNonFloatWindow				(void);
void 								FW_validateWindowList							(void);
SInt32							FW_GetWRefCon											(WindowPtr);
void								FW_SetWRefCon											(WindowPtr,SInt32);
void 								activateWindow										(WindowPtr);
void 								deactivateWindow									(WindowPtr);
void 								highlightAndActivateWindow				(WindowPtr,Boolean);
WindowPtr 					findLastFloatingWindow						(void);
OSErr								checkWindowOrdering								(Boolean,WindowPtr *);
WindowPtr						getNextVisibleWindow							(WindowPtr);
Boolean							getWasVisible											(WindowPtr);
void								setWasVisible											(WindowPtr,Boolean);
Boolean 						isWindowModal											(WindowPtr);
void								bringFloatersToFront							(WindowPtr);
Boolean							isFrontProcess										(void);
Boolean							getIsWindowVisible								(WindowPtr);
WindowPtr 					getNextWindow											(WindowPtr);
SInt16							getWindowKind											(WindowPtr);
RgnHandle						getStructureRegion								(WindowPtr);
RgnHandle						getContentRegion									(WindowPtr);
ActivateHandlerUPP	getActivateHandler								(WindowPtr);
void								setActivateHandler								(WindowPtr,ActivateHandlerUPP);

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× FW_GetNewCWindow

OSErr  FW_GetNewCWindow(WindowPtr *windowPtr,SInt16 windResourceID,WindowPtr behind,
											 ActivateHandlerUPP activateFunctionUPP,Boolean isFloater)
{
	OSErr								result;
	refConExtendHandle	refConExtendHdl;
	WindowPtr						newWindowPtr;
	SInt32							replacementRefCon;
	Boolean							isVisible;

	*windowPtr = NULL;

 	result = checkWindowOrdering(isFloater,&behind);
 	if(result != noErr)
 		return result;  

	refConExtendHdl = (refConExtendHandle) NewHandle(sizeof(refConExtend));
	
	if((result = MemError()) == noErr)
	{
		newWindowPtr = GetNewCWindow(windResourceID,NULL,behind);

		if(newWindowPtr != NULL)
		{
			*windowPtr = newWindowPtr;

			replacementRefCon = GetWRefCon(newWindowPtr);
			SetWRefCon(newWindowPtr,(SInt32) refConExtendHdl);
			FW_SetWRefCon(newWindowPtr,replacementRefCon);

			setActivateHandler(newWindowPtr,activateFunctionUPP);

			if(isFloater)
			{
				((WindowPeek) newWindowPtr)->windowKind = kFloaterKind;

				isVisible = ((WindowPeek) newWindowPtr)->visible;

				if(isVisible)
				{
					if(!isFrontProcess())
					{
						ShowHide(newWindowPtr,false);
						setWasVisible(newWindowPtr,true);
					}	
				}
			}
		}
		else
		{
			DisposeHandle((Handle) refConExtendHdl);
			result = eWindowNotCreatedError;
		}
	}
	return result;
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× FW_GetNewDialog

OSErr  FW_GetNewDialog(WindowPtr *windowPtr,SInt16 windResourceID,WindowPtr behind,
											 ActivateHandlerUPP activateFunctionUPP,Boolean isFloater)
{
	OSErr								result;
	refConExtendHandle	refConExtendHdl;
	WindowPtr						newWindowPtr;
	SInt32							replacementRefCon;
	Boolean							isVisible;

	*windowPtr = NULL;

 	result = checkWindowOrdering(isFloater,&behind);
 	if(result != noErr)
 		return result;  

	refConExtendHdl = (refConExtendHandle) NewHandle(sizeof(refConExtend));
	
	if((result = MemError()) == noErr)
	{
		newWindowPtr = (WindowPtr)GetNewDialog(windResourceID,NULL,behind);

		if(newWindowPtr != NULL)
		{
			*windowPtr = newWindowPtr;

			replacementRefCon = GetWRefCon(newWindowPtr);
			SetWRefCon(newWindowPtr,(SInt32) refConExtendHdl);
			FW_SetWRefCon(newWindowPtr,replacementRefCon);

			setActivateHandler(newWindowPtr,activateFunctionUPP);

			if(isFloater)
			{
				((WindowPeek) newWindowPtr)->windowKind = kFloaterKind;

				isVisible = ((WindowPeek) newWindowPtr)->visible;

				if(isVisible)
				{
					if(!isFrontProcess())
					{
						ShowHide(newWindowPtr,false);
						setWasVisible(newWindowPtr,true);
					}	
				}
			}
		}
		else
		{
			DisposeHandle((Handle) refConExtendHdl);
			result = eWindowNotCreatedError;
		}
	}
	return result;
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× FW_DisposeWindow

void  FW_DisposeWindow(WindowPtr windowPtr)
{
	refConExtendHandle	refConExtendHdl;

	if(getIsWindowVisible(windowPtr))
		FW_HideWindow(windowPtr);

	refConExtendHdl = (refConExtendHandle) GetWRefCon(windowPtr);
	if(refConExtendHdl != NULL)
		DisposeHandle((Handle) refConExtendHdl);

	DisposeWindow(windowPtr);
}

// ×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× FW_SelectWindow

void  FW_SelectWindow(WindowPtr windowPtr)
{
	Boolean		frontProcess, isFloatingWindow;
	WindowPtr	currentFrontWindowPtr, lastFloatingWindowPtr;

	frontProcess = isFrontProcess();

	if(getWindowKind(windowPtr) == kFloaterKind) 
	{
		isFloatingWindow = true;
		currentFrontWindowPtr = FrontWindow();
	}
	else 
	{
		isFloatingWindow = false;
		currentFrontWindowPtr = FW_findFrontNonFloatWindow();
		lastFloatingWindowPtr = findLastFloatingWindow();
	}

	if(currentFrontWindowPtr != windowPtr) 
	{
		if(isFloatingWindow)
		{
			BringToFront(windowPtr);
		}
		else
		{
			if(frontProcess)
				deactivateWindow(currentFrontWindowPtr);

			if(lastFloatingWindowPtr == NULL)
			{
				BringToFront(windowPtr);
			}
			else
			{
				SendBehind(windowPtr,lastFloatingWindowPtr);
			}

			if(frontProcess)
				activateWindow(windowPtr);
		}
	}
}

// ×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× FW_HideWindow

void  FW_HideWindow(WindowPtr windowPtr)
{
	WindowPtr	frontFloatWindowPtr, frontNonFloatWindowPtr, windowBehind;

	if(getIsWindowVisible(windowPtr) == false)
		return;
	
	frontFloatWindowPtr = FrontWindow();
	if(getWindowKind(frontFloatWindowPtr) != kFloaterKind)
		frontFloatWindowPtr = NULL;

	frontNonFloatWindowPtr = FW_findFrontNonFloatWindow();
	
	ShowHide(windowPtr,false);
	
	if(windowPtr == frontFloatWindowPtr)
	{
		windowBehind = getNextVisibleWindow(windowPtr);

		if((windowBehind != NULL) && (getWindowKind(windowBehind) == kFloaterKind))
			BringToFront(windowBehind);
	}
	else if(windowPtr == frontNonFloatWindowPtr)
	{
		windowBehind = getNextVisibleWindow(windowPtr);
		
		if(windowBehind != NULL)
		{
			SendBehind(windowPtr,windowBehind);

			if(isFrontProcess())
				activateWindow(windowBehind);
		}
	}
}

// ×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× FW_ShowWindow

void  FW_ShowWindow(WindowPtr windowPtr)
{
	Boolean							frontProcess, needsActivate, highlightState, modalUp, showingModal;
	SInt16							windowKind;
	WindowPtr						windowBehind;
	WindowPtr						frontNonFloatWindowPtr;
	ActivateHandlerUPP	activateFunctionUPP;

	if(getIsWindowVisible(windowPtr) != false)
		return;

	frontProcess = isFrontProcess();

	windowKind = getWindowKind(windowPtr);

	modalUp = isWindowModal(FrontWindow());

	showingModal = isWindowModal(windowPtr);
	if(showingModal)
		BringToFront(windowPtr);

	FW_validateWindowList();

	if(!frontProcess && windowKind == kFloaterKind)
	{
		setWasVisible(windowPtr,true);
		return;
	}

	if(frontProcess)
	{
		if(showingModal)
		{
			highlightState = true;
			needsActivate = true;
		}
		else if(modalUp)
		{
			highlightState = false;
			needsActivate = false;
		}
		else
		{
			if(windowKind == kFloaterKind)
			{
				highlightState = true;
				needsActivate = true;
			}
			else
			{
				frontNonFloatWindowPtr = FW_findFrontNonFloatWindow();
				if(frontNonFloatWindowPtr == NULL)
				{
					highlightState = true;
					needsActivate = true;
				}
				else
				{
					highlightState = false;
					needsActivate = false;

					windowBehind = getNextWindow(windowPtr);
					while(windowBehind != NULL)
					{
						if(windowBehind == frontNonFloatWindowPtr)
						{
							deactivateWindow(frontNonFloatWindowPtr);
							highlightState = true;
							needsActivate = true;
							break;
						}
						windowBehind = getNextVisibleWindow(windowBehind);
					}
				}
			}
		}
	}
	else
	{
		highlightState = false;
		needsActivate = false;
	}

	HiliteWindow(windowPtr,highlightState);
	ShowHide(windowPtr,true);

	if(needsActivate) 
	{
		activateFunctionUPP = getActivateHandler(windowPtr);
		if(activateFunctionUPP != NULL)
			CallActivateHandlerProc(activateFunctionUPP,windowPtr,true);
	}
}

// ×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× FW_DragWindow

void  FW_DragWindow(WindowPtr windowPtr,Point startPoint,const Rect *draggingBounds)
{
	SInt16		topLimit, newHorizontalWindowPosition, newVerticalWindowPosition;
	Rect			dragRect;
	SInt16		horizontalOffset, verticalOffset;
	GrafPtr		savePort, windowManagerPort;
	KeyMap		keyMap;
	Boolean		commandKeyDown = false;
	RgnHandle	dragRegionHdl, windowContentRegionHdl;
	SInt32		dragResult;

	if(WaitMouseUp()) 
	{
		topLimit = LMGetMBarHeight() + 4;
		dragRect = *draggingBounds;
		if(dragRect.top < topLimit)
			dragRect.top = topLimit;
	
		GetPort(&savePort);
		GetWMgrPort(&windowManagerPort);
		SetPort(windowManagerPort);

		SetClip(GetGrayRgn());

		GetKeys(keyMap);
		if(keyMap[1] & 0x8000)
			commandKeyDown = true;
	
		if((commandKeyDown == true) || (getWindowKind(windowPtr) != kFloaterKind)) 
		{
			if(commandKeyDown == false)
				ClipAbove(FW_findFrontNonFloatWindow());
			else
				ClipAbove(windowPtr);
		}

		dragRegionHdl = NewRgn();
		CopyRgn(getStructureRegion(windowPtr),dragRegionHdl);
	
		dragResult = DragGrayRgn(dragRegionHdl,startPoint,&dragRect,&dragRect,noConstraint,
														 NULL);
		SetPort(savePort);

		if(dragResult != 0) 
		{
			horizontalOffset = dragResult & 0xFFFF;
			verticalOffset = dragResult >> 16;

			if(verticalOffset != -32768) 
			{
				windowContentRegionHdl = NewRgn();
				CopyRgn(getContentRegion(windowPtr),windowContentRegionHdl);
	
				newHorizontalWindowPosition = (**windowContentRegionHdl).rgnBBox.left + 
																			horizontalOffset;
				newVerticalWindowPosition = (**windowContentRegionHdl).rgnBBox.top + 
																		verticalOffset;
				MoveWindow(windowPtr,newHorizontalWindowPosition,newVerticalWindowPosition,
									 false);
									 
				DisposeRgn(windowContentRegionHdl);
			}
		}

		if(commandKeyDown == false)
			FW_SelectWindow(windowPtr);

		DisposeRgn(dragRegionHdl);
		DisposeRgn(windowContentRegionHdl);
	}
}

// ×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× FW_doSuspendEvent

void  FW_doSuspendEvent(void)
{
	WindowPtr	currentWindowPtr;
	Boolean		windowIsVisible;

	currentWindowPtr = LMGetWindowList();

	if(isWindowModal(FrontWindow()))
		return;

	while((currentWindowPtr != NULL) && 
				(getWindowKind(currentWindowPtr) == kFloaterKind))
	{
		windowIsVisible = getIsWindowVisible(currentWindowPtr);
		setWasVisible(currentWindowPtr,windowIsVisible);

		if(windowIsVisible)
			ShowHide(currentWindowPtr,false);
			
		currentWindowPtr = getNextWindow(currentWindowPtr);
	}

	currentWindowPtr = FW_findFrontNonFloatWindow();
	if(currentWindowPtr != NULL)
		deactivateWindow(currentWindowPtr);
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× FW_doResumeEvent

void  FW_doResumeEvent(void)
{
	WindowPtr	currentWindowPtr;
	Boolean		windowWasVisible;

	currentWindowPtr = LMGetWindowList();

	if(isWindowModal(FrontWindow()))
		return;
		
	while((currentWindowPtr != NULL) &&
				(getWindowKind(currentWindowPtr) == kFloaterKind))
	{
		windowWasVisible = getWasVisible(currentWindowPtr);
		if(windowWasVisible) 
		{
			ShowHide(currentWindowPtr,true);
			activateWindow(currentWindowPtr);
		}

		currentWindowPtr = getNextWindow(currentWindowPtr);
	}

	currentWindowPtr = FW_findFrontNonFloatWindow();
	if(currentWindowPtr != NULL)
		activateWindow(currentWindowPtr);
}

// ×××××××××××××××××××××××××××××××××××××××××××××××××××××× FW_activateFloatsAndFirstDocWin

void  FW_activateFloatsAndFirstDocWin(void)
{
	WindowPtr	firstWindowPtr, secondDocumentWindowPtr, currentWindowPtr;
	Boolean		frontProcess;

	if(isWindowModal(FrontWindow()))
		return;

	frontProcess = isFrontProcess();

	if(frontProcess == false)
		FW_doSuspendEvent();
	else 
	{
		firstWindowPtr = FrontWindow();
		secondDocumentWindowPtr = FW_findFrontNonFloatWindow();

		if(secondDocumentWindowPtr != NULL)
			secondDocumentWindowPtr = getNextWindow(secondDocumentWindowPtr);

		currentWindowPtr = firstWindowPtr;

		while(currentWindowPtr != secondDocumentWindowPtr) 
		{
			if(getIsWindowVisible(currentWindowPtr))
				activateWindow(currentWindowPtr);
			currentWindowPtr = getNextWindow(currentWindowPtr);
		}
	}
}

// ×××××××××××××××××××××××××××××××××××××××××××××××××××× FW_deactivateFloatsAndFirstDocWin

void  FW_deactivateFloatsAndFirstDocWin(void)
{
	WindowPtr	firstWindowPtr, secondDocumentWindowPtr, currentWindowPtr;

	FW_validateWindowList();

	firstWindowPtr = FrontWindow();
	secondDocumentWindowPtr = FW_findFrontNonFloatWindow();

	if(secondDocumentWindowPtr != NULL)
		secondDocumentWindowPtr = getNextWindow(secondDocumentWindowPtr);

	currentWindowPtr = firstWindowPtr;

	while(currentWindowPtr != secondDocumentWindowPtr) 
	{
		if(getIsWindowVisible(currentWindowPtr))
			deactivateWindow(currentWindowPtr);
		currentWindowPtr = getNextWindow(currentWindowPtr);
	}
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× FW_findFrontNonFloatWindow

WindowPtr  FW_findFrontNonFloatWindow(void)
{
	WindowPtr	windowPtr;

	windowPtr = FrontWindow();

	while((windowPtr != NULL) && (getWindowKind(windowPtr) == kFloaterKind)) 
	{
		do 
		{
			windowPtr = getNextWindow(windowPtr);
		} while((windowPtr != NULL) && (getIsWindowVisible(windowPtr) == false));
	}

	return windowPtr;
}

// ×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× FW_validateWindowList

void  FW_validateWindowList(void)
{
	WindowPtr	windowPtr;
	WindowPtr	lastFloatingWindowPtr;

	windowPtr = LMGetWindowList();
	lastFloatingWindowPtr = findLastFloatingWindow();

	if(lastFloatingWindowPtr == NULL)
		return;

	if(getWindowKind(windowPtr) == kFloaterKind)
		return;

	if(isWindowModal(windowPtr))
		return;

	bringFloatersToFront(windowPtr);
}

// ×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× FW_GetWRefCon

SInt32  FW_GetWRefCon(WindowPtr windowPtr)
{
	refConExtendHandle	refConExtendHdl;

	refConExtendHdl =(refConExtendHandle) GetWRefCon(windowPtr);
	if(refConExtendHdl != NULL)
		return((*refConExtendHdl)->replacementRefCon);
	else
		return 0;
}

// ×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× FW_SetWRefCon

void  FW_SetWRefCon(WindowPtr windowPtr,SInt32 refCon)
{
	refConExtendHandle	refConExtendHdl;

	refConExtendHdl =(refConExtendHandle) GetWRefCon(windowPtr);
	if(refConExtendHdl != NULL)
	{
		(*refConExtendHdl)->replacementRefCon = refCon;
	}
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× activateWindow

void  activateWindow(WindowPtr windowPtr)
{
	highlightAndActivateWindow(windowPtr,true);
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× deactivateWindow

void  deactivateWindow(WindowPtr windowPtr)
{
	highlightAndActivateWindow(windowPtr,false);
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× highlightAndActivateWindow

void  highlightAndActivateWindow(WindowPtr windowPtr,Boolean activate)
{
	ActivateHandlerUPP	activateFunctionUPP;

	if(windowPtr == NULL)
		return;

	activateFunctionUPP = getActivateHandler(windowPtr);

	HiliteWindow(windowPtr,activate);

	if(activateFunctionUPP != NULL)
		CallActivateHandlerProc(activateFunctionUPP,windowPtr,activate);
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× findLastFloatingWindow

WindowPtr  findLastFloatingWindow(void)
{
	WindowPtr	windowPtr;
	WindowPtr	lastFloatingWindowPtr;

	windowPtr = LMGetWindowList();

	lastFloatingWindowPtr = NULL;
	
	while(windowPtr != NULL) 
	{
		if(getWindowKind(windowPtr) == kFloaterKind)
			lastFloatingWindowPtr = windowPtr;

		windowPtr = getNextWindow(windowPtr);
	}

	return lastFloatingWindowPtr;
}

// ×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× checkWindowOrdering

OSErr  checkWindowOrdering(Boolean isFloater,WindowPtr *behind)
{
	OSErr			result = noErr;
	WindowPtr	lastFloaterPtr;
	
	if(isFloater)
	{
		if(((*behind == NULL) && (FW_findFrontNonFloatWindow() != NULL)) ||
			 ((*behind != (WindowPtr) -1) && (getWindowKind(*behind) != kFloaterKind))	)
		{
			result = eInvalidWindowOrderingError;
		}
	}
 	else
	{
		lastFloaterPtr = findLastFloatingWindow();
 
		if(lastFloaterPtr != NULL)
		{
			if(*behind == (WindowPtr) -1)
			{
				*behind = lastFloaterPtr;
			}
			else if(*behind != NULL && *behind != lastFloaterPtr &&
							(getWindowKind(*behind) == kFloaterKind))
			{
				result = eInvalidWindowOrderingError;
			}
		}
	}
	
 	return result;
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× getNextVisibleWindow

WindowPtr  getNextVisibleWindow(WindowPtr windowPtr)
{
	WindowPtr	nextWindowPtr;
	
	nextWindowPtr = getNextWindow(windowPtr);

	while(nextWindowPtr != NULL)
	{
		if(getIsWindowVisible(nextWindowPtr))
			break;
		nextWindowPtr = getNextWindow(nextWindowPtr);
	}

	return nextWindowPtr;
}

// ×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× getWasVisible

Boolean  getWasVisible(WindowPtr windowPtr)
{
	refConExtendHandle	refConExtendHdl;
	
	refConExtendHdl =(refConExtendHandle) GetWRefCon(windowPtr);

	if(refConExtendHdl != NULL)
		return((*refConExtendHdl)->wasVisible);
	else
		return false;
}

// ×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× setWasVisible

void  setWasVisible(WindowPtr windowPtr,Boolean wasVisible)
{
	refConExtendHandle	refConExtendHdl;

	refConExtendHdl = (refConExtendHandle) GetWRefCon(windowPtr);

	if(refConExtendHdl != NULL)
	{
		(*refConExtendHdl)->wasVisible = wasVisible;
		SetWRefCon(windowPtr,(SInt32) refConExtendHdl);
	}
}

// ×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× isWindowModal

Boolean  isWindowModal(WindowPtr windowPtr)
{
	SInt16	windowVariant;
	SInt16	windowKind;

	windowVariant = GetWVariant(windowPtr);
	windowKind = getWindowKind(windowPtr);

	if(windowKind == kDialogWindowKind && (windowVariant == 2 || windowVariant == 3))
		return true;
	else
		return false;
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× bringFloatersToFront

void  bringFloatersToFront(WindowPtr windowPtr)
{
	WindowPtr	thisFloaterPtr;

	while(windowPtr != NULL && getWindowKind(windowPtr) != kFloaterKind)
		windowPtr = getNextWindow(windowPtr);

	if(windowPtr == NULL)
		return;

	thisFloaterPtr = windowPtr;

	bringFloatersToFront(getNextWindow(windowPtr));

	BringToFront(thisFloaterPtr);
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× isFrontProcess

Boolean	isFrontProcess(void)
{
	ProcessSerialNumber	frontPSN, currentPSN;
	OSErr								getFrontProcessResult, getCurrentProcessResult;
	Boolean							isSameProcess = false;

	getFrontProcessResult = GetFrontProcess(&frontPSN);
	getCurrentProcessResult = GetCurrentProcess(&currentPSN);
	
	if((getFrontProcessResult == noErr) && (getCurrentProcessResult == noErr))
		SameProcess(&frontPSN,&currentPSN,&isSameProcess);

	return(isSameProcess);
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× getIsWindowVisible

Boolean  getIsWindowVisible(WindowPtr windowPtr)
{
	return(((WindowPeek) windowPtr)->visible);
}

// ×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× getNextWindow

WindowPtr  getNextWindow(WindowPtr windowPtr)
{
	return((WindowPtr) ((WindowPeek) windowPtr)->nextWindow);
}

// ×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× getWindowKind

SInt16  getWindowKind(WindowPtr windowPtr)
{
	return(((WindowPeek) windowPtr)->windowKind);
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× getStructureRegion

RgnHandle  getStructureRegion(WindowPtr windowPtr)
{
	return(((WindowPeek) windowPtr)->strucRgn);
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× getContentRegion

RgnHandle  getContentRegion(WindowPtr windowPtr)
{
	return(((WindowPeek) windowPtr)->contRgn);
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× getActivateHandler

ActivateHandlerUPP  getActivateHandler(WindowPtr windowPtr)
{
	refConExtendHandle	refConExtendHdl;

	refConExtendHdl =(refConExtendHandle) GetWRefCon(windowPtr);
	if(refConExtendHdl != NULL)
		return((*refConExtendHdl)->activateFunctionUPP);
	else
		return NULL;
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××× setActivateHandler

void  setActivateHandler(WindowPtr windowPtr,ActivateHandlerUPP activateFunctionUPP)
{
	refConExtendHandle	refConExtendHdl;

	refConExtendHdl =(refConExtendHandle) GetWRefCon(windowPtr);
	if(refConExtendHdl != NULL)
	{
		(*refConExtendHdl)->activateFunctionUPP = activateFunctionUPP;
		SetWRefCon(windowPtr,(SInt32) refConExtendHdl);
	}
}

// ××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××
