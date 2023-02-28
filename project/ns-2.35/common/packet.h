#ifndef ns_packet_h
#define ns_packet_h
#include <string.h>
#include <assert.h>
#include "config.h"
#include "scheduler.h"
#include "object.h"
#include "lib/bsd-list.h"
#include "packet-stamp.h"
#include "ns-process.h"
#define RT_PORT		255	
#define HDR_CMN(p)      (hdr_cmn::access(p))
#define HDR_ARP(p)      (hdr_arp::access(p))
#define HDR_MAC(p)      (hdr_mac::access(p))
#define HDR_MAC802_11(p) ((hdr_mac802_11 *)hdr_mac::access(p))
#define HDR_MAC_TDMA(p) ((hdr_mac_tdma *)hdr_mac::access(p))
#define HDR_SMAC(p)     ((hdr_smac *)hdr_mac::access(p))
#define HDR_LL(p)       (hdr_ll::access(p))
#define HDR_HDLC(p)     ((hdr_hdlc *)hdr_ll::access(p))
#define HDR_IP(p)       (hdr_ip::access(p))
#define HDR_RTP(p)      (hdr_rtp::access(p))
#define HDR_TCP(p)      (hdr_tcp::access(p))
#define HDR_SCTP(p)     (hdr_sctp::access(p))
#define HDR_SR(p)       (hdr_sr::access(p))
#define HDR_TFRC(p)     (hdr_tfrc::access(p))
#define HDR_TORA(p)     (hdr_tora::access(p))
#define HDR_IMEP(p)     (hdr_imep::access(p))
#define HDR_CDIFF(p)    (hdr_cdiff::access(p))  
#define HDR_LMS(p)		(hdr_lms::access(p))

typedef unsigned int packet_t;

static const packet_t PT_TCP = 0;
static const packet_t PT_UDP = 1;
static const packet_t PT_CBR = 2;
static const packet_t PT_AUDIO = 3;
static const packet_t PT_VIDEO = 4;
static const packet_t PT_ACK = 5;
static const packet_t PT_START = 6;
static const packet_t PT_STOP = 7;
static const packet_t PT_PRUNE = 8;
static const packet_t PT_GRAFT = 9;
static const packet_t PT_GRAFTACK = 10;
static const packet_t PT_JOIN = 11;
static const packet_t PT_ASSERT = 12;
static const packet_t PT_MESSAGE = 13;
static const packet_t PT_RTCP = 14;
static const packet_t PT_RTP = 15;
static const packet_t PT_RTPROTO_DV = 16;
static const packet_t PT_CtrMcast_Encap = 17;
static const packet_t PT_CtrMcast_Decap = 18;
static const packet_t PT_SRM = 19;
static const packet_t PT_REQUEST = 20;
static const packet_t PT_ACCEPT = 21;
static const packet_t PT_CONFIRM = 22;
static const packet_t PT_TEARDOWN = 23;
static const packet_t PT_LIVE = 24;   
static const packet_t PT_REJECT = 25;
static const packet_t PT_TELNET = 26; 
static const packet_t PT_FTP = 27;
static const packet_t PT_PARETO = 28;
static const packet_t PT_EXP = 29;
static const packet_t PT_INVAL = 30;
static const packet_t PT_HTTP = 31;
static const packet_t PT_ENCAPSULATED = 32;
static const packet_t PT_MFTP = 33;
static const packet_t PT_ARP = 34;
static const packet_t PT_MAC = 35;
static const packet_t PT_TORA = 36;
static const packet_t PT_DSR = 37;
static const packet_t PT_AODV = 38;
static const packet_t PT_IMEP = 39;
static const packet_t PT_RAP_DATA = 40;
static const packet_t PT_RAP_ACK = 41;
static const packet_t PT_TFRC = 42;
static const packet_t PT_TFRC_ACK = 43;
static const packet_t PT_PING = 44;        
static const packet_t PT_PBC = 45;
static const packet_t PT_DIFF = 46;
static const packet_t PT_RTPROTO_LS = 47;
static const packet_t PT_LDP = 48;
static const packet_t PT_GAF = 49;
static const packet_t PT_REALAUDIO = 50;
static const packet_t PT_PUSHBACK = 51;
  
  #ifdef HAVE_STL
static const packet_t PT_PGM = 52;
  #endif 
