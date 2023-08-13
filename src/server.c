/*
 * date_proc.c - remote procedures; called by server stub.
 */
#include <rpc/rpc.h>
#include <time.h>
#include <sys/types.h>
#include <linux/kernel.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "date.h"
#include <pwd.h>
#include <malloc.h>
#include <string.h>

#define MAX_LEN 100
/*
 * Return the binary date and time.
 */
char **date_1(long *option)
{
        struct tm *timeptr; /* Pointer to time structure      */
        time_t clock;       /* Clock value (in secs)          */
        static char *ptr;   /* Return string                  */
        static char err[] = "Invalid Response \0";
        static char s[MAX_LEN];

        clock = time(0);
        timeptr = localtime(&clock);

        switch (*option)
        {
        case 1:
                strftime(s, MAX_LEN, "%A, %B %d, %Y", timeptr);
                ptr = s;
                break;

        case 2:
                strftime(s, MAX_LEN, "%T", timeptr);
                ptr = s;
                break;

        case 3:
                strftime(s, MAX_LEN, "%A, %B %d, %Y - %T", timeptr);
                ptr = s;
                break;

        default:
                ptr = err;
                break;
        }
        return (&ptr);
}

double *cpu_1(void)
{
        // Open the /proc/stat file
        FILE *fp = fopen("/proc/stat", "r");

        if (fp == NULL)
        {
                printf("Failed to open /proc/stat file\n");
                return NULL;
        }

        // Read the first line of the /proc/stat file
        char line[256];
        fgets(line, sizeof(line), fp);

        // Close the /proc/stat file
        fclose(fp);

        // Parse the line to obtain CPU usage information
        char *token = strtok(line, " ");
        int user = atoi(strtok(NULL, " "));
        int nice = atoi(strtok(NULL, " "));
        int system = atoi(strtok(NULL, " "));
        int idle = atoi(strtok(NULL, " "));
        int iowait = atoi(strtok(NULL, " "));
        int irq = atoi(strtok(NULL, " "));
        int softirq = atoi(strtok(NULL, " "));
        int steal = atoi(strtok(NULL, " "));
        int guest = atoi(strtok(NULL, " "));
        int guest_nice = atoi(strtok(NULL, " "));

        // Compute the total CPU usage
        int total_cpu = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;

        // Compute the percentage of CPU usage
        static double cpu_usage;
        cpu_usage = (double)(total_cpu - idle) / total_cpu * 100.0;
        return &cpu_usage;
}

char **memory_1(void)
{
        static char *ptr;
        static double percent;
        struct sysinfo memInfo;
        sysinfo(&memInfo);
        long long totalPhysMem = memInfo.totalram;
        long long physMemUsed = memInfo.totalram - memInfo.freeram;
        percent = (double)physMemUsed / (double)totalPhysMem * 100;
        int charLength = 4;
        long long *ram_values = (long long *)malloc(charLength * sizeof(long long));
        static char responseString[MAX_LEN];
        memset(responseString, 0, sizeof(responseString));
        ram_values[0] = totalPhysMem;
        ram_values[1] = memInfo.freeram;
        ram_values[2] = physMemUsed;
        ram_values[3] = percent;

        int i = 0;
        for (i = 0; i < charLength; i++)
        {
                char temString[MAX_LEN];
                char temString2[MAX_LEN];
                switch (i)
                {
                case 0:
                        strncpy(temString, "Total RAM(B)", MAX_LEN);
                        break;
                case 1:
                        strncpy(temString, "Free RAM(B):", MAX_LEN);
                        break;
                case 2:
                        strncpy(temString, "RAM used(B):", MAX_LEN);
                        break;
                case 3:
                        strncpy(temString, "Percent Used(%%):", MAX_LEN);
                        break;
                default:
                        break;
                }
                int pos = 0;
                pos += sprintf(&temString2[pos], "%s", temString);
                sprintf(&temString2[pos], " %lld \n", ram_values[i]);
                strcat(responseString, temString2);
        }
        ptr = responseString;

        return &ptr;
}

char **dynamicmemoryinfo_1(void)
{
        static char *ptr;
        struct mallinfo info = mallinfo();
        double page_size = (double)getpagesize();
        double phys_pages = (double)get_phys_pages();
        int charLength = 4;
        double *mallinfo_values = (double *)malloc(charLength * sizeof(long double));

        static char responseString[MAX_LEN];
        memset(responseString, 0, sizeof(responseString));

        mallinfo_values[0] = (double)info.arena;   // total amount of memory allocated in bytes
        mallinfo_values[1] = (double)info.ordblks; // total number of chunks of free memory
        mallinfo_values[2] = (double)page_size;    // single page size of system in bytes
        mallinfo_values[3] = (double)phys_pages;   // total physical pages available
        int i = 0;
        for (i = 0; i < charLength; i++)
        {
                char temString[100];
                char temString2[100];
                switch (i)
                {
                case 0:
                        strncpy(temString, "Total Memory(B)", MAX_LEN);
                        break;
                case 1:
                        strncpy(temString, "Free Chunks(B):", MAX_LEN);
                        break;
                case 2:
                        strncpy(temString, "Single Page(B):", MAX_LEN);
                        break;
                case 3:
                        strncpy(temString, "Total pages:", MAX_LEN);
                        break;
                default:
                        break;
                }
                int pos = 0;
                pos += sprintf(&temString2[pos], "%s", temString);
                sprintf(&temString2[pos], " %0.2f \n", mallinfo_values[i]);
                strcat(responseString, temString2);
        }
        ptr = responseString;

        return &ptr;
}

char **load_1(void)
{
        static char *ptr;
        int charLength = 3;
        static char responseString[MAX_LEN];
        memset(responseString, 0, sizeof(responseString));

        static double *loadavg;
        loadavg = (double *)malloc(charLength * sizeof(double));
        int responseLoadAvg = getloadavg(loadavg, 3);
        if (responseLoadAvg < 0)
        {
                return NULL;
        }
        int i = 0;
        for (i = 0; i < charLength; i++)
        {
                char temString2[100];
                sprintf(temString2, " %f \t", loadavg[i]);
                strcat(responseString, temString2);
        }
        ptr = responseString;

        return &ptr;
}

char **user_1(void)
{
        static char *ptr; /* Return string                  */
        static char err[] = "Invalid Response \0";
        static char s[MAX_LEN];
        FILE *passwd_file = fopen("/etc/passwd", "r");
        if (passwd_file == NULL)
        {
                ptr = err;
                return (&ptr);
        }

        struct passwd *pw;
        while ((pw = fgetpwent(passwd_file)) != NULL)
        {
                sprintf(s, "%s\n", pw->pw_name);
        }
        ptr = s;

        fclose(passwd_file);
        return (&ptr);
}
