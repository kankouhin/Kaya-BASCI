#include "core.h"
#include <stdlib.h>
#include <locale.h>
#include <iostream>

#ifdef __BPPWIN__
	#include <windows.h>
#endif

#include <QtWidgets/QApplication>

using namespace bpp;

extern "C" void abort_with_error(const string& s)
{
	std::cout << "Error: " << s << std::endl;
	while (true)
	{
		static bool first = true;
		int line = dbg_getline();
		string func = dbg_getfunc();
		if (!line)
			break;
		if (first)
		{
			std::cout << "    raised";
			first = false;
		}
		else
			std::cout << string(",\r\n    called");
		std::cout << string(" by ") << func << string("() at line #") << line;
	}
	
	std::cout << string("\r\n");	
	exit(0);
}

namespace bpp {
	namespace System {
		extern bpp::array<string> Command;
	}
}

int main(int argc, char* argv[])
{
	#ifdef __BPPWIN__
		CoInitialize(NULL);
	#endif
	
	//setlocale(LC_ALL, "");
	QApplication app(argc, argv);
	
	bpp::System::Command.redim(0, argc -1 );
	for ( int i = 0; i < argc; i++ ) {
		bpp::System::Command(i) = argv[i];
	}

	try
	{
		__init_all();
		Main();
		__final_all();
	}
	catch (const string& s)
	{
		abort_with_error(s);
	}
	catch (...)
	{
		abort_with_error("");
	}
	
	auto ret = app.exec();
	
	#ifdef __BPPWIN__
		CoUninitialize();
	#endif
	
	return ret;
}
