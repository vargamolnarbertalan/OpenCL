# Párhuzamos eszközök programozása
Varga-Molnár Bertalan | PY7QFH órai előrehaladás

# Féléves feladat: WAW audio normalizáló program
A szórakoztatóiparban léteznek audiora vonatkozó sztenderdek, mint például az EBU R 128, amik sok megkötést tartalmazhatnak, de legfőbb céljuk, hogy a műsorok egységes hangerőn szóljanak, így egy bizonyos intervallumot vagy akár abszolút peaket limitálnak. Egy primitív audiofájl halkítót készítünk, ami az eredeti fájl hangerejét a nyolcadára veszi vissza. A nyolcas osztást használják például élő mikrofonok kompresszorában, de miért is ne lehetne a feladatban is ez az arányszámunk. A programot elkészítettem szekvenciális illetve OpenCL-es megoldásként is, továbbá a 'Feladat' jegyzék tartalmaz egy 'wav_writer' aljegyzéket, amiben egy tetszőleges hosszúságú .wav fájl előállítására alkalmas programot találunk, illetve egy 'measurements' aljegyzéket, amiben a szekvenciális és OpenCL-es futtatási idők vannak kiexportálva egyesével, de ezeket célszerűbb már együtt, összehasonlítással együtt megtekinteni a 'comparision.xlsx' dokumentumban. 

A mérésekből kiolvasható, hogy minden diszkrét mérési pontban az OpenCL a gyorsabb megoldás, de minél nagyobb fájlt (azaz hosszabb hanganyagot) kívánunk halkítani, az OpenCL fölénye annál egyértelműbb. Ez abból adódik, hogy a kernel felé történő átadási idők arányaiban a nagyobb problémaméretnél egyre elenyészőbb plusz időt adnak hozzá a futáshoz, és a párhuzamosítás egyre hasznosabb. 

Megjegyzés: Nem biztos, hogy konvenció szerint elfogadott feltölteni a generált és a módosított .wav fájlokat, de így minden program futtatása nélkül is hallható, hogy:
- az audio valóban halkabb lett
- az audio nem sérült, ugyanolyan "tone" hallható, mint az eredetiben
