///恒生ESB消息相关常量定义
/**
 *源程序名称:tag_def.h
 *软件著作权:恒生电子股份有限公司
 *系统名称:恒生企业服务总线ESB
 *模块名称:ESB Message
 *功能说明:定义ESB消息域相关常量
 *         ESB消息域以tag=value的形式表示，每个域之间以\0分隔;
 *         每个tag均为一个数字值，在本文件中定义;
 *         消息域按其取值的数据类型分为：
 *           (1)整数域，即Int域，数字字符串；
 *           (2)字符串域，即String域，任意可见字符串；
 *           (3)二进制域，即RawData域，可以为任意字符组成；该域前，必须有一个整数型的 TAG_RAWDATALEN 域来表示该二进制域长度；
 *           (4)数组域，即Array域，可以为任意字符组成；该域前，必须有一个整数型的 TAG_ARRAYLEN 域来表示该数组大小。
 *         消息域按其使用情况，有二种比较特别的域：
 *           (1)系统域，有二个：TAG_RAWDATALEN 二进制长度域，TAG_ARRAYLEN 数据大小域 ；
 *           (2)内部域，消息中的这种域只在ESB节点的多个本地子进程之间传递，不在网络上传递；
 *作    者:  maoyinjie
 *开发日期:2018/1/29 9:38:58
 *备    注: 根据文档《ESBMessage数据词典.xls》生成
 */

#ifndef _TAG_DEF_H
#define _TAG_DEF_H

const int INTERNAL_TAG_BEGIN = 100; //内部域tag起始值(含) 
const int INTERNAL_TAG_END   = 199; //内部域tag结束值(含) 

///域名：RawData域长度  数据类型：整数
const int TAG_RAWDATALEN = 1 ; //系统TAG,表明报文中下一个域为RawData型,域值即为下个RawData域的数据长度

///域名：数组域大小  数据类型：整数
const int TAG_ARRAYLEN = 2 ; //系统TAG,表明报文中下一个域为数组型,域值即为下个数组域的成员个数

///域名：包类型  数据类型：整数
const int TAG_PACKET_TYPE = 3 ; //描述包体业务数据类型
    const int REQUEST_PACKET = 0 ; //请求
    const int ANSWER_PACKET = 1 ; //应答
    const int INTERNAL_REQUEST_PACKET = 2 ; //内部请求
    const int INTERNAL_ANSWER_PACKET = 3 ; //内部应答

///域名：处理子系统号  数据类型：整数
const int TAG_SUB_SYSTEM_NO = 4 ; //子系统ID（相当于一批功能）

///域名：功能号  数据类型：整数
const int TAG_FUNCTION_ID = 5 ; //各业务系统产品自行分配,以下为ESB内部请求
    const int FUNCTION_PLUGIN_REGISTER = 2 ; //插件注册
    const int FUNCTION_NBR_CHANGED = 3 ; //邻居变化
    const int FUNCTION_OSPF = 4 ; //OSPF报文
    const int FUNCTION_NBR_PAUSE = 5 ; ////20091029 xuxp 增加 用来通知邻居暂停用的功能号
    const int FUNCTION_PUBLISH = 6 ; //发布
    const int FUNCTION_SUBSCIBE = 7 ; //订阅
    const int FUNCTION_PLUGIN_MANAGE = 8 ; //插件管理报文
    const int FUNCTION_SVR_STATUS = 9 ; //进程状态检测
    const int FUNCTION_SVR_RESET = 10 ; //进程复位
    const int FUNCTION_MEMBER_ACTIVE = 11 ; //f1协议下组内成员信息变化通知的功能号
    const int FUNCTION_F2_REG = 20 ; //F2邻居注册
    const int FUNCTION_F2_EXCHPWD = 21 ; //F2交换密钥
    const int FUNCTION_F2_VALIDATE_CERT = 22 ; //F2身份认证
    const int FUNCTION_T2_DATAPWD = 30 ; //T2交换数据密码
    const int FUNCTION_T2_CHG_COMMPWD = 31 ; //T2修改通信密码
    const int FUNCTION_T2_REG = 32 ; //T2邻居注册
    const int FUNCTION_T2_HEARTBEAT = 33 ; //T2心跳
    const int FUNCTION_T2_SECURITY_INFO = 34 ; //T2安全接入信息
    const int FUNCTION_BULK_BEGIN = 40 ; //大块数据传送协商
    const int FUNCTION_BULK_DATA = 41 ; //大块数据分片报文
    const int FUNCTION_BULK_END = 42 ; //大块数据传送结束
    const int FUNCTION_BULK_CANCEL = 43 ; //大块数据传送取消
    const int FUNCTION_LINK_STATUS = 44 ; //链路状态消息
    const int FUNCTION_RELIABLE_TRANS = 59 ; //可靠传输标签
    const int FUNCTION_UFX_NOTIFY = 60 ; //UFX通知功能
    const int FUNCTION_PROC_LOAD = 61 ; //汇报PROC负载的功能号
    const int FUNCTION_ROUTER_TEST = 62 ; //返回路由测试功能号

