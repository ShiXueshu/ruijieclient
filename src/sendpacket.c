/*******************************************************************************\
 * RuijieClient -- a CLI based Ruijie Client authentication modified from mystar *
 *                                                                               *
 * Copyright (C) Gong Han, Chen Tingjun  microcai                                *
 \*******************************************************************************/

/*
 * This program is modified from MyStar, the original author is netxray@byhh.
 *
 * Many thanks to netxray@byhh
 *
 * AUTHORS:
 *   Gong Han  <gong AT fedoraproject.org> from CSE@FJNU CN
 *   Chen Tingjun <chentingjun AT gmail.com> from POET@FJNU CN
 *   microcai <microcai AT sina.com > from ZSTU
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "sendpacket.h"
#include "blog.h"

// broadcast packet for finding server
static uint8_t broadPackage[0x3E8] =
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Destination MAC
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Source MAC
      0x88, 0x8E, // Ethertype = 0x888E (8021X)
      0x01, // Version = 1
      0x01, // Packet Type  0x00
      // 0x01,EAPOL-Start
      // 0x02 ;0x03 ;0x04
      0x00, 0x00, // Packet Body Length
    };

// echo packet incorporating user name and MD5 sum
static uint8_t ackPackage[0x3E8] =
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Destination MAC
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Source MAC
      0x88, 0x8E, // Ethertype = 0x888E (8021X)
      0x01, // Version = 1
      0x00, // Packet Type 0x00 ;0x01,EAPOL-Start ;0x02 ;0x03 ;0x04
      0x00, 0x00, // Packet Body Length
      0x02, // Response
      0x00, // id
      0x00, 0x00, // length
      0x00 // type  1 identify  4 MD5-Challenge
    };

// end certification packet
static uint8_t ExitPacket[0x3E8] =
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Destination MAC
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Source MAC
      0x88, 0x8E, // Ethertype = 0x888E (8021X)
      0x01, // Version = 1
      0x02, // Packet Type  0x00
      // 0x01,EAPOL-Start
      // 0x02 ;0x03 ;0x04
      0x00, 0x00, // Packet Body Length
    };

// keep-alive echo packet
static uint8_t echoPackage[] =
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x88, 0x8E, 0x01, 0xBF, 0x00, 0x1E, 0xFF, 0xFF, 0x37, 0x77, 0x7F, 0x9F,
      0xF7, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x37, 0x77, 0x7F, 0x9F, 0xF7, 0xFF,
      0x00, 0x00, 0xFF, 0xFF, 0x37, 0x77, 0x7F, 0x3F, 0xFF };

//Ruijie OEM Extra （V2.56）  by soar
static uint8_t RuijieExtra[144] =
  {
      // OEM Extra
      // 0 --> 22
      0xff,
      0xff,
      0x37,
      0x77, // Encode( 0x00,0x00,0x13,0x11 )
      // 求反并头尾颠倒.add by lsyer
      0xff, // Encode( 0x01/00  EnableDHCP flag )
      // 0xff:Static IP  0x3f:DHCP
      0x00,
      0x00,
      0x00,
      0x00, // Encode( IP )
      0x00,
      0x00,
      0x00,
      0x00, // Encode( SubNetMask )
      0x00,
      0x00,
      0x00,
      0x00, // Encode( NetGate )
      0x00,
      0x00,
      0x00,
      0x00, // Encode( DNS )
      0x00,
      0x00, // Checksum( )
      // 23 --> 58
      // ASCII 8021x.exe
      0x00, 0x00, 0x13, 0x11, 0x38, 0x30, 0x32, 0x31, 0x78, 0x2E, 0x65, 0x78,
      0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00,
      0x00,
      0x00,
      0x00,
      0x00,
      0x00,
      0x00,
      0x00,
      0x00,
      0x00,
      // 59 --> 77
      0x00,
      0x00,
      0x00,
      0x00, // 8021x.exe File Version (2.56.00)
      // base16 code.add by lsyer
      0x00, // unknow flag
      // Const strings
      0x00, 0x00, 0x13, 0x11, 0x00, 0x28, 0x1A, 0x28, 0x00, 0x00,
      0x13,
      0x11,
      0x17,
      0x22,
      // 78 --> 118
      // 32bits spc. Random strings
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
      0x0C, 0x0D, 0x0E, 0x0F,
      // 32bits spc. Random strings
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
      0x0C, 0x0D, 0x0E, 0x0F,
      // Const strings
      0x00, 0x00, 0x13, 0x11, 0x18, 0x06, 0x00, 0x00, 0x00,
      // 119
      0x00, // DHCP and first time flag
      // V2.56 (and upper?) added
      // 120 -->
      0x1A, 0x0E, 0x00, 0x00, 0x13, 0x11, 0x2D, 0x08, // Const strings
      // 128 --> 141
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // True NIC MAC
      0x1A, 0x08, 0x00, 0x00, 0x13, 0x11, 0x2F, 0x02 // Const strings
    };

int
FillVersion(char * m_fakeVersion)
{
  unsigned int c_ver1, c_ver2;

  if (m_fakeVersion && sscanf(m_fakeVersion, "%u.%u", &c_ver1, &c_ver2))
    {
#ifdef DEBUG
      printf("## c_ver1=%u ## c_ver2=%u\n", c_ver1, c_ver2);
#endif
      RuijieExtra[0x3B] = c_ver1;
      RuijieExtra[0x3C] = c_ver2;
      return 0;
    }
  else
    {
      return -1;
    }
}

/* comment out for further usage
 int
 FillFakeMAC(unsigned char * fMAC, char * m_fakeMAC)
 {

 #ifdef DEBUG
 int i;
 #endif

 if (m_fakeMAC && sscanf(m_fakeMAC, "%x:%x:%x:%x:%x:%x", &fMAC[0],
 &fMAC[1], &fMAC[2], &fMAC[3], &fMAC[4], &fMAC[5]))
 {
 #ifdef DEBUG
 printf("## MAC =");
 for (i = 0; i < 6; i++)
 printf(" %2X ", fMAC[i]);
 putchar('\n');
 #endif

 return 0;
 }
 else
 {
 return -1;
 }
 }
 */

