#include <object.h>
#include <queue.h>
#include <drop-tail.h>
#include <packet.h>
#include <cmu-trace.h>
#include "priqueue.h"
typedef int (*PacketFilter)(Packet *, void *);
PriQueue_List PriQueue::prhead = { 0 };

static class PriQueueClass : public TclClass {
public:
  PriQueueClass() : TclClass("Queue/DropTail/PriQueue") {}
  TclObject* create(int, const char*const*) {
    return (new PriQueue);
  }
} class_PriQueue;

PriQueue::PriQueue() : DropTail()
{
        bind("Prefer_Routing_Protocols", &Prefer_Routing_Protocols);
	LIST_INSERT_HEAD(&prhead, this, link);
}

int
PriQueue::command(int argc, const char*const* argv)
{
  if (argc == 2 && strcasecmp(argv[1], "reset") == 0)
    {
      Terminate();
    }
  return DropTail::command(argc, argv);
}

void
PriQueue::recv(Packet *p, Handler *h)
{
        struct hdr_cmn *ch = HDR_CMN(p);

        if(Prefer_Routing_Protocols) {
                switch(ch->ptype()) {
		case PT_DSR:
		case PT_MESSAGE:
        case PT_TORA:
        case PT_AODV:
		case PT_AOMDV:
		case PT_MDART:
		case PT_ASPU:
			recvHighPriority(p, h);
                        break;
                default:
                        Queue::recv(p, h);
                }
        }
        else {
                Queue::recv(p, h);
        }
}

void 
PriQueue::recvHighPriority(Packet *p, Handler *)
{
	q_->enqueHead(p);
	if (q_->length() >= qlim_)
    {
      Packet *to_drop = q_->lookup(q_->length()-1);
      q_->remove(to_drop);
      drop(to_drop);
    }
  
  if (!blocked_) {
    p = deque();
    if (p != 0) {
      blocked_ = 1;
      target_->recv(p, &qh_);
    }
  } 
}
 
void 
PriQueue::filter(PacketFilter filter, void * data)
{
  int i = 0;
  while (i < q_->length())
    {
      Packet *p = q_->lookup(i);
      if (filter(p,data))
	{
	  q_->remove(p);
	}
      else i++;
    }
}

Packet*
PriQueue::filter(nsaddr_t id)
{
	Packet *p = 0;
	Packet *pp = 0;
	struct hdr_cmn *ch;

	for(p = q_->head(); p; p = p->next_) {
		ch = HDR_CMN(p);
		if(ch->next_hop() == id)
			break;
		pp = p;
	}
	if(p) {
		if(pp == 0)
			q_->remove(p);
		else
			q_->remove(p, pp);
	}
	return p;
}

void
PriQueue::Terminate()
{
	Packet *p;
	while((p = deque())) {
		drop(p, DROP_END_OF_SIMULATION);
	}
}