///域名：分支号  数据类型：整数
const int TAG_BRANCH_NO = 6 ; //各业务系统产品自行设定

///域名：返回码  数据类型：整数
const int TAG_RETURN_CODE = 7 ; //各业务系统产品自行设定,以下三个返回码被保留
    const int RT_ERR_NONE = 0 ; //正常处理
    const int RT_ERR_PROCESS = -1 ; //消息处理时发生系统错误
    const int RT_ERR_TRANSFER = 1 ; //消息转发时生成系统错误(不能转发)

///域名：消息体  数据类型：RawData
const int TAG_MESSAGE_BODY = 8 ; //一般用于存放业务报文

///域名：目标路由信息  数据类型：数组
const int TAG_ROUTE_INFO = 9 ; //目标的详细定位信息
    const int IDX_OSPF_NAME = 0 ; //数组下标,可OSPF路由的应用实例名,如果没有该字段,则标明是本应用实例名：应用组名.编号
    const int IDX_NBR_NAME = 1 ; //数组下标,邻居名,如果没有该字段,标明是本应用实例
    const int IDX_SVR_NAME = 2 ; //数组下标,合适服务子进程名:服务进程名.编号
    const int IDX_PLUGIN_ID = 3 ; //数组下标,合适的服务插件ID：插件ID：编号
    const int IDX_CONNECT_ID = 4 ; //数组下标,对于通信通道插件,用于标识本通道的某个通信连接。对于F1插件通道用于标识某个Session.
    const int IDX_MEMBER_NO = 5 ; //数组下标,对于F1通道插件,用于标明某个Session内的成员ID（组内编号）

///域名：发送者信息  数据类型：数组
const int TAG_SENDER_PATH = 10 ; //请求者的信息,可以根据该信息对请求者发送反馈信息,等同于TAG_ROUTE_INFO

///域名：包流水号  数据类型：整数
const int TAG_PACKET_ID = 11 ; //消息发起方自行累计,用于请求应答包的匹配,应答包原样返回

///域名：发送者编号  数据类型：整数
const int TAG_SENDERID = 12 ; //消息发起方自行设置,用于在一个进程里识别多种应用,应答包原样返回

///域名：优先级  数据类型：整数
const int TAG_PRIORITY = 13 ; //数值越大优先级越高,高优先级者会被优先传送,优先处理,业务消息严禁使用>13的优先级.>13的高优先级只能由ESB系统内部使用,确保内部的可靠.业务消息的合法取值为4--13.包括4和13.如果消息中不出现本字段,则按照优先级8处理

///域名：允许经过的跳数  数据类型：整数
const int TAG_TTL = 14 ; //报文每经过一次转发,此域会被减1,到该值被减到0,则消息会被丢弃,如果消息中不出现本字段,则对该消息不作TTL控制

///域名：允许存在的时间  数据类型：整数
const int TAG_LIVE_TIME = 15 ; //单位毫秒,一般用于在队列中等待处理时间,如果设置为0或者不出现本字段,则表明无限生存期

///域名：负载因子  数据类型：整数
const int TAG_LOAD = 16 ; //用于向邻居节点汇报本节点的负载情况,以便相邻节点在转发报文时进行负载均衡

///域名：F1地址层信息  数据类型：RawData
const int TAG_F1_SENDER = 17 ; //F1的附加数据字段APPEND_ADDRESS信息保存

