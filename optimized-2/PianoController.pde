
class PianoController {

    /* JSON client stuff */
    private final static String JSON_STREAM_DELIMITER = "<!>";

    private Client client;
    private String data_buffer;

    /* internal lists of filenames of unapplied key presses and key releases */
    private ArrayList<ColinMovie> pressed_keys;
    private ArrayList<String> released_keys;
    private MovieGroups movie_groups;
    // private ArrayList<MovieGroups> movie_groups_arr;
    private HashMap<String, Integer> folder_nums;

    private double[] current_color;
    
    private optimized2 parent;

    public PianoController(optimized2 parent) {
        this.parent = parent;
        client = new Client(parent, "127.0.0.1", 3000);
        data_buffer = "";
        current_color = new double[]{0, 0, 0};

        pressed_keys = new ArrayList<ColinMovie>();
        released_keys = new ArrayList<String>();

        // movie_groups_arr = new ArrayList<MovieGroups>();
        // for (int i = 0; i < MOVIE_FOLDER_COPIES; i++) {
        //     movie_groups_arr.add(new MovieGroups(i));
        // }
        movie_groups = new MovieGroups();

        folder_nums = new HashMap<String, Integer>();
        /* Populate folder_nums with index 0. Will be incremented when a duplicate movie is attempted to be played */
        File data_folder = new File("C:/Users/Transient/Documents/colinvideos_october_copy0");
        File[] file_list = data_folder.listFiles();
        for (int i = 0; i < file_list.length; i++) {
            if (file_list[i].isFile()) {
                String filename = file_list[i].getName();
                folder_nums.put(filename, 0);
            }
        }
    }

    /* Update internal pressed_keys and released_keys. Return true if a message was read */
    public boolean readClient() {
        if (client.available() == 0) return false;

        String tmp = client.readString();

        if (tmp == null) return false;
        if (tmp.isEmpty()) return false;

        data_buffer += tmp;

        String[] incoming = data_buffer.split(JSON_STREAM_DELIMITER);

        if (data_buffer.endsWith(JSON_STREAM_DELIMITER)) { // message was not split
            for (int i = 0; i < incoming.length; i++) {
                JSONObject e = parseJSONObject(incoming[i]);
                processKeyEvent(e);
            }
            data_buffer = "";
        } else {
            for (int i = 0; i < incoming.length - 1; i++) {
                JSONObject e = parseJSONObject(incoming[i]);
                processKeyEvent(e);
            }
            data_buffer = incoming[incoming.length - 1];
        }
        return true;
    }

    /* Process a JSON read event and update keys pressed or released with the event */
    private void processKeyEvent(JSONObject object) {
        JSONArray colorBears = object.getJSONArray("color");
        JSONArray arr = object.getJSONArray("notes_pressed");

        if (colorBears != null && colorBears.size() >= 3) {
            int r = colorBears.getInt(0);
            int g = colorBears.getInt(1);
            int b = colorBears.getInt(2);

            current_color[0] = (double) r;
            current_color[1] = (double) g;
            current_color[2] = (double) b;

            // println("Set current color to ", r, g, b);

            for (int i = 0; i < arr.size(); i++) {
                JSONArray note = arr.getJSONArray(i);
                String pitch = note.getString(0);
                int velocity = note.getInt(1);

                String filename = movie_groups.getFileName(pitch, velocity);
                if (filename == null) {
                  println("Skipping this note");
                  continue;
                }
                boolean fullscreen = movie_groups.isFullscreen(pitch);

                /*  Because the folder index is specific to each individual file, it's okay to increment it every time the file is played.
                    This saves the time it would take to check for the filename in active_movies. */
                int old_folder_num = folder_nums.get(filename);
                int new_folder_num = (old_folder_num + 1) % MOVIE_FOLDER_COPIES;
                folder_nums.replace(filename, new_folder_num);
                String filepath = "C:/Users/Transient/Documents/colinvideos_october_copy" + Integer.toString(new_folder_num) + "/" + filename;

                ColinMovie new_movie = new ColinMovie(this.parent, filepath, pitch, velocity, r + offset(6), g + offset(6), b + offset(6), fullscreen);
                pressed_keys.add(new_movie);
            }
        }

        JSONArray arr2 = object.getJSONArray("notes_released");
        for (int i = 0; i < arr2.size(); i++) {
            String note = arr2.getString(i);
            released_keys.add(note);
        }
    }

