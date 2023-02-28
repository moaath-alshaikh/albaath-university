Simulator set useasim_ 1
Asim set debug_ false
set MAXSEQ 1073741824
set tcl_precision 17
Connector set debug_ false
TTLChecker set debug_ false
Trace set src_ -1
Trace set dst_ -1
Trace set callback_ 0
Trace set show_tcphdr_ 0
Trace set show_sctphdr_ 0
Trace set debug_ false
CMUTrace set debug_ false
CMUTrace set show_sctphdr_ 0
CMUTrace set radius_scaling_factor_ 1.0
CMUTrace set duration_scaling_factor_ 3.0e4
Scheduler/RealTime set maxslop_ 0.010; 
Scheduler/Calendar set adjust_new_width_interval_ 10;	
Scheduler/Calendar set min_bin_width_ 1e-18;
Integrator set lastx_ 0.0
Integrator set lasty_ 0.0
Integrator set sum_ 0.0
Queue set limit_ 50
Queue set blocked_ false
Queue set unblock_on_resume_ true
Queue set interleave_ false
Queue set acksfirst_ false
Queue set ackfromfront_ false
Queue set debug_ false
Queue/SFQ set maxqueue_ 40
Queue/SFQ set buckets_ 16
Queue/FQ set secsPerByte_ 0
FQLink set queueManagement_ DropTail
Queue/DropTail set drop_front_ false
Queue/DropTail set summarystats_ false
Queue/DropTail set queue_in_bytes_ false
Queue/DropTail set mean_pktsize_ 500
Queue/DropTail/PriQueue set Prefer_Routing_Protocols    1
CMUPriQueue set qlen_logthresh_ 10
CMUPriQueue set fw_logthresh_ 25
CMUPriQueue set debug_ false
Queue/dsRED set numQueues_ 4
Queue/dsRED set ecn_ 0
Queue/XCP set tcp_xcp_on_ 0  ;
Queue/RED set bytes_ true ;	
Queue/RED set queue_in_bytes_ true ;
Queue/RED set thresh_ 0
Queue/RED set maxthresh_ 0
Queue/RED set thresh_queue_ [Queue set limit_]
Queue/RED set maxthresh_queue_ [Queue set limit_]
Queue/RED set mean_pktsize_ 500
Queue/RED set idle_pktsize_ 100
Queue/RED set q_weight_ -1
Queue/RED set wait_ true
Queue/RED set linterm_ 10
Queue/RED set mark_p_ 0.1
Queue/RED set use_mark_p_ true
Queue/RED set setbit_ false
Queue/RED set gentle_ true
Queue/RED set drop_tail_ true
Queue/RED set drop_front_ false
Queue/RED set drop_rand_ false
Queue/RED set doubleq_ false
Queue/RED set ns1_compat_ false
Queue/RED set dqthresh_ 50
Queue/RED set ave_ 0.0
Queue/RED set prob1_ 0.0
Queue/RED set curq_ 0
Queue/RED set cur_max_p_ 0
Queue/RED set summarystats_ false
Queue/RED set alpha_ 0.01
Queue/RED set beta_ 0.9 
Queue/RED set adaptive_ 0
Queue/RED set interval_ 0.5
Queue/RED set targetdelay_ 0.005
Queue/RED set top_ 0.5
Queue/RED set bottom_ 0
Queue/RED set cautious_ 0
Queue/RED set feng_adaptive_ 0
Queue/RED/RIO set bytes_ false
Queue/RED/RIO set queue_in_bytes_ false
Queue/RED/RIO set thresh_ 5
Queue/RED/RIO set maxthresh_ 15
Queue/RED/RIO set in_thresh_ 15
Queue/RED/RIO set in_maxthresh_ 30
Queue/RED/RIO set out_thresh_ 5
Queue/RED/RIO set out_maxthresh_ 15
Queue/RED/RIO set mean_pktsize_ 500
Queue/RED/RIO set q_weight_ 0.002
Queue/RED/RIO set wait_ true
Queue/RED/RIO set linterm_ 10
Queue/RED/RIO set in_linterm_ 50
Queue/RED/RIO set out_linterm_ 5
Queue/RED/RIO set setbit_ false
Queue/RED/RIO set gentle_ false
Queue/RED/RIO set in_gentle_ false
Queue/RED/RIO set out_gentle_ false
Queue/RED/RIO set drop_tail_ true
Queue/RED/RIO set drop_front_ false
Queue/RED/RIO set drop_rand_ false
Queue/RED/RIO set doubleq_ false
Queue/RED/RIO set ns1_compat_ false
Queue/RED/RIO set dqthresh_ 50
Queue/RED/RIO set ave_ 0.0
Queue/RED/RIO set in_ave_ 0.0
Queue/RED/RIO set out_ave_ 0.0
Queue/RED/RIO set prob1_ 0.0
Queue/RED/RIO set in_prob1_ 0.0
Queue/RED/RIO set out_prob1_ 0.0
Queue/RED/RIO set curq_ 0
Queue/RED/RIO set priority_method_ 0
Queue/RED/PD set auto_ false
Queue/RED/PD set global_target_ false
Queue/RED/PD set noMonitored_ 0
Queue/RED/PD set targetBW_ 0
Queue/RED/PD set unresponsive_penalty_ 1
Queue/RED/PD set P_testFRp_ -1
Queue/RED/PD set noidle_ false
Queue/PI set bytes_ false
Queue/PI set queue_in_bytes_ false
Queue/PI set a_ 0.00001822
Queue/PI set b_ 0.00001816
Queue/PI set w_ 170
Queue/PI set qref_ 50
Queue/PI set mean_pktsize_ 500
Queue/PI set setbit_ false
Queue/PI set prob_ 0
Queue/PI set curq_ 0
Queue/Vq set queue_in_bytes_ true
Queue/Vq set markpkts_ false 
Queue/Vq set ecnlim_ 0.8
Queue/Vq set buflim_ 1.0 
Queue/Vq set gamma_ 0.98
Queue/Vq set mean_pktsize_ 1000
Queue/Vq set curq_ 0
Queue/Vq set drop_front_ 0
Queue/Vq set markfront_ 0
Queue/REM set gamma_ 0.001
Queue/REM set phi_ 1.001
Queue/REM set inw_ 1
Queue/REM set mean_pktsize_ 1000
Queue/REM set pupdtime_ 0.002
Queue/REM set pbo_ 20.0
Queue/REM set prob_ 0.0
Queue/REM set curq_ 0.0
Queue/REM set pmark_ 0.0
Queue/REM set markpkts_ false
Queue/REM set qib_ false
Queue/GK set ecnlim_ 0.95
Queue/GK set mean_pktsize_ 1000
Queue/GK set curq_ 0
Queue/GK set drop_front_ 0
Queue/DRR set buckets_ 10
Queue/DRR set blimit_ 25000
Queue/DRR set quantum_ 250
Queue/DRR set mask_ 0
Queue/SRR set maxqueuenumber_ 16
Queue/SRR set mtu_ 1000
Queue/SRR set granularity_ 1000
Queue/SRR set blimit_ 25000
Queue/CBQ set algorithm_ 0 ;
Queue/CBQ set maxpkt_ 1024
CBQClass set priority_ 0
CBQClass set level_ 1
CBQClass set extradelay_ 0.0
CBQClass set def_qtype_ DropTail
CBQClass set okborrow_ true
CBQClass set automaxidle_gain_ 0.9375
CBQClass set debug_ false
SnoopQueue/In set debug_ false
SnoopQueue/Out set debug_ false
SnoopQueue/Drop set debug_ false
SnoopQueue/EDrop set debug_ false
SnoopQueue/Tagger set debug_ false
PacketQueue/Semantic set acksfirst_ false
PacketQueue/Semantic set filteracks_ false
PacketQueue/Semantic set replace_head_ false
PacketQueue/Semantic set priority_drop_ false
PacketQueue/Semantic set random_drop_ false
PacketQueue/Semantic set reconsAcks_ false
PacketQueue/Semantic set random_ecn_ false
QueueMonitor set size_ 0
QueueMonitor set pkts_ 0
QueueMonitor set parrivals_ 0
QueueMonitor set barrivals_ 0
QueueMonitor set pdepartures_ 0
QueueMonitor set bdepartures_ 0
QueueMonitor set pdrops_ 0
QueueMonitor set pmarks_ 0
QueueMonitor set bdrops_ 0
QueueMonitor set qs_pkts_ 0
QueueMonitor set qs_bytes_ 0
QueueMonitor set qs_drops_ 0
QueueMonitor set first_pkt_ 0
QueueMonitor set last_pkt_ 0
QueueMonitor set keepRTTstats_ 0
QueueMonitor set maxRTT_ 5
QueueMonitor set binsPerSec_ 100
QueueMonitor set keepSeqnoStats_ 0
QueueMonitor set maxSeqno_ 1000
QueueMonitor set SeqnoBinSize_ 10
QueueMonitor set k_ 0.1
QueueMonitor set prevTime_ 0
QueueMonitor set startTime_ 0
QueueMonitor set estRate_ 0
QueueMonitor set estimate_rate_ 0
QueueMonitor/ED set epdrops_ 0
QueueMonitor/ED set ebdrops_ 0
QueueMonitor/ED set mon_epdrops_ 0                     
QueueMonitor/ED set mon_ebdrops_ 0
QueueMonitor/ED/Flowmon set enable_in_ true
QueueMonitor/ED/Flowmon set enable_out_ true
QueueMonitor/ED/Flowmon set enable_drop_ true
QueueMonitor/ED/Flowmon set enable_edrop_ true
QueueMonitor/ED/Flowmon set enable_mon_edrop_ true
QueueMonitor/ED/Flow set src_ -1
QueueMonitor/ED/Flow set dst_ -1
QueueMonitor/ED/Flow set flowid_ -1
QueueMonitor/ED/Flow/TB set target_rate_ 128000 
QueueMonitor/ED/Flow/TB set bucket_depth_ 10000
QueueMonitor/ED/Flow/TB set tbucket_ 10000
QueueMonitor/ED/Flow/TSW set target_rate_ 0
QueueMonitor/ED/Flow/TSW set win_len_ 10
QueueMonitor/ED/Flow/TSW set wait_ true
QueueMonitor/ED/Flow/RedPD set targetBW_ 0
QueueMonitor/ED/Flow/RedPD set currentBW_ 0
QueueMonitor/ED/Flow/RedPD set monitored_ 0
QueueMonitor/ED/Flow/RedPD set unresponsive_ 0
QueueMonitor/ED/Flow/RedPD set monitorStartTime_ 0
QueueMonitor/ED/Flow/RedPD set unresponsiveStartTime_ 0
QueueMonitor/ED/Flow/RedPD set lastDropTime_ 0 
QueueMonitor/ED/Flow/RedPD set auto_ 0 
DelayLink set bandwidth_ 1.5Mb
DelayLink set delay_ 100ms
DelayLink set debug_ false
DelayLink set avoidReordering_ false ;	
DynamicLink set status_ 1
DynamicLink set debug_ false
Filter set debug_ false
Filter/Field set offset_ 0
Filter/Field set match_  -1
Classifier set offset_ 0
Classifier set shift_ 0
Classifier set mask_ 0xffffffff
Classifier set debug_ false
Classifier/Hash set default_ -1; 
Classifier/Replicator set ignore_ 0
Classifier/Addr/MPLS set ttl_   32
Classifier/Addr/MPLS set trace_mpls_ 0
Classifier/Addr/MPLS set label_ -1
Classifier/Addr/MPLS set enable_reroute_    0
Classifier/Addr/MPLS set reroute_option_ 0
Classifier/Addr/MPLS set control_driven_ 0
Classifier/Addr/MPLS set data_driven_ 0
FECModel set debug_ false
ErrorModule set debug_ false
ErrorModel set enable_ 1
ErrorModel set markecn_ false
ErrorModel set delay_pkt_ false
ErrorModel set delay_ 0
ErrorModel set rate_ 0
ErrorModel set bandwidth_ 2Mb
ErrorModel set debug_ false
ErrorModel/Trace set good_ 123456789
ErrorModel/Trace set loss_ 0
ErrorModel/Periodic set period_ 1.0
ErrorModel/Periodic set offset_ 0.0
ErrorModel/Periodic set burstlen_ 0.0
ErrorModel/Periodic set default_drop_ 0 ;
ErrorModel/MultiState set curperiod_ 0.0
ErrorModel/MultiState set sttype_ pkt
ErrorModel/MultiState set texpired_ 0
SelectErrorModel set enable_ 1
SelectErrorModel set markecn_ false
SelectErrorModel set rate_ 0
SelectErrorModel set bandwidth_ 2Mb
SelectErrorModel set pkt_type_ 2
SelectErrorModel set drop_cycle_ 10
SelectErrorModel set drop_offset_ 1
SelectErrorModel set debug_ false
SelectErrorModel set delay_pkt_ false
SelectErrorModel set delay_ 0
SRMErrorModel set enable_ 1
SRMErrorModel set markecn_ false
SRMErrorModel set rate_ 0
SRMErrorModel set bandwidth_ 2Mb
SRMErrorModel set pkt_type_ 2
SRMErrorModel set drop_cycle_ 10
SRMErrorModel set drop_offset_ 1
SRMErrorModel set debug_ false
SRMErrorModel set delay_pkt_ false
SRMErrorModel set delay_ 0
rtModel set startTime_ 0.5
rtModel set finishTime_ "-"
rtModel/Exponential set upInterval_   10.0
rtModel/Exponential set downInterval_  1.0
rtModel/Deterministic set upInterval_   2.0
rtModel/Deterministic set downInterval_ 1.0
Application/Traffic/CBR_PP set rate_ 448Kb ;
Application/Traffic/CBR_PP set packetSize_ 210
Application/Traffic/CBR_PP set random_ 0
Application/Traffic/CBR_PP set maxpkts_ 268435456; 
Application/Traffic/CBR_PP set PBM_ 2
Application/Traffic/Exponential set burst_time_ .5
Application/Traffic/Exponential set idle_time_ .5
Application/Traffic/Exponential set rate_ 64Kb
Application/Traffic/Exponential set packetSize_ 210
Application/Traffic/Pareto set burst_time_ 500ms
Application/Traffic/Pareto set idle_time_ 500ms
Application/Traffic/Pareto set rate_ 64Kb
Application/Traffic/Pareto set packetSize_ 210
Application/Traffic/Pareto set shape_ 1.5
Application/Traffic/RealAudio set burst_time_ 0.05ms
Application/Traffic/RealAudio set idle_time_ 1800ms
Application/Traffic/RealAudio set rate_ 2Kb
Application/Traffic/RealAudio set packetSize_ 240
Application/Traffic/RealAudio set minCDF_ 0
Application/Traffic/RealAudio set maxCDF_ 1
Application/Traffic/RealAudio set interpolation_ 0
Application/Traffic/RealAudio set maxEntry_ 32
Application/Traffic/CBR set rate_ 448Kb	;
Application/Traffic/CBR set packetSize_ 210
Application/Traffic/CBR set random_ 0
Application/Traffic/CBR set maxpkts_ 268435456; 
Application/Telnet set interval_ 1.0
Application/Worm set ScanRate 4000
Application/Worm set ScanPort 1434
Application/Worm set ScanPacketSize 404
Application/Worm/An set TimeStep 1
Application/SctpApp1 set interval_ 1.0
Application/SctpApp1 set numStreams_ 1
Application/SctpApp1 set numUnreliable_ 0
Application/SctpApp1 set reliability_ 0
RandomVariable/Uniform set min_ 0.0
RandomVariable/Uniform set max_ 1.0
RandomVariable/Exponential set avg_ 1.0
RandomVariable/Erlang set lambda_ 1.0
RandomVariable/Erlang set k_ 1.0
RandomVariable/Gamma set alpha_ 1.0
RandomVariable/Gamma set beta_ 1.0
RandomVariable/Pareto set avg_ 1.0
RandomVariable/Pareto set shape_ 1.5
RandomVariable/ParetoII set avg_ 10.0
RandomVariable/ParetoII set shape_ 1.2
RandomVariable/Constant set val_ 1.0
RandomVariable/HyperExponential set avg_ 1.0
RandomVariable/HyperExponential set cov_ 4.0
RandomVariable/Empirical set minCDF_ 0
RandomVariable/Empirical set maxCDF_ 1
RandomVariable/Empirical set interpolation_ 0
RandomVariable/Empirical set maxEntry_ 32
RandomVariable/Normal set avg_ 0.0
RandomVariable/Normal set std_ 1.0
RandomVariable/LogNormal set avg_ 1.0
RandomVariable/LogNormal set std_ 1.0
RandomVariable/Weibull set scale_ 1.0
RandomVariable/Weibull set shape_ 1.0
ADC/MS set debug_ false
ADC/HB set debug_ false
ADC/Param set debug_ false
ADC/ACTP set debug_ false
ADC/ACTO set debug_ false
Est/Null set debug_ false
Est/TimeWindow set debug_ false
Est/ExpAvg set debug_ false
Est/PointSample set debug_ false
MeasureMod set debug_ false
SALink set debug_ false
Node set multiPath_ 0
Node set rtagent_port_ 255
Node set DIFFUSION_APP_PORT 254
Node/MobileNode set X_				0
Node/MobileNode set Y_				0
Node/MobileNode set Z_				0
Node/MobileNode set speed_				0
Node/MobileNode set position_update_interval_	0
Node/MobileNode set bandwidth_			0	;
Node/MobileNode set delay_				0	;
Node/MobileNode set REGAGENT_PORT 0
Node/MobileNode set DECAP_PORT 1
AddrParams set ALL_BITS_SET 0x7fffffff
AddrParams PortShift 0
AddrParams PortMask [AddrParams set ALL_BITS_SET]
AddrParams set domain_num_ 1
AddrParams set def_clusters 4
AddrParams set def_nodes 5
AllocAddrBits set DEFADDRSIZE_ 31
AllocAddrBits set MAXADDRSIZE_ 31                
Simulator set node_factory_ Node
Simulator set nsv1flag 0
Simulator set mobile_ip_ 0			 ;
Simulator set routingAgent_ ""
Simulator set addressType_   ""
Simulator set MovementTrace_ OFF
Simulator set EotTrace_ OFF
Simulator set IMEPFlag_ ""
Simulator set WirelessNewTrace_ 0
Simulator set propInstCreated_ 0
SessionSim set rc_ 0
Simulator set McastBaseAddr_ 0x40000000
Simulator set McastAddr_ 0x40000000
Simulator set AgentTrace_ ON
Simulator set RouterTrace_ OFF
Simulator set MacTrace_   OFF
Simulator set PhyTrace_   OFF
Simulator set TaggedTrace_ OFF
Simulator set rtAgentFunction_ ""

