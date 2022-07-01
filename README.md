# Klasická hra Šachy

****
## Zadanie

**Implementujte následující varianty:**
1. pro 2 hráče na jednom počítači
2. pro hru proti počítači

**Hra musí splňovat následující funkcionality:**
- Dodržování všech pravidel dané varianty (u klasické varianty tedy i rošáda, braní mimochodem, proměna pěšce).
- Ukládání (resp. načítání) rozehrané hry do (resp. ze) souboru (vytvořte vhodný formát a uživatelské rozhraní)
- Oznamovat stav a konec hry (šach, mat, pat) a její výsledek.
- Umělá inteligence (škálovatelná nebo alespoň 3 různé druhy, jeden druh můžou být náhodné tahy, ale nestačí implementovat pouze náhodné tahy)

**Kde lze využít polymorfismus? (doporučené)**
- Ovládání hráčů: lokální hráč, umělá inteligence (různé druhy), síťový hráč
- Pohyby figurek: král, dáma, věž, jezdec,...
- Uživatelské rozhraní: konzolové, ncurses, SDL, OpenGL (různé druhy),...
- Pravidla různých variant: klasické šachy, žravé šachy, asijské šachy
- Jednotlivá pravidla: tahy, rošáda, braní mimochodem, proměna (jejich výběr pak může být konfigurovatelný)

**Další informace**
- https://cs.wikipedia.org/wiki/Šachy
- https://cs.wikipedia.org/wiki/Šachové_varianty

****
## Bližšia špecifikácia

Hra sa bude dať hrať v dvoch režimoch, buď 1v1 za jedným PC alebo jeden hráč proti počítaču. Pri hre proti počítaču si bude môcť hráč zvoliť úroveň bota, proti ktorému bude hrať z easy, medium, hard.

Hra bude mať iba jedno uživatelské rozhranie a to terminál, bude implementované triedou Interface, ktorá ako fieldy obsahuje referencie na std::ostream a std::istream. Hra sa teda bude ovládať jednoduchými príkazmi, ktoré budú dostupné podľa aktuálneho stavu aplikácie. Aplikácia bude mať 4 stavy, ktoré rozhodujú o tom, čo môže užívateľ robiť a aké commandy používať. Stavy sú nasledujúce: MENU (užívateľ môže začať novú hru alebo načítať uloženú hru), GAME_MODE (užívateľ môže hrať single player alebo versus mode), LEVEL (v prípade single player si užívateľ vyberá úroveň svojho oponenta), IN_GAME (hráči hýbu figúrkami). 

Commandy sú nasledujúce:

**MENU:** (Aplikácia po spustení začína v stave MENU)
- new - vytvorí novú hru
- load - ukáže uživatelovi uložené hry a načíta zvolenú hru zo súboru 
- help - zobrazí nápovedu s dostupnými príkazmi
- quit - ukončí celú aplikáciu

**GAME_MODE**
- help - zobrazí nápovedu s dostupnými príkazmi
- single - vytvorí novú single player
- versus - vytvorí novú 1v1 hru
- quit - ukončí celú aplikáciu

**LEVEL (v prípade voľby versus, ide hra rovno do stavu IN_GAME):**
- help - zobrazí nápovedu s dostupnými príkazmi
- easy - náhodné ťahy
- medium - bot náhodne vyberá jeden zo svojich najlepších ťahov podľa vyhodnocovacej funkcie 
- hard - minimax algoritmus
- quit - ukončí celú aplikáciu

**IN_GAME:**
- help - zobrazí nápovedu s dostupnými príkazmi
- move [from][to] (“move a1a2”) - pohyb s figúrkou z [from] do [to]
- board - ukáže uživateľovi šachovnicu a stav hry (kto je na ťahu, či je šach, atd.)
- captured - ukáže všetky zajaté biele aj čierne figúrky
- save [filename] - uloží rozohratú hru do súboru [filename]
- quit - ukončí celú aplikáciu

****
## Polymorfizmus

