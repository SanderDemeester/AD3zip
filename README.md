Burrows-Wheeler
===============

Project Algoritmes en Datastructuren 3: Data compressie Burrows-Wheeler

### File layout
-----------------------------------------------
|4 byte header|4byte bwt-index,1byte_|bwt data|
-----------------------------------------------
->1 byte methode_header
->3 byte blocksize
### TODO

Implementeren van BWT decoderen.

### Veronderstellingen:
We gaan er vanuit dat de bwt van de vorm is: n_string, waar n past in 1 byte (char)   
### Know bugs:
Quicksoort heeft nog wat probleempjes als de zelfde karakters na elkaar voorkomen, het lijkt dat het geen stabiel sorteeralgoritme is