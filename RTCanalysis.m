
clc;
clear all;
rtc_init;

%% Response time analysis -- preemptive case

%% period, deadline, wcet and prioriy
T = [40, 30, 3, 1;
     40, 30, 8, 2;
     50, 49, 12, 3;
     50, 30, 10, 4;
     ];
 
 
a1 = rtcpjd(T(1,1), 0, T(1,2));
a2 = rtcpjd(T(2,1), 0, T(2,2));
a3 = rtcpjd(T(3,1), 0, T(3,2));
a4 = rtcpjd(T(4,1), 0, T(4,2));
b = rtcfs(1);


[a_1 b_1 del_1 buf_1] = rtcgpc(a1, b, T(1,3));
[a_2 b_2 del_2 buf_2] = rtcgpc(a2, b_1, T(2,3));
[a_3 b_3 del_3 buf_3] = rtcgpc(a3, b_2, T(3,3));
[a_4 b_4 del_4 buf_4] = rtcgpc(a4, b_3, T(4,3));

R = [del_1 del_2 del_3 del_4]