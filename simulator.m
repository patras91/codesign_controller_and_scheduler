function [p0, p1, p2] = simulator(K, b, n, delayInput)
%disp('K');
%k1 = K;
%disp(K);
%disp('b');
%disp(b);
%disp('S');
%disp(s);
%disp('C');
%disp(C);
%disp(n);
%global n;
%global P D Delay1 Delay2 Delay3;
%delay = 0;
%delay = computeDelay(s,C,n);
%delay
%discard = 0;
%for i=1:1:n
%  if (delay(i) == Inf)
 %   discard = 1;
 % end
%end

%if(discard == 1) 
%for i =1:1:n
%p0(i)=0;
%p1(i)=0;
%p2(i)=0;
%end
%else
for i=1:1:n
    %if(i== 1) 
    fname = strcat('simulator',int2str(i));
    fh = str2func(fname);
    [p0(i), p1(i), p2(i)] = fh(K(i), b(i));
    
    %else
     %   p0(i) = 8;
      %  p1(i) = 9;
       % p2(i) = 10;
   % end
end
%end