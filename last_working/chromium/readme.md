[Origin]

https://github.com/deemru/chromium-gost  


[Description]

chromium 49.0.2623.112, this is the last version still support XP.  
This build has additonal GOST cipher otherwise it should has same feature as  
build from https://github.com/henrypp/chromium/  

Changes:  
replace redist D3DCompiler_47.dll with D3DCompiler_43.dll as it's not for XP  
chrome.dll -> patched not to show "unsupported OS" (henrypp's already like this)  
add missing wow_helper.exe so it will run on XP64  

License:  
BSD + various  


[Installation]

Copy to somewhere, make shortcut for it.  
 