///域名：通道安全类型  数据类型：整数
const int TAG_SAFE_TYPE = 18 ; //通信双方协商的安全级别
    const int ENM_ST_NONE = 0 ; //明文
    const int ENM_ST_PWD = 1 ; //通信密码
    const int ENM_ST_SSL = 2 ; //数字证书

///域名：错误号  数据类型：整数
const int TAG_ERROR_NO = 19 ; //

///域名：错误信息  数据类型：字符串
const int TAG_ERROR_INFO = 20 ; //

///域名：密码  数据类型：RawData
const int TAG_PASSWORD = 21 ; //

///域名：证书  数据类型：RawData
const int TAG_CERT = 22 ; //

///域名：子服务进程状态  数据类型：整数
const int TAG_SERVICE_STATUS = 23 ; //多进程模式的节点,各子进程间表明活动状态
    const int ENM_CS_NONE = 0 ; //不正常
    const int ENM_CS_OK = 1 ; //正常

///域名：客户ID  数据类型：字符串
const int TAG_CLIENT_ID = 24 ; //客户ID,T2协议

///域名：加密的客户ID  数据类型：RawData
const int TAG_EN_CLIENT_ID = 25 ; //用通信密码加密后的客户ID,T2协议

///域名：数据密码  数据类型：RawData
const int TAG_DATA_PWD = 26 ; //数据密码,T2协议

///域名：是否需要改密  数据类型：整数
const int TAG_CHANGE_PWD = 27 ; //是否需要修改通信密码,T2协议
    const int ENM_CP_YES = 0 ; //不需要
    const int ENM_CP_NO = 1 ; //需要

///域名：原通信密码  数据类型：字符串
const int TAG_OLD_COMM_PWD = 28 ; //原通信密码,T2协议

///域名：通信密码  数据类型：字符串
const int TAG_COMM_PWD = 29 ; //通信密码,T2协议

///域名：身份标识  数据类型：字符串
const int TAG_IDENTITY_NAME = 30 ; //

///域名：许可证编号  数据类型：字符串
const int TAG_LICENSE_NO = 31 ; //许可证编号,T2协议

///域名：许可证内容  数据类型：数组
const int TAG_LICENSE_DATA = 32 ; //许可证内容,T2协议
    const int IDX_MAX_SEND_PACKETS = 0 ; //数组下标,每连接最多每分钟请求数
    const int IDX_MAX_SEND_BYTES = 1 ; //数组下标,每连接最多每分钟发送字节数
    const int IDX_ROUTE_BY_NAME = 2 ; //数组下标,是否允许有名注册，使可根据名字路由
    const int IDX_BULK_SEND = 3 ; //数组下标,是否允许大块数据传输

///域名：大块数据说明  数据类型：字符串
const int TAG_BULK_COMMENT = 33 ; //大块数据有关说明信息,大块传送协议

///域名：bypass时间戳  数据类型：字符串
const int TAG_BYPASS_TIMESTAMP = 34 ; //bypass时间戳

///域名：全链路分析  数据类型：数组
const int TAG_MSGTRACE = 35 ; //消息跟踪信息
    const int IDX_TRACE_ID = 0 ; //数组下标,消息的唯一标识
    const int IDX_DEPOT_ID = 1 ; //数组下标,消息跨度里的层级
    const int IDX_SPAN_ID = 2 ; //数组下标,消息跨度
    const int IDX_ANNOTATION = 3 ; //数组下标,应用自定义数据
    const int IDX_TRACE_FLAG = 4 ; //数组下标,是否需要全链路跟踪

///域名：接受方会话ID  数据类型：整数
const int TAG_RECE_SESSION_ID = 36 ; //接受方会话ID,大块传送协议

///域名：分片编号  数据类型：整数
const int TAG_PIECES_NUMBER = 37 ; //分片编号，从0开始,大块传送协议

///域名：分片数据  数据类型：RawData
const int TAG_PIECES_DATA = 38 ; //分片数据,大块传送协议

///域名：丢失的分片号  数据类型：字符串
const int TAG_MISS_PIECES = 39 ; //丢失的分片号,大块传送协议

