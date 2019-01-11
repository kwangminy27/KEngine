cd %~dp0

xcopy Utility\Inc\*.h _Utility\Inc\ /d /s /y /exclude:ignore.txt
xcopy Utility\Inc\*.inl _Utility\Inc\ /d /s /y /exclude:ignore.txt
xcopy Utility\Bin\*.* Engine\Bin\ /d /s /y /exclude:ignore.txt
