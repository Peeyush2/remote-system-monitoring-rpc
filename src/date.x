/*
 * date.x - Specification of remote date, time, date and time service.
 */

/*
 * Define 1 procedure :
 * date_1() accepts a long and returns a string. 
 */

program DATE_PROG {
    version DATE_VERS {
	string	DATE(long) = 1;     /* procedure number = 1 */ 
    string MEMORY(void) = 2;
    double CPU(void) = 3;
    string USER(void)=4;
    string LOAD(void)=5;
    string DYNAMICMEMORYINFO(void)=6;
    } = 1;			                /* version number = 1     */
} = 0x31234567;			   /* program number        */

