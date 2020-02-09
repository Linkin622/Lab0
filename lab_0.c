//Joshua Hanna, Lab0, Comp 322
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


typedef	unsigned char BYTE;
const	char*	SPECIAL_CHARACTERS[] = { "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL",  "FF", "CR", "SO"
,"BS", "HT", "LF", "VT" "SI","DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB", "CAN","EM", "SUB", "ESC","RS", "US", "DEL", "FS", "GS" };

BYTE	inputBuffer[7],
outputASCII[4],
outputParity[5],
outputNumerical;
int pFile;

void CheckPar()	
{
	int	countOnes = 0,
		currentBinaryDigit = 0,
		bitParity = outputNumerical & 0x80,
		i = 0;

	outputNumerical = (BYTE)strtol(inputBuffer, NULL, 2);

	for (i = 0, currentBinaryDigit = 1; i < 8; i++, currentBinaryDigit *= 2)
	{
		if ((outputNumerical & currentBinaryDigit) == currentBinaryDigit)
			countOnes++;
	}

	if (bitParity == 0x80)		
	{
		if ((countOnes % 2) == 0)	
		{
			strncpy(outputParity, "EVEN\0", 5);
			
		}
		else
		{
			strncpy(outputParity, "ODD\0", 4);
			
		}
	}
	else	
	{
		if ((countOnes % 2) == 0)	
		{
			strncpy(outputParity, "EVEN\0", 5);
			
		}
		else
		{
			strncpy(outputParity, "ODD\0", 4);
			
		}
	}
}

void GetAscii()
{
	int i = 0;
	for (i = 0; i < 3; i++)
		outputASCII[i] = NULL;

	outputNumerical = (BYTE)(strtol(inputBuffer, NULL, 2) & 0x7F);
	if (outputParity == "EVEN" && inputBuffer[0] == '1')
		outputNumerical++;

	if ((outputNumerical > 0x1F) && (outputNumerical != 0x7F))
		outputASCII[0] = outputNumerical;
	else if (outputNumerical == 0x7F)
		strncpy(outputASCII, SPECIAL_CHARACTERS[20], sizeof(SPECIAL_CHARACTERS[20]));
	else
		strncpy(outputASCII, SPECIAL_CHARACTERS[outputNumerical], sizeof(SPECIAL_CHARACTERS[outputNumerical]));
}

void ProcessChar()
{
	CheckPar();
	GetAscii();
	printf("%8s\t%5s\t\t%5d\t\t%5s\t\n", inputBuffer, outputASCII, outputNumerical, outputParity);
}

void ParseInput()
{
	int		charPos = 0,
		bytesRead = 0;
	BYTE	tempBuffer[1] = {NULL};
	do
	{
		bytesRead = read(pFile, tempBuffer,1);
		inputBuffer[charPos] = tempBuffer[0];
		
		if (inputBuffer[charPos] == '1' || inputBuffer[charPos] == '0')
		{
			if (charPos == 7)
			{
				charPos = -1;
				ProcessChar();
			}
			charPos += bytesRead;
		}
	} while (bytesRead != 0);

	if ((charPos > 0) && (charPos < 8))	
	{
		for (; charPos < 8; charPos++)
			inputBuffer[charPos] = '0';

		ProcessChar();
	}

	close(pFile);
}

int	main(int argc, char** argv)
{
	int i = 0;

	printf("Original\t  ASCII  \tDecimal\t\t Parity\t\n");
	printf("--------\t--------\t--------\t--------\t\n");

	if (argc == 1)	
	{
		pFile = stdin;
		ParseInput();
	}
	else
	{
		for (i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "-") == 0)		
			{
				pFile = stdin;
				ParseInput();
			}
			else
			{
				if (access(argv[i], 0) != -1)			
				{
					pFile = open(argv[i], 0);
					ParseInput();
				}
			}
		}
	}
	return 0;
}