static const packet_t PT_LMS = 53;
static const packet_t PT_LMS_SETUP = 54;
static const packet_t PT_SCTP = 55;
static const packet_t PT_SCTP_APP1 = 56;
static const packet_t PT_SMAC = 57;
static const packet_t PT_XCP = 58;
static const packet_t PT_HDLC = 59;
static const packet_t PT_BLTRACE = 60;
static const packet_t PT_AOMDV = 61;
static const packet_t PT_PUMA = 62;
static const packet_t PT_DCCP = 63;
static const packet_t PT_DCCP_REQ = 64;
static const packet_t PT_DCCP_RESP = 65;
static const packet_t PT_DCCP_ACK = 66;
static const packet_t PT_DCCP_DATA = 67;
static const packet_t PT_DCCP_DATAACK = 68;
static const packet_t PT_DCCP_CLOSE  = 69;
static const packet_t PT_DCCP_CLOSEREQ = 70;
static const packet_t PT_DCCP_RESET = 71;
static const packet_t PT_MDART = 72;
static const packet_t PT_ASPU = 73;
static packet_t       PT_NTYPE = 74; 
enum packetClass
{
	UNCLASSIFIED,
	ROUTING,
	DATApkt
  };

class PacketClassifier
{
	public:
		PacketClassifier(): next_(0){}
		virtual ~PacketClassifier() {}
		void setNext(PacketClassifier *next){next_ = next;}
		PacketClassifier *getNext(){return next_;}
		packetClass classify(packet_t type) 
		{
		        packetClass c = getClass(type);
		        if(c == UNCLASSIFIED && next_)
		                c = next_->classify(type);
		        return c;
		}

	protected:
		virtual packetClass getClass(packet_t type) = 0;        
		PacketClassifier *next_;
};

