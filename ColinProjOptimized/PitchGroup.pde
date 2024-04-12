
public class PitchGroup {
  
  private int groupNumber;
  
  private ArrayList<String> vel_1;
  private ArrayList<String> vel_2;
  private ArrayList<String> vel_3;
  private ArrayList<String> vel_4;
 
  public PitchGroup(int num) {
    
    if (DEBUG) {
      println("pitchgroup init, num = " + num);
    }
    
    groupNumber = num;
    
    vel_1 = new ArrayList<String>();
    vel_2 = new ArrayList<String>();
    vel_3 = new ArrayList<String>();
    vel_4 = new ArrayList<String>();
    
    File dataFolder = new File("C:/colin-project/Processing/videoTestNumbers/data");
    File[] listOfFiles = dataFolder.listFiles();
    
    for (int i = 0; i < listOfFiles.length; i++) {
      if (listOfFiles[i].isFile()) {
        String filename = listOfFiles[i].getName();
        if (listOfFiles[i].getName().length() > 13) {
          int groupNum = Integer.parseInt(filename.substring(5,6));
          int velNum = Integer.parseInt(filename.substring(10,11));
          if (groupNum == this.groupNumber) {
            if (velNum == 1) {
              vel_1.add(filename);
              println("Added " + filename + " to vel group 1, pitch group " + groupNumber);
            } else if (velNum == 2) {
              vel_2.add(filename);
              println("Added " + filename + " to vel group 2, pitch group " + groupNumber);
            } else if (velNum == 3) {
              vel_3.add(filename);
              println("Added " + filename + " to vel group 3, pitch group " + groupNumber);
            } else { // velNum == 4
              vel_4.add(filename);
              println("Added " + filename + " to vel group 4, pitch group " + groupNumber);
            }
          }
        }
      }
    }
  }
  
  public String getMovie(int velocity) {
    
    if (DEBUG) {
      println("called PitchGroup.getMovie(" + velocity + ") in pitch group " + groupNumber);
    }
    
    ArrayList<String> vel_group;

    if (velocity < 50) {
      vel_group = vel_1;
    } else if (velocity < 65) {
      vel_group = vel_2;
    } else if (velocity < 80) {
      vel_group = vel_3;
    } else { // velocity < 128
      vel_group = vel_4;
    }
    
    return vel_group.get(int(random(vel_group.size())));
  }
}
