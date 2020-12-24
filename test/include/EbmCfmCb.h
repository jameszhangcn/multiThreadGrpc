#ifndef EBM_CFM_CB_H
#define EBM_CFM_CB_H

#include "GrpcAdpStru.h"

extern void update_route_table(CuCpUpHdrG* cpUpHdr);

extern void send_sec_cfg_cfm();

extern char* get_ebm_ip_port(char* ebmID);


#endif