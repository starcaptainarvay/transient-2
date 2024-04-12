
public class ColinMovieNames {
  
  private HashMap<String, Integer> noteGroupMap;
  private ArrayList<PitchGroup> pitchGroups;
 
  public ColinMovieNames() {
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
  }
  
  public String getMovie(String pitch, int velocity) {
    if (DEBUG) {
      println("called ColinMovieNames.getMovie( " + pitch + ", " + velocity + ")");
    }
    pg = noteGroupMap.get(pitch);
    return pitchGroups.get(pg).getMovie(velocity);
    // missing error handling
  }

  public 
  
}
