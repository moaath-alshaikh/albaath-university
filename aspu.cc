#include <aspu/aspu.h>
#include <aspu/aspu_function.h>

int hdr_aspu::offset_;
static class ASPUHeaderClass : public PacketHeaderClass {
	public:
		ASPUHeaderClass() : PacketHeaderClass("PacketHeader/ASPU", sizeof(hdr_all_aspu)) {
			bind_offset (&hdr_aspu::offset_);
		}
} class_rtProtoASPU_hdr;

static class ASPUClass : public TclClass {
	public:
		ASPUClass() : TclClass("Agent/ASPU") {
		}
		TclObject* create(int argc, const char* const* argv) {
			assert(argc == 5);
			ASPU* aspu_ = new Aspu((nsaddr_t)Address::instance().str2addr(argv[4]));
			assert(aspu_);
			return (aspu_);
		}
} class_rtProtoASPU;

inline static void macFailedCallback(Packet *recvPkt, void *arg) {
	((Aspu*)arg)->macFailed(recvPkt);
}

void ASPUTimer::handl(Event*) {
	double interval_ = NDP_MIN_HELLO_INTERVAL + ((NDP_MAX_HELLO_INTERVAL - NDP_MIN_HELLO_INTERVAL) * Random::uniform());
	switch(numberOfCall_) {
		case 0:
			numberOfCall_++;
			interval_ = Random::uniform();
			Scheduler::instance().schedule(this, &intr, interval_);
			break;
		case 1:
			numberOfCall_++;

			aspu_->ndp_->neighborPurge();
			aspu_->validateAddress();
			aspu_->ndp_->updateRoutingTable();
			aspu_->ndp_->startHelloTimer();
			Scheduler::instance().schedule(this, &intr, interval_);
			break;
		default:
			aspu_->ndp_->neighborPurge();
			aspu_->validateAddress();
			aspu_->ndp_->updateRoutingTable();
			Scheduler::instance().schedule(this, &intr, interval_);
	}
}

ASPU::ASPU(nsaddr_t id) : Agent(PT_ASPU) , pkt_timer_(this){
    bind_bool("accesible_var_", &accesible_var_);
	bind_bool("macFailed_", &macFailed_);
	bind_bool("etxMetric_", &etxMetric_);
	id_ = id;
	bitset<ADDR_SIZE> bitAddress_ (0);
	address_ = (unsigned int) bitAddress_.to_ulong();
	aspuTimer_ = new ASPUTimer(this);
	ndp_ = new NDP(this);
	adp_ = new ADP(this);
	routingTable_ = new RoutingTable(this);
	queue_ = new ASPUQueue();
	assert(aspuTimer_);
	assert(ndp_);
	assert(routingTable_);
	numinterf = 0;
}

ASPU::~ASPU() {
#ifdef DEBUG
	fprintf(stdout, "%.9f\tASPU::~ASPU()\t\t\t\tin node %d\twith address %s\n", CURRENT_TIME, id_, bitString(address_));
#endif
	delete aspuTimer_;
	delete ndp_;
}
 
int ASPU::command(int argc, const char* const* argv) {
#ifdef DEBUG
			fprintf(stdout, "%.9f\tASPU::command()\t\t\tin node %d\twith address %s\n", CURRENT_TIME, id_, bitString(address_));
#endif
	if (argc == 2) {
		if (strcasecmp(argv[1], "start") == 0) {
			startTimer();
			return TCL_OK;
		}
		else if (strcasecmp(argv[1], "tableSize") == 0) {
			if (logtarget_ != 0) {
				sprintf(logtarget_->pt_->buffer(), "x -t %.9f aspu table -Hs %d -Ds %s -Rt %d", CURRENT_TIME, id_, bitString(address_), routingTable_->size());
				logtarget_->pt_->dump();
			}
			else {
#ifdef DEBUG
				fprintf(stdout, "%f_ %d_ If you want to print the routing table size you must create a trace file in your tcl script", CURRENT_TIME, address_);
#endif
			}
			return TCL_OK;
		}
		else if (strcasecmp(argv[1], "neighborDegree") == 0) {
			if (logtarget_ != 0) {
				sprintf(logtarget_->pt_->buffer(), "x -t %.9f aspu neighbor -Hs %d -Ds %s -Nd %d", CURRENT_TIME, id_, bitString(address_), ndp_->neighborDegree());
				logtarget_->pt_->dump();
			}
			else {
#ifdef DEBUG
				fprintf(stdout, "%f_ %d_ If you want to print the neighbor degree you must create a trace file in your tcl script", CURRENT_TIME, address_);
#endif
			}
			return TCL_OK;
		}
		else if (strcasecmp(argv[1], "realNeighborDegree") == 0) {
			if (logtarget_ != 0) {
				sprintf(logtarget_->pt_->buffer(), "x -t %.9f aspu realNeighbor -Hs %d -Ds %s -Nd %d", CURRENT_TIME, id_, bitString(address_), ndp_->realNeighborDegree());
				logtarget_->pt_->dump();
			}
			else {
#ifdef DEBUG
				fprintf(stdout, "%f_ %d_ If you want to print the real neighbor degree you must create a trace file in your tcl script", CURRENT_TIME, address_);
#endif
			}
			return TCL_OK;
		}

	else if (strcasecmp(argv[1], "dhtTablePrint") == 0) {
			if (logtarget_ != 0) {
				sprintf(logtarget_->pt_->buffer(), "x -t %.9f aspu dhtTable -Hs %d -Ds %s", CURRENT_TIME, id_, bitString(address_));
				logtarget_->pt_->dump();
				unsigned int i;
				for(i=0; i<adp_->DHTSize(); i++) {
					sprintf(logtarget_->pt_->buffer(), "\tid\t%d\taddress\t%s", adp_->getEntry(i)->id(), bitString(adp_->getEntry(i)->address()));
					logtarget_->pt_->dump();
				}
			}
			else {
				fprintf(stdout, "%f_ %d_ If you want to print the lookup table you must create a trace file in your tcl script", CURRENT_TIME, address_);
			}
			return TCL_OK;
		}

		else if (strcasecmp(argv[1], "routingTablePrint") == 0) {
			if (logtarget_ != 0) {
				sprintf(logtarget_->pt_->buffer(), "x -t %.9f aspu routingTable -Hs %d -Ds %s", CURRENT_TIME, id_, bitString(address_));
				logtarget_->pt_->dump();
				unsigned int i;
				for (i=0; i<ADDR_SIZE; i++) {
					bitset<ADDR_SIZE> tempSibling_ (address_);
					tempSibling_.flip(i);
					sprintf(logtarget_->pt_->buffer(), "\tlevel sibling = %i\tsibling = %s", i, tempSibling_.to_string().c_str());
					logtarget_->pt_->dump();
					entrySet::iterator itEntry_;
					for (itEntry_ = routingTable_->getSibling(i)->begin(); itEntry_ != routingTable_->getSibling(i)->end(); ++itEntry_) {
						sprintf(logtarget_->pt_->buffer(), "\t\tnextHopId = %d\tnextHopAdd = %s\thopNumber = %d\tetxMetric = %f", (*itEntry_)->nextHopId(), bitString((*itEntry_)->nextHopAdd()), (*itEntry_)->hopNumber(), (*itEntry_)->etxMetric());
						logtarget_->pt_->dump();
					}
				}
			}
			else {
#ifdef DEBUG
				fprintf(stdout, "%f_ %d_ If you want to print the lookup table you must create a trace file in your tcl script", CURRENT_TIME, address_);
#endif
			}
			return TCL_OK;
		}
		else if (strcasecmp(argv[1], "neighborPrint") == 0) {
			if (logtarget_ != 0) {
				sprintf(logtarget_->pt_->buffer(), "x -t %.9f aspu neighborPrint -Hs %d -Ds %s", CURRENT_TIME, id_, bitString(address_));
				logtarget_->pt_->dump();
				neighborSet::iterator neighbor_;
				for (neighbor_ = ndp_->neighborSet_->begin(); neighbor_ != ndp_->neighborSet_->end(); ++neighbor_) {
					sprintf(logtarget_->pt_->buffer(), "\tneighbor id = %d\taddress = %s", (*neighbor_)->id(), bitString((*neighbor_)->address()));
					logtarget_->pt_->dump();
				}
			}
			else {
#ifdef DEBUG
				fprintf(stdout, "%f_ %d_ If you want to print the lookup table you must create a trace file in your tcl script", CURRENT_TIME, address_);
#endif
			}
			return TCL_OK;
		}
	}
	else if (argc == 3) {
		if (strcmp(argv[1], "port-dmux") == 0) {
			dmux_ = (PortClassifier*)TclObject::lookup(argv[2]);
			if (dmux_ == 0) {
				fprintf(stderr, "%s: %s lookup of %s failed\n", __FILE__, argv[1], argv[2]);
				return TCL_ERROR;
			}
			return TCL_OK;
		}
		else if (strcmp(argv[1], "log-target") == 0 || strcmp(argv[1], "tracetarget") == 0) {
			logtarget_ = (Trace*)TclObject::lookup(argv[2]);
			if (logtarget_ == 0) {
				return TCL_ERROR;
			}
			return TCL_OK;
		}
		else if(strcmp(argv[1], "drop-target") == 0) {
			int stat = queue_->command(argc,argv);
			if (stat != TCL_OK) {
				return stat;
			}
			return Agent::command(argc, argv);
		}
	}
	else if (argc == 4) {
		if (strcmp(argv[1], "if-queue") == 0) {
			ASPUQueue *ifq = (ASPUQueue*)TclObject::lookup(argv[3]);
			int temp_ = atoi(argv[2]);
			if (temp_ == numinterf) {
				numinterf++;
			}
			ifqueuelist[temp_] = ifq;
			if (ifqueuelist[temp_]) {
				return TCL_OK;
			} else {
				return TCL_ERROR;
			}
		}
		if (strcmp(argv[1], "target") == 0) {
			int temp_ = atoi(argv[2]);
			if (temp_ == numinterf) {
				numinterf++;
			}
			targetlist[temp_] = (NsObject*) TCLObject::lookup(argv[3]);
			if (targetlist[temp_]) {
			return TCL_OK;
			} else {
				return TCL_ERROR;
			}	
		}
	}
	
	return Agent::command(argc, argv);
}

void ASPU::recv(Packet* recvPkt_, Handler* h) {
#ifdef DEBUG
	fprintf(stdout, "%.9f\tASPU::recv()\t\t\t\tin node %d\twith address %s\n", CURRENT_TIME, id_, bitString(address_));
#endif
	struct hdr_cmn* ch = HDR_CMN(recvPkt_);
	struct hdr_ip* ih = HDR_IP(recvPkt_);
	if (ih->saddr() == id_) {
		if (ch->num_forwards() == 0) {
			
			ch->size() += IP_HDR_LEN;
		}
		else {
			if (ch->ptype() != PT_ASPU) {
#ifdef DEBUG_PACKET_FORWARDING
				fprintf(stdout, "%.9f\tASPU::recv()\t\t\t\tin node %d\twith address %s\n", CURRENT_TIME, id_, bitString(address_));
				fprintf(stdout, "DROP_RTR_ROUTE_LOOP\n");
#endif
				drop(recvPkt_, DROP_RTR_ROUTE_LOOP);
				return;			 
			}
		}
	}	
	else {
		if (--ih->ttl_ == 0) {
#ifdef DEBUG_PACKET_FORWARDING
			fprintf(stdout, "%.9f\tASPU::recv()\t\t\t\tin node %d\twith address %s\n", CURRENT_TIME, id_, bitString(address_));
			fprintf(stdout, "DROP_RTR_TTL\n");
#endif
			drop(recvPkt_, DROP_RTR_TTL);
			return;
		}
	}
	if (ch->ptype() == PT_ASPU) {
		recvASPU(recvPkt_);
		return;
	}
	else {		
		if (ch->num_forwards() == 0) {
			format(recvPkt_);
		}
		else {
			forward(recvPkt_);
			return;
		}
	}
}

