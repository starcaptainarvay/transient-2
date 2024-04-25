import processing.video.*;
import processing.net.*;
import java.util.Set;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.NoSuchElementException;
import java.util.Arrays;
import java.nio.file.Paths;

ConcurrentHashMap<String, ColinMovie> active_movies;
PianoController piano_reader;

final static int MAX_ACTIVE_MOVIES = 30;
final static int MOVIE_WIDTH = 853/2;
final static int MOVIE_HEIGHT = 480/2;
final static int MOVIE_FOLDER_COPIES = 5;
final static int WIDTH = 1400; //1920;
final static int HEIGHT = 472; //1080; //647;+
final static float COLIN_SCALING = 5.0; // 1/COLIN_SCALING is the size of the videos

/* One-time setup at startup. */
void setup() {
    size(1400, 1050, P2D);
    imageMode(CENTER);
    frameRate(24);
    noSmooth();
    fullScreen(P2D, 1);

    active_movies = new ConcurrentHashMap<String, ColinMovie>();
    piano_reader = new PianoController(this);
}

/* Runs 24 times per second, when possible */
void draw() {
    /* Black background */
    background(0);
        translate(0, (1050 - HEIGHT) / 2);

    fill(255);
    rect(0,0,WIDTH,HEIGHT);
    // scale(0.5);

    /* Update list of active movies */
    if (piano_reader.readClient()) {
        /* Add movies for new keys pressed */
        for (ColinMovie new_movie : piano_reader.getKeysPressed()) {
            if (active_movies.size() < MAX_ACTIVE_MOVIES) {
                new_movie.play();
                active_movies.put(new_movie.filename, new_movie); 
            }
        }
        /* change color of movies that have had their key released */
        double[] current_color = piano_reader.getCurrentColor();
        for (String pitch : piano_reader.getKeysReleased()) {
            for (String key : active_movies.keySet()) {
                if (key.startsWith(pitch)) {
                    ColinMovie movie = active_movies.get(key);
                    movie.setTargetColor(current_color[0], current_color[1], current_color[2]);
                }
            }
        }
    }

    /* Prune active_movies and draw the rest */
    for (ColinMovie movie : active_movies.values()) {
        if (movie.isOver()) {
            active_movies.remove(movie.filename);
        } else {
            movie.drawMovie();
        }
    }

    /* Display frame rate */
    //fill(255);
    //text(frameRate, 30, 30);
}

/* ??? */
void movieEvent(Movie m) {
    m.read();
}
