
import java.net.*;
import java.util.concurrent.*;

public class ScanPort
{
	public static void main(String args[])
	{
		String ip="localhost";
		ExecutorService threadPool = Executors.newFixedThreadPool(1000);
		if(args.length == 0)
		{
			System.out.println("Please enter IP ,");
			System.out.println("Or default is localhost!");
		}
		else{
			ip = args[0];
			
		}

		for(int i=0;i<60000;i++){
			ConnectRemote ct = new ConnectRemote(ip,i);
			threadPool.execute(ct);
		}
	}
	public static class ConnectRemote extends Thread
	{
		private  String ip;
		private int port;
		ConnectRemote(String ip,int port)
		{
			this.ip = ip;
			this.port = port;
		}
		public void run()
		{
			try{
				Socket socket = new Socket(ip,port);
				System.out.println("port:"+port+"--success!");
				socket.close();
			}
			catch(Exception e)
			{
				System.out.println("fialed!");
			}
		}
	}
}

