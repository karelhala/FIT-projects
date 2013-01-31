package server.threads;

import java.io.IOException;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Trido pro zavolani jednotlivych vlaken.
 * @author charlie
 */
public class caller implements Runnable{
   Socket s;
   final callMe target;
   public Thread t;

   /**
    * Funkce pro volani vlakna.
    * @param target volane vlakno.
    * @param s socket, na kterem prisla zprava.
    */
    public caller(callMe target, Socket s) {
      this.target = target;
      this.s = s;
      this.t = new Thread(this);
      t.start();
    }

    /**
     * Jednotliva vlakna.
     */
    public void run() {
        synchronized(target) { // synchronized block
            try {
                // synchronized block
                target.call(s);
            } catch (IOException ex) {
                Logger.getLogger(caller.class.getName()).log(Level.SEVERE, null, ex);
            }
      }
    }


}
