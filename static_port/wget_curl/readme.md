[Origin]

https://curl.haxx.se/download/curl-7.56.1.tar.xz  
https://ftp.gnu.org/gnu/wget/wget-1.19.2.tar.gz  


[Description]

WGET: Web archiver
Curl: uploader and downloader

License: 
(wget)GPL + (GnuTLS)LGPL+many -> LGPL  
(CURL)MIT + (GnuTLS)LGPL+many -> GPL  



[Installation]

Copy to somewhere in the PATH environment lookout, call it from console  
Updating secure connection certificate  

1st method  
```
curl -R -O http://www.download.windowsupdate.com/msdownload/update/v3/static/trustedr/en/authroots.sst
curl -R -O http://www.download.windowsupdate.com/msdownload/update/v3/static/trustedr/en/delroots.sst
curl -R -O http://www.download.windowsupdate.com/msdownload/update/v3/static/trustedr/en/roots.sst
curl -R -O http://www.download.windowsupdate.com/msdownload/update/v3/static/trustedr/en/updroots.sst
curl -R -O http://www.download.windowsupdate.com/msdownload/update/v3/static/trustedr/en/disallowedcert.sst
updroots.exe authroots.sst
updroots.exe updroots.sst
updroots.exe -l roots.sst
updroots.exe -d delroots.sst
rvkroots.exe -l -u disallowedcert.sst
```
(You could find above "retired" tools "somewhere on the net")  

2nd method  
If above doesn't work (say you're not admin)  
you can use certificate from curl website:  
`curl -k -R -o curl-ca-bundle.crt  https://curl.haxx.se/ca/cacert.pem`  
