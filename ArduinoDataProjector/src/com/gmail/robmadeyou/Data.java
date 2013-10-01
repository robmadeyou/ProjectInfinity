package com.gmail.robmadeyou;

import java.util.ArrayList;

/**
 * Created with IntelliJ IDEA.
 * User: robert
 * Date: 18/08/13
 * Time: 00:50
 * To change this template use File | Settings | File Templates.
 */
public class Data {
    public static ArrayList<Double> totalData = new ArrayList<Double>();

    public static ArrayList<Double> lightData = new ArrayList<Double>();
    public static ArrayList<Double> humData = new ArrayList<Double>();
    public static ArrayList<Double> tempData = new ArrayList<Double>();

    public static ArrayList<Double> longData = new ArrayList<Double>();
    public static ArrayList<Double> latData = new ArrayList<Double>();
    public static ArrayList<Double> altData = new ArrayList<Double>();

    public static void sort(){
        int times = totalData.size() / 6;
        for(int i = 0; i < times; i++){
            lightData.add(totalData.get(i * 6));
            tempData.add(totalData.get(i * 6 + 1));
            humData.add(totalData.get(i * 6 + 2));

            longData.add(totalData.get(i * 6 + 3));
            latData.add(totalData.get(i * 6 + 4));
            altData.add(totalData.get(i * 6 + 5));
        }
    }
}
