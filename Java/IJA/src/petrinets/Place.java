package petrinets;

import java.util.ArrayList;
/**
* Trida reprezentujici misto v petriho sitich
* @author libor
*/
public class Place extends GraphElement {
	/** oznaceni pro nazev mista */
	private String name = "";
	/** oznaceni promenne zastupujici misto (x,y,..) */
	private String tag = "";
	/** Seznam hodnot v danem miste */
	private ArrayList<Integer> values = new ArrayList<Integer>();
	
	// Konstruktory
	public Place(int id) {
        this.name = "p" + id;
    }
	public Place(int id, String tag, int[] values)
	{
		this.name = "p" + id;
		this.tag = tag;
		for(int i = 0; i < values.length; i++)
		{
			this.addValue(values[i]);
		}
	}
	public Place(String name, String tag)
	{
		this.name = name;
		this.tag = tag;
	}
	public Place(int id, String tag, String values)
	{
		this.name = "p" + id;
		this.tag = tag;
		addValues(values);
	}
	public Place(int x, int y, int width, int height, int id, String tag, String values)
	{
		this.x = x;
		this.y = y;
		this.width = width;
		this.height = height;
		
		this.name = "p" + id;
		this.tag = tag;
		addValues(values);
	}
	/**
	* Prida oznaceni pro misto
	* @param tag oznaceni mista napr. x, y, apod.
	*/
	public void addTag(String tag){
		this.tag = tag;
	}
	/**
	* Prida hodnotu k mistu
	* @param value hodnota, napr. 0,3,4,...
	*/
	public void addValue(int value)
	{
		values.add(value);
	}
	/**
	* Prida hodnoty k mistu z retezce
	* @param valuesInStr hodnoty v retezci, napr. "0,3 ,4" apod.
	*/
	public boolean addValues(String valuesInStr)
	{
		this.values.clear();
		if (valuesInStr.trim().equals(""))
			return true;
		String[] split = valuesInStr.split(",");
		for(int i = 0; i < split.length; i++)
		{
			if(!split[i].trim().equals(""))
			{
				try
				{
					this.values.add(Integer.valueOf(split[i].trim()));
				}
				catch(NumberFormatException exp)
				{
					return false;
				}
			}
		}
		return true;
	}
	public String getName(){
		return this.name;
	}
	public String getTag(){
		return this.tag;
	}
	/**
	* Ziska z daneho mista vsechny hodnoty
	* @return seznam hodnot v ArrayListu
	*/
	public ArrayList<Integer> getValues(){
		return values;
	}
	
	public String getValuesAsString(){
		String result = "";
	    for(int i: this.values)
	    {
	    	result += String.valueOf(i) + ",";
	    }
	    return result;
	}
	/**
	* Pomoci tagu ziska z daneho seznamu mist potrebne misto
	* @param tag pro misto (x, y, apod.)
	* @param places seznam mist v ArrayListu, ktere se ma prohledavat
	* @return nalezene misto
	*/
	public Place getPlaceWithTag(String tag, ArrayList<Place> places)
	{
		for(Place place: places )
		{
			if(place.getTag().equals(tag))
				return place;
		}
		return null;
	}
	
	public void removeValueAtIndex(int i)
	{
		this.values.remove(i);
	}
}