SessionHelper set rc_ 0                      ;
SessionHelper set debug_ false

NetworkInterface set debug_ false
TBF set rate_ 64k
TBF set bucket_ 1024
TBF set qlen_ 0
MIPEncapsulator set addr_ 0
MIPEncapsulator set port_ 0
MIPEncapsulator set shift_ 0
MIPEncapsulator set mask_ [AddrParams set ALL_BITS_SET]
MIPEncapsulator set ttl_ 32
MIPEncapsulator set debug_ false
GAFPartner set addr_ 0
GAFPartner set port_ 254
GAFPartner set shift_ 0
GAFPartner set mask_ [AddrParams set ALL_BITS_SET]
GAFPartner set debug_ false                  
Mac set debug_ false
ARPTable set debug_ false
ARPTable set avoidReordering_ false ; 
God set debug_ false
Mac/Tdma set slot_packet_len_	1500
Mac/Tdma set max_node_num_	64
LL set mindelay_                50us
LL set delay_                   25us
LL set bandwidth_               0       ;
LL set debug_ false
LL set avoidReordering_ false ;	
Snoop set debug_ false
 Mac/802_11 set CWMin_         31
 Mac/802_11 set CWMax_         1023
 Mac/802_11 set SlotTime_      0.000020        ;
 Mac/802_11 set SIFS_          0.000010        ;
 Mac/802_11 set PreambleLength_        144             ;
 Mac/802_11 set PLCPHeaderLength_      48              ;
 Mac/802_11 set PLCPDataRate_  1.0e6           ;
 Mac/802_11 set RTSThreshold_  0               ;
 Mac/802_11 set ShortRetryLimit_       7               ;
 Mac/802_11 set LongRetryLimit_        4               ;
 Mac/802_11 set bugFix_timer_ true;    
 Mac/802_11 set BeaconInterval_	       0.1		;	
 Mac/802_11 set ScanType_	PASSIVE
 Mac/802_11 set ProbeDelay_	0.0001		;
 Mac/802_11 set MaxChannelTime_ 0.011		;
 Mac/802_11 set MinChannelTime_ 0.005		; 
 Mac/802_11 set ChannelTime_ 0.12		;
