# linux_network_programming
《linux 网络编程》中的协议实例以及我自己做的一小部分修改


get_server_and_client_information
----------------------------
获取服务端和客户端的信息，包括内容：

check_order.c  
turn_order_htonl_ntohl.c    
inet_aton_addr_lnaof_netof.c       
inet_pton_ntop.c               
check_is_socket_type.c    
get_host_by_name_addr.c     
get_peer_name.c      
get_protocol_name.c  

get_set_ifconfig_information
----------------------------
通过ioctl获取网卡的信息，并且通过ioctl设置网卡信息    

get_ifconfig_infor.c      
set_ifconfig_infor.c    



socket_opt_ioctl_fcntl  
---------------------
socket的ioctl和文件控制

socketopt_type           
socketopt_show     
socketopt_keepalive     
socketopt_bufsize      
socket_ioctl    


udp_read_write_data
-----------------
udp的读写操作，包括udp操作中常见问题的解决

udp_mtu_packet_lost    
udp_rw_buffer    
udp_connect    
udp_none_window    
udp_Multiple_network_card    
udp_packet_truncation    
client_01    
client_02 

read_write_readv_recv_recvmsg_select_poll
-------------------------
linux的IO操作和IO复用功能的实现
包括文件的读写，发送接收，IO复用，阻塞和非阻塞功能的实现
read_write  
readv_writev  
recv_send  
recvmsg_sendmsg  
select  
poll  
non_block    

