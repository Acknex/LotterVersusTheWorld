@echo off
echo in case of error adjust path to your acknex8 installation

rem adjust path to engine
Set "AckPath=d:\apps\acknex8" 
Set "AckPath2=c:\Program Files (x86)\gstudio" 


Set "path=%path%;%AckPath%;%AckPath2%"

del lottivstheworld.cd\*
wed -r lottivstheworld.c
xcopy media lottivstheworld.cd\media /Y
xcopy level\template*.dat lottivstheworld.cd\level\ /Y
del lottivstheworld.exe

pause