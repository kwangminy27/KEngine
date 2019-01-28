cd %~dp0

xcopy Common\Inc\*.h _Common\Inc\ /d /s /y /exclude:ignore.txt
xcopy Common\Inc\*.inl _Common\Inc\ /d /s /y /exclude:ignore.txt
xcopy Common\Bin\*.* Network\Bin\ /d /s /y /exclude:ignore.txt

xcopy Network\Inc\*.h _Network\Inc\ /d /s /y /exclude:ignore.txt
xcopy Network\Inc\*.inl _Network\Inc\ /d /s /y /exclude:ignore.txt
xcopy Network\Bin\*.* Engine\Bin\ /d /s /y /exclude:ignore.txt

xcopy Physics\Inc\*.h _Physics\Inc\ /d /s /y /exclude:ignore.txt
xcopy Physics\Inc\*.inl _Physics\Inc\ /d /s /y /exclude:ignore.txt
xcopy Physics\Bin\*.* Engine\Bin\ /d /s /y /exclude:ignore.txt

xcopy Utility\Inc\*.h _Utility\Inc\ /d /s /y /exclude:ignore.txt
xcopy Utility\Inc\*.inl _Utility\Inc\ /d /s /y /exclude:ignore.txt
xcopy Utility\Bin\*.* Engine\Bin\ /d /s /y /exclude:ignore.txt

xcopy Engine\Inc\*.h _Engine\Inc\ /d /s /y /exclude:ignore.txt
xcopy Engine\Inc\*.inl _Engine\Inc\ /d /s /y /exclude:ignore.txt
xcopy Engine\Bin\*.* Client\Bin\ /d /s /y /exclude:ignore.txt
