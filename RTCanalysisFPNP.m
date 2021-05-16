
function test()

clc;
clear all;
rtc_init;

%% Response time analysis -- non-preemptive case

%% period, deadline, wcet and prioriy
T = [40, 30, 3, 1;
     40, 30, 8, 2;
     50, 49, 12, 3;
     50, 30, 10, 4;
     ];
 
number_of_tasks = length(T(:,1));
for task = 1:number_of_tasks
  blocking_period(task) = 0;
  for i = 1:number_of_tasks
      if (i ~= task) && (T(i,4)>T(task,4)) && blocking_period(task) < T(i,3)
        blocking_period(task) = T(i,3);
%         blocking_period(task) = 0;
      end             
  end  
end

% blocking_period
 
 
a1 = rtcpjd(T(1,1), 0, T(1,2));
a1_r = rtctimes(a1, T(1,3));
a1 = a1_r;
a2 = rtcpjd(T(2,1), 0, T(2,2));
a2_r = rtctimes(a2, T(2,3));
a2 = a2_r;
a3 = rtcpjd(T(3,1), 0, T(3,2));
a3_r = rtctimes(a3, T(3,3));
a3 = a3_r;
a4 = rtcpjd(T(4,1), 0, T(4,2));
a4_r = rtctimes(a4, T(4,3));
a4 = a4_r;
b = rtcfs(1);


[a_1 b_1 del_1 buf_1] = rtcgpc(a1, b);
del_1 = del_1 + blocking_period(1);

a_hp_2 = a1;
[a_2 b_2 del_2] = rtcFPNP(a_hp_2, a2, b, T(2,3), blocking_period(2));

a_hp_3 = rtcplus(a_hp_2, a2);
[a_3 b_3 del_3] = rtcFPNP(a_hp_3, a3, b, T(3,3), blocking_period(3));

a_hp_4 = rtcplus(a_hp_3, a3);
[a_4 b_4 del_4] = rtcFPNP(a_hp_4, a4, b, T(4,3), blocking_period(4));

R = [del_1 del_2 del_3 del_4]


function [a_out_prio2 b_ethernet_rem2 del_prio2_FPNP buf_prio2] = rtcFPNP(sum_hp_rb_i, a_in_r, beta_rb, e_i, E_lp_max)

gamma_u_i = rtcmax(rtcmaxdeconv(rtcminus(beta_rb(1), sum_hp_rb_i(2)),0),0);
gamma_l_i = rtcmaxconv(rtcminus(beta_rb(2), sum_hp_rb_i(1)),0);

beta_u_i_rb = rtcmin(beta_rb(1), rtcplus(gamma_u_i, e_i));
beta_l_i_rb = rtcmax(0, rtcminus(gamma_l_i, E_lp_max));

beta_r = [beta_u_i_rb beta_l_i_rb];
beta_r_approx = rtcapprox(beta_r, 0,0); 

[a_out_prio2 b_ethernet_rem2 del_prio2_FPNP buf_prio2] = rtcgpc(a_in_r, beta_r_approx);


