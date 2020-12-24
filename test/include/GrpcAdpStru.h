#ifndef __GRPC_ADP_STRU_H
#define __GRPC_ADP_STRU_H

#include <stdbool.h>

typedef unsigned char           uint8_t;  
typedef unsigned short int      uint16_t;  
#ifndef __uint32_t_defined  
typedef unsigned int            uint32_t;  
# define __uint32_t_defined  
#endif  


typedef enum 
{
    IPV4 = 0,
    IPV6 = 1
}IpAddressTypeG;

#define IP_ADDR_LEN_G 16
typedef struct IpAddressG {
    IpAddressTypeG type;
    char addr[IP_ADDR_LEN_G];
}IpAddressG;

typedef struct pstG
{
    uint32_t  dstProcID;
    uint32_t  srcProcID;
    uint32_t  dstEnt;
    uint32_t  dstInst;
    uint32_t  srcEnt;
    uint32_t  srcInst;
    uint32_t  prior;
    uint32_t  route;
    uint32_t  event;
    uint32_t  region;
    uint32_t  pool;
    uint32_t  selector;
    uint32_t  interfaceVersion;
    uint32_t  cellID;
}PstG;

typedef struct cuCpIDG
{
	uint32_t cpId;
	uint32_t cellId;
	uint32_t cpUeId;
	uint32_t cpRbId;
} CuCpIDG;

typedef struct cuUpIDG
{
	uint32_t upId;
	uint32_t cellId;
	uint32_t upUeId;
	uint32_t upRbId;
} CuUpIDG;

typedef struct suSpIDG
{
	uint32_t suId;
	uint32_t spId;
} SuSpIDG;

typedef struct cuCpUpHdrG
{
	PstG cuCpPst;
	CuCpIDG cuCpId;
	CuUpIDG cuUpId;
	SuSpIDG suSpId;
} CuCpUpHdrG;



typedef enum 
{
    DL = 0,
    UL = 1,
    Dl_DATA_FWD = 2,
    UL_DATA_FWD = 3,
    LIPA = 4
}TunnelTypeG;

typedef enum {
    S1 = 0,
    V1 = 1,
    X2 = 2
}InterfaceTypeG;

typedef enum {
    NONE = 0,
    S1_BASED = 1,
    X2_BASED = 2,
    INTRA_BBU = 3,
    INTRA_DU_BBU = 4,
    INTER_DU_BBU = 5
}HandoverTypeG;

#define CPJ_CIPHER_KEY_LEN_G 16

typedef struct SecurityConfigureG {
    uint32_t  algorithm_type;
    uint32_t encryption_key[CPJ_CIPHER_KEY_LEN_G] ;
}SecurityConfigureG;

typedef struct GtpTunnelIdG {
    uint32_t  ue_id;
    uint32_t  rb_id;
    TunnelTypeG  tnl_type;
    uint32_t  tnl_priority ;
}GtpTunnelIdG;

typedef struct GtpTunInfoG {
    GtpTunnelIdG tnl_id;
    InterfaceTypeG   interface_type;
    IpAddressG   dst_addr;
    IpAddressG   src_addr;
    uint32_t  rem_teid;
    bool    ho_src_enodeb;
    HandoverTypeG  ho_type;
    bool    id_fullCfg;
    uint32_t  rlc_mode;
}GtpTunInfoG;

typedef struct RlcInfoG {
    uint32_t  direction;
    uint32_t  mode;
    bool    status_rep_require;
    uint32_t  pdcp_sn_size;
}RlcInfoG;

#define MAX_POT_PROFILES_G 2
typedef struct HeaderCompressConfigureG {
    bool    hdr_comp_used;
    uint32_t  max_cid;
    uint32_t profile_list[MAX_POT_PROFILES_G];
}HeaderCompressConfigureG;

typedef struct PDCPConfigureG {
    uint32_t  sn_len;
    uint32_t  qci;
    uint32_t  discard_timer;
    RlcInfoG rlc_info;
    HeaderCompressConfigureG    hdr_cmp_cfg;
}PDCPConfigureG;




#endif
