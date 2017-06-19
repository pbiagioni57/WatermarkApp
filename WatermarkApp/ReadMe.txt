-----------------------------------README.txt----------------------------------

Using the Image Watermarking program "WatermarkApp.exe"

RETRIEVING THE FILES:
---------------------
The readme, project, source, and binary files are located on GitHub under the Public
repository "pbiagioni9694/WatermarkApp".
 

INSTALLATION:
------------
1. Copy the program "WatermarkApp\x64\Release\watermarkapp.exe" to a folder where user
   has read/execute permission.
2. Copy the test files "WatermarkApp\pizza.png" and "WatermarkApp\watermark.tif"
   to a folder where the user has read permission.
3. The Output file "watermarked.png" is written to "C:\temp". Create this
   folder and ensure that the user has write permission.


COMMAND LINE SWITCHES:
---------------------
  Required switches
    -b <backgroundImageFilename.png>
    -w <watermarkImageFilename.tif>

  Optional switches
    -o <0.nn>    Opacity where valid range is: 0.00 - 1.00.
                 Default value is 0.5
    -a           About information displayed

  Help:
    No args      Displays help
    -h           Displays help  


EXAMPLE OF USAGE:
-----------------
  $watermarkapp.exe -b c:\temp\backgroundfile.png -w c:\temp\watermarkfile.tif -o 0.3


PLATFORM CONFIGURATION:
----------------------
Operating System:
  - Windows 10 Home, x64
Development Platform:
  - Visual Studio 2017 Community V15.2,
  - .NET Framework 4.6.01586,
  - Visual C++ 2017 


DEVELOPMENT:
-----------
This project is comprised of original code, refactored Microsoft code snipets
for use of WIC and Direct2D, and other refactored code snipets from the 
Microsoft Visual Studio community and developer network.
 
The project has been developed to meet most of the requirements and some of the
bonus features. There are sections of code marked "Todo" where development is not
complete due to lack of time. There are also cases where the most time efficient
method was implemented instead of the most elegant again due to time constraints.

There are several classes defined:
  ParseArgs - used for parsing command line arguements into a vector and retrieving parsed
    values by token (i.e., command line switch).
  D2DWrapper - The class exposes the WIC COM object for creating a WIC Factory and accessing
    other methods. It also exposes the Direct2D interfaces for image manipulation.
  CustomError - Provides custom error message handling.

Other files:
  DirectXSample.h - error handling helper functions
  WatermarkApp.cpp - main application


NOTES:
-----
The implemented Error handling is by no means comprehensive. Error handling was
implemented as a demonstration of concept and covers some of the obvious user
errors.

  1. Help message is returned if either image file is missing from command line.
  2. Error message is returned if either image file cannot be opened.
  2. All run-time errors are caught and error description returned to standard out.
  3. All unknown errors are caught and returned with "Unknown error" message.


Thank you for reviewing this document and the associated application and source code.
I'm hoping to have the opportunity to answer any questions and entertain suggestions
as to how the implementation of this project could be improved.

Sincerley,
Paul