class p_info {
public:
	p_info()
	{
		initName();
	}
	const char* name(packet_t p) const { 
		if ( p <= p_info::nPkt_ ) return name_[p];
		return 0;
	}
	static bool data_packet(packet_t type) {
		return ( (type) == PT_TCP || \
		         (type) == PT_TELNET || \
		         (type) == PT_CBR || \
		         (type) == PT_AUDIO || \
		         (type) == PT_VIDEO || \
		         (type) == PT_ACK || \
		         (type) == PT_SCTP || \
		         (type) == PT_SCTP_APP1 || \
		         (type) == PT_HDLC || \
				 (type) == PT_ASPU \
		        );
	}
	static packetClass classify(packet_t type) {		
		if (type == PT_DSR || 
		    type == PT_MESSAGE || 
		    type == PT_TORA ||
		    type == PT_PUMA ||
		    type == PT_AODV ||
		    type == PT_MDART)
			return ROUTING;		
		if (type == PT_TCP || 
		    type == PT_TELNET || 
		    type == PT_CBR || 
		    type == PT_AUDIO || 
		    type == PT_VIDEO || 
		    type == PT_ACK || 
		    type == PT_SCTP || 
		    type == PT_SCTP_APP1 || 
		    type == PT_HDLC)
			return DATApkt;
		if (pc_)
			return pc_->classify(type);
		return UNCLASSIFIED;
	}
	static void addPacketClassifier(PacketClassifier *pc)
	{
		if(!pc)
		        return;
		pc->setNext(pc_);
		pc_ = pc;
	}       
	static void initName()
	{
		if(nPkt_ >= PT_NTYPE+1)
		        return;
		char **nameNew = new char*[PT_NTYPE+1];
		for(unsigned int i = (unsigned int)PT_SMAC+1; i < nPkt_; i++)
		{
		        nameNew[i] = name_[i];
		}
		if(!nPkt_)
		        delete [] name_;
		name_ = nameNew;
		nPkt_ = PT_NTYPE+1;
		name_[PT_TCP]= "tcp";
		name_[PT_UDP]= "udp";
		name_[PT_CBR]= "cbr";
		name_[PT_AUDIO]= "audio";
		name_[PT_VIDEO]= "video";
		name_[PT_ACK]= "ack";
		name_[PT_START]= "start";
		name_[PT_STOP]= "stop";
		name_[PT_PRUNE]= "prune";
		name_[PT_GRAFT]= "graft";
		name_[PT_GRAFTACK]= "graftAck";
		name_[PT_JOIN]= "join";
		name_[PT_ASSERT]= "assert";
		name_[PT_MESSAGE]= "message";
		name_[PT_RTCP]= "rtcp";
		name_[PT_RTP]= "rtp";
		name_[PT_RTPROTO_DV]= "rtProtoDV";
		name_[PT_CtrMcast_Encap]= "CtrMcast_Encap";
		name_[PT_CtrMcast_Decap]= "CtrMcast_Decap";
		name_[PT_SRM]= "SRM";
		name_[PT_REQUEST]= "sa_req";	
		name_[PT_ACCEPT]= "sa_accept";
		name_[PT_CONFIRM]= "sa_conf";
		name_[PT_TEARDOWN]= "sa_teardown";
		name_[PT_LIVE]= "live"; 
		name_[PT_REJECT]= "sa_reject";
		name_[PT_TELNET]= "telnet";
		name_[PT_FTP]= "ftp";
		name_[PT_PARETO]= "pareto";
		name_[PT_EXP]= "exp";
		name_[PT_INVAL]= "httpInval";
		name_[PT_HTTP]= "http";
		name_[PT_ENCAPSULATED]= "encap";
		name_[PT_MFTP]= "mftp";
		name_[PT_ARP]= "ARP";
		name_[PT_MAC]= "MAC";
		name_[PT_TORA]= "TORA";
		name_[PT_DSR]= "DSR";
		name_[PT_AODV]= "AODV";
		name_[PT_MDART]= "MDART";
		name_[PT_IMEP]= "IMEP";
		name_[PT_RAP_DATA] = "rap_data";
		name_[PT_RAP_ACK] = "rap_ack";
 		name_[PT_TFRC]= "tcpFriend";
		name_[PT_TFRC_ACK]= "tcpFriendCtl";
		name_[PT_PING]="ping";
		name_[PT_PBC] = "PBC";
 		name_[PT_DIFF] = "diffusion";
		name_[PT_RTPROTO_LS] = "rtProtoLS";
		name_[PT_LDP] = "LDP";
                name_[PT_GAF] = "gaf";      
		name_[PT_REALAUDIO] = "ra";
		name_[PT_PUSHBACK] = "pushback";

#ifdef HAVE_STL
		name_[PT_PGM] = "PGM";
#endif 
		name_[PT_LMS]="LMS";
		name_[PT_LMS_SETUP]="LMS_SETUP";
		name_[PT_SCTP]= "sctp";
 		name_[PT_SCTP_APP1] = "sctp_app1";
		name_[PT_SMAC]="smac";
		name_[PT_HDLC]="HDLC";
		name_[PT_XCP]="xcp";
		name_[PT_BLTRACE]="BellLabsTrace";
		name_[PT_AOMDV]= "AOMDV";
		name_[PT_PUMA]="PUMA";
		name_[PT_DCCP]="DCCP";
		name_[PT_DCCP_REQ]="DCCP_Request";
		name_[PT_DCCP_RESP]="DCCP_Response";
		name_[PT_DCCP_ACK]="DCCP_Ack";
		name_[PT_DCCP_DATA]="DCCP_Data";
		name_[PT_DCCP_DATAACK]="DCCP_DataAck";
		name_[PT_DCCP_CLOSE]="DCCP_Close";
		name_[PT_DCCP_CLOSEREQ]="DCCP_CloseReq";
		name_[PT_DCCP_RESET]="DCCP_Reset";
		name_[PT_ASPU]="aspu";
		name_[PT_NTYPE]= "undefined";
	}
	static int addPacket(char *name);
	static packet_t getType(const char *name)
	{
		for(unsigned int i = 0; i < nPkt_; i++)
		{
		        if(strcmp(name, name_[i]) == 0)
				return i;
		}
		return PT_NTYPE;

	}
private:
	static char** name_;
	static unsigned int nPkt_;
	static PacketClassifier *pc_;
};
extern p_info packet_info; 

#define DATA_PACKET(type) ( (type) == PT_TCP || \
                            (type) == PT_TELNET || \
                            (type) == PT_CBR || \
                            (type) == PT_AUDIO || \
                            (type) == PT_VIDEO || \
                            (type) == PT_ACK || \
                            (type) == PT_SCTP || \
                            (type) == PT_SCTP_APP1 \
                            )
#define OFFSET(type, field) ( (char *)&( ((type *)256)->field )  - (char *)256)

class PacketData : public AppData {
public:
	PacketData(int sz) : AppData(PACKET_DATA) {
		datalen_ = sz;
		if (datalen_ > 0)
			data_ = new unsigned char[datalen_];
		else
			data_ = NULL;
	}
	PacketData(PacketData& d) : AppData(d) {
		datalen_ = d.datalen_;
		if (datalen_ > 0) {
			data_ = new unsigned char[datalen_];
			memcpy(data_, d.data_, datalen_);
		} else
			data_ = NULL;
	}
	virtual ~PacketData() { 
		if (data_ != NULL) 
			delete []data_; 
	}
	unsigned char* data() { return data_; }
	virtual int size() const { return datalen_; }
	virtual AppData* copy() { return new PacketData(*this); }
private:
	unsigned char* data_;
	int datalen_;
};

