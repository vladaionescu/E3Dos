@ECHO OFF

D:\Watcom\Sources\3D\OTime.exe
ECHO --------------------------------------------------------

D:\Watcom\binnt\wmake.exe -f D:\Watcom\Sources\3D\3d.mk -h -e
IF ERRORLEVEL = 1 GOTO EXITFAIL

: EXITSUCCESS
ECHO Success!
GOTO EXIT

: EXITFAIL
ECHO Failed!
GOTO EXIT


: EXIT

ECHO --------------------------------------------------------
D:\Watcom\Sources\3D\OTime.exe