unsigned char *
ComputeHash(unsigned char * src, int i)
{
  MD5_CTX context;
  static unsigned char digest[16];
  MD5_Init(&context);
  MD5_Update(&context, src, i);
  MD5_Final(digest, &context);
  return digest;
}

int
GetServerMsg(ruijie_packet*this, char*outbuf, size_t buflen)
{
  memset(outbuf, 0, buflen);
  size_t len = ntohs(*((u_int16_t*) (this->pkt_data + 0x10))) - 10;
  if (buflen < len)
    // space allocation of buffer exceeded
    return -1;
  if (len < 0)
    // did not retrieve any messages from sever.
    return 0;
  char *msgBuf = (typeof(msgBuf)) (this->pkt_data + 0x1c);

  //remove the leading "\r\n" which seems always exist!
#ifdef DEBUG
  puts("-- MSG INFO");
  printf("## msgBuf(GB) %s\n", msgBuf);
#endif
  if (len > 3 && (msgBuf[0] == 0xd) && (msgBuf[1] == 0xa))
    {
#ifdef DEBUG
      puts("@@ /r/n found");
      puts("-- END");
#endif
      msgBuf += 2;
    }
#ifdef DEBUG
  else
    {
      puts("@@ /r/n not found");
      puts("-- END");
    }
#endif
  code_convert(outbuf, buflen, msgBuf, strlen(msgBuf));
}

int
WaitPacket(ruijie_packet *this, int timeout)
{
  struct pollfd pfd;
  pfd.fd = this->m_pcap_no;
  pfd.events = POLLIN;

  return poll(&pfd, 1, timeout);
}

