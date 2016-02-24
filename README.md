# FilesSharing

Tutaj prosty opis protokolu: https://docs.google.com/document/d/1zfyikaawUBJHN8wwl9vRPu-XHttYT0Gcos012A3PdRo/edit?usp=sharing

Kiedyś planowalismy coś takiego:
![Plany](http://i.imgur.com/qADBxbt.jpg)

Krótka idea projektu:
- Projekt wymaga cmake >= 2.8.x i kompilator c++14 (testowane na clang > 3.5 i gcc >= 5.1, libstdc++ i libc++), oraz boost >= 1.5.3. Budowanie: z poziomu folderów client i server cmakiem a potem wedle upodobania (np. make czy ninja).
- Projekt dzieli się na 2 niezależne programy, klient i serwer. Klient synchronizuje pliki z serwerem, a serwer z wieloma klientami.
- Pliki w katalogu synchronizacyjnym klienta można modyfikować, a serwera nie. Nie było testowane na plikach > 1 MB.
- Klienta można uruchomić bez parametrów, wówczas szuka serwera na localhoscie:4096 i używa .test_dir jako folder do synchronizacji.
- Klienta można uruchomić z 3 parametrami, oznaczającymi kolejno ip serwera, port i scieżkę do synchronizacji

- Serwer można uruchomić bez parametrów, wówczas nasłuchuje na porce 4096
- Serwer można uruchomić z 1 parametrem którym jest numer portu do nasłuchiwania
- synchronizowanym katalogiem jest syncRoot
- Serwer równolegle obsługuje żądania klientów
- Pracę serwera kończymy wpisując q, quit lub exit, wielkość liter nie ma znaczenia
