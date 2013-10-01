package com.gmail.robmadeyou;



import com.gmail.robmadeyou.Draw.Box;
import com.gmail.robmadeyou.Effects.Color;
import com.gmail.robmadeyou.Gui.Text;
import com.gmail.robmadeyou.Input.Mouse;
import com.gmail.robmadeyou.World.Camera;

/**
 * Created with IntelliJ IDEA.
 * User: robert
 * 
 * Date: 17/08/13
 * Time: 23:54
 * To change this template use File | Settings | File Templates.
 */
public class Main {
    public static void main(String args[]){
        Screen.create(400, 400, "Screen", Screen.GameType.CUSTOM, false);
        
        Camera cam = Engine.addNewCamera(new Camera(0,0, 0, 0, Screen.getWidth(), Screen.getHeight()));
        Read.read("Data/DATA.TXT");
        Data.sort();
        while(!Screen.isAskedToClose()){
        	int second = 0;
        	int minute = 0;;
        	int hour = 0;
        	double maxLight = 0;
        	double minLight = 100;
        	
        	double minHumidity = 1000;
        	double maxHumidity = 0;
        	double averageSpeed = 0;
        	
        	double maxSpeed = 0;
        	double minSpeed = 1000;
        	double total = 0;
        	int start = 2100;
        	for(int i = start; i < Data.humData.size(); i++){
        		second++;
        		if(second >= 60){
        			second = 0;
        			minute++;
        		}
        		if(minute >= 60){
        			minute = 0;
        			hour++;
        		}
        		
        		Screen.update(60);
        		try{
        			Thread.sleep(000);
        		}catch(InterruptedException e){
        		}
        		averageSpeed = Math.round(Data.altData.get(i) - Data.altData.get(i - 1));
        		if(averageSpeed != 0)
        		System.out.println(averageSpeed);
        		if(maxSpeed < averageSpeed && averageSpeed > 0 && averageSpeed <= 1000){
        			total += averageSpeed;
        		}
        		if(minSpeed > averageSpeed && averageSpeed != 0){
        			minSpeed = averageSpeed;
        		}
    			Text.drawString("fps: ^5" + Screen.actualFps, Mouse.getTranslatedX() + 10, Mouse.getTranslatedY() + 20, 1, 1, 1, Color.White, true, false);
    			Text.drawString("Frame: ^5"+ i, 40, 5, 1, 1f, 1.0, Color.White, false, false);
    			Text.drawString("Average Speed: ^5"+ Math.round(total / (i - start))+ "^3 meters per second", 40, 20, 1, 1f, 1.0, Color.White, false, false);
        		Text.drawString("Lat: ^5"+Data.latData.get(i), 40, 40, 1, 1, 1, Color.White,false, false);
        		Text.drawString("Long: ^5"+Data.longData.get(i), 40, 55, 1, 1f, 1.0, Color.White, false, false);
        		Text.drawString("Alt: ^5"+Data.altData.get(i), 40, 70, 1, 1f, 1.0, Color.White, false, false);
        		Text.drawString("Temp: ^5"+Data.tempData.get(i), 40, 85, 1, 1f, 1.0, Color.White, false, false);
        		Text.drawString("Hum: ^5"+Data.humData.get(i), 40, 100, 1, 1f, 1.0, Color.White, false, false);
        		Text.drawString("Light: ^5"+Data.lightData.get(i), 40, 115, 1, 1f, 1.0, Color.White, false, false);
        		
        		if(maxLight < Data.lightData.get(i)){
        			maxLight = Data.lightData.get(i);
        		}
        		if(minLight > Data.lightData.get(i) && Data.lightData.get(i) != null){
        			minLight = Data.lightData.get(i);
        		}
        		if(minHumidity > Data.humData.get(i) && Data.humData.get(i) != 0){
        			minHumidity = Data.humData.get(i);
        		}
        		if(maxHumidity < Data.humData.get(i) && Data.humData.get(i) != 255){
        			maxHumidity = Data.humData.get(i);
        		}
        		Text.drawString("Max Light: ^5"+ maxLight, 40, 180, 1, 1f, 1.0, Color.White, false, false);
        		Text.drawString("Min Light: ^5"+ minLight, 40, 195, 1, 1f, 1.0, Color.White, false, false);
        		
        		Text.drawString("Max Humidity: ^5"+ maxHumidity, 40, 210, 1, 1f, 1.0, Color.White, false, false);
        		Text.drawString("Min Humidity: ^5"+ minHumidity, 40, 225, 1, 1f, 1.0, Color.White, false, false);
        		
        		Text.drawString("Time spent in air ^5"+ hour + " ^2hours  ^5" + minute + "^2 minutes, ^5" + second + " ^2seconds", 40, 240, 1, 1f, 1.0, Color.White, false, false);
        		int multiplier = 1;
        		if(Data.altData.get(i) > 4000 && Data.altData.get(i) < 8000){
        			multiplier = 3;
        		}else if(Data.altData.get(i) > 8000 && Data.altData.get(i) < 14000){
        			multiplier = 5;
        		}else if(Data.altData.get(i) > 14000 && Data.altData.get(i) < 20000){
        			multiplier = 7;
        		}
        		Box.drawBox(40, 130, Data.altData.get(i) / (10 * multiplier), 40);
        		Screen.refresh();
        		if(Screen.isAskedToClose()){
        			Screen.destroy();
        		}
        	}
        }
    }
}
