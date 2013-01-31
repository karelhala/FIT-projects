package petrinets;
import java.util.ArrayList;
/**
* Trida reprezentujici petriho sit
* @author libor
*/
public class PetriNet {

	/** seznam mist petriho site */
	private ArrayList<Place> places = new ArrayList<Place>();
	/** seznam prechodu petriho site */
	private ArrayList<Transition> transitions = new ArrayList<Transition>();

	/**
	 * Vraci seznam mist
	 * @return mista
	 */
	public ArrayList<Place> getPlaces()
	{
		return places;
	}
	/**
	 * Vraci misto podle id
	 * @param id
	 * @return misto (nebo null)
	 */
	public Place getPlaceById(int id)
	{
		for (Place place: this.getPlaces())
		{
			if(place.getName().equals("p" + id))
				return place;
		}
		return null;
	}
	/**
	 * Vraci misto podle oznaceni
	 * @param tag oznaceni
	 * @return misto
	 */
	public Place getPlaceByTag(String tag) {
		for (Place place: this.getPlaces())
		{
			if(place.getTag().equals(tag))
				return place;
		}
		return null;
	}
	/**
	 * Vraci misto podle jmena
	 * @param name
	 * @return misto
	 */
	public Place getPlaceByName(String name) {
		for (Place place: this.getPlaces())
		{
			if(place.getName().equals(name))
				return place;
		}
		return null;
	}
	/**
	 * Vraci seznam prechodu
	 * @return seznam prechodu
	 */
	public ArrayList<Transition> getTransitions()
	{
		return transitions;
	}
	/**
	 * Vraci prechod podle id
	 * @param id
	 * @return nalezeny prechod (nebo null)
	 */
	public Transition getTransitionById(int id)
	{
		for (Transition transition: this.getTransitions())
		{
			if(transition.getName().equals("e" + id))
				return transition;
		}
		return null;
	}
	/**
	 * Prida misto s danym id, oznacenim a hodnotami
	 * @param id
	 * @param tag
	 * @param values
	 */
	public void addPlace(int id, String tag, int[] values)
	{
		Place place = new Place(id,tag,values);
		this.addPlace(place);
	}
	/**
	 * Prida misto s danym jmenem a oznacenim
	 * @param name
	 * @param tag
	 */
	public void addPlace(String name, String tag)
	{
		Place place = new Place(name,tag);
		this.addPlace(place);
	}
	/**
	 * Prida misto s danym id, oznacenim a hodnotami v retezci
	 * @param id
	 * @param tag
	 * @param values
	 */
	public void addPlace(int id, String tag, String values)
	{
		Place place = new Place(id,tag,values);
		this.addPlace(place);
	}
	/**
	 * Prida misto s danymi souradnicemi, sirkou, vyskou, id,oznacenim a hodnotami v retezci
	 * @param x
	 * @param y
	 * @param width
	 * @param height
	 * @param id
	 * @param tag
	 * @param values
	 */
	public void addPlace(int x, int y, int width, int height, int id, String tag, String values)
	{
		Place place = new Place(x,y,width,height,id,tag,values);
		this.addPlace(place);
	}
	/**
	 * Prida misto
	 * @param place
	 */
	public void addPlace(Place place) {
        this.places.add(place);
    }
	/**
	 * Prida prechod s danym id
	 * @param id
	 */
	public void addTransition(int id)
	{
		Transition transition = new Transition(id);
		this.addTransition(transition);
	}
	/**
	 * Prida prechod s danymi souradnicemi, vyskou, sirkou a id
	 * @param x
	 * @param y
	 * @param width
	 * @param height
	 * @param id
	 */
	public void addTransition(int x, int y, int width, int height, int id)
	{
		Transition transition = new Transition(x,y,width,height,id);
		this.addTransition(transition);
	}
	/**
	 * Prida prechod s danym jmenem
	 * @param name
	 */
	public void addTransition(String name)
	{
		Transition transition = new Transition(name);
		this.addTransition(transition);
	}
	/**
	 * Prida prechod
	 * @param transition
	 */
	public void addTransition(Transition transition) {
		this.transitions.add(transition);
	}
	/**
	 * Prida do daneho prechodu, dane misto jako vstupni s danym oznacenim
	 * @param place
	 * @param transition
	 * @param tag
	 */
	public void addInputArc(Place place, Transition transition, String tag)
	{
		transition.addInPlace(place, tag);
	}
	/**
	 * Prida do daneho prechodu, dane misto jako vystupni s danym oznacenim
	 * @param place
	 * @param transition
	 * @param tag
	 */
	public void addOutputArc(Place place, Transition transition, String tag)
	{
		transition.addOutPlace(place, tag);
	}
	/**
	 * Odstrani dane misto ze seznamu mist
	 * @param place
	 */
	public void removePlace(Place place)
	{
		this.places.remove(place);
	}
	/**
	 * Odstrani dany prechod ze seznamu prechodu
	 * @param transition
	 */
	public void removeTransition(Transition transition)
	{
		this.transitions.remove(transition);
	}
	/**
	 * Vymaze celou petriho sit, jak mista tak prechody
	 */
	public void clear()
	{
		places.clear();
		transitions.clear();
	}
	/**
	 * Zjisti zda dane misto je jiz u nejakeho prechodu jako vstupni misto
	 * @param place
	 * @return true nebo false
	 */
	public boolean isInPlaceInTransitions(Place place)
	{
		for(Transition transition: this.transitions)
		{
			for(Place inPlace: transition.getInPlaces())
			{
				if(inPlace.getName() == place.getName())
					return true;
			}
		}
		return false;
	}
	/**
	 * Zjisti zda dane misto je jiz nekde vedeno jako vystupni misto
	 * @param place
	 * @return
	 */
	public boolean isOutPlaceInTransitions(Place place)
	{
		for(Transition transition: this.transitions)
		{
			for(Place outPlace: transition.getOutPlaces())
			{
				if(outPlace.getName() == place.getName())
					return true;
			}
		}
		return false;
	}
	/**
	 * Provede jeden krok simulace petriho site
	 * @return boolean zda bylo neco zmeneno
	 */
	public boolean simulateOneStep()
	{
		boolean somethingChange = false;
		for(Transition transition: this.transitions)
		{
			if(transition.Step())
				somethingChange = true;
		}
		return somethingChange;
	}
	/**
	 * Provede celou simulaci petriho site;
	 */
	public void simulate()
	{
		while(simulateOneStep());
	}
	public String getInfo()
	{
            return String.format("\nNet contains:\n%d places\nand\n%d transitions", places.size(),transitions.size());
	}
	/**
	 * Metoda slouzici pro vytisknuti obsahu petriho site
	 */
	public void printPetriNet()
	{
		for(Place place: this.places)
		{
			System.out.println("Place name: " + place.getName() + " tag: " + place.getTag());
		}
		for(Transition transition: this.transitions)
		{
			System.out.println("Transition name: " + transition.getName());
			for(Place inPlace: transition.getInPlaces())
			{
				System.out.println("-- InPlace name: " + inPlace.getName());
			}
			for(Place outPlace: transition.getOutPlaces())
			{
				System.out.println("-- OutPlace name: " + outPlace.getName());
			}
			for(Condition condition: transition.getConditions())
			{
				System.out.println("-- Condition first: " + condition.getFirstPlaceTag() + " operator: " + condition.getOperator() + " second: " + condition.getSecondPlaceTag());
			}
			for(Output output: transition.getOutputs())
			{
				System.out.println("-- Output place: " + output.getOutPlaceTag());
			}
		}
	}
}