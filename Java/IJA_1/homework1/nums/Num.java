package ija.homework1.nums;

public class Num{
	protected int x;
	public int min;
	public int max;
	

	public Num(int x, int max) {
	this.x=x;
	this.max=max;
	this.min=0;
    }
	public Num(int x, int min, int max) {
	this.x=x;
	this.min=min;
	this.max=max;
    }

	public int getValue() {

		if (x>max){
			x=(x-min)%((max-min)+1)+min;
		} 

		else if (x<min)
		{
			x=max-((min-(x+1))%((max-min)+1));
		}

		return x;
    }

	public Num plus(Num another)
	{
		Num n1 = new Num(x+another.getValue(),min,max);
		return n1;
	}

	
}