void ASPU::macFailed(Packet* recvPkt_) {
#ifdef DEBUG_PACKET_FORWARDING
	struct hdr_cmn* recvPktCh_ = HDR_CMN(recvPkt_);
	struct hdr_ip *recvPktIh_ = HDR_IP(recvPkt_);
	struct hdr_aspu_encp *recvPktRh_ = HDR_ASPU_ENCP(recvPkt_);
	fprintf(stdout, "%.9f\tASPU::macFailed())\t\t\t\tin node %d\twith address %s\n", CURRENT_TIME, id_, bitString(address_));
	fprintf(stdout, "\trecvPktCh_->next_hop = %d\n", recvPktCh_->next_hop());
	fprintf(stdout, "\trecvPktIh_->saddr = %d\n", recvPktIh_->saddr());
	fprintf(stdout, "\trecvPktIh_->daddr = %d\n", recvPktIh_->daddr());
	fprintf(stdout, "\trecvPktRh_->srcId_ = %d\n", recvPktRh_->srcId_);
	fprintf(stdout, "\trecvPktRh_->srcAdd_ = %s\n", bitString(recvPktRh_->srcAdd_));
	fprintf(stdout, "\trecvPktRh_->dstId_ = %d\n", recvPktRh_->dstId_);
	fprintf(stdout, "\trecvPktRh_->dstAdd_ = %s\n", bitString(recvPktRh_->dstAdd_));
	fprintf(stdout, "\trecvPktRh_->txCount_ = %d\n", recvPktRh_->txCount_);
	fprintf(stdout, "\trecvPktCh_->xmit_reason_ = %d\n", recvPktCh_->xmit_reason_);
	routingTable_->print();	
#endif
	if (macFailed_ == 1) {

#ifdef DEBUG_PACKET_FORWARDING
		routingTable_->print();
#endif 
		struct hdr_cmn* recvPktCh_ = HDR_CMN(recvPkt_);
		struct hdr_aspu_encp *recvPktRh_ = HDR_ASPU_ENCP(recvPkt_);
		routingTable_->macFailed(recvPktCh_->next_hop());
		if (recvPktRh_->txCount_++ < MAC_FAILED_ALLOWED_LOSS)
			forward(recvPkt_);
		else
			drop(recvPkt_, "DROP_RTR_TX_COUNT");
	} else {

		drop(recvPkt_, DROP_RTR_MAC_CALLBACK);
	}

}
 
void ASPU::format(Packet* sendPkt_) {
#ifdef DEBUG_ADP
	fprintf(stdout, "%.9f\tASPU::format()\t\t\t\tin node %d\twith address %s\n", CURRENT_TIME, id_, bitString(address_));
#endif
	struct hdr_cmn* sendPktCh_ = HDR_CMN(sendPkt_);
	struct hdr_ip* sendPktIh_ = HDR_IP(sendPkt_);
	struct hdr_aspu_encp* sendPktRh_ = HDR_ASPU_ENCP(sendPkt_);
	nsaddr_t dstAdd_ = (nsaddr_t) adp_->findAdd(sendPktIh_->daddr());
	sendPktRh_->dstAdd_ = dstAdd_;
	sendPktRh_->dstId_ = sendPktIh_->daddr();
	sendPktRh_->srcAdd_ = address_;
	sendPktRh_->srcId_ = id_;
#ifdef DEBUG_ADP
	fprintf(stdout, "\tsendPktRh_->dstId_ = %d\n", sendPktRh_->dstId_);
	fprintf(stdout, "\tsendPktRh_->srcAdd_ = %s\n", bitString(sendPktRh_->srcAdd_));
	fprintf(stdout, "\tsendPktRh_->dstAdd_ = %s\n", bitString(sendPktRh_->dstAdd_));
	fprintf(stdout, "\tsendPktRh_->srcId_ = %d\n", sendPktRh_->srcId_);
#endif
	if (dstAdd_ == (nsaddr_t) IP_BROADCAST) {
#ifdef DEBUG_PACKET_FORWARDING
		fprintf(stdout, "\tDestination dynamic address unknown for node %d\n", sendPktIh_->daddr());
#endif
		queue_->enque(sendPkt_);
		adp_->sendDarq(sendPktIh_->daddr(), sendPktCh_->uid());
		return;
	}
#ifdef DEBUG_PACKET_FORWARDING
	fprintf(stdout, "\tDestination dynamic address %s for node %d\n", bitString(dstAdd_), sendPktIh_->daddr());
#endif
	forward(sendPkt_);
}

void ASPU::forward(Packet* sendPkt_) {
#ifdef DEBUG_PACKET_FORWARDING
	fprintf(stdout, "%.9f\tASPU::forward()\t\t\t\tin node %d\twith address %s\n", CURRENT_TIME, id_, bitString(address_));
#endif
	struct hdr_cmn* sendPktCh_ = HDR_CMN(sendPkt_);
	struct hdr_ip* sendPktIh_ = HDR_IP(sendPkt_);
	struct hdr_aspu_encp* sendPktRh_ = HDR_ASPU_ENCP(sendPkt_);
#ifdef DEBUG_PACKET_FORWARDING
	fprintf(stdout, "\tsendPktIh_->saddr = %d\n", sendPktIh_->saddr());
	fprintf(stdout, "\tsendPktIh_->daddr = %d\n", sendPktIh_->daddr());
	fprintf(stdout, "\tsendPktRh_->srcAdd_ = %s\n", bitString(sendPktRh_->srcAdd_));
	fprintf(stdout, "\tsendPktRh_->dstAdd_ = %s\n", bitString(sendPktRh_->dstAdd_));
	fprintf(stdout, "\tsendPktRh_->srcId_ = %d\n", sendPktRh_->srcId_);
	fprintf(stdout, "\tsendPktRh_->dstId_ = %d\n", sendPktRh_->dstId_);
	adp_->printDHT();
	routingTable_->print();
#endif
	if (sendPktCh_->direction() == hdr_cmn::UP && ((u_int32_t)sendPktIh_->daddr() == IP_BROADCAST || sendPktIh_->daddr() == id_)) {
#ifdef DEBUG_PACKET_FORWARDING
		fprintf(stdout, "%.9f\tASPU::forward()\t\t\t\tin node %d\twith address %s\n", CURRENT_TIME, id_, bitString(address_));
		fprintf(stdout, "!!!\tdmux\n");
#endif
		dmux_->recv(sendPkt_, (Handler *) 0);
		return;
	}
	sendPktCh_->direction() = hdr_cmn::DOWN;
	sendPktCh_->addr_type() = NS_AF_INET;
	if ((u_int32_t)sendPktIh_->daddr() == IP_BROADCAST) {
#ifdef DEBUG_PACKET_FORWARDING
		fprintf(stdout, "%.9f\tASPU::forward()\t\t\t\tin node %d\twith address %s\n", CURRENT_TIME, id_, bitString(address_));
		fprintf(stdout, "\tpacket dropped: DROP_IP_BROADCAST\n");
		adp_->printDHT();
		routingTable_->print();
#endif
		drop(sendPkt_, "DROP_IP_BROADCAST");
		return;
	}
#ifdef ADP_DATA_CACHING
	adp_->addEntry(sendPktRh_->dstId_, sendPktRh_->dstAdd_);
	adp_->addEntry(sendPktRh_->srcId_, sendPktRh_->srcAdd_);
	adp_->addEntry(sendPktRh_->forId_, sendPktRh_->forAdd_);
#endif
#ifdef DEBUG_PACKET_FORWARDING
	fprintf(stdout, "\tADP_DHT::findAdd(%d) = %s\n", sendPktIh_->daddr(), bitString(sendPktRh_->dstAdd_));
#endif
	if (sendPktRh_->dstAdd_ == address_) {
#ifdef DEBUG_PACKET_FORWARDING
		fprintf(stdout, "%.9f\tASPU::forward()\t\t\t\tin node %d\twith address %s\n", CURRENT_TIME, id_, bitString(address_));
		fprintf(stdout, "\tpacket dropped: DROP_RTR_DADD\n");
		adp_->printDHT();
		routingTable_->print();
#endif
		drop(sendPkt_, "DROP_RTR_DADD");
		return;
	}
	nsaddr_t nextHop_ = routingTable_->getEntry(sendPktRh_->dstAdd_);
	if ((u_int32_t) nextHop_ == IP_BROADCAST) {
#ifdef DEBUG_PACKET_FORWARDING
		fprintf(stdout, "%.9f\tASPU::forward()\t\t\t\tin node %d\twith address %s\n", CURRENT_TIME, id_, bitString(address_));
		fprintf(stdout, "\tpacket dropped: DROP_RTR_ADPNRT\tDAddress_=%s\tnextHop=%d\n", bitString(sendPktRh_->dstAdd_), nextHop_);
		adp_->printDHT();
		routingTable_->print();
#endif
		drop(sendPkt_, "DROP_RTR_ADPNRT");
		return;
	}
	sendPktCh_->next_hop() = nextHop_;
	sendPktRh_->forAdd_ = address_;
	sendPktRh_->forId_ = id_;
	sendPktCh_->xmit_failure_ = macFailedCallback;
	sendPktCh_->xmit_failure_data_ = (void*)this;
#ifdef DEBUG_PACKET_FORWARDING
	fprintf(stdout, "\tselected nextHop = %d\n", sendPktCh_->next_hop());
	fprintf(stdout, "\tsendPktIh_->saddr = %d\n", sendPktIh_->saddr());
	fprintf(stdout, "\tsendPktIh_->daddr = %d\n", sendPktIh_->daddr());
	fprintf(stdout, "\tsendPktRh_->srcAdd_ = %s\n", bitString(sendPktRh_->srcAdd_));
	fprintf(stdout, "\tsendPktRh_->dstAdd_ = %s\n", bitString(sendPktRh_->dstAdd_));
	fprintf(stdout, "\tsendPktRh_->srcId_ = %d\n", sendPktRh_->srcId_);
#endif
		Scheduler::instance().schedule(target_, sendPkt_, 0.0);
}

void ASPU::recvASPU(Packet* recvPkt_) {
#ifdef DEBUG
	fprintf(stdout, "%.9f\t%s\t\t\tin node %d\twith address %s\n",  CURRENT_TIME, __FUNCTION__, id_, bitString(address_));
#endif
	struct hdr_aspu* recvPktRh_ = HDR_ASPU(recvPkt_);
	switch(recvPktRh_->type_) {
		case ASPU_TYPE_HELLO:
		{
#ifdef ADP_HELLO_CACHING
			struct hdr_aspu_hello* recvPktRhHello_ = HDR_ASPU_HELLO(recvPkt_);
			adp_->addEntry(recvPktRhHello_->srcId_, recvPktRhHello_->srcAdd_);
#endif
			ndp_->recvHello(recvPkt_);
			break;
		}
		case ASPU_TYPE_DARQ:
			adp_->recvDarq(recvPkt_);
			break;
		case ASPU_TYPE_DARP:
			adp_->recvDarp(recvPkt_);
			break;
		case ASPU_TYPE_DAUP:
			adp_->recvDaup(recvPkt_);
			break;
		case ASPU_TYPE_DABR:
			adp_->recvDabr(recvPkt_);
			break;

		default:
#ifdef DEBUG
			fprintf(stderr, "!!!\tinvalid ASPU type (%x)\n", recvPktRh_->type_);
#endif
			exit(1);
	}
}

void	ASPU::validateAddress() {
#ifdef DEBUG_ADDRESS_ALLOCATION
	fprintf(stdout, "%.9f\tASPU::validateAddress()\t\t\tin node %d\twith address %s\n", CURRENT_TIME, id_, bitString(address_));

#endif
	if (!ndp_->validateAddress()) {

    	routingTable_->clear();
		oldAddress_ = address_;
		ndp_->selectAddress();
#ifdef DEBUG_ADDRESS_ALLOCATION		
		fprintf(stdout, "\tNew address is = %s\n", bitString(address_));
		routingTable_->print();
#endif
		if (logtarget_ != 0) {
			sprintf(logtarget_->pt_->buffer(), "x -t %.9f aspu invalidAddress -Hs %d -Ds %s -Rt %d", CURRENT_TIME, id_, bitString(address_), routingTable_->size());
			logtarget_->pt_->dump();
		}
#ifdef FAST_ADDRESS_CONVERGENCE
		ndp_->sendHello();
#endif
	}
	routingTable_->clear();
}

NDP::NDP(aspu* aspu) {
	aspu_ = aspu;
	neighborSet_ = new neighborSet;
	helloTimer_ = new NDPHelloTimer(this);
	neighborTimer_ = new NDPNeighborTimer(this);
	helloSeqNum_ = 0;
}

