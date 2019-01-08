cd %~dp0

xcopy Common\Inc\*.h _Common\Inc\ /d /s /y /exclude:ignore.txt
xcopy Common\Inc\*.inl _Common\Inc\ /d /s /y /exclude:ignore.txt
xcopy Common\Bin\*.* Network\Bin\ /d /s /y /exclude:ignore.txt
