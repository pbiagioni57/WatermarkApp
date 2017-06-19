// WatermarkApp4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "D2DWrapper.h"
#include "ParseArgs.h"
#include "CustomError.h"

#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "D3D11")
#pragma comment(lib, "Dwrite")
using namespace std;
using namespace WatermarkComponent;
void ShowHelp(void);
void ShowAbout(void);

int main(int argc, char **argv)
{
	try
	{

		ParseArgs parser(argc, argv);
		// no parameters or requesting help
		if (!parser.haveTokens() || parser.cmdOptionExists("-h"))
		{
			ShowHelp();
			return 0;
		}

		// Check for about
		if (parser.cmdOptionExists("-a"))
		{
			ShowAbout();
			return 0;
		}

		// Get required parameters: -b backgroundfile -s splashfile
		if (!(parser.cmdOptionExists("-b") && parser.cmdOptionExists("-w")))
		{
			ShowHelp();
			return 0;
		}

		// Create an instance of the class
		D2DWrapper image;
		image.Initialize();

		// Create render target of background image
		string param = "-b";
		image.SetBitmapRenderTarget(parser.getCmdOption(param));

		// Call BeginDraw() - loads the background image
		image.BeginDraw();

		// Get opacity, then call DrawImage() - loads the splash screen
		float opacity = 0.5;
		if (parser.cmdOptionExists("-o"))
		{
			param = "-o";
			float val = stof(parser.getCmdOption("-o"));
			opacity = (((val >= 0.0) && val <= 1.0) ? val : 0.5);
		}
		param = "-w";
		image.DrawImage(parser.getCmdOption(param), /*Point startPoint,*/opacity);

		// Call EndDraw()
		image.EndDraw();

		// Call SaveBitmapToFile()
		const wchar_t* pszOutputImageFile = L"C:\\temp\\WaterMarked.png";// Parse command line parameters.
		image.SaveBitmapToFile(pszOutputImageFile);

		return 0;
	}
	catch (exception& e) {
		printf("Exception: %s\n", e.what());
	}
	catch (HRESULT hr) {
		_com_error err(hr);
		const wchar_t * wErrMsg = err.ErrorMessage();
		printf("Exception: %S\n", wErrMsg);
	}
	catch (...) {
		printf("Exception: unknown error\n");
	}
	
	return 0;
}

void ShowHelp()
{
	std::cerr << "\nUsage: WatermarkApp [-b backgroundImageName] [-w watermarkImageName] [-o 0.nn] [-a] [-h]\n\n "
		<< "\tNo args\t\tDisplay help. Show this help message.\n"
		<< "\t-b\t\tBackground image file. Provide full path and file name.\n"
		<< "\t-w\t\tWatermark image file. Provide full path and file name.\n"
		<< "\t-o 0.nn \tOpacity - Valid range: 0.00 - 1.00\n"
		<< "\t-a\t\tAbout. Display about information.\n"
		<< "\t-h\t\tHelp - Show this help message. Same as not typing any options.\n"
		<< std::endl;

 }
void ShowAbout()
{
	std::cerr << "\n\tWatermarkApp.exe\n "
		<< "\tAuthor: Paul Biagioni\n"
		<< "\tVersion 1.0\n"
		<< std::endl;

}