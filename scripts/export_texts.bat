python localization.py ../data/texts/texts.xls

xcopy "*.str" "../project/race3d/data/text" /E /F /Y /I
xcopy "*.chr" "../project/race3d/data/text" /E /F /Y /I
xcopy "*.str" "../project/race3d/data-ram/data-gles1/text" /E /F /Y /I

del *.str
del *.chr