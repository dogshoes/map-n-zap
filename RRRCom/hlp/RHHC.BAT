@ECHO ีออออออออออออออออออออออออออออออออออออออออออธ
@ECHO ณ                                          ณ
@ECHO ณ  R o b o   H E L P                  ฐ ฐ  ณ
@ECHO ณ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ              ฐ ฐ ฐ ฐ ณ
@ECHO ณ  Make Help Project              ฐ ฐฐฐ ฐ ฐณ
@ECHO ณ                                  ฐ ฐฐฐ  ฐณ
@ECHO ณ                                 ฐ ฐ  ฐฐ  ณ
@ECHO ณ                                  ฐ ฐ  ฐฐ ณ
@ECHO ณ                                       ฐฐฐณ
@ECHO ณ B L U E   S K Y   S O F T W A R E      ฐฐณ
@ECHO ณ                                        ฐฐณ
@ECHO ณฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฐฐฐณ
@ECHO ณฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฐฐฐฐฐณ
@ECHO ณฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฐฐฐฐฐฐฐฐฐฐฐณ
@ECHO ณฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฐฐฐฐฐฐฐฐฐฐฐฐฐฐฐฐฐฐฐณ
@ECHO ิออออออออออออออออออออออออออออออออออออออออออพ
@ECHO Batch File: d:\rrr\rrrcom\hlp\RHHC.BAT
@ECHO Project File: d:\rrr\rrrcom\hlp\cyecom.hpj
del cyecom.err

if (%TMP%)==() set TMP=%TEMP%
if (%TMP%)==() set TMP=C:\

D:\WINHELP\ROBOHELP\HCP.EXE cyecom.hpj
md ".\WinHelp3"
copy "cyecom.hlp" ".\WinHelp3"


