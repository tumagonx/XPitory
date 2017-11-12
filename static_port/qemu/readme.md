[Origin]

https://download.qemu.org/qemu-2.10.1.tar.xz  


[Description]

Qemu 2.10.1, A generic and open source machine emulator and virtualizer.  

This build specifically for XP (AVX optimization disabled) and has all features enabled except:  
`vnc-sasl` disabled because it drag too many crypto (openssl, heimdal) when this build already bundled too many (mbedTLS, NSS and GnuTLS).  
`GTK-GL` which need version 3.16 this build use GTK 3.6 which more static build friendly.  


License: GPL  


[Installation]

Extract to your program files, make shortcut if you wish  
For HAXM you may try http://software.intel.com/sites/default/files/haxm-windows_r05.zip  
Console-less executables are omitted, to generate it run makeqemuw.bat  


