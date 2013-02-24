package ija.homework1.nums.bounded;

import ija.homework1.nums.Num;

public class OddNum extends Num{

	public OddNum(int x, int min, int max) {
		super (x,min,max);
    }

	public OddNum(int x, int max) {
		super (x,max);
    }

	public int getValue(){
		if ((x%2)==0)
			x=x+1;

		if (x>max){
			x=(x-min)%((max-min)+1)+min;

		} else if (x<min){

			x=max-((min-(x+1))%((max-min)+1));
		}

		if ((x%2)==0)
		{
			if (x==max && ((min%2)==0))
			{
				x=min+1;
			}

			else if (x==max)
				x=min;

			else
				x=x+1;
			}
		return x;
    }

	public OddNum plus(Num another)
	{
		OddNum n1 = new OddNum(x+another.getValue(), min, max);
		return n1;
	}
}