Mac/802_11Ext set HeaderDuration_   0.000020  ;
Mac/802_11Ext set SymbolDuration_   0.000004  ;
Mac/802_11Ext set BasicModulationScheme_ 0    ;
Mac/802_11Ext set use_802_11a_flag_	true
Mac/802_11Ext set CWMin_            15
Mac/802_11Ext set CWMax_            1023
Mac/802_11Ext set SlotTime_         0.000009
Mac/802_11Ext set SIFS_             0.000016
Mac/802_11Ext set RTSThreshold_     3000
Mac/802_11Ext set ShortRetryLimit_  7
Mac/802_11Ext set LongRetryLimit_   4
Mac/802_11Ext set MAC_DBG           0
Classifier/Replicator set direction_ false
Mac set abstract_ false
Mac/802_3 set trace_ false
Mac/SMAC set syncFlag_ 1
Mac/SMAC set selfConfigFlag_ 1
Mac/SMAC set dutyCycle_ 10                                                                                                                
Antenna/OmniAntenna set X_ 0
Antenna/OmniAntenna set Y_ 0
Antenna/OmniAntenna set Z_ 1.5 
Antenna/OmniAntenna set Gt_ 1.0
Antenna/OmniAntenna set Gr_ 1.0
Phy set debug_ false
Phy/WirelessPhy set CPThresh_ 10.0
Phy/WirelessPhy set CSThresh_ 1.559e-11
Phy/WirelessPhy set RXThresh_ 3.652e-10
Phy/WirelessPhy set bandwidth_ 2e6
Phy/WirelessPhy set Pt_ 0.28183815
Phy/WirelessPhy set freq_ 914e+6
Phy/WirelessPhy set L_ 1.0  
Phy/WirelessPhyExt set CSThresh_ 6.30957e-12           ;
Phy/WirelessPhyExt set noise_floor_ 7.96159e-14        ;
Phy/WirelessPhyExt set PowerMonitorThresh_ 2.653e-14   ;
Phy/WirelessPhyExt set Pt_  0.1
Phy/WirelessPhyExt set freq_ 5.18e+9                   ;
Phy/WirelessPhyExt set HeaderDuration_   0.000020      ;
Phy/WirelessPhyExt set BasicModulationScheme_ 0        ;
Phy/WirelessPhyExt set L_ 1.0                          ;
Phy/WirelessPhyExt set PreambleCaptureSwitch_ 1
Phy/WirelessPhyExt set DataCaptureSwitch_ 0
Phy/WirelessPhyExt set SINR_PreambleCapture_ 3.1623;   ;
Phy/WirelessPhyExt set SINR_DataCapture_ 10.0;         ;
Phy/WirelessPhyExt set trace_dist_ 1e6                 ;
Phy/WirelessPhyExt set PHY_DBG_ 0
Phy/WirelessPhyExt set CPThresh_ 0 ;
Phy/WirelessPhyExt set RXThresh_ 0 ;
Phy/WiredPhy set bandwidth_ 10e6
Propagation/Shadowing set pathlossExp_ 2.0
Propagation/Shadowing set std_db_ 4.0
Propagation/Shadowing set dist0_ 1.0
Propagation/Shadowing set seed_ 0
Propagation/Nakagami set gamma0_ 1.9
Propagation/Nakagami set gamma1_ 3.8
Propagation/Nakagami set gamma2_ 3.8
Propagation/Nakagami set d0_gamma_ 200
Propagation/Nakagami set d1_gamma_ 500
Propagation/Nakagami set use_nakagami_dist_ false
Propagation/Nakagami set m0_  1.5
Propagation/Nakagami set m1_  0.75
Propagation/Nakagami set m2_  0.75
Propagation/Nakagami set d0_m_ 80
Propagation/Nakagami set d1_m_ 200
Mac/SMAC set syncFlag_ 0
Agent set fid_ 0
Agent set prio_ 0
Agent set agent_addr_ -1
Agent set agent_port_ -1
Agent set dst_addr_ -1
Agent set dst_port_ -1
Agent set flags_ 0
Agent set ttl_ 32 ; # arbitrary choice here
Agent set debug_ false
Agent set class_ 0
Agent/Ping set packetSize_ 64
Agent/UDP set packetSize_ 1000
Agent/UDP instproc done {} { }
Agent/UDP instproc process_data {from data} { }
Agent/SCTP set debugMask_ 0             ;
Agent/SCTP set debugFileIndex_ -1       ;
Agent/SCTP set associationMaxRetrans_ 10;
Agent/SCTP set pathMaxRetrans_ 5        ;
Agent/SCTP set changePrimaryThresh_ -1  ;
Agent/SCTP set maxInitRetransmits_ 8    ;
Agent/SCTP set heartbeatInterval_ 30    ;
Agent/SCTP set mtu_ 1500                ;
Agent/SCTP set initialRwnd_ 65536       ;
Agent/SCTP set initialSsthresh_ 65536   ;
Agent/SCTP set initialCwnd_ 2           ;
Agent/SCTP set initialRto_ 3.0          ;    
Agent/SCTP set minRto_ 1.0              ;        
Agent/SCTP set maxRto_ 60.0             ;         
Agent/SCTP set fastRtxTrigger_ 3        ;
Agent/SCTP set numOutStreams_ 1         ;
Agent/SCTP set numUnrelStreams_ 0       ;
Agent/SCTP set reliability_ 0           ;
Agent/SCTP set unordered_ 0             ;
Agent/SCTP set ipHeaderSize_ 20         ;
Agent/SCTP set dataChunkSize_ 1468      ;
Agent/SCTP set useDelayedSacks_ 1       ;
Agent/SCTP set sackDelay_ 0.200         ;
Agent/SCTP set useMaxBurst_ 1           ;
Agent/SCTP set rtxToAlt_ 1              ;
Agent/SCTP set dormantAction_ 0		;
Agent/SCTP set initialSwnd_ 0          ;
Agent/SCTP set useNonRenegSacks_ 0     ;
Agent/SCTP set routeCalcDelay_ 0        ;         
Agent/SCTP set routeCacheLifetime_ 1.2  ;
Agent/SCTP set trace_all_ 0             ;
Agent/SCTP set cwnd_ 0                 ; 
Agent/SCTP set rwnd_ 0                 ; 
Agent/SCTP set rto_ 0                  ;
Agent/SCTP set errorCount_ 0           ;
Agent/SCTP set frCount_ 0              ;                                      
Agent/SCTP set timeoutCount_ 0         ;                                      
Agent/SCTP set rcdCount_ 0             ;   
Agent/SCTP/MultipleFastRtx set mfrCount_ 0                                    
Agent/SCTP/MfrTimestamp set mfrCount_ 0    
Agent/SCTP/CMT set useCmtReordering_ 1  ;
Agent/SCTP/CMT set useCmtCwnd_ 1        ;
Agent/SCTP/CMT set useCmtDelAck_ 1      ;
Agent/SCTP/CMT set eCmtRtxPolicy_ 4     ;
Agent/SCTP/CMT set useCmtPF_ 1          ;
Agent/SCTP/CMT set cmtPFCwnd_ 2	        ;
Agent/SCTP/CMT set countPFToActiveNewData_ 0 ;
Agent/SCTP/CMT set countPFToActiveRtxms_ 0;   
Agent/TCP set seqno_ 0
Agent/TCP set t_seqno_ 0
Agent/TCP set maxburst_ 0
Agent/TCP set aggressive_maxburst_ 1 ;  
Agent/TCP set maxcwnd_ 0
Agent/TCP set numdupacks_ 3
Agent/TCP set numdupacksFrac_ -1 ;	
Agent/TCP set exitFastRetrans_ true ;	
Agent/TCP set window_ 20
Agent/TCP set windowInit_ 2 ;		
Agent/TCP set windowInitOption_ 1
Agent/TCP set syn_ true ;		
Agent/TCP set max_connects_ -1 ;	
Agent/TCP set windowOption_ 1
Agent/TCP set windowConstant_ 4
Agent/TCP set windowThresh_ 0.002
Agent/TCP set decrease_num_ 0.5
Agent/TCP set increase_num_ 1.0
Agent/TCP set k_parameter_ 0.0 ;
Agent/TCP set l_parameter_ 1.0 ; 
Agent/TCP set overhead_ 0
Agent/TCP set ecn_ 0
Agent/TCP set old_ecn_ 0
Agent/TCP set bugfix_ss_ 1 ;		
Agent/TCP set packetSize_ 1000
Agent/TCP set tcpip_base_hdr_size_ 40
Agent/TCP set ts_option_size_ 10; 
Agent/TCP set bugFix_ true
Agent/TCP set bugFix_ack_ false ;      
Agent/TCP set bugFix_ts_ false ;
Agent/TCP set lessCareful_ false ;	
Agent/TCP set timestamps_ false
Agent/TCP set ts_resetRTO_ false ;	
Agent/TCP set slow_start_restart_ true
Agent/TCP set restart_bugfix_ true
Agent/TCP set tcpTick_ 0.01 ;		
Agent/TCP set maxrto_ 60 ; 		
Agent/TCP set minrto_ 0.2 ;		
Agent/TCP set srtt_init_ 0
Agent/TCP set rttvar_init_ 12
Agent/TCP set rtxcur_init_ 3.0 ;	
Agent/TCP set T_SRTT_BITS 3
Agent/TCP set T_RTTVAR_BITS 2
Agent/TCP set rttvar_exp_ 2
Agent/TCP set updated_rttvar_ true ;	
Agent/TCP set timerfix_ true ; 	
Agent/TCP set rfc2988_ true ;		
Agent/TCP instproc done {} { }
Agent/TCP set noFastRetrans_ false
Agent/TCP set partial_ack_ false ;	
Agent/TCP set dupacks_ 0
Agent/TCP set ack_ 0
Agent/TCP set cwnd_ 0
Agent/TCP set awnd_ 0
Agent/TCP set ssthresh_ 0
Agent/TCP set rtt_ 0
Agent/TCP set srtt_ 0
Agent/TCP set rttvar_ 0
Agent/TCP set backoff_ 0
Agent/TCP set maxseq_ 0
Agent/TCP set singledup_ 1 ;		
Agent/TCP set LimTransmitFix_ false ;	
Agent/TCP set precisionReduce_ true ;	
Agent/TCP set oldCode_ false
Agent/TCP set useHeaders_ true ;	
Agent/TCP set low_window_ 38 ;		
Agent/TCP set high_window_ 83000
Agent/TCP set high_p_ 0.0000001
Agent/TCP set high_decrease_ 0.1
Agent/TCP set max_ssthresh_ 0
Agent/TCP set cwnd_range_ 0 ;		
Agent/TCP set rate_request_ 0
Agent/TCP set qs_enabled_ false
Agent/TCP set tcp_qs_recovery_ true ;	
Agent/TCP set qs_request_mode_ 1
Agent/TCP set qs_rtt_ 50
Agent/TCP set print_request_ false
Agent/TCP set qs_thresh_ 4
Agent/TCP set ndatapack_ 0
Agent/TCP set ndatabytes_ 0
Agent/TCP set nackpack_ 0
Agent/TCP set nrexmit_ 0
Agent/TCP set nrexmitpack_ 0
Agent/TCP set nrexmitbytes_ 0
Agent/TCP set necnresponses_ 0
Agent/TCP set ncwndcuts_ 0 
Agent/TCP set ncwndcuts1_ 0
Agent/TCP set trace_all_oneline_ false
Agent/TCP set QOption_ 0 
Agent/TCP set EnblRTTCtr_ 0
Agent/TCP set control_increase_ 0
Agent/TCP set SetCWRonRetransmit_ true ;
Agent/TCP set nam_tracevar_ false
Agent/TCP/Fack set ss-div4_ false
Agent/TCP/Fack set rampdown_ false
Agent/TCP/Reno/XCP set timestamps_ true
Agent/TCP/FullTcp/Newreno/XCP set timestamps_ true
Agent/TCP set eln_ 0
Agent/TCP set eln_rxmit_thresh_ 1
Agent/TCP set delay_growth_ true ;	
Agent/TCP set CoarseTimer_      0
Agent/TCP set frto_enabled_	0 ;	
Agent/TCP set sfrto_enabled_	0 ;	
Agent/TCP set spurious_response_ 1 ;	
Agent/TCPSink set sport_        0
Agent/TCPSink set dport_        0         
Agent/TCPSink set packetSize_ 40
Agent/TCPSink set maxSackBlocks_ 3
Agent/TCPSink set ts_echo_bugfix_ true ;	
Agent/TCPSink set ts_echo_rfc1323_ false ;	
Agent/TCPSink set generateDSacks_ false
Agent/TCPSink set qs_enabled_ false
Agent/TCPSink set RFC2581_immediate_ack_ true
Agent/TCPSink set SYN_immediate_ack_ true ; 
Agent/TCPSink set bytes_ 0
Agent/TCPSink set ecn_syn_ false ;	
Agent/TCPSink/DelAck set interval_ 100ms
catch {
	Agent/TCPSink/Asym set interval_ 100ms
	Agent/TCPSink/Asym set maxdelack_ 5
}
Agent/TCPSink/Sack1/DelAck set interval_ 100ms
Agent/TCP/Newreno set newreno_changes_ 0
Agent/TCP/Newreno set newreno_changes1_ 1
Agent/TCP/Newreno set partial_window_deflation_ 1 ; 
Agent/TCP/Newreno set exit_recovery_fix_ 0
Agent/TCP/Vegas set v_alpha_ 1
Agent/TCP/Vegas set v_beta_ 3
Agent/TCP/Vegas set v_gamma_ 1
Agent/TCP/Vegas set v_rtt_ 0
Agent/TCP/Vegas/RBP set rbp_scale_ 0.75
Agent/TCP/Vegas/RBP set rbp_rate_algorithm_ 1
Agent/TCP/Vegas/RBP set rbp_segs_actually_paced_ 0
Agent/TCP/Vegas/RBP set rbp_inter_pace_delay_ 0
Agent/TCP/Reno/RBP set rbp_scale_ 0.75
Agent/TCP/Reno/RBP set rbp_segs_actually_paced_ 0
Agent/TCP/Reno/RBP set rbp_inter_pace_delay_ 0
Agent/TCP/Asym set g_ 0.125
Agent/TCP/Reno/Asym set g_ 0.125
Agent/TCP/Newreno/Asym set g_ 0.125
Agent/TCP/RFC793edu set add793expbackoff_  true 
Agent/TCP/RFC793edu set add793jacobsonrtt_ false
Agent/TCP/RFC793edu set add793fastrtx_     false
Agent/TCP/RFC793edu set add793slowstart_   false
Agent/TCP/RFC793edu set add793additiveinc_ false
Agent/TCP/RFC793edu set add793karnrtt_     true 
Agent/TCP/RFC793edu set rto_               60
Agent/TCP/RFC793edu set syn_               true
Agent/TCP/RFC793edu set add793exponinc_    false
Agent/TCP/FullTcp instproc done_data {} { }
Agent/TFRC set rate_ 0 
Agent/TFRC set ndatapack_ 0 ;	
Agent/TFRC set ndatabytes_ 0 ;	
Agent/TFRC set true_loss_rate_ 0.0 ; 
Agent/TFRC set srtt_init_ 0 ;	
Agent/TFRC set rttvar_init_ 12  
Agent/TFRC set rtxcur_init_ 6.0	
Agent/TFRC set rttvar_exp_ 2	
Agent/TFRC set T_SRTT_BITS 3	
Agent/TFRC set T_RTTVAR_BITS 2	
Agent/TFRC set voip_ 0 ;         
Agent/TFRC set voip_max_pkt_rate_ 100 ;  
Agent/TFRC set fsize_ 1460 ;	
Agent/TFRC set headersize_ 32 ; 
Agent/TFRC set rate_init_option_ 2 ;	
Agent/TFRC set slow_increase_ 1 ;
Agent/TFRC set maxHeavyRounds_ 0; 
Agent/TFRC set conservative_ 0 ;  
Agent/TFRC set scmult_ 1.5 ;	
Agent/TFRC set oldCode_ false ; 
Agent/TFRC set packetSize_ 1000 
Agent/TFRC set df_ 0.95 ;	
Agent/TFRC set tcp_tick_ 0.1 ;	
Agent/TFRC set InitRate_ 300 ;	
Agent/TFRC set overhead_ 0 ;	
Agent/TFRC set ssmult_ 2 ; 	
Agent/TFRC set bval_ 1 ;	
Agent/TFRC set ca_ 1 ; 	 	
Agent/TFRC set printStatus_ 0 
Agent/TFRC set ecn_ 0 ;		
Agent/TFRC set minrto_ 0.0 ;	
Agent/TFRC set SndrType_ 0 ;   
Agent/TFRC set maxqueue_ MAXSEQ ;  
Agent/TFRC set rate_init_ 2 ;		
Agent/TFRC set useHeaders_ true ;	
Agent/TFRC set idleFix_ true ;	
Agent/TFRCSink set packetSize_ 40
Agent/TFRCSink set InitHistorySize_ 100000
Agent/TFRCSink set NumFeedback_ 1 
Agent/TFRCSink set AdjustHistoryAfterSS_ 1
Agent/TFRCSink set NumSamples_ -1
Agent/TFRCSink set discount_ 1;	
Agent/TFRCSink set minDiscountRatio_ 0.5; 
Agent/TFRCSink set printLoss_ 0
Agent/TFRCSink set smooth_ 1 ;	
Agent/TFRCSink set ShortIntervals_ 0 ; 
Agent/TFRCSink set ShortRtts_ 2 ; 
Agent/TFRCSink set minlc_ 4
Agent/TFRCSink set algo_ 1 ;  	
Agent/TFRCSink set maxint_ 1000 ;    
Agent/TFRCSink set history_ 0.75 ;   
Agent/TFRCSink set PreciseLoss_ 1 ;   
Agent/TFRCSink set numPkts_ 1;	
Agent/TFRCSink set bytes_ 0 ;	

