import java.util.Scanner;
public class Fibonacci
{
	private static int fibonacci(int n)
	{
	if(n==1||n==2) return 1;
 	if(n<1)   return -1;
	return fibonacci(n-1)+fibonacci(n-2);
	}
	public static void main(String args[])
	{
		int n;
		Scanner in=new Scanner(System.in);
		n=in.nextInt();
		
		while(n>0)
		{
		 	if(fibonacci(n)%2!=0)
				System.out.println("ODD");
			else
				System.out.println("EVEN");
			n=in.nextInt();
		}
	}
}	
