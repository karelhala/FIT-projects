package ija.homework2.nums;

public class EvenInteger extends AbstractInteger{
    protected Num object;
    public EvenInteger(int x) {
        super(x);
    }

    @Override
    protected int validate(int x) {
        if ((x%2)==0){
            return x;
			
		}
        else
		{

            return x+1;
		}
    }

    public Num plus(Num dal) {
        object = new EvenInteger(x+dal.intValue());
        return object;
    }

}
