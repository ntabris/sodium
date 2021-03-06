#ifdef powerc
typedef	long double		xvar;
#else
typedef	extended80		xvar;
#endif


void	main(void);
void	TitleScreen(void);

void	HandleError(int type);
Boolean	ValidPos(Rect rect);
Rect	GetWinPosRect(WindowPtr window);
void	CenterRect(Rect *newRect, Rect larger);
void	AppendString(Str255 a, Str255 b);

void	ToolBoxInit(void);
void	BlackWhite(void);
int		GestaltAppear(void);
int		GestaltAppleEvent(void);
void	Setup(void);
void	SetupTableRects(void);
void	MenuBarInit(void);
void	DragRectInit(void);
void	TableInit(void);

void	PictureOpen(void);
void	DataOpen(void);
void	IsotopesOpen(void);
short	OpenDialog(OSType type);
void	UsePicts(void);
void	UseData(void);
void	UseIsotopes(int x);

pascal	Boolean	EventFilter(DialogPtr dialogPtr,EventRecord *eventStrucPtr,SInt16 *itemHit);
OSErr	AEOpenAppEvent(AppleEvent *appleEvent, AppleEvent *reply, long handlerRefcon);
OSErr	AEOpenDocEvent(AppleEvent *appleEvent, AppleEvent *reply, long handlerRefcon);
OSErr	AEPrintDocEvent(AppleEvent *appleEvent, AppleEvent *reply, long handlerRefcon);
OSErr	AEQuitEvent(AppleEvent *appleEvent, AppleEvent *reply, long handlerRefcon);
void	HandleEvents(void);
void	HighLevelEvents(void);
int		HandleKey(void);
void	HandleMouse(void);
void	HandleContent(WindowRef whichWindow);
void	HandleMouseUp(void);
void	UpdateWindow(WindowRef whichWindow);
void	HandleMenu(long int menuChoice);
void	HandleApple(int item);
void	HandleFile(int item);
void	HandleEdit(int item);
void	HandleSpecial(int item);
void	HandleListMenu(int item);
void	CloseWin(WindowRef whichWindow, Point whereClose);
void	HandleTableDown(Point gclick);
Boolean	HandleTableUp(Point gclick);
void	HandleGraphClick(Point gclick);
void	HandleListClick(Point gclick);
void	AboutSodium(void);
void	CloseWinM(WindowRef whichWindow);
void	HandleDisk(void);
void	HandleElementClick(Point where);
int		ElementClick(Point where);
void	HandleElementDlog(int item);
void	HandleCalculator(int item);
void	CorrectMenus(void);
void	CorrectControls(void);

void	Element(int e);
void	CopyBitsElement(void);
void	E3d(int e);
void	LoadText(int e);
void	LoadQShell(int e);
void	ConvertStringCFK(Str255 String1);
void	ConvertLowStringCFK(Str255 String1);
void	DrawElemText(int e);
void	DrawQShells(void);
void	LewisDots(int e, int h, int v);
void	Dot(int h, int v);
void	DrawElemPict(int e);
void	ClearElement(void);

pascal	void	ScrollProc(ControlHandle theControl, short theCode);
pascal	void	LiveScrollProc(ControlHandle theControl, short theCode);
void	Isotope(int e);
void	ClearIsotope(void);
void	IsotopeScrolled(void);
int		Lines(void);
void	LoadIsotopes(int e);
void	DrawIsotopeText(int e, int x, int y);

void	PreferencesDialog(void);
void	GetPrefs(void);
short	CopyRes(ResType resType, int resID, int sourceFileRefNum, int destFileRefNum);
void	SavePreferences(void);

void	GraphDialog(void);
void	Graph(void);
void	GraphLine(int id, RGBColor color);

void	SearchDialog(void);
int		SearchName(Str255 str);

void	HandlePrint(void);
void	PrintPage(int what, TPPrPort port);
pascal	TPPrDlg	PrintInitFunct(THPrint hPrint);
void	AppendTheDITL(TPPrDlg theDialog);
pascal	PrintItemEvaluationFunct(TPPrDlg theDialog, short itemHit);
pascal	Boolean	PrintEventFilter(DialogPtr dialogPtr, EventRecord *EventPtr, SInt16 *itemHit);
void	Print(int what);
void	PrintElementText(void);
void	PrintTableText(int height);

void	HandleCopy(int item);
void	AppendLabel(Str255 a, int item);
void	CopyText(Str255 *String, int item);
void	CopyPict(PicHandle pict);
void	Copy(Handle scrapHdl, ResType type, Size dataLength);

void	Gradient(void);
void	GradientName(Str255 string);
void	DrawRow(int e, Str255 string);
void	GradientTable(int id);
void	SmallerNumber(Str255 String1);
void	LewisTable(void);

void	CalculatorWindow(void);
void	Calculator(Str255 formulaString, Str255 *answerString);
void	CalcGetSymbol(Str255 string, int *index, xvar *amu);
void	CalcGetNumber(Str255 string, int *index, int *number);

void	ConvertToLD(int id, ResType type);
void	StringToDouble(Str255 String, long double *Double);
void	StringToX(Str255 String, extended80 *Number);
void	WriteNumber(int e, ResType type, long double *data);
void	GetNumber(int e, ResType type, xvar *number);

void	SetupList(void);
void	SaveList(void);
int		LoadList(void);
void	CompareWindow(void);
void	CompareList(void);
void	UpdateCompare(void);
void	AddElement(int e);
void	RemoveElement(int item);
void	SortList(void);
int		SortCompare(int i);
void	ShrinkCompareWindow(void);


void	Quit(void);


Point	ItTable(int x);
int		TableIt(int period, int row);


int		GetTempPrefs(SInt16 fileRN);
int 	GetHilitePrefs(SInt16 fileRN);
int		GetGraphPrefs(SInt16 fileRN);
int 	GetWindPosPrefs(SInt16 fileRN);
int 	GetTablePrefs(SInt16 fileRN);
int		GetWindSizePrefs(SInt16 fileRN);
int		GetPrintingPrefs(SInt16 fileRN);
int		GetListPrefs(SInt16 fileRN);
int		GetCopyLabelsPrefs(SInt16 fileRN);


void	SaveTempPrefs(void);
void	SaveHilitePrefs(void);
void	SaveGraphPrefs(void);
void	SaveWindPosPrefs(void);
void	SaveTablePrefs(void);
void	SaveWindSizePrefs(void);
void	SavePrintingPrefs(void);
void	SaveListPrefs(void);
void	SaveCopyLabelsPrefs(void);