# streamfiler
#### NAME
streamfiler - TCP/IP stream fogadó és lemezre író alkalmazás

#### SYNOPSIS
streamfiler [-options] portnumber

#### DESCRIPTION
A streamfiler egy porton figyelő alkalmazás, ami a csatlakozott connection tartalmát lemezre írja. A kiírt fájl neve megegyezik az érkezési időponttal (milisecundum pontossággal). Egyszerre több connection-t tud fogadni paraméterezéstől függően.

#### OPTIONS

-c connections
    - A maximálisan fogadható párhuzamos connection-ök száma

-f folder
    - A mappa, ahova az érkztetett állományokat letárolja a program. Ha nem létezik induláskor, akkor a program létrehozza azt.

-h
    - Ez a help

-l limit
    - Ekkora limit adatmennyiséget enged szálanként és másodpercenként kilobyte-ban a portjára folyatni a program

-t timeout
    - Azt az időt határozza meg másodpercben, ami után a program bontja az idle connection-öket. Timeout -1 esetén nem bontja.
    
# Tesztelési esetek
#### Posix szerver
###### Kapcsolódás teszt
* Kilens kapcsolodás a szerverhez
* Több kliens, de annyi ami még kevesebb mint a max szám
* Több kliens mint a max szám
* Egyszerre több kliens mint a max szám, majd amikor egy kapcsolódott leáll új kliens kapcsolódás
* Pont annyi kliens mint a max szám, és ha 1 leállt akkor új kapcsolódás
###### Timeout tesz
* Kliens folyamatosan addja az adatojat, és lezárja a kapcsolatot ha kész
* Kliens direkt timeoutol
* Kliens közel jár a timeouthoz, de még nem éri el
###### Adat olvasás teszt
* A kliens annyi adatt küld amennyi a megengedett alatt van
* A kliens több adatot küld de többszörösét a limitnek
* A kliens a limithez képes nem egész számú többszöröst küld
#### File kimentés
* Sikeresen file mentés
* Nem étező könyvtár
* Sikertelen file menmtés
#### Argumentum teszt
* Nincs port megadva
* Van port
* Helyes timeout
* timeout < -1
* timeout = 0
* Helyes limit
* limit = 0
* Helyes kliens szám
* kliens szám = 0
* -h próba
* -f próba
* helytelem kapcsló
* csak egy - jel
#### Program leállítási teszt
* Leállítás kliens nélkül
* Leállítás 1 futó kliensel
* Leállíts 1 lekapcsolódott kliensel
* Leállítás több futó kliensel