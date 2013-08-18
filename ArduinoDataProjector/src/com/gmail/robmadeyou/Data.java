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
    public static ArrayList<Integer> totalData = new ArrayList<Integer>();

    public static ArrayList<Integer> lightData = new ArrayList<Integer>();
    public static ArrayList<Integer> humData = new ArrayList<Integer>();
    public static ArrayList<Integer> tempData = new ArrayList<Integer>();

    public static ArrayList<Integer> longData = new ArrayList<Integer>();
    public static ArrayList<Integer> latData = new ArrayList<Integer>();
    public static ArrayList<Integer> altData = new ArrayList<Integer>();

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
