[Origin]

http://www.mono-project.com/
https://github.com/mono/mono/releases/


[Description]

Mono 5.0.1.1  
Mono is open source .NET 4.5 implementation by Xamarin (MS owned)  

The last version forXP was 4.0.5 and never released in binary, Mono.NET is not suitable to run GUI apps as it lack WPF support (which instead Mono use cross platform GTK.net). The more realistic use case is to run commandline .NET app or do cross platform .NET development  

License: Various (mostly MIT) see license.txt in the archive  
Note: GTK.net is not included and no LLVM/WinAOT  
Build notes:  
apply patches and run autogen.sh  
edit configure and replace `0x0600` with `0x0501`  
optionally comment all lines contain `-DDLL_EXPORT` is you want static build  
run `configure --with-mcs-docs=no LIBS="-lmingwex -lmsvcrt -lntdll"`  


[Installation]

Copy to your program files\Mono, make shortcut to command prompt pointing to that folder\bin or add it to %PATH%
