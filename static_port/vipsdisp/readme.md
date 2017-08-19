[Origin]

http://www.vips.ecs.soton.ac.uk/
https://github.com/jcupitt/vipsdisp-tiny/


[Description]

VIPS Display Tiny (mod)  
This viewer is suitable for 32bit OS to open huge images by stream-read them (need < 20MB RAM)  
It display following formats:
jpeg: up to 4GP (gigapixels)
png: theoretically up to 1TP (terapixels)
tiff(bigtiff): unlimited size

In case of image takes longer to load this means vipsdisp need caching it to %TEMP%  
vipsdisp could optimize the image for fast loading by exporting to tiled bigtiff  
e.g.: `vipsdisp E:\pic.jpg E:\optpic`  

License: (vipsdisp-tiny)GPL + (VIPS and deps)LGPL -> LGPL



[Installation]

Copy to your program files, use `open with...` association to vipsdisp.exe if you wish