int
SendFindServerPacket(ruijie_packet *this)
{
  uint8_t StandardAddr[] =
    { 0x01, 0x80, 0xC2, 0x00, 0x00, 0x03 };
  uint8_t StarAddr[] =
    { 0x01, 0xD0, 0xF8, 0x00, 0x00, 0x03 };

  int i;

  if (this->m_authenticationMode)
    memcpy(broadPackage, StarAddr, 6);
  else
    memcpy(broadPackage, StandardAddr, 6);

  memcpy(broadPackage + ETH_ALEN, this->m_ETHHDR + ETH_ALEN, ETH_ALEN); // fill local MAC

  this->m_ruijieExtra = RuijieExtra;

  // check blog.c and bloc.h for details
  InitializeBlog(this);
  FillNetParamater(this);

  memcpy(broadPackage + 18, this->m_ruijieExtra, sizeof(RuijieExtra));

  fputs(">> Searching for server...\n", stdout);

  for (i = 0; i < 3; i++)
    {
      if (pcap_sendpacket(this->m_pcap, broadPackage, 0x3E8))
        return -1;
      switch (WaitPacket(this, 1500))
        {
      case 0: // timeout
        continue; // retry
      case -1:
        break;
      default: // it should be 1
        if (pcap_next_ex(this->m_pcap, &this->pkt_hdr, &this->pkt_data) <= 0)
          continue;
        if (this->pkt_data[0x12] == 0x01 && this->pkt_data[0x16] == 0x01)
          {
            // get server's MAC address.
            memcpy(this->m_ETHHDR, this->pkt_data + ETH_ALEN, ETH_ALEN);
            //gets last ID
            this->m_lastID = this->pkt_data[0x13];

            return 0;
          }
        continue;
        }
    }
  return -1;
}

int
SendNamePacket(ruijie_packet *this)
{
  //  extern char *m_name;
  int nameLen;

  nameLen = strlen(this->m_name);
  memcpy(ackPackage, this->m_ETHHDR, 12); // fill destined MAC and local MAC

  ackPackage[0x13] = this->m_lastID; //id, HERE as if it's alway 1 from ShiDa ??
  *(short *) (ackPackage + 0x10) = htons((short) (5 + nameLen));// length
  *(short *) (ackPackage + 0x14) = *(short *) (ackPackage + 0x10);// length
  ackPackage[0x16] = 0x01; //Type: Identify
  memcpy(ackPackage + 0x17, this->m_name, nameLen); // fill name

  memcpy(ackPackage + 0x17 + nameLen, this->m_ruijieExtra, sizeof(RuijieExtra));

  fputs(">> Sending user name...\n", stdout);
  int i;
  for (i = 0; i < 3; i++)
    {
      if (pcap_sendpacket(this->m_pcap, ackPackage, 0x3E8))
        return -1;
      switch (WaitPacket(this, 1500))
        {
      case 0: // timeout
        continue; // retry
      case -1:
        break;
      default: // it should be 1

        if (pcap_next_ex(this->m_pcap, &this->pkt_hdr, &this->pkt_data) <= 0)
          continue;
        if (this->pkt_data[0x12] == 0x01 && this->pkt_data[0x16] == 0x04)
          {
            //gets last ID
            this->m_lastID = this->pkt_data[0x13];
            this->m_MD5value_len = this->pkt_data[0x17];
            memcpy(this->m_MD5value, this->pkt_data + 0x18,
                this->m_MD5value_len);
            return 0;
          }
        continue;
        }
    }
  return -1;
}

