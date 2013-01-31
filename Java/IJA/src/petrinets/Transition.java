package petrinets;

import java.util.ArrayList;

import petrinets.SignedVariable.Sign;

/**
 * Trida reprezentujici prechod v petriho sitich
 *
 * @author libor
 */
public class Transition extends GraphElement {
	/** oznaceni pro nazev prechodu */
	private String name = "";
	/** Seznam mist, ktera vstupuji do prechodu */
	private ArrayList<Place> inPlaces = new ArrayList<Place>();
	/** Seznam mist, ktera vystupuji z prechodu */
	private ArrayList<Place> outPlaces = new ArrayList<Place>();
	/** Seznam podminek (strazi) */
	private ArrayList<Condition> conditions = new ArrayList<Condition>();
	/** Seznam vystupu */
	private ArrayList<Output> outputs = new ArrayList<Output>();

	/**
	* Metoda pro vykonani jednoho kroku petriho site.
	* Vytvori si pomocny seznam, ktery bude obsahovat tagy a obsahy vstupnich mist.
	* Pomoci tohoto seznamu si potom zkontroluju podminky a dokud nejsou vsechny pravdive,
	* tak projedu hodnoty ve vstupnich mistech.
	* Pomoci seznamu vystupu pote vyplnim vsechna vystupni mista a nakonec odeberu pouzite hodnoty
	* ze vstupnich mist
	* @return Vraci true nebo false podle toho jestli bylo mozne krok vykonat nebo ne
	*/
	public boolean Step()
	{
		ArrayList<StepHelper> stepArrayList = new ArrayList<StepHelper>();
		for(Place pl: this.inPlaces)
		{
			StepHelper sh = new StepHelper(pl.getTag(),pl.getValues());
			stepArrayList.add(sh);
		}

		while(!isAllConditionTrue(stepArrayList))
		{
			int i = stepArrayList.size() - 1;
			while(stepArrayList.get(i).setToNextActualValue())
			{
				i--;
				if(i < 0)
					return false;
			}
		}
		for(Output out: this.outputs)
		{
			int outputValue = 0;
			boolean addVal = true;
			for(SignedVariable sv: out.getVariables())
			{
				boolean found = false;
				for(StepHelper sh: stepArrayList)
				{
					if(sv.getName() == sh.getTag())
					{
						found = true;
						if(!sh.isEmpty())
						{

							if(sv.getSign() == Sign.PLUS)
							{
								outputValue += sh.getActualValue();
							}
							else
							{
								outputValue -= sh.getActualValue();
							}
						}
						else
							addVal = false;
					}
				}
				if(!found)
				{
					if(sv.getSign() == Sign.PLUS)
					{
						outputValue += sv.getNameAsValue();
					}
					else
					{
						outputValue -= sv.getNameAsValue();
					}
				}
			}
			if(addVal)
				out.getOutPlace().addValue(outputValue);
			else
				return false;

		}
		for(StepHelper sh: stepArrayList)
		{
			if(sh.isEmpty())
				continue;
			for(Place pl: this.inPlaces)
			{
				if(sh.getTag() == pl.getTag())
				{
					pl.removeValueAtIndex(sh.getActualIndex());
				}
			}
		}
		return true;
	}

	/**
	 * Tato metoda zkontroluje vsechny podminky a vraci hodnotu zda jsou splneny nebo ne
	 * @param arrayToCheck je seznam obsahujici stephelper
	 * @return boolean zda jsou splneny podminky
	 */
	public boolean isAllConditionTrue(ArrayList<StepHelper> arrayToCheck) {
		for (Condition con : conditions) {
			Place pl1 = con.getFirstPlace();
			Place pl2 = con.getSecondPlace();

			int i = 0;
			int j = 0;

			if (pl1 == null)
				i = Integer.valueOf(con.getFirstAsString());

			if (pl2 == null)
				j = Integer.valueOf(con.getSecondAsString());

			for (StepHelper sh : arrayToCheck) {

				if (pl1 != null && sh.getTag() == pl1.getTag())
				{
					if(sh.isEmpty())
						return false;
					else
						i = sh.getActualValue();
				}
				if (pl2 != null && sh.getTag() == pl2.getTag())
				{
					if(sh.isEmpty())
						return false;
					else
						j = sh.getActualValue();
				}
			}

			if (!con.isConditionTrue(i, j))
				return false;

		}
		return true;
	}

	/** Konstruktory */
	public Transition(int id) {
		this.name = "e" + id;
	}
	public Transition(String name) {
		this.name = name;
	}
	public Transition(int x, int y, int width, int height, int id) {
		this.x = x;
		this.y = y;
		this.width = width;
		this.height = height;
		this.name = "e" + id;
	}
	public Transition(int id, String conditions, String outputs) {
		this.name = "e" + id;
		setConditionsFromString(conditions);
		setOutputsFromString(outputs);
	}
    /**
     * Vraci nazev prechodu
     * @return nazev prechodu
     */
	public String getName() {
		return this.name;
	}

