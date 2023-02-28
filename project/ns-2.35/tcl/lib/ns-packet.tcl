PacketHeaderManager set hdrlen_ 0
PacketHeaderManager set tab_(Common) 1

proc add-packet-header args {
	foreach cl $args {
		PacketHeaderManager set tab_(PacketHeader/$cl) 1
	}
}

proc add-all-packet-headers {} {
	PacketHeaderManager instvar tab_
	foreach cl [PacketHeader info subclass] {
		if [info exists tab_($cl)] { 
			PacketHeaderManager set tab_($cl) 1
		}
	}
}

proc remove-packet-header args {
	foreach cl $args {
		if { $cl == "Common" } {
			warn "Cannot exclude common packet header."
			continue
		}
		PacketHeaderManager unset tab_(PacketHeader/$cl)
	}
}

proc remove-all-packet-headers {} {
	PacketHeaderManager instvar tab_
	foreach cl [PacketHeader info subclass] {
		if { $cl != "PacketHeader/Common" } {
			if [info exists tab_($cl)] { 
				PacketHeaderManager unset tab_($cl)
			}
		}
	}
}

foreach prot {
	Common 
	Flags
	IP 	# IP
	NV 	# NixVector classifier for stateless routing 
	rtProtoDV 	# distance vector routing protocol
	rtProtoLS 	# link state routing protocol
	SR 	# source routing, dsr/hdr_sr.cc
	Src_rt 	# source routing, src_rtg/hdr_src.cc
	LDP 	# mpls/ldp.cc
	MPLS 	# MPLS, MultiProtocol Label Switching
	Resv 	# Token buckets, for reservations.
	UMP 	# Admission control, adc/ump.cc
	Pushback 	# Pushback, router-to-router

  	aSRM 	# mcast/srm.cc
	CtrMcast 	# Centralized Multicast routing
	mcastCtrl 	# mcast/mcast_ctrl.cc
	MFTP 	# Multicast File Transfer Protocol
	PGM 	# PGM multicast
	PGM_SPM # PGM multicast
	PGM_NAK # PGM multicast
  	SRM 	# SRM, multicast
  	SRMEXT 	# SRM, multicast
	HttpInval 	# HTTP
	IVS 	# Inria video conferencing system 
	QS 	# Quick-Start
	RAP 	# Rate Adaption Protocol, transport protocol.
	RTP 	# RTP.  Also used for UPD traffic.
	SCTP 	# SCTP, transport protocol
	Snoop 	# tcp/snoop.cc
	TCP 	# TCP, transport protocol
	TCPA 	# Asymmetric TCP, transport protocol
	TFRC 	# TFRC, transport protocol
	TFRC_ACK 	# TFRC, transport protocol
	XCP 	# XCP, transport protocol
        DCCP            # DCCP, transport protocol
        DCCP_ACK        # DCCP, transport protocol
        DCCP_RESET      # DCCP, transport protocol
        DCCP_REQ        # DCCP, transport protocol
        DCCP_RESP       # DCCP, transport protocol
        DCCP_DATA       # DCCP, transport protocol
        DCCP_DATAACK    # DCCP, transport protocol
        DCCP_CLOSE      # DCCP, transport protocol
        DCCP_CLOSEREQ   # DCCP, transport protocol
	Message # a protocol to carry text messages
	Ping 	# Ping
    PBC     # PBC
	ARP 	# Address Resolution Protocol, network wireless stack
	GAF 	# Geographic Adaptive Delity, for ad-hoc networks
	LL 	# network wireless stack
        LRWPAN  # zheng, wpan/p802_15_4mac.cc
	Mac 	# network wireless stack
	AODV 	# routing protocol for ad-hoc networks
	Diffusion 	# diffusion/diffusion.cc
	IMEP 	# Internet MANET Encapsulation Protocol, for ad-hoc networks
    MIP 	# Mobile IP, mobile/mip-reg.cc
	Smac 	# Sensor-MAC
	TORA 	# routing protocol for ad-hoc networks
	AOMDV   # routing protocol for ad-hoc networks
	MDART 	# routing protocol for ad-hoc networks
	ASPU    # routing protocol for ad-hoc networks
	Encap 	# common/encap.cc
    IPinIP 	# IP encapsulation 
	HDLC 	# High Level Data Link Control
} {
	add-packet-header $prot
}

proc PktHdr_offset { hdrName {field ""} } {
	set offset [$hdrName offset]
	if { $field != "" } {
		incr offset [$hdrName set offset_($field)]
	}
	return $offset
}

Simulator instproc create_packetformat { } {
	PacketHeaderManager instvar tab_
	set pm [new PacketHeaderManager]
	foreach cl [PacketHeader info subclass] {
		if [info exists tab_($cl)] {
			set off [$pm allochdr $cl]
			$cl offset $off
		}
	}
	$self set packetManager_ $pm
}

PacketHeaderManager instproc allochdr cl {
	set size [$cl set hdrlen_]

	$self instvar hdrlen_
	set NS_ALIGN 8
	set incr [expr ($size + ($NS_ALIGN-1)) & ~($NS_ALIGN-1)]
	set base $hdrlen_
	incr hdrlen_ $incr

	return $base
}