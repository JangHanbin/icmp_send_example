#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>

using namespace std;

int in_cksum( u_short *p, int n );

int main(int argc, char* argv[])
{
    int raw_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if(raw_fd<0)
    {
        perror("Socket open error : ");
        exit(1);
    }

    struct sockaddr_in din;
    memset(&din,0,sizeof(din));
    din.sin_family = AF_INET;
    din.sin_addr.s_addr = inet_addr(argv[1]);



    struct icmp icmph;
    memset(&icmph, 0,sizeof(struct icmp));
    icmph.icmp_type=0xfc;
    icmph.icmp_code=IPPROTO_ICMP;
    icmph.icmp_cksum=in_cksum((u_short*)&icmph,sizeof(icmph));

    if (sendto(raw_fd, &icmph, sizeof(icmph), MSG_DONTWAIT, (struct sockaddr*)&din, sizeof(din)) < 0)
    {
        perror("sendto error! ");
        exit(1);
    }

    return 0;
}

int in_cksum( u_short *p, int n )
{
    register u_short answer;
    register long sum = 0;
    u_short odd_byte = 0;

    while( n > 1 )
    {
        sum += *p++;
        n -= 2;

    }/* WHILE */


    /* mop up an odd byte, if necessary */
    if( n == 1 )
    {
        *( u_char* )( &odd_byte ) = *( u_char* )p;
        sum += odd_byte;

    }/* IF */

    sum = ( sum >> 16 ) + ( sum & 0xffff );    /* add hi 16 to low 16 */
    sum += ( sum >> 16 );                    /* add carry */
    answer = ~sum;                            /* ones-complement, truncate*/

    return ( answer );

} /* in_cksum() */

