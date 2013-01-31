package server.threads;

import com.thoughtworks.xstream.XStream;
import com.thoughtworks.xstream.io.xml.DomDriver;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Vector;

import petrinets.xmlpacket;
import petrinets.PetriNet;
import petrinets.filePath;
import server.User;

/**
 * Trida obsahujici jednotliva vlakna.
 * @author charlie
 */
public class callMe {
    void call(Socket s) throws IOException {

        xmlpacket MyData;
        BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
        PrintStream out = new PrintStream(s.getOutputStream());
        String inpacket;
        if((inpacket = in.readLine())!=null){
        //ulozeni nacteneho packetu
        MyData = xmlpacket.getData(inpacket);

        String login = MyData.getLogin();
        String passwd = MyData.getPasswd();
        String task = MyData.getTask();
        //registrace uzivatele
        if (task.equals("register")){
            if (SaveUserToFile(login, passwd))
                out.println("user succesfully added");
        }
        //kontrola jmena a hesla a pote provadeni dalsich ukonu
        else if (task.equals("log_me_in"))
        {
            ArrayList<User> users = GetUsersFromFile();
            for(User us: users)
            {
                if(us.isUserNameAndPasswordRight(login, passwd))
                {
                    out.println("logged");
                    out.flush();
                }
             }
        }
        else if (task.equals("simulate_one_step"))
        {
            PetriNet sit = MyData.getPetri();
            sit.simulateOneStep();
            Vector<filePath> message = displayFiles();
            if (MyData.getName()!=null)
                for (filePath paths : message)
                    for (String string : paths.getFiles())
                        if (string.equals(MyData.getName()))
                        {
                            SaveSimulated(login, paths.getFile());
                        }
            String petrinet = xmlpacket.createXMLAsString("result", sit);
            petrinet = petrinet.replaceAll("\n+", "");
            out.println(petrinet);
        }
        else if (task.equals("simulate"))
        {
            PetriNet sit = MyData.getPetri();
            sit.simulate();
            Vector<filePath> message = displayFiles();
            if (MyData.getName()!=null)
                for (filePath paths : message)
                    for (String string : paths.getFiles())
                        if (string.equals(MyData.getName()))
                        {
                            SaveSimulated(login, paths.getFile());
                        }

            String petrinet = xmlpacket.createXMLAsString("result", sit);
            petrinet = petrinet.replaceAll("\n+", "");
            out.println(petrinet);
        }
        else if(task.equals("save"))
        {
            int version=0;
            String writeFile = "dest-server/"+"serverNets/"+MyData.getName()+"/"+".info.txt";
            String[] fileInfo = saveInfo(MyData.getLogin(),writeFile);
            //System.out.println(fileInfo[1].substring(9,10));
            version = Integer.parseInt(fileInfo[1].substring(16));
            version++;
            String dataInfo = createInfo(fileInfo[0],login,MyData.getName(),version);
            if (SaveToFile(dataInfo,MyData.getName(),MyData.getPetri(),version))
                {out.println("file has been saved");}
            else
                {out.println("error");}
        }
        else if(task.equals("load"))
        {
            Vector<filePath> message = displayFiles();
            String string = xmlpacket.createXML(message);
            string = string.replaceAll("\n", "\t");
            out.println(string);
        }
        else if(task.equals("load_petri")){
            PetriNet MyPetri = OpenFile(MyData.getPath(),MyData.getFile());
            String[] info = saveInfo("Anonymous","dest-server/serverNets/"+MyData.getPath()+"/"+".info.txt");
            String joined="";
            for (String pom:info)
                joined+=pom+"\n";
            System.out.println(joined);
            String simulated = openSimulated(MyData.getPath());
            String message = xmlpacket.createXMLAsString("this_file",MyPetri,joined,simulated);
            message = message.replaceAll("\n+", "\t");
            out.println(message);
        }
        else if(task.equals("run_petri")){
            Vector<filePath> message = displayFiles();
            for (filePath paths : message)
                if (paths.getFile().equals(MyData.getName()))
                {
                    SaveSimulated(login, paths.getFile());
                    String writeFile = "dest-server/"+"serverNets/"+paths.getFile()+"/"+".info.txt";
                    String[] fileInfo = saveInfo(MyData.getLogin(),writeFile);
                    int version = Integer.parseInt(fileInfo[1].substring(16));
                    version++;
                    PetriNet MyPetri = OpenFile(paths.getFile(),paths.getLast());
                    MyPetri.simulate();
                    String dataInfo = createInfo(fileInfo[0],login,paths.getFile(),version);
                    if (SaveToFile(dataInfo,paths.getFile(),MyPetri,version))
                        {out.println("file has been saved");}
                    else
                        {out.println("error");}
                }
        }
        out.close();
        in.close();
        s.close();
   }
}
    /**
     * Funkce na ulozeni uzivatelu, kteri simulovali nejakou sit, ktera je ulozena na serveru
     * @param login jmeno uzivatee
     * @param path slozka ve ktere je ulozen soubor
     * @return true pokud se povede
     */
    private boolean SaveSimulated(String login,String path)
    {
        System.out.println(login);
        System.out.println(path);
        String OutData="";
        try{
            FileInputStream fstream = new FileInputStream("dest-server/"+"serverNets/"+path+"/"+".simulated.txt");
            DataInputStream in = new DataInputStream(fstream);
            BufferedReader br = new BufferedReader(new InputStreamReader(in));
            String strLine="";

            while ((strLine = br.readLine()) != null)   {
              // Print the content on the console
              OutData+=strLine+"\n";
            }
        }
        catch (IOException e){
           System.out.println("soubor s uzivateli neexistuje");
        }
        Calendar cal = Calendar.getInstance();
        OutData+=login+":"+cal.getTime();
        try {
            String writeFile = "dest-server/serverNets/"+path+"/"+".simulated.txt";
            BufferedWriter MyOut = new BufferedWriter(new FileWriter(writeFile));
            MyOut.write(OutData);
            MyOut.close();
        }
        catch (IOException event)
        {
                event.printStackTrace();
        }
        return true;
    }
    
