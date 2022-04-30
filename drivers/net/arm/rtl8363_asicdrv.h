
#ifndef _RTL8363_ASICDRV_H_
#define _RTL8363_ASICDRV_H_

//Added for SPR ETH driver [Start]
#ifndef NULL
#define NULL 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef SUCCESS
#define SUCCESS 	0
#endif
#ifndef FAILED
#define FAILED -1
#endif

/* Since there are two PHY ports in RTL switch, try to minimize efforts to find a PHY by using these constants */
#define Phy_0 0
#define Phy_1 1
//Added for SPR ETH driver [End]


#define  MACPAG 0x0
#define  UTPPAG  0x4
#define  SERLPAG  0x5
#define  SERHPAG 0x6


#define RTL8363_IDLE_TIMEOUT	(100000UL)   /*use it to realize a long time delay*/
#define RTL8363_TS1K_TIMEOUT     (100) /*use it to realize ts1k time out*/
#define RTL8363_DELAY(x, y) do { for ((x = 0); (x) < (y); (x)++); } while(0) 

/*choose one  serface:SMI, ISA, 8051*/
#define  RTL8363_INF RTL8363_INF_SMI   
#define  RTL8363_INF_SMI 0
#define  RTL8363_INF_ISA 1
#define  RTL8363_INF_8051 2
#define  RTL8363_BASEADDR_ISA (0xbe000000 + 0x400000)
/*choose ISA BUS WIDTH*/
#define  RTL8363_BUSWI_ISA  RTL8363_32BITS_ISA  
#define  RTL8363_32BITS_ISA 0
#define  RTL8363_16BITS_ISA 1
#define  RTL8363_8BITS_ISA   2
/*address multiply coefficients, because FLASH BANK 
is offen used as ISA serface,  but its address may not be 
coded by byte, but by serface width*/
#define  RTL8363_BUSMUL_ISA  RTL8363_32BITSMUL_ISA
#define  RTL8363_32BITSMUL_ISA 4        
#define  RTL8363_16BITSMUL_ISA 2
#define  RTL8363_8BITSMUL_ISA 1

/*PHY Reg.*/
#define PHY_CONTROL_REG						0
#define PHY_STATUS_REG							1
#define PHY_AN_ADVERTISEMENT_REG				4
#define PHY_AN_LINKPARTNER_REG				5
#define PHY_1000_BASET_CONTROL_REG			9
#define PHY_1000_BASET_STATUS_REG			10



/*VLAN macro*/
#define RTL8363_VLAN_IRTAG	 0 /*The switch will remove VLAN tags and add new tags*/
#define RTL8363_VLAN_RTAG 	 1 /*The switch will remove VLAN tags*/
#define RTL8363_VLAN_ITAG	 2/*	The switch will  add new VLANtag*/
#define RTL8363_VLAN_UNDOTAG 3	/*Do not insert or remove  VLAN tag*/

/*TS1000 OAM S field*/
/*PHY 0~1, reg22, mac_pag6*/
#define TS1K_S0_PWRSTATUS  (0X1 << 3)
#define TS1K_S1_OPTICALRXSIGSTATS  (0X1 << 2)
#define TS1K_S2_NETLINKSTATUS (0X1 << 1)
#define TS1K_S3_MCSTATUS (0X1 << 0)
/*PHY 0~1, reg23, mac_pag6*/
#define TS1K_S4_LOSSOPNOTIFWAY (0X1 << 7)
#define TS1K_S5_OPERSTATUS (0X1 << 6)
#define TS1K_S6_MCOPTIONB (0X1 << 5)
#define TS1K_S7_TERMINALLINKRATE (0X1 << 4)
#define TS1K_S8_TERMINALLINKRATE (0X1 << 3)
#define TS1K_S9_TERMINALLINKDUP (0X1 << 2)
#define TS1K_S10_TERMINALLINKNWAY (0X1 << 1)
#define TS1K_S11_NUMPHYINTERFACE (0X1 << 0)

/*LB flag MASK*/
#define TS1K_TXLBENDINDFLAG (0X1 << 0)  /*For terminal MC, sent LB end indication OAM*/
#define TS1K_TXLBENDRSPFLAG (0X1 << 1)  /*For terminal MC, sent LB end response OAM*/
#define TS1K_TXLBSTARTRSPFLAG (0X1 << 2) /*For terminal MC, sent LB start response OAM*/
#define TS1K_TXLBENDREQFLAG (0X1 << 3)   /*For center MC, sent LB end request OAM*/
#define TS1K_TXLBSTARTREQFLAG (0X1 << 4) /*For center MC, sent LB start request OAM*/
#define TS1K_LBCPLFLAG (0X1 << 5) /*Loop back frame send has completed. CPU read and clear this bit.*/
#define TS1K_AUTOLBCPL (0X1 << 6)  /*Auto loop back test is completed*/
#define TS1K_LBDEBUGCLR (0X1 << 7) /* "1"" means clear all flags including Tx and Rx loopback debug information (flag and counters)*/
#define TS1K_TIMER2EXP (0X1 << 8) /*Check if Timer2 expired*/
#define TS1K_TIMER1EXP (0X1 << 9) /*Check if Timer1 expired*/
#define TS1K_RXLBENDINDFLAG (0X1 << 10) /*For center MC, received LB end indication OAM*/
#define TS1K_RXLBENDRSPFLAG (0X1 << 11) /*For center MC, received LB end response OAM*/
#define TS1K_RXLBSTARTRSPFLAG (0X1 << 12) /*For center MC, received LB start response OAM*/
#define TS1K_AUTOLBRESULT (0X1 << 13) /*For auto loop back test result 0: fail 1: good*/


#define TS1K_INT_DYINGGASP ( 0x1 << 15)
#define TS1K_INT_LINKFAULT (0x1 << 14)
#define TS1K_INT_RXUSEROAM (0x1 << 11)
#define TS1K_INT_TSLINKSPD (0x1 << 10)
#define TS1K_INT_TSLINKDUP (0x1 << 9)
#define TS1K_INT_TSLINKNWAY (0x1 << 8)
#define TS1K_INT_RXOAMINT (0x1 << 7)
#define TS1K_INT_PWRSTATINT (0x1 << 6)
#define TS1K_INT_OPSTAT (0x1 << 5)
#define TS1K_INT_TERLINKSTAT (0x1 << 4)
#define TS1K_INT_MCSTAT (0x1 << 3)
#define TS1K_INT_LOSSOPSIG (0x1 << 2)
#define TS1K_INT_PHYINF (0x1 << 1)
#define TS1K_INT_RXMAGIC (0x1 << 0)

