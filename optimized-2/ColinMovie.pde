
import processing.video.*;
import java.util.Map;
import java.util.HashMap;

public class ColinMovie {
    private Movie movie;
    private int opacity;
    private int x;
    private int y;
    private float extra;
    protected float timeScale;
     protected boolean fullScreen;
    
    private double r;
    private double g;
    private double b;
    
    private double r1;
    private double g1;
    private double b1;

    // new for transient 2
    public String filename;
    
    public ColinMovie(PApplet papp, String filename, String pitch, int velocity, int r, int g, int b/*, boolean fullScreen*/) {
        this.filename = filename;
        setColor(r, g, b);
        setTargetColor(r + velocity * .25, g + velocity * .25, b + velocity * .25);
        start(papp, filename, pitch, velocity/*, fullScreen*/);
        fullScreen = false;
    }

    protected void start(PApplet papp, String filename, String pitch, int velocity/*, boolean fullScreen*/) {
    //   println("Looking up movie with filename: ", filename); 
        movie = new Movie(papp, filename); 
        timeScale = Math.min(((float) velocity)/127 * 2 + .4, 4);
        //if (DEBUG) {
        //  println(this.timeScale);
        //}
        String octave_str = pitch.substring(pitch.length() - 1);
        int octave_int = Integer.parseInt(octave_str) - 2; // minus 2 to map octave 2 to 0, being furthest to the left. The highest octave, 6, becomes 4. NOTE: THIS WILL CHANGE WHEN WE ADD THE NEW KEYS
        int minX = (int) ((WIDTH - MOVIE_WIDTH)/5) * octave_int + MOVIE_WIDTH/2;
        x = (int) random(minX, minX + ((WIDTH - MOVIE_WIDTH)/5) );
        //x = ((int) random(MOVIE_WIDTH/2, WIDTH - MOVIE_WIDTH/2));
        y = ((int) random(MOVIE_HEIGHT/2, HEIGHT - MOVIE_HEIGHT/2));
        extra = random(4);

        movie.noLoop();
        // movie.play(); // commented out for transient 2
        // this.fullScreen = fullScreen;
    }

    // added for transient 2
    public void play() {
        movie.play();
    }
    
    protected double time() {
        return movie.time() * timeScale;
    }
    
    protected double duration() {
        return movie.duration();
    }
    
    protected double progress() {
        return time()/duration()/timeScale; 
    }
    
    protected int calcOpacity() {
        return (int) (255 * (-16 * Math.pow(time()/duration() - .5, 4) + 1));
    }
    
    public void setColor(int r, int g, int b) {
        this.r = (double) r;
        this.g = (double) g;
        this.b = (double) b;
    }
    
    public void setTargetColor(double r1, double g1, double b1) {
        this.r1 = r1;
        this.g1 = g1;
        this.b1 = b1;
    }

    private double lerp(double a, double b, double n) {
        return a + ((b - a) * n);
    }

    private void lerpColor(double n) {
        r = lerp(r, r1, n);
        g = lerp(g, g1, n);
        b = lerp(b, b1, n);
    }

    public void updateColor() {
        tint((int) r, (int) g, (int) b, calcOpacity());
        lerpColor(.2);
    }

    public void drawMovie() {
        updateColor();

        try {
            if (fullScreen) {
                // scale(2);
                image(movie, width/2, height/2);
                // scale(0.5);
            } else {
                scale(0.333);
                image(movie, x*3, y*3);//, (int) (72 + extra * 72), (int) (128 + extra * 128));
                scale(3);
            }
        } catch (ArrayIndexOutOfBoundsException e) {
            //if (DEBUG) {
            //  println("Daaayyuu- aw...");
            //}
        }
    }

    public boolean isOver() {
        if (progress() >= 0.8){
            movie.pause();
            movie.dispose();
            return true;
        } 
        return false;
    }
}

public class ColinImage extends ColinMovie {
    protected float timeScale;
    private PImage image;
    private int startMillis;
    private int x;
    private int y;

    public ColinImage(PApplet papp, String note, String pitch, int velocity, int r, int g, int b/*, boolean fullScreen*/) {
        super(papp, note, pitch, velocity, r, g, b/*, fullScreen*/);
    }

    @Override
    protected void start(PApplet papp, String note, String pitch, int velocity/*, boolean fullScreen*/) {
        image = loadImage(note);
        timeScale = Math.min(((float) velocity)/127 * 2 + .4, 4);
        startMillis = millis();
        // this.fullScreen = true;
        String octave_str = pitch.substring(pitch.length() - 1);
        int octave_int = Integer.parseInt(octave_str) - 2; // minus 2 to map octave 2 to 0, being furthest to the left. The highest octave, 6, becomes 4. NOTE: THIS WILL CHANGE WHEN WE ADD THE NEW KEYS
        int minX = (int) ((WIDTH - MOVIE_WIDTH)/5) * octave_int + MOVIE_WIDTH/2;
        x = (int) random(minX, minX + ((WIDTH - MOVIE_WIDTH)/5) );
        //x = ((int) random(MOVIE_WIDTH/2, WIDTH - MOVIE_WIDTH/2));
        y = ((int) random(MOVIE_HEIGHT/2, HEIGHT - MOVIE_HEIGHT/2));
        //extra = random(4);
    }

    @Override
    public void drawMovie() {
        updateColor();
        // scale(2);
        scale(0.5);
        image(image, x*2, y*2);
        scale(2);
        // scale(0.5);
    }

    @Override
    protected double progress() {
        return time()/duration()/timeScale; 
    }

    @Override
    protected double time() {
        return ((millis()/1000.0) - (startMillis/1000.0)) * timeScale;
    }
    
    @Override
    protected double duration() {
        return 2;
    }

    @Override
    public boolean isOver() {
      return (progress() >= 0.8);
    }

    @Override
    public void play() {
        return;
    }
    
    @Override
    protected int calcOpacity() {
        return (int) (255 * (-16 * Math.pow(time()/duration()/* - .5*/, 4) + 1));
    }
}
