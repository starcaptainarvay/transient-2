
class PianoController {

    /* JSON client stuff */
    private final static String JSON_STREAM_DELIMITER = "<!>";

    private Client client;
    private String data_buffer;

    /* internal lists of filenames of unapplied key presses and key releases */
    private ArrayList<ColinMovie> pressed_keys;
    private ArrayList<String> released_keys;
    private MovieGroups movie_groups;

    private double[] current_color;
    
    private optimized2 parent;

    public PianoController(optimized2 parent) {
        this.parent = parent;
        client = new Client(parent, "127.0.0.1", 3000);
        data_buffer = "";
        current_color = new double[]{0, 0, 0};

        pressed_keys = new ArrayList<ColinMovie>();
        released_keys = new ArrayList<String>();
        movie_groups = new MovieGroups();
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

            for (int i = 0; i < arr.size(); i++) {
                JSONArray note = arr.getJSONArray(i);
                String pitch = note.getString(0);
                int velocity = note.getInt(1);

                String filename = movie_groups.getFileName(pitch, velocity);
                boolean fullscreen = movie_groups.isFullscreen(pitch);
                ColinMovie new_movie = new ColinMovie(this.parent, filename, velocity, r + offset(6), g + offset(6), b + offset(6), fullscreen);
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

    /* Return pressed_keys filename list and clear it */
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
            note_group_map = Map.of(
                "C2",  1, "C#2", 1, "D2",  1, "D#2", 1, "E2",  1, "F2",  1, "F#2", 1, "G2",  1, "G#2", 1, "A2",  1, "A#2", 1, "B2",  1, "C3",  1, "C#3", 1, "D3",  1, "D#3", 1, 
                "E3",  2, "F3",  2, "F#3", 2, "G3",  2, "G#3", 2, "A3",  2, "A#3", 2, "B3",  2, "C4",  2, "C#4", 2, "D4",  2, "D#4", 2, "E4",  2, "F4",  2, "F#4", 2, "G4",  2,
                "G#4", 3, "A4",  3, "A#4", 3, "B4",  3, "C5",  3, "C#5", 3, "D5",  3, "D#5", 3, "E5",  3, "F5",  3, "F#5", 3, "G5",  3, "G#5", 3,
                "A5",  4, "A#5", 4, "B5",  4, "C6",  4, "C#6", 4, "D6",  4, "D#6", 4, "E6",  4, "F6",  4, "F#6", 4, "G6",  4, "G#6", 4, "A6",  4, "A#6", 4, "B6",  4, "C7",  4
            );

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
            int pitch_group_num = note_group_map.get(pitch) - 1;
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
                File data_folder = new File("C:/colin-project/data/");
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
