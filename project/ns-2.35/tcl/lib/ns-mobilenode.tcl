
ARPTable instproc init args {
	eval $self next $args		
}

ARPTable set bandwidth_         0
ARPTable set delay_             5us

Node/MobileNode instproc init args {
	eval $self next $args
	$self instvar nifs_ arptable_ X_ Y_ Z_ nodetype_
	set X_ 0.0
	set Y_ 0.0
	set Z_ 0.0
        set arptable_ ""               
	set nifs_	0		
        $self makemip-New$nodetype_
}

Node/MobileNode instproc makemip-NewMobile {} {
}

Node/MobileNode instproc makemip-NewBase {} {
}

Node/MobileNode instproc makemip-New {} {
}

Node/MobileNode instproc makemip-NewMIPBS {} {
	$self instvar regagent_ encap_ decap_ agents_ id_

	set dmux [new Classifier/Port/Reserve]
	$dmux set mask_ 0x7fffffff
	$dmux set shift_ 0
	$self install-demux $dmux
   
	set regagent_ [new Agent/MIPBS $self]
	$self attach $regagent_ [Node/MobileNode set REGAGENT_PORT]
	$self attach-encap 
	$self attach-decap
}

Node/MobileNode instproc attach-encap {} {
	$self instvar encap_ 
	set encap_ [new MIPEncapsulator]
	$encap_ set mask_ [AddrParams NodeMask 1]
	$encap_ set shift_ [AddrParams NodeShift 1]
	set nodeaddr [AddrParams addr2id [$self node-addr]]
	$encap_ set addr_ [expr ( ~([AddrParams NodeMask 1] << \
			[AddrParams NodeShift 1]) & $nodeaddr )]
	$encap_ set port_ 1
	$encap_ target [$self entry]
	$encap_ set node_ $self
}

Node/MobileNode instproc attach-decap {} {
	$self instvar decap_ dmux_ agents_
	set decap_ [new Classifier/Addr/MIPDecapsulator]
	lappend agents_ $decap_
	$decap_ set mask_ [AddrParams NodeMask 1]
	$decap_ set shift_ [AddrParams NodeShift 1]
	$dmux_ install [Node/MobileNode set DECAP_PORT] $decap_
}

Node/MobileNode instproc makemip-NewMIPMH {} {
	$self instvar regagent_
 
	set dmux [new Classifier/Port/Reserve]
	$dmux set mask_ 0x7fffffff
	$dmux set shift_ 0
	$self install-demux $dmux

	set regagent_ [new Agent/MIPMH $self]
	$self attach $regagent_ [Node/MobileNode set REGAGENT_PORT]
	$regagent_ set mask_ [AddrParams NodeMask 1]
	$regagent_ set shift_ [AddrParams NodeShift 1]
 	$regagent_ set dst_addr_ [expr (~0) << [AddrParams NodeShift 1]]
	$regagent_ set dst_port_ 0
	$regagent_ node $self
}


Node/MobileNode instproc reset {} {
	$self instvar arptable_ nifs_ netif_ mac_ ifq_ ll_ imep_
    for {set i 0} {$i < $nifs_} {incr i} {
		$netif_($i) reset
		$mac_($i) reset
		$ll_($i) reset
		$ifq_($i) reset
		if { [info exists opt(imep)] && $opt(imep) == "ON" } { 
			$imep_($i) reset 
		}
		if { $arptable_($i) != "" } {
		$arptable_($i) reset 
	}
	}
}

Node/MobileNode instproc add-target { agent port } {
	$self instvar dmux_ imep_ toraDebug_ numifs_
	set ns [Simulator instance]
	set newapi [$ns imep-support]
	$agent set sport_ $port
set numIfsSimulator [$numifs 3]
	set toraonly [string first "TORA" [$agent info class]] 
	if {$toraonly != -1 } {
		$agent if-queue [$self set ifq_(0)]  ;
		$agent imep-agent [$self set imep_(0)]
		[$self set imep_(0)] rtagent $agent
	}
	set aomdvonly [string first "AOMDV" [$agent info class]] 
	if {$aomdvonly != -1 } {
		$agent if-queue [$self set ifq_(0)]   ;
	}
	set aodvonly [string first "AODV" [$agent info class]] 
	if {$aodvonly != -1 } {
		$agent if-queue [$self set ifq_(0)]   ;
	}

	if { $port == [Node set rtagent_port_] } {	
		if {$numIfsSimulator != ""} {
			for {set i 0} {$i < $numifs_} {inc i} {
				$agent if-queue $i
			}
		}
		$self add-target-rtagent $agent $port
		return
	}
	set namfp [$ns get-nam-traceall]
	if { [Simulator set AgentTrace_] == "ON" } {
		if {$newapi != ""} {
			set sndT [$self mobility-trace Send "AGT"]
		} else {
			set sndT [cmu-trace Send AGT $self]
		}
		if { $namfp != "" } {
			$sndT namattach $namfp
		}
		$sndT target [$self entry]
		$agent target $sndT
		if {$newapi != ""} {
			set rcvT [$self mobility-trace Recv "AGT"]
		} else {
			set rcvT [cmu-trace Recv AGT $self]
		}
		if { $namfp != "" } {
			$rcvT namattach $namfp
		}
		$rcvT target $agent
		$dmux_ install $port $rcvT
	} else {
		$agent target [$self entry]
		$dmux_ install $port $agent
	}
}

