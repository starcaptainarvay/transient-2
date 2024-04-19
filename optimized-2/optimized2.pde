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

final static int MAX_ACTIVE_MOVIES = 10;

/* One-time setup at startup. */
void setup() {
    size(1920, 1080);
    imageMode(CENTER);
    frameRate(24);
    fullScreen();

    active_movies = new ConcurrentHashMap<String, ColinMovie>();
    piano_reader = new PianoController(this);
}

/* Runs 24 times per second, when possible */
void draw() {
    /* Black background */
    background(0);

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
    fill(255);
    text(frameRate, 30, 30);
}

/* ??? */
void movieEvent(Movie m) {
    m.read();
}
