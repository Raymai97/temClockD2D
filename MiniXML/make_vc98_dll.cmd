@echo off
call "D:\vc60\vcvar.cmd"
set include=%cd%\src\vcnet;%include%
cl -nologo ^
	-MD -LD -DWIN32 ^
	src/mxml-attr.c ^
	src/mxml-entity.c ^
	src/mxml-file.c ^
	src/mxml-get.c ^
	src/mxml-index.c ^
	src/mxml-node.c ^
	src/mxml-private.c ^
	src/mxml-search.c ^
	src/mxml-set.c ^
	src/mxml-string.c ^
	-link ^
	-opt:nowin98 ^
	-def:src/vcnet/mxml1.def ^
	-implib:mxml1.lib ^
	-out:mxml1.dll || pause
del /q *.obj
exit/b
