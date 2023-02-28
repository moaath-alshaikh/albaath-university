#ifndef __cmu_trace__
#define __cmu_trace__
#include "trace.h"
#include "god.h"
#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ ("")
#endif 
#define	DROP            'D'
#define	RECV            'r'
#define	SEND    	's'
#define	FWRD    	'f'
#define EOT             'x'
#define TR_ROUTER	0x01
#define TR_MAC		0x02
#define TR_IFQ		0x04
#define TR_AGENT	0x08
#define TR_PHY		0x10
#define DROP_END_OF_SIMULATION		"END"
#define	DROP_MAC_COLLISION		"COL"
#define DROP_MAC_DUPLICATE		"DUP"
#define DROP_MAC_PACKET_ERROR		"ERR"
#define DROP_MAC_RETRY_COUNT_EXCEEDED	"RET"
#define DROP_MAC_INVALID_STATE		"STA"
#define DROP_MAC_BUSY			"BSY"
#define DROP_MAC_INVALID_DST            "DST"
#define DROP_MAC_SLEEP                  "SLP"   
#define DROP_RTR_NO_ROUTE		"NRTE"  
#define DROP_RTR_ROUTE_LOOP		"LOOP"  
#define DROP_RTR_TTL                    "TTL"   
#define DROP_RTR_QFULL                  "IFQ"   
#define DROP_RTR_QTIMEOUT               "TOUT"  
#define DROP_RTR_MAC_CALLBACK           "CBK"   
#define DROP_RTR_SALVAGE	        "SAL"
#define DROP_IFQ_QFULL                  "IFQ"   
#define DROP_IFQ_ARP_FULL               "ARP"   
#define DROP_IFQ_FILTER                 "FIL"
#define DROP_OUTSIDE_SUBNET             "OUT"  
#define MAX_ID_LEN	3
#define MAX_NODE	4096

class PacketTracer
{
	public:
		PacketTracer();
        	virtual ~PacketTracer();
		void setNext(PacketTracer *next);
		PacketTracer *getNext();
		int format_unknow(Packet *p, int offset, BaseTrace *pt_, int newtrace);
	protected:
		virtual int format(Packet *p, int offset, BaseTrace *pt_, int newtrace) = 0;	//return 0 if the packet is unknown
		PacketTracer *next_;
};


class CMUTrace : public Trace {
public:
	CMUTrace(const char *s, char t);
	void	recv(Packet *p, Handler *h);
	void	recv(Packet *p, const char* why);

	static void addPacketTracer(PacketTracer *pt);


private:
	char	tracename[MAX_ID_LEN + 1];
	int	nodeColor[MAX_NODE];
        int     tracetype;
        MobileNode *node_;
	int     newtrace_;

        static double  bradius;
        static double  radius_scaling_factor_;
        static double  duration_scaling_factor_;
        static void calculate_broadcast_parameters();
        int initialized() { return node_ && 1; }
	int node_energy();
	int	command(int argc, const char*const* argv);
	void	format(Packet *p, const char *why);
    void    nam_format(Packet *p, int offset);
	void	format_phy(Packet *p, int offset);
	void	format_mac_common(Packet *p, const char *why, int offset);
	void    format_mac(Packet *p, int offset);
	void    format_smac(Packet *p, int offset);
	void	format_ip(Packet *p, int offset);
	void	format_arp(Packet *p, int offset);
	void    format_hdlc(Packet *p, int offset);
	void	format_dsr(Packet *p, int offset);
	void	format_msg(Packet *p, int offset);
	void	format_tcp(Packet *p, int offset);
	void    format_sctp(Packet *p, int offset);
	void	format_rtp(Packet *p, int offset);
	void	format_tora(Packet *p, int offset);
    void    format_imep(Packet *p, int offset);
    void    format_aodv(Packet *p, int offset);
	void    format_aomdv(Packet *p, int offset);
	void    format_mdart(Packet *p, int offset);
	void    format_aspu(Packet *p, int offset);
	static PacketTracer *pktTrc_;
};
#endif 