/*TS1K Magic Remote Read/Write*/
#define TS1K_RXBUSYRSPFLAG (0x1 << 7)  /*For Center MC Only*/
#define TS1K_RXMAGRSPFLAG (0x1 << 13)    /*For Center MC Only*/
#define TS1K_RXINVDRSPFLAG (0x1 << 14) /*For Center MC Only*/
#define TS1K_RXMAGREQFLAG (0x1 << 15) /*For Terminal MC only*/


#define TS1K_DISABLED  -2
#define TS1K_NOTRXRSP -3  /*Not receive the response*/
#define TS1K_RXINVDRSP -4 
#define TS1K_NOTRXMAGRSP -5
#define TS1K_MAGICNUMERR -6
#define TS1K_RXBUSYRSP -7 /*Rx busy response*/
#define TS1K_DISREMORW -8  /*center does not enable remote rw ability*/
#define TS1K_NOTCEN -9       /*not center*/

#define TS1K_OUI_B0 0X00    /*Realtek OUI 00-E0-4C*/
#define TS1K_OUI_B1 0XE0
#define TS1K_OUI_B2 0X4C

#define TS1K_STATUSREQ_C 0X0206
#define TS1K_STATUSREP_C 0X020C
#define TS1K_LBSTARTREQ_C 0X0106
#define TS1K_LBSTARTREP_C 0X010C
#define TS1K_LBENDREQ_C 0X0006
#define TS1K_LBENDREP_C 0X000C
#define TS1K_INDITUP_C 0X0208
#define TS1K_INDITDOWN_C 0X020A

#define TS1K_OAMKEY_B0  0X4B  /*OAMKEY 0x5453314B*/
#define TS1K_OAMKEY_B1  0X31
#define TS1K_OAMKEY_B2  0X53
#define TS1K_OAMKEY_B3  0X54
#define TS1K_OAMKEY_PAG 13
#define TS1K_OAMKEY_REG0 20
#define TS1K_OAMKEY_REG1 21
#define TS1K_OAMKEY_REG2 22
#define TS1K_OAMKEY_REG3 23




/*IEEE 802.3ah OAM */
#define DOT3AH_FIBERPKT2CPU (0x3 << 0)
    #define  RTL8363_DOT3AHOAM_NORFWD 0
    #define  RTL8363_DOT3AHOAM_TRAPCPU 1
    #define  RTL8363_DOT3AHOAM_DISCARD 2
    #define  RTL8363_DOT3AHOAM_LB 3
#define DOT3AH_DISOTHPORTPKT (0x1 << 2)
#define DOT3AH_TRIGGEREVENGEN (0x1 << 3)
#define DOT3AH_ENAUTOCREVENOAMTX (0x1 << 4)
#define DOT3AH_ENABLE  (0x1 << 5)
#define DOT3AH_LBDASA_MASK (0x3 << 6)
#define DOT3AH_LBDASA_OFFSET 6
    #define RTL8363_DOT3AHOAM_UNDOSADA 0
    #define RTL8363_DOT3AHOAM_SA  1
    #define RTL8363_DOT3AHOAM_DA  2
    #define RTL8363_DOT3AHOAM_SADA  3
    
#define DOT3AH_LINKFAULT (0x1 << 0)
#define DOT3AH_DYINGGASP (0x1 << 1)
#define DOT3AH_CRITICALEVENT (0x1 << 2)
#define DOT3AH_LOCALEVALUATING (0x1 << 3)
#define DOT3AH_LOCALSTABLE (0x1 << 4)
#define DOT3AH_REMOTEEVALUATING (0x1 << 5)
#define DOT3AH_REMOTESTABLE (0x1 << 6)



#define RTL8363_PORT_RX  0
#define RTL8363_PORT_TX  1
#define RTL8363_QUEUE0	0
#define RTL8363_QUEUE1	1
#define RTL8363_QUEUE2	2
#define RTL8363_QUEUE3	3
#define RTL8363_MAXRATE 0X4000 /*1000Mbps, times of 64Kbps*/


#define RTL8363_ACL_PRIO 0
#define RTL8363_DSCP_PRIO 1
#define RTL8363_1QBP_PRIO 2
#define RTL8363_PBP_PRIO 3
#define RTL8363_CPUTAG_PRIO 4
#define RTL8363_IPB_PRIO 5
#define RTL8363_1QDEFAULT_PRIO 6
#define RTL8363_1QTAG_PRIO 7


#define RTL8363_PRIO0		0
#define RTL8363_PRIO1		1
#define RTL8363_PRIO2		2
#define RTL8363_PRIO3		3

#define RTL8363_PORT_NUMBER 3
#define RTL8363_PORT0 		0x0
#define RTL8363_PORT1 		0x1
#define RTL8363_PORT2 		0x2
#define RTL8363_NOCPUPORT 3
#define RTL8363_ANYPORT 0x3

#define RTL8363_DSCP_EF			0
#define RTL8363_DSCP_AFL1		1
#define RTL8363_DSCP_AFM1		2
#define RTL8363_DSCP_AFH1		3
#define RTL8363_DSCP_AFL2		4	
#define RTL8363_DSCP_AFM2		5		
#define RTL8363_DSCP_AFH2		6		
#define RTL8363_DSCP_AFL3		7			
#define RTL8363_DSCP_AFM3		8
#define RTL8363_DSCP_AFH3		9
#define RTL8363_DSCP_AFL4		10	
#define RTL8363_DSCP_AFM4		11		
#define RTL8363_DSCP_AFH4		12
#define RTL8363_DSCP_NC		13
#define RTL8363_DSCP_REG_PRI	14			
#define RTL8363_DSCP_BF			15

#define RTL8363_DSCP_USERA		0
#define RTL8363_DSCP_USERB		1
#define RTL8363_IPADD_A	0
#define RTL8363_IPADD_B	1

#define RTL8363_FCOFF			      0x0
#define RTL8363_FCON				0x1
#define RTL8363_FCO_DSC			0x0
#define RTL8363_FCO_QLEN	 		0x1
#define RTL8363_FCO_FULLTHR		0x0
#define RTL8363_FCO_OVERTHR 		0x1


/*ACL Function related MACRO*/
#define RTL8363_ACL_ENTRYNUM	16

