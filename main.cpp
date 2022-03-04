#include "defs.h"
#include "doom.cpp"

//read data from input file to corresponding variables
void readFile (FILE* f, int& N, eventList*& pEvent)
{
	char c = fgetc(f);
	int minus;
	long num;
	int firstLine = 1;

	eventList* tail = NULL;

	while (c != -1 && !feof(f))
	{
		minus = 0;

		while ((c < '0' || c > '9') && c != -1)
		{
			if (c == '-') minus = 1;
			c = fgetc(f);
			
		}

		if (c == -1) break;

		num = c - '0';
		c = fgetc(f);

		while (c >= '0' && c <= '9')
		{
			num = num * 10 + c - '0';
			c = fgetc(f);
		}

		if (minus) num = -num;

		if (firstLine != 1) // the second line contains events
		{
			eventList* current_event = new eventList();
			current_event->nEventCode = num;
			current_event->pNext = NULL;
			
			if (pEvent == NULL)
				pEvent = current_event;
			
			if (tail != NULL)
				tail->pNext = current_event;
			tail = current_event;
		}
		else  // the first line contains knight's HP
		{
			N = num; // HP
			firstLine = 0;
		}
	}
}

//------- display -------------
void display(int N, eventList* pEvent, int result)
{
	if (N == 777 || N == -777)
	{ 
		if (checkSCC (N, pEvent))
			printf ("Eppur Si Muove\n");
	}
	else
	{
		printf ("%d\n", result);
	}
}

//-------- main program --------
int main()
{
	eventList* pEvent = NULL;
	int N = 0, result = 0;

	// initialize the input file
	char* file_name = "input.txt";
	FILE* f = fopen(file_name, "r");
	if (f == NULL)	//file not found or cannot read
	{
		printf("File not found \n");
		return 0;
	}

	readFile(f, N, pEvent);
	result = travel(N, pEvent);
	display(N, pEvent, result);

	// free event list memory
	eventList* p;
	while (pEvent != NULL)
	{
		p = pEvent;
		pEvent = pEvent->pNext;
		delete p;
		p = NULL;
	}
   
   return 0;
}
