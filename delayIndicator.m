function [discard, delay1, delay2, delay3] = delayIndicator(s)
%delay = 0;
%global Delay11 Delay12 Delay13 Delay21 Delay22  Delay31 Delay32 delay;
global delay;
C = sum(s);
n = 3;
delay = computeDelay(s,C,n);
%Delay11 = delay(1,1);
%Delay12 = delay(1,2);
%Delay13 = delay(1,3);
%Delay21 = delay(2,1);
%Delay22 = delay(2,2);
%Delay31 = delay(3,1);
%Delay32 = delay(3,2);

discard = 0;
for i=1:1:n
    for j=1:1:2
    if (delay(i,j) == Inf)
        discard = 1;  
    end
    end
end
if (delay(1,3) == Inf)
    discard = 1; 
end

delay1 = delay(1,:);
delay2 = delay(2,:);
delay3 = delay(3,:);
end