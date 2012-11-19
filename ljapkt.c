/**
 * @file ljapkt.c
 * @brief �����������ݰ���ֻ����C��׼�⡣
 * @author LJA
 * @version 0.0.1
 * @date 2012-11-13
 */

#include  "ljapkt.h"
#include  <assert.h>
#include  <stdio.h>
#include  <arpa/inet.h>
/**
 * @brief ��ӡmac��ַ��(ʵ�ʾ��Ǵ�ӡmac����6���ֽ�)
 *
 * @param mac
 */
void display_mac(u_char *mac)
{
	assert(mac != NULL);

	u_int8 i;

	printf("%.2x",mac[0]);
	for(i = 1; i< 6; i++)
	{
		printf(":");
		printf("%.2x",mac[i]);
	}
	
	return ;
}

/**
 * @brief ������·�����ݰ���
 *
 * @param size  ���ݰ���С
 * @param data  ���ݰ���ʼλ��
 * @param info  �����ݰ�����·���н��������������Ϣ
 *
 * ��������: 
 *   ��̫��Э���װ��IPV4 ARP RARP IPV6 MPLS PPPOE(discovery and Session) 802.1Q 
 *   ieee802.2/802.3��װ��IPV4 ARP RARP IPV6 MPLS PPPOE(discovery and Session) 802.1Q 
 *
 */
void parse_data_linker(u_int16 size, u_char *data, net_info *info/**<[out] ���������ĵ��������Ϣ*/)
{
	assert(size >= 0);
	assert(data != NULL);
	assert(info != NULL);

	data_link_hdr *hdr = (data_link_hdr*)data;
	u_char* ret = NULL;

	info->data = NULL;
	info->type = NET_INVALID;
	info->size = 0;

	display_mac(hdr->dmac);
	printf(" <- ");
	printf("Src MAC: ");
	display_mac(hdr->smac);
	switch (htons(hdr->type))
	{
		case 0x0800 :  //IPv4
			printf("[IPv4]");
			info->data = data+14;
			info->type = IPV4;
			info->size = size - 18;  //6+6+2+4(dmac+smac+type+CRC)
			break;
		case 0x0806 :  //ARP
			printf("[ARP]");
			info->data = data+14;
			info->type = ARP;
			info->size = size - 18;  //6+6+2+4(dmac+smac+type+CRC)
			break;
		case 0x8035 :  //RAPP
			printf("[RARP]");
			info->data = data+14;
			info->type = RARP;
			info->size = size - 18;  //6+6+2+4(dmac+smac+type+CRC)
			break;
		case 0x86DD :  //IPv6
			printf("[IPv6]");
			info->data = data+14;
			info->type = IPV6;
			info->size = size - 18;  //6+6+2+4(dmac+smac+type+CRC)
			break;
		case 0x8847 :  //MPLS Label
			printf("[MPLS]");
			info->data = data+14;
			info->type = MPLS;
			info->size = size - 18;  //6+6+2+4(dmac+smac+type+CRC)
			break;
		case 0x8863 :  //PPPoe - Discovery
			printf("[PPPoe]");
			info->data = data+14;
			info->type = PPPOED;
			info->size = size - 18;  //6+6+2+4(dmac+smac+type+CRC)
			break;
		case 0x8864 :  //PPPoe - Session
			printf("[PPPoe]");
			info->data = data+14;
			info->type = PPPOES;
			info->size = size - 18;  //6+6+2+4(dmac+smac+type+CRC)
			break;
		case 0x8100 :  //802.1Q tag
			printf("[802.1Q]");
			info->data = data+14;
			info->type = p8021Q;
			info->size = size - 18;  //6+6+2+4(dmac+smac+type+CRC)
			break;
		default :      //IEEE802.2/802.3 rfc1084
			//TODO 
			printf("[IEEE802.2/802.3]");
			parse_ieee_8022_8023(hdr->type,data+14,info);
			break;
	}
	return ;
}

/**
 * @brief ������·���IEEE802.2/802.3
 *
 * @param size  IEEE802.2/802.3�еĳ����ֶ�ָ���ĳ���
 * @param data  IEEE802.2/802.3���ݰ���ʼλ��(�����ֶ��Ժ�)
 * @param info  �����ݰ�����·���н��������������Ϣ
 *
 * ��������: 
 *   ieee802.2/802.3��װ��IPV4 ARP RARP IPV6 MPLS PPPOE(discovery and Session) 802.1Q 
 */