#define RTL8363_ACL_INVALID		0x0
#define RTL8363_ACL_PORT0          0x1
#define RTL8363_ACL_PORT1          0x2
#define RTL8363_ACL_PORT2          0x3
#define RTL8363_ACL_PORT01        0x4
#define RTL8363_ACL_PORT02        0x5
#define RTL8363_ACL_PORT12        0x6
#define RTL8363_ACL_ANYPORT  	0x7

#define RTL8363_ACL_ETHER		0x0
#define RTL8363_ACL_TCP			0x1
#define RTL8363_ACL_UDP			0x2
#define RTL8363_ACL_TCPUDP		0x3

/*ACL Packet processing method*/
#define RTL8363_ACT_DROP 	0x0      /*drop the packet		*/
#define RTL8363_ACT_PERMIT	0x1	    /*permit the packet		*/
#define RTL8363_ACT_TRAPCPU	0x2	    /*trap the packet to cpu*/		
#define RTL8363_ACT_MIRROR	0x3      /*mirror the packet  */
#define RTL8363_ACT_FLOOD	0x4	    /*flood the packet	*/	

/*LUT macro*/
#define RTL8363_LUT_ENTRY0		0
#define RTL8363_LUT_ENTRY1		1
#define RTL8363_LUT_ENTRY2		2
#define RTL8363_LUT_ENTRY3		3
#define RTL8363_LUT_FULL               -2  /*Four way of the same entry are all written by cpu*/
#define RTL8363_LUT_NOTEXIST     -3
#define RTL8363_LUT_DYNAMIC		0
#define RTL8363_LUT_STATIC		1
#define RTL8363_LUT_UNAUTH		0
#define RTL8363_LUT_AUTH			1

#define RTL8363_CPUPORT_MASK (0x3 << 4)
#define RTL8363_CPUPORT_OFFSET 4
#define RTL8363_CPUTAGIST    (0x1 << 3)  /*Enable inserting CPU tag bit*/
#define RTL8363_CPUTAGRM    (0x1 << 2)  /*Enable remove CPU tag bit*/
#define RTL8363_CPUTAGAW   (0x1 << 1)  /*Enable CPU tag aware*/
#define RTL8363_CPUTAGCRCCHK (0x1 << 0) /*Disable CRC check for the CPU tagged packet*/

/*IGMP/MLD SNOOPNG macro*/
#define RTL8363_IGMPSNP 0
#define RTL8363_MLDSNP 1
#define HWSNP_ENSIPSR (0x1 << 5)    
#define HWSNP_ENTAGIM (0x1 << 4)        /*Enable snooping the CPU tagged IGMP/MLD packet in the hardware snooping*/ 
#define HWSNP_CNTPRD (0x1 << 3)         /*Group member serval granularity 0:4 seconds(default) 1 :8 seconds*/
#define HWSNP_DISUCMLD (0x1 << 2)
#define IGMP_ADPM_MASK (0x7UL << 12) /*The added port mask. Record the user assigned member port and router port for all the groups in hardware snooping*/
#define IGMP_ADPM_OFFSET 12
#define IGMP_LMQC_MASK (0x3 << 10)    /*Last Member Query Count for snooping*/
#define IGMP_LMQC_OFFSET 10
#define IGMP_RV_MASK (0x3 << 8)         /*Robustness value for snooping*/
#define IGMP_RV_OFFSET 8
#define IGMP_GQI_MASK (0xFFUL << 8)  /*General query serval in unit of second, default 125 */
#define IGMP_GQI_OFFSET 8
#define IGMP_V1MAXRSP 0xFF
#define MLD_ADPM_MASK (0x7UL << 4)
#define MLD_ADPM_OFFSET 4
#define MLD_LMQC_MASK (0x3 << 2)
#define MLD_LMQC_OFFSET 2
#define MLD_RV_MASK (0x3)
#define MLD_RV_OFFSET 0
#define MLD_GQI_MASK (0xFFUL << 8)
#define MLD_GQI_OFFSET 8

/*802.1x*/


/*storm filter*/



/* Register access macro (REG*()).
*/
#define RTL8363_REG32(reg) 			(*((volatile u32 *)((reg)*(RTL8363_BUSMUL_ISA) + (RTL8363_BASEADDR_ISA))))
#define RTL8363_REG16(reg) 			(*((volatile u16 *)((reg)*(RTL8363_BUSMUL_ISA) + (RTL8363_BASEADDR_ISA))))
#define RTL8363_REG8(reg) 			(*((volatile u8 *)((reg)*(RTL8363_BUSMUL_ISA) + (RTL8363_BASEADDR_ISA))))
#define WRITE_ISA32(reg,val)  RTL8363_REG32(reg)= (val)
#define WRITE_ISA16(reg,val)  RTL8363_REG16(reg)= (val)
#define WRITE_ISA8(reg,val)    RTL8363_REG8(reg)= (val)
#define READ_ISA32(reg)         RTL8363_REG32(reg)
#define READ_ISA16(reg)         RTL8363_REG16(reg)
#define READ_ISA8(reg)           RTL8363_REG8(reg)

enum TrigLBPktAct
{
    TriggerUsrOAM = 14,       /*Trigger user defined TS-1000 OAM frame*/
    TriggerAutoLB =  12 ,     /*Trigger auto loop back test process for center MC*/
    TriggerLBStartReq = 11, /*Trigger a loop back test start request to terminal-side*/
    TriggerLBEndReq = 10,  /*Trigger loop back test end request to terminal-side*/    
    TriggerStatusReq = 9,   /*Trigger status notification request to terminal-side*/
    TriggerGenTestPkt = 8, /*Trigger loop back test frame to terminal-side*/
};

enum ISAPAG
{
    ISA0_127 = 0,
    ISA128_255 = 1,
    ISA256_383 = 2,
}; 
enum MIBOpType
{
    RELEASE = 0,
    STOP = 1,
    RESET = 2,    
};

enum SPTPortState
{
    DISABLE = 0,
    BLOCK, 
    LEARN, 
    FORWARD,
};
    
