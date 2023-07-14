#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated

void heapify(int arr[], int n, int i)
{
    // store largest as the root element
    int largest = i;
 
    int left = 2 * i + 1;
    int right  = 2 * i + 2;
 
    // now check whether the right and left right is larger than the root or not
    if (left < n && arr[left] > arr[largest])
    {
        largest = left;
    }
 
    if (right < n && arr[right] > arr[largest])
    {
        largest = right;
    }
 
    // if the root is smaller than the children then swap it with the largest children's value
    if (largest != i)
    {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
 
        // again heapify that side of the heap where the root has gone
        heapify(arr, n, largest);
    }
}


void heapSort(int arr[], int n)
{
    // build the binary max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
 
    // sort the max heap
    for (int i = n - 1; i >= 0; i--)
    {
        // swap the root node and the last leaf node
        int temp = arr[i];
        arr[i] = arr[0];
        arr[0] = temp;
 
        // again heapify the max heap from the root 
        heapify(arr, i, 0);
    }
}

//merge and mergeSort function taken from webcourses merge2.c 
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    int *L = (int*) malloc(n1 * sizeof(int));
    int *R = (int*) malloc(n2 * sizeof(int));
    //add the allocated bytes to the overall amount of extra memory
    extraMemoryAllocated += (n1 * sizeof(int) + n2 * sizeof(int));
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if(L[i] <= R[j])
            arr[k] = L[i++];
        else
            arr[k] = R[j++];
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
        arr[k++] = L[i++];

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
        arr[k++] = R[j++];
    free(L);
    free(R);
}




// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
    if (l < r){
        // get the mid point
        int m = (l+r)/2;
        // Sort first and second halves
        mergeSort(pData, l, m);
        mergeSort(pData, m+1, r);
        merge(pData, l, m, r);
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\n\t");
	if(dataSz > 200){
	    for (i=0;i<100;++i)
		    printf("%d ",pData[i]);
	    printf("\n\t");
	}
	else{
	    for(i = 0; i < dataSz; i++){
	        printf("%d ", pData[i]);
	    }
	    printf("\n\n");
	    return;
	}
	    for (i=sz;i<dataSz;++i)
	    {
		    printf("%d ",pData[i]);
	    }
	    printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.3lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.3lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}
