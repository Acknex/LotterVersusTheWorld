@echo off
echo in case of error adjust path to acknex8

rem adjust path to engine
Set "AckPath=d:\apps\acknex8" 


Set "path=%path%;%AckPath%" 
del lottivstheworld.cd\*
wed -r lottivstheworld.c
xcopy media lottivstheworld.cd\media /Y
xcopy level\template*.dat lottivstheworld.cd\level\ /Y
del lottivstheworld.exe

pause