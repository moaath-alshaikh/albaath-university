#ifndef __aspu_h__        
#define __aspu_h__

#include <aspu/aspu_function.h>
#include <aspu/aspu_table.h>

class NDP;
class ASPU;
class ADP;
class RoutingTable;

class ASPUTimer : public Handler {
	public:
		ASPUTimer(ASPU* aspu) {
			aspu_ = aspu;
			numberOfCall_ = 0;
		}
		void 		handle(Event*);
	private:
		ASPU*		aspu_;
		int		numberOfCall_;
		Event		intr;
}; 

class ASPU : public Agent {
	friend class ASPUTimer;
	friend class NDP;
	friend class ADP;
	friend class RoutingTable; 
public:
	ASPU(nsaddr_t);
	~ASPU();
	int command(int, const char* const*);
	inline NsObject*		getTarget() {
		return target_;
	}

	void recv(Packet*, Handler*);
	void macFailed(Packet*);
	void recvRoutingUpdate();
	void validateAddress();

private:
	void	startTimer() {
		aspuTimer_->handle((Event*) 0);
	}
	
	void format(Packet*);
	void forward(Packet*);
	void recvASPU(Packet*);
	void sendDARequest(nsaddr_t dst);
	void sendDAUpdate();
	
	PortClassifier*	dmux_;
	Trace*			logtarget_;
	ASPUTimer*		aspuTimer_;
	NDP*			ndp_;
	ADP*			adp_;
	RoutingTable*	routingTable_;
	ASPUQueue* 				queue_;
	nsaddr_t		address_;
	nsaddr_t		oldAddress_;
	nsaddr_t		id_;		
	int			macFailed_;
	
	PriQueue        *ifqueue; 
	int accesible_var_;
	int			etxMetric_;
int numinterf;
NsObject *targetlist[MAXINTERF];
PriQueue *ifqueuelist[MAXINTERF];
};

class NDPHelloTimer;
class NDPNeighborTimer;
class ASPU;

struct neighborCompare {
	bool operator() (const Neighbor* entry1, const Neighbor* entry2) const {
		assert(entry1);
		assert(entry2);
		if ((entry1->insertionPoint() > entry2->insertionPoint()) || ((entry1->insertionPoint() == entry2->insertionPoint()) && (entry1->id() < entry2->id()))) {
			return 1;
		}
		return 0;
	}
};

typedef std::multiset<Neighbor*, neighborCompare> neighborSet;

class NDP {
	friend class ASPU;
	public:
		NDP(ASPU*);
		~NDP();
		void		sendHello();
		void		recvHello(Packet*);
		void		startHelloTimer();
		void		startNeighborTimer();
		void		neighborInsert(nsaddr_t, nsaddr_t, u_int32_t, string, double);
		Neighbor*	neighborLookup(nsaddr_t);
		void		neighborPurge();
		void		neighborPrint();
		int			neighborDegree();
		int			realNeighborDegree();
		string		getNeighborQuality();
		void		selectAddress();
		bool		validateAddress();
		void		updateRoutingTable();
	private:
		ASPU*				aspu_;
		NDPHelloTimer*		helloTimer_;
		NDPNeighborTimer*	neighborTimer_;
		neighborSet*		neighborSet_;
		inline u_int32_t	helloSeqNum() {
			return helloSeqNum_++;
		}
		u_int32_t			helloSeqNum_;
};
 
class NDPHelloTimer : public Handler {
	public:
		NDPHelloTimer(NDP* ndp) {
			assert(ndp);
			ndp_ = ndp;
		}
		inline void	handle(Event*) {
			ndp_->sendHello();
			double interval_ = NDP_MIN_HELLO_INTERVAL + ((NDP_MAX_HELLO_INTERVAL - NDP_MIN_HELLO_INTERVAL) * Random::uniform());
			assert(interval_ >= 0);
			Scheduler::instance().schedule(this, &intr, interval_);			
		}
	protected:
		NDP*	ndp_;
		Event	intr;
};

