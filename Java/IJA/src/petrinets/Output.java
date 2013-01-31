package petrinets;
import java.util.ArrayList;

import petrinets.SignedVariable.Sign;
/**
* Trida reprezentujici vystup v prechodu petriho site
*
* @author libor
*/
public class Output {
	/** vystupni misto */
	private Place outPlace;
	/** seznam promennych ze kterych se budou skladat vystupy spolecne se znamenkem */
	private ArrayList<SignedVariable> variables = new ArrayList<SignedVariable>(); 
	
	public Output(Place out) {
		this.outPlace = out;
	}
	public Output()
	{
		
	}
	/**
	* Konstruktor pro vystup
	* @param outputInString vystup jako string, napr. "x = a + b"
	* @param outPlaces seznam vystupnich mist z daneho prechodu, slouzi pro dosazeni za vystupni misto
	*/
	public Output(String outputInString, ArrayList<Place> outPlaces)
	{
		this.setOutputFromString(outputInString, outPlaces);
	}
	public String getOutPlaceTag()
	{
		return this.outPlace.getTag();
	}
	
	public ArrayList<SignedVariable> getVariables()
	{
		return this.variables;
	}
	public String getVariablesAsString()
	{
		String result = "";
		for(SignedVariable var: this.variables)
		{
			if (result.equals(""))
			{
				if(var.getSignAsString().equals("+"))
				{
					result += var.getName();
				}
				else
				{
					result += var.getSignAsString() + var.getName();
				}
			}
			else
			    result += " " + var.getSignAsString() + " " + var.getName();
		}
		return result;
	}
	public Place getOutPlace()
	{
		return this.outPlace;
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
	
	public void addVariable(String sign, String varName)
	{
		SignedVariable variable = new SignedVariable(SignedVariable.getSignFromString(sign),varName);
		this.variables.add(variable);
	}
	
	public boolean setOutputFromString(String outputInString, ArrayList<Place> outPlaces)
	{
		String[] split = outputInString.split("=");
		this.outPlace = getPlaceWithTag(split[0].trim(),outPlaces);
		if(this.outPlace == null)
			return false;
		String varName = "";
		Sign sign = Sign.PLUS; 
		for(int i = 0; i < split[1].length(); i++)
		{
			if(split[1].charAt(i) == '+')
			{
				if(varName.equals(""))
					sign = Sign.PLUS;
				else
				{
					SignedVariable variable = new SignedVariable(sign,varName);
					variables.add(variable);
					varName = "";
				}
				sign = Sign.PLUS;
			}
			else if(split[1].charAt(i) == '-')
			{
				if(varName.equals(""))
					sign = Sign.MINUS;
				else
				{
					SignedVariable variable = new SignedVariable(sign,varName);
					variables.add(variable);
					varName = "";
				}
				sign = Sign.MINUS; 
			}
			else if (split[1].charAt(i) == ' ')
			{
				if(varName.equals(""))
					continue;
				else
				{
					SignedVariable variable = new SignedVariable(sign,varName);
					variables.add(variable);
					sign = Sign.PLUS;
					varName = "";
				}
			}
			else
			{
				varName += split[1].charAt(i);
			}
				
		}
		if(!varName.equals(""))
		{
			SignedVariable variable = new SignedVariable(sign,varName);
			variables.add(variable);
		}
		return true;
	}
}
