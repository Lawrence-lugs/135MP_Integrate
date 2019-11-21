#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

//hello

float *elements;
int nelements;
float treturns[4];

void *threadRoutineTrap(void *arg);
void *threadRoutineSimp(void *arg);
void *threadRoutineBoole(void *arg);

int main(int argc, char** argv)
{
	printf("\n");

	struct timeval t1,t2;
	double runtime;

	if(argc > 3){
		printf("Invalid number of arguments.\n");
	}

	FILE * file;
	file = fopen(argv[1],"rb");
	if(!file){
		printf("Failed to open file.\n");
	}

	float num_elements;
	fread((void*)(&num_elements),sizeof(num_elements),1,file);
	nelements = (int)num_elements;
	printf("n = %i\n",nelements);

	float dx;
	fread((void*)(&dx),sizeof(dx),1,file);
	printf("dx = %f\n",dx);

	elements = (float*)malloc(sizeof(float)*nelements);
	int i;
	float res;

	printf("Reading elements...\n");
	fread(elements,sizeof(float),nelements,file);
	printf("Elements read.\n");

	printf("Total elements: %i\n",nelements);

	printf("---Single thread: Trapezoid---\n");

	gettimeofday(&t1,NULL);
	res=0;
	for(i=1;i<nelements-1;i++){
		res+=elements[i];
	}
	res += (elements[nelements-1]+elements[0])/2;
	res *= dx;

	gettimeofday(&t2,NULL);
	runtime = (t2.tv_sec - t1.tv_sec) * 1000000;
	runtime += t2.tv_usec - t1.tv_usec;

	//printf("t1:%li,%li\n",t1.tv_sec,t1.tv_usec);
	//printf("t2:%li,%li\n",t2.tv_sec,t2.tv_usec);
	printf("Time: %fus\t Res: %f\t\n",runtime,res);


	printf("---Multithreaded: Trapezoid---\n");

	gettimeofday(&t1,NULL);

	res = 0;
	pthread_t threads[4];
	int ints[4] = {1,2,3,4};
	for(i=0;i<4;i++){
		pthread_create(&threads[i],NULL,threadRoutineTrap,(void*)&ints[i]);
	}

	float *ret;
	for(i=0;i<4;i++){
		pthread_join(threads[i],NULL);
		res+=treturns[i];
	}
	res += (elements[nelements-1]+elements[0])/2;
	res -= elements[nelements-1] + elements[0];
	res *= dx;

	gettimeofday(&t2,NULL);
	runtime = (t2.tv_sec - t1.tv_sec) * 1000000;
	runtime += t2.tv_usec - t1.tv_usec;

	printf("Time: %fus\t Res: %f\t\n",runtime,res);

	printf("---Single thread: Simpson's Rule---\n");

	gettimeofday(&t1,NULL);
	res=0;
	for(i=1;i<nelements-1;i++){
		if(i%2 == 1){
			res+=4*elements[i];
		}else{
			res+=2*elements[i];
		}
	}
	res += (elements[nelements-1]+elements[0]);
	res *= dx/3;

	gettimeofday(&t2,NULL);
	runtime = (t2.tv_sec - t1.tv_sec) * 1000000;
	runtime += t2.tv_usec - t1.tv_usec;

	//printf("t1:%li,%li\n",t1.tv_sec,t1.tv_usec);
	//printf("t2:%li,%li\n",t2.tv_sec,t2.tv_usec);
	printf("Time: %fus\t Res: %f\t\n",runtime,res);

	printf("---Multithreaded: Simpson's Rule---\n");

	gettimeofday(&t1,NULL);

	res = 0;
	for(i=0;i<4;i++){
		pthread_create(&threads[i],NULL,threadRoutineSimp,(void*)&ints[i]);
	}

	for(i=0;i<4;i++){
		pthread_join(threads[i],NULL);
		res+=treturns[i];
	}
	res = res + elements[0] - elements[nelements-1];
	res *= dx/3;

	gettimeofday(&t2,NULL);
	runtime = (t2.tv_sec - t1.tv_sec) * 1000000;
	runtime += t2.tv_usec - t1.tv_usec;

	printf("Time: %fus\t Res: %f\t\n",runtime,res);

	printf("---Single thread: Boole's Rule---\n");

	gettimeofday(&t1,NULL);
	res=0;
	for(i=1;i<nelements-1;i++){
		if(i%4 == 0){
			res+=14*elements[i];
		}else if(i%4 == 1){
			res+=32*elements[i];
		}else if(i%4 == 2){
			res+=12*elements[i];
		}else if(i%4 == 3){
			res+=32*elements[i];
		}
	}
	if(i%4 == 0){
		res+=7*elements[nelements-1];
		res*=2*(dx/45);
	}else if(i%4 == 1){
		res-=7*elements[nelements-1];
		res*=2*(dx/45);
	}else if(i%4 == 2){
		res-=32*elements[nelements-1];
		res-=7*elements[nelements-2];
		res*=2*(dx/45);
		res+=elements[nelements-1];
	}else if(i%4 == 3){
		res-=12*elements[nelements-1];
		res-=32*elements[nelements-2];
		res-=7*elements[nelements-3];
		res*=2*(dx/45);
		res+=elements[nelements-1];
		res+=elements[nelements-2];
	}

	gettimeofday(&t2,NULL);
	runtime = (t2.tv_sec - t1.tv_sec) * 1000000;
	runtime += t2.tv_usec - t1.tv_usec;

	//printf("t1:%li,%li\n",t1.tv_sec,t1.tv_usec);
	//printf("t2:%li,%li\n",t2.tv_sec,t2.tv_usec);
	printf("Time: %fus\t Res: %f\t\n",runtime,res);

	printf("---Multithreaded: Boole's Rule---\n");

	gettimeofday(&t1,NULL);

	res = 0;
	for(i=0;i<4;i++){
		pthread_create(&threads[i],NULL,threadRoutineBoole,(void*)&ints[i]);
	}

	for(i=0;i<4;i++){
		pthread_join(threads[i],NULL);
		res+=treturns[i];
	}
	if((nelements-1)%4 == 0){
		res-=7*elements[nelements-2];
		res*=2*(dx/45);
	}else if((nelements-1)%4 == 1){
		res-=7*elements[nelements-2];
		res-=32*elements[nelements-1];
		res*=2*(dx/45);
		res+=elements[nelements-1];
	}else if((nelements-1)%4 == 2){
		res-=7*elements[nelements-3];
		res-=32*elements[nelements-2];
		res-=12*elements[nelements-1];
		res*=2*(dx/45);
		res+=elements[nelements-1];
		res+=elements[nelements-2];
	}else if((nelements-1)%4 == 3){
		res-=7*elements[nelements-4];
		res-=32*elements[nelements-3];
		res-=12*elements[nelements-2];
		res-=32*elements[nelements-1];
		res*=2*(dx/45);
		res+=elements[nelements-1];
		res+=elements[nelements-2];
		res+=elements[nelements-3];
	}

	gettimeofday(&t2,NULL);
	runtime = (t2.tv_sec - t1.tv_sec) * 1000000;
	runtime += t2.tv_usec - t1.tv_usec;

	printf("Time: %fus\t Res: %f\t\n",runtime,res);

}

