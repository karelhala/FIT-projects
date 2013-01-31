package petrinets;
import java.util.ArrayList;
/**
* Trida reprezentujici podminku v prechodu petriho site
*
* @author libor
*/
public class Condition {
	/** vyctovy typ pro danou podminku */
	public enum Operator{LESS, LESSOREQ, GREATER, GREATERORQE, EQUAL, NOTEQUAL };
	/** prvni misto */
	private Place first;
	private int firstInt;
	/** operator */
	private Operator operator;
	/** druhe misto */
	private Place second;
	private int secondInt;
	
	public static Operator getOperatorFromString(String operator)
	{
		return Operator.valueOf(operator);
	}
	
	/** Konstruktory */
	public Condition()
	{
		this.first = null;
		this.operator = null;
		this.second = null;
	}
	public Condition(Place first, Operator op, Place second)
	{
		this.first = first;
		this.operator = op;
		this.second = second;
	}
	public Condition(String condition, ArrayList<Place> inPlaces)
	{
		this.setConditionFromString(condition, inPlaces);
	}
	public String getFirstPlaceTag()
	{
		return this.first.getTag();
	}
	public String getFirstAsString()
	{
		if(this.first != null)
			return this.first.getTag();
		else
			return String.valueOf(this.firstInt);
	}

	public String getSecondPlaceTag()
	{
		return this.second.getTag();
	}
	public String getSecondAsString()
	{
		if(this.second != null)
			return this.second.getTag();
		else
			return String.valueOf(this.secondInt);
	}
	public String getOperator()
	{
		return this.operator.toString();
	}
	public String getOperatorAsString()
	{
		if(this.operator == Operator.EQUAL)
			return "==";
		else if(this.operator == Operator.GREATER)
			return ">";
		else if(this.operator == Operator.GREATERORQE)
			return ">=";
		else if(this.operator == Operator.LESS)
			return "<";
		else if(this.operator == Operator.LESSOREQ)
			return "<=";
		else if(this.operator == Operator.NOTEQUAL)
			return "!=";
		else
			return "?";
	}
	
	public boolean setConditionFromString(String condition, ArrayList<Place> inPlaces)
	{
		if(condition.indexOf("<=") > 0)
		{
			String[] split = condition.split("<=");
			if(!setCondition(split[0],Operator.LESSOREQ,split[1],inPlaces))
				return false;
			
		}
		else if(condition.indexOf(">=") > 0)
		{
			String[] split = condition.split(">=");
			if(!setCondition(split[0],Operator.GREATERORQE,split[1],inPlaces))
				return false;
		}
		else if(condition.indexOf("==") > 0)
		{
			String[] split = condition.split("==");
			if(!setCondition(split[0],Operator.EQUAL,split[1],inPlaces))
				return false;
		}
		else if(condition.indexOf("!=") > 0)
		{
			String[] split = condition.split("!=");
			if(!setCondition(split[0],Operator.NOTEQUAL,split[1],inPlaces))
				return false;
		}
		else if(condition.indexOf("<") > 0)
		{
			String[] split = condition.split("<");
			if(!setCondition(split[0],Operator.LESS,split[1],inPlaces))
				return false;
		}
		else if(condition.indexOf(">") > 0)
		{
			String[] split = condition.split(">");
			if(!setCondition(split[0],Operator.GREATER,split[1],inPlaces))
				return false;
		}
		else
		{
			return false;
		}
		return true;
	}
	/**
	* Vytvori podminku z danych parametru
	*/
	private boolean setCondition(String first, Operator op, String second, ArrayList<Place> inPlaces)
	{
		this.operator = op;
		this.first = getPlaceWithTag(first.trim(), inPlaces);
		if(this.first == null)
		{
			try
			{
				this.firstInt = Integer.getInteger(first.trim());
			}
			catch(NullPointerException exp)
			{
				return false;
			}
			catch(NumberFormatException exp)
			{
				return false;
			}
		}
		this.second = getPlaceWithTag(second.trim(), inPlaces);
		if(this.second == null)
		{
			try
			{
				this.secondInt = Integer.valueOf(second.trim());
			}
			catch(NumberFormatException exp)
			{
				return false;
			}
		}
		return true;
	}
	
	/**
	* Zkontroluje zda dana podminka plati pro hodnoty v mistech
	*/
	public boolean isConditionTrue(int i, int j)
	{
		if(this.operator == Operator.EQUAL)
			return (i == j);
		else if (this.operator == Operator.GREATER)
			return (i > j);
		else if (this.operator == Operator.GREATERORQE)
			return (i >= j);
		else if (this.operator == Operator.LESS)
			return (i < j);
		else if (this.operator == Operator.LESSOREQ)
			return (i <= j);
		else if (this.operator == Operator.NOTEQUAL)
			return (i != j);
		else
			return false;
	}
	public Boolean isRight()
	{
		return true;
	}
	public Place getPlaceWithTag(String tag, ArrayList<Place> places)
	{
		for(Place place: places )
		{
			if(place.getTag().equals(tag))
				return place;
		}
		return null;
	}
	
	public Place getFirstPlace()
	{
		return this.first;
	}
	public Place getSecondPlace()
	{
		return this.second;
	}
}