if [TclObject is-class Agent/TCP/FullTcp] {
	Agent/TCP/FullTcp set segsperack_ 1;
	Agent/TCP/FullTcp set spa_thresh_ 0; 
	Agent/TCP/FullTcp set segsize_ 536; 
	Agent/TCP/FullTcp set tcprexmtthresh_ 3; 
	Agent/TCP/FullTcp set iss_ 0; 
	Agent/TCP/FullTcp set nodelay_ false; 
	Agent/TCP/FullTcp set data_on_syn_ false; 
	Agent/TCP/FullTcp set dupseg_fix_ true ; 
	Agent/TCP/FullTcp set dupack_reset_ false; 
	Agent/TCP/FullTcp set interval_ 0.1 ; 
	Agent/TCP/FullTcp set close_on_empty_ false; 
	Agent/TCP/FullTcp set signal_on_empty_ false; 
	Agent/TCP/FullTcp set ts_option_size_ 10; 
	Agent/TCP/FullTcp set reno_fastrecov_ true; 
	Agent/TCP/FullTcp set pipectrl_ false; 
	Agent/TCP/FullTcp set open_cwnd_on_pack_ true; 
	Agent/TCP/FullTcp set halfclose_ false; 
	Agent/TCP/FullTcp set nopredict_ false; 
    Agent/TCP/FullTcp set ecn_syn_ false; 
    Agent/TCP/FullTcp set ecn_syn_wait_ 0; 
    Agent/TCP/FullTcp set debug_ false;  
	Agent/TCP/FullTcp/Newreno set recov_maxburst_ 2; 
	Agent/TCP/FullTcp/Sack set sack_block_size_ 8;
	Agent/TCP/FullTcp/Sack set sack_option_size_ 2; 
	Agent/TCP/FullTcp/Sack set max_sack_blocks_ 3; 
	Agent/TCP/FullTcp/Sack set clear_on_timeout_ true; 
	Agent/TCP/FullTcp/Sack set sack_rtx_cthresh_ 1; 
	Agent/TCP/FullTcp/Sack set sack_rtx_bthresh_ 1; 
	Agent/TCP/FullTcp/Sack set sack_rtx_threshmode_ 1; 
	Agent/TCP/FullTcp/Tahoe instproc init {} {
		$self next
		$self instvar reno_fastrecov_
		set reno_fastrecov_ false
	}

	Agent/TCP/FullTcp/Sack instproc init {} {
		$self next
		$self instvar reno_fastrecov_ open_cwnd_on_pack_
		set reno_fastrecov_ false
		set open_cwnd_on_pack_ false
	}

	Agent/TCP/FullTcp/Newreno instproc init {} {
		$self next
		$self instvar open_cwnd_on_pack_
		set open_cwnd_on_pack_ false
	}
}

