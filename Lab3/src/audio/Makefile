app:
	g++ -Wall -c $(DEBUG) -pedantic -pthread -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0 mp3_handler.cpp

	g++ -Wall -c $(DEBUG) -pedantic -pthread -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0 audio_server.cpp

	g++ -Wall audio_server.o mp3_handler.o main.cpp -o mp3_player -pthread -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0

clean:
	rm mp3_player *.o
