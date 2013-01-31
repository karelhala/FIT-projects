package petrinets;

import java.util.ArrayList;

public class StepHelper {

	private ArrayList<Integer> values = new ArrayList<Integer>();
	private String tag;
	private int actualIndex;
	
	public StepHelper(String tag, ArrayList<Integer> values)
	{
		this.tag = tag;
		this.values = values;
		this.actualIndex = 0;
	}
	
	public int getActualValue()
	{
		return this.values.get(this.actualIndex);
	}
	public int getActualIndex()
	{
		return this.actualIndex;
	}
	public String getTag()
	{
		return this.tag;
	}
	
	public boolean setToNextActualValue()
	{
		if(values.size() > (this.actualIndex + 1))
		{
			this.actualIndex++;
			return false;
		}
		else
		{
			this.actualIndex = 0;
			return true;
		}
	}
	
	public void setActualIndex(int i)
	{
		this.actualIndex = i;
	}
	
	public boolean isEmpty()
	{
		return (this.values.size() < 1);
	}
}
