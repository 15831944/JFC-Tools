echo
pause
dir
set path = %path%
FOR /D /R %%X IN (at_r_debug) DO RD /S /Q "%%X"
FOR /D /R %%X IN (debug) DO RD /S /Q "%%X"
FOR /D /R %%X IN (bin) DO RD /S /Q "%%X"
FOR /D /R %%X IN (obj) DO RD /S /Q "%%X"
FOR /D /R %%X IN (release) DO RD /S /Q "%%X"
pause
