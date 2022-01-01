call "%VS120COMNTOOLS%vsvars32.bat"

cl gm82dx8.c /O2 /GS- /nologo /link /nologo /dll /out:..\gm82dx8.dll
del ..\gm82dx8.exp
del ..\gm82dx8.lib
del gm82dx8.obj

pause