///域名：路由方式  数据类型：整数
const int TAG_ROUTE_TYPE = 40 ; //1.0的路由方式

///域名：路由方式值  数据类型：字符串
const int TAG_ROUTE_VALUE = 41 ; //根据TAG_ROUTE_TYPE值，使用TAG_ROUTE_VALUE或者TAG_INTERNAL_ROUTE路由

///域名：F1的原始路由信息  数据类型：RawData
const int TAG_F1_ROUTE = 42 ; //取自F1的APPEND_ROUTE_INFO，存储用

///域名：压缩算法编号  数据类型：数组
const int TAG_COMPRESS_ID = 43 ; //msgbody数据压缩信息
    const int IDX_COMPRESS_ID = 0 ; //数组下标,压缩算法 0：不压缩
    const int IDX_ORG_PACK_LEN = 1 ; //数组下标,msgbody数据原始大小

///域名：内部许可证编号  数据类型：整数
const int TAG_INTERNAL_LICENSE_NO = 44 ; //内部许可证编号

///域名：T1头部数据  数据类型：RawData
const int TAG_T1_HEAD = 45 ; //T1头部数据,包括地址层数据

///域名：处理系统号  数据类型：整数
const int TAG_SYSTEM_NO = 46 ; //系统ID（相当于一批营业部）

///域名：附加数据标识  数据类型：整数
const int TAG_F1_APPEND_FLAG = 47 ; //

///域名：  数据类型：整数
const int TAG_F1_RESERVED = 48 ; //

///域名：F1的附加数据长度  数据类型：整数
const int TAG_F1_APPEND_LEN = 49 ; //用于保存所有F1收到的附加数据的长度

///域名：报序号  数据类型：整数
const int TAG_SEQUECE_NO = 50 ; //消息发布时的报序号,用以检测报是否连续

///域名：消息中心保留  数据类型：RawData
const int TAG_MSG_CENTER_RESERVED = 51 ; //消息发布时的报序号,用以检测报是否连续

///域名：时间戳,各个处理模块可以自行加入时间戳  数据类型：数组
const int TAG_TIMESTAMP = 52 ; //每个处理模块加入自己的时间戳,可以获得每个模块的处理时间

///域名：消息类型  数据类型：整数
const int TAG_ISSUE_TYPE = 53 ; //

///域名：主键  数据类型：RawData
const int TAG_KEY_INFO = 54 ; //

///域名：文件头部  数据类型：RawData
const int TAG_FILE_HEAD = 55 ; //

///域名：F1的附加数据  数据类型：RawData
const int TAG_F1_APPEND_DATA = 56 ; //用于保存所有F1收到的附加数据

///域名：有无压缩功能标志  数据类型：整数
const int TAG_UNCOMPRESS_MSGBODY = 57 ; //T2_SDK向T2_Channel注册时，表明自己是否有压缩功能
    const int ENM_COMPRESS = 0 ; //有压缩功能
    const int ENM_UNCOMPRESS = 1 ; //无压缩功能

///域名：可靠传输头部  数据类型：RawData
const int TAG_RELIABLE_BODY = 58 ; //可靠传输头部及附带数据体

///域名：业务保留域  数据类型：业务自行定义
const int TAG_APP_RESERVED = 59 ; //目前基财可能会定义成会话号

///域名：多进程内存数据库请求  数据类型：数组
const int TAG_MDB_RPC = 60 ; //mdb远程调用
    const int IDX_MDB_RPC_REQUESTID = 0 ; //数组下标,请求ID
    const int IDX_MDB_RPC_PROCEDURE = 1 ; //数组下标,函数
    const int IDX_MDB_RPC_RETCODE = 2 ; //数组下标,调用返回
    const int IDX_MDB_RPC_ARG1 = 3 ; //数组下标,调用参数1
    const int IDX_MDB_RPC_ARG2 = 4 ; //数组下标,调用参数2
    const int IDX_MDB_RPC_ARG3 = 5 ; //数组下标,调用参数3
    const int IDX_MDB_RPC_ARG4 = 6 ; //数组下标,调用参数3

