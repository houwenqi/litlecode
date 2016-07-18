#include <assert.h>
 #include<unistd.h>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>
#include <stdint.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdarg.h>
#include "Log.h"
#include "internal_protocol.h"
#include "ProxyProtocol.h"
#include "PackageHeader.h"
#include "MmByteBuffer.h"
#include "protocal_msg.pb.h"

using namespace std;
using namespace ptt;
using namespace mm;

struct Ping
{
	unsigned char  magic; // 心跳标志0x1C
	unsigned char  ver:4; // 版本1
	unsigned char  rep:4; // 0x0,表示不需要回复，0x1表示回传客户端外网ip：port（格式参考下面回传ip：port），其他值保留
	unsigned short reserb;  // 保留
	unsigned int  uid;        // 用户id
	unsigned int  exp;        // 有效时长，表示客户端在exp时间内会再次发送心跳
	unsigned int reee:24;
	unsigned int endtag:8;   /// 结束标志0x1C
};

struct Pong
{
	unsigned char magic;   // 心跳回传标志， 0x1F
  	unsigned char ver:4;     // 版本1
  	unsigned int sip;       // 服务器ip
  	unsigned short sport;  // 服务器端口
  	unsigned int cip;       // 客户端外网ip
  	unsigned short cport;  // 客户端外网端口
  	unsigned char endtag;  // 结束标志，0x1F
};


unsigned int uin =0;
int cmd = 0;
int rspcmd = 0; 
char buffer[8096] ={0};
string sid = "0";



int HandleGetFenceReq();
int HandleGetOutFenceMsgReq();
int HandleReport();
int HandleGot();
int HandleResp( int);

//
int main(int argc,char* argv[])
{
	for (int c; (c = getopt (argc, argv, "a:b:c:g:r:")) != -1;)
	{
		switch (c)
		{
		case 'g':
			uin =  atoi(optarg);
            printf ("called: %s\n", optarg);
			break;
        
		case 'c':
			cmd = atoi( optarg );
			printf ("cmd: %s\n", optarg);
			break;
		case 'r':
            rspcmd = atoi( optarg );
            printf ("cmd: %s\n", optarg);
            break;
		default:
			break;
		}
	}
	if(cmd ==0 )
		return -1;

	printf ("parse args \t\t\t[ok]\n");
	Log::instance()->init_log ("./log/", 5, "test", 1000000);

	if(cmd == 1)//handle get req
	{
		HandleGetFenceReq();
	}
	else if( cmd == 2 )
	{
		HandleGetOutFenceMsgReq();
	}
	else if(cmd == 3)//handle a report
	{
		HandleReport();
	}
	else if(cmd == 5)//handle got push warning 
	{
		HandleGot();
	}
	else if(cmd == 8)//b response
	{
		HandleResp(rspcmd);
	}
	delete Log::instance ();
	return 0;
}

int SendAndRecv(char *ip, unsigned short port, int length )
{
	static int sockfd = 0;
	struct sockaddr_in server_addr; 
	if ( ( sockfd = socket ( AF_INET , SOCK_DGRAM , 0) ) == - 1)
	{ 
         printf ( "socket error\n" ) ; 
         return -1; 
    } 
    
	memset ( & server_addr, 0, sizeof ( struct sockaddr ) ) ; 
    server_addr. sin_family = AF_INET ; 
    server_addr. sin_port = htons ( port ) ; 
    server_addr. sin_addr. s_addr = inet_addr( ip ) ; 
    

	if (sendto (sockfd, buffer, length, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) !=  length)
	{
		LOG (LOG_ERROR, "sendto error,%s",strerror(errno));
		return -2;
	}else{
		printf("Do  sendto is OK \n");
	}

	//接收返回值分析
	
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof addr;

	int numbytes=0;
	if ( ( numbytes = recvfrom ( sockfd, buffer, 8096, MSG_TRUNC, (struct sockaddr*)&addr, &addrlen) ) == - 1) 
	{ 
		printf ( "recv error\n" );
		return -3; 
	}

	printf("recvfrom is OK \n");
	ByteBuffer byteBuffer;
	byteBuffer.write(buffer, numbytes);
	
	PackageHeader header;
	if( !header.read(byteBuffer) )
	{
		LOG(LOG_ERROR, "header read error");
		return -1;
	}
	//assert( header.packageLength == numbytes );
	if( header.cmd == PTT_PKT_GET_USER_OUT_FENCE_MSG_REP)
	{
		printf("Parse GET_OUT_FENCE_MSG \n");
		GetUserOutFenceMsgRep resp;
		if( !resp.ParseFromString(string(buffer+header.headerLength,numbytes-header.headerLength)))
		{
			printf("parse GetUserOutFenceMsgRep sring error. Command is %d,bufferlen is %d",header.cmd,numbytes);
		}
		else
		{
			printf("now get cmdid and cmdbuffer\n");
			const CmdList &lst = resp.userlist();
			int sum = lst.count();
			printf("sum is %d\n",sum);
			for( int i=0;i<sum;i++)
			{
				const CmdItem &tem = lst.list(i);
				int ncmdid = tem.cmdid();
				const SKBuiltinBuffer_t &t = tem.cmdbuf();
				//解里层的包
				UserInfoRep rep;
				int buflen = t.ilen();
				if( !rep.ParseFromString(t.buffer()))
				{
					printf("Parse buf is failed\n");
				}
				else
				{
					unsigned int uin = rep.uin();
					unsigned int status = rep.status();

					const SKBuiltinString_t &name = rep.username();
					const SKBuiltinString_t &avatar = rep.useravatar();
					const SKBuiltinString_t &phone = rep.phone();
					
					printf("uin is %d, status is %d, name is %s, avatar is %s, phone is %s \n",
								uin,status,name.string().c_str(),avatar.string().c_str(),phone.string().c_str());
				}
				
			}
		}
	}
	else if( header.cmd == PTT_PKT_PING_RESP)
	{
		printf("Parse PING_RESP \n");
		PingResponse rsp;
		if(!rsp.ParseFromString( string(buffer + header.headerLength , numbytes - header.headerLength)))
		{
		  printf(" parse ping response failed\r\n");
		  return -6;
		}
		BaseResponse basersp = rsp.baseresponse();
		sid = basersp.errmsg().string();
		printf( "Get session key:%s\r\n" , sid.c_str());
	}
	
	close( sockfd); 
	return 0;
}

