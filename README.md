Wunderdog koodauspähkinä kesä 2015
==================================
Solution to http://wunderdog.fi/koodaus-pahkina-kesa-2015


This code requires file "alastalon_salissa.txt" to be in the working directory
In this file, the charset is changed to ISO-8859-1 or ISO-8859-15 to get rid of multibyte characters (äöå).
You can do this by running the following command in just about any *nix system:

`iconv -f UTF-8 -t ISO-8859-1 alastalon_salissa.txt > converted.txt && mv converted.txt alastalon_salissa.txt`

### Build command
`g++ -O3 --std=c++0x -o Fooo` or `make`

### Run command
`./Fooo`


### Expected result
```
Words: 22930
Total number of word-pairs: 9
 
vahingonkamfiili kirjoitusp�ydille
vahingonkamfiili kirjoitusp�yd�st�
vahingonkamfiili kirjoitusp�yd�lt�
vahingonkamfiili kirjoitusp�yd�n
vahingonpillastumisen ker�j�p�yd�n
vahingonpillastumisen kirjoitusp�yd�st�
vahingonpillastumisen kirjoitusp�yd�lt�
vahingonpillastumisen kirjoitusp�yd�n
j�rjensyhyttelemisen k�ydenpingottuvaa
 
Muhkeus: 21
```