///域名：多进程处理信息  数据类型：数组
const int TAG_MULT_PROC = 61 ; //proc_s与proc_c本地通信
    const int IDX_DEST_SUBPROC_ID = 0 ; //数组下标,子服务调用目标子进程号
    const int IDX_PROC_PACKET_ID = 1 ; //数组下标,主进程与子进程包匹配符
    const int IDX_SOUR_SUBPROC_ID = 2 ; //数组下标,源子进程号
    const int IDX_SEND_MODE = 3 ; //数组下标,子进程发出去的包的模式（同步、异步），基金使用
    const int IDX_DEST_SERVER_ID = 4 ; //数组下标,目标服务的组内编号
    const int IDX_PUSH_QUEUE_TIME = 5 ; //数组下标,消息入队列时间

///域名：身份类型  数据类型：整数
const int TAG_IDENTITY_TYPE = 62 ; //身份类型:  0:AR, 1:BAR, 2:LS, 3:AS

///域名：  数据类型：整数
const int TAG_ASYN_REGISTER_ID = 63 ; //通用通道插件的异步发送注册ID,由通用通道生成,其他插件在异步发送时指明

///域名：  数据类型：整数
const int TAG_JAVA_SENDID = 64 ; //Java的t2通道sdk专用的，使用者不可以添加，用于请求应答连接的匹配,应答包原样返回

///域名：fix插件信息  数据类型：数组
const int TAG_FIX_INFO = 65 ; //
    const int IDX_FIX_SENDID = 0 ; //数组下标,发送者编号
    const int IDX_FIX_DEST_NAME = 1 ; //数组下标,目标fix连接名
    const int IDX_FIX_MODE = 2 ; //数组下标,同步0，异步1
    const int IDX_FIX_ARG2 = 3 ; //数组下标,参数2

///域名：bypass信息  数据类型：数组
const int TAG_BYPASS_ORGREQ = 66 ; //保存sendpath

///域名：原始发送者信息  数据类型：数组
const int TAG_BRIDGE_ORGREQ = 67 ; //用来保存原始发送者信息，是数组和TAG_SENDER_PATH一样

///域名：桥插件信息  数据类型：数组
const int TAG_BRIDGE_SEND = 68 ; //用来保存桥插件的发送者信息，是数组和TAG_SENDER_PATH一样

///域名：保留给手机平台用  数据类型：RawData
const int TAG_MOBILE_RESERVED = 69 ; //

///域名：手机平台用  数据类型：RawData
const int TAG_MOBILE_NV = 70 ; //

///域名：机构编号,目前没有用到  数据类型：整数
const int TAG_COMP_ID = 71 ; //

///域名：多进程会话  数据类型：数组
const int TAG_MULTI_CONVERSATION = 72 ; //多进程的会话,模拟texudo会话
    const int IDX_CONVER_FLAG = 0 ; //数组下标,当前会话标志，比如syn，ack，fin，身份  
    const int IDX_CONVER_NO = 1 ; //数组下标,当前会话号                             
    const int IDX_CONVER_APP_FLAG = 2 ; //数组下标,应用层标志，比如tuxedo的标志           
    const int IDX_CONVER_SEQ_NO = 3 ; //数组下标,发送序号                               
    const int IDX_CONVER_ACK_NO = 4 ; //数组下标,确认序号                               

///域名：业务类别号,统一接入用到  数据类型：整数
const int TAG_BIZTYPE_ID = 45 ; //这里,是故意将该TAG定义成和TAG_T1_HEAD一致,以使得在06版也可以使用

///域名：以下5字段为JRES保留,JRES发送者  数据类型：数组
const int TAG_JRES_SENDER_INFO = 73 ; //
    const int IDX_JRES_NODE_NAME = 0 ; //数组下标,JRES节点名
    const int IDX_JRES_NODE_NO = 1 ; //数组下标,节点编号
    const int IDX_JRES_CONNECT_ID = 2 ; //数组下标,对于T2通道插件，表示连接号
    const int IDX_JRES_PLUGIN_NAME = 3 ; //数组下标,JRES插件名
    const int IDX_JRES_MEMBER_NO = 4 ; //数组下标,对T2通道插件，表示通信连接的组内编号，暂时无用

///域名：JRES路由目的地  数据类型：数组
const int TAG_JRES_ROUTE_INFO = 74 ; //Jres指定路由信息