enum SpecialMac
{
    NIPMLT = 0,           /*non-IP multicast*/
    BRD = 1,               /*broadcast*/        
    PIGMP = 2,           /*IGMP in PPPoE frame*/
    PMLD = 3,             /*MLD(IPv6 ICMP) in PPPoE frame*/
    IGMP = 4,             /*IGMP packet but not PPPoE frame*/
    MLD = 5,              /*MLD(IPv6 ICMP) but not PPPoE frame*/
    RSV_XX = 6,         /*reserved multicast 01-80-c2-00-00-xx (xx != 00,01,02,03,10,20,21,04~0F,22~2F)*/
    RSV_22 = 7,         /*reserved multicast 01-80-c2-00-00-22~2F*/
    RSV_04 = 8,         /*reserved multicast 01-80-c2-00-00-04~0F*/
    RSV_21 = 9,         /* reserved multicast 01-80-c2-00-00-21*/
    RSV_20 = 10,       /*reserved multicast 01-80-c2-00-00-20*/
    RSV_10 = 11,       /*reserved multicast 01-80-c2-00-00-10*/
    RSV_03 = 12,       /*reserved multicast 01-80-c2-00-00-03*/
    RSV_02 = 13,       /*reserved multicast 01-80-c2-00-00-02*/
    RSV_01 = 14,       /*reserved multicast 01-80-c2-00-00-01*/
    RSV_00 = 15,       /*reserved multicast 01-80-c2-00-00-00*/
    PIP6MLT = 16,      /*IPv6 multicast in PPPoE frame*/
    PIP4MLT = 17,      /*IPv4 multicast in PPPoE frame*/
    KIP6MLT = 18,      /*Known IPv6 multicast*/
    KIP4MLT = 19,      /*Known IPv4 multicast*/        
    UDA1 = 20,          /*user defined address 1*/  
    UDA2 = 21,           /*user defined address 2*/
    UDA3 = 22,           /*user defined address 3*/     
    UDA4 = 23,           /*user defined address 3*/  
    UKU = 24,             /*unkown DA unicast*/
};

enum ChipID
{
    RTL8213M,
    RTL8203M,
    RTL8363H,
    RTL8303H,
    RTL8363S,
};

enum VerNum
{
    VER_A,
    VER_B,
    VER_C,
    VER_D,
};

enum GPIO
{
    GA0,
    GA1, 
    GA2, 
    GA3,
    GA4,    
    GB0,
    GB1,
    GB2,
    GB3,
};

typedef struct phyCfg_s
{
      u8	  AutoNegotiation;    /*PHY register 0.12 setting for auto-negotiation process*/
      u8  Speed;                  /*PHY speed, [0.6 0.13]:00= 10Mpbs, 01= 100Mpbs,10= 1000Mpbs,11=Reserved */
                #define   PHY_10M       0x0
                #define   PHY_100M     0x1
                #define   PHY_1000M   0x2                 
      u8 Fullduplex;           /*full or half duplex*/
      u8 Cap_10Half;		  /*PHY register 4.5 setting for 10BASE-TX half duplex capable*/
      u8 Cap_10Full;		  /*PHY register 4.6 setting for 10BASE-TX full duplex capable*/
	u8 Cap_100Half;        /*PHY register 4.7 setting for 100BASE-TX half duplex capable*/
	u8 Cap_100Full;	  /*PHY register 4.8 setting for 100BASE-TX full duplex capable*/
	u8 Cap_1000Full;	  /*PHY register 9.9 setting for 1000BASE-T full duplex capable*/
	u8 FC;			        /*PHY register 4.10 setting for flow control capability*/
	u8 AsyFC;		        /*PHY register 4.11 setting for  asymmetric flow control capability*/
} phyCfg_t;

typedef struct Ts1kCfgPara_s
{
    u8 EnTS1K;
    u8 MCIdnt;
    u8 EnPwrFTx;
    u8 SpMCOptionA;
    u8 spMCOptionB;
    u8 EnAutoStasInd;
} Ts1kCfgPara_t;

typedef struct TS1kLBPara_s
{
    u8 enTimer2;
    u8 LB_Timer1; /*set loopback timer1*/
        #define LB_TIMER1_2000MS 0
        #define LB_TIMER1_4000MS 1
        #define LB_TIMER1_8000MS 2
        #define LB_TIMER1_16000MS 3
    u8 LB_Timer2;/*set loopback timer2*/
        #define LB_TIMER2_1000MS 0
        #define LB_TIMER2_1200MS 1
        #define LB_TIMER2_1600MS 2
        #define LB_TIMER2_1800MS 3
    /*When TriggerAutoLB, for center MC, if Tx LBStartReq 
    and not received LBStartRps, MaxRetryAutoLBStartReq 
    define the max retry times.*/
    u8 MaxRetryAutoLBStartReq;
        #define LB_MAXRETRY_2 0
        #define LB_MAXRETRY_4 1
        #define LB_MAXRETRY_8 2
        #define LB_MAXRETRY_16 3
    /*When TriggerAutoLB, for center MC, 
    if Tx LBStartReq and not received LBStartRps,
    MaxLBStartReqInv define the max time out serval.*/    
    u8 MaxLBStartReqInv;
        #define LB_RETYRINV_20MS 0
        #define LB_RETYRINV_40MS 1
    u8 OAMLBTFDA[6];/*Customer configuration OAM Loop back test frame's DA address*/
    u8 LB_Interval; /*Loop test Frame serval*/
        #define LB_INTERVAL_10MS 0
        #define LB_INTERVAL_20MS 1
        #define LB_INTERVAL_40MS 2
        #define LB_INTERVAL_80MS 3    
    u8 LoopTestFramLen; /*Loop test Frame length*/
        #define LB_TESTFRM_64B 0
        #define LB_TESTFRM_128B 1
        #define LB_TESTFRM_512B 2
        #define LB_TESTFRM_1518B 3        
    u8 LoopTestFramPattern;/*Loop test Frame pattern*/ 
        #define LB_TESTFRM_0000 0
        #define LB_TESTFRM_FFFF 1
        #define LB_TESTFRM_INC   2
        #define LB_TESTFRM_55AA 3
    u8 TxLBpktNum; /*Packet number for TS-1000 Loop back test sending*/

} TS1kLBPara_t;

typedef struct TS1kLBDebugCnt_s
{
    u16 RxOAMFrameCnt;
    u8 RxOAMCRCCnt;
    u8 RxStartReqOAMCnt;
    u8 RxStartRspOAMCnt;
    u8 RxEndReqOAMCnt;
    u8 RxEndRspOAMCnt;
    u8 RxEndIndOAMCnt;
    u8 RxStatReqOAMCnt;
    u8 RxStatRspOAMCnt;
    u8 RxStatIndOAMCnt;
} TS1kLBDebugCnt_t;