    /**
     * Funkce na ulozeni noveho uzivatele do souboru
     * @param name jmeno uzivatele
     * @param password plain heslo
     * @return true, pokud se povedlo
     */
    private boolean SaveUserToFile(String name,String password)
    {
        String OutData="";
        try {
              boolean success = (new File("dest-server/"+"serverNets/")).mkdirs();
              if (success) {
              System.out.println("Directory: "+ "dest-server" + " created");
              }
              else
              System.out.println("folder allready created");
        }
         catch (Exception e) {
                e.printStackTrace();
        }
        try{
            FileInputStream fstream = new FileInputStream("dest-server/"+"serverNets/.users.txt");
            DataInputStream in = new DataInputStream(fstream);
            BufferedReader br = new BufferedReader(new InputStreamReader(in));
            String strLine="";

            while ((strLine = br.readLine()) != null)   {
              // Print the content on the console
              OutData+=strLine+"\n";
            }
        }
        catch (IOException e){
           System.out.println("soubor s uzivateli neexistuje");
        }
        ArrayList<User> users = GetUsersFromFile();
        for(User us: users)
        {
            if (us.getLogin().equals(name))
                return false;
        }
        OutData +=name + ":" + password;
        try {
            String writeFile = "dest-server/serverNets/.users.txt";
            BufferedWriter MyOut = new BufferedWriter(new FileWriter(writeFile));
            MyOut.write(OutData);
            MyOut.close();
        }
        catch (IOException event)
        {
                event.printStackTrace();
        }
        return true;
    }
    