NDP::~NDP() {
	delete helloTimer_;
	delete neighborTimer_;
	delete neighborSet_;
}

 void NDP::sendHello() {
#ifdef DEBUG_NDP
	fprintf(stdout, "%.9f\tNDP::sendHello()\t\t\tin node %d\twith address %s\n", CURRENT_TIME, aspu_->id_, bitString(aspu_->address_));
#endif
	Packet *sendPkt_ = Packet::alloc();
	struct hdr_cmn *sendPktCh_ = HDR_CMN(sendPkt_);
	struct hdr_ip *sendPktIh_ = HDR_IP(sendPkt_);
	struct hdr_aspu_hello *sendPktRh_ = HDR_aspu_HELLO(sendPkt_);
	sendPktCh_->ptype() = PT_aspu;
	sendPktCh_->size() = IP_HDR_LEN + aspu_->size();
	sendPktCh_->iface() = -2;
	sendPktCh_->error() = 0;
	sendPktCh_->addr_type() = NS_AF_NONE;
	sendPktCh_->next_hop() = IP_BROADCAST;
	sendPktIh_->saddr() = aspu_->id_;
	sendPktIh_->daddr() = IP_BROADCAST;
	sendPktIh_->sport() = RT_PORT;
	sendPktIh_->dport() = RT_PORT;
	sendPktIh_->ttl_ = IP_DEF_TTL;
	sendPktRh_->type_ = aspu_TYPE_HELLO;
	sendPktRh_->dstAdd_ = DATYPE_BROADCAST;
	sendPktRh_->srcAdd_ = aspu_->address_;
	sendPktRh_->srcId_ = aspu_->id_;
	sendPktRh_->seqNum_ = helloSeqNum();
	string string_ = aspu_->routingTable_->getUpdate();
	string_ += "-";
 	if (aspu_->etxMetric_ == 1) {
		string_ += getNeighborQuality();
	}
	PacketData* data_ = new PacketData(string_.length() + 2);
	strcpy((char *)data_->data(), string_.data());
	sendPkt_->setdata(data_);
	sendPktCh_->size() = IP_HDR_LEN + string_.length() + 2 + aspu_->size();
#ifdef DEBUG_NDP
	fprintf(stdout, "\tsendPktIh_->saddr = %d\n", sendPktIh_->saddr());
	fprintf(stdout, "\tsendPktIh_->daddr = %d\n", sendPktIh_->daddr());
	fprintf(stdout, "\tsendPktRh_->srcId_ = %d\n", sendPktRh_->srcId_);
	fprintf(stdout, "\tsendPktRh_->srcAdd_ = %s\n", bitString(sendPktRh_->srcAdd_));
	fprintf(stdout, "\tsendPktRh_->dstAdd_ = %s\n", bitString(sendPktRh_->dstAdd_));
	fprintf(stdout, "\tsendPktRh_->seqNum_ = %d\n", sendPktRh_->seqNum_);
	fprintf(stdout, "\tsendPktRh_->data() = %s\n", ((PacketData*)sendPkt_->userdata())->data());

#endif
	if(numinterf) {
		for(int i=0; i<numinterf; i++) {
				Packet *p_copy = p->copy();
				Scheduler::instance().schedule(targetlist[i], p_copy, 0.0);
		}
		Packet::free(p);
	} else {
		Scheduler::instance().schedule(aspu_->getTarget(), sendPkt_, 0.0);
	}
}

void NDP::recvHello(Packet* recvPkt_) {
	struct hdr_aspu_hello *recvPktRh_ = HDR_aspu_HELLO(recvPkt_);
#ifdef DEBUG_NDP
	fprintf(stdout, "%.9f\tNDP::recvHello()\t\t\tin node %d\twith address %s\n", CURRENT_TIME, aspu_->id_, bitString(aspu_->address_));
	struct hdr_ip *recvPktIh_ = HDR_IP(recvPkt_);
	
	fprintf(stdout, "\trecvPktIh_->saddr = %d\n", recvPktIh_->saddr());
	fprintf(stdout, "\trecvPktIh_->daddr = %d\n", recvPktIh_->daddr());
	fprintf(stdout, "\trecvPktRh_->dstAdd_ = %s\n", bitString(recvPktRh_->dstAdd_));
	fprintf(stdout, "\trecvPktRh_->srcAdd_ = %s\n", bitString(recvPktRh_->srcAdd_));
	fprintf(stdout, "\trecvPktRh_->srcId_ = %d\n", recvPktRh_->srcId_);
	fprintf(stdout, "\trecvPktRh_->seqNum_ = %d\n", recvPktRh_->seqNum_);
	fprintf(stdout, "\tsendPktRh_->data() = %s\n", ((PacketData*)recvPkt_->userdata())->data());
#endif
	ostringstream strStream_;
	strStream_ << ((PacketData*)recvPkt_->userdata())->data();
	string string_ = strStream_.str();
	string::size_type start_ = string_.find_first_not_of("-", 0);
	string::size_type end_ = string_.find_first_of("-", start_);

	string table_ = string_.substr(start_, end_);
	double forQuality_ = (double) 0.0;
#ifdef DEBUG_NDP
	fprintf(stdout, "\ttable_ = %s\n", table_.c_str());
#endif
	if (aspu_->etxMetric_ == 1) {
		string quality_ = string_.substr(end_ + 1, string_.size() - end_ - 1);
#ifdef DEBUG_NDP
		fprintf(stdout, "\tquality_ = %s\n", quality_.c_str());
#endif
		string::size_type lastPos_ = quality_.find_first_not_of(";", 0);
		string::size_type pos_ = quality_.find_first_of(";", lastPos_);
		while (string::npos != pos_ || string::npos != lastPos_) {
			string neighbor_ = quality_.substr(lastPos_, pos_ - lastPos_);
			string::size_type idLastPos_ = neighbor_.find_first_not_of(",", 0);
			string::size_type idPos_ = neighbor_.find_first_of(",", idLastPos_);
			nsaddr_t id_  = atoi(neighbor_.substr(idLastPos_, idPos_ - idLastPos_).c_str());
			string::size_type qualityLastPos_ = neighbor_.find_first_not_of(",", idPos_);
			string::size_type qualityPos_ = neighbor_.find_first_of(",", qualityLastPos_);
		#ifdef DEBUG_NDP
			fprintf(stdout, "\tneigh id %d\tquality %f\n", id_, forQuality_);
		#endif
			if (id_ == aspu_->id_) {
		#ifdef DEBUG_NDP
				fprintf(stdout, "\t\tinsert\n");
		#endif
				forQuality_ = atof(neighbor_.substr(qualityLastPos_, qualityPos_ - qualityLastPos_).c_str());
			}
			lastPos_ = quality_.find_first_not_of(";", pos_);
			pos_ = quality_.find_first_of(";", lastPos_);
		}
	}
	neighborInsert(recvPktRh_->srcId_, recvPktRh_->srcAdd_, recvPktRh_->seqNum_, table_, forQuality_);
	Packet::free(recvPkt_);
	if(numinterf) {
		for(int i=0; i<numinterf; i++) {
			Scheduler::instance().schedule(ifqueuelist[i], p_copy, 0.0);
		}
		Packet::free(p);
	} else {
		Scheduler::instance().schedule(aspu_->getTarget(), recvPkt, 0.0);
	}

}

void NDP::startHelloTimer() {
	helloTimer_->handle((Event*) 0);
}

void NDP::startNeighborTimer() {
	neighborTimer_->handle((Event*) 0);
}

Neighbor* NDP::neighborLookup(nsaddr_t id) {

	neighborSet::iterator neighbor_;
	for (neighbor_ = neighborSet_->begin(); neighbor_ != neighborSet_->end(); ++neighbor_) {
		if((*neighbor_)->id() == id) {

			return (*neighbor_);
		}
	}
	return 0;
}

void NDP::neighborInsert(nsaddr_t id, nsaddr_t address, u_int32_t helloSeqNum, string table, double quality) {
#ifdef DEBUG_NDP
	fprintf(stdout, "%.9f\tNDP::neighborInsert(%d, %s, %f)\t\tin node %d\twith address %s\n", CURRENT_TIME, id, bitString(address), quality, aspu_->id_, bitString(aspu_->address_));
#endif
	Neighbor *neighbor_ = neighborLookup(id);
	if (neighbor_ == 0) {

		neighbor_ = new Neighbor(id, address, helloSeqNum);
		neighborSet_->insert(neighbor_);
	}
	else {
		neighbor_->address(address, helloSeqNum);

	}

	neighbor_->forLinkQuality(quality);

	neighbor_->updateTable(table);
}


void NDP::neighborPurge(void) {
 #ifdef DEBUG_NDP
 	fprintf(stdout, "%.9f\tNDP::neighborPurge()\t\t\tin node %d\twith address %s\n", CURRENT_TIME, aspu_->id_, bitString(aspu_->address_));
 #endif
	double now = CURRENT_TIME;
	neighborSet::iterator neighbor_ = neighborSet_->begin();
	while (neighbor_ != neighborSet_->end()) {
		neighborSet::iterator nextNeighbor_ = neighbor_;
		++nextNeighbor_;
		if((*neighbor_)->expire() <= now) {

			neighborSet_->erase(neighbor_);
			delete (*neighbor_);
#ifdef DEBUG_NDP
 			fprintf(stdout, "\tNeighbor purged\n");
#endif
		}
		else {
			(*neighbor_)->purgeHello();
		}
		neighbor_ = nextNeighbor_;
	}
}

void NDP::neighborPrint() {
#ifdef DEBUG_ADDRESS_ALLOCATION
 	fprintf(stdout, "\tNDP::neighborPrint()\t\t\tin node %d\twith address %s\n", aspu_->id_, bitString(aspu_->address_));
#endif
	neighborSet::iterator neighbor_;

   for (neighbor_ = neighborSet_->begin(); neighbor_ != neighborSet_->end(); ++neighbor_) {
      fprintf(stdout, "\t\tNeighbor id=%d\taddress=%s\n", (*neighbor_)->id(), bitString((*neighbor_)->address()));
   }
}

int NDP::neighborDegree() {
	int degree = 0;
	neighborSet::iterator neighbor_;
	for (neighbor_ = neighborSet_->begin(); neighbor_ != neighborSet_->end(); ++neighbor_) {
		degree += 1;
	}
	return degree;
}

int NDP::realNeighborDegree() {
	int degree = 0;
	neighborSet::iterator neighbor_;
	for (neighbor_ = neighborSet_->begin(); neighbor_ != neighborSet_->end(); ++neighbor_) {
		if(((*neighbor_)->expire() - (1.5 * NDP_ALLOWED_HELLO_LOSS * NDP_HELLO_INTERVAL) + NDP_HELLO_INTERVAL) >= CURRENT_TIME) {
			degree += 1;
		}
	}
	return degree;
}

string NDP::getNeighborQuality() {
#ifdef DEBUG_NDP
	fprintf(stdout, "%.9f\tNDP::getNeighborQuality()\n", CURRENT_TIME);
#endif
	string str_;
	neighborSet::iterator neighbor_;
	for (neighbor_ = neighborSet_->begin(); neighbor_ != neighborSet_->end(); ++neighbor_) {

			ostringstream stream_;
			stream_ << (*neighbor_)->id();
			stream_ << ',';
			stream_ << (*neighbor_)->revLinkQuality();
			stream_ << ';';
			str_ += stream_.str();
	}
#ifdef DEBUG_NDP
	fprintf(stdout, "\tdata = %s\n", str_.data());
#endif
	return str_;
}