typedef void (*FailureCallback)(Packet *,void *);

class Packet : public Event {
private:
	unsigned char* bits_;	

	AppData* data_;		
	static void init(Packet*);     
	bool fflag_;
protected:
	static Packet* free_;	
	int	ref_count_;	
public:
	Packet* next_;		
	static int hdrlen_;
	Packet() : bits_(0), data_(0), ref_count_(0), next_(0) { }
	inline unsigned char* bits() { return (bits_); }
	inline Packet* copy() const;
	inline Packet* refcopy() { ++ref_count_; return this; }
	inline int& ref_count() { return (ref_count_); }
	static inline Packet* alloc();
	static inline Packet* alloc(int);
	inline void allocdata(int);
	inline void initdata() { data_  = 0;}
	static inline void free(Packet*);
	inline unsigned char* access(int off) const {
		if (off < 0)
			abort();
		return (&bits_[off]);
	}
	inline unsigned char* accessdata() const { 
		if (data_ == 0)
			return 0;
		assert(data_->type() == PACKET_DATA);
		return (((PacketData*)data_)->data()); 
	}
	inline AppData* userdata() const {
		return data_;
	}
	inline void setdata(AppData* d) { 
		if (data_ != NULL)
			delete data_;
		data_ = d; 
	}
	inline int datalen() const { return data_ ? data_->size() : 0; }
	static void dump_header(Packet *p, int offset, int length);
        PacketStamp	txinfo_;  
        u_int8_t        incoming;
};

class iface_literal {
public:
	enum iface_constant { 
		UNKN_IFACE= -1, 
		ANY_IFACE= -2   
	};
	iface_literal(const iface_constant i, const char * const n) : 
		value_(i), name_(n) {}
	inline int value() const { return value_; }
	inline const char * name() const { return name_; }
private:
	const iface_constant value_;
	const char * const name_; 
};

static const iface_literal UNKN_IFACE(iface_literal::UNKN_IFACE, "?");
static const iface_literal ANY_IFACE(iface_literal::ANY_IFACE, "*");
enum ns_af_enum { NS_AF_NONE, NS_AF_ILINK, NS_AF_INET };
enum ModulationScheme {BPSK = 0, QPSK = 1, QAM16 = 2, QAM64 = 3};
struct hdr_cmn {
	enum dir_t { DOWN= -1, NONE= 0, UP= 1 };
	packet_t ptype_;	
	int	size_;		
	int	uid_;		
	int	error_;		
	int     errbitcnt_;     
	int     fecsize_;
	double	ts_;		
	int	iface_;		
	dir_t	direction_;	 
        char src_rt_valid;
	double ts_arr_; 
	nsaddr_t prev_hop_;     
	nsaddr_t next_hop_;	
	int      addr_type_; 
	nsaddr_t last_hop_;     
	int aomdv_salvage_count_;
        FailureCallback xmit_failure_; 
        void *xmit_failure_data_;
        int     xmit_reason_;
#define XMIT_REASON_RTS 0x01
#define XMIT_REASON_ACK 0x02
        int num_forwards_;	
        int opt_num_forwards_;   
	double txtime_;
	inline double& txtime() { return(txtime_); }

	static int offset_;	
	inline static int& offset() { return offset_; }
	inline static hdr_cmn* access(const Packet* p) {
		return (hdr_cmn*) p->access(offset_);
	}
	
	inline packet_t& ptype() { return (ptype_); }
	inline int& size() { return (size_); }
	inline int& uid() { return (uid_); }
	inline int& error() { return error_; }
	inline int& errbitcnt() {return errbitcnt_; }
	inline int& fecsize() {return fecsize_; }
	inline double& timestamp() { return (ts_); }
	inline int& iface() { return (iface_); }
	inline dir_t& direction() { return (direction_); }
	inline nsaddr_t& next_hop() { return (next_hop_); }
	inline int& addr_type() { return (addr_type_); }
	inline int& num_forwards() { return (num_forwards_); }
	inline int& opt_num_forwards() { return (opt_num_forwards_); }
	ModulationScheme mod_scheme_;
	inline ModulationScheme& mod_scheme() { return (mod_scheme_); }
};