///域名：JRES服务名  数据类型：字符串
const int TAG_JRES_SERVICE_ID = 75 ; //JRES服务名

///域名：JRES路经节点名  数据类型：字符串
const int TAG_JRES_PASSED_NODES = 76 ; //请求事件经历过节点列表，每个节点用分号隔开

///域名：JRES邻居类型  数据类型：整数
const int TAG_NODE_TYPE = 77 ; //
    const int NODE_TYPE_JRES = 1 ; //
    const int NODE_TYPE_CRES = 2 ; //

///域名：SEE产品保留  数据类型：array
const int TAG_SEE_KEY = 78 ; //具体数组定义的意义由SEE产品组定义

///域名：路由测试  数据类型：整数
const int TAG_ROUTE_TEST = 79 ; //类似tracert的路由测试

///域名：统一接入保留  数据类型：array
const int TAG_SYSTEM_INFO = 80 ; //
    const int IDX_SDK_VER = 0 ; //客户端SDK的版本
    const int IDX_SDK_CHECKSUM = 1 ; //客户端SDK的校验和
    const int IDX_SDK_IP_MAC = 2 ; //客户端机器的IP地址，MAC地址
    const int IDX_SDK_HEARTBEAT = 3 ; //服务端要求客户端的心跳时间
    const int SDK_DEADLINE = 4 ; //许可证有效期截止日期
    const int IDX_UFX_TIME = 5 ; //ufx发送数据的时间
    const int IDX_COMM_INDEX = 6 ; //适配器编号
    const int IDX_COMM_CHANNELID = 7 ; //通道号
    const int IDX_COMM_CONNECTID = 8 ; //连接号
    const int IDX_HD_SERIALNO = 9 ; //硬盘序列号
    const int IDX_CPU_SERIALNO = 10 ; //CPU序列号
    const int IDX_MACHINE_CODE = 11 ; //机器特征码
    const int IDX_WIDE_IP_MAC = 13 ; //外网IP    
    const int IDX_USER_CODE = 14 ; //UserCode  
    const int IDX_SESSION_NO = 15 ; //SessionNo 
    const int IDX_USER_PARAM1 = 16 ; //UserParam1
    const int IDX_USER_PARAM2 = 17 ; //UserParam2
    const int IDX_USER_PARAM3 = 18 ; //UserParam3
    const int IDX_AUTH_PRODUCT_TYPE = 19 ; //
    const int IDX_AUTH_KEY = 20 ; //
    const int IDX_ENCRYPT_KEY = 21 ; //动态密钥

///域名：统一接入保留  数据类型：RawData
const int TAG_CLIENTFIX_PWD = 81 ; //固定密钥加密后的身份标识

///域名：统一接入保留  数据类型：RawData
const int TAG_CLIENTDYN_PWD = 82 ; //加密后的身份标识

///域名：统一接入保留  数据类型：RawData
const int TAG_COMMPRI_PWD = 83 ; //固定密钥加密后的数据密钥

///域名：统一接入保留  数据类型：RawData
const int TAG_COMMPUB_PWD = 84 ; //加密后的数据密钥

///域名：同步发送报文标志  数据类型：整数
const int TAG_SYN_FLAG = 85 ; //如果不出现,或者0,表明是异步发送的,否则是同步发送

///域名：在线压测使用  数据类型：数组
const int TAG_ONLINE_TEST_INFO = 86 ; //在线压测信息
    const int IDX_ONLINE_FLAG = 0 ; //数组下标,标识该消息是压测消息

///域名：主题编号  数据类型：整数
const int TAG_MC_TOPICNO = 87 ; //主题编号

///域名：主题名字  数据类型：字符串
const int TAG_MC_TOPICNAME = 88 ; //主题名字，父子主题之间用.分割

///域名：会话连接的状态  数据类型：整数
const int TAG_MCSESSION_CONNECT = 89 ; //会话建立连接的状态，1表示SYN报文，2表示SYN|ACK报文，3表示SYN|ACK的ACK报文

