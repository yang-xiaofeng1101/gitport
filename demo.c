#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>


int main(int * argc, char **argv)
{
    double *Vec = NULL;
    double *Ans = NULL;
    double **Mat = NULL;
    int M,N;
    struct timeval start, end;
    

    /*************GET vector Size and Mat size*********************/
    if(argc != 3){
        printf("please input the size of vector, like ./demo 100\n");
        return -1;
    }
    else{
        M = atoi(argv[1]);
        N = atoi(argv[2]);

        printf("M = %d\t N = %d\n", M,N);
    }

    /**************Malloc memory**************/
    Vec = (double*)malloc(sizeof(double)*N);
    Ans = (double*)malloc(sizeof(double)*M);

    Mat = (double**)malloc(sizeof(double*)*M);
    for(int i=0;i<M;i++){
        Mat[i] = (double*)malloc(sizeof(double)*N);
    }


    gettimeofday(&start, NULL);
    
    /**************init value*****************/
    for(int i =0;i<M;i++){
        for(int j=0;j<N;j++){
            Mat[i][j] = rand();
        }
    }
    for(int i =0;i<N;i++){
        Vec[i] = rand();
    }

    /*************compute ********************/
    int sum;
    sum = 0;

    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            sum+=Mat[i][j]*Vec[j];
        }
        Ans[i]=sum;
        sum=0;
    }

    gettimeofday(&end, NULL);

    long timeuse = 1000000*(end.tv_sec - start.tv_sec)+end.tv_usec-start.tv_usec;

    printf("the time in init and compute is %lf ms\n", (double)timeuse/1000);

    return 0;
}
