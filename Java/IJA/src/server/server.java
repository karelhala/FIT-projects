/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package server;

import java.net.ServerSocket;
import java.net.Socket;

import server.threads.callMe;
import server.threads.caller;


/**
 * Trida server slouzi jako server pro simulaci siti
 * @author Libor
 * @author charlie
 */
public class server {
    
    public static void main(String[] args){
        
        boolean clientExists=false;
        System.out.println("server byl spusten!");
        ServerSocket ss = null;
        try {
        	ss = new ServerSocket(9001);
        	while(true) {
        		Socket s = ss.accept();
                        callMe target = new callMe();
                        caller ob1 = new caller(target, s);
        		
                        // wait for threads to end
                        try {
                            ob1.t.join();
                        } catch(InterruptedException e) {
                            System.out.println("Interrupted");
                        }        		
        	}

        }
        catch(java.io.IOException exp)
        {
        	exp.printStackTrace();
        }
    }
}