void NDP::selectAddress() {
#ifdef DEBUG_NDP
	fprintf(stdout, "\tNDP::selectAddress()\t\t\tin node %d\twith address %s\n", aspu_->id_, bitString(aspu_->address_));
#endif
	neighborSet::iterator neighbor_;
	for (neighbor_ = neighborSet_->begin(); neighbor_ != neighborSet_->end(); ++neighbor_) {
#ifdef DEBUG_NDP_LINK_QUALITY
		if (aspu_->etxMetric_ == 1 && (*neighbor_)->linkQuality() < LQE_THRESHOLD) {
			fprintf(stdout, "%.9f\tNDP::selectAddress()\t\t\tin node %d\twith address %s\n", CURRENT_TIME, aspu_->id_, bitString(aspu_->address_));
			fprintf(stdout, "\tlinkQuality() = %f\trevLinkQuality() = %f\n", (*neighbor_)->linkQuality(), (*neighbor_)->revLinkQuality());
		}
#endif
		if ((aspu_->etxMetric_ == 1 && (*neighbor_)->linkQuality() > LQE_THRESHOLD) || aspu_->etxMetric_ == 0) {
#ifdef DEBUG_NDP
			fprintf(stdout, "\t\tselected neighbor id=%d\taddress=%s\n", (*neighbor_)->id(), bitString((*neighbor_)->address()));
#endif
			int i;
			for(i=(*neighbor_)->insertionPoint(); i>-1; i--) {
				std::bitset<ADDR_SIZE>	bitNewAddress_((*neighbor_)->address());
				bitNewAddress_.flip(i);
				aspu_->address_ = bitNewAddress_.to_ulong();
#ifdef DEBUG_ADDRESS_ALLOCATION
				fprintf(stdout, "\t\ttrying address=%s\n", bitString(aspu_->address_));
#endif
				aspu_->routingTable_->setUpdate((*neighbor_));
				if (validateAddress()) {
#ifdef DEBUG_ADDRESS_ALLOCATION
					fprintf(stdout, "\t\taddress selected is %s\n", bitNewAddress_.to_string().c_str());
#endif 
					aspu_->adp_->updateAdd();
					return;
				}
				else {
#ifdef DEBUG_NDP
					fprintf(stdout, "\t\taddress selected %s is not valid\n", bitNewAddress_.to_string().c_str());
#endif
					aspu_->address_ = aspu_->oldAddress_;
					aspu_->routingTable_->clear();
				}
			}
		}
	}
#ifdef DEBUG_NDP
	fprintf(stdout, "\t\tcouldn't select a valid address\n");
#endif
}

bool NDP::validateAddress() {
#ifdef DEBUG_ADDRESS_ALLOCATION
	fprintf(stdout, "\tNDP::validateAddress()\t\t\tin node %d\twith address %s\n", aspu_->id_, bitString(aspu_->address_));
	aspu_->routingTable_->print();
#endif
	neighborSet::iterator neighbor_;
	for (neighbor_ = neighborSet_->begin(); neighbor_ != neighborSet_->end(); ++neighbor_) {
#ifdef DEBUG_NDP_LINK_QUALITY
		fprintf(stdout, "\tid = %d\tlinkQuality() = %f\trevLinkQuality() = %f\n", (*neighbor_)->id(), (*neighbor_)->linkQuality(), (*neighbor_)->revLinkQuality());
#endif
		if ((aspu_->etxMetric_ == 1 && (*neighbor_)->linkQuality() >= LQE_THRESHOLD) || aspu_->etxMetric_ == 0) {
			int levelSibling_ = DiffBit(aspu_->address_, (*neighbor_)->address());
#ifdef DEBUG_ADDRESS_ALLOCATION
			fprintf(stdout, "\t\tNeighbor id=%d\taddress = %s\tLevel sibling = %d\n", (*neighbor_)->id(), bitString((*neighbor_)->address()), levelSibling_);
#endif
			if (levelSibling_ == -1) {
#ifdef DEBUG_ADDRESS_ALLOCATION
				if ((*neighbor_)->id() >= aspu_->id_) {
					fprintf(stdout, "\t\t\tNeighbor id=%d >= node id=%d\n", (*neighbor_)->id(), aspu_->id_);
				}
#endif
				if ((*neighbor_)->id() < aspu_->id_) {
#ifdef DEBUG_ADDRESS_ALLOCATION
					fprintf(stdout, "\t\t\tNeighbor id=%d < node id=%d\n", (*neighbor_)->id(), aspu_->id_);
					fprintf(stdout, "\t\t\tinvalid address\n");
#endif
					return false;
				}
			}
			else {
#ifdef DEBUG_ADDRESS_ALLOCATION
				if (!(*neighbor_)->entryPresent(levelSibling_)) {
					fprintf(stdout, "\t\t\tNeighbor Entry not present\n");
				}
#endif
				if ((*neighbor_)->entryPresent(levelSibling_)) {
#ifdef DEBUG_ADDRESS_ALLOCATION
					if ((*neighbor_)->networkId(levelSibling_) >= aspu_->routingTable_->levelId(levelSibling_-1)) {
						fprintf(stdout, "\t\t\tNeighborTableEntry->networkId(%d)=%d >= node levelId(%d)=%d\n", levelSibling_, (*neighbor_)->networkId(levelSibling_), levelSibling_ -1 , aspu_->routingTable_->levelId(levelSibling_-1));
					}
#endif
					if ((*neighbor_)->networkId(levelSibling_) < aspu_->routingTable_->levelId(levelSibling_-1)) {
#ifdef DEBUG_ADDRESS_ALLOCATION
						fprintf(stdout, "\t\t\tNeighborTableEntry->networkId(%d)=%d < node levelId(%d)=%d\n", levelSibling_, (*neighbor_)->networkId(levelSibling_), levelSibling_ -1 , aspu_->routingTable_->levelId(levelSibling_-1));
						fprintf(stdout, "\t\t\tinvalid address\n");
#endif
						return false;
					}
				}
			}
#ifdef DEBUG_ADDRESS_ALLOCATION
			fprintf(stdout, "\t\t\tvalid address\n");
#endif
		}
	}
	return true;
}

void NDP::updateRoutingTable() {
#ifdef DEBUG_NDP_LINK_QUALITY
	fprintf(stdout, "%.9f\tNDP::updateRoutingTable()\t\t\tin node %d\twith address %s\n", CURRENT_TIME, aspu_->id_, bitString(aspu_->address_));
#endif
	neighborSet::iterator neighbor_;
	for (neighbor_ = neighborSet_->begin(); neighbor_ != neighborSet_->end(); ++neighbor_) {
#ifdef DEBUG_NDP_LINK_QUALITY
		if (aspu_->etxMetric_ == 1 && (*neighbor_)->linkQuality() < LQE_THRESHOLD) {
			fprintf(stdout, "\tid = %d\tlinkQuality() = %f\trevLinkQuality() = %f\n", (*neighbor_)->id(), (*neighbor_)->linkQuality(), (*neighbor_)->revLinkQuality());

		}
#endif
		if ((aspu_->etxMetric_ == 1 && (*neighbor_)->linkQuality() >= LQE_THRESHOLD) || aspu_->etxMetric_ == 0)
			aspu_->routingTable_->setUpdate(*neighbor_);
	}
}

DHTTimer::DHTTimer(DHT* dht) {
	dht_ = dht;
	counter = 0;
	handle((Event*) 0);
}

void DHTTimer::handle(Event*) {

	if (counter > 1)
		dht_->purge();
	else
		counter++;
	Scheduler::instance().schedule(this, &intr, ASPU_DHT_PURGE_INTERVAL);
}

DHT::DHT() {
	timer_ = new DHTTimer(this);
	table_ = new dhtTable;
}

DHT::~DHT() {
	clear();
}

void DHT::addEntry(nsaddr_t id, nsaddr_t address) {
#ifdef DEBUG_DHT
	fprintf(stdout, "%.9f\tDHT::addEntry(%d, %d)\texpire = %f\n", CURRENT_TIME, id, address, CURRENT_TIME + ASPU_DHT_ENTRY_TIMEOUT);
	printDHT();
#endif

	DHT_Entry* entry_ = getEntry(id);
	if (entry_ == NULL) {
		entry_ = new DHT_Entry(id, address);
		table_->insert(entry_);
	}
	else {
		entry_->address(address);
		entry_->expire(CURRENT_TIME + ASPU_DHT_ENTRY_TIMEOUT);
	}
#ifdef DEBUG_DHT
	printDHT();
#endif
}

void DHT::addEntry(nsaddr_t id, nsaddr_t address, double expire) {
#ifdef DEBUG_DHT
	fprintf(stdout, "%.9f\tDHT::addEntry(%d, %d)\texpire = %f\n", CURRENT_TIME, id, address, CURRENT_TIME + ASPU_DHT_ENTRY_TIMEOUT);
	printDHT();
#endif

	DHT_Entry* entry_ = getEntry(id);
	if (entry_ == NULL) {
		entry_ = new DHT_Entry(id, address);
		entry_->expire(expire);
		table_->insert(entry_);
	}
	else {
		entry_->address(address);
		entry_->expire(expire);
	}
#ifdef DEBUG_DHT
	printDHT();
#endif
}

void DHT::purge() {
#ifdef DEBUG_DHT
	fprintf(stdout, "%.9f\tDHT::purge()\n", CURRENT_TIME);
	printDHT();
#endif
	dhtTable::iterator entry_ = table_->begin();
	while(entry_ != table_->end() && !table_->empty()) {
		dhtTable::iterator nextEntry_ = entry_;
		nextEntry_++;
		if ((*entry_)->expire() < CURRENT_TIME) {
#ifdef DEBUG_DHT
			fprintf(stdout, "\tpurging\tnode id = %d\tnode address = %d\texpire time = %f\n", (*entry_)->id(), (*entry_)->address(), (*entry_)->expire());
#endif
			table_->erase(entry_);
			delete (*entry_);
		}
		entry_ = nextEntry_;
	}
#ifdef DEBUG_DHT
	printDHT();
#endif
}

DHT_Entry* DHT::getEntry(u_int32_t pos) {
#ifdef DEBUG_DHT
	fprintf(stdout, "%.9f\tDHT::getEntry(%d)\n", CURRENT_TIME, pos);
#endif
	dhtTable::iterator entry_ = table_->begin();
	while (pos  > 0) {
		entry_++;
		pos--;
	}
	return (*entry_);
}

nsaddr_t DHT::findAdd(nsaddr_t id) {
#ifdef DEBUG_DHT
	fprintf(stdout, "%.9f\tDHT::findAdd(%d)\n", CURRENT_TIME, id);
#endif
	dhtTable::iterator entry_;
	for(entry_ = table_->begin(); entry_ != table_->end(); ++entry_) {
		if ((*entry_)->id() == id) {
#ifdef DEBUG_DHT
			fprintf(stdout, "\tfind address %d\n", (*entry_)->address());
#endif
			return (*entry_)->address();
		}
	}
	return IP_BROADCAST;
}
	
nsaddr_t DHT::findId(nsaddr_t address) {
#ifdef DEBUG_DHT
	fprintf(stdout, "%.9f\tDHT::findId(%d)\n", CURRENT_TIME, address);
#endif
	dhtTable::iterator entry_;
	for(entry_ = table_->begin(); entry_ != table_->end(); ++entry_) {
		if ((*entry_)->address() == address)
#ifdef DEBUG_DHT
			fprintf(stdout, "\tfind id %d\n", (*entry_)->id());
#endif
			return (*entry_)->id();
	}
	return IP_BROADCAST;
}

u_int32_t DHT::DHTSize() {
	return table_->size();
}

PacketData* DHT::getDHTUpdate() {
#ifdef DEBUG_DHT
	fprintf(stdout, "%.9f\tDHT::getDHTUpdate()\n", CURRENT_TIME);
#endif
	string string_;
	dhtTable::iterator entry_;
	for(entry_ = table_->begin(); entry_ != table_->end(); ++entry_) {
		ostringstream stream_;
		stream_ << (*entry_)->id();
		stream_ << ',';
		stream_ << (*entry_)->address();
		stream_ << ',';
		stream_ << (*entry_)->expire();
		stream_ << ';';
		string_ += stream_.str();
	}
#ifdef DEBUG_DHT
	fprintf(stdout, "\tdata = %s\n", string_.data());
#endif
	PacketData* data_ = new PacketData(string_.length() + 2);
	strcpy((char *)data_->data(), string_.data());
	return data_;	
}

void DHT::printDHT() {
	dhtTable::iterator entry_;
	fprintf(stdout, "\tDHT::printDHT()\n");
	for(entry_ = table_->begin(); entry_ != table_->end(); ++entry_) {
		fprintf(stdout, "\t\tnode id = %d\tnode address = %s\texpire time = %f\n", (*entry_)->id(), bitString((*entry_)->address()), (*entry_)->expire());
	}
}

DHT_Entry* DHT::getEntry(nsaddr_t id) {
	dhtTable::iterator entry_;
	for(entry_ = table_->begin(); entry_ != table_->end(); entry_++) {
		if ((*entry_)->id() == id) {
			return (*entry_);
		}
	}
	return NULL;
}

void DHT::removeEntry(nsaddr_t id) {
	dhtTable::iterator entry_ = table_->begin();
	while (entry_ != table_->end() && !table_->empty()) {
		dhtTable::iterator nextEntry_ = entry_;
		++nextEntry_;
		if ((*entry_)->id() == id) {
			table_->erase(entry_);
			delete (*entry_);
		}
		entry_ = nextEntry_;
	}
}

