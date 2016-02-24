# FilesSharing

Tutaj prosty opis protokolu: https://docs.google.com/document/d/1zfyikaawUBJHN8wwl9vRPu-XHttYT0Gcos012A3PdRo/edit?usp=sharing

Kiedyś planowalismy coś takiego:
![Plany](http://i.imgur.com/qADBxbt.jpg)

Krótka idea projektu:
- Projekt dzieli się na 2 niezależne programy, klient i serwer. Klient synchronizuje pliki z serwerem, a serwer z wieloma klientami.
- Pliki w katalogu synchronizacyjnym klienta można modyfikować, a serwera nie.
- Klienta można uruchomić bez parametrów, wówczas szuka serwera na localhoscie:4096 i używa .test_dir jako folder do synchronizacji.
- Klienta można uruchomić z 3 parametrami, oznaczającymi kolejno ip serwera, port i scieżkę do synchronizacji
- Opisz serwer, Łukasz
