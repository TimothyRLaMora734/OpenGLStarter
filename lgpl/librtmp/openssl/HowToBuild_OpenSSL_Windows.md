## How to Build OpenSSL for Windows

You need to select the VC-WIN32 build or VC_WIN64A build

### VC-WIN32

1) Install Visual Studio
2) Install Notepad++ from https://notepad-plus-plus.org/downloads/
3) Download nasm 32 bit from https://www.nasm.us/
4) Download and install Strawberry Perl from http://strawberryperl.com/
5) Install GitBash from https://gitforwindows.org/
6) Setup the windows $path environment variable

There will have some values, you need to include the values to match your development environment.

You need to include all tools you installed to the path.

All directories you need to setup in $path:

```
C:\nasm32
C:\Strawberry\perl\bin
C:\Strawberry\c\bin
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\bin\Hostx86\x86
C:\Program Files (x86)\Windows Kits\10\bin\10.0.17763.0\x86
```

7) Clone the repository from GitHub

Open GitBash from the folder you want to put the source code in.

```
mkdir openssl32
cd openssl32
git clone https://github.com/openssl/openssl.git .
```

8) Close GitBash and open CMD

You can close the gitbash from here.

You need to open CMD from startmenu and change the directory to the path you downloaded the library.

```
CMD
c:
cd c:\Your\Path\To\The\Library
```

8) Configure compilation parameters

```
perl configure --prefix="%cd%\dist" no-idea no-mdc2 no-rc5 shared VC-WIN32
```

9) Fix the makefile generated

Although the configuration is OK from the script perspective, you need to put some include directories
to the makefile to allow it to work.

Open the generated makefile in Notepad++.exe and put these lines at the begining of the file:

```
VS_INCLUDE_DIR=-I "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\include" \
	-I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\ucrt" \
	-I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\um" \
	-I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.17763.0\shared"

VS_LIB_DIR=/LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\lib\x86" \
	/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\ucrt\x86" \
	/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\um\x86"
```

These lines are references to your Visual Studio environment. So you need to change the paths 14.16.27023 and 10.0.17763.0 to match your installation.
	
Look for:

```
LDFLAGS=/nologo /debug
```

And change it to:

```
LDFLAGS=/nologo /debug $(VS_LIB_DIR)
```

Look for:

```
RCFLAGS=
```

And change it to:

```
RCFLAGS=$(VS_INCLUDE_DIR)
```

Look for:

```
CNF_CPPFLAGS=-D"OPENSSL_SYS_WIN32" -D"WIN32_LEAN_AND_MEAN" -D"UNICODE" -D"_UNICODE" -D"_CRT_SECURE_NO_DEPRECATE" -D"_WINSOCK_DEPRECATED_NO_WARNINGS" -D"NDEBUG"
```

Change it to:

```
CNF_CPPFLAGS=/MT -D"OPENSSL_SYS_WIN32" -D"WIN32_LEAN_AND_MEAN" -D"UNICODE" -D"_UNICODE" -D"_CRT_SECURE_NO_DEPRECATE" -D"_WINSOCK_DEPRECATED_NO_WARNINGS" -D"NDEBUG" $(VS_INCLUDE_DIR)
```

Look for:

```
CNF_CFLAGS=/Gs0 /GF /Gy /MD
```

Change it to:

```
CNF_CFLAGS=/Gs0 /GF /Gy /MT $(VS_INCLUDE_DIR)
```

Look for:

```
CNF_CXXFLAGS=
```

Change it to:

```
CNF_CXXFLAGS=/MT $(VS_INCLUDE_DIR)
```

10) Compile all libs and examples

```
nmake
```

11) At the end you will have the files compiled in the root directory.

```
libssl.def
libssl.lib
libssl-3.dll
libcrypto.def
libcrypto.lib
libcrypto-3.dll
```

### VC-WIN64A

You need to follow the same steps you take in VC-WIN32, but the paths of the tools need to point to the 64 bit paths.

In step 6, the environment $path will be replaced by:

```
C:\nasm64
C:\Strawberry\perl\bin
C:\Strawberry\c\bin
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\bin\Hostx64\x64
C:\Program Files (x86)\Windows Kits\10\bin\10.0.17763.0\x64
```

In step 8, change VC-WIN32 to VC-WIN64A:

```
perl configure --prefix="%cd%\dist" no-idea no-mdc2 no-rc5 shared VC-WIN64A
```

In step 9, the variable VS_LIB_DIR will be replaced by:

```
VS_LIB_DIR=/LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\lib\x64" \
	/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\ucrt\x64" \
	/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\um\x64"
```

In step 10, you need to add BUILD64BIT=1 to the nmake command:

```
nmake BUILD64BIT=1
```