void parse_ieee_8022_8023(u_int16 size, u_char *data, net_info *info/**<[out] ���������ĵ��������Ϣ*/)
{
	assert(size != 0);
	assert(data != NULL);
	assert(info != NULL);

	info->data = NULL;
	info->type = NET_INVALID;
	info->size = 0;

	ieee_8022_8023_hdr *hdr=(ieee_8022_8023_hdr *)data;
	switch (htons(hdr->type)){
		case 0x0800 :  //IPv4
			printf("[IPv4]");
			info->data = data+8;
			info->type = IPV4;
			info->size = size-8;  //ieee_8022_8033_hdr'size
			break;
		case 0x0806 :  //ARP
			printf("[ARP]");
			info->data = data+8;
			info->type = ARP;
			info->size = size-8;  //ieee_8022_8033_hdr'size
			break;
		case 0x8035 :  //RAPP
			printf("[RARP]");
			info->data = data+8;
			info->type = RARP;
			info->size = size-8;  //ieee_8022_8033_hdr'size
			break;
		case 0x86DD :  //IPv6
			printf("[IPv6]");
			info->data = data+8;
			info->type = IPV6;
			info->size = size-8;  //ieee_8022_8033_hdr'size
			break;
		case 0x8847 :  //MPLS Label
			printf("[MPLS]");
			info->data = data+8;
			info->type = MPLS;
			info->size = size-8;  //ieee_8022_8033_hdr'size
			break;
		case 0x8863 :  //PPPoe - Discovery
			printf("[PPPoe]");
			info->data = data+8;
			info->type = PPPOED;
			info->size = size-8;  //ieee_8022_8033_hdr'size
			break;
		case 0x8864 :  //PPPoe - Session
			printf("[PPPoe]");
			info->data = data+8;
			info->type = PPPOES;
			info->size = size-8;  //ieee_8022_8033_hdr'size
			break;
		case 0x8100 :  //802.1Q tag
			printf("[802.1Q]");
			info->data = data+8;
			info->type = p8021Q;
			info->size = size-8;  //ieee_8022_8033_hdr'size
			break;
		default :      //IEEE802.2/802.3 rfc1084
			printf("[Unknow]");
	}
	return ;
}

/**
 * @brief ���������ipv4����
 *
 * @param size �����ipv4���ݰ��Ĵ�С
 * @param data �����ipv4���ݰ��Ŀ�ʼλ��
 * @param info �������ipv4���ݰ��н������Ĵ������Ϣ
 */
void parse_ipv4(u_int16 size, u_char *data, tran_info *info/**<[out] ���������ĵĴ������Ϣ*/)
{
	assert(size != 0);
	assert(data != NULL);
	assert(info != NULL);

	ipv4_hdr *hdr=(ipv4_hdr *)data;
	display_ipv4_hdr(hdr);
	//TODO �������������tran_info
	
	return ;
}

/**
 * @brief ��ӡipv4��ͷ��Ϣ
 *
 * @param hdr ipv4_hdr*
 */
void display_ipv4_hdr(ipv4_hdr *hdr)
{
	u_int8 option_len = hdr->ihl - 5;
	u_int16 tmp = htons(hdr->frag_off);

	printf("ver %d ",hdr->ver);
	printf("hdl %d ",hdr->ihl);
	printf("tos 0x%x ",hdr->tos);
	printf("tot len %d ",htons(hdr->tot_len));
	printf("id %d ",htons(hdr->id));
	printf("flag 0x%x ",tmp>>13);
	printf("frag off %d*4 ",tmp&0x1fff);
	printf("ttl %d ",hdr->ttl);
	printf("procol 0x%x ",hdr->procol);
	printf("check 0x%x ",hdr->check);
	printf("saddr ");
	display_ipv4_addr(&(hdr->saddr));
	printf(" daddr ");
	display_ipv4_addr(&(hdr->daddr));
	
	printf(" option ");
	u_int16 i=0;
	for(i; i<option_len * 4; i++)
	{
		printf("%x",hdr->option[i]);
	}
	
	return ;
}

/**
 * @brief ��ӡ�����ʽ��ipv4��ַ
 *
 * @param addr ipv4��ַָ��
 */
void display_ipv4_addr(u_int32 *addr)
{
	assert(addr != NULL);
	u_int8 *p = (u_int8*)addr;

	u_int8 i = 0;

	for(i; i<3; i++)
	{
		printf("%d",p[i]);
		printf(":");
	}
	printf("%d",p[i]);
	
	return ;
}

/**
 * @brief ��������㱨��
 *
 * @param netinfo  ������������Ϣ
 * @param traninfo �����Ĵ������Ϣ
 */
void parse_net(net_info *netinfo/**<[in]����㱨����Ϣ*/, tran_info *traninfo/**<[out] �������Ĵ���㱨����Ϣ*/)
{
	assert(netinfo != NULL);
	assert(traninfo != NULL);

	traninfo->data = NULL;
	traninfo->type = TRAN_INVALID;
	traninfo->size = 0;

	switch (netinfo->type)
	{
		case NET_INVALID: //TODO
			break;
		case IPV4 :  //!IPV4    0x0800
			parse_ipv4(netinfo->size,netinfo->data,traninfo);
			break;
		case ARP  :  //!ARP     0x0806
			break;
		case RARP :  //!RARP    0x8035
			break;
		case IPV6 :  //!IPV6    0x86DD
			break;
		case MPLS :  //!MPLS    0x8847
			break;
		case PPPOED: //!PPPoE   0x8863 - Discovery
			break;
		case PPPOES: //!PPPoE   0x8864 - Session
			break;
		case p8021Q:  //!802.1Q  0x8100
			break;
		case NET_MAX: //TODO
			break;
		default :
			break;
	}
	return ;
}