class PacketHeaderClass : public TclClass {
protected:
	PacketHeaderClass(const char* classname, int hdrsize);
	virtual int method(int argc, const char*const* argv);
	void field_offset(const char* fieldname, int offset);
	inline void bind_offset(int* off) { offset_ = off; }
	inline void offset(int* off) {offset_= off;}
	int hdrlen_;		
	int* offset_;		
public:
	virtual void bind();
	virtual void export_offsets();
	TclObject* create(int argc, const char*const* argv);
};

inline void Packet::init(Packet* p)
{
	bzero(p->bits_, hdrlen_);
}

inline Packet* Packet::alloc()
{
	Packet* p = free_;
	if (p != 0) {
		assert(p->fflag_ == FALSE);
		free_ = p->next_;
		assert(p->data_ == 0);
		p->uid_ = 0;
		p->time_ = 0;
	} else {
		p = new Packet;
		p->bits_ = new unsigned char[hdrlen_];
		if (p == 0 || p->bits_ == 0)
			abort();
	}
	init(p); 
	(HDR_CMN(p))->next_hop_ = -2; 
	(HDR_CMN(p))->last_hop_ = -2; 
	p->fflag_ = TRUE;
	(HDR_CMN(p))->direction() = hdr_cmn::DOWN;
	p->next_ = 0;
	return (p);
}

inline void Packet::allocdata(int n)
{
	assert(data_ == 0);
	data_ = new PacketData(n);
	if (data_ == 0)
		abort();
}

inline Packet* Packet::alloc(int n)
{
	Packet* p = alloc();
	if (n > 0) 
		p->allocdata(n);
	return (p);
}

#include "dccp/dccp_packets.h"

inline void Packet::free(Packet* p)
{
        hdr_dccp *dccph;
	if (p->fflag_) {
		if (p->ref_count_ == 0) {
                        switch (HDR_CMN(p)->ptype_){
                        case PT_DCCP:
                        case PT_DCCP_REQ:
                        case PT_DCCP_RESP:
                        case PT_DCCP_ACK:
                        case PT_DCCP_DATA:
                        case PT_DCCP_DATAACK:
                        case PT_DCCP_CLOSE:
                        case PT_DCCP_CLOSEREQ:
                        case PT_DCCP_RESET:
                                dccph = hdr_dccp::access(p);
                                if (dccph->options_ != NULL){
                                        delete (dccph->options_);
                                }
                                break;
                        default:
                                ;
                        }
			assert(p->uid_ <= 0);
			if (p->data_ != 0) {
				delete p->data_;
				p->data_ = 0;
			}
			init(p);
			p->next_ = free_;
			free_ = p;
			p->fflag_ = FALSE;
		} else {
			--p->ref_count_;
		}
	}
}

inline Packet* Packet::copy() const
{
        hdr_dccp *dccph, *dccph_p;
	Packet* p = alloc();
	memcpy(p->bits(), bits_, hdrlen_);
        switch (HDR_CMN(this)->ptype_){
        case PT_DCCP:
        case PT_DCCP_REQ:
        case PT_DCCP_RESP:
        case PT_DCCP_ACK:
        case PT_DCCP_DATA:
        case PT_DCCP_DATAACK:
        case PT_DCCP_CLOSE:
        case PT_DCCP_CLOSEREQ:
        case PT_DCCP_RESET:
                dccph = hdr_dccp::access(this);
                dccph_p = hdr_dccp::access(p);
                if (dccph->options_ != NULL)
                        dccph_p->options_ = new DCCPOptions(*dccph->options_);
                break;
        default:
                ;
        }
	if (data_) 
		p->data_ = data_->copy();
	p->txinfo_.init(&txinfo_);
	return (p);
}

inline void
Packet::dump_header(Packet *p, int offset, int length)
{
        assert(offset + length <= p->hdrlen_);
        struct hdr_cmn *ch = HDR_CMN(p);
        fprintf(stderr, "\nPacket ID: %d\n", ch->uid());
        for(int i = 0; i < length ; i+=16) {
                fprintf(stderr, "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
                        p->bits_[offset + i],     p->bits_[offset + i + 1],
                        p->bits_[offset + i + 2], p->bits_[offset + i + 3],
                        p->bits_[offset + i + 4], p->bits_[offset + i + 5],
                        p->bits_[offset + i + 6], p->bits_[offset + i + 7],
                        p->bits_[offset + i + 8], p->bits_[offset + i + 9],
                        p->bits_[offset + i + 10], p->bits_[offset + i + 11],
                        p->bits_[offset + i + 12], p->bits_[offset + i + 13],
                        p->bits_[offset + i + 14], p->bits_[offset + i + 15]);
        }
}
#endif