class NDPNeighborTimer : public Handler {
	public:
		NDPNeighborTimer(NDP* ndp) {
			assert(ndp);
			ndp_ = ndp;
		}
		void	handle(Event*) {
			ndp_->neighborPurge();
			Scheduler::instance().schedule(this, &intr, NDP_HELLO_INTERVAL);
		}
	protected:
		NDP* ndp_;
		Event intr;
};

class DHT_Entry {
	public:
		DHT_Entry(nsaddr_t id, nsaddr_t address) {
			id_ = id;
			address_ = address;
			expire_ = CURRENT_TIME + ASPU_DHT_ENTRY_TIMEOUT;
		}
		inline nsaddr_t id() const {
			return id_;
		}
		inline nsaddr_t address() const {
			return address_;
		}
		inline void	address(nsaddr_t address) {
			address_ = address;
		}
		inline double	expire() const {
			return expire_;
		}
		inline void expire(double expire) {
			expire_ = expire;
		}
private:
		nsaddr_t		id_;
		nsaddr_t		address_;
		double		expire_;
};

typedef std::set<DHT_Entry*> dhtTable;
class DHT;
class DHTTimer : public Handler {
public:
	DHTTimer(DHT* dht);
	void	handle(Event*);
protected:
	DHT*			dht_;
	Event			intr;
	u_int32_t	counter;
};

class DHT {
public:
	DHT();
	void		addEntry(nsaddr_t, nsaddr_t);
	void		addEntry(nsaddr_t, nsaddr_t, double);
	void		purge();
	DHT_Entry*	getEntry(u_int32_t);
	nsaddr_t	findAdd(nsaddr_t);
	nsaddr_t	findId(nsaddr_t);
	u_int32_t	DHTSize();
	PacketData*	getDHTUpdate();
	void		printDHT();
private:
	DHT_Entry*	getEntry(nsaddr_t);
	void		removeEntry(nsaddr_t);
	void		clear();
	DHTTimer*	timer_;
	dhtTable*	table_;
};

class ASPU;
class ADPDaupTimer;
class ADPDarqTimer;

class ADP {
public:
	ADP(ASPU* aspu);
	void					updateAdd();
	void					checkDarq();
	void					sendDarq(nsaddr_t, int);
	void					recvDarq(Packet*);
	void					sendDarp(Packet*);
	void					recvDarp(Packet*);
	void					sendDaup();
	void					recvDaup(Packet*);
	void					sendDabr();
	void					recvDabr(Packet*);
	void					addEntry(nsaddr_t id, nsaddr_t address);
	inline uint32_t			DHTSize() {
		return dht_->DHTSize();
	}
	inline void				printDHT() {
		return dht_->printDHT();
	}
	inline nsaddr_t			findAdd(nsaddr_t id) {
		return dht_->findAdd(id);
	}
	inline DHT_Entry*		getEntry(u_int32_t pos) {
		return dht_->getEntry(pos);
	}
	inline nsaddr_t			findId(nsaddr_t add) {
		return dht_->findId(add);
	}
private:
	inline u_int32_t	pckSeqNum() {
		return pckSeqNum_++;
	}
	ASPU*		aspu_;
	DHT*		dht_;
	ADPDaupTimer*	timer_;
	ADPDarqTimer*	timerRx_;
	u_int32_t	pckSeqNum_;
};

class ADPDaupTimer : public Handler {
public:
	ADPDaupTimer(ADP* adp) {
		adp_ = adp;
		counter = 0;
		handle((Event*) 0);
	}
	void	handle(Event*) {
		if (counter > 0) {
			adp_->checkDarq();
		}
		else {
			counter++;
		}
		Scheduler::instance().schedule(this, &intr, ADP_DARQ_CHECK_FREQ);
	}
protected:
	ADP*		adp_;
	Event		intr;
	uint32_t	counter;
};

class ADPDarqTimer : public Handler {
public:
	ADPDarqTimer(ADP* adp) {
		adp_ = adp;
		counter = 0;
		handle((Event*) 0);
	}
	void	handle(Event*) {
		if (counter > 0) {
			adp_->checkDarq();
		}
		else {
			counter++;
		}
		Scheduler::instance().schedule(this, &intr, ADP_DARQ_CHECK_FREQ);
	}
protected:
	ADP*		adp_;
	Event		intr;
	uint32_t	counter;
};

