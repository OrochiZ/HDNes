#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#pragma once
#include "common.h"
#include <wx\wx.h>

class hdnesPackEditormainForm;

class main : public wxApp{
public:
    static hdnesPackEditormainForm* mForm;
    static string exeDir;
    static string intToHex(int value);
    static string intToStr(int value);
    static vector<string> split(const string &s, char delim, vector<string> &elems);
    static void hexToByteArray(const string &s, UINT8* ar);

	main(void);
	~main(void);
    bool OnInit();
};

#endif // MAIN_H_INCLUDED


