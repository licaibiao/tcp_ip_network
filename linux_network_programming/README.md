# linux_network_programming
《linux 网络编程》中的协议实例以及我自己做的一小部分修改


get_server_and_client_information
----------------------------
获取服务端和客户端的信息，包括内容：

001_check_order.c  
002_turn_order_htonl_ntohl.c
003_inet_aton_addr_lnaof_netof.c     
004_inet_pton_ntop.c              
005_check_is_socket_type.c
006_get_host_by_name_addr.c  
007_get_peer_name.c     
008_get_protocol_name.c

get_set_ifconfig_information
----------------------------
通过ioctl获取网卡的信息，并且通过ioctl设置网卡信息
get_ifconfig_infor.c
set_ifconfig_infor.c

socket_opt_ioctl_fcntl  
---------------------
socket的ioctl和文件控制
00_socketopt_type
01_socketopt_show
02_socketopt_keepalive
03_socketopt_bufsize
04_socket_ioctl

udp_read_write_data
-----------------
udp的读写操作，包括udp操作中常见问题的解决

001_udp_mtu_packet_lost    
002_udp_rw_buffer    
003_udp_connect    
004_udp_none_window    
005_udp_Multiple_network_card    
006_udp_packet_truncation    
udp_client_01    
udp_client_02   

