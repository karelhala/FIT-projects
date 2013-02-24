package ija.homework2.nums;

public abstract class AbstractInteger implements Num{
    protected int x;
    public AbstractInteger (int x)
    {
       this.x = validate(x);
    }
    protected abstract int validate (int x);
    public int intValue() {
        return this.validate(x);
    }

    public String stringValue() {
        return Integer.toString(validate(x));
    }
	public int hashCode(){
		return (27*intValue());
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
