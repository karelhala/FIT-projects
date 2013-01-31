package petrinets;

import petrinets.SignedVariable.Sign;

/**
* Trida reprezentujici promennou, ktera muze byt ve vystupu
*
* @author libor
*/

public class SignedVariable {
	/** vyberovy typ pro znamenko */
	public enum Sign{PLUS, MINUS};
	
	/** znamenko */
	private Sign sign;
	/** nazev promenne */
	private String name;
	
	public SignedVariable(Sign sign, String varName) {
		this.sign = sign;
		this.name = varName;
	}
	
	public static Sign getSignFromString(String sign)
	{
		return Sign.valueOf(sign);
	}
	public Sign getSign()
	{
		return this.sign;
	}
	
	public String getSignAsString()
	{
		if (this.sign == Sign.PLUS)
			return "+";
		else
			return "-";
	}

	public String getName()
	{
		return this.name;
	}
	
	public int getNameAsValue()
	{
		return Integer.valueOf(this.name);
	}
	
}
