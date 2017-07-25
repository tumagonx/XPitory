[Origin]

http://invisible-island.net/dialog/  
https://wiki.gnome.org/action/show/Projects/Zenity


[Description]

Dialog 1.3, formerly known as cdialog is based on the Debian package for dialog 0.9a.  
It allow widget (dialog box) creation with console interaction.  
This build is using PDCurses fork from http://github.com/Bill-Gray/PDCurses which use GDI window.  
Zenity 3.8, is GTK version of cdialog.  

License:  
(dialog)LGPL + (PDCurses)PD -> LGPL  
(zenity)GPL + (GTK and deps)LGPL -> GPL  

Notes:  
dialog compiled with mouse and uncode support, but I don't know if it can display unicode char  
zenity don't handle "close window" event so it confuse %ERRORLEVEL%  
you can supply your own zenity.ui to override internal template



[Installation]

Copy to somewhere in the PATH environment lookout, call it from console
