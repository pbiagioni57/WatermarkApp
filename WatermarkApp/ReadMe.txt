-----------------------------------README.txt----------------------------------

Using the Image Watermarking program "WatermarkApp.exe"


INSTALLATION:
------------
1. Copy the file "watermarkapp.exe" to folder where user has read/execute
   permission.
2. Output file "watermarked.png" is written to "C:\temp". Create this folder
   and ensure that user has write permission.


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


NOTES:
-----
  1. Help message returned if either image file is missing from command line.
  2. Error message is returned if either image file cannot be opened.
  2. All run-time errors are caught and error description returned to standard out.
  3. All unknown errors are caught and returned with "Unknown error" message.