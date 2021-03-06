#include "ExternGlobals.h"
#include "FuncDefs.h"
#include <fp.h>

void CalculatorWindow()
{
	if(gCalcWindow == 0L)
	{
		UseData();
		if((gCalcWindow = GetNewDialog(132, 0L, (WindowRef)-1)) == 0L)
			HandleError(24);
		HNoPurge((Handle)gCalcWindow);
		gCalcOpen = 1;
	
		SetDialogDefaultItem(gCalcWindow, 1);
	}
	ShowWindow(gCalcWindow);
	SelectWindow(gCalcWindow);
	
	//*DrawDialog(gCalcWindow);

	CorrectMenus();
	CorrectControls();
}

void Calculator(Str255 formulaString, Str255 *answerString)
{
	int		index, sub;
	int		pre;
	xvar	workingAMU, totalAMU;

	index = 1;
	workingAMU = totalAMU = 0;

startcalc:
	CalcGetNumber(formulaString, &index, &pre);
	if(pre == 0)
		pre = 1;
	
	while(index <= StrLength(formulaString))
	{
		workingAMU = 0;
		sub = 1;
		
		CalcGetSymbol(formulaString, &index, &workingAMU);
		CalcGetNumber(formulaString, &index, &sub);
		
		totalAMU += (workingAMU * sub) * (pre);
		
		if(formulaString[index] == ' ')
			index++;
		if(formulaString[index] == '�')
		{
			do
				index++;
			while (formulaString[index] == ' ');
			goto startcalc;
		}
	}
	
	if(totalAMU > 0)
	{	
		extended80			number;
		NumFormatStringRec	nfRec;
		Handle				itlHandle;
		long				offset, length;
		
		workingAMU = 0;
		
#ifdef powerc
		ldtox80(&totalAMU, &number);
#else	
		number = totalAMU;
#endif
		
		GetIntlResourceTable(smCurrentScript, smNumberPartsTable, &itlHandle, &offset, &length);
		StringToFormatRec("\p#######.#####;-#######.#####;0", (NumberParts *)(*itlHandle + offset), &nfRec);
		ExtendedToString(&number, &nfRec, (NumberParts *)(*itlHandle + offset), *answerString);
	}
	else
		*answerString[0] = 0;
}

void CalcGetSymbol(Str255 string, int *index, xvar *amu)
{
	unsigned char	currentChar, working[15];
	int				i, e;
	static int		parren;
#ifdef powerc
#else
	Str255				NumberString;
	extended80			number;
	NumFormatStringRec	nfRec;
	Handle				itlHandle;
	long				offset, length;
#endif
	
	
	i = 0;
	currentChar = *(string+(*index)+i);
	
	if(currentChar == ' ' || currentChar == '�')
	{
		(*index)++;
		return;
	}
	
	if(currentChar == '(')
	{
		xvar	inAmu;
		int		sub;
		
		parren++;
		(*index)++;
		
		currentChar = *(string+(*index)+i);
		while(currentChar != ')' && *index <= StrLength(string))
		{
			CalcGetSymbol(string, index, &inAmu);
			CalcGetNumber(string, index, &sub);
			*amu += inAmu * sub;
			currentChar = *(string+(*index)+i);
		}
		(*index)++;
		return;
	}
	
	if(currentChar < 'A' || currentChar > 'Z')
	{
		amu = 0;
		(*index) += i;
	}
	working[i+1] = currentChar;
	
	i++;
	currentChar = *(string+(*index)+i);
	while(currentChar >= 'a' && currentChar <= 'z' && (*index)+i <= StrLength(string))
	{
		working[i+1] = currentChar;
		i++;
		currentChar =  *(string+(*index)+i);
	}
	working[0] = i;
	
	if(i > 0)
	{
		e = SearchName(working);
		if(e > 0)
		{
#ifdef powerc
			GetNumber(e, kTypeWeight, amu);
#else
			GetIndString(NumberString, kResWeight, e);
			
			GetIntlResourceTable(smCurrentScript, smNumberPartsTable, &itlHandle, &offset, &length);
			StringToFormatRec("\p#######.#####;-#######.#####;0", (NumberParts *)(*itlHandle + offset), &nfRec);
			StringToExtended(NumberString, &nfRec, (NumberParts *)(*itlHandle + offset), amu);
#endif
		}
		else
		{
			ParamText(working, "\pRemember to use correct capitalization.", "\p", "\p");
			NoteAlert(140, 0L);
		}
	}
		
	(*index) += i;
}

void CalcGetNumber(Str255 string, int *index, int *number)
{
	unsigned char	currentChar, working[15];
	int				i, x;
	
	i = 0;
	currentChar =  *(string+(*index)+i);
	while(currentChar >= '0' && currentChar <= '9' && (i+(*index) <= StrLength(string)))
	{
		working[i+1] = currentChar;
		i++;
		currentChar =  *(string+(*index)+i);
	}
	if(i > 0)
	{
		working[0] = i;

		StringToNum(working, (long *)&x);
		*number = x;
		
		(*index) += i;
	}
	else
	{
		*number = 1;
	}	
}