void DHT::clear() {
	dhtTable::iterator entry_ = table_->begin();
	while (entry_ != table_->end() && !table_->empty()) {
		dhtTable::iterator nextEntry_ = entry_;
		++nextEntry_;
		table_->erase(entry_);
		delete (*entry_);
		entry_ = nextEntry_;
	}
}

ADP::ADP(ASPU* aspu) {
	aspu_ = aspu;
	timer_ = new ADPDaupTimer(this);
	timerRx_ = new ADPDarqTimer(this);
	dht_ = new DHT();
	pckSeqNum_ = 0;
 }

 ADP::~ADP() {
	delete timer_;
	delete dht_;
}
 
void ADP::updateAdd() {
	addEntry(aspu_->id_, aspu_->address_);
#ifdef ADP_DABR_ENABLE 
	if (DHTSize() > 0) {
		sendDabr();
	}
#endif
}

void ADP::checkDarq() {
#ifdef DEBUG_ADP
	fprintf(stdout, "%.9f\tASPU::checkDarq()\t\t\tin node %d\twith address %s\n", CURRENT_TIME, aspu_->id_, bitString(aspu_->address_));
#endif
	for (int i=0; i<aspu_->queue_->size(); i++) {
		if (aspu_->queue_->isExpired(i)) {
			Packet *p = aspu_->queue_->getPacket(i);
#ifdef DEBUG_ADP
			fprintf(stdout, "\tre-sending darq for node %d\n", HDR_IP(p)->daddr());
#endif
			sendDarq(HDR_IP(p)->daddr(), HDR_CMN(p)->uid());			
		}
	}
}

void ADP::sendDarq(nsaddr_t reqId, int reqpktId) {
#ifdef DEBUG_ADP
	fprintf(stdout, "%.9f\tADP::sendDarq(%d)\t\t\tin node %d\twith address %s\n", CURRENT_TIME, reqId, aspu_->id_, bitString(aspu_->address_));
#endif
	nsaddr_t dstAdd_ = hash(reqId);
#ifdef DEBUG_ADP
	fprintf(stdout, "\tsending darq for node %s\n", bitString(dstAdd_));
#endif
	if (dstAdd_ == aspu_->address_ ) {
#ifdef DEBUG_ADP
		fprintf(stdout, "\tDARQ DROP_RTR_ADPMISS\n");
#endif
		return;
	}
	if (aspu_->routingTable_->DAGetEntry(dstAdd_) == (nsaddr_t)IP_BROADCAST) {
		bitset<ADDR_SIZE> oldAdd_ (dstAdd_);
#ifdef DEBUG_ADP
		fprintf(stdout, "\told dstAdd_ = %s\n", bitString(dstAdd_));
#endif
		int i = 0;
		while ((aspu_->routingTable_->DAGetEntry(dstAdd_) == (nsaddr_t)IP_BROADCAST) && (i < ADDR_SIZE)) {
			oldAdd_.reset(i);
			dstAdd_ = (nsaddr_t) oldAdd_.to_ulong();
			i++;
		}
#ifdef DEBUG_ADP
		fprintf(stdout, "\tnew dstAdd_ = %s\n", bitString(dstAdd_));
#endif
		if (aspu_->routingTable_->DAGetEntry(dstAdd_) == (nsaddr_t)IP_BROADCAST) {
#ifdef DEBUG_ADP
			fprintf(stdout, "\tDARQ DROP_RTR_ADPNRT\n");
#endif
			return;
		}
	}
	nsaddr_t nextHop_ = aspu_->routingTable_->DAGetEntry(dstAdd_);
#ifdef DEBUG_ADP
	fprintf(stdout, "\tselected nextHop %d\n", nextHop_);
#endif
	Packet *sendPkt_ = Packet::alloc();
	struct hdr_cmn *sendPktCh_ = HDR_CMN(sendPkt_);
	struct hdr_ip *sendPktIh_ = HDR_IP(sendPkt_);
	struct hdr_aspu_darq *sendPktRh_ = HDR_ASPU_DARQ(sendPkt_);
	sendPktCh_->ptype() = PT_ASPU;
	sendPktCh_->size() = IP_HDR_LEN + aspu_->size();
	sendPktCh_->iface() = -2;
	sendPktCh_->error() = 0;
	sendPktCh_->addr_type() = NS_AF_NONE;
	sendPktCh_->next_hop() = nextHop_;		
	sendPktCh_->ts_ = CURRENT_TIME;		
	sendPktIh_->saddr() = aspu_->id_;
	sendPktIh_->daddr() = nextHop_;
	sendPktIh_->sport() = RT_PORT;
	sendPktIh_->dport() = RT_PORT;
	sendPktIh_->ttl_ = IP_DEF_TTL;		
	sendPktRh_->type_ = ASPU_TYPE_DARQ;
	sendPktRh_->srcId_ = aspu_->id_;
	sendPktRh_->srcAdd_ = aspu_->address_;
	sendPktRh_->forId_ = aspu_->id_;
	sendPktRh_->forAdd_ = aspu_->address_;
	sendPktRh_->dstId_ = findId(dstAdd_);
	sendPktRh_->dstAdd_ = dstAdd_;
	sendPktRh_->lifetime_ = ADP_DARQ_TIMEOUT;
	sendPktRh_->reqId_ = reqId;
	sendPktRh_->seqNum_ = pckSeqNum();
#ifdef DEBUG_ADP
	fprintf(stdout, "\tsendPktIh_->saddr = %d\n", sendPktIh_->saddr());
	fprintf(stdout, "\tsendPktIh_->daddr = %d\n", sendPktIh_->daddr());
	fprintf(stdout, "\tsendPktRh_->srcId_ = %d\n", sendPktRh_->srcId_);
	fprintf(stdout, "\tsendPktRh_->srcAdd_ = %s\n", bitString(sendPktRh_->srcAdd_));
	fprintf(stdout, "\tsendPktRh_->forId_ = %d\n", sendPktRh_->forId_);
	fprintf(stdout, "\tsendPktRh_->forAdd_ = %s\n", bitString(sendPktRh_->forAdd_));
	fprintf(stdout, "\tsendPktRh_->dstId_ = %d\n", sendPktRh_->dstId_);
	fprintf(stdout, "\tsendPktRh_->dstAdd_ = %s\n", bitString(sendPktRh_->dstAdd_));
	fprintf(stdout, "\tsendPktRh_->reqId_ = %d\n", sendPktRh_->reqId_);
	fprintf(stdout, "\tsendPktRh_->seqNum_ = %d\n", sendPktRh_->seqNum_);
#endif
	if(numinterf) {
		for(int i=0; i<numinterf; i++) {
				Packet *p_copy = p->copy();
				Scheduler::instance().schedule(targetlist[i], p_copy, 0.0);
		}
		Packet::free(p);
	} else {
		Scheduler::instance().schedule(aspu_->getTarget(), sendPkt_, 0.0);
	}
	
}
void ADP::recvDarq(Packet* recvPkt) {
#ifdef DEBUG_ADP
	fprintf(stdout, "%.9f\tADP::recvDarq()\t\t\tin node %d\twith address %s\n", CURRENT_TIME, aspu_->id_, bitString(aspu_->address_));
#endif
	struct hdr_cmn *recvPktCh_ = HDR_CMN(recvPkt);
	struct hdr_ip *recvPktIh_ = HDR_IP(recvPkt);
	struct hdr_aspu_darq *recvPktRh_ = HDR_ASPU_DARQ(recvPkt);
	
#ifdef DEBUG_ADP
	fprintf(stdout, "\treceveing darq for node %s\n", bitString(recvPktRh_->dstAdd_));
	aspu_->routingTable_->print();
#endif
#ifdef ADP_DAXX_CACHING
	addEntry(recvPktRh_->srcId_, recvPktRh_->srcAdd_);
	addEntry(recvPktRh_->forId_, recvPktRh_->forAdd_);
#endif
	nsaddr_t findAdd_ = findAdd(recvPktRh_->reqId_);
	bool changeAdd_ = false;		
	if (findAdd_ != (nsaddr_t) IP_BROADCAST) {
#ifdef DEBUG_ADP
		fprintf(stdout, "\tDARQ received\n");
#endif
		sendDarp(recvPkt);
		return;
	}	
	if (recvPktRh_->dstAdd_ == aspu_->address_) {
#ifdef DEBUG_ADP
		fprintf(stdout, "\tDARQ DROP_RTR_ADPMISS\n");
#endif
		aspu_->drop(recvPkt, "DROP_RTR_ADPMISS");
		return;
	}
	if (aspu_->routingTable_->DAGetEntry(recvPktRh_->dstAdd_) == (nsaddr_t)IP_BROADCAST) {
		changeAdd_ = true;	
		bitset<ADDR_SIZE> oldAdd_ (recvPktRh_->dstAdd_);
#ifdef DEBUG_ADP
		fprintf(stdout, "\told dstAdd_ = %s\n", bitString(recvPktRh_->dstAdd_));
#endif
		int i = 0;
		while (((aspu_->routingTable_->DAGetEntry(recvPktRh_->dstAdd_) == recvPktRh_->forId_) || (aspu_->routingTable_->DAGetEntry(recvPktRh_->dstAdd_) == (nsaddr_t)IP_BROADCAST)) && (i < ADDR_SIZE)) {
			oldAdd_.reset(i);
			recvPktRh_->dstAdd_ = (nsaddr_t) oldAdd_.to_ulong();
			i++;
		}
#ifdef DEBUG_ADP
		fprintf(stdout, "\tnew dstAdd_ = %s\n", bitString(recvPktRh_->dstAdd_));
#endif
		if (aspu_->routingTable_->DAGetEntry(recvPktRh_->dstAdd_) == (nsaddr_t)IP_BROADCAST) {
#ifdef DEBUG_ADP
			fprintf(stdout, "\tDARQ DROP_RTR_ADPNRT\n");
#endif
			aspu_->drop(recvPkt, "DROP_RTR_ADPNRT");
			return;
		}
	}
	nsaddr_t nextHop_ = aspu_->routingTable_->DAGetEntry(recvPktRh_->dstAdd_);
	if (nextHop_ == recvPktRh_->forId_ && !changeAdd_) {
#ifdef DEBUG_ADP
		fprintf(stdout, "\tDARQ DROP_RTR_ADPLOOP\n");
#endif
		aspu_->drop(recvPkt, "DROP_RTR_ADPNRT");
		return;
	}
#ifdef DEBUG_ADP
	fprintf(stdout, "\t\tselected nextHop_ = %d\n", nextHop_);
#endif
	recvPktCh_->direction() = hdr_cmn::DOWN;
	recvPktCh_->next_hop() = nextHop_;		
	recvPktIh_->daddr() = nextHop_;
	recvPktRh_->forId_ = aspu_->id_;
	recvPktRh_->forAdd_ = aspu_->address_;
	recvPktRh_->dstId_ = findId(recvPktRh_->dstAdd_);
#ifdef DEBUG_ADP
	fprintf(stdout, "\trecvPktIh_->saddr = %d\n", recvPktIh_->saddr());
	fprintf(stdout, "\trecvPktIh_->daddr = %d\n", recvPktIh_->daddr());
	fprintf(stdout, "\trecvPktRh_->srcId_ = %d\n", recvPktRh_->srcId_);
	fprintf(stdout, "\trecvPktRh_->srcAdd_ = %s\n", bitString(recvPktRh_->srcAdd_));
	fprintf(stdout, "\trecvPktRh_->forId_ = %d\n", recvPktRh_->forId_);
	fprintf(stdout, "\trecvPktRh_->forAdd_ = %s\n", bitString(recvPktRh_->forAdd_));
	fprintf(stdout, "\trecvPktRh_->dstId_ = %d\n", recvPktRh_->dstId_);
	fprintf(stdout, "\trecvPktRh_->dstAdd_ = %s\n", bitString(recvPktRh_->dstAdd_));
	fprintf(stdout, "\trecvPktRh_->reqId_ = %d\n", recvPktRh_->reqId_);
#endif	
	if(numinterf) {
		for(int i=0; i<numinterf; i++) {
			Scheduler::instance().schedule(ifqueuelist[i], p_copy, 0.0);
		}
		Packet::free(p);
	} else {
		Scheduler::instance().schedule(aspu_->getTarget(), recvPkt, 0.0);
	}
}

