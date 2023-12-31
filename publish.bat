mkdir dist
xcopy /s/y .\bin\cleo.exe .\dist\cleo.exe && butler push .\dist etmm/cleo:win
RD /S /Q .\dist
