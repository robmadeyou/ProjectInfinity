package com.gmail.robmadeyou;

import java.awt.*;
import javax.swing.*;

/**
 * Created with IntelliJ IDEA.
 * User: robert
 * Date: 17/08/13
 * Time: 23:54
 * To change this template use File | Settings | File Templates.
 */
public class Main {
    public static void main(String args[]){
        Screen.create(500, 500, "Screen", Screen.GameType.CUSTOM, false);

        Read.read("/home/robert/Code/Arduino/ProjectInfinity/DataProjector/ArduinoDataProjector/Data/test.txt");
        Data.sort();
        while(!Screen.isAskedToClose()){
            Screen.update(60);
            try{
                Thread.sleep(1000);
            }catch(InterruptedException e){
            }
            System.out.println("test");

            Screen.refresh();
        }
    }
}