void ADP::sendDarp(Packet* recvPkt) {
#ifdef DEBUG_ADP
	fprintf(stdout, "%.9f\tASPU::sendDarp()\t\t\tin node %d\twith address %s\n", CURRENT_TIME, aspu_->id_, bitString(aspu_->address_));
#endif
	struct hdr_aspu_darq *recvPktRh_ = HDR_ASPU_DARQ(recvPkt);
	nsaddr_t nextHop_ = aspu_->routingTable_->getEntry(recvPktRh_->srcAdd_);
#ifdef DEBUG_ADP
	fprintf(stdout, "\t\tselected nextHop_ = %d\n", nextHop_);
#endif
	Packet *sendPkt_ = Packet::alloc();
	struct hdr_cmn *sendPktCh_ = HDR_CMN(sendPkt_);
	struct hdr_ip *sendPktIh_ = HDR_IP(sendPkt_);
	struct hdr_aspu_darp *sendPktRh_ = HDR_ASPU_DARP(sendPkt_);
	sendPktCh_->ptype() = PT_ASPU;
	sendPktCh_->size() = IP_HDR_LEN + aspu_->size();
	sendPktCh_->iface() = -2;
	sendPktCh_->error() = 0;
	sendPktCh_->addr_type() = NS_AF_NONE;
	sendPktCh_->next_hop() = nextHop_;
	sendPktIh_->saddr() = aspu_->id_;
	sendPktIh_->daddr() = nextHop_;
	sendPktIh_->sport() = RT_PORT;
	sendPktIh_->dport() = RT_PORT;
	sendPktIh_->ttl_ = IP_DEF_TTL;		
	sendPktRh_->type_ = ASPU_TYPE_DARP;
	sendPktRh_->srcId_ = aspu_->id_;
	sendPktRh_->srcAdd_ = aspu_->address_;
	sendPktRh_->forId_ = aspu_->id_;
	sendPktRh_->forAdd_ = aspu_->address_;
	sendPktRh_->dstId_ = recvPktRh_->srcId_;
	sendPktRh_->dstAdd_ = recvPktRh_->srcAdd_;
	sendPktRh_->reqId_ = recvPktRh_->reqId_;
	sendPktRh_->reqAdd_ = findAdd(recvPktRh_->reqId_);
	sendPktRh_->lifetime_ = ADP_DARP_TIMEOUT;
	sendPktRh_->seqNum_ = pckSeqNum();
#ifdef DEBUG_ADP
	fprintf(stdout, "\tsendPktIh_->saddr = %d\n", sendPktIh_->saddr());
	fprintf(stdout, "\tsendPktIh_->daddr = %d\n", sendPktIh_->daddr());
	fprintf(stdout, "\tsendPktRh_->srcId = %d\n", sendPktRh_->srcId_);
	fprintf(stdout, "\tsendPktRh_->srcAdd = %s\n", bitString(sendPktRh_->srcAdd_));
	fprintf(stdout, "\tsendPktRh_->forId = %d\n", sendPktRh_->forId_);
	fprintf(stdout, "\tsendPktRh_->forAdd = %s\n", bitString(sendPktRh_->forAdd_));
	fprintf(stdout, "\tsendPktRh_->dstId = %d\n", sendPktRh_->dstId_);
	fprintf(stdout, "\tsendPktRh_->dstAdd = %s\n", bitString(sendPktRh_->dstAdd_));
	fprintf(stdout, "\tsendPktRh_->reqId = %d\n", recvPktRh_->reqId_);
	fprintf(stdout, "\tsendPktRh_->reqAdd = %s\n", bitString(sendPktRh_->reqAdd_));
#endif
	Packet::free(recvPkt);
	if(numinterf) {
		for(int i=0; i<numinterf; i++) {
				Packet *p_copy = p->copy();
				Scheduler::instance().schedule(targetlist[i], p_copy, 0.0);
		}
		Packet::free(p);
	} else {
		Scheduler::instance().schedule(aspu_->getTarget(), sendPkt_, 0.0);
	}
}

void ADP::recvDarp(Packet* recvPkt) {
#ifdef DEBUG_ADP
	fprintf(stdout, "%.9f\tASPU::recvDarp()\t\t\tin node %d\twith address %s\n", CURRENT_TIME, aspu_->id_, bitString(aspu_->address_));
#endif
	struct hdr_cmn *recvPktCh_ = HDR_CMN(recvPkt);
	struct hdr_ip *recvPktIh_ = HDR_IP(recvPkt);
	struct hdr_aspu_darp* recvPktRh_ = HDR_ASPU_DARP(recvPkt);
#ifdef DEBUG_ADP
	fprintf(stdout, "\treceveing darp from node %s\n", bitString(recvPktRh_->srcAdd_));
#endif
#ifdef ADP_DAXX_CACHING
	addEntry(recvPktRh_->srcId_, recvPktRh_->srcAdd_);
	addEntry(recvPktRh_->forId_, recvPktRh_->forAdd_);
	addEntry(recvPktRh_->dstId_, recvPktRh_->dstAdd_);
	addEntry(recvPktRh_->reqId_, recvPktRh_->reqAdd_);
#endif
	if (recvPktRh_->dstAdd_ == aspu_->address_) {
		addEntry(recvPktRh_->reqId_, recvPktRh_->reqAdd_);
		Packet::free(recvPkt);
		return;
	}
	nsaddr_t nextHop_ = aspu_->routingTable_->getEntry(recvPktRh_->dstAdd_);
	if (nextHop_ == (nsaddr_t)IP_BROADCAST) {
#ifdef DEBUG_ADP
		fprintf(stdout, "\tDARP DROP_RTR_ADPRTR\n");
#endif
		Packet::free(recvPkt);
		return;
	}
	if (nextHop_ == recvPktRh_->forId_) {
#ifdef DEBUG_ADP
		fprintf(stdout, "\tDARP DROP_RTR_ADPLOOP\n");
#endif
		Packet::free(recvPkt);
		return;
	}
#ifdef DEBUG_ADP
	fprintf(stdout, "\t\tselected nextHop_ = %d\n", nextHop_);
#endif
	recvPktCh_->direction() = hdr_cmn::DOWN;
	recvPktCh_->next_hop() = nextHop_;		

	recvPktIh_->daddr() = nextHop_;

	recvPktRh_->forId_ = aspu_->id_;
	recvPktRh_->forAdd_ = aspu_->address_;
#ifdef DEBUG_ADP
	fprintf(stdout, "\tsendPktIh_->saddr = %d\n", recvPktIh_->saddr());
	fprintf(stdout, "\tsendPktIh_->daddr = %d\n", recvPktIh_->daddr());
	fprintf(stdout, "\tsendPktRh_->srcId = %d\n", recvPktRh_->srcId_);
	fprintf(stdout, "\tsendPktRh_->srcAdd = %s\n", bitString(recvPktRh_->srcAdd_));
	fprintf(stdout, "\tsendPktRh_->forId = %d\n", recvPktRh_->forId_);
	fprintf(stdout, "\tsendPktRh_->forAdd = %s\n", bitString(recvPktRh_->forAdd_));
	fprintf(stdout, "\tsendPktRh_->dstId = %d\n", recvPktRh_->dstId_);
	fprintf(stdout, "\tsendPktRh_->dstAdd = %s\n", bitString(recvPktRh_->dstAdd_));
	fprintf(stdout, "\tsendPktRh_->reqId = %d\n", recvPktRh_->reqId_);
	fprintf(stdout, "\tsendPktRh_->reqAdd = %s\n", bitString(recvPktRh_->reqAdd_));
#endif	
	if(numinterf) {
		for(int i=0; i<numinterf; i++) {
			Scheduler::instance().schedule(ifqueuelist[i], p_copy, 0.0);
		}
		Packet::free(p);
	} else {
		Scheduler::instance().schedule(aspu_->getTarget(), recvPkt, 0.0);
	}
}

void ADP::sendDaup() {
#ifdef DEBUG_ADP
	fprintf(stdout, "%.9f\tASPU::sendDaup()\t\t\t\tin node %d\twith address %s\n", CURRENT_TIME, aspu_->id_, bitString(aspu_->address_));
#endif
	nsaddr_t dstAdd_ = hash(aspu_->id_);
#ifdef DEBUG_ADP
	fprintf(stdout, "\tsending daup for node  %s\n", bitString(dstAdd_));
	aspu_->routingTable_->print();
#endif
	if (dstAdd_ == aspu_->address_) {
		addEntry(aspu_->id_, aspu_->address_);
#ifdef DEBUG_ADP
		fprintf(stdout, "\tDAUP received\n");
#endif
		return;
	}
	if (aspu_->routingTable_->DAGetEntry(dstAdd_) == (nsaddr_t)IP_BROADCAST) {
		bitset<ADDR_SIZE> oldAdd_ (dstAdd_);
#ifdef DEBUG_ADP
		fprintf(stdout, "\told dstAdd_ = %s\n", bitString(dstAdd_));
#endif
		int i = 0;
		while ((aspu_->routingTable_->DAGetEntry(dstAdd_) == (nsaddr_t)IP_BROADCAST) && (i < ADDR_SIZE)) {
			oldAdd_.reset(i);
			dstAdd_ = (nsaddr_t) oldAdd_.to_ulong();
			i++;
		}
#ifdef DEBUG_ADP
		fprintf(stdout, "\tnew dstAdd_ = %s\n", bitString(dstAdd_));
#endif
		if (aspu_->routingTable_->DAGetEntry(dstAdd_) == (nsaddr_t)IP_BROADCAST) {
			addEntry(aspu_->id_, aspu_->address_);
#ifdef DEBUG_ADP
			fprintf(stdout, "\tDAUP DROP_RTR_ADPNRT\n");
#endif		
			return;
		}
	}
	nsaddr_t nextHop_ = aspu_->routingTable_->DAGetEntry(dstAdd_);
#ifdef DEBUG_ADP
	fprintf(stdout, "\tselected nextHop_ = %d\n", nextHop_);
#endif
	Packet *sendPkt_ = Packet::alloc();
	struct hdr_cmn *sendPktCh_ = HDR_CMN(sendPkt_);
	struct hdr_ip *sendPktIh_ = HDR_IP(sendPkt_);
	struct hdr_aspu_daup *sendPktRh_ = HDR_ASPU_DAUP(sendPkt_);
	sendPktCh_->ptype() = PT_ASPU;
	sendPktCh_->size() = IP_HDR_LEN + aspu_->size();
	sendPktCh_->iface() = -2;
	sendPktCh_->error() = 0;
	sendPktCh_->addr_type() = NS_AF_NONE;
	sendPktCh_->next_hop() = nextHop_;		
	sendPktIh_->saddr() = aspu_->id_;
	sendPktIh_->daddr() = nextHop_;
	sendPktIh_->sport() = RT_PORT;
	sendPktIh_->dport() = RT_PORT;
	sendPktIh_->ttl_ = IP_DEF_TTL;		
	sendPktRh_->type_ = ASPU_TYPE_DAUP;
	sendPktRh_->srcId_ = aspu_->id_;
	sendPktRh_->srcAdd_ = aspu_->address_;
	sendPktRh_->forId_ = aspu_->id_;
	sendPktRh_->forAdd_ = aspu_->address_;
	sendPktRh_->dstId_ = IP_BROADCAST;
	sendPktRh_->dstAdd_ = dstAdd_;
	sendPktRh_->lifetime_ = ADP_DARP_TIMEOUT;
	sendPktRh_->seqNum_ = pckSeqNum();
#ifdef DEBUG_ADP
	fprintf(stdout, "\tsendPktIh_->saddr = %d\n", sendPktIh_->saddr());
	fprintf(stdout, "\tsendPktIh_->daddr = %d\n", sendPktIh_->daddr());
	fprintf(stdout, "\tsendPktRh_->srcId_ = %d\n", sendPktRh_->srcId_);
	fprintf(stdout, "\tsendPktRh_->srcAdd_ = %s\n", bitString(sendPktRh_->srcAdd_));
	fprintf(stdout, "\tsendPktRh_->forId_ = %d\n", sendPktRh_->forId_);
	fprintf(stdout, "\tsendPktRh_->forAdd_ = %s\n", bitString(sendPktRh_->forAdd_));
	fprintf(stdout, "\tsendPktRh_->dstId_ = %d\n", sendPktRh_->dstId_);
	fprintf(stdout, "\tsendPktRh_->dstAdd_ = %s\n", bitString(sendPktRh_->dstAdd_));
#endif
	if(numinterf) {
		for(int i=0; i<numinterf; i++) {
				Packet *p_copy = p->copy();
				Scheduler::instance().schedule(targetlist[i], p_copy, 0.0);
		}
		Packet::free(p);
	} else {
		Scheduler::instance().schedule(aspu_->getTarget(), sendPkt_, 0.0);
	}
}

