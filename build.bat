call "%VS120COMNTOOLS%vsvars32.bat"

del gm82dx8.dll

cl gm82dx8.c /O2 /W2 /WX /GS- /nologo /link /nologo /dll /out:gm82dx8.dll

if not exist gm82dx8.dll pause && exit

del *.obj
del *.exp
del *.lib

build_gex.py gm82dx8.gej

pause