typedef std::vector<Packet*> queue;
class ASPUQueue;
class AspuQueueTimer : public Handler {
public:
	AspuQueueTimer(aspuQueue* dht);
	void	handle(Event*);
protected:
	aspuQueue*	queue_;
	Event		intr;
	u_int32_t	counter;
};

class AspuQueue : public Connector {
	public:
		ASPUQueue();
		~ASPUQueue();
		inline void	recv(Packet *, Handler*) {
			abort();
		}
		inline int	command(int argc, const char * const* argv) {
			return Connector::command(argc, argv);
		}
		void		enque(Packet *p);
		Packet*		deque();
		bool		isExpired(int);
		Packet*		getPacket(int);
		inline int	size() {
			return (int) queue_->size();
		}
		void		purge();
		void		printNumPacket(nsaddr_t dst);
		void		printQueue();
	private:
		void		removeHead();
		void		clear();
		ASPUQueueTimer*	timer_;
		queue*		queue_;
};

class NeighborTable {
public:
	NeighborTable() {
		int i;
		for (i=0; i<ADDR_SIZE; i++) {
			hopNumber_[i] = INFINITO;
			etxMetric_[i] = RTR_ETX_MAX;
			interface_[i] = 255;
		}
	}

	void update(string);
	inline int32_t insertionPoint() const {
		int i;
		for (i=ADDR_SIZE-1; i>-1; i--) {
			if (hopNumber_[i] == INFINITO) {
				return i;
			}	
		}
		return i;
	}
	inline nsaddr_t levelId(u_int32_t levelSibling, nsaddr_t id) const {
		int i;
		for (i=levelSibling; i>-1; i--) {
			if (hopNumber_[i] < INFINITO && networkId_[i] < id) {
				id = networkId_[i];
			}
		}
		return id;
	}
	inline nsaddr_t networkId(u_int32_t levelSibling) const {
		nsaddr_t networkId =  INFINITO;
		if (hopNumber_[levelSibling] < INFINITO && networkId_[levelSibling] < networkId) {
			networkId = networkId_[levelSibling];
		}
		return networkId;
	}
	inline nsaddr_t hopNumber(u_int32_t levelSibling) const {
		u_int32_t hopNumber = INFINITO;
		if (hopNumber_[levelSibling] < hopNumber) {
			hopNumber = hopNumber_[levelSibling];
		}
		return hopNumber;
	}
	inline nsaddr_t interface(u_int8_t levelSibling) const {
		u_int8_t interface = 255;
		if (interface_[levelSibling] < numinterf) {
			interface = interface_[levelSibling];
		}
		return interface;
	}
	inline double etxMetric(u_int32_t levelSibling) const {
		double etxMetric = RTR_ETX_MAX;
		if (etxMetric_[levelSibling] < etxMetric) {
			etxMetric = etxMetric_[levelSibling];
		}
		return etxMetric;
	}
	inline bitset<ADDR_SIZE> routeLog(u_int32_t levelSibling) const {
		bitset<ADDR_SIZE> routeLog;
		routeLog.reset();
		if (hopNumber_[levelSibling] < INFINITO) {
			routeLog = routeLog_[levelSibling];
		}
		return routeLog;
	}
	inline u_int32_t routeLog(u_int32_t levelSibling, u_int32_t j) const {
		bitset<ADDR_SIZE> routeLog;
		routeLog.reset();
		if (hopNumber_[levelSibling] < INFINITO) {
			routeLog = routeLog_[levelSibling];
		}
		return (u_int32_t) routeLog[j];
	}
	inline bool entryPresent(u_int32_t levelSibling_) const {
		return (hopNumber_[levelSibling_] < INFINITO);
	}
	void print(nsaddr_t address_) const;
	void purgeHello();
	void printHello();
	void NTable();
private:
	nsaddr_t			networkId_[ADDR_SIZE];
	u_int32_t			hopNumber_[ADDR_SIZE];
	double				etxMetric_[ADDR_SIZE];
	u_int8_t			interface_[ADDR_SIZE];
	bitset<ADDR_SIZE>	routeLog_[ADDR_SIZE];
}; 

