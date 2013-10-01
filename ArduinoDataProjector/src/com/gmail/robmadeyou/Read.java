package com.gmail.robmadeyou;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;

/**
 * Created with IntelliJ IDEA.
 * User: robert
 * Date: 18/08/13
 * Time: 00:50
 * To change this template use File | Settings | File Templates.
 */
public class Read {
    public static boolean allRead = false;

    public static void read(String filedir){


        BufferedReader br = null;

        try{
            String currentLine;
            br = new BufferedReader(new FileReader(filedir));
            while((currentLine = br.readLine()) != null){
                Data.totalData.add(Double.parseDouble(currentLine));
            }

            allRead = true;
        }catch(IOException e){
            e.printStackTrace();
        }finally {
            try{
                if(br != null) br.close();
            }catch(IOException ex){
                ex.printStackTrace();
            }
        }
    }
}