Node/MobileNode instproc add-target-rtagent { agent port } {
	$self instvar imep_ toraDebug_ numifs_
	set ns [Simulator instance]
	set newapi [$ns imep-support]
	set namfp [$ns get-nam-traceall]
	set dmux_ [$self demux]
	set classifier_ [$self entry]
set numIfsSimulator [$numifs 3]	
	$agent port-dmux $dmux_
	if { [Simulator set RouterTrace_] == "ON" } {
		if {$newapi != ""} {
			set sndT [$self mobility-trace Send "RTR"]
		} else {
			set sndT [cmu-trace Send "RTR" $self]
		}
		if { $namfp != "" } {
			$sndT namattach $namfp
		}
		if { $newapi == "ON" } {
			$agent target $imep_(0)
			$imep_(0) sendtarget $sndT
			if { [info exists toraDebug_] && $toraDebug_ == "ON"} {
				set sndT2 [$self mobility-trace Send "TRP"]
				$sndT2 target $imep_(0)
				$agent target $sndT2
			}
			$sndT target [$self set ll_(0)]
		} else {  ;
			if {$numIfsSimulator != ""} {
				for {set i 0} {$i < $numifs_} {inc i} {
					set sndT [cmu-trace Send "RTR" $self]
					$agent target $i $sndT 
					$sndT target [$self set ll_($i)]
				}
			} else {
			$agent target $sndT
			$sndT target [$self set ll_(0)]
			}
		}
		if {$newapi != ""} {
			set rcvT [$self mobility-trace Recv "RTR"]
		} else {
			set rcvT [cmu-trace Recv "RTR" $self]
		}
		if { $namfp != "" } {
			$rcvT namattach $namfp
		}
		if {$newapi == "ON" } {
			[$self set ll_(0)] up-target $imep_(0)
			$classifier_ defaulttarget $agent
			if {[info exists toraDebug_] && $toraDebug_ == "ON" } {
				set rcvT2 [$self mobility-trace Recv "TRP"]
				$rcvT2 target $agent
				$classifier_ defaulttarget $rcvT2
			}
		} else {
			$rcvT target $agent
			$classifier_ defaulttarget $rcvT
			$dmux_ install $port $rcvT
		}
	} else {
		if { $newapi == "ON" } {
			$agent target $imep_(0)
			if { [info exists toraDebug_] && $toraDebug_ == "ON"} {
				set sndT2 [$self mobility-trace Send "TRP"]
				$sndT2 target $imep_(0)
				$agent target $sndT2
			}
			$imep_(0) sendtarget [$self set ll_(0)]
		} else {  ;
			if {$numIfsSimulator != ""} {
				for {set i 0} {$i < $numifs_} {inc i} {
					$sndT target $i [$self set ll_($i)]
				}
			} else {
				$sndT target [$self set ll_(0)]
			}
			
		}    
		if {$newapi == "ON" } {
			[$self set ll_(0)] up-target $imep_(0)
			$classifier_ defaulttarget $agent
			if {[info exists toraDebug_] && $toraDebug_ == "ON" } {
				set rcvT2 [$self mobility-trace Recv "TRP"]
				$rcvT2 target $agent
				[$self set classifier_] defaulttarget $rcvT2
			}
		} else {
			$classifier_ defaulttarget $agent
			$dmux_ install $port $agent
		}
	}
}
Node/MobileNode instproc add-interface { channel pmodel lltype mactype qtype qlen iftype anttype topo inerrproc outerrproc fecproc } {
	$self instvar arptable_ nifs_ netif_ mac_ ifq_ ll_ imep_ inerr_ outerr_ fec_
	set ns [Simulator instance]
	set imepflag [$ns imep-support]
	set t $nifs_
	incr nifs_
	set netif_($t)	[new $iftype]		;
	set mac_($t)	[new $mactype]		;
	set ifq_($t)	[new $qtype]		;
	set ll_($t)	[new $lltype]		;
        set ant_($t)    [new $anttype]
	$ns mac-type $mactype
	set inerr_($t) ""
	if {$inerrproc != ""} {
		set inerr_($t) [$inerrproc]
	}
	set outerr_($t) ""
	if {$outerrproc != ""} {
		set outerr_($t) [$outerrproc]
	}
	set fec_($t) ""
	if {$fecproc != ""} {
		set fec_($t) [$fecproc]
	}

	set namfp [$ns get-nam-traceall]
        if {$imepflag == "ON" } {              
		set imep_($t) [new Agent/IMEP [$self id]]
		set imep $imep_($t)
		set drpT [$self mobility-trace Drop "RTR"]
		if { $namfp != "" } {
			$drpT namattach $namfp
		}
		$imep drop-target $drpT
		$ns at 0.[$self id] "$imep_($t) start"   ;
        }
	set nullAgent_ [$ns set nullAgent_]
	set netif $netif_($t)
	set mac $mac_($t)
	set ifq $ifq_($t)
	set ll $ll_($t)

	set inerr $inerr_($t)
	set outerr $outerr_($t)
	set fec $fec_($t)
		set arptable_($t) [new ARPTable $self $mac]
		set arptable $arptable_($t) 
		if {$imepflag != ""} {
			set drpT [$self mobility-trace Drop "IFQ"]
		} else {
			set drpT [cmu-trace Drop "IFQ" $self]
		}
		$arptable_ drop-target $drpT
		if { $namfp != "" } {
			$drpT namattach $namfp
		}
	$ll arptable $arptable_
	$ll mac $mac
	$ll down-target $ifq

	if {$imepflag == "ON" } {
		$imep recvtarget [$self entry]
		$imep sendtarget $ll
		$ll up-target $imep
        } else {
		$ll up-target [$self entry]
	}
	$ifq target $mac
	$ifq set limit_ $qlen
	if {$imepflag != ""} {
		set drpT [$self mobility-trace Drop "IFQ"]
	} else {
		set drpT [cmu-trace Drop "IFQ" $self]
        }
	$ifq drop-target $drpT
	if { $namfp != "" } {
		$drpT namattach $namfp
	}
	if {[$ifq info class] == "Queue/XCP"} {		
		$mac set bandwidth_ [$ll set bandwidth_]
		$mac set delay_ [$ll set delay_]
		$ifq set-link-capacity [$mac set bandwidth_]
		$ifq queue-limit $qlen
		$ifq link $ll
		$ifq reset
		
	}
	$mac netif $netif
	$mac up-target $ll
	if {$outerr == "" && $fec == ""} {
		$mac down-target $netif
	} elseif {$outerr != "" && $fec == ""} {
		$mac down-target $outerr
		$outerr target $netif
	} elseif {$outerr == "" && $fec != ""} {
		$mac down-target $fec
		$fec down-target $netif
	} else {
		$mac down-target $fec
		$fec down-target $outerr
		$err target $netif
	}
	set god_ [God instance]
    if {$mactype == "Mac/802_11Ext"} {
		$mac nodes [$god_ num_nodes]
	}
    if {$mactype == "Mac/802_11"} {
		$mac nodes [$god_ num_nodes]
	}
	$netif channel $channel
	if {$inerr == "" && $fec == ""} {
		$netif up-target $mac
	} elseif {$inerr != "" && $fec == ""} {
		$netif up-target $inerr
		$inerr target $mac
	} elseif {$err == "" && $fec != ""} {
		$netif up-target $fec
		$fec up-target $mac
	} else {
		$netif up-target $inerr
		$inerr target $fec
		$fec up-target $mac
	}
	$netif propagation $pmodel	;
	$netif node $self		;
	$netif antenna $ant_($t)
	$channel addif $netif
	$channel add-node $self		
	$topo channel $channel
	if { [Simulator set PhyTrace_] == "ON" } {
		if {$imepflag != ""} {
			set drpPhyT [$self mobility-trace Drop "PHY"]
		} else {
			set drpPhyT [cmu-trace Drop "PHY" $self]
		}
		$netif drop-target $drpPhyT
		if { $namfp != "" } {
			$drpPhyT namattach $namfp
		}
        if {$imepflag != ""} {
            set sndPhyT [$self mobility-trace Send "PHY"]
        } else {
            set sndPhyT [cmu-trace Send "PHY" $self]
        }
        $sndPhyT target $channel
        $netif down-target $sndPhyT
        if { $namfp != "" } {
            $sndPhyT namattach $namfp
        }
        
	} else {
		$netif drop-target [$ns set nullAgent_]
	}

	if { [Simulator set MacTrace_] == "ON" } {
		if {$imepflag != ""} {
			set rcvT [$self mobility-trace Recv "MAC"]
		} else {
			set rcvT [cmu-trace Recv "MAC" $self]
		}
		$mac log-target $rcvT
		if { $namfp != "" } {
			$rcvT namattach $namfp
		}
		if {$imepflag != ""} {
			set sndT [$self mobility-trace Send "MAC"]
		} else {
			set sndT [cmu-trace Send "MAC" $self]
		}
		$sndT target [$mac down-target]
		$mac down-target $sndT
		if { $namfp != "" } {
			$sndT namattach $namfp
		}
		if {$imepflag != ""} {
			set rcvT [$self mobility-trace Recv "MAC"]
		} else {
			set rcvT [cmu-trace Recv "MAC" $self]
		}
		$rcvT target [$mac up-target]
		$mac up-target $rcvT
		if { $namfp != "" } {
			$rcvT namattach $namfp
		}
		if {$imepflag != ""} {
			set drpT [$self mobility-trace Drop "MAC"]
		} else {
			set drpT [cmu-trace Drop "MAC" $self]
		}
		$mac drop-target $drpT
		if { $namfp != "" } {
			$drpT namattach $namfp
		}
	} else {
		$mac log-target [$ns set nullAgent_]
		$mac drop-target [$ns set nullAgent_]
	}
       if { [Simulator set EotTrace_] == "ON" } {
               if {$imepflag != ""} {
                       set eotT [$self mobility-trace EOT "MAC"]
               } else {
                       set eoT [cmu-trace EOT "MAC" $self]
               }
               $mac eot-target $eotT
       }
	$self addif $netif
}
Node/MobileNode instproc setPt { val } {
	$self instvar netif_
	$netif_(0) setTxPower $val
}
Node/MobileNode instproc setPr { val } {
	$self instvar netif_
	$netif_(0) setRxPower $val
}
Node/MobileNode instproc setPidle { val } {
	$self instvar netif_
	$netif_(0) setIdlePower $val
}
Node/MobileNode instproc setPsleep { val } {
	$self instvar netif_
	$netif_(0) setSleepPower $val
}
Node/MobileNode instproc setTSleep { val } {
	$self instvar netif_
	$netif_(0) setSleepTime $val
}
Node/MobileNode instproc setPtransition { val } {
	$self instvar netif_
	$netif_(0) setTransitionPower $val
}
Node/MobileNode instproc setTtransition { val } {
	$self instvar netif_
	$netif_(0) setTransitionTime $val
}
 Node/MobileNode instproc getMac {param0} {
       $self instvar mac_
       return $mac_($param0)
 }
 Node/MobileNode instproc CFP { beacon_period cfp_duration } {
       if {$mactype == "Mac/802_11"} {
               $self instvar mac_
               set ns_ [Simulator instance]
               set beacon_period [$ns_ delay_parse $beacon_period]
               set cfp_duration [$ns_ delay_parse $cfp_duration]
               $mac_(0) cfp $beacon_period $cfp_duration
       }
 }