void ADP::recvDaup(Packet* recvPkt) {
#ifdef DEBUG_ADP
	fprintf(stdout, "%.9f\tADP::recvDaup()\t\tin node %d\twith address %s\n", CURRENT_TIME, aspu_->id_, bitString(aspu_->address_));
//	printDHT();
#endif
	struct hdr_cmn *recvPktCh_ = HDR_CMN(recvPkt);
	struct hdr_ip *recvPktIh_ = HDR_IP(recvPkt);
	struct hdr_aspu_daup *recvPktRh_ = HDR_ASPU_DAUP(recvPkt);
	bool changeAdd_ = false;	
#ifdef DEBUG_ADP
	fprintf(stdout, "\treceveing daup for node %s\n", bitString(recvPktRh_->dstAdd_));
	aspu_->routingTable_->print();
#endif
#ifdef	ADP_DAXX_CACHING
	addEntry(recvPktRh_->srcId_, recvPktRh_->srcAdd_);
	addEntry(recvPktRh_->forId_, recvPktRh_->forAdd_);
#endif
	if (recvPktRh_->dstAdd_ == aspu_->address_) {
		addEntry(recvPktRh_->srcId_, recvPktRh_->srcAdd_);
#ifdef DEBUG_ADP
		fprintf(stdout, "\tDAUP received\n");
#endif
		Packet::free(recvPkt);
		return;
	}
	if (aspu_->routingTable_->DAGetEntry(recvPktRh_->dstAdd_) == (nsaddr_t)IP_BROADCAST) {
		changeAdd_ = true;	
		bitset<ADDR_SIZE> oldAdd_ (recvPktRh_->dstAdd_);
#ifdef DEBUG_ADP
		fprintf(stdout, "\told dstAdd_ = %s\n", bitString(recvPktRh_->dstAdd_));
#endif
		int i = 0;
		while (((aspu_->routingTable_->DAGetEntry(recvPktRh_->dstAdd_) == recvPktRh_->forId_) || (aspu_->routingTable_->DAGetEntry(recvPktRh_->dstAdd_) == (nsaddr_t)IP_BROADCAST)) && (i < ADDR_SIZE)) {
			oldAdd_.reset(i);
			recvPktRh_->dstAdd_ = (nsaddr_t) oldAdd_.to_ulong();
			i++;
		}
#ifdef DEBUG_ADP
		fprintf(stdout, "\tnew dstAdd_ = %s\n", bitString(recvPktRh_->dstAdd_));
#endif
		if (aspu_->routingTable_->DAGetEntry(recvPktRh_->dstAdd_) == (nsaddr_t)IP_BROADCAST) {
			addEntry(recvPktRh_->srcId_, recvPktRh_->srcAdd_);
#ifdef DEBUG_ADP
			fprintf(stdout, "\tDAUP DROP_RTR_ADPNRT\n");
#endif		
			aspu_->drop(recvPkt, "DROP_RTR_ADPNRT");
			return;
		}
	}
	nsaddr_t nextHop_ = aspu_->routingTable_->DAGetEntry(recvPktRh_->dstAdd_);
	if (nextHop_ == recvPktRh_->forId_ && !changeAdd_) {
		addEntry(recvPktRh_->srcId_, recvPktRh_->srcAdd_);
#ifdef DEBUG_ADP
		fprintf(stdout, "\tDAUP DROP_RTR_ADPLOOP\n");
#endif
		aspu_->drop(recvPkt, "DROP_RTR_ADPLOOP");
		return;
	}
#ifdef DEBUG_ADP
	fprintf(stdout, "\t\tselected nextHop_ = %d\n", nextHop_);
#endif
	recvPktCh_->direction() = hdr_cmn::DOWN;
	recvPktCh_->next_hop() = nextHop_;
	recvPktIh_->daddr() = nextHop_;
	recvPktRh_->forId_ = aspu_->id_;
	recvPktRh_->forAdd_ = aspu_->address_;
	recvPktRh_->dstId_ = findId(recvPktRh_->dstAdd_);
#ifdef DEBUG_ADP
	fprintf(stdout, "\trecvPktIh_->saddr = %d\n", recvPktIh_->saddr());
	fprintf(stdout, "\trecvPktIh_->daddr = %d\n", recvPktIh_->daddr());
	fprintf(stdout, "\trecvPktRh_->srcId_ = %d\n", recvPktRh_->srcId_);
	fprintf(stdout, "\trecvPktRh_->srcAdd_ = %s\n", bitString(recvPktRh_->srcAdd_));
	fprintf(stdout, "\trecvPktRh_->forId_ = %d\n", recvPktRh_->forId_);
	fprintf(stdout, "\trecvPktRh_->forAdd_ = %s\n", bitString(recvPktRh_->forAdd_));
	fprintf(stdout, "\trecvPktRh_->dstId_ = %d\n", recvPktRh_->dstId_);
	fprintf(stdout, "\trecvPktRh_->dstAdd_ = %s\n", bitString(recvPktRh_->dstAdd_));
#endif	
	if(numinterf) {
		for(int i=0; i<numinterf; i++) {
			Scheduler::instance().schedule(ifqueuelist[i], p_copy, 0.0);
		}
		Packet::free(p);
	} else {
		Scheduler::instance().schedule(aspu_->getTarget(), recvPkt, 0.0);
	}
}

void ADP::sendDabr() {
#ifdef DEBUG_ADP
	fprintf(stdout, "%.9f\tASPU::sendDabr()\t\t\t\tin node %d\twith address %s\n", CURRENT_TIME, aspu_->id_, bitString(aspu_->address_));
	printDHT();
#endif
	Packet *sendPkt_ = Packet::alloc();
	struct hdr_cmn *sendPktCh_ = HDR_CMN(sendPkt_);                                                  
	struct hdr_ip *sendPktIh_ = HDR_IP(sendPkt_);
	struct hdr_aspu_dabr *sendPktRh_ = HDR_ASPU_DABR(sendPkt_);
	sendPktCh_->ptype() = PT_ASPU;
	sendPktCh_->size() = IP_HDR_LEN + aspu_->size();
	sendPktCh_->iface() = -2;
	sendPktCh_->error() = 0;
	sendPktCh_->addr_type() = NS_AF_NONE;
	sendPktCh_->next_hop() = IP_BROADCAST;
	sendPktIh_->saddr() = aspu_->id_;
	sendPktIh_->daddr() = IP_BROADCAST;
	sendPktIh_->sport() = RT_PORT;
	sendPktIh_->dport() = RT_PORT;
	sendPktIh_->ttl_ = IP_DEF_TTL;
	sendPktRh_->type_ = ASPU_TYPE_DABR;
	sendPktRh_->srcId_ = aspu_->id_;
	sendPktRh_->srcAdd_ = aspu_->address_;
	sendPktRh_->dstAdd_ = DATYPE_BROADCAST;
#ifdef DEBUG_ADP
	fprintf(stdout, "\tsendPktIh_->saddr = %d\n", sendPktIh_->saddr());
	fprintf(stdout, "\tsendPktIh_->daddr = %d\n", sendPktIh_->daddr());
	fprintf(stdout, "\tsendPktRh_->srcId_ = %d\n", sendPktRh_->srcId_);
	fprintf(stdout, "\tsendPktRh_->srcAdd_ = %s\n", bitString(sendPktRh_->srcAdd_));
	fprintf(stdout, "\tsendPktRh_->dstAdd_ = %s\n", bitString(sendPktRh_->dstAdd_));
#endif
	PacketData* data_ = dht_->getDHTUpdate();
	int size_ = data_->size();
	sendPkt_->setdata(data_);
	sendPktCh_->size() = IP_HDR_LEN + size_ + 2 + aspu_->size();
	if(numinterf) {
		for(int i=0; i<numinterf; i++) {
				Packet *p_copy = p->copy();
				Scheduler::instance().schedule(targetlist[i], p_copy, 0.0);
		}
		Packet::free(p);
	} else {
		Scheduler::instance().schedule(aspu_->getTarget(), sendPkt_, 0.0);
	}
}

void ADP::recvDabr(Packet* recvPkt) {
#ifdef DEBUG_ADP
	fprintf(stdout, "%.9f\tASPU::recvDabr()\t\t\t\tin node %d\twith address %s\n", CURRENT_TIME, aspu_->id_, bitString(aspu_->address_));
	printDHT();
#endif
#ifdef DEBUG_ADP
	struct hdr_aspu_dabr *recvPktRh_ = HDR_ASPU_DABR(recvPkt);
	struct hdr_ip *recvPktIh_ = HDR_IP(recvPkt);
	fprintf(stdout, "\trecvPktIh_->saddr = %d\n", recvPktIh_->saddr());
	fprintf(stdout, "\trecvPktIh_->daddr = %d\n", recvPktIh_->daddr());
	fprintf(stdout, "\trecvPktRh_->srcId_ = %d\n", recvPktRh_->srcId_);
	fprintf(stdout, "\trecvPktRh_->srcAdd_ = %s\n", bitString(recvPktRh_->srcAdd_));
	fprintf(stdout, "\trecvPktRh_->dstAdd_ = %s\n", bitString(recvPktRh_->dstAdd_));
	fprintf(stdout, "\trecvPktRh_->data = %s\n", ((PacketData*)recvPkt->userdata())->data());
	fprintf(stdout, "\trecvPktRh_->data length = %d\n", ((PacketData*)recvPkt->userdata())->size());
#endif
	ostringstream strStream_;
	strStream_ << ((PacketData*)recvPkt->userdata())->data();
	string str_ = strStream_.str();
	string::size_type lastPos_ = str_.find_first_not_of(";", 0);
	string::size_type pos_ = str_.find_first_of(";", lastPos_);
	while (string::npos != pos_ || string::npos != lastPos_) {
		string entry_ = str_.substr(lastPos_, pos_ - lastPos_);
		string::size_type idLastPos_ = entry_.find_first_not_of(",", 0);
		string::size_type idPos_ = entry_.find_first_of(",", idLastPos_);
		nsaddr_t id_  = atoi(entry_.substr(idLastPos_, idPos_ - idLastPos_).c_str());
		string::size_type addressLastPos_ = entry_.find_first_not_of(",", idPos_);
		string::size_type addressPos_ = entry_.find_first_of(",", addressLastPos_);
		nsaddr_t address_  = atoi(entry_.substr(addressLastPos_, addressPos_ - addressLastPos_).c_str());
		string::size_type expireLastPos_ = entry_.find_first_not_of(",", addressPos_);
		string::size_type expirePos_ = entry_.find_first_of(",", expireLastPos_);
		double expire_ = (atof(entry_.substr(expireLastPos_, expirePos_ - expireLastPos_).c_str()));
#ifdef DEBUG_DHT
		fprintf(stdout, "\tid %d\taddress %d\texpire %f\n", id_, address_, expire_);
#endif
		addEntry(id_, address_, expire_);
		lastPos_ = str_.find_first_not_of(";", pos_);
		pos_ = str_.find_first_of(";", lastPos_);
	}
	Packet::free(recvPkt);
#ifdef DEBUG_ADP
	printDHT();
#endif
	if(numinterf) {
		for(int i=0; i<numinterf; i++) {
			Scheduler::instance().schedule(ifqueuelist[i], p_copy, 0.0);
		}
		Packet::free(p);
	} else {
		Scheduler::instance().schedule(aspu_->getTarget(), recvPkt, 0.0);
	}

}

void ADP::addEntry(nsaddr_t id, nsaddr_t address) {
#ifdef DEBUG_ADP
	fprintf(stdout, "%.9f\tADP::addEntry(%d, %s)\t\tin node %d\twith address %s\n", CURRENT_TIME, id, bitString(address), aspu_->id_, bitString(aspu_->address_));
#endif
	dht_->addEntry(id, address);
	Packet* p;
#ifdef DEBUG_ADP
	fprintf(stdout,"\tlooking for packet to %d\n", id);
	atr_->queue_->printQueue();
#endif
	while( (p = aspu_->queue_->deque(id)) ) {
#ifdef DEBUG_ADP
		fprintf(stdout,"\tsending data packet for node %d\n", id);
#endif
		nsaddr_t addr_ = findAdd(id);
		struct hdr_aspu_encp* rh_ = HDR_ASPU_ENCP(p);		
		rh_->dstAdd_ = addr_;
		aspu_->forward(p);
	}
}

