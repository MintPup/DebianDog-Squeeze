/* saintless 20170711 - alternative chpupsocket for DebianDog - License GPLv3 
source: https://unix.stackexchange.com/questions/263912/how-can-i-succesfully-call-the-execv-function
 */

#include <unistd.h>

int main(int arcg, char *argv[])
{
    char *const args[] = {"chown", *(argv+1), "/tmp/pup_volume_monitor_socket", 0}; 
    execv("/bin/chown", args);
}
