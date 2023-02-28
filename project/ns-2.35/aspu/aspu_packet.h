#ifndef __aspu_packet_h__
#define __aspu_packet_h__
#include <packet.h>
#define ASPU_TYPE_HELLO		0x00
#define ASPU_TYPE_DARQ		0x01
#define ASPU_TYPE_DARP		0x02
#define ASPU_TYPE_DAUP		0x03
#define ASPU_TYPE_DABR		0x04
#define ASPU_TYPE_ENCP		0x05
#define ASPU_TYPE_INST		0x06
#define ASPU_TYPE_INRQ		0x07
#define ASPU_TYPE_INRP		0x08
#define DATYPE_BROADCAST	((u_int32_t) 0xffffffff)
#define HDR_ASPU(p)			(struct hdr_aspu *)hdr_aspu::access(p)
#define HDR_ASPU_HELLO(p)	(struct hdr_aspu_hello *)hdr_aspu::access(p)
#define HDR_ASPU_DARQ(p)		(struct hdr_aspu_darq *)hdr_aspu::access(p)
#define HDR_ASPU_DARP(p)		(struct hdr_aspu_darp *)hdr_aspu::access(p)
#define HDR_ASPU_DAUP(p)		(struct hdr_aspu_daup *)hdr_aspu::access(p)
#define HDR_ASPU_DABR(p)		(struct hdr_aspu_dabr *)hdr_aspu::access(p)
#define HDR_ASPU_ENCP(p)		(struct hdr_aspu_encp *)hdr_aspu::access(p)
#define HDR_ASPU_INFO(p)		(struct hdr_aspu_info *)hdr_aspu::access(p)

struct hdr_aspu {
	u_int8_t	type_;
	static int offset_;
	inline static int& offset() {
		return offset_;
	}
	inline static hdr_aspu* access(const Packet* p) {
		return (hdr_aspu*)p->access(offset_);
	}
	inline int size() {
		int size_ = 0;
		size_ = sizeof(int)			
				+ sizeof(u_int8_t);
		assert(size_ >= 0);
		return size_;
	}
};

struct hdr_aspu_hello {
	u_int8_t	type_;
	nsaddr_t	srcId_;					
	nsaddr_t	srcAdd_;			
	nsaddr_t	dstAdd_;				
	u_int32_t	seqNum_;				
	inline int size() {
		int size_ = 0;
		size_ = sizeof(u_int8_t)		
				+ sizeof(nsaddr_t)		
				+ sizeof(nsaddr_t)		
				+ sizeof(nsaddr_t)		
				+ sizeof(u_int32_t);	
		assert(size_ >= 0);
		return size_;
	}
};

struct hdr_aspu_darq {
	u_int8_t		type_;
	nsaddr_t		srcId_;				
	nsaddr_t		srcAdd_;			
	nsaddr_t		forId_;				
	nsaddr_t		forAdd_;			
	nsaddr_t		dstId_;				
	nsaddr_t		dstAdd_;			
	nsaddr_t		reqId_;				
	double			lifetime_;			
	u_int32_t		seqNum_;			
	inline int size() {
		int size_ = 0;
		size_ = sizeof(u_int8_t)	
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(double)			
			+ sizeof(u_int32_t);		
		assert(size_ >= 0);
		return size_;
	}
};

struct hdr_aspu_darp {
	u_int8_t		type_;				
	nsaddr_t		srcId_;				
	nsaddr_t		srcAdd_;			
	nsaddr_t		forId_;			
	nsaddr_t		forAdd_;			
	nsaddr_t		dstId_;				
	nsaddr_t		dstAdd_;			
	nsaddr_t		reqId_;				
	nsaddr_t		reqAdd_;		
	double		lifetime_;			
	u_int32_t	seqNum_;			
	inline int size() {
		int size_ = 0;
		size_ = sizeof(u_int8_t)	
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(double)			
			+ sizeof(u_int32_t);		
		assert(size_ >= 0);
		return size_;
	}
};

struct hdr_aspu_daup {
	u_int8_t	type_;
	nsaddr_t	srcId_;				
	nsaddr_t	srcAdd_;			
	nsaddr_t	forId_;				
	nsaddr_t	forAdd_;			
	nsaddr_t	dstId_;				
	nsaddr_t	dstAdd_;			
	double		lifetime_;			
	u_int32_t	seqNum_;			
		inline int size() {
		int size_ = 0;
		size_ = sizeof(u_int8_t)	
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(double)		
			+ sizeof(u_int32_t);	
		assert(size_ >= 0);
		return size_;
	}
};

struct hdr_aspu_dabr {
	u_int8_t	type_;
	nsaddr_t	srcId_;			
	nsaddr_t	srcAdd_;		
	nsaddr_t	dstAdd_;	
	inline int size() {
		int size_ = 0;
		size_ = sizeof(u_int8_t)	
				+ sizeof(nsaddr_t)	
				+ sizeof(nsaddr_t)	
				+ sizeof(nsaddr_t);	
		assert(size_ >= 0);
		return size_;
	}
};

struct hdr_aspu_encp {
	nsaddr_t	srcId_;					
	nsaddr_t	srcAdd_;				
	nsaddr_t	forId_;				
	nsaddr_t	forAdd_;				
	nsaddr_t	dstId_;				
	nsaddr_t	dstAdd_;			
	double		lifetime_;			
	u_int32_t	txCount_;			
	u_int32_t	darqCount_;			
	inline int size() {
		int size_ = 0;
		size_ = sizeof(nsaddr_t)	
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(double)		
			+ sizeof(u_int32_t)		
			+ sizeof(u_int32_t);	
		assert(size_ >= 0);
		return size_;
	}
};

struct hdr_aspu_info {
	u_int8_t	type_;
	nsaddr_t	srcId_;					
	nsaddr_t	srcAdd_;			
	nsaddr_t	forId_;				
	nsaddr_t	forAdd_;				
	nsaddr_t	dstId_;				
	nsaddr_t	dstAdd_;			
	double lifetime_;				
	u_int32_t seqNum_;				
	inline int size() {
		int size_ = 0;
		size_ = sizeof(u_int8_t)	
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(nsaddr_t)		
			+ sizeof(double)			
			+ sizeof(u_int32_t);	
		assert(size_ >= 0);
		return size_;
	}
};
union hdr_all_aspu {
	hdr_aspu           h;
	hdr_aspu_hello hello;
	hdr_aspu_darq darq;
	hdr_aspu_darp darp;
	hdr_aspu_daup daup;
	hdr_aspu_dabr dabr;
	hdr_aspu_encp encp;
	hdr_aspu_info info;
};
#endif