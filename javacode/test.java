public class test
{
  public static void main(String [] args)
  {
    int [] a;
    a=new int[10];
    a[0]=1;
    a[2]=2;
    a[3]=3;
    for (int i=1;i<a.length;i++){
      a[i]+=a[i-1];
    }
    for(int i=0;i<a.length;i++)
    {
      System.out.println(a[i]);
    }
    for(int x:a){
      System.out.print(x+"  ");
    }
  }
}