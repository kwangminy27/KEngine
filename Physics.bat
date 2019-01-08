cd %~dp0

xcopy Physics\Inc\*.h _Physics\Inc\ /d /s /y /exclude:ignore.txt
xcopy Physics\Inc\*.inl _Physics\Inc\ /d /s /y /exclude:ignore.txt
xcopy Physics\Bin\*.* Engine\Bin\ /d /s /y /exclude:ignore.txt
