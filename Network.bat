cd %~dp0

xcopy Network\Inc\*.h _Network\Inc\ /d /s /y /exclude:ignore.txt
xcopy Network\Inc\*.inl _Network\Inc\ /d /s /y /exclude:ignore.txt
xcopy Network\Bin\*.* Engine\Bin\ /d /s /y /exclude:ignore.txt