    /**
     * Funkce na ziskani uzivatelu a hesel ze souboru
     * @return seznam uzivatelu
     */
    private ArrayList<User> GetUsersFromFile()
    {
    	ArrayList<User> users = new ArrayList<User>();
    	String line = "";
        try {
        	String readFile = "dest-server/serverNets/.users.txt";
            BufferedReader reader = new BufferedReader(new FileReader(readFile));
            while((line = reader.readLine()) != null)
            {
            	String[] datas = line.split(":");
            	User user = new User(datas[0],datas[1]);
            	users.add(user);
            }

            reader.close();
        }
        catch (IOException e)
        {
        	System.out.println("chyba");
        }
        return users;
    }
    /**
     * Funkce na ulozeni nove verze site
     * @param info informace k ulozeni o siti
     * @param name jmeno uzivatele, ktery uklada novou verzi
     * @param Mypetri ukladana sit
     * @param version cislo verze
     * @return true, pokud se povedlo
     */
    private boolean SaveToFile(String info,String name,PetriNet Mypetri,int version)
    {
        System.out.println(name);
        try {
              boolean success = (new File("dest-server/serverNets")).mkdirs();
              if (success) {
              System.out.println("Directory: "+ "dest-server" + " created");
              }
              else
              System.out.println("folder allready created");
        }
         catch (Exception e) {
                e.printStackTrace();
                return false;
        }
        try{
           boolean success = (new File("dest-server/serverNets/"+name)).mkdir();
           if (success) {
              System.out.println("Directory: "+ name + " created");
              }
           else
              System.out.println("folder allready created");
        }
        catch (Exception e){
                e.printStackTrace();
                return false;
        }
        if (Mypetri!=null){
            XStream xstream = new XStream(new DomDriver());
            String Myxml = xstream.toXML(Mypetri);
            try {
                String writeFile = "dest-server"+"/"+"serverNets/"+name+"/"+".info.txt";
                BufferedWriter MyOut = new BufferedWriter(new FileWriter(writeFile));
                info+=Mypetri.getInfo();
                MyOut.write(info);
                MyOut.close();
            }
            catch (IOException e)
            {
                    System.out.println("Exception ");
                    return false;
            }
            try {
                String writeFile = "dest-server"+"/"+"serverNets/"+name+"/"+name+"-"+String.valueOf(version)+".xml";
                BufferedWriter MyOut = new BufferedWriter(new FileWriter(writeFile));
                MyOut.write(Myxml);
                MyOut.close();
            }
            catch (IOException e)
            {
                    System.out.println("Exception ");
                    return false;
            }

        }
    return true;
    }

    /**
     * Vytvoreni informaci o nove vznikle siti
     * @param author jmeno autora
     * @param file umistneni souboru
     * @return [suthor: jmeno,Version:cislo,[]]
     */
    private String[] saveInfo(String author,String file)
    {
        String OutData="";
        try{
            FileInputStream fstream = new FileInputStream(file);
            DataInputStream in = new DataInputStream(fstream);
            BufferedReader br = new BufferedReader(new InputStreamReader(in));
            String strLine="";
            while ((strLine = br.readLine()) != null)   {
              // Print the content on the console
              OutData+=strLine+"\n";
            }
        }
        catch (Exception e)
        {
            OutData = "author: "+author+"\n"+"Latest version: "+"0";
        }
        return OutData.split("\n");
    }

    /**
     * Funkce na vytvoreni informaci o siti
     * @param author autor site
     * @param login stavajici uzivatel
     * @param name jmeno site
     * @param version posledni verze
     * @return navraci string obsahujuci tyto informace
     */
    private String createInfo(String author,String login, String name, int version) {
        Calendar cal = Calendar.getInstance();
        return (author+"\n"+"Latest version: "+String.valueOf(version)+"\n"+"Last changed by: "+login+"\n"+"PetriNet's Name: "+name+"\n"+"Last changed: "+cal.getTime());
    }