struct hello {
	double		time_;
	u_int32_t	seqNum_;
};
typedef std::vector<hello> helloVector;

class Neighbor {
	public:
		Neighbor(nsaddr_t id, nsaddr_t address, u_int32_t helloSeqNum);
		void purgeHello();
		void printHello();
		inline double etxMetric() {
			double etxMetric_ = RTR_ETX_MAX;
			if ((revLinkQuality() * forLinkQuality_) != 0)
				etxMetric_ = 1/(revLinkQuality() * forLinkQuality_);
			if (etxMetric_ > RTR_ETX_MAX)
				etxMetric_ = RTR_ETX_MAX;
			return etxMetric_;
		}
		inline double linkQuality() {
			return revLinkQuality() * forLinkQuality_;
		}
		inline double revLinkQuality() {
			double revLinkQuality_ = 0;
			helloVector::iterator entry_;
			for(entry_ = helloVector_.begin(); entry_ != helloVector_.end(); ++entry_) {
				revLinkQuality_++;
			}
			if (revLinkQuality_/LQE_MA_EXPECTED_HELLO > 1.0)
				return 1.0;
			return revLinkQuality_/LQE_MA_EXPECTED_HELLO;
		}
		inline void forLinkQuality(double quality) {
			forLinkQuality_ = quality;
#ifdef DEBUG_NEIGHBOR
			fprintf(stdout, "\t\tforLinkQuality_ = %f\n",forLinkQuality_);
#endif
		}
		inline nsaddr_t id() const {
			return id_;
		}
		inline nsaddr_t address() const {
			return address_;
		}
		inline void address(nsaddr_t address, u_int32_t helloSeqNum) {
			address_ = address;
			expire_ = CURRENT_TIME + NDP_NEIGHBOR_EXPIRE;
			purgeHello();
			addHello(helloSeqNum);
		}
		inline double expire() const {
			return expire_;
		}
		inline void addHello(u_int32_t seqNum) {
#ifdef DEBUG_NEIGHBOR
			fprintf(stdout, "%.9f\tNeighbor::addHello(%d)\n", CURRENT_TIME, seqNum);
			printHello();
#endif
			hello hello_;
			hello_.time_ = CURRENT_TIME;
			hello_.seqNum_ = seqNum;
			helloVector_.push_back(hello_);
#ifdef DEBUG_NEIGHBOR
			printHello();
			fprintf(stdout, "\treverse link quality = %f\n", revLinkQuality());
#endif
		}
		inline void updateTable(string str) {
			table_->update(str);
		}
		inline int32_t insertionPoint() const {
			return table_->insertionPoint();
		}
		inline nsaddr_t levelId(u_int32_t levelSibling) const {
			return table_->levelId(levelSibling, id_);
		}
		inline nsaddr_t networkId(u_int32_t levelSibling) const {
			return table_->networkId(levelSibling);
		}
		inline u_int32_t hopNumber(u_int32_t levelSibling) const {
			return table_->hopNumber(levelSibling);
		}
		inline double etxMetric(u_int32_t levelSibling) const {
			return table_->etxMetric(levelSibling);
		}
		inline bitset<ADDR_SIZE> routeLog(u_int32_t levelSibling) const {
			return table_->routeLog(levelSibling);
		}
		inline u_int32_t routeLog(u_int32_t levelSibling, u_int32_t j) const {
			return table_->routeLog(levelSibling, j);
		}
		inline bool entryPresent(u_int32_t levelSibling_) const {
			return table_->entryPresent(levelSibling_);
		}
		inline void	 printTable() const {
			table_->print(address_);
		}
private:
	double			forLinkQuality_;
	nsaddr_t		address_;
	nsaddr_t		id_;
	double			expire_;
	NeighborTable*	table_;
	helloVector		helloVector_;
};
#endif 