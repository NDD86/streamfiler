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
