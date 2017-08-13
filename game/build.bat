@echo off
echo in case of error adjust path to your acknex8 installation

rem adjust path to engine
Set "AckPath=d:\apps\acknex8" 
Set "AckPath2=c:\Program Files (x86)\gstudio" 


Set "path=%path%;%AckPath%;%AckPath2%"

rd lottivstheworld.cd /S /Q
ren media media_renamed_by_buildscript
timeout /t 1 /nobreak > NUL
wed -r lottivstheworld.c > build.log
ren media_renamed_by_buildscript media

rem specific files to be copied
xcopy media lottivstheworld.cd\media\ /Y
xcopy level\template*.dat lottivstheworld.cd\level\ /Y
xcopy readme.txt lottivstheworld.cd /Y
xcopy credits.dat lottivstheworld.cd /Y

rem cleanup
del lottivstheworld.exe

pause