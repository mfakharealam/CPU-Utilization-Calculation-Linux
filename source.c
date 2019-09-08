#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void main()
{
	int count = 0, current_total_sum = 0, previous_total_sum = 0, previous_idle = 0, current_idle = 0, i = 0, a = 0, b = 0;
	float cpu_util = 0.0;
	char cpu_usage[1024], delim[2] = " ";
	FILE* file = popen("grep 'cpu ' /proc/stat", "r");
	if(!file){
		perror("popen");
		exit(EXIT_FAILURE);
	}
	while(fgets(cpu_usage, sizeof(cpu_usage), file))
	{
		i = 0;
		// puts(cpu_usage);
		char* token = strtok(cpu_usage, delim);
		while(token != NULL)
		{
			token = strtok(NULL, delim);	// skipped first cpu string
			if(token)
				current_total_sum += atoi(token);
			if(i == 3)
				current_idle = atoi(token);
			i++; 
		}
		if(count == 1)	// for average of the two
		{
			a = current_idle - previous_idle;
			b = current_total_sum - previous_total_sum;
			cpu_util = (float)a/b;
			cpu_util = 1.0 - cpu_util;
			cpu_util *= 100;
			printf("Average CPU Utilization: %lf%% \n\n", cpu_util);
			cpu_util = 0.0;
			count = 0;
		}
		sleep(1);
		file = popen("grep 'cpu ' /proc/stat", "r");
		previous_total_sum = current_total_sum;
		previous_idle = current_idle;
		current_total_sum = 0;
		count++;
	}
	pclose(file);
}
