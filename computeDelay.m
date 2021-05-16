function [delay] = computeDelay(s,C,n) 
%s = [1 1 1];
%C = 3;

%disp('Computing Delay');
S1 = rtcpjd(20, 0, 1);
S2 = rtcpjd(30, 6, 1);
S3 = rtcpjd(40, 2, 1);
UI = rtcpjd(20, 2, 1);

P = rtcfs(1);

R1 = rtctdma(s(1), C, 1);

R2u = rtctdmau(s(2), C, 1);
R2l = rtctdmal(s(2), C, 1);
R2u1 = rtcaffine(R2u, 1, s(1));
R2l1 = rtcaffine(R2l, 1, s(1));
R2 = [R2u1; R2l1];

R3u = rtctdmau(s(3), C, 1);
R3l = rtctdmal(s(3), C, 1);
R3u1 = rtcaffine(R3u, 1, s(1)+s(2));
R3l1 = rtcaffine(R3l, 1, s(1)+s(2));
R3 = [R3u1; R3l1];

ed_T1 = 2;
ed_T2 = 2;
ed_T3 = 3;
ed_T4 = 3;
ed_T5 = 1;
ed_T6 = 2;
ed_T7 = 4;
ed_T8 = 1;
ed_T9 = 2;
ed_T10 = 3;
ed_T11 = 6;

ed_m1 = 4;
ed_m2 = 2;
ed_m3 = 2;
ed_m5 = 3;
ed_m6 = 2;
ed_m8 = 2;
ed_m10 = 2;

%Controller 1 delay
[m1 B31 del_T1 bufm1] = rtcgpc(S1, P, ed_T1);
[m2 B11 del_T2 bufm2] = rtcgpc(UI, P, ed_T2);

[m1_out R11 del_m1 bufm1_out] = rtcgpc(m1, R1, ed_m1);
[m2_out R12 del_m2 bufm2_out] = rtcgpc(m2, R11, ed_m2);

T3_in = rtcmax(m1_out, m2_out);
[m3 B21 del_T3 bufm3] = rtcgpc(T3_in, P, ed_T3);

[m3_out R13 del_m3 bufm3_out] = rtcgpc(m3, R12, ed_m3);

[a1 B41 del_T4 bufa1] = rtcgpc(m3_out, P, ed_T4);

%delay(1) = max( del_T1 + del_m1, del_T2 + del_m2) + del_T3 + del_m3 + del_T4;
delay(1,1) = del_T1 + del_m1;
delay(1,2) = del_T2 + del_m2;
delay(1,3) =  del_T3 + del_m3 + del_T4;

%Controller 2 delay
[m5 B32 del_T5 bufm5] = rtcgpc(S2, B31, ed_T5);

[m5_out R21 del_m5 bufm5_out] = rtcgpc(m5, R2, ed_m5);

[m6 B22 del_T6 bufm6] = rtcgpc(m5_out, B21, ed_T6);

[m6_out R22 del_m6 bufm6_out] = rtcgpc(m6, R21, ed_m6);

[a2 B42 del_T7 bufa2] = rtcgpc(m6_out, B41, ed_T7);

%delay(2) = del_T5 + del_m5 + del_T6 + del_m6 + del_T7;
delay(2,1) = del_T5 + del_m5 ;
delay(2,2) = del_T6 + del_m6 + del_T7;

%Controller 3 delay
[m8 B33 del_T8 bufm8] = rtcgpc(S3, B32, ed_T8);

[m8_out R31 del_m8 bufm8_out] = rtcgpc(m8, R3, ed_m8);

[m9 B23 del_T9 bufm9] = rtcgpc(m8_out, B22, ed_T9);
[m10 B24 del_T10 bufm10] = rtcgpc(m9, B23, ed_T10);

[m10_out R32 del_m10 bufm10_out] = rtcgpc(m10, R31, ed_m10);

[a3 B43 del_T11 bufa3] = rtcgpc(m10_out, B42, ed_T11);

%delay(3) = del_T8 + del_m8 + del_T9 + del_T10 + del_m10 + del_T11;
delay(3,1) = del_T8 + del_m8 ;
delay(3,2) = del_T9 + del_T10 + del_m10 + del_T11;

for i =1:1:n
    for j=1:1:2
    delay(i,j) = delay(i,j)/1000;
    end
end

delay(1,3) = delay(1,3)/1000;
%delay
%[m1 del_T1 buf1 P_out1] = rtcgpc(S1, P, ed_T1);
%[m1 del_T1 buf1 m5 del_T5 buf2 m8 del_T8 buf3 P_out] = rtcedf(S1, ed_T1, 4, S2, ed_T5, 8, S3, ed_T8, 12, P); 


%for i=1:1:n
 %   delay(i) = i;
%end