///域名：会话的通信信息  数据类型：数组
const int TAG_MCSESSION_INFO = 90 ; //会话层信息，用于维护会话可靠传输
    const int IDX_SELF_CONNECTNO = 0 ; //数组下标,当前连接在自己的序号
    const int IDX_OPPOSITE_CONNECTNO = 1 ; //数组下标,当前连接在对方的序号
    const int IDX_SEND_SEQUENO = 2 ; //数组下标,当前的发送序号
    const int IDX_NEXTRECV_SEQUENO = 3 ; //数组下标,下一个期望接收的序号
    const int IDX_SUBCRIBE_INDEX = 4 ; //数组下标,订阅项标识，推送消息时带回
    const int IDX_HEART_TIME = 5 ; //数组下标,心跳的时间间隔

///域名：订阅的参数  数据类型：数组
const int TAG_MCSUBCRIBE_INFO = 91 ; //订阅所有参数信息，具体打包结构看后面的说明(数组)
    const int IDX_MCSUBCRIBE_INDEX = 0 ; //数组下标,订阅标识，订阅的时候带上
    const int IDX_SENDINTERVAL = 1 ; //数组下标,频度
    const int IDX_RETURNFILEDS = 2 ; //数组下标,返回字段,以'\2'区隔
    const int IDX_REPLACE = 3 ; //数组下标,是否覆盖
    const int IDX_REBULID = 4 ; //数组下标,是否补缺
    const int IDX_BORN_SUBSCRIBE = 5 ; //数组下标,是否是先天订阅
    const int IDX_RECOVER_SUBSCRIBE = 6 ; //数组下标,是否是重连后的订阅
    const int IDX_UPDATE_MSGNO = 7 ; //数组下标,数组下标,是否更新消息序号
    const int IDX_BATCHSUB_CHECKALL = 8 ; //数组下标,数组下标，是否要一次性业务校验

///域名：附加数据  数据类型：二进制数据
const int TAG_MCADDITION_DATA = 92 ; //订阅的附加数据，推送的时候原样返回

///域名：订阅的通知状态  数据类型：整数
const int TAG_MCNOTIFY_STATUS = 93 ; //订阅成功的状态通知，是进入补全状态还是主推状态

///域名：过滤信息  数据类型：数组
const int TAG_MCFILTER_INFO = 94 ; //每个主题有固定的6个过滤条件，自定义的过滤条件从下标6开始，一个字段名，一个字段值，字段值要求不可以有\0 \1 \2三个字符
    const int IDX_FILTERINDEX_1 = 0 ; //数组下标,已定义的过滤条件1的值
    const int IDX_FILTERINDEX_2 = 1 ; //数组下标,已定义的过滤条件2的值
    const int IDX_FILTERINDEX_3 = 2 ; //数组下标,已定义的过滤条件3的值
    const int IDX_FILTERINDEX_4 = 3 ; //数组下标,已定义的过滤条件4的值
    const int IDX_FILTERINDEX_5 = 4 ; //数组下标,已定义的过滤条件5的值
    const int IDX_FILTERINDEX_6 = 5 ; //数组下标,已定义的过滤条件6的值

///域名：业务层名字  数据类型：字符串
const int TAG_MCAPPLICATION_NAME = 95 ; //消息中心的上层应用名字，尽量唯一

///域名：消息编号  数据类型：整数
const int TAG_MCMSG_ID = 96 ; //发布端发送的消息编号，一般是连续递增

///域名：令牌标识  数据类型：整数
const int TAG_MCTOKEN_ID = 97 ; //业务层校验返回的检验码

///域名：消息中心1.0发布兼容标志  数据类型：数组
const int TAG_MCCOMPATIBLE_FLAG = 98 ; //兼容消息中心1.0的发布标志，标志着发布消息是放在key_info还是message_body

///域名：重置序号标志  数据类型：整数
const int TAG_MCRESET_MSGNO = 99 ; //重置消息序号的标志，用于在发不断发布的第一个报文都需要带上这个标志，1表示重置序号

