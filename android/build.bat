setlocal

:: First make sure you have the Android NDK https://developer.android.com/tools/sdk/ndk/index.html installed in your root folder
:: Then make sure you have the Android SDK http://developer.android.com/sdk/index.html installed in your root folder
:: Then run the SDK manager and make sure you have API version 18 installed
:: Then install the latest JDK and make sure JAVA_HOME below points to it
:: Then download ant http://ant.apache.org/bindownload.cgi and copy it into the Android SDK folder (next to the eclipse and sdk folders)
:: Then download the SDL source code https://www.libsdl.org/download-2.0.php and place it in the jni folder, folder renamed to "SDL"
:: Then download the SDL_image source code https://www.libsdl.org/projects/SDL_image/ and place it in the jni folder, no rename necessary
:: Then find and install the ADB driver for your device (or install this: https://plus.google.com/103583939320326217147/posts/BQ5iYJEaaEH) and enable debugging on the device from your PC
:: Then if you're building for release mode make sure you sign the package with the right key: http://developer.android.com/tools/publishing/app-signing.html
:: Then run this batch script.
:: If it asks you something like "File or directory?" the answer is D

set JAVA_HOME=\Program Files\Java\jdk1.8.0_11

call \android-ndk-r10\ndk-build NDK_DEBUG=1

IF ERRORLEVEL 1 (
	pause
	exit /b
)

xcopy /s /y ..\install assets
del "assets\*.dll" 
del "assets\*.exe" 
call \adt-bundle-windows-x86_64-20140702\sdk\platform-tools\adb.exe start-server
call \adt-bundle-windows-x86_64-20140702\apache-ant-1.9.4\bin\ant debug install
pause
