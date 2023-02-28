#ifndef __aspu_function__
#define __aspu_function__
#include <random.h>
#include <packet.h>
#include <agent.h>
#include <trace.h>
#include <timer-handler.h>
#include <classifier-port.h>
#include <cmu-trace.h>
#include <bitset>
#include <string>
#include <sstream>
#include <set>
#include <vector>
#include <aspu/aspu_packet.h>
#define MAXINTERF 3
#define MAC_FAILED_ALLOWED_LOSS		5
#define ADDR_SIZE						16
#define INFINITO						0xFFFF
#define CURRENT_TIME					Scheduler::instance().clock()
#define JITTER							(Random::uniform() * 0.5)
#define MESH_SIZE						2
#define NDP_MEMORY					5
#define NDP_WEIGTH					0.5
#define NDP_HELLO_INTERVAL			1.0
#define NDP_MAX_HELLO_INTERVAL	(1.25 * NDP_HELLO_INTERVAL)
#define NDP_MIN_HELLO_INTERVAL	(0.75 * NDP_HELLO_INTERVAL)
#define NDP_ALLOWED_HELLO_LOSS	3
#define RTR_ETX_MAX					512
#define LQE_THRESHOLD				0.6
#define LQE_RV_THRESHOLD			0.4
#define LQE_MA_EXPECTED_HELLO		10.0
#define NDP_NEIGHBOR_EXPIRE		10.5
#define ASPU_QUEUE_LEN				64
#define ASPU_QUEUE_TIMEOUT			30
#define ASPU_QUEUE_PURGE_INTERVAL	1.0
#define ASPU_DARQ_RETRANSMIT	5
#define ADP_DAUP_FREQ				5.0
#define ADP_DAUP_TIMEOUT			5.0
#define ADP_DARQ_TIMEOUT			5.0
#define ADP_DARQ_CHECK_FREQ			0.1
#define ADP_DARP_TIMEOUT			5.0
#define ADP_DARQ_EB
#define ADP_DARQ_EB_TIMEOUT			5.0
#define ADP_DABR_ENABLE
#define ADP_HELLO_CACHING
#define ADP_DAXX_CACHING
#define ADP_DATA_CACHING
#define ASPU_DHT_PURGE_INTERVAL		5.0
#define ASPU_DHT_ENTRY_TIMEOUT		10 * ADP_DAUP_FREQ

inline const char* bitString(nsaddr_t add) {
	bitset<ADDR_SIZE> tempSet (add);
	string temp = tempSet.to_string();
	return temp.c_str();
}

inline int DiffBit(nsaddr_t n1, nsaddr_t n2) {
	bitset<ADDR_SIZE> tempN1 (n1);
	bitset<ADDR_SIZE> tempN2 (n2);
	int i;
	for(i=ADDR_SIZE-1; i>-1; i--) {
		if (tempN1.test(i) != tempN2.test(i)) {
			break;
		}
	}
	return i;
}

inline nsaddr_t hash(nsaddr_t id) {
	bitset<ADDR_SIZE> tempAddress_ (id);
	bitset<ADDR_SIZE> address_;
	for(unsigned int i=0; i<ADDR_SIZE; i++) {
		if (tempAddress_.test(i)) {
			address_.set(ADDR_SIZE - 1 - i);
		}
	}
	address_.flip(ADDR_SIZE - 1);
	nsaddr_t temp = (nsaddr_t) address_.to_ulong();
#ifdef DEBUG
	fprintf(stdout, "\thash = %s\n", bitString(temp));
#endif
	return temp;
}

#endif