typedef struct TS1kRemoteRWPara_s
{
    u8 RTMOP;
        #define TS1K_RTMOP_MASK (0x3 << 8)
        #define TS1K_RTMOP_OFFSET 8
        #define TS1K_RTMOP_READ     0         /*remote Read*/
        #define TS1K_RTMOP_WRITE    1        /* remote Write*/
        #define TS1K_RTMOP_INVADREP 2      /*Invalid Remote Read/write Response*/
        #define TS1K_RTMOP_BUSYREP 3       /*Busy response*/
    u8 ISAREG;
    u16 ISAPAG;
    u8 RMRWData;
    u8 RxMagReqFlag;
    u8 RxInvdRspFlag;
    u8 RxMagRspFlag;  
    u8 RxBusyRspFlag;
} TS1kRemoteRWPara_t;

typedef struct DOT3AHPara_s
{
    u8 enOAM;
    u8 enAutoCrEvnTx;
    u8 enLBChDASA;
    u8 ahLBChDA[6];
    u8 ahLBChSA[6];
} DOT3AHPara_t;

typedef struct FWDmode_s
{
    u8 mode;
        #define RTL8363_FWD_PT   3   /*pass through mode*/
        #define RTL8363_FWD_SPT 2  /*smart pass through mode*/
        #define RTL8363_FWD_MCT 1  /*modified cut through mode*/
        #define RTL8363_FWD_NOR 0  /*normal mode*/
}  FWDmode_t;
typedef struct LUTPara_s
{
    u8 type;
        #define LUT_L2DYN   0   /*Layer2 dynamic entry*/
        #define LUT_L2STA   1   /*Layer2  static entry*/
        #define LUT_IGMPV3 2  /*IP Multicast entry for IGMPV3*/
        #define LUT_HDSNP   3 /*IP Multicast entry for hardware IGMP/MLD snooping*/
    u8 mac[6];
        #define LUT_MAC5_MASK 0XFF
        #define LUT_MAC5_OFFSET 0
        #define LUT_MAC4_MASK (0XFFUL << 8)        
        #define LUT_MAC4_OFFSET 8
        #define LUT_MAC3_MASK 0XFF
        #define LUT_MAC3_OFFSET 0
        #define LUT_MAC2_MASK (0XFFUL << 8)        
        #define LUT_MAC2_OFFSET 8
        #define LUT_MAC1_MASK 0XFF
        #define LUT_MAC1_OFFSET 0
        #define LUT_MAC0_MASK (0XFFUL << 8)                
        #define LUT_MAC0_OFFSET 8        
    u8 port;
        #define LUT_SPA_MASK (0x3 << 4)
        #define LUT_SPA_OFFSET 4
        #define LUT_MBR_MASK      (0x7 << 4)
        #define LUT_MBR_OFFSET   4        
    u8 isStatic;
        #define LUT_IP_MULT (0x1 << 13)
        #define LUT_STATIC (0x1 << 12)
    u8 isAuth;
        #define LUT_AUTH (0x1 << 11)        
    u8 age;
		#define LUT_AGE_MASK (0X3 << 6)
		#define LUT_AGE_OFFSET 6
        #define RTL8363_LUT_AGEOUT 		0
        #define RTL8363_LUT_AGE100		0x1
        #define RTL8363_LUT_AGE200		0x3
        #define RTL8363_LUT_AGE300		0x2        
    u8 enblk;  /* indicate that the packets from source MAC address ID of this entry will be dropped*/
        #define LUT_BLK (0x1 << 10)
    u32 dip;
    u32 sip;    
    u16 gmitv_p[3]; /*each port group member serval, time is second*/ 
        #define LUT_GMITV_P2_MASK   (0x7FUL << 6)
        #define LUT_GMITV_P2_OFFSET 6
        #define LUT_GMITV_P0_MASK   (0x7FUL << 8 )
        #define LUT_GMITV_P0_OFFSET 8
        
} LUTPara_t;


typedef struct SNPCfg_S
{
    u8 ensnp ;
    u8 enfastlv;
    u8 adpm; /*The added port mask. Record the user assigned member port and router port for all the groups in snooping*/    
} SNPCfg_t;

typedef struct SNPPara_S
{
    u8 GQI;
    u8 RV;
    u8 LMQC;
    u8 ENTAGIM;
    u8 CNTPRD;
    u8 enUCMLD;
    u8 V1MAXRSP;        
    
} SNPPara_t;

typedef struct QosPRatePara_S
{
    u8 enabled;     /*Enable or disable rate limit */
    //u8  enIPGPRE; /* Enable concern preamble and IPG when calculating the  rate */
    u16 rate;     /*Rate of the port in times of 64Kbps*/    
}  QosPRatePara_t;


typedef struct QosPSchePara_S
{
    u8 q3_wt;
    u8 q2_wt;
    u8 q1_wt;
    u8 q0_wt;
    u8 q3_enstrpri;
    u8 q2_enstrpri;
    u8 q3_enLB;
    u8 q2_enLB;
    u8 q3_bursize;
    u8 q2_bursize;
    u16 q3_rate;
    u16 q2_rate;    
} QosPSchePara_t;

typedef struct QosPri2QIDMap_S
{
    u8 qid_pri3;
    u8 qid_pri2;
    u8 qid_pri1;
    u8 qid_pri0;    
} QosPri2QIDMap_t;

typedef struct QosPriAssign_S
{
    u8 lev_pbp; /*port-based priority level*/
    u8 lev_1qbp; /*1Q-based priority level*/
    u8 lev_acl;   /*ACL-based priority level*/
    u8 lev_dscp; /*DSCP-based priority level*/
    
} QosPriAssign_t;

typedef struct QosPriEnable_S
{
    u8 enPB;
    u8 en1QB;
    u8 enDSCPB;
    u8 enIPB;
    u8 enCPUTAGB;
} QosPriEnable_t;

typedef struct QosIPPri_S
{
    u32 ip;
    u32 ipmask;
    u8 enabled;
} QosIPPri_t;

typedef struct QosQFC_S
{
    u8 q0_ON;
    u8 q0_OFF;
    u8 q1_ON;
    u8 q1_OFF;
    u8 q2_ON;
    u8 q2_OFF;
    u8 q3_ON;
    u8 q3_OFF;
    u8 q0_DROP; /*drop threshold when flow control disabled*/
    u8 q1_DROP;
    u8 q2_DROP;
    u8 q3_DROP;
} QosQFC_t;


typedef struct QosPFC_S
{
    u8 dscon;
    u8 dscoff;
    u8 dscdrop;
} QosPFC_t;

