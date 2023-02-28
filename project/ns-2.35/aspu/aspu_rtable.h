
#ifndef __aspu_table__
#define __aspu_table__
#include <aspu/aspu_function.h>
#include <aspu/aspu.h>

class RoutingEntry {
	public:
		RoutingEntry(nsaddr_t nextHopAdd, nsaddr_t nextHopId, nsaddr_t networkId, u_int32_t hopNumber, double etxMetric,
		bitset<ADDR_SIZE> routeLog, u_int8_t interface) {
			nextHopId_ = nextHopId;
			nextHopAdd_ = nextHopAdd;
			networkId_ = networkId;
			hopNumber_ = hopNumber;
			etxMetric_ = etxMetric;
			macFailed_ = false;
			routeLog_ = routeLog;
			interface_ = interface;
		}
		inline nsaddr_t nextHopId() const {
			return nextHopId_;
		}
		inline nsaddr_t nextHopAdd() const {
			return nextHopAdd_;
		}
		inline nsaddr_t networkId() const {
			return networkId_;
		}
		inline u_int32_t hopNumber() const {
			return hopNumber_;
		}
		inline double etxMetric() const {
			return etxMetric_;
		}
		inline void upLinkQuality() {
			etxMetric_++;
		}
		inline bitset<ADDR_SIZE> routeLog() const {
			return routeLog_;
		}
		inline bool macFailed() const {
			return macFailed_;
		}
		inline void macFailed(bool macFailed) {
			macFailed_ = macFailed;
		}
		inline u_int8_t interface() const {
			return interface_;
		}
	private:
		nsaddr_t			nextHopId_;
		nsaddr_t			nextHopAdd_;
		nsaddr_t			networkId_;
		u_int32_t			hopNumber_;
		double				etxMetric_;
		bool				macFailed_;
		bitset<ADDR_SIZE>	routeLog_;
		u_int8_t 			interface_;
};

struct setCompare {
	bool operator() (const RoutingEntry* entry1, const RoutingEntry* entry2) const {
		bitset<ADDR_SIZE> n1 (entry1->nextHopAdd());
		bitset<ADDR_SIZE> n2 (entry2->nextHopAdd());
		int i;
		for(i=ADDR_SIZE-1; i>-1; i--) {
			if (n1.test(i) < n2.test(i)) {
				return 1;
			}
			if (n1.test(i) > n2.test(i)) {
				return 0;
			}
		}
		return 0;
	}
};

typedef std::set<RoutingEntry *, setCompare> entrySet;

class RoutingTable {
	public:
		RoutingTable(ASPU* aspu) {
			aspu_ = aspu;
			table_ = new std::vector<entrySet*>;
			int i;
			for(i=0; i<ADDR_SIZE; i++) {
				entrySet* entrySet_ = new entrySet;
				table_->push_back(entrySet_);
			}
		}		 
		void				clear();
		void				clear(u_int32_t);
		void				purge(nsaddr_t id);
		string				getUpdate();
		void				setUpdate(Neighbor* neighbor_);
		void				macFailed(nsaddr_t nextHop);		 
		nsaddr_t			levelId(u_int32_t);
		nsaddr_t			id(u_int32_t);
		nsaddr_t			networkId(u_int32_t);
		u_int32_t			hopNumber(u_int32_t);
		double				etxMetric(u_int32_t);
		bitset<ADDR_SIZE>	routeLog(u_int32_t);
		u_int8_t			interface(u_int8_t);
		void				addEntry(int, nsaddr_t, nsaddr_t, nsaddr_t, u_int32_t, double, bitset<ADDR_SIZE>);
		nsaddr_t			getEntry(nsaddr_t);
		nsaddr_t			DAGetEntry(nsaddr_t);
		inline entrySet*	getSibling(u_int32_t levelSibling) const {
			return table_->at(levelSibling);
		}		 
		void				print();
		u_int32_t			size();
	private:
		ASPU* aspu_;
		std::vector<entrySet*>* table_;
};

#endif