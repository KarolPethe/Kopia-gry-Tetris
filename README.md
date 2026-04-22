Celem projektu było odwzorowanie i stworzenie gry podobnej do klasycznego „Tetrisa” w języku C++. W projekcie zgodnie z wymaganiami korzystałem z programowania obiektowego wraz z jego elementami takimi jak polimorfizm czy dziedziczenie. Ponadto korzystałem z biblioteki graficznej SFML w celu oprawy graficznej mojego programu, ponieważ biblioteka ta zapewnia łatwe rysowanie 2D i obsługę okien. Program obsługuje podstawową mechanikę gry: spadające klocki, usuwanie pełnych linii, punktację, oraz ekrany Start, Playing i GameOver. Dodatkowo zaimplementowano „ghost piece” oraz licznik czasu w osobnym wątku.

Czym jest Tetris? 

Tetris jest popularną logiczną grą zręcznościową stworzoną w latach osiemdziesiątych. Rozgrywka jest pozornie prosta i intuicyjna: polega na sterowaniu spadającymi klockami tak, aby wypełnić poziomo ułożone linie na planszy. Gracz ma za zadanie sterować tak, żeby klocki idealnie pasowały do utworzonych dziur. Kiedy linia zostanie zapełniona – znika, gracz zdobywa punkt, a rozgrywka trwa dalej. Gra jest kontynuowana aż do momentu kiedy nowe klocki nie mogą zostać umieszczone na planszy.

Specyfikacja zewnętrzna:

1.	Uruchomienie programu:
Po skompilowaniu programu oraz uruchomieniu gry, użytkownikowi wyświetla się niewielkie okno programu z nazwą gry oraz ostatnim zdobytym wynikiem jak i tym najlepszym. Dodatkowo poniżej znajdują się dwa przyciski: przycisk START, który rozpoczyna rozgrywkę oraz przycisk EXIT, który zamyka program. Rozmiar okna nie jest przypadkowy, małe wymiary (plansza 10×20, z blokami o wymiarze 32×32 piksele) pozwalają na korzystanie z programu w tle lub podczas innych czynności na komputerze 

2.	Sterowanie oraz rozgrywka:
Po kliknięciu przycisku START użytkownik rozpoczyna rozgrywkę. Jest ona praktycznie taka sama jak w klasycznej wersji gry. Zadaniem gracza jest uzyskanie jak największej liczby punktów, poprzez ułożenia spadających klocków (składających się z czterech bloków) w taki sposób, aby zakryły one cały jeden poziomy pas planszy. Za ułożenie tylko jednego poziomego paska użytkownik dostaje 100 punktów, za zakrycie dwóch pasów jednocześnie – 300 punktów, za zakrycie trzech pasów – 500 punktów, a za zakrycie czterech pasów jednocześnie już 800 punktów. Punkty widoczne są w lewym górnym rogu planszy wraz z licznikiem czasu aktualnej rozgrywki. Rozgrywka kończy się w momencie kiedy spadające klocki przekroczą górną granice planszy. Każdy klocek upada tzn. przemieszcza się o jeden poziom w dół co pół sekundy. Jest 7 możliwych opcji klocków, które mogą się pojawić, kolejność ich występowania jest losowa.

Użytkownik steruje klockami w sposób następujący:
-  Strzałka w lewo/prawo: przesunięcie klocki o jeden blok w poziomie.
-  Strzałka w dół: przyspieszenie opadania.
-  Spacja: natychmiastowe opuszczenie klocki na możliwie najniższe położenie.
-  Klawisz G: włączanie i wyłączanie podglądu ghost‐piece.
-  ENTER: potwierdzenie wyboru w menu Start i Game Over.
-  ESC: wyjście z gry lub powrót do menu podczas ekranu Game Over.


Po zakończeniu gry wyświetlane jest okno menu przegranej gdzie widoczne są statystyki: Last Score i Best Score, zapisane między sesjami w pliku scores.dat.

Specyfikacja wewnętrzna: 

Klasy zawarte w projekcie, pliki podzielone zostały na pliki .cpp oraz .h:

•	Struktura Punkt – prosta struktua reprezentująca pozycję pojedynczego bloku na planszy (pola x, y).

•	Klasa Figura – abstrakcyjna klasa bazowa definiująca interfejs klocka: zawiera metody: obrot(), przestun(dx,dy), getBlok(), getKolor().

•	Klasa Forma – konkretna implementacja klocka: Przechowuje std::array<Punkt,4> bloki i kopię w std::vector<Punkt> blokVector. Dodatkowo zawiera metodę prywatną syncVector(), która synchronizuje blokVector z aktualnymi pozycjami w tablicy.

•	Klasa TworzenieF – fabryka generująca losowe figury. Flaga seeded stosuje by jednorazowo zasiewać generator. Metoda losowa() zwraca która  std::unique_ptr<Figura> wskazujący na nową instancję Forma.

•	Klasa Obszar – klasa odpowiadająca za logikę planszy. Zawiera deklaracje i ustalenie rozmiarów okna. Zawiera metody, które zapewniają kontrolę klocków na planszy: poprawna(bloki), zablokuj(figura), usun(), rysuj(window).

•	Klasa Gra – główna klasa aplikacji. Zarządza obiektami SFML: sf::RenderWindow, sf::Clock dropClock, sf::Font. Klasa ta odpowiada również za obsługę wątku timera (std::thread, std::atomic<bool> timerRunning, std::atomic<int> elapsedSeconds). 
