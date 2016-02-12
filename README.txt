Smart Grids: the Game
=====================

Deze documentatie hoort bij de Arduino-code voor Smart Grids. Het doel ervan is
om een globale inleiding te bieden aan mensen met enige programmeerkennis, die
de code moeten gebruiken.

Wie de code moet aanpassen is waarschijnlijk beter gediend met de tekst in
game/ontwerp.txt.


Arduino-code in het algemeen
----------------------------

Dit programma is gemaakt voor een Arduino Due. Sluit die aan, en download het
bijbehorende programma van
    https://www.arduino.cc/en/Main/Software

In dat programma moet je bovendien de libraries
    Adafruit RGB LCD Shield Library
    Adafruit MCP23017 Arduino Library
downloaden (menu Sketch, Include library, Manage libraries) en
    Arduino SAM Boards (32-bits ARM Cortex-M3)
wat ondersteuning biedt voor de Due (menu Tools, Board, Boards Manager).

Sluit de Arduino aan, laad een programma, selecteer de juiste poort (menu Tools,
Port - als er meerdere opties zijn zul je ze allemaal bij langs moeten) en klik
op de Upload-knop (->).


Game (de Smart Grid Game)
-------------------------

Dit programma bestaat uit meerdere delen; het volstaat om het game.ino-bestand
te openen en te uploaden (de rest wordt auto- matisch ook gecompileerd en
geupload).

Inhoudelijke documentatie van het spel wordt bij het spel geleverd; technische
documentatie van de code staat in game/ontwerp.txt.


Test-button (diagnostiek voor de knoppen)
-----------------------------------------

Dit programma biedt de gelegenheid om te testen of de knopjes werken. Het print
op de display twee regels informatie.

Regel 1 bevat alle nummers van poorten tussen 14 en 28 (excl) die 'high'
uitlezen. Dat gebeurt als de knop is ingedrukt, maar ook als die niet goed is
aangesloten. Bij een goed werkend spel hoort hier geen poort te staan waar een
knop op is aangesloten tenzij je die indrukt.

Regel 2 bevat de waarde tussen 0 en 1024 die uitgelezen worden op analoge
poorten 8, 9, 10 en 11. Twee paar knoppen zijn immers aangesloten op deze
poorten. Ook deze horen laag (0) te zijn tenzij ingedrukt.


Test-led (diagnostiek voor LED-lampjes)
---------------------------------------

Dit programma biedt de gelegenheid om te testen of de lampjes werken. Het zet de
lampjes een voor een een seconde aan.

Op het schermpje verschijnt het nummer N van het lampje dat op dat moment aan
zou moeten staan; d.w.z. digitale poort N + 28 is op dat moment hoog en alle
andere zijn laag.
