
package ija.homework1;

import ija.homework1.nums.Num;
import ija.homework1.nums.bounded.OddNum;
import ija.homework1.nums.roman.RomanNum;

import java.lang.reflect.*;

/** Trida testujici prvni ukol */
public class Homework1 {

    public static void main(String[] argv) {
        testNum();
        testOddNum();
        testRomanNum();
        System.out.println("OK");
    }

    /** Test tridy Num */
    public static void testNum() {
	Num n1 = new Num(20, 50);
	Num n2 = new Num(0, 10, 50);

	assert n1.getValue() == 20 : "n1 != 20";
	assert n2.getValue() == 41 : "n2 != 41";

	Num n3 = n1.plus(n2);
	assert n3.getValue() == 10 : "n2 != 10";
    }

    /** Test tridy OddNum */
    public static void testOddNum() {
	Num n1 = new OddNum(20, 50);
	Num n2 = new OddNum(9, 10, 50);
	
	assert n1.getValue() == 21 : "n1 != 21";
	assert n2.getValue() == 11 : "n2 != 11";

	Num n3 = n1.plus(n2);
	assert n3.getValue() == 33 : "n2 != 33";

	// test, zda je vysledek operace plus instanci tridy OddNum
	Class c = n3.getClass();
	assert c.equals(OddNum.class) : "n3 neni instanci tridy OddNum";
    }

    /** Test tridy RomanNum */
    public static void testRomanNum() {
	RomanNum n1 = new RomanNum("20", 50);
	RomanNum n2 = new RomanNum("0", 10, 50);
	
	assert n1.getValue().equals("20") : "n1 != 20";
	assert n2.getValue().equals("41") : "n2 != 41";

	RomanNum n3 = n1.plus(n2);
	assert n3.getValue().equals("10") : "n3 != 10";
	// test, zda trida RomanNum obsahuje instanci tridy Num jako atribut
	Class c = n3.getClass();
	boolean ok = false;
	Field[] fields = c.getDeclaredFields();
	for (int i = 0; i < fields.length; i++) {
	    Class typeClass = fields[i].getType();
	    if (typeClass.equals(Num.class)) ok=true;
	}
	assert ok : "Trida RomanNum neobsahuje objekt tridy Num";
    }

}
