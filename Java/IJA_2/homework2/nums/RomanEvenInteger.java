package ija.homework2.nums;

public class RomanEvenInteger implements Num{
    protected Num numbers;
    protected String my_string;
    public RomanEvenInteger(String string) {
        this.my_string = string;
    }

    public Num plus(Num x) {
        numbers = new EvenInteger(Integer.parseInt(my_string));
        return numbers.plus(x);
    }

    public String stringValue() {
		numbers = new EvenInteger(Integer.parseInt(my_string));
		my_string = numbers.stringValue();
       return my_string;
    }

    public int intValue() {
		numbers = new EvenInteger(Integer.parseInt(my_string));
		my_string = numbers.stringValue();
        return Integer.parseInt(my_string);
    }
	public int hashCode(){
		return (31*intValue());
	}
	
	@Override
	public boolean equals(Object obj)
	{
		if (obj instanceof Num)
		{
			Num object = (Num) obj;
			if (this.hashCode()==object.hashCode())
				return true;
		}
		return false;
	}
}