if [TclObject is-class Agent/TCP/BayFullTcp] {
	Agent/TCP/BayFullTcp set segsperack_ 1; 
	Agent/TCP/BayFullTcp set segsize_ 536; 
	Agent/TCP/BayFullTcp set tcprexmtthresh_ 3;
	Agent/TCP/BayFullTcp set iss_ 0; 
	Agent/TCP/BayFullTcp set nodelay_ false; 
	Agent/TCP/BayFullTcp set data_on_syn_ false; 
	Agent/TCP/BayFullTcp set dupseg_fix_ true ; 
	Agent/TCP/BayFullTcp set dupack_reset_ false; 
	Agent/TCP/BayFullTcp set interval_ 0.1 ;
	Agent/TCP/BayFullTcp set close_on_empty_ false; 
	Agent/TCP/BayFullTcp set ts_option_size_ 10; 
	Agent/TCP/BayFullTcp set reno_fastrecov_ true; 
	Agent/TCP/BayFullTcp set pipectrl_ false; 
	Agent/TCP/BayFullTcp set open_cwnd_on_pack_ true; 
	Agent/TCP/BayFullTcp set halfclose_ false; 
	Agent/TCP/BayFullTcp/Newreno set recov_maxburst_ 2; 
	Agent/TCP/BayFullTcp/Sack set sack_block_size_ 8; # bytes in a SACK block
	Agent/TCP/BayFullTcp/Sack set sack_option_size_ 2; # bytes in opt hdr
	Agent/TCP/BayFullTcp/Sack set max_sack_blocks_ 3; # max # of sack blks
}