    /**
     * Funkce na zobrazeni souboru na serveru
     * @return vraci vektor siti
     * @throws IOException
     */
    private Vector<filePath> displayFiles()
    {
        Vector<filePath> myFiles = new Vector<filePath>();
        try{
        File f = new File("dest-server/serverNets/");
        File[] files = f.listFiles();
        String path="";
        for (File file : files) {
            if (file.isDirectory()) {
                path = file.getCanonicalPath();
                int index = path.lastIndexOf("serverNets");
                if (path.contains("serverNets"))
                    path = path.substring(index+11);
                System.out.println(path);
                File directory = new File("dest-server/serverNets/"+path+"/");
                if (directory.listFiles()==null)
                    return null;
                File[] nets = directory.listFiles();
                Vector<String> pom= new Vector<String>();
                for (File net : nets) {
                    if (net.isFile())
                    {
                        String petriNetFile="";
                        petriNetFile=net.getCanonicalPath();
                        if (petriNetFile.contains("dest-server/serverNets/"+path+"/"))
                        {
                            String filePath=petriNetFile.substring(petriNetFile.lastIndexOf("dest-server/serverNets/"+path+"/")+path.length()+24);
                            if (!filePath.substring(0, 1).equals("."))
                                pom.add(petriNetFile.substring(petriNetFile.lastIndexOf("dest-server/serverNets/"+path+"/")+path.length()+24));
                        }
                        else if (petriNetFile.contains("dest-server\\serverNets\\"+path))
                        {
                            String filePath=petriNetFile.substring(petriNetFile.lastIndexOf("dest-server\\serverNets\\"+path)+path.length()+24);
                                if (!filePath.substring(0, 1).equals("."))
                            pom.add(petriNetFile.substring(petriNetFile.lastIndexOf("dest-server\\serverNets\\"+path)+path.length()+24));
                        }
                        else
                            pom.add(petriNetFile);
                    }
                }
                String writeFile = "dest-server/"+"serverNets/"+path+"/"+".info.txt";
                String[] fileInfo = saveInfo("None",writeFile);
                String simulated = openSimulated(path);
                String lastVersion = path+"-"+ fileInfo[1].substring(16)+".xml";
                String joined="";
                for (String string:fileInfo)
                    joined+=string+"\n";
                filePath info = new filePath(path, pom, lastVersion,joined,simulated);
                myFiles.add(info);
            }
        }

        }catch (IOException e){
            return null;
        }
        return myFiles;
    }
    /**
     * Funkce na ziskani informaci, kdo a kdy simuloval.
     * @param path cesta k souboru.
     * @return vraci obsah souboru .simulated.txt
     */
    private String openSimulated(String path)
    {
        String OutData="";
        String readPath = "dest-server/serverNets/"+path+"/"+".simulated.txt";
        try{
            FileInputStream fstream = new FileInputStream(readPath);
            DataInputStream in = new DataInputStream(fstream);
            BufferedReader br = new BufferedReader(new InputStreamReader(in));
            String strLine="";
            while ((strLine = br.readLine()) != null)   {
              // Print the content on the console
              OutData+=strLine+"\n";
            }
        }
        catch (IOException e)
        {
            return "Not simulated";
        }

        return OutData;
    }
    /**
     * Funkce na otevreni presne dane site
     * @param path jmeno site
     * @param file jmeno verze
     * @return vraci petriho sit
     */
    private PetriNet OpenFile(String path,String file)
    {
        String OutData="";
        try{
            FileInputStream fstream = new FileInputStream("dest-server/serverNets/"+path+"/"+file);
            DataInputStream in = new DataInputStream(fstream);
            BufferedReader br = new BufferedReader(new InputStreamReader(in));
            String strLine="";
            while ((strLine = br.readLine()) != null)   {
              // Print the content on the console
              OutData+=strLine+"\n";
            }
        }
        catch (Exception e)
        {
            System.err.println("Error while working with files on server");
        }
        return xmlpacket.getPetri(OutData);
    }
}
