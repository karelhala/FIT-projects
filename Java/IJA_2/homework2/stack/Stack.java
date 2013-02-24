package ija.homework2.stack;

import ija.homework2.nums.Num;
public class Stack {
	Num [] nums;
 	int size;
	int x=0;
	public Stack(int size)
	{
		nums = new Num [size];
		this.size = size;
	}
	public boolean push (Num object)
        {
            if (x<size)
            {
                nums[x] = object;
                x++;
                return true;
            }
            else
               return false;
        }
        public boolean pop ()
        {
            if (x!=0)
            {
                x--;
                return true;
            }
            else
               return false;
        }
        public boolean contains (Num object)
        {
			return nums[x-1].equals(object);
        }
}
