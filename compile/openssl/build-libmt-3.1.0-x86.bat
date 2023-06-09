@echo off
set PLATFORM=Win32
set DISK=%~d0\
set CURRENT=%cd%\
set ROOT=%CURRENT%..\..\
set OPENSSL_SRC=%CURRENT%..\..\third_party\openssl\openssl-3.1.0\

set PATH_INSTALL=%OPENSSL_SRC%\install\
mkdir %PATH_INSTALL%
cd %DISK%
cd %OPENSSL_SRC%
rem pause
rem echo Enable OPENSSL FIPS
rem perl Configure VC-WIN32 no-shared no-asm enable-md2 enable-rc5 enable-fips --prefix=%PATH_INSTALL_TEMP%
rem no-shared no-asm enable-md2 enable-rc5 enable-fips
perl Configure VC-WIN32 threads no-asm no-ssl2 no-ssl3 no-idea enable-fips no-mdc2 no-ec2m no-shared enable-md2 enable-rc5 --prefix=%PATH_INSTALL%
nmake
nmake install

set INSTALL_INCLUDE=%ROOT%include\openssl\3.1.0\openssl\
set INSTALL_LIB=%ROOT%runtimes\openssl\3.1.0\%PLATFORM%\
rmdir /S/Q %INSTALL_INCLUDE%
rmdir /S/Q %INSTALL_LIB%
mkdir %INSTALL_INCLUDE%
mkdir %INSTALL_LIB%
xcopy %PATH_INSTALL%\include\openssl\* %INSTALL_INCLUDE%* /O /X /E /H /K /Y /R /C /S
for /f %%k in ('dir %PATH_INSTALL%\lib\*.lib /s /b') do xcopy %%k %INSTALL_LIB% /O /X /H /K /Y /R /C /A

nmake clean
rmdir (x86)
rmdir /S/Q %PATH_INSTALL%
cd %CURRENT%
echo The project build finished.