void MakeBase( unsigned int uin,  BaseRequest *base )
{
	//base =  r.baserequest();
	base->set_uin(uin);
	base->set_sessionkey(sid.c_str());
	base->set_deviceid(3);
	SKBuiltinString_t *t = base->mutable_clientversion();
	t->set_string("test");
	base->set_devicetype("5");
}

int Send_User_Msg( string output, int cmd, char *ip, unsigned short port )
{
	PackageHeader _header;
	ByteBuffer _data;
	_data.clear();
	_data.write( output.c_str(), output.size() );
	_header.cmd = cmd;
	_header.headerLength = sizeof(_header);
	_header.packageLength = _data.size() + _header.headerLength;
	_header.seq = 0;

	ByteBuffer buf;
	_header.write(buf);
	//LOG(LOG_DEBUG, "out put len:%d, data size:%d, buffer size:[%d]", output.size(), _data.size(), buf.size() );
	printf("output len is %d, data size is %d, buffer size is %d \n", output.size(), _data.size(), buf.size() );
	_data.insert(0, buf.begin(), buf.remaining());

	memcpy(buffer, _data.base() , _data.size() );
	
	SendAndRecv(ip, port, _data.size() );
	return 0;
}

int HandleGetFenceReq()
{
	GetUserFenceInfoReq r;
	MakeBase( uin, r.mutable_baserequest() ) ;	

	string output;
	r.SerializeToString( &output );

	return Send_User_Msg( output, PTT_PKT_GET_USER_FENCE_INFO_REQ , "121.41.33.130", 22346 );
}

int HandleLogin()
{
     printf("send HandleLogin\r\n");
     PingRequest req;
     MakeBase( uin , req.mutable_baserequest() );
     SKBuiltinBuffer_t* key = req.mutable_currentsynckey();
     key->set_buffer("666",3);
     key->set_ilen(3);
	 
     string output; 
     req.SerializeToString(&output);
     int iRet = Send_User_Msg(output, PTT_PKT_PING_REQ, "172.16.1.21", 22346);
     if( 0 != iRet )
     {
      printf( "HandleLogin Failed: iRet:%d" , iRet);
     }
     else    
     {
      printf( "Handlelogin Success\r\n");
     }
	 return 0;
}

int HandleGetOutFenceMsgReq()
{
	//构造发送ping请求
	HandleLogin();

	printf("NOW DO FENCEREQ \n");
	GetUserOutFenceMsgReq r;
	MakeBase(uin,r.mutable_baserequest());
	//printf("sesion key is %s\n",sid.c_str());
	r.set_uin(200025);
	r.set_rad(10);
	r.set_gid(3599);
	r.set_countryid(0);

	string output;
	r.SerializeToString(&output);

	return Send_User_Msg(output,PTT_PKT_GET_USER_OUT_FENCE_MSG_REQ,"172.16.1.21",22346);
}

int HandleReport()
{
	UserReportFenceInfoReq r;
	MakeBase( uin, r.mutable_baserequest() ) ;	
	r.set_fid(2);
	r.set_gps_lat(3);
	r.set_gps_lon(4);
	r.set_trigger_type(5);
	r.set_trigger_time(6);

	string output;
	r.SerializeToString( &output );

	return Send_User_Msg( output, PTT_PKT_USER_REPORT_FENCE_INFO_REQ, "121.41.33.130", 22346 );
}

int HandleGot()
{
	PushWarningInfoRep r;
	
    BaseResponse *baseresponse = r.mutable_baseresponse();
    baseresponse->set_ret(0);
        
    SKBuiltinString_t *t = baseresponse->mutable_errmsg();
    t->set_string("ok");

    r.set_uin(uin);
	r.set_seq(2000001);
	string output;
	r.SerializeToString( &output );

	return Send_User_Msg( output, PTT_PKT_PUSH_FENCE_WARNING_REP, "121.41.33.130", 22346 );
}

int HandleResp( int)
{
	PushUserFenceInfoRep r;
	
    BaseResponse *baseresponse = r.mutable_baseresponse();
    baseresponse->set_ret(0);
        
    SKBuiltinString_t *t = baseresponse->mutable_errmsg();
    t->set_string("ok");

    r.set_uin(uin);
	r.set_seq(1000001);
	string output;
	r.SerializeToString( &output );

	return Send_User_Msg( output, PTT_PKT_PUSH_USER_FENCE_INFO_REP , "121.41.33.130", 22346 );
}