///域名：同步之间使用的内部域  数据类型：数组
const int TAG_MCMSGCENTER_INFO = 110 ; //主要用于组内同步，标识不同的功能号
    const int IDX_MSGCENTER_FUNCID = 0 ; //数组下标,功能号
    const int IDX_MSGCENTER_PACKTYPE = 1 ; //数组下标,包类型
    const int IDX_MSGCENTER_PACKID = 2 ; //数组下标,包序号
    const int IDX_MSGCENTER_SENDERPATH = 3 ; //数组下标,用来组内同步消息的来源的中间件名字
    const int IDX_MSGCENTER_LOGINCNT = 4 ; //数组下标,登陆个数
    const int IDX_MSGCENTER_SENDERID = 5 ; //数组下标,组播发送者编号
    const int IDX_MSGCENTER_RECVERID = 6 ; //数组下标,组播接收者编号
    const int IDX_MSGCENTER_GROUPNO = 7 ; //数组下标,成员编号

///域名：持久化需要保存的信息  数据类型：数组
const int TAG_MCPERSIS_INFO = 111 ; //持久化需要增加的内部字段序号
    const int IDX_PERSIS_INTERNELNO = 0 ; //数组下标,消息内部序号
    const int IDX_PERSIS_MSGLIFE = 1 ; //数组下标,消息有效期
    const int IDX_MSG_TIME = 2 ; //数组下标,消息创建的绝对时间
    const int IDX_PUBLISHER_NO = 3 ; //数组下标,发布者内部编号
    const int IDX_MSG_OPPOTIME = 4 ; //数组下标,消息已经存活的时间，用于开机补缺

///域名：下级消息中心的标志位  数据类型：整数
const int TAG_MC_CHILDFLAG = 112 ; //下级消息中心的标志位，用来通知发布模块针对这种报文特殊处理，不用回应答，不用校验，不用向客户端补缺，0表示上级，1表示下级

///域名：内部路由信息  数据类型：数组
const int TAG_INTERNAL_ROUTE = 100 ; //内部TAG
    const int IDX_IR_IPC_CHANNEL = 0 ; //数组下标,内部通道的连接编号（应用以多进程模式实现时，通过内部通道以点对多型式互连）
    const int IDX_IR_SVR_NAME = 1 ; //数组下标,服务子进程名:配置的服务进程名.编号
    const int IDX_IR_PLUGIN_ID = 2 ; //数组下标,服务插件ID：插件ID：编号
    const int IDX_IR_CONNECT_ID = 3 ; //数组下标,对于通信通道插件,用于标识本通道的某个通信连接。对于F1插件通道用于标识某个Session.
    const int IDX_IR_MEMBER_NO = 4 ; //数组下标,对于F1通道插件,用于标明某个Session内的成员ID（组内编号）

///域名：邻居上下级标志  数据类型：整数
const int TAG_NBR_FLAG = 101 ; //内部TAG,邻居建立时的上下级关系
    const int ENM_NF_UP = 0 ; //上级
    const int ENM_NF_DOWN = 1 ; //下级
    const int ENM_NF_UNKNOWN = 2 ; //未知

///域名：邻居是否需要OSPF代理  数据类型：整数
const int TAG_NBR_PROXY = 102 ; //内部TAG,标识此邻居是否需要OSPF代理, 默认为0
    const int ENM_NP_YES = 0 ; //不需要
    const int ENM_NP_NO = 1 ; //需要

///域名：邻居变化  数据类型：整数
const int TAG_CHANGE_FLAG = 103 ; //内部TAG,邻居变化方式建立或者注销
    const int ENM_CF_CONNECT = 0 ; //建立
    const int ENM_CF_DISCONNECT = 1 ; //断开

///域名：消息来源  数据类型：整数
const int TAG_FROM = 104 ; //内部TAG
    const int ENM_FROM_DOWN = 0 ; //来自下级
    const int ENM_FROM_UP = 1 ; //来自上级

///域名：入列时间  数据类型：整数
const int TAG_PUSH_QUEUE_TIME = 105 ; //入列时间，用以计算PROC中队列的阻塞时间，单位是毫秒

///域名：连接号  数据类型：整数
const int TAG_CONNECT_ID = 106 ; //内部TAG

///域名：内部表明F1邻居是有名客户端  数据类型：整数
const int TAG_F1_FAMOUS_CLIENT = 107 ; //表明F1邻居是有名客户端

///域名：内部返回路由项的统计信息  数据类型：整数
const int TAG_ROUTERITEM_STAT = 127 ; //内部TAG,返回路由项的统计信息

#endif