typedef struct QosFCEn_S
{
    u8 enpfc; /*enable/disable port-based output flow control*/
    u8 qfcmask; /*enable queue -based flow control mask*/
} QosFCEn_t;

typedef struct QosQsize_S
{
    /*Each port total entry is 128, 
    so for 4 queue mode, q3_size = 128 - (q0_size + q1_size + q2_size)*/
    u8 q0_size;
    u8 q1_size;
    u8 q2_size; 
    u8 q3_size;
} QosQsize_t;

typedef struct QosDebug_S
{
    u8 DscrunoutFlag;
    u8 MaxpktBuf;
    u8 Pn_MaxTxDscCount[3];
    u8 Pn_MaxRxDscCount[3];    
    u8 Pn_MaxPktCount[3];
    u8 PnQ0_MaxDscCount[3];
    u8 PnQ1_MaxDscCount[3];
    u8 PnQ2_MaxDscCount[3];    
    u8 PnQ3_MaxDscCount[3];   
    u8 PnQ0_MaxPktCount[3];
    u8 PnQ1_MaxPktCount[3];
    u8 PnQ2_MaxPktCount[3];    
    u8 PnQ3_MaxPktCount[3];  
} QosDebug_t;


typedef struct FwdSpecialMac_S
{
    u8 fwd_bhv;
        #define RTL8363_FWD_NORMAL 0
        #define RTL8363_FWD_EXCPU  1
        #define RTL8363_FWD_TRAPCPU 2
        #define RTL8363_FWD_DROP 3
    u8 rgnPri;
    u8 enRgnPri;
    u8 enUDA;      
} FwdSpecialMac_t;

typedef struct mirrorPara_s
{
    u8 mirport;
    u8 rxport;
    u8 txport;
    u8 mac[6];
    u8 enMirMac;
} mirrorPara_t;


typedef struct stormPara_s
{
    u8 enBRD;
    u8 enMUL;
    u8 enUDA;
    u8 trigNum;
        #define RTL8363_STM_FILNUM64      0  /*continuous 64 pkts will trigger storm fileter*/
        #define RTL8363_STM_FILNUM32      1  /*continuous 32 pkts will trigger storm fileter*/
        #define RTL8363_STM_FILNUM16      2  /*continuous 16 pkts will trigger storm fileter*/
        #define RTL8363_STM_FILNUM8       3  /*continuous 8 pkts will trigger storm fileter*/    
    u8 filTime;
        #define RTL8363_STM_FIL800MS      0  /*filter 800ms after trigger storm filter*/
        #define RTL8363_STM_FIL400MS      1  /*filter 400ms after trigger storm filter*/
        #define RTL8363_STM_FIL200MS      2  /*filter 200ms after trigger storm filter*/
        #define RTL8363_STM_FIL100MS      3  /*filter 100ms after trigger storm filter*/        
    u8 onlyTmClr;    
} stormPara_t;

typedef struct wolCfg_s
{
    u8 WOLEn; /*enable/disable WOL mode*/
    u8 LUEn;   /*whether support link up event wake up CPU*/
    u8 MPEn ; /*whether support Magic packet wake up CPU*/
    u8 WFEn; /*whether support wake-up packet wake up CPU, the wake-up packet is defined by user*/
    u8 WOLSerdes; /*whether Serdes can work in WOL mode, set it ture when using fiber*/
    u8 WSig;  /*wake-up signal*/
       #define RTL8363_SIG_HIGH 0 /*active high*/
       #define RTL8363_SIG_LOW 1  /*active low*/
       #define RTL8363_SIG_POSPUL 2 /*positive high pulse*/
       #define RTL8363_SIG_NEGPUL 3 /*positive low pulse*/
       #define RTL8363_SIG_NONE 4 /*disable wake-up signal output*/
    u8 nodeID[6]; /*node ID in magic packet, it  is switch NIC MAC address*/
} wolCfg_t;

typedef struct wakeupFrameMask_s
{
    u16 crc;
    u16 Wakeupmask[8];
} wakeupFrameMask_t;

typedef struct GPIOcfg_s
{
    u8 dir; /*true = output*/
    u8 ens; /*enable gpio pin generate serrupt, the configuration*/
    u8 ssig; /*signal property will cause serrupt, the configuration will affect whole gpio group*/    
} GPIOcfg_t;

typedef struct WRRPara_s
{
    u8 q3_wt;
    u8 q2_wt;
    u8 q1_wt;
    u8 q0_wt;
    u8 q3_enstrpri;
    u8 q2_enstrpri;
} WRRPara_t;

void  mac2index(u8 *macAddr, u32 *pIndex);
void  ip2index(u32 sip, u32 dip, u32 *pIndex);

//Added for SPR ETH driver [Start]
void rtl8368_printEthernetPHYRegs(u8 phy);
//Added for SPR ETH driver [End]

