[Origin]

http://ffmpeg.org/releases/ffmpeg-3.3.5.tar.xz  


[Description]

A complete, cross-platform solution to record, convert and stream audio and video.  
... and to (dis)play most kind of image, audio and video  

This build specifically for XP (AVX optimization disabled) and has all features enabled for ffmpeg.exe  
ffplay.exe is built with "small size" in mind.  
This build use aotuv for vorbis and able to encode ~32kbps at `-aq 0`  
```
ffmpeg version 3.3.5 Copyright (c) 2000-2017 the FFmpeg developers
  built with gcc 6.4.0 (tumaG86)
  configuration: --enable-nonfree --enable-gpl --enable-version3 --enable-small
--enable-avresample --enable-avisynth --enable-chromaprint --enable-frei0r --ena
ble-gnutls --enable-ladspa --enable-libass --enable-libbluray --enable-libbs2b -
-enable-libcaca --enable-libcdio --enable-libdc1394 --enable-libfdk-aac --enable
-libflite --enable-libfontconfig --enable-libfreetype --enable-libfribidi --enab
le-libgme --enable-libilbc --enable-libkvazaar --enable-libmodplug --enable-libm
p3lame --enable-libnut --enable-libopencore-amrnb --enable-libopencore-amrwb --e
nable-libopencv --enable-libopenh264 --enable-libopenjpeg --enable-libopenmpt --
enable-libopus --enable-libpulse --enable-librubberband --enable-librtmp --enabl
e-libschroedinger --enable-libshine --enable-libsnappy --enable-libsoxr --enable
-libspeex --enable-libssh --enable-libtesseract --enable-libtheora --enable-libt
wolame --enable-libvidstab --enable-libvo-amrwbenc --enable-libvorbis --enable-l
ibvpx --enable-libwavpack --enable-libwebp --enable-libx264 --enable-libx265 --e
nable-libxavs --enable-libxvid --enable-libzimg --enable-libzmq --enable-libzvbi
 --enable-decklink --enable-netcdf --enable-openal --enable-opencl --enable-open
gl --enable-libmfx --disable-avx --disable-avx2 --pkg-config-flags=--static --ex
tra-libs='-lstdc++ -lgomp' --cpu=pentium3
  libavutil      55. 58.100 / 55. 58.100
  libavcodec     57. 89.100 / 57. 89.100
  libavformat    57. 71.100 / 57. 71.100
  libavdevice    57.  6.100 / 57.  6.100
  libavfilter     6. 82.100 /  6. 82.100
  libavresample   3.  5.  0 /  3.  5.  0
  libswscale      4.  6.100 /  4.  6.100
  libswresample   2.  7.100 /  2.  7.100
  libpostproc    54.  5.100 / 54.  5.100
```

License: launch ffmpeg.exe  



[Installation]

Copy to your program files, use `open with...` association to vipsdisp.exe if you wish  
Copy your custom fonts into `share\fonts` sub-directory
Copy your frei0r dlls into `frei0r` sub-directory  
Copy your ladspa dlls into `ladspa` sub-directory  
Copy libbdplus.dll+libaacs.dll or libmmbd.dll in the root directory  