void *threadRoutineSimp(void *arg){
	int n = *(int*)arg;
	int i;
	float tres=0;

	for(i=(((n-1)*nelements)/4);i<=(((n*nelements)/4)-1);i++){
			if(i%2 == 1){
				tres+=4*elements[i];
			}else{
				tres+=2*elements[i];
			}
	}
	//printf("thread%i (%i,%i):%f\n",n,(((n-1)*nelements)/4),(((n*nelements)/4)-1),tres);
	treturns[n-1]=tres;


}

void *threadRoutineBoole(void *arg){
	int n = *(int*)arg;
	int i;
	float tres=0;

	for(i=(((n-1)*nelements)/4);i<=(((n*nelements)/4)-1);i++){
		if(i%4 == 0){
			tres+=14*elements[i];
		}else if(i%4 == 1){
			tres+=32*elements[i];
		}else if(i%4 == 2){
			tres+=12*elements[i];
		}else if(i%4 == 3){
			tres+=32*elements[i];
		}
	}
	//printf("thread%i (%i,%i):%f\n",n,(((n-1)*nelements)/4),(((n*nelements)/4)-1),tres);
	treturns[n-1]=tres;
}

void *threadRoutineTrap(void *arg){
	int n = *(int*)arg;
	int i;
	float tres=0;

	for(i=(((n-1)*nelements)/4);i<=(((n*nelements)/4)-1);i++){
		tres+=elements[i];
	}
	//printf("thread%i (%i,%i):%f\n",n,(((n-1)*nelements)/4),(((n*nelements)/4)-1),tres);
	treturns[n-1]=tres;
}