Polymorfizmus využívam v rámci abstraktnej triedy Player, z ktorej dedia triedy PlayerHuman a PlayerBot, z PlayerBot ešte dedia triedy PlayerEasyBot, PlayerMediumBot a PlayerHardBot. Tieto 4 triedy implementujú polymorfnú metódu take_turn. Implementácia v triede PlayerHuman očakáva, že užívateľ rozhodne ako chce odohrať svoj ťah, teda najprv načíta vstup od užívateľa a po validácii súradníc pohne danou figúrkou. Implementácia v podtriedach PlayerBot hýbe figúrkami náhodne, prípadne podľa nejakého algoritmu, záleží na úrovni bota, ktorú si uživatel vyberie na začiatku hry a teda na konkrétnej impelemntácii a podtriede. V PlayerEasyBot je metóda take_turn impelentovaná pomocou náhodných ťahov. V triede PlayerMediumBot sa náhodne vyberie z najlepších ťahov podľa vyhodnocovacej funkcie implementovanej v triede Board, teda ak je to možné tento hráč vyradí súperovu figúrku. V triede PlayerHardBot je implemntácia založená na rekurzivnom algoritme minimax s alpha-beta orezávaním. Na ohodnotenie šachovnice je použitá rovnaká vyhodnocovacia funkcia ako v PlayerMediumBot. V triede Game budú okrem iných fieldov dva unique pointre na Player objekt (player_white, player_black) a pri move commande sa na nich zavolá polymorfná métoda take_turn, takže v metódach triedy Game sa nemusíme starať o to aký hráč je na ťahu (človek/bot), práve preto využívame polymorfizmus, ktorý sa o to postará. Polymorfizmus nám v tomto prípade ešte veľmi pomôže, ak by sme v budúcnosti chceli do hry pridať iný typ hráča. Napríklad ďalšiu úroveň AI alebo hráča, ktorý by hral po sieti, ak by sme do hry chceli pridať multiplayer, teda hru pre 2 hráčov za 2 PC.

Ďalší polymorfizmus je v abstraktnej triede Piece a v podtriedach King, Queen, Pawn, Knight, Rook a Bishop, ktoré reprezentujú konkrétne typy šachových figúrok. Tieto triedy implementujú metódu available_moves, ktorá vráti list všetkých možných pohybov danej figúrky z daných súradníc, pričom pre každý typ figúrky táto metóda funguje inak, keďže každá figúrka sa podľa pravidiel šachu hýbe inak. Objekty týchto tried budú spolu uložené ako shared pointre v triede Board, v 2D array o veľkosti 8x8. Metody triedy Board teda nezaujíma aká figúrka sa kde nachádza a akou figúrkou bude hráč hýbať, jednoducho zavolá polymorfnú métodu available_moves, ktorá sa postará o spravné vygenerovanie všetkých dostupných pohybov pre danú figúrky (daný typ a umiestnenie). Okrem toho si tieto pointre budeme ešte držať v triede Player, teda každý hráč si bude držať aké figúrky má, čo mi uľahčí implementáciu.

****
## Vyhodnocovacia funkcia

Vyhodncovacia funkcia využívana v PlayerMediumBot a PlayerHardBot funguje následovne. Každý typ figúrky má pridelenú nejakú číselnú hodnotu, napríklad královna 950, pešiak 100 atd. Pre každú figúrku daného typu vo zvolenej farbe pričítame danú hodnotu do celkovej hodnoty. Pre opačnú farbu odčítame z celkovej hodnoty. Okrem toho je ešte dôležité aká figúrka sa kde na šachovnici nachádza, napríklad kôň (knight, jezdec) je cennejší v strede šachovnice ako na kraji. Hodnota šachovnice sa vypočítava pre nejakú zvolenú farbu.

Pri vymýšlaní tejto funkcie a algoritmov AI som sa učil a informácie čerpal z internetu https://en.wikipedia.org/wiki/Chess_piece_relative_value, https://en.wikipedia.org/wiki/Minimax, https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning, https://www.chessprogramming.org/Alpha-Beta, https://www.chessprogramming.org/Minimax, https://www.youtube.com/watch?v=l-hh51ncgDI&ab_channel=SebastianLague, https://www.youtube.com/watch?v=STjW3eH0Cik&ab_channel=MITOpenCourseWare a materiálov predmetu BI-ZUM https://courses.fit.cvut.cz/BI-ZUM/. 

****
**Marko Hujo**