	/**
	 * Vraci seznam vstupnich mist
	 * @return vstupni mista
	 */
	public ArrayList<Place> getInPlaces() {
		return this.inPlaces;
	}

	/**
	 * Vraci seznam vystupnich mist
	 * @return vystupni mista
	 */
	public ArrayList<Place> getOutPlaces() {
		return this.outPlaces;
	}

	/**
	 * Vraci seznam podminek
	 * @return podminky
	 */
	public ArrayList<Condition> getConditions() {
		return this.conditions;
	}
	/**
	 * Vraci seznam vystupu
	 * @return vystupy
	 */
	public ArrayList<Output> getOutputs() {
		return this.outputs;
	}

	/**
	 * Vraci podminky jako retezece pro zobrazeni site. Napr."x > 0"
	 * @return podminky jako retezec
	 */
	public String getConditionsAsString() {
		String result = "";
		for (Condition cond : this.conditions) {
			if (result == "") {
				result += cond.getFirstAsString() + " "
						+ cond.getOperatorAsString() + " "
						+ cond.getSecondAsString();
			} else {
				result += " & " + cond.getFirstAsString() + " "
						+ cond.getOperatorAsString() + " "
						+ cond.getSecondAsString();
			}
		}
		return result;
	}

	/**
	 * Vraci vystupy jako retezec
	 * @return vystupy jako retezec
	 */
	public String getOutputsAsString() {
		String result = "";
		for (Output output : this.outputs) {
			if (result == "")
				result += output.getOutPlaceTag() + " = "
						+ output.getVariablesAsString();
			else
				result += " , " + output.getOutPlaceTag() + " = "
						+ output.getVariablesAsString();
		}
		return result;
	}

	/**
	 * Vytvori ze stringu jednotlive vystupy pro dana mista
	 *
	 * @param outputsInStr
	 *            vystup jako string, napr. "x = a + b, y = a - c"
	 */
	public boolean setOutputsFromString(String outputsInStr) {
		this.outputs.clear();
		if (outputsInStr.trim().equals(""))
			return true;
		String[] outs = outputsInStr.split(",");
		for (int i = 0; i < outs.length; i++) {
			Output output = new Output();
			if(output.setOutputFromString(outs[i], outPlaces))
				outputs.add(output);
			else
				return true;
		}
		return true;
	}

	/**
	 * Vytvori ze stringu jednotlive podminky
	 *
	 * @param conditionsInStr
	 *            podminky jako string, napr. "x > 5 & y != 0"
	 */
	public boolean setConditionsFromString(String conditionsInStr) {
		this.conditions.clear();
		String[] cons = conditionsInStr.split("&");
		for (int i = 0; i < cons.length; i++) {
			Condition condition = new Condition();
			if(condition.setConditionFromString(cons[i], inPlaces))
				conditions.add(condition);
			else
				return false;
		}
		return true;
	}

	/**
	 * Prida vstupni misto a oznaci misto pro prechod
	 *
	 * @param place
	 *            misto reprezentovano tridou place
	 * @param tag
	 *            oznaceni mista pro prechod, napr. x, y, atd.
	 */
	public void addInPlace(Place place, String tag) {
		place.addTag(tag);
		inPlaces.add(place);
	}

	/**
	 * Prida vystupni misto a oznaci misto pro prechod
	 *
	 * @param place
	 *            misto reprezentovano tridou place
	 * @param tag
	 *            oznaceni mista pro prechod, napr. x, y, atd.
	 */
	public void addOutPlace(Place place, String tag) {
		place.addTag(tag);
		outPlaces.add(place);
	}

	/**
	 * Prida podminku k aktualnim podminkam
	 * @param podminka
	 */
	public void addCondition(Condition condition) {
		this.conditions.add(condition);
	}

	/**
	 * Prida vystup k aktualnim vystupum
	 * @param vystup
	 */
	public void addOutput(Output output) {
		this.outputs.add(output);
	}

	/**
	 * Odstrani misto ze vstupnich mist
	 * @param misto k odstraneni
	 */
	public void removeInPlace(Place deletePlace)
    {
    	ArrayList<Place> tempPlaces = new ArrayList<Place>();
     	for (Place place:inPlaces)
    	{
     		if (!place.equals(deletePlace))
     			tempPlaces.add(place);
        }
        this.inPlaces = tempPlaces;
	}
	/**
	 * Odstrani misto z vystupnich mist
	 * @param misto k odstraneni
	 */
	public void removeOutPlace(Place deletePlace)
	{
		ArrayList<Place> tempPlaces = new ArrayList<Place>();
    	for (Place place:outPlaces)
        {
    		if (!place.equals(deletePlace))
    			tempPlaces.add(place);
        }
    	this.outPlaces = tempPlaces;
	}
}