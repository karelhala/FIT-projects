package ija.homework1.nums.roman;

import ija.homework1.nums.Num;

public class RomanNum {

	protected Num number;

	public RomanNum(String myString, int max) {
		number = new Num(Integer.parseInt(myString),max);
    }

	public RomanNum(String myString,int min, int max) {
		number = new Num(Integer.parseInt(myString),min,max);
    }

	public String getValue() {
		return Integer.toString (number.getValue());
	}
	public RomanNum plus (RomanNum another)
	{
		RomanNum n1 = new RomanNum(another.getValue()+Integer.toString(number.getValue()),number.min,number.max);
		return n1;
	}
}