Agent/Null set sport_           0
Agent/Null set dport_           0
Agent/CBR set sport_            0
Agent/CBR set dport_            0
Agent/HttpInval set inval_hdr_size_ 40
Agent/RTP set seqno_ 0
Agent/RTP set interval_ 3.75ms
Agent/RTP set random_ 0
Agent/RTP set packetSize_ 210
Agent/RTP set maxpkts_ 0x10000000
Agent/RTP instproc done {} { }
Agent/RTCP set seqno_ 0
Agent/Message set packetSize_ 180
Agent/MessagePassing set packetSize_ 1500
Agent/LossMonitor set nlost_ 0
Agent/LossMonitor set npkts_ 0
Agent/LossMonitor set bytes_ 0
Agent/LossMonitor set lastPktTime_ 0
Agent/LossMonitor set expected_ 0
Agent/RAP set packetSize_ 512
Agent/RAP set seqno_ 0
Agent/RAP set sessionLossCount_ 0
Agent/RAP set ipg_ 2.0
Agent/RAP set alpha_ 1.0
Agent/RAP set beta_ 0.5
Agent/RAP set srtt_ 2.0
Agent/RAP set variance_ 0.0
Agent/RAP set delta_ 0.5
Agent/RAP set mu_ 1.2
Agent/RAP set phi_ 4.0
Agent/RAP set timeout_ 2.0
Agent/RAP set overhead_ 0
Agent/RAP set useFineGrain_ 0
Agent/RAP set kfrtt_ 0.9
Agent/RAP set kxrtt_ 0.01
Agent/RAP set debugEnable_ 0
Agent/RAP set rap_base_hdr_size_ 44
Agent/RAP set dpthresh_ 50
Agent/RAP instproc done {} { }
Agent/Mcast/Control set packetSize_ 80
Agent/rtProto set preference_ 200		;
Agent/rtProto/Direct set preference_ 100
Agent/rtProto/DV set preference_	120
Agent/rtProto/DV set INFINITY		 [Agent set ttl_]
Agent/rtProto/DV set advertInterval	  2
Agent/Encapsulator set status_ 1
Agent/Encapsulator set overhead_ 20
Agent/DSRAgent set sport_ 255
Agent/DSRAgent set dport_ 255
Agent/MIPBS set adSize_ 48
Agent/MIPBS set shift_ 0
Agent/MIPBS set mask_ [AddrParams set ALL_BITS_SET]
Agent/MIPBS set ad_lifetime_ 2
Agent/MIPMH set home_agent_ 0
Agent/MIPMH set rreqSize_ 52
Agent/MIPMH set reg_rtx_ 0.5
Agent/MIPMH set shift_ 0
Agent/MIPMH set mask_ [AddrParams set ALL_BITS_SET]
Agent/MIPMH set reg_lifetime_ 2
Agent/Diff_Sink set packetSize_ 512
Agent/Diff_Sink set interval_   0.5
Agent/Diff_Sink set random_     1
Agent/Diff_Sink set maxpkts_    10000
Agent/Diff_Sink set data_type_  0
Agent/LossMonitor/PLM set flag_PP_ 0
Agent/LossMonitor/PLM set packet_time_PP_ 0
Agent/LossMonitor/PLM set fid_PP_ 0
Agent/LossMonitor/PLM set seqno_ 0
Agent/LDP set trace_ldp_ 0
Simulator set nix-routing 0
RtModule set classifier_ ""
RtModule/Base set classifier_ ""
LMSErrorModel set rate_		0.0	;
LMSErrorModel set errPkt_	0
LMSErrorModel set errByte_	0
LMSErrorModel set errTime_	0.0
LMSErrorModel set onlink_	0
LMSErrorModel set enable_	0
LMSErrorModel set ndrops_	0
LMSErrorModel set bandwidth_	2Mb
LMSErrorModel set markecn_	false
LMSErrorModel set debug_	false
LMSErrorModel set delay_pkt_ false
LMSErrorModel set delay_ 0
set lmsPacketSize 1024
set lsize [Application/Traffic/CBR set packetSize_]
RtModule/LMS set node_	""
Agent/LMS set lms_enabled_	1
Agent/LMS set packetSize_	$lmsPacketSize
Agent/LMS/Sender set interval_ 4.0ms
Agent/LMS/Sender set packetSize_ $lsize
Agent/LMS/Sender set lmsPacketSize_ $lmsPacketSize
Agent/LMS/Sender set random_ 0
Agent/LMS/Sender set maxpkts_ 0x10000000
Agent/LMS/Sender set odat_ 0
Agent/LMS/Sender instproc done {} { }
Agent/LMS/Receiver set lmsPacketSize_ $lmsPacketSize
Agent/LMS/Receiver set bytes_ 0
Agent/LMS/Receiver set nlost_ 0
Agent/LMS/Receiver set npkts_ 0
Agent/LMS/Receiver set expected_ 0
Agent/LMS/Receiver set lastPktTime_ 0.0
Agent/LMS/Receiver instproc done {} { }
Agent/LMS/Receiver set packetSize_ $lsize
Agent/QSAgent set qs_enabled_ 1
Agent/QSAgent set old_classifier_ 0
Agent/QSAgent set state_delay_ 0.2 ;	
Agent/QSAgent set alloc_rate_ 0.85 ; 	
Agent/QSAgent set threshold_ 0.85 ; 	
Agent/QSAgent set max_rate_ 256
Agent/QSAgent set mss_ [Agent/TCP set packetSize_]
Agent/QSAgent set rate_function_ 2
Agent/QSAgent set algorithm_ 3	; 	
Queue set util_weight_ 0.8
Queue set util_check_intv_ 0.2 ;	
Queue set util_records_ 5 ; 		
Delayer set debug_ false
Agent/DCCP set packetSize_ 500
Agent/DCCP set initial_rtx_to_ 3.0
Agent/DCCP set max_rtx_to_ 75.0
Agent/DCCP set resp_to_ 75.0
Agent/DCCP set sb_size_ 1000
Agent/DCCP set opt_size_ 512
Agent/DCCP set feat_size_ 24
Agent/DCCP set ackv_size_ 20
Agent/DCCP set ccid_ 0
Agent/DCCP set use_ecn_local_ 0
Agent/DCCP set use_ecn_remote_ 0
Agent/DCCP set ack_ratio_local_ 2
Agent/DCCP set ack_ratio_remote_ 2
Agent/DCCP set use_ackv_local_ 0
Agent/DCCP set use_ackv_remote_ 0
Agent/DCCP set q_scheme_ 0
Agent/DCCP set q_local_ 0
Agent/DCCP set q_remote_ 0
Agent/DCCP set snd_delay_ 0.0001
Agent/DCCP set nam_tracevar_ false
Agent/DCCP set trace_all_oneline_ false
Agent/DCCP set allow_mult_neg_ 0
Agent/DCCP set ndp_limit_ 8
Agent/DCCP set ccval_limit_ 16
Agent/DCCP set cscov_ 0
Agent/DCCP set num_data_pkt_ 0
Agent/DCCP set num_ack_pkt_ 0
Agent/DCCP set num_dataack_pkt_ 0
Agent/DCCP/TCPlike set ccid_ 2
Agent/DCCP/TCPlike set use_ecn_local_ 1
Agent/DCCP/TCPlike set use_ecn_remote_ 1
Agent/DCCP/TCPlike set use_ackv_local_ 1
Agent/DCCP/TCPlike set use_ackv_remote_ 1
Agent/DCCP/TCPlike set initial_cwnd_ 3
Agent/DCCP/TCPlike set cwnd_timeout_ 1
Agent/DCCP/TCPlike set initial_ssthresh_ 65535
Agent/DCCP/TCPlike set cwnd_ 2
Agent/DCCP/TCPlike set cwnd_frac_ 0
Agent/DCCP/TCPlike set ssthresh_ 20
Agent/DCCP/TCPlike set pipe_ 0
Agent/DCCP/TCPlike set initial_rto_ 3.0
Agent/DCCP/TCPlike set min_rto_ 1.0
Agent/DCCP/TCPlike set rto_ 3.0
Agent/DCCP/TCPlike set srtt_ -1.0
Agent/DCCP/TCPlike set rttvar_ 0.0
Agent/DCCP/TCPlike set rtt_sample_ 0.0
Agent/DCCP/TCPlike set alpha_ 0.125
Agent/DCCP/TCPlike set beta_ 0.25
Agent/DCCP/TCPlike set k_ 4
Agent/DCCP/TCPlike set g_ 0.01
Agent/DCCP/TCPlike set num_dup_acks_ 3
Agent/DCCP/TCPlike set q_min_t_ 0.2
Agent/DCCP/TCPlike set q_opt_ratio_ 1
Agent/DCCP/TCPlike set dack_delay_ 0.2
Agent/DCCP/TCPlike set ackv_size_lim_ 10
Agent/DCCP/TFRC set ccid_ 3
Agent/DCCP/TFRC set use_ecn_local_ 1
Agent/DCCP/TFRC set use_ecn_remote_ 1
Agent/DCCP/TFRC set use_ackv_local_ 1
Agent/DCCP/TFRC set use_ackv_remote_ 1
Agent/DCCP/TFRC set use_loss_rate_local_ 1
Agent/DCCP/TFRC set use_loss_rate_remote_ 1
Agent/DCCP/TFRC set rtt_scheme_local_ 0
Agent/DCCP/TFRC set rtt_scheme_remote_ 0
Agent/DCCP/TFRC set num_dup_acks_ 3
Agent/DCCP/TFRC set p_tol_ 0.05
Agent/DCCP/TFRC set win_count_per_rtt_ 4
Agent/DCCP/TFRC set max_wc_inc_ 5
Agent/DCCP/TFRC set s_use_osc_prev_ 1
Agent/DCCP/TFRC set s_x_ 500.0
Agent/DCCP/TFRC set s_x_inst_ 500.0
Agent/DCCP/TFRC set s_x_recv_ 0.0
Agent/DCCP/TFRC set s_r_sample_ 0.0
Agent/DCCP/TFRC set s_rtt_ 0.0
Agent/DCCP/TFRC set s_r_sqmean_ 0.0
Agent/DCCP/TFRC set s_smallest_p_ 0.00001
Agent/DCCP/TFRC set s_rtt_q_ 0.9
Agent/DCCP/TFRC set s_rtt_q2_ 0.9
Agent/DCCP/TFRC set s_t_mbi_ 64.0
Agent/DCCP/TFRC set s_os_time_gran_ 0.01
Agent/DCCP/TFRC set s_s_ 500
Agent/DCCP/TFRC set s_initial_x_ 500.0
Agent/DCCP/TFRC set s_initial_rto_ 2.0
Agent/DCCP/TFRC set s_x_ 500.0
Agent/DCCP/TFRC set s_x_inst_ 500.0
Agent/DCCP/TFRC set s_x_recv_ 0.0
Agent/DCCP/TFRC set s_r_sample_ 0.0
Agent/DCCP/TFRC set s_rtt_ 0.0
Agent/DCCP/TFRC set s_r_sqmean_ 0.0
Agent/DCCP/TFRC set s_p_ 0.0
Agent/DCCP/TFRC set s_q_opt_ratio_ 1
Agent/DCCP/TFRC set r_s_ 500
Agent/DCCP/TFRC set r_rtt_ 0.0
Agent/DCCP/TFRC set r_p_ 0.0
Agent/DCCP/TFRC set q_min_t_ 0.2
Agent/TCP/Linux set rtxcur_init_ 3
Agent/TCP/Linux set maxrto_ 120
Agent/TCP/Linux set minrto_ 0.2
Agent/TCP/Linux set ts_resetRTO_ true
Agent/TCP/Linux set next_pkts_in_flight_ 0
Agent/TCP/Linux set delay_growth_ false
Agent/PBC set payloadSize 200
Agent/PBC set periodicBroadcastInterval 1
Agent/PBC set periodicBroadcastVariance 0.1
Agent/PBC set modulationScheme 0
Agent/MDART set macFailed_ true
Agent/MDART set etxMetric_ true
Agent/Aspu set macFailed_ true
Agent/Aspu set etxMetric_ true