s8 rtl8363_setAsicReg(u8 phyad, u8 regad, u8 pagtype,  u8 npage, u16 value);
s8 rtl8363_getAsicReg(u8 phyad, u8 regad, u8 pagtype,  u8 npage, u16 *pvalue);
s8 rtl8363_setAsicRegBit(u8 phyad, u8 regad, u8 bitpos, u8 pagtype, u8 npage,  u16 value) ;
s8 rtl8363_getAsicRegBit(u8 phyad, u8 regad, u8 bitpos, u8 pagtype, u8 npage,  u16 * pvalue); 
s8 rtl8363_setAsicEthernetPHY(u8 phy, phyCfg_t cfg);
s8 rtl8363_getAsicEthernetPHY(u8 phy, phyCfg_t* cfg);
s8 rtl8363_getAsicPHYLinkStatus(u8 phy, u8 *linkStatus);
s8 rtl8363_setAsicPortLearnAbility(u8 port, u8 enabled);    
s8 rtl8363_getAsicPortLearnAbility(u8 port, u8 *pEnabled);
s8 rtl8363_setAsicSoftReset(void);
s8 rtl8363_setAsicFWDMode(FWDmode_t fwd);
s8 rtl8363_getAsicFWDMode(FWDmode_t *pFwd);
s8 rtl8363_setAsicDot3ahOAMEnable(u8 port, DOT3AHPara_t *pConfig);
s8 rtl8363_getAsicDot3ahOAMEnable(u8 port, DOT3AHPara_t *pConfig);
s8 rtl8363_setAsicDot3ahOAMPortState(u8 port, u8 enOth2Cur, u8 curState);
s8 rtl8363_getAsicDot3ahOAMPortState(u8 port, u8* pEnOth2Cur, u8 *pCurState);
s8 rtl8363_setAsicDot3ahOAMTrigFlag(u8 port, u8 enTrigEvnGen, u8 flagVal);
s8 rtl8363_getAsicDot3ahOAMTrigFlag(u8 port, u8 *pFlagVal);
s8 rtl8363_setAsicTs1kOAMEnable(u8 port, Ts1kCfgPara_t cfg);
s8 rtl8363_getAsicTs1kOAMEnable(u8 port, Ts1kCfgPara_t* pCfg);
s8 rtl8363_setAsicTs1kOAMSField(u8 port, u16 sval);
s8 rtl8363_getAsicTs1kOAMSField(u8 port, u16* psval);
s8 rtl8363_setAsicTs1kOAMLBPara(u8 port, TS1kLBPara_t lbpara);
s8 rtl8363_getAsicTs1kOAMLBPara(u8 port, TS1kLBPara_t *pLbpara);
s8 rtl8363_setAsicTs1kOAMTrigFrame(u8 port, enum TrigLBPktAct trigact);
s8 rtl8363_getAsicTs1kOAMAutoLBResult(u8 port, u8* pIsEnd, u8* pIsGood );    
s8 rtl8363_setAsicTs1kOAMForceExtLB(u8 port);
s8 rtl8363_setAsicTs1kOAMTxUsrFrame(u16 UsrOAMTxC, u16 UsrOAMTxS , u8* UsrOAMTxM);
s8 rtl8363_getAsicTs1kOAMTxUsrFrame(u16 *pUsrOAMTxC, u16 *pUsrOAMTxS , u8* pUsrOAMTxM);
s8 rtl8363_getAsicTs1kOAMRxUsrFrame(u8 port, u8 *pRxUserOAMFlag,  u16 *pUsrOAMTxC, u16 *pUsrOAMTxS , u8* pUsrOAMTxM);
s8 rtl8363_getAsicTs1kOAMRxFrame(u8 port, u16* pOAMRxBLPC, u16* pOAMRxBLPS, u8* pOAMRxBLPM);
s8 rtl8363_getAsicTs1kOAMLBFlag(u8 port, u16* flagmask);
s8 rtl8363_getAsicTs1kOAMLBCounter(u8 port, u8 *pTxLBpktCnt, u8 *pRxLBpktCnt, u8 *pRxLBpktErrorCnt, u8 *pPsthrLBpktcnt);    
s8 rtl8363_setAsicTs1kOAMLBDebugClr(u8 port);
s8 rtl8363_setAsicTs1kOAMDebugCntCtl(u8 port, u8 enabled);
s8 rtl8363_getAsicTs1kOAMDebugCnt(u8 port, TS1kLBDebugCnt_t* pLBDebugCnt);
s8 rtl8363_setAsicTs1kOAMRemoteRWAbility(u8 port, u8 EnLocalMagicOAM, u8 EnRemoteMagicOAM, u8 EnHwAutoMagicRsp);
s8 rtl8363_getAsicTs1kOAMRemoteRWAbility(u8 port, u8* pEnLocalMagicOAM, u8* pEnRemoteMagicOAM, u8* pEnHwAutoMagicRsp);
s8 rtl8363_setAsicTs1kOAMRemoteRWPageSel(u8 port, enum ISAPAG isapag);
s8 rtl8363_setAsicTs1kOAMTrigRemoteRW(u8 port, TS1kRemoteRWPara_t* pRmtRWPara );
s8 rtl8363_getAsicTs1kOAMRemoteRWinfo(u8 port, TS1kRemoteRWPara_t* pRmtRWPara);
s8 rtl8363_setAsicTs1kOAMRemoteRWEnable(u8 fibport, u8 enabled);
s8 rtl8363_setAsicTs1kOAMInt(u8 port, u16 smask);
s8 rtl8363_getAsicTs1kOAMInt(u8 port, u16 *pIntmask);
s8 rtl8363_getAsicTs1kOAMIntFlag(u8 port, u16 *pIntflag);
s8 rtl8363_setAsicEnableAcl(u8 enabled);
s8 rtl8363_getAsicEnableAcl(u8* enabled);
s8 rtl8363_setAsicAclEntry(u8 entryadd, u8 phyport, u8 action, u8 protocol, u16 dataval, u8 priority) ;
s8 rtl8363_getAsicAclEntry(u8 entryadd, u8 *phyport, u8 *action, u8 *protocol, u16  *dataval, u8 *priority) ;
s8 rtl8363_setAsicVlanEnable(u8 enabled) ;
s8 rtl8363_getAsicVlanEnable(u8* pEnabled);
s8 rtl8363_setAsicVlanTagAware(u8 enabled) ;
s8 rtl8363_getAsicVlanTagAware(u8 *pEnabled);
s8 rtl8363_setAsicVlanIngressFilter(u8 enabled) ;
s8 rtl8363_getAsicVlanIngressFilter(u8 *pEnabled) ;
s8 rtl8363_setAsicVlanTaggedOnly(u8 enabled);
s8 rtl8363_getAsicVlanTaggedOnly(u8 *pEnabled);
s8 rtl8363_setAsicVlan(u8 vlanIndex, u16 vid, u8 memberPortMask) ;
s8 rtl8363_getAsicVlan(u8 vlanIndex, u16 *pVid, u8 *pMemberPortMask) ;
s8 rtl8363_setAsicPortVlanIndex(u8 port, u8 vlanIndex);
s8 rtl8363_getAsicPortVlanIndex(u8 port, u8 *pVlanIndex) ;
s8 rtl8363_setAsicLeakyVlan(u8 enabled);
s8 rtl8363_getAsicLeakyVlan(u8 *pEnabled);
s8 rtl8363_setAsicArpVlan(u8 enabled);
s8 rtl8363_getAsicArpVlan(u8 *pEnabled);
s8 rtl8363_setAsicMulticastVlan(u8 enabled) ;
s8 rtl8363_getAsicMulticastVlan(u8 *pEnabled) ;
s8 rtl8363_setAsicMirrorVlan(u8 enabled) ;
s8 rtl8363_getAsicMirrorVlan(u8 *pEnabled);
s8 rtl8363_setAsicNullVidReplaceVlan(u8 port, u8 enabled) ;
s8 rtl8363_getAsicNullVidReplaceVlan(u8 port, u8 *pEnabled);
s8 rtl8363_setAsicVlanTagInsertRemove(u8 port, u8 option) ;
s8 rtl8363_getAsicVlanTagInsertRemove(u8 port, u8 *pOption) ;
s8 rtl8363_setAsicVlanTrapToCPU(u8 enabled) ;
s8 rtl8363_getAsicVlanTrapToCPU(u8 *pEnabled);
s8 rtl8363_setAsic1pRemarkingVlan(u8 port, u8 enabled);
s8 rtl8363_getAsic1pRemarkingVlan(u8 port, u8 *pEnabled);
s8 rtl8363_setAsic1pRemarkingPriority(u8 pri, u8 pri1p);
s8 rtl8363_getAsic1pRemarkingPriority(u8 pri, u8 *pPri1p);
s32 rtl8363_setAsicLUT(u32 entry, LUTPara_t * pLut);
s32 rtl8363_getAsicLUT(u32 entry, LUTPara_t * pLut);
s8 rtl8363_setAsicCPUPort(u8 port, u8 enTag) ;
s8 rtl8363_getAsicCPUPort(u8 *pPort, u8 *pEntag);
s8 rtl8363_setAsicHWSNPEnable(u8 type, SNPCfg_t snp);
s8 rtl8363_getAsicHWSNPEnable(u8 type, SNPCfg_t *pSnp);
s8 rtl8363_setAsicHWSNP(u8 type, SNPPara_t snpcfg);
s8 rtl8363_getAsicHWSNP(u8 type, SNPPara_t* pSnpcfg);
s8 rtl8363_setAsicQosPortRate(u8 port, u8 direction, QosPRatePara_t portRate) ;
s8 rtl8363_getAsicQosPortRate(u8 port, u8 direction, QosPRatePara_t *pPortRate );
s8 rtl8363_setAsicQosPortScheduling(u8 port, QosPSchePara_t portSch);
s8 rtl8363_getAsicQosPortScheduling(u8 port, QosPSchePara_t* pPortSch );
s8 rtl8363_setAsicQosPortQueueNum(u8 port, u8 qnum) ;
s8 rtl8363_getAsicQosPortQueueNum(u8 port, u8 *pQnum);
s8 rtl8363_setAsicQosPrioritytoQIDMapping(u8 port, QosPri2QIDMap_t priqid) ;
s8 rtl8363_getAsicQosPrioritytoQIDMapping(u8 port, QosPri2QIDMap_t *priqid) ;
s8 rtl8363_setAsicQosPrioritySourceArbit(QosPriAssign_t priassign);
s8 rtl8363_getAsicQosPrioritySourceArbit(QosPriAssign_t *pPriassign);
s8 rtl8363_setAsicQosPrioritySourcePriority(u8 pri_type, u8 pri_obj, u8 pri_val);
s8 rtl8363_getAsicQosPrioritySourcePriority(u8 pri_type, u8 pri_obj, u8 *pPri_val);
s8 rtl8363_setAsicQosIPAddress(u8 entry, QosIPPri_t ippri);
s8 rtl8363_getAsicQosIPAddress(u8 entry, QosIPPri_t *pIppri);
s8 rtl8363_setAsicQosPrioritySourceEnable(u8 port, QosPriEnable_t enQos) ;
s8 rtl8363_getAsicQosPrioritySourceEnable(u8 port, QosPriEnable_t *pEnQos);
s8 rtl8363_setAsicQosFCEnable(u8 port, QosFCEn_t enfc );
s8 rtl8363_getAsicQosFCEnable(u8 port, QosFCEn_t *pEnfc);
s8 rtl8363_setAsicQosQueueFCThr(u8 port, u8 type, QosQFC_t qfcth);    
s8 rtl8363_getAsicQosQueueFCThr(u8 port, u8 type, QosQFC_t *pQfcth);
s8 rtl8363_setAsicQosPortFCThr(u8 port, u8 direction, QosPFC_t pfc);
s8 rtl8363_getAsicQosPortFCThr(u8 port, u16 direction, QosPFC_t *pPfc);
s8 rtl8363_setAsicQosQueueSize(u8 port, u8 qnum, QosQsize_t qsize);
s8 rtl8363_getAsicQosQueueSize(u8 port, u8 qnum, QosQsize_t* pQsize);
s8 rtl8363_setAsicQosDebugCntClr(void);
s8 rtl8363_getAsicQosDebugCnt(QosDebug_t *pQosdbg);
s8 rtl8363_setAsicSpecialMACFWD(enum SpecialMac SpecialMac, FwdSpecialMac_t  fwdcfg);
s8 rtl8363_getAsicSpecialMACFWD(enum SpecialMac SpecialMac, FwdSpecialMac_t  *pFwdcfg);
s8 rtl8363_setAsicUsrDefMac(enum SpecialMac SpecialMac, u8* pMac);
s8 rtl8363_getAsicUsrDefMac(enum SpecialMac SpecialMac, u8* pMac);
s8 rtl8363_setAsicMIB(u8 port, enum MIBOpType mibop);
s8 rtl8363_getAsicMIB(u8 MIBaddr, u8* pMIBcnt);
s8 rtl8363_setAsic1dPortState(u8 port, enum SPTPortState state);
s8 rtl8363_getAsic1dPortState(u8 port, enum SPTPortState *pState);
s8 rtl8363_setAsicMirror(mirrorPara_t mir);
s8 rtl8363_getAsicMirror(mirrorPara_t *pMir);
s8 rtl8363_setAsicStormFilter(stormPara_t stm);
s8 rtl8363_getAsicStormFilter(stormPara_t *pStm);
s8 rtl8363_getAsicChipID(enum ChipID *id, enum VerNum *ver);
s8 rtl8363_setAsicGPIO(enum GPIO gpio_pin, GPIOcfg_t *pCfg);
s8 rtl8363_setAsicGpioPin(enum GPIO gpio_pin, u8 val);
s8 rtl8363_getAsicGpioPin(enum GPIO gpio_pin, u8 *pData);
s8 rtl8363_setAsicWolEnable(wolCfg_t *pWolCfg);
s8 rtl8363_setAsicWolWakeupFrame(u8 num, wakeupFrameMask_t *pMask);
s8 rtl8363_setAsicEnterSleepMode(u8 port);
s8 rtl8363_getAiscWolQueEmpty(u8 port , u8 * pIsEmpty);
s8 rtl8363_setAsicExitSleepMode(u8 port, WRRPara_t *pWrr );
s8 rtl8363_setAsicInit(void);
#endif