Node/MobileNode instproc mobility-trace { ttype atype } {
	set ns [Simulator instance]
        set tracefd [$ns get-ns-traceall]
        if { $tracefd == "" } {
	        puts "Warning: You have not defined you tracefile yet!"
	        puts "Please use trace-all command to define it."
		return ""
	}
	set T [new CMUTrace/$ttype $atype]
	$T newtrace [Simulator set WirelessNewTrace_]
	$T tagged [Simulator set TaggedTrace_]
	$T target [$ns nullagent]
	$T attach $tracefd
        $T set src_ [$self id]
        $T node $self
	return $T
}
Node/MobileNode instproc nodetrace { tracefd } {
	set T [new Trace/Generic]
	$T target [[Simulator instance] set nullAgent_]
	$T attach $tracefd
	$T set src_ [$self id]
	$self log-target $T    
}
Node/MobileNode instproc agenttrace {tracefd} {
	set ns [Simulator instance]
	set ragent [$self set ragent_]
	set drpT [$self mobility-trace Drop "RTR"]
	set namfp [$ns get-nam-traceall]
	if { $namfp != ""} {
		$drpT namattach $namfp
	}
	$ragent drop-target $drpT
	set T [new Trace/Generic]
	$T target [$ns set nullAgent_]
	$T attach $tracefd
	$T set src_ [$self id]
	$ragent tracetarget $T
	set imepflag [$ns imep-support]
	if {$imepflag == "ON"} {
		[$self set imep_(0)] log-target $T
	}
}
Node/MobileNode instproc mip-call {ragent} {
	$self instvar regagent_
	if [info exists regagent_] {
		$regagent_ ragent $ragent
	}
}
Node/MobileNode instproc attach-gafpartner {} {
        $self instvar gafpartner_ address_ ll_ 
        set gafpartner_ [new GAFPartner]
	$gafpartner_ set mask_ [AddrParams NodeMask 1]
	$gafpartner_ set shift_ [AddrParams NodeShift 1]
	set nodeaddr [AddrParams addr2id [$self node-addr]]
	$gafpartner_ set addr_ $nodeaddr
	$gafpartner_ set port_ 254
        $gafpartner_ target [$self entry]
	$ll_(0) up-target $gafpartner_
}
Node/MobileNode instproc unset-gafpartner {} {
	$self instvar gafpartner_	
	$gafpartner_ set-gafagent 0
}
Node/MobileNode instproc sscs args {
	$self instvar mac_
	eval $mac_(0) sscs $args
}
Node/MobileNode instproc NodeClr {arg1} {
	$self instvar mac_
	$mac_(0) NodeClr $arg1
}
Node/MobileNode instproc NodeLabel args {
	$self instvar mac_
	eval $mac_(0) NodeLabel $args
}
Node/MobileNode instproc node-down {} {
	$self instvar mac_
	$mac_(0) node-down
}
Node/MobileNode instproc node-up {} {
	$self instvar mac_
	$mac_(0) node-up
}
Node/MobileNode instproc RNType args {
	$self instvar ragent_
	eval $ragent_ RNType $args
}
Class SRNodeNew -superclass Node/MobileNode
SRNodeNew instproc init args {
	$self instvar dsr_agent_ dmux_ entry_point_ address_
        set ns [Simulator instance]
	eval $self next $args	;
	if {$dmux_ == "" } {
		set dmux_ [new Classifier/Port]
	}
	set dsr_agent_ [new Agent/DSRAgent]
	$dsr_agent_ addr $address_
	$dsr_agent_ node $self
	if [Simulator set mobile_ip_] {
		$dsr_agent_ port-dmux [$self set dmux_]
	}
	$self addr $address_
	if { [Simulator set RouterTrace_] == "ON" } {
		set rcvT [$self mobility-trace Recv "RTR"]
		set namfp [$ns get-nam-traceall]
		if {  $namfp != "" } {
			$rcvT namattach $namfp
		}
		$rcvT target $dsr_agent_
		set entry_point_ $rcvT	
	} else {
		set entry_point_ $dsr_agent_
	}
	$self set ragent_ $dsr_agent_
	$dsr_agent_ target $dmux_
	set nullAgent_ [$ns set nullAgent_]
	$dmux_ install [Node set rtagent_port_] $nullAgent_
	$self instvar classifier_
	set classifier_ "srnode made illegal use of classifier_"
	return $self
}
SRNodeNew instproc start-dsr {} {
	$self instvar dsr_agent_
	$dsr_agent_ startdsr
}
SRNodeNew instproc entry {} {
        $self instvar entry_point_
        return $entry_point_
}
SRNodeNew instproc add-interface args {
	eval $self next $args
	$self instvar dsr_agent_ ll_ mac_ ifq_
	set ns [Simulator instance]
	$dsr_agent_ mac-addr [$mac_(0) id]
	if { [Simulator set RouterTrace_] == "ON" } {
		set sndT [$self mobility-trace Send "RTR"]
		set namfp [$ns get-nam-traceall]
		if {$namfp != "" } {
			$sndT namattach $namfp
		}
		$sndT target $ll_(0)
		$dsr_agent_ add-ll $sndT $ifq_(0)
	} else {
		$dsr_agent_ add-ll $ll_(0) $ifq_(0)
	}
	$dsr_agent_ install-tap $mac_(0)
}
SRNodeNew instproc reset args {
	$self instvar dsr_agent_
	eval $self next $args
	$dsr_agent_ reset
}