int
SendPasswordPacket(ruijie_packet *this)
{

  // msg offset
  u_int16_t offset;
  ULONG_BYTEARRAY uTemp;

  unsigned char md5Data[256]; // password,md5 buffer
  unsigned char *md5Dig; // result of md5 sum
  int md5Len = 0;

  int nameLen, passwordLen;

  nameLen = strlen(this->m_name);
  passwordLen = strlen(this->m_password);

  memcpy(ackPackage, this->m_ETHHDR, 12);// fill destined MAC and local MAC

  ackPackage[0x13] = this->m_lastID; //id

  ackPackage[0x16] = 0x04; // Type: MD5-Challenge

  *(short *) (ackPackage + 0x10) = htons((short) (22 + nameLen)); // length
  *(short *) (ackPackage + 0x14) = *(short *) (ackPackage + 0x10);

  md5Data[md5Len++] = this->m_lastID;//ID
  memcpy(md5Data + md5Len, this->m_password, passwordLen);
  md5Len += passwordLen; // password

  memcpy(md5Data + md5Len, this->m_MD5value, this->m_MD5value_len);
  md5Len += this->m_MD5value_len; // private key

  md5Dig = (unsigned char *) ComputeHash(md5Data, md5Len);

  ackPackage[0x17] = 16; // length of md5 sum is always 16.
  memcpy(ackPackage + 0x18, md5Dig, 16);

  memcpy(ackPackage + 0x28, this->m_name, nameLen);

  memcpy(ackPackage + 0x28 + nameLen, this->m_ruijieExtra, sizeof(RuijieExtra));

  fputs(">> Sending password... \n", stdout);
  int i;
  for (i = 0; i < 3; i++)
    {
      if (pcap_sendpacket(this->m_pcap, ackPackage, 0x3E8))
        return -1;
      switch (WaitPacket(this, 1500))
        {
      case 0:
        // timeout
        continue; // retry
      case -1:
        break;
      default:
        // it should be 1
        if (pcap_next_ex(this->m_pcap, &this->pkt_hdr, &this->pkt_data) <= 0)
          continue;
        if (this->pkt_data[0x12] == 3)
          // if succeed in sending password
          {
            //gets last ID
            this->m_lastID = this->pkt_data[0x13];

            //get 心跳信息初始码
            //uTemp.ulValue = *(((u_long *)(pkt_data+0x9d)));
            offset = ntohs(*((u_int16_t*) (this->pkt_data + 0x10)));
            uTemp.ulValue = *((u_int32_t *) (this->pkt_data + (0x11 + offset)
                - 0x08));
            this->m_key.btValue[0] = Alog(uTemp.btValue[3]);
            this->m_key.btValue[1] = Alog(uTemp.btValue[2]);
            this->m_key.btValue[2] = Alog(uTemp.btValue[1]);
            this->m_key.btValue[3] = Alog(uTemp.btValue[0]);

            return 0;
          }
        else if (this->pkt_data[0x12] == 4)
          {
            return 1; // if failed.
          }
        continue;
        }
    }
  return -1;
}
// Here, we exert IMAP to detect the status of connection,
// namely, the fairly common command 'ping'
int
IfOnline(ruijie_packet *this)
{
  // TODO: put code here

  return 0;
}
int
SendEchoPacket(ruijie_packet *this)
{

  ULONG_BYTEARRAY uCrypt1, uCrypt2, uCrypt1_After, uCrypt2_After;

  this->m_serialNo.ulValue++;
  /* m_serialNo is initialized at the beginning of main() of ruijieclient.c, and
   * m_key is initialized in ruijieclient.c when the 1st Authentication-Success
   * packet is received.
   * */

  uCrypt1.ulValue = this->m_key.ulValue + this->m_serialNo.ulValue;
  uCrypt2.ulValue = this->m_serialNo.ulValue;

  memcpy(echoPackage, this->m_ETHHDR, 12);

  uCrypt1_After.ulValue = htonl(uCrypt1.ulValue);
  uCrypt2_After.ulValue = htonl(uCrypt2.ulValue);

  echoPackage[0x18] = Alog(uCrypt1_After.btValue[0]);
  echoPackage[0x19] = Alog(uCrypt1_After.btValue[1]);
  echoPackage[0x1a] = Alog(uCrypt1_After.btValue[2]);
  echoPackage[0x1b] = Alog(uCrypt1_After.btValue[3]);
  echoPackage[0x22] = Alog(uCrypt2_After.btValue[0]);
  echoPackage[0x23] = Alog(uCrypt2_After.btValue[1]);
  echoPackage[0x24] = Alog(uCrypt2_After.btValue[2]);
  echoPackage[0x25] = Alog(uCrypt2_After.btValue[3]);

  if (WaitPacket(this, 1000) != 0)
    {
      // watch out! Here is the crucial part of determining interruption
      // of network connection
      return -1; // if the connection has been interrupted.
    }
  return pcap_sendpacket(this->m_pcap, echoPackage, 0x2d);
}

int
SendEndCertPacket(ruijie_packet *l)
{
  memcpy(ExitPacket, l->m_ETHHDR, 12);// fill destined MAC and local MAC

  memcpy(ExitPacket + 18, l->m_ruijieExtra, sizeof(RuijieExtra));

  fputs(">> Logouting... \n", stdout);
  return (pcap_sendpacket(l->m_pcap, ExitPacket, 0x80) == 0x80) ? 0 : -1;
}