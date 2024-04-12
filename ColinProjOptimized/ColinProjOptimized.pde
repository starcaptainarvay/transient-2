import processing.video.*;
import processing.net.*;
import java.util.Set;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.NoSuchElementException;
import java.util.Arrays;
import java.nio.file.Paths;

final static boolean DEBUG = false;
final String JSON_STREAM_DELIMITER_AVYAY = "<!>";
final int MILES_LIMIT = 10;

ConcurrentHashMap<String, colinMovie> active_movies;

HashMap<String, Integer> notegroup_map;
ArrayList<PitchGroup> pitchGroups;

Client client;
String dataBuffer;

double[] current_color = {0, 0, 0};

String[] PITCH_CLASSES = {
    /* 
    "C", "Db", "D", "Eb",
    "E", "F", "Gb", "G",
    "Ab", "A", "Bb", "B" 
    */

    "C", "C#", "D", "D#",
    "E", "F", "F#", "G", 
    "G#", "A", "A#", "B"
};


String getMovie(String pitch, int velocity) {
    if (DEBUG) {
        println("called ColinMovieNames.getMovie( " + pitch + ", " + velocity + ")");
    }
    int pg = notegroup_map.get(pitch);
    return pitchGroups.get(pg).getMovie(velocity);
    // missing error handling
}

void setup() {
    size(2560, 1080);
    imageMode(CENTER);
    frameRate(24);
    fullScreen();
    
    // *************************** IMPORTANT ***************************
    //         YOU MUST CLONE THE PROJECT TO C:/colin-project/
    // *****************************************************************
    noteGroupMap = Map.of(
      "C2",  1,
      "C#2", 1,
      "D2",  1,
      "D#2", 1,
      "E2",  1,
      "F2",  1,
      "F#2", 1,
      "G2",  1,
      "G#2", 1,
      "A2",  1,
      "A#2", 1,
      "B2",  1,
      "C3",  1,
      "C#3", 1,
      "D3",  1,
      "D#3", 1,
      "E3",  2,
      "F3",  2,
      "F#3", 2,
      "G3",  2,
      "G#3", 2,
      "A3",  2,
      "A#3", 2,
      "B3",  2,
      "C4",  2,
      "C#4", 2,
      "D4",  2,
      "D#4", 2,
      "E4",  2,
      "F4",  2,
      "F#4", 2,
      "G4",  2,
      "G#4", 3,
      "A4",  3,
      "A#4", 3,
      "B4",  3,
      "C5",  3,
      "C#5", 3,
      "D5",  3,
      "D#5", 3,
      "E5",  3,
      "F5",  3,
      "F#5", 3,
      "G5",  3,
      "G#5", 3,
      "A5",  4,
      "A#5", 4,
      "B5",  4,
      "C6",  4,
      "C#6", 4,
      "D6",  4,
      "D#6", 4,
      "E6",  4,
      "F6",  4,
      "F#6", 4,
      "G6",  4,
      "G#6", 4,
      "A6",  4,
      "A#6", 4,
      "B6",  4,
      "C7",  4
    );
    pitchGroups = new ArrayList<>(Arrays.asList(
      new PitchGroup(1),
      new PitchGroup(2),
      new PitchGroup(3),
      new PitchGroup(4)
    ));

    active_movies = new ConcurrentHashMap<String, colinMovie>();
    client = new Client(this, "127.0.0.1", 3000);
    dataBuffer = "";

    Movie dummyMovie = new Movie(this, getMovie("C4", 50));
    dummyMovie.dispose();
}  


void draw() {    
    background(0);
    scale(0.5);
    getKeysPressed();
    
    Set<String> names = active_movies.keySet();
    for (String name : names) {
        colinMovie movie = active_movies.get(name);
        if (movie.pauseIfOver()) {
            active_movies.remove(name);
        } else {
            movie.drawMovie();
        }
    }
    
    fill(255);
    text(frameRate, 30, 30);
}


void getKeysPressed() {
    if (client.available() > 0) {
        String tmp = client.readString();
        
        if (tmp == null) return;
        if (tmp.isEmpty()) return;
        
        dataBuffer += tmp;
        
        String[] incoming = dataBuffer.split(JSON_STREAM_DELIMITER_AVYAY);
        
        if (dataBuffer.endsWith("<!>")) {
            if (DEBUG) {
              println("incoming.length: " + incoming.length);
            }
            for (int i = 0; i < incoming.length; i++) {
                JSONObject e = parseJSONObject(incoming[i]);
                processKeyEvent(e);
            }
            dataBuffer = "";
        } else {
            for (int i = 0; i < incoming.length - 1; i++) {
                JSONObject e = parseJSONObject(incoming[i]);
                processKeyEvent(e);
            }
            dataBuffer = incoming[incoming.length - 1];
        }
    }
}


void processKeyEvent(JSONObject object) {    
    if (active_movies.size() < MILES_LIMIT) {
        JSONArray colorBears = object.getJSONArray("color");
        JSONArray arr = object.getJSONArray("notes_pressed");

        if (colorBears != null && colorBears.size() >= 3) {
            int r = colorBears.getInt(0);
            int g = colorBears.getInt(1);
            int b = colorBears.getInt(2);

            current_color[0] = (double) r;
            current_color[1] = (double) g;
            current_color[2] = (double) b;

            for (int i = 0; i < arr.size(); i++) {
                JSONArray note = arr.getJSONArray(i);
                String pitch = note.getString(0); //note 
                int velocity = note.getInt(1); //velocity
                
                if (DEBUG) {
                    println("Playing " + pitch + " with velocity " + velocity + "fr: " + frameRate);
                }

                playMovie(pitch, velocity, r, g, b);
            }
        }

        JSONArray arr2 = object.getJSONArray("notes_released");
        
        for (int i = 0; i < arr2.size(); i++) {
            String note = arr2.getString(i);    
            println("Releasing " + note);
            releaseMovies(note);
        }
    }
}

void movieEvent(Movie m) {
    m.read();
}

int offset(int range) {
    return (int) random(-range/2, range/2);
}

boolean getFullScreen(String pitch) {
    return (notegroup_map.get(pitch) == 1);
}

void playMovie(String source_note, int velocity, int r, int g, int b) {
    String name = source_note + str(millis());
    try {
        String filename = colinMovieNames.getMovie(source_note, velocity);
        boolean fullScreen = colinMovieNames.getFullScreen(source_note);

        if (filename.endsWith(".png")) {
            active_movies.put(name, new colinImage(this, filename, velocity, r + offset(6), g + offset(6), b + offset(6), fullScreen));
        } else { // .mov
            active_movies.put(name, new colinMovie(this, filename, velocity, r + offset(6), g + offset(6), b + offset(6), fullScreen));
        }
    } catch (NullPointerException e) {
        if (DEBUG) {
            println("Error, note out of range!");
        }
    }
}

void releaseMovies(String note) {
    for (String key: active_movies.keySet()) {
        if (key.startsWith(note)) {
            colinMovie movie = active_movies.get(key);
            movie.setTargetColor(current_color[0], current_color[1], current_color[2]);
        }
    }
}

boolean doesFileExist(String filePath) {
    return new File(dataPath(filePath)).exists();
}


void keyPressed() {
    if (key == 'q') {
        active_movies.clear();
        String[] incoming = dataBuffer.split(JSON_STREAM_DELIMITER_AVYAY);
        if (incoming.length != 0)
            dataBuffer = incoming[incoming.length - 1];
    }
}
