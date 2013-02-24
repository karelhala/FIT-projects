package petrinets;
import java.util.ArrayList;
/**
 *
 * @author xhalak00
 * @author xzaple29
 */
public class condition {
	/** výčtový typ pro danou podmínku */
	public enum Operator{LESS, LESSOREQ, GREATER, GREATERORQE, EQUAL, NOTEQUAL };
	/** první místo */
	private Place first;
	/** operátor */
	private String operator;
	/** druhé místo */
	private Place second;

        /**
         * Vyparsuje operator ze stringu
         * @param operator retezec hodnot
         * @return operator
         */
	public static Operator getOperatorFromString(String operator)
	{
		return Operator.valueOf(operator);
	}

	/**
         * Kontruktor pro vytvoreni spojeni 2 policek
         * @param first prvni objekt
         * @param op operator
         * @param second druhy objekt
         */
	public condition(Place first, String op, Place second)
	{
		this.first = first;
		this.operator = op;
		this.second = second;
	}

        /**
         * Urceni, ktery operator je pouzit na danem miste
         * @param condition retezec hodnot
         * @param inPlaces Mista v siti
         */
	public void Condition(String condition, ArrayList<Place> inPlaces)
	{
		if(condition.indexOf("<=") > 0)
		{
			String[] split = condition.split("<=");
			setCondition(split[0],Operator.LESSOREQ,split[1],inPlaces);

		}
		else if(condition.indexOf(">=") > 0)
		{
			String[] split = condition.split(">=");
			setCondition(split[0],Operator.GREATERORQE,split[1],inPlaces);
		}
		else if(condition.indexOf("==") > 0)
		{
			String[] split = condition.split("==");
			setCondition(split[0],Operator.EQUAL,split[1],inPlaces);
		}
		else if(condition.indexOf("!=") > 0)
		{
			String[] split = condition.split("!=");
			setCondition(split[0],Operator.NOTEQUAL,split[1],inPlaces);
		}
		else if(condition.indexOf("<") > 0)
		{
			String[] split = condition.split("<");
			setCondition(split[0],Operator.LESS,split[1],inPlaces);
		}
		else if(condition.indexOf(">") > 0)
		{
			String[] split = condition.split(">");
			setCondition(split[0],Operator.GREATER,split[1],inPlaces);
		}
		else
		{
			// chyba
		}
	}
        /**
         * Vrati prvni misto
         * @return prvni misto
         */
	public Place getFirstPlace()
	{
		return this.first;
	}

        /**
         * Vrati druhe misto
         * @return druhe misto
         */
	public Place getSecondPlace()
	{
		return this.second;
	}

        /**
         * Vrati operator
         * @return operator
         */
	public String getOperator()
	{
		return this.operator.toString();
	}

	/**
	* Vytvoří podmínku z daných parametrů
	*/
	private void setCondition(String first, Operator op, String second, ArrayList<Place> inPlaces)
	{
		//this.operator = op;
		this.first = getPlaceWithTag(first.trim(), inPlaces);
		this.second = getPlaceWithTag(second.trim(), inPlaces);
	}

	/**
	* Zkontroluje zda daná podmínka platí pro hodnoty v místech
	*/
	public Boolean isRight()
	{
		return true;
	}
	public Place getPlaceWithTag(String tag, ArrayList<Place> places)
	{
		/*for(Place place: places )
		{
			if(place.getTag().equals(tag))
				return place;
		}*/
		return null;
	}
}