    /* provide a random +- number with a given range. Used for color randomization */
    private int offset(int range) {
        return int(random(-range/2, range/2));
    }

    public double[] getCurrentColor() {
        return current_color;
    }

    /* Return pressed_keys movie list and clear it */
    public ArrayList<ColinMovie> getKeysPressed() {
        ArrayList<ColinMovie> pressed_keys_out = (ArrayList<ColinMovie>) pressed_keys.clone();
        pressed_keys.clear();
        return pressed_keys_out;
    }

    /* Return released_keys filename list and clear it */
    public ArrayList<String> getKeysReleased() {
        ArrayList<String> released_keys_out = (ArrayList<String>) released_keys.clone();
        released_keys.clear();
        return released_keys_out;
    }

    /* Maps pitches to pitch groups, which are stored private to MovieGroups. */
    private class MovieGroups {

        /* HashMap of note Strings (e.g. "C4") to note group number (int from 1 to 4) */
        private HashMap<String, Integer> note_group_map;
        /* ArrayList of PitchGroup objects. */
        private ArrayList<PitchGroup> pitch_groups;

        /* Constructor */
        public MovieGroups() {

            /* Initialize the map of note Strings to pitch group numbers */
            note_group_map = new HashMap<String, Integer>();
            note_group_map.put("E1",  1);
            note_group_map.put("F1",  1);
            note_group_map.put("F#1",  1);
            note_group_map.put("G1",  1);
            note_group_map.put("G#1",  1);
            note_group_map.put("A1",  1);
            note_group_map.put("A#1",  1);
            note_group_map.put("B1",  1);
            note_group_map.put("C2",  1);
            note_group_map.put("C#2", 1);
            note_group_map.put("D2",  1);
            note_group_map.put("D#2", 1);
            note_group_map.put("E2",  1);
            note_group_map.put("F2",  1);
            note_group_map.put("F#2", 1);
            note_group_map.put("G2",  1);
            note_group_map.put("G#2", 1);
            note_group_map.put("A2",  1);
            note_group_map.put("A#2", 1);
            note_group_map.put("B2",  1);
            note_group_map.put("C3",  2);
            note_group_map.put("C#3", 2);
            note_group_map.put("D3",  2);
            note_group_map.put("D#3", 2);
            note_group_map.put("E3",  2);
            note_group_map.put("F3",  2);
            note_group_map.put("F#3", 2);
            note_group_map.put("G3",  2);
            note_group_map.put("G#3", 2);
            note_group_map.put("A3",  2);
            note_group_map.put("A#3", 2);
            note_group_map.put("B3",  2);
            note_group_map.put("C4",  2);
            note_group_map.put("C#4", 2);
            note_group_map.put("D4",  2);
            note_group_map.put("D#4", 2);
            note_group_map.put("E4",  2);
            note_group_map.put("F4",  2);
            note_group_map.put("F#4", 2);
            note_group_map.put("G4",  2);
            note_group_map.put("G#4", 3);
            note_group_map.put("A4",  3);
            note_group_map.put("A#4", 3);
            note_group_map.put("B4",  3);
            note_group_map.put("C5",  3);
            note_group_map.put("C#5", 3);
            note_group_map.put("D5",  3);
            note_group_map.put("D#5", 3);
            note_group_map.put("E5",  3);
            note_group_map.put("F5",  3);
            note_group_map.put("F#5", 3);
            note_group_map.put("G5",  3);
            note_group_map.put("G#5", 3);
            note_group_map.put("A5",  4);
            note_group_map.put("A#5", 4);
            note_group_map.put("B5",  4);
            note_group_map.put("C6",  4);
            note_group_map.put("C#6", 4);
            note_group_map.put("D6",  4);
            note_group_map.put("D#6", 4);
            note_group_map.put("E6",  4);
            note_group_map.put("F6",  4);
            note_group_map.put("F#6", 4);
            note_group_map.put("G6",  4);
            note_group_map.put("G#6", 4);
            note_group_map.put("A6",  4);
            note_group_map.put("A#6", 4);
            note_group_map.put("B6",  4);
            note_group_map.put("C7",  4);
            note_group_map.put("C#7",  4);
            note_group_map.put("D7",  4);
            note_group_map.put("D#7",  4);
            note_group_map.put("E7",  4);
            note_group_map.put("F7",  4);
            note_group_map.put("F#7",  4);
            note_group_map.put("G7",  4);

            /* Initialize the list of PitchGroup objects */
            pitch_groups = new ArrayList<>(Arrays.asList(
                new PitchGroup(1),
                new PitchGroup(2),
                new PitchGroup(3),
                new PitchGroup(4)
            ));
        }