void ADP::addEntry(nsaddr_t id, nsaddr_t address, double expire) {
#ifdef DEBUG_ADP
	fprintf(stdout, "%.9f\tADP::addEntry(%d, %s, %f)\t\tin node %d\twith address %s\n", CURRENT_TIME, id, bitString(address), expire, aspu_->id_, bitString(aspu_->address_));
#endif
	dht_->addEntry(id, address, expire);
	Packet* p;
#ifdef DEBUG_ADP
	fprintf(stdout,"\tlooking for packet to %d\n", id);
	aspu_->queue_->printQueue();
#endif
	while( (p = aspu_->queue_->deque(id)) ) {
#ifdef DEBUG_ADP
		fprintf(stdout,"\tsending data packet for node %d\n", id);
#endif
		nsaddr_t addr_ = findAdd(id);
		struct hdr_aspu_encp* rh_ = HDR_ASPU_ENCP(p);		
		rh_->dstAdd_ = addr_;
		aspu_->forward(p);
	}
}

ASPUQueueTimer::ASPUQueueTimer(ASPUQueue* queue) {
	queue_ = queue;
	counter = 0;
	handle((Event*) 0);
}

void ASPUQueueTimer::handle(Event*) {
	if (counter >=3 0)
		queue_->selectPaths(3);
	else
		counter++;
	Scheduler::instance().schedule(this, &intr, ASPU_QUEUE_PURGE_INTERVAL);
}

ASPUQueue::ASPUQueue() {
	queue_ = new queue;
	timer_ = new ASPUQueueTimer(this); 	
}

ASPUQueue::~ASPUQueue() {
	clear();
}

void ASPUQueue::enque(Packet *p) {
#ifdef DEBUG_QUEUE
	fprintf(stdout, "%.9f\tASPUQueue::enque(%d)\n",CURRENT_TIME, HDR_IP(p)->daddr());
	printQueue();
#endif
	struct hdr_cmn* ch_ = HDR_CMN(p);
	struct hdr_ASPU_encp* rh_ = HDR_ASPU_ENCP(p);
	
	if (queue_->size() >= ASPU_QUEUE_LEN)
		removeHead();
#ifdef ADP_DARP_EB
	ch_->ts_ = CURRENT_TIME + ASPU_QUEUE_TIMEOUT;
	rh_->lifetime_ = CURRENT_TIME + ADP_DARQ_EB_TIMEOUT;
	rh_->darqCount_++;
#else
	ch_->ts_ = CURRENT_TIME + ASPU_QUEUE_TIMEOUT;
	rh_->lifetime_ = CURRENT_TIME + ADP_DARQ_TIMEOUT;
#endif	
	queue_->push_back(p);
}

Packet* ASPUQueue::deque() {
#ifdef DEBUG_QUEUE
		fprintf(stdout, "%.9f\tASPUQueue::deque()\n", CURRENT_TIME);
#endif
	queue::iterator entry_ = queue_->begin();
	if (entry_ != queue_->end()) {
#ifdef DEBUG_QUEUE
		fprintf(stdout, "\tfind\tdst=%d\n", HDR_IP(*entry_)->daddr());
#endif
		Packet *p = (*entry_);
		queue_->erase(entry_);
		return p;
	}
	return NULL;
}

Packet* ASPUQueue::deque(nsaddr_t dst) {
#ifdef DEBUG_QUEUE
#endif
	queue::iterator entry_ = queue_->begin();
	while (entry_ != queue_->end()) {
#ifdef DEBUG_QUEUE
		fprintf(stdout, "\t\tqueue packet to %d\n", dst);
#endif			
		if (HDR_IP(*entry_)->daddr() == dst) {
#ifdef DEBUG_QUEUE
			fprintf(stdout, "\tfind\n");
#endif			
			Packet *p = (*entry_);
			queue_->erase(entry_);
			return p;
		}
		entry_++;
	}
	return NULL;
}

bool ASPUQueue::isExpired(int pos) {
#ifdef DEBUG_QUEUE
	fprintf(stdout, "%.9f\tASPUQueue::isExpired(%d)\n", CURRENT_TIME, pos);
#endif
	if (pos <= (int) queue_->size()) {
		struct hdr_ASPU_encp* rh_ = HDR_ASPU_ENCP(queue_->at(pos));
		if (rh_->lifetime_ <= CURRENT_TIME) {
#ifdef DEBUG_QUEUE
			fprintf(stdout, "\texpired \n");
#endif
#ifdef ADP_DARQ_EB
			rh_->lifetime_ = CURRENT_TIME + rh_->darqCount_ * ADP_DARQ_EB_TIMEOUT;
			rh_->darqCount_++;
#else
			rh_->lifetime_ = CURRENT_TIME + ADP_DARQ_TIMEOUT;
#endif
			return true;
		}
	}
	return false;
}

Packet* ASPUQueue::getPacket(int pos) {
#ifdef DEBUG_QUEUE
	fprintf(stdout, "%.9f\tASPUQueue::get(%d)\n", CURRENT_TIME, pos);
#endif
	if (pos <= (int) queue_->size()) {
		return queue_->at(pos);
	}
	return NULL;
}

void ASPUQueue::purge() {
#ifdef DEBUG_QUEUE
	fprintf(stdout, "%.9f\tASPUQueue::purge()\n",CURRENT_TIME);
	printQueue();
#endif
	queue::iterator entry_ = queue_->begin();
	while (entry_ != queue_->end()) {
#ifdef DEBUG_QUEUE
		fprintf(stdout, "\tchecking for %d\n", HDR_IP(*entry_)->daddr());
#endif
		queue::iterator nextEntry_ = entry_;
		nextEntry_++;
		if (HDR_CMN(*entry_)->ts_ <= CURRENT_TIME) {
#ifdef DEBUG_QUEUE
			fprintf(stdout, "\tdropped packet for %d\n", HDR_IP(*entry_)->daddr());
#endif
			drop((*entry_), DROP_RTR_QTIMEOUT);
			queue_->erase(entry_);
		}
		else 
			entry_ = nextEntry_;
	}
#ifdef DEBUG_QUEUE
	printQueue();
#endif

}

void ASPUQueue::printNumPacket(nsaddr_t dst){
	int num = 0;
	queue::iterator entry_;
	
	for(entry_ = queue_->begin(); entry_ != queue_->end(); entry_++) {
		if (HDR_IP(*entry_)->daddr() == dst) {
			num++;
		}
	}
	fprintf(stdout, "\tASPUQueue::numPacket(%d) = %d\n", dst, num);
}

void ASPUQueue::printQueue() {
	queue::iterator entry_;
	fprintf(stdout, "\tASPUQueue::printQueue()\n");
	for(entry_ = queue_->begin(); entry_ != queue_->end(); ++entry_) {
		fprintf(stdout, "\t\tpacket dst = %d\texpire time = %f\n", HDR_IP(*entry_)->daddr(), HDR_CMN(*entry_)->ts_);
	}
}

void ASPUQueue::removeHead() {
#ifdef DEBUG_QUEUE
	fprintf(stdout, "%.9f\tASPUQueue::removeHead()\n", CURRENT_TIME);
	printQueue();
#endif
	queue::iterator entry_ = queue_->begin();
	if (entry_ != queue_->end()) {
		if(HDR_CMN(*entry_)->ts_ <= CURRENT_TIME) {
#ifdef DEBUG_QUEUE
			fprintf(stdout, "\tDROP_RTR_QTIMEOUT\n");
#endif
			drop(*entry_, DROP_RTR_QTIMEOUT);
		}
		else {
#ifdef DEBUG_QUEUE
			fprintf(stdout, "\tDROP_RTR_QFULL\n");
#endif
			drop(*entry_, DROP_RTR_QFULL);
		}
		queue_->erase(entry_);
	}
#ifdef DEBUG_QUEUE
	printQueue();
#endif
}

void ASPUQueue::clear() {
	queue::iterator entry_ = queue_->begin();
	while (entry_ != queue_->end()) {
		queue::iterator nextEntry_ = entry_;
		nextEntry_++;
		queue_->erase(entry_);
		delete (*entry_);
		entry_ = nextEntry_;
	}
}

void NeighborTable::update(string str_) {
#ifdef DEBUG_NEIGHBOR
	fprintf(stdout, "%.9f\tNeighborTable::update()\n", CURRENT_TIME);
	fprintf(stdout, "\tdata = %s\n", str_.c_str());
#endif
	int i=ADDR_SIZE;
	string::size_type lastPos_ = str_.find_first_not_of(";", 0);
	string::size_type pos_ = str_.find_first_of(";", lastPos_);
		while (string::npos != pos_ || string::npos != lastPos_) {
			string entry_ = str_.substr(lastPos_, pos_ - lastPos_);
			string::size_type networkIdLastPos_ = entry_.find_first_not_of(",", 0);
			string::size_type networkIdPos_ = entry_.find_first_of(",", networkIdLastPos_);
			nsaddr_t networkId = atoi(entry_.substr(networkIdLastPos_, networkIdPos_ - networkIdLastPos_).c_str());
			string::size_type hopLastPos_ = entry_.find_first_not_of(",", networkIdPos_);
			string::size_type hopPos_ = entry_.find_first_of(",", hopLastPos_);
			u_int32_t hopNumber = atoi(entry_.substr(hopLastPos_, hopPos_ - hopLastPos_).c_str());
			string::size_type qualityLastPos_ = entry_.find_first_not_of(",", hopPos_);
			string::size_type qualityPos_ = entry_.find_first_of(",", qualityLastPos_);
			double etxMetric = atof(entry_.substr(qualityLastPos_, qualityPos_ - qualityLastPos_).c_str());
			string::size_type routeLogLastPos_ = entry_.find_first_not_of(",", qualityPos_);
			string::size_type routeLogPos_ = entry_.find_first_of(",", routeLogLastPos_);
			bitset<ADDR_SIZE> routeLog(atoi(entry_.substr(routeLogLastPos_, routeLogPos_ - routeLogLastPos_).c_str()));
			i--;
			networkId_[i] = networkId;
			hopNumber_[i] = hopNumber;
			etxMetric_[i] = etxMetric;
			routeLog_[i] = routeLog;
			interface_[i] = interface;
	#ifdef DEBUG_NEIGHBOR
			fprintf(stdout, "\t\tentry %d:\t%d\t%d\t%f\t%s\n", i, networkId_[i], hopNumber_[i], etxMetric_[i], routeLog_[i].to_string().c_str());
	#endif
			lastPos_ = str_.find_first_not_of(";", pos_);
			pos_ = str_.find_first_of(";", lastPos_);
	}
}

void NeighborTable::print(nsaddr_t address_) const {
	unsigned int i;
	for (i=0; i<ADDR_SIZE; i++) {
		if (hopNumber_[i] < INFINITO) {
			bitset<ADDR_SIZE> tempSibling_ (address_);
			tempSibling_.flip(i);
			fprintf(stdout, "\t\t\tlevel sibling = %i\tsibling = %s\tnetworkId = %d\thopNumber = %d\tetxMetric = %f\trouteLog = %s\n", i, tempSibling_.to_string().c_str(), networkId_[i], hopNumber_[i], etxMetric_[i], routeLog_[i].to_string().c_str());

		}
	}
}

Neighbor::Neighbor(nsaddr_t id, nsaddr_t address, u_int32_t helloSeqNum) {
	id_ = id;
	address_ = address;
	expire_ = CURRENT_TIME + NDP_NEIGHBOR_EXPIRE;
	table_ = new NeighborTable();
	forLinkQuality_ = 0;
	addHello(helloSeqNum);
}

void Neighbor::purgeHello() {
#ifdef DEBUG_NEIGHBOR
	fprintf(stdout, "%.9f\tNeighbor::purgeHello()\n",CURRENT_TIME);
#endif
	helloVector::iterator entry_ = helloVector_.begin();
	while (entry_ != helloVector_.end()) {
		helloVector::iterator nextEntry_ = entry_;
		nextEntry_++;
		if ((*entry_).time_ + NDP_NEIGHBOR_EXPIRE <= CURRENT_TIME) {
			helloVector_.erase(entry_);
		}
		else 
			entry_ = nextEntry_;
	}
}

void Neighbor::printHello() {
	helloVector::iterator entry_;
	fprintf(stdout, "\tNeighbor::printHello()\n");
	for(entry_ = helloVector_.begin(); entry_ != helloVector_.end(); ++entry_) {
		fprintf(stdout, "\t\thello num = %d\ttime = %f\n", (*entry_).seqNum_, (*entry_).time_);
	}
}