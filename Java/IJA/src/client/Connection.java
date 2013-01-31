package client;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.Socket;
import java.net.UnknownHostException;

public class Connection {
	private Socket s;
	private BufferedReader in;
	private PrintStream out;

	public boolean open(String host, int port)
	{
		String response = "";
    	try {
    		s = new Socket(host, port);
    		out = new PrintStream(s.getOutputStream());
    		in = new BufferedReader(new InputStreamReader(s.getInputStream()));
    	}
    	catch(UnknownHostException exp)
    	{
    		exp.printStackTrace();
    		return false;
    	}
    	catch(java.io.IOException exp)
    	{
    		exp.printStackTrace();
    		return false;
    	}
        return true;
	}

	public String send(String packet)
	{
		String response = "";
		packet = packet.replaceAll("\n+", "");
		out.println(packet);
		out.flush();
		try {
			response = in.readLine();
		} catch (IOException e) {
			e.printStackTrace();
			return null;
		}
		return response;
	}

	public boolean close()
	{
		try {
    		out.close();
    		in.close();
    		s.close();
    	}
    	catch(UnknownHostException exp)
    	{
    		exp.printStackTrace();
    		return false;
    	}
    	catch(java.io.IOException exp)
    	{
    		exp.printStackTrace();
    		return false;
    	}
        return true;
	}
}