        /* Get a file name corresponding to a given pitch and velocity */
        public String getFileName(String pitch, int velocity) {
            int pitch_group_num;
          try {
              pitch_group_num = note_group_map.get(pitch) - 1;
            } catch (NullPointerException e) {
              println("Pitch out of range!");
              return null;
            }
            PitchGroup pitch_group = pitch_groups.get(pitch_group_num);
            return pitch_group.getFileName(velocity);
        }

        /* Return whether a movie/image within a given pitch group should be fullscreen */
        public boolean isFullscreen(String pitch) {
            return (note_group_map.get(pitch) == 1);
        }

        /* Maps a velocity to a list of filenames for corresponding movie files. */
        private class PitchGroup {

            /* This PitchGroup instance's pitch group number */
            private int pitch_group_num;
            /* ArrayList of the velocity groups contained within this pitch group. */
            private ArrayList<ArrayList<String>> velocity_groups;

            public PitchGroup(int pitch_num) {
                this.pitch_group_num = pitch_num;
                /* A velocity group is a list of filenames that correspond to the selected pitch and veloicty group numbers */
                velocity_groups = new ArrayList<>(Arrays.asList(
                    new ArrayList<String>(),
                    new ArrayList<String>(),
                    new ArrayList<String>(),
                    new ArrayList<String>()
                ));
                
                /* Read file names */
                File data_folder = new File("C:/Users/Transient/Documents/colinvideos_october_copy0");
                File[] file_list = data_folder.listFiles();

                /* Assign file names to velocity groups */
                for (int i = 0; i < file_list.length; i++) {
                    if (file_list[i].isFile()) {
                        String filename = file_list[i].getName();
                        if (file_list[i].getName().length() > 13) {
                            int pitch_group_num = Integer.parseInt(filename.substring(5,6));
                            int vel_num = Integer.parseInt(filename.substring(10,11));
                            if (pitch_group_num == this.pitch_group_num) {
                                velocity_groups.get(vel_num-1).add(filename);
                                 println("I just added ", filename, "to pg ", pitch_group_num, " vg ", vel_num);
                            }
                        }
                    }
                }
            }

            /* Get a file name corresponding to a given velocity within this PitchGroup */
            public String getFileName(int velocity) {
                ArrayList<String> velocity_group;

                /* Determine which velocity group to pick from */
                if (velocity < 50) {
                    velocity_group = velocity_groups.get(0);
                } else if (velocity < 65) {
                    velocity_group = velocity_groups.get(1);
                } else if (velocity < 80) {
                    velocity_group = velocity_groups.get(2);
                } else { // velocity < 128
                    velocity_group = velocity_groups.get(3);
                }
                
                /* Choose a random filename from the velocity group */
                int random_index = int(random(velocity_group.size()));
                return velocity_group.get(random_index);
            }

        }
    }

}
