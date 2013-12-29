zpr
===

Katalogi:

	- lighttpd 
	lighttpd.conf - plik konfiguracyjny lighttpd; w linijce "server.document-root" trzeba ustawić ścieżkę do katalogu z plikiem "index.html"
	lighttpd odpalamy w tym katalogu poleceniem: lighttpd -D -f ./lighttpd.conf
	pod adresem http://127.0.0.1:3000/index.html powinna pojawić się strona z formularzem. Na razie jest tu stronka pozwalająca na logowanie.

	- cgi-bin
	tu mają się znaleźć skrypty wywoływane przez CGI

	- src
	katalog na pliki cpp. Na razie jest tu przykładowy program pozwalający zapisywać logujących się użytkowników do bazy. Polecenie "make" stworzy obiekt dzielony "userbase.so". Należy go przenieść do katalogu cgi-bin. Następnie po zrestartowaniu serwera lighttpd zapisywanie